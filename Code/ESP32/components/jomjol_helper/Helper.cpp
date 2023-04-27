#include "Helper.h"
#include <sys/types.h>
#include <sys/stat.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <dirent.h>
#ifdef __cplusplus
}
#endif

#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
#include "driver/sdmmc_defs.h"

#include "driver/gpio.h"

#include <stdio.h>
#include <string.h>
#include "esp_console.h"
#include "esp_vfs_dev.h"
#include "driver/uart.h"



#include <string.h>
#include <esp_log.h>

static const char *TAG = "main";

#define PATH_MAX_STRING_SIZE 256

using namespace std;

bool Init_SDCard_GPIO(bool _onebitmode)
{
    ESP_LOGI(TAG, "Initializing SD card\n");
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();

    if (_onebitmode)
    {
        host.flags = SDMMC_HOST_FLAG_1BIT;
        printf("SD-Card using 1-line connection mode. Not compatible with all SD-Cards.\n");
    }
    else  
    {
        host.flags = SDMMC_HOST_FLAG_4BIT;
        printf("SD-Card using 4-line connection mode. GPIO12/13 is used by SD-Card.\n");
    }
  
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    esp_vfs_fat_sdmmc_mount_config_t mount_config = { };
    mount_config.format_if_mount_failed = false;
    mount_config.max_files = 5;

    sdmmc_card_t* card;
    esp_err_t ret;
    ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. If you want the card to be formatted, set format_if_mount_failed = true.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%d). Make sure SD card lines have pull-up resistors in place.", ret);
        }
        return false;
    }
    sdmmc_card_print_info(stdout, card);
	return true;
}

bool CheckPSRAM()
{
	std::string _zws;
	multi_heap_info_t aMultiHead_info ;
	heap_caps_get_info (&aMultiHead_info, MALLOC_CAP_SPIRAM);
    size_t gesamt_externalram = aMultiHead_info.total_free_bytes + aMultiHead_info.total_allocated_bytes;

    if (gesamt_externalram < 2000100)
    {
        _zws = "Kein PSRAM verfügbar. Freier Speicher nur: " + std::to_string(gesamt_externalram) + "\n";
        _zws = _zws + "Es können nur sehr kleine Modelle berechnet werden.\n";
        _zws = _zws + "Entweder verfügt das Board über keinen PSRAM oder er wurde in der Konfiguration nicht aktiviert.\n";
        printf(_zws.c_str());
		return false;
	}
    if (gesamt_externalram < 4000000)
    {
        _zws = "Nur 2MByte PSRAM verfügbar. Freier Speicher: " + std::to_string(gesamt_externalram) + "\n";
        _zws = _zws + "Es können nur sehr mittelgroße Modelle berechnet werden.\n";
        _zws = _zws + "Es gibt Boards mit 2 oder 4 MBytpe verfügbaren PSRAM.\n";
        printf(_zws.c_str());
		return true;
	}

	_zws = "PSRAM initialized - free memory available: "  + std::to_string(gesamt_externalram) + "\n";
    printf(_zws.c_str());
	return true;
}


void EnableConsoleInputMode()
{
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    ESP_ERROR_CHECK(uart_driver_install(CONFIG_ESP_CONSOLE_UART_NUM, 256, 0, 0, NULL, 0));
    esp_vfs_dev_uart_use_driver(CONFIG_ESP_CONSOLE_UART_NUM);
    esp_vfs_dev_uart_port_set_rx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CR);
    esp_vfs_dev_uart_port_set_tx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CRLF);
}



string getFileType(string filename)
{
	int lastpos = filename.find(".", 0);
	int neu_pos;
	while ((neu_pos = filename.find(".", lastpos + 1)) > -1)
	{
		lastpos = neu_pos;
	}

	string zw = filename.substr(lastpos + 1, filename.size() - lastpos);
	zw = toUpper(zw);

	return zw;
}

string toUpper(string in)
{
	for (int i = 0; i < in.length(); ++i)
		in[i] = toupper(in[i]);
	
	return in;
}
