# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(mqtt_client)

target_add_binary_data(mqtt_client.elf "main/client.crt" TEXT)
target_add_binary_data(mqtt_client.elf "main/client.key" TEXT)
