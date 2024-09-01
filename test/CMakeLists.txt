cmake_minimum_required(VERSION 3.10)
project(MqttManager)

find_library(MOSQUITTO_LIB CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(MqttManager MqttManager.cpp)
target_link_libraries(MqttManager mosquitto nlohmann_json EziMOTIONPlusE)
