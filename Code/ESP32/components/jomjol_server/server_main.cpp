#include "server_main.h"
#include <string>
#include "server_help.h"
#include "esp_wifi.h"


httpd_handle_t server = NULL;   

static const char *TAG_SERVERMAIN = "server-main";

std::string cnn_result = "";


bool fileislocked = false;

void UnLockFile(bool _lock)
{
    fileislocked = _lock;
}


void SetResult(std::string _input)
{
    cnn_result = _input;
}


esp_err_t hello_main_handler(httpd_req_t *req)
{
    char filepath[50];
    printf("uri: %s\n", req->uri);
    int _pos;
    esp_err_t res;

    char *base_path = (char*) req->user_ctx;
    std::string filetosend(base_path);

    const char *filename = get_path_from_uri(filepath, base_path,
                                             req->uri - 1, sizeof(filepath));    
    printf("1 uri: %s, filename: %s, filepath: %s\n", req->uri, filename, filepath);

    if (strcmp(req->uri, "/result") == 0)
    {
            httpd_resp_sendstr_chunk(req, cnn_result.c_str());
        	httpd_resp_send_chunk(req, NULL, 0);

        return ESP_OK;
    }


    if ((strcmp(req->uri, "/") == 0) || (strcmp(req->uri, "/index.html") == 0))
    {
        printf("Main page\n");
        httpd_resp_sendstr_chunk(req, "<!DOCTYPE html><html>");
        httpd_resp_sendstr_chunk(req, "<head><meta http-equiv=\"refresh\" content=\"1\"></head>");
        
        httpd_resp_sendstr_chunk(req, "<body><table>");
        if (cnn_result.length() == 0)
        {
            httpd_resp_sendstr_chunk(req, "<tr><td>No image taken and processed yet. <br> Please wait...</td></tr>");   
        }
        else
        {
            
            httpd_resp_sendstr_chunk(req, "<h1>Counter Detection with ESP32-CAM</h1>");
			httpd_resp_sendstr_chunk(req, "<p>Developed by Dereck Alpizar, Nisha Kumari & Neeranjan JK | WS22-23 Machine Learning | HS Emden-Leer</p>");
			httpd_resp_sendstr_chunk(req, "<tr><td>Original Picture:<br><img src = \"/original.jpg\"></td></tr>");
            httpd_resp_sendstr_chunk(req, "<tr><td>Region-of-Interest (ROI):<br><img src = \"/roi.jpg\"></td></tr>");
            httpd_resp_sendstr_chunk(req, "<tr><td>Neural Network Result is:<br><img src = \"/resize.bmp\"></td></tr>");
            httpd_resp_sendstr_chunk(req, "<tr><td><h2>");
            httpd_resp_sendstr_chunk(req, cnn_result.c_str());
            httpd_resp_sendstr_chunk(req, "</h2></td></tr>");   

        }
        httpd_resp_sendstr_chunk(req, "</table></body></html>");
    	httpd_resp_send_chunk(req, NULL, 0);

        return ESP_OK;
    }
    else
    {
        filetosend = filetosend + std::string(req->uri);
        _pos = filetosend.find("?");
        if (_pos > -1){
            filetosend = filetosend.substr(0, _pos);
        }
    }

    printf("File requested: %s\n", filetosend.c_str());    

    if (!filename) {
        ESP_LOGE(TAG_SERVERMAIN, "Filename is too long");
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Filename too long");
        return ESP_FAIL;
    }

    while (fileislocked)
    {
        TickType_t xDelay = 100 / portTICK_PERIOD_MS;
        printf("file is blocked: sleep for : %ldms\n", (long) xDelay*10);
        vTaskDelay( xDelay ); 
    }


    res = send_file(req, filetosend);
    httpd_resp_send_chunk(req, NULL, 0);
    if (res != ESP_OK)
        return res;

    httpd_resp_sendstr(req, "");
    httpd_resp_send_chunk(req, NULL, 0);

    return ESP_OK;
}


void register_server_main_uri(httpd_handle_t server, const char *base_path)
{
    httpd_uri_t main_rest_handle = {
        .uri       = "/*",  // Match all URIs
        .method    = HTTP_GET,
        .handler   = hello_main_handler,
        .user_ctx  = (void*) base_path
    };
    httpd_register_uri_handler(server, &main_rest_handle);
}



httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.uri_match_fn = httpd_uri_match_wildcard;

    // Start the httpd server
    ESP_LOGI(TAG_SERVERMAIN, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG_SERVERMAIN, "Registering URI handlers");
        return server;
    }

    ESP_LOGI(TAG_SERVERMAIN, "Error starting server!");
    return NULL;
}

void stop_webserver(httpd_handle_t server)
{
    httpd_stop(server);
}


void disconnect_handler(void* arg, esp_event_base_t event_base, 
                               int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server) {
        ESP_LOGI(TAG_SERVERMAIN, "Stopping webserver");
        stop_webserver(*server);
        *server = NULL;
    }
}

void connect_handler(void* arg, esp_event_base_t event_base, 
                            int32_t event_id, void* event_data)
{
    httpd_handle_t* server = (httpd_handle_t*) arg;
    if (*server == NULL) {
        ESP_LOGI(TAG_SERVERMAIN, "Starting webserver");
        *server = start_webserver();
    }
}
