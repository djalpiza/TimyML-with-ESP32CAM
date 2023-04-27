// @imports
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include <string.h>
#include "sdkconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "Helper.h"
#include "CImageBasis.h"
#include "CRotateImage.h"
#include "CImageCut.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize.h"

#include "CTfLiteClass.h"
#include "Helper.h"
#include "connect_wlan.h"
#include "server_main.h"

#include "esp_camera.h"

static const char *TAG = "ESP32CAM-CNN";

////////////////////////////////////////////////////////////////////////////////////////////////////
/////////// 0) Configuration and Settings //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
    // WiFi settings
    const char* WLAN_ssid     = "FRITZ!Box 7590 BC";
    const char* WLAN_password = "51325457781445939271";
    //const char* WLAN_ssid     = "Dereck";
    //const char* WLAN_password = "Galleta123";
    //const char* WLAN_ssid     = "nisha";
    //const char* WLAN_password = "12345678";
    //const char* WLAN_ssid     = "SSID";
    //const char* WLAN_password = "PASSWORD";
    

    // Image settings Image size = QVGA (320x240x3 colors)
    int Camera_Image_Size_x = 320;
    int Camera_Image_Size_y = 240;
    int Camera_Channels = 3;

    // ROI settings
    // The ROI defines the section of the camera image that is initially cut out
    // and then rescaled to the input size of the neural network.
    bool ROI_rotate = false;             // true: Rotate the ROI by 90°
    int ROI_x = 20;
    int ROI_y = 20;
    int ROI_dx = 136;
    int ROI_dy = 205;

    // File names for saving the images on the SD card
    std::string image_name_input  = "/sdcard/original.jpg";
    std::string image_name_ROI    = "/sdcard/roi.jpg";
    std::string image_name_resize = "/sdcard/resize.bmp";

    // Settings for the neural network
    // std::string cnn_tflite = "/sdcard/model.tflite";
    std::string cnn_tflite = "/sdcard/model.ico";
    int cnn_input_x = 20;
    int cnn_input_y = 32;


    // Camera configuration (GPIO allocation, timer, image format/size)
    static camera_config_t camera_config = {
        .pin_pwdn = 32,
        .pin_reset = -1,
        .pin_xclk = 0,
        .pin_sscb_sda = 26,
        .pin_sscb_scl = 27,
        .pin_d7 = 35,
        .pin_d6 = 34,
        .pin_d5 = 39,
        .pin_d4 = 36,
        .pin_d3 = 21,
        .pin_d2 = 19,
        .pin_d1 = 18,
        .pin_d0 = 5,
        .pin_vsync = 25,
        .pin_href = 23,
        .pin_pclk = 22,

        .xclk_freq_hz = 20000000,
        .ledc_timer = LEDC_TIMER_0,
        .ledc_channel = LEDC_CHANNEL_0,

        .pixel_format = PIXFORMAT_JPEG, // ATTENTION: JPEG required
        .frame_size = FRAMESIZE_QVGA,   // ATTENTION: when changing, the image size must also be adjusted above! 
        .jpeg_quality = 5, 
        .fb_count = 1,      
        .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
    };
/////////// End Config ////////////////////////////////////////////////////////////////////////////


extern "C" void app_main()
{
    printf("----- START LOOP -----\n");
    // Initialize hardware
    if (!Init_SDCard_GPIO())
        return;
    printf("Initialized SD Card\n");
    CheckPSRAM();
    Init_NVS();

    ESP_LOGI(TAG, "Reading file");
    FILE* f = fopen("/sdcard/model.ico", "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return;
    }
    else{
        printf("Opened the file\n");
    }

    if (esp_camera_init(&camera_config) != ESP_OK)
    {
        ESP_LOGE(TAG, "Camera Init Failed");
        return;
    }
    printf("\n");

    // Connect to WiFi and start the http server
    wifi_init_sta(WLAN_ssid, WLAN_password);

    printf("Start Server ...\n");
    server =  start_webserver();
    register_server_main_uri(server, "/sdcard");


    // Load and initialize the neural network
    printf("Load and initialize neural network ... \n");
    CTfLiteClass* neuralnetwork = new CTfLiteClass;
    if (!neuralnetwork->LoadModelFromFile(cnn_tflite)) 
        return;   // dynamic
    SetResult("Not-a-Number");

    // Endless loop with image acquisition, processing, neural calculation
    while (1)
    {
        printf("IP-Adresse: %s\n", GetIPAdresse().c_str());
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////// 1) Take picture from camera ////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        CImageCut ImageCamera(Camera_Image_Size_x, Camera_Image_Size_y, Camera_Channels);

        ESP_LOGI(TAG, "Taking picture...");
        camera_fb_t *fb = esp_camera_fb_get();
        ImageCamera.LoadFromMemory(fb->buf, fb->len);
        esp_camera_fb_return(fb);

        ImageCamera.drawRect(ROI_x-1, ROI_y-1, ROI_dx+2, ROI_dy+2, 255, 0, 0);
        UnLockFile(true);
        ImageCamera.SaveToFile("/sdcard/original.jpg");
        UnLockFile(false);

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////// 2) Change ROI image size and save //////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        CImageBasis ImageROI(ROI_dx, ROI_dy, Camera_Channels);

        ESP_LOGI(TAG, "Cut ROI and save to file...");
        ImageCamera.CutAndSave(ROI_x, ROI_y, ROI_dx, ROI_dy, &ImageROI);
        UnLockFile(true);
        ImageROI.SaveToFile(image_name_ROI);
        UnLockFile(false);

        ESP_LOGI(TAG, "Rotate and Resize ROI ...");
        CRotateImage ImageInputCNN(&ImageROI);
        if (ROI_rotate)
            ImageInputCNN.Rotate(90);
        ImageInputCNN.Resize(cnn_input_x, cnn_input_y);

        UnLockFile(true);
        ImageInputCNN.SaveToFile(image_name_resize);
        UnLockFile(false);


        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /////////// Send reduced image through TFlite //////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        int result;

        printf("Loading image data...\n");
        UnLockFile(true);
        neuralnetwork->LoadInputImage(image_name_resize.c_str());
        UnLockFile(false);
        neuralnetwork->Invoke();
        result = neuralnetwork->GetOutClassification();
    
        printf("  CNN classification: %d\n", (int) result);
        if (result == 10)
            SetResult("Not-a-Number");
        else
            SetResult(to_string(result));


        TickType_t xDelay = 100 / portTICK_PERIOD_MS;
        printf("main: sleep for : %ldms\n", (long) xDelay*10);
        vTaskDelay( xDelay ); 
    }

    delete neuralnetwork;       // Free up memory
}
