FILE(GLOB_RECURSE app_sources ${CMAKE_CURRENT_SOURCE_DIR}/*.*)

idf_component_register(SRCS ${app_sources}
                    INCLUDE_DIRS "." "./include"
                    REQUIRES esp_http_server esp_http_client nvs_flash spiffs jomjol_helper)


