#include "CTfLiteClass.h"
#include "stb_image.h"

#include <sys/stat.h>

bool debugdetailtflite = false;

float CTfLiteClass::GetOutputValue(int nr)
{
		TfLiteTensor* output2 = this->interpreter->output(0);

		int numeroutput = output2->dims->data[1];
		if ((nr+1) > numeroutput)
			return -1000;

		return output2->data.f[nr];
}


int CTfLiteClass::GetOutClassification()
{
	float zw_max = 0;
	float zw;
	int zw_class = -1;

	TfLiteTensor* output2 = interpreter->output(0);
	if (output2 == NULL)
		return -1;

	int numeroutput = output2->dims->data[1];

	for (int i = 0; i < numeroutput; ++i)
	{
		zw = output2->data.f[i];
		if (zw > zw_max)
		{
				zw_max = zw;
				zw_class = i;
		}
	}
	return zw_class;
}

void CTfLiteClass::GetInputDimension(bool silent)
{
	TfLiteTensor* input2 = this->interpreter->input(0);

	int numdim = input2->dims->size;


	if (!silent) printf("Anzahl Eingangsdimensionen: %d\n", numdim - 1);  

	int sizeofdim;
	for (int j = 1; j < numdim; ++j)
	{
		sizeofdim = input2->dims->data[j];
		if (!silent) printf("  Größe Dimension %d: %d\n", j, sizeofdim);  
		if (j == 1) im_height = sizeofdim;
		if (j == 2) im_width = sizeofdim;
		if (j == 3) im_channel = sizeofdim;
	}
}

int CTfLiteClass::GetOutputDimension(bool silent)
{
	TfLiteTensor* output2 = this->interpreter->output(0);

	int numdim = output2->dims->size;

	if (!silent)
	{
		printf("Anzahl Ausgangsdimensionen: %d\n", numdim - 1);  

		int sizeofdim;
		for (int j = 1; j < numdim; ++j)
		{
			sizeofdim = output2->dims->data[j];
			printf("  Größe Dimension %d: %d\n", j, sizeofdim);  
		}
	}

	if (numdim > 0)
		return output2->dims->data[numdim-1];

	return 0;
}


void CTfLiteClass::Invoke()
{
	interpreter->Invoke();
}


bool CTfLiteClass::LoadInputImage(std::string _fn)
{
	int width, height, bpp; 
	int channels = 3;
	uint8_t* rgb_image;
	stbi_uc* p_source;
	unsigned char red, green, blue;
	
	rgb_image = stbi_load(_fn.c_str(), &width, &height, &bpp, channels);

	if (!rgb_image)
	{
		printf("\n!!! ERROR - Bilddatei konnte nicht geladen werden (%s). !!!\n\n", _fn.c_str());
		return false;
	}

	if ((width != im_width) || (height != im_height)) 
	{
		printf("Bildgröße passt nicht zum neuronalen Netz !!!\n");
		printf(" Image    : %d x %d (%s)\n", width, height, _fn.c_str());
		printf(" CNN-Größe: %d x %d\n", im_width, im_height);
		return false;
	}

	input_i = 0;
	float* input_data_ptr = (interpreter->input(0))->data.f;

	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
			{
					p_source = rgb_image + (channels * (y * width + x));
					red = p_source[0];
					green = p_source[1];
					blue = p_source[2];
					*(input_data_ptr) = (float) red;
					input_data_ptr++;
					*(input_data_ptr) = (float) green;
					input_data_ptr++;
					*(input_data_ptr) = (float) blue;
					input_data_ptr++;
			}

	return true;
}


void CTfLiteClass::MakeAllocate()
{
	/*
	This is where the various operators (the neural layers, e.g. Conv2d, MaxPooling, Full, ...) are loaded.
	You can load every single operator you need here. This saves storage space. Here will be
	general operator "AllOpsResolver" is loaded, covering all available operators.
	*/
	static tflite::AllOpsResolver resolver;
	interpreter = new tflite::MicroInterpreter(model, resolver, tensor_arena, kTensorArenaSize, error_reporter);

	TfLiteStatus allocate_status = interpreter->AllocateTensors();
	if (allocate_status != kTfLiteOk) {
				TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
		GetInputDimension();   
		return;
	}

}

long CTfLiteClass::GetFileSize(std::string filename)
{
		struct stat stat_buf;
		printf("\n------ Stat ------\n(%s)\n", filename.c_str());
		long rc = stat(filename.c_str(), &stat_buf);
		return rc == 0 ? stat_buf.st_size : -1;
}


unsigned char* CTfLiteClass::ReadFileToCharArray(std::string _fn)
{
		long size;
		
		size = GetFileSize(_fn.c_str());
		printf("\n Size: (%ld)\n",size);
		
		printf("\n------ Test ------\n(%s)\n", _fn.c_str());

		if (size == -1)
		{
		printf("\nFile does not exist.\n");
				return NULL;
		}


	unsigned char *result = (unsigned char*) malloc(size);
	
	if(result != NULL) {
//		printf("\nMemory is reserved:: %ld\n", size);
				FILE* f = fopen(_fn.c_str(), "rb");     // vorher  nur "r"
				fread(result, 1, size, f);
				fclose(f);        
	}else {
		printf("\nNo free memory available.\n");
	}    
	return result;
}

bool CTfLiteClass::LoadModelFromFile(std::string _fn)
{
	printf("\n****** Model file try is: (%s). ******\n", _fn.c_str());
	LoadedModelDescription_tflite = ReadFileToCharArray(_fn.c_str());
	if (!LoadedModelDescription_tflite)
	{
		printf("\nModel file could not be loaded (%s).\n", _fn.c_str());
		return false;
	}
	printf("\nModel DONE!\n");
	return LoadModelFromCharArray(LoadedModelDescription_tflite);
}

bool CTfLiteClass::LoadModelFromCharArray(unsigned char *_input){
	static tflite::MicroErrorReporter micro_error_reporter;
	error_reporter = &micro_error_reporter;

	model = tflite::GetModel(_input);
	TFLITE_MINIMAL_CHECK(model != nullptr); 		

	if (!model)
	{
		printf("!\n");
		return false;
	}

	MakeAllocate(); 
	GetInputDimension(true);
	return true;
}



CTfLiteClass::CTfLiteClass(int _memsize)
{
	model = nullptr;
	interpreter = nullptr;
	input = nullptr;
	output = nullptr;  
	kTensorArenaSize = _memsize;
	tensor_arena = (uint8_t*) malloc(kTensorArenaSize);
	if (!tensor_arena)
		printf("CTfLiteClass - Memory (kTensorArenaSize) could not be allocated\n");
}

CTfLiteClass::~CTfLiteClass()
{
	delete this->tensor_arena;
	delete this->interpreter;
	delete this->error_reporter;
	if (LoadedModelDescription_tflite)
		free(LoadedModelDescription_tflite);
}        
