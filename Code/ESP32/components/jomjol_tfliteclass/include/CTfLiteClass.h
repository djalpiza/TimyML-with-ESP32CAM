
#define TFLITE_MINIMAL_CHECK(x)                              \
  if (!(x)) {                                                \
    fprintf(stderr, "Error at %s:%d\n", __FILE__, __LINE__); \
    exit(1);                                                 \
  }

#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/micro/kernels/micro_ops.h"



/**
 * Klasse, um die Ansteuerung der TFlite-Bibliothek von Tensorflow zu kapseln und über einfache Funktionen
 * neuronale Netze zu laden, sowie Bilder zu berechnen.
 */    
class CTfLiteClass
{
    protected:
        tflite::ErrorReporter *error_reporter;
        const tflite::Model* model;
        tflite::MicroInterpreter* interpreter;
        TfLiteTensor* output = nullptr;     
        static tflite::AllOpsResolver resolver;

        unsigned char *LoadedModelDescription_tflite = NULL;

        int kTensorArenaSize;
        uint8_t *tensor_arena;

        float* input;
        int input_i;
        int im_height = -1;
        int im_width = -1;
        int im_channel = -1;

        long GetFileSize(std::string filename);
        unsigned char* ReadFileToCharArray(std::string _fn);
        void MakeAllocate();

    public:
		/**
		 * Initialisierung der Klasse und Bereitstellung Speicherplatz.
		 *
		 * @param _memsize Speichergröße für interne Datenablage und Berechnungen von TfLite Micro.
		 *                 Defaultwert = 600k
		 */    
        CTfLiteClass(int _memsize = 200*1024);

		/**
		 * Deinitialiserung und Freigabe von Speicherplatz.
		 */    
        ~CTfLiteClass(); 

		/**
		 * Laden eines TFlite-Modelfiles von der Speicherkarte.
		 *
		 * @param _fn Dateiname des Files inklusive der Pfadangabe.
		 * @return true falls Laden erfolgreich, ansonsten false.
		 */    
        bool LoadModelFromFile(std::string _fn);

		/**
		 * Laden eines TFlite-Modelfiles aus einem Char-Array.
		 *
		 * @param _input Zeiger auf ein Char-Array mit der Binarkodierten TFLite-Beschreibung.
		 * @return true falls Laden erfolgreich, ansonsten false.
		 */    
        bool LoadModelFromCharArray(unsigned char *_input);

		/**
		 * Laden des Eingangsbild aus einer Datei.
		 *
		 * @param _fn Dateiname des Files inklusive der Pfadangabe.
		 * @return true:  Bild erfolgreich geladen 
		 * 		   false: Bild konnte nicht geladen werden oder die Bildgröße passt nicht zum neuronalen Netz
		 */    
        bool LoadInputImage(std::string _fn);

		/**
		 * Berechnung des neuronalen Netzes nachdem das TFlite-File und das Eingangsbild geladen wurde.
		 */    
        void Invoke();

		/**
		 * Klassifizierungsergebnis der zurückliegenden Berechnung.
		 * Die Berechnung muss zuvor über das Laden eines Eingangbildes
		 * und dem Aufruf von Invoke() durchgeführt worden sein.
		 *
		 * @return Ergebnisklasse als Zahl beginnend von 0 (=erste Klasse).
		 */    
        int GetOutClassification();

		/**
		 * Rückgabe der Aktivierung eines einzelnen Ausgabeneurons.
		 *
		 * @param nr Nummer des abgefragten Neurons (Zählung beginnt bei 0).
		 * @return Aktivierung des Neurons mit der Nummer "nr".
		 */    
        float GetOutputValue(int nr);

		/**
		 * Informationen zum Eingangslayer eines geladenen neuronalen Netzes.
		 * Informationen werden auch in internen Parameters gespeicher
		 *
		 * @param silent Schalter, um die Ausgabe auf die Konsole zu deaktivieren (Default = aktiv)
		 * 				 Falls Aktiv, werden die Netzparameter nur in den internen Parametern aktualisiert
		 * 				 ansonsten auch an die Konsole ausgegeben.
		 */    
        void GetInputDimension(bool silent = false);

		/**
		 * Informationen zum Ausgangslayer eines geladenen neuronalen Netzes.
		 * Informationen werden auch in internen Parameters gespeicher
		 *
		 * @param silent Schalter, um die Ausgabe auf die Konsole zu deaktivieren (Default = aktiv)
		 * 				 Falls Aktiv, werden die Netzparameter nur in den internen Parametern aktualisiert
		 * 				 ansonsten auch an die Konsole ausgegeben.
		 */    
        int GetOutputDimension(bool silent = false);

};

