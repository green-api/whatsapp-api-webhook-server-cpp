@echo off
set "user_adapter_cpp=source\user_adapter.cpp"
set "user_adapter_h=include\user_adapter.h"
set "user_adapter_template_cpp=source\_user_adapter.cpp"
set "user_adapter_template_h=include\_user_adapter.h"

if not exist "%user_adapter_cpp%" (
    goto createUserAdapter
)

if not exist "%user_adapter_h%" (
    goto createUserAdapter
)

goto aftercreateUserAdapter

:createUserAdapter
    echo Could not find "%user_adapter_cpp%" and "%user_adapter_h%" files. They are required for your functions.
    echo If you already have these files, please enter "N" to cancel build and copy them manually.
    echo If you don't have these files, please enter "Y" to create them.
    set /p userinput=Create source\user_adapter.cpp and include\user_adapter.h files? [Y/N]:
    if /i "%userinput%"=="y" (
        if exist "%user_adapter_template_cpp%" copy "%user_adapter_template_cpp%" "%user_adapter_cpp%"
        if exist "%user_adapter_template_h%" copy "%user_adapter_template_h%" "%user_adapter_h%"
    ) else (
        echo Exiting script.
        exit /b
    )

:aftercreateUserAdapter
if exist "%user_adapter_cpp%" if exist "%user_adapter_h%" (
    echo User Adapter found.
)

mkdir build

git clone --branch=main --depth=1 https://github.com/pocoproject/poco lib/poco
git clone --branch=v3.11.3 --depth=1 https://github.com/nlohmann/json lib/nlohmann_json
git clone --branch=main --depth=1 https://github.com/pboettch/json-schema-validator lib/nlohmann_json_schema_validator

cd lib\poco
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_XML:BOOL="0" -DENABLE_JSON:BOOL="0" -DENABLE_NETSSL:BOOL="0" -DENABLE_CRYPTO:BOOL="0" -DENABLE_JWT:BOOL="0" -DENABLE_DATA:BOOL="0" -DENABLE_DATA_SQLITE:BOOL="0" -DENABLE_DATA_MYSQL:BOOL="0" -DENABLE_DATA_POSTGRESQL:BOOL="0" -DENABLE_DATA_ODBC:BOOL="0" -DENABLE_MONGODB:BOOL="0" -DENABLE_REDIS:BOOL="0" -DENABLE_PDF:BOOL="0" -DENABLE_ZIP:BOOL="0" -DENABLE_SEVENZIP:BOOL="0" -DENABLE_APACHECONNECTOR:BOOL="0" -DENABLE_CPPPARSER:BOOL="0" -DENABLE_ENCODINGS:BOOL="0" -DENABLE_ENCODINGS_COMPILER:BOOL="0" -DENABLE_PAGECOMPILER:BOOL="0" -DENABLE_PAGECOMPILER_FILE2PAGE:BOOL="0" -DENABLE_POCODOC:BOOL="0" -DENABLE_TESTS:BOOL="0" -DENABLE_SAMPLES:BOOL="0" -DENABLE_LONG_RUNNING_TESTS:BOOL="0" ..
cmake --build . --config Debug
cd ..\..

cd nlohmann_json
mkdir build
cd build
cmake -DCMAKE_CXX_STANDARD=17 -DJSON_BuildTests=OFF -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug
cd ..\..

cd nlohmann_json_schema_validator
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug
cd ..\..\..

mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_XML:BOOL="0" -DENABLE_JSON:BOOL="0" -DENABLE_NETSSL:BOOL="0" -DENABLE_CRYPTO:BOOL="0" -DENABLE_JWT:BOOL="0" -DENABLE_DATA:BOOL="0" -DENABLE_DATA_SQLITE:BOOL="0" -DENABLE_DATA_MYSQL:BOOL="0" -DENABLE_DATA_POSTGRESQL:BOOL="0" -DENABLE_DATA_ODBC:BOOL="0" -DENABLE_MONGODB:BOOL="0" -DENABLE_REDIS:BOOL="0" -DENABLE_PDF:BOOL="0" -DENABLE_ZIP:BOOL="0" -DENABLE_SEVENZIP:BOOL="0" -DENABLE_APACHECONNECTOR:BOOL="0" -DENABLE_CPPPARSER:BOOL="0" -DENABLE_ENCODINGS:BOOL="0" -DENABLE_ENCODINGS_COMPILER:BOOL="0" -DENABLE_PAGECOMPILER:BOOL="0" -DENABLE_PAGECOMPILER_FILE2PAGE:BOOL="0" -DENABLE_POCODOC:BOOL="0" -DENABLE_TESTS:BOOL="0" -DENABLE_SAMPLES:BOOL="0" -DENABLE_LONG_RUNNING_TESTS:BOOL="0" ..
cmake --build . --config Debug

cd ..
if not exist "build\build\Debug" mkdir "build\Debug"
xcopy /e /i /y "jsonSchema" "build\Debug\jsonSchema"
copy /y "config.json" "build\Debug"
copy /y "lib\poco\build\bin\PocoNetd.dll" "build\Debug"
copy /y "lib\poco\build\bin\PocoFoundationd.dll" "build\Debug"