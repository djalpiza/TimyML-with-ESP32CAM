idf_component_register(SRCS ${CMAKE_SOURCE_DIR}/main/main.cpp
                    INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include
                    REQUIRES  esp32-camera-master jomjol_tfliteclass jomjol_helper log nvs_flash jomjol_image jomjol_wlan jomjol_server)

                
