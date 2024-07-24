cmake_minimum_required(VERSION 3.5)
project(whatsapp-api-webhook-server-cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:DebugDLL>$<$<CONFIG:Release>:DLL>")
endif()

set(SOURCES
    source/webhook.cpp
    source/main.cpp
    source/validator.cpp
    source/user_adapter.cpp
    source/config.cpp
    source/logger.cpp
)

if(MSVC OR MSYS OR MINGW)
    set(Poco_DIR "${CMAKE_SOURCE_DIR}/lib/poco")
	find_package(Poco REQUIRED Util Net)
else()
	add_subdirectory("${CMAKE_SOURCE_DIR}/lib/poco/")
endif()

include_directories(include)

add_executable(whatsapp-api-webhook-server-cpp ${SOURCES})

set_target_properties(whatsapp-api-webhook-server-cpp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build
)

set(nlohmann_json_DIR "${CMAKE_SOURCE_DIR}/lib/nlohmann_json/")
find_package(nlohmann_json CONFIG REQUIRED)

add_subdirectory("${CMAKE_SOURCE_DIR}/lib/nlohmann_json_schema_validator")


target_link_libraries(whatsapp-api-webhook-server-cpp
    Poco::Util
    Poco::Net
    nlohmann_json::nlohmann_json
    nlohmann_json_schema_validator
)
