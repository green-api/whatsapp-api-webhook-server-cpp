user_adapter_cpp="source/user_adapter.cpp"
user_adapter_template_cpp="source/_user_adapter.cpp"

if [ ! -f "$user_adapter_cpp" ]; then
    echo "Could not find '$user_adapter_cpp' file. It is required for your functions."
    echo "If you already have this file, please copy it to '$user_adapter_cpp'"
    echo "If you don't have this file, please use a template, copy:"
    echo "- '$user_adapter_template_cpp' to '$user_adapter_cpp'"
    exit 1
fi

if [ -f "$user_adapter_cpp" ]; then
    echo "User Adapter found."
fi

mkdir build

git clone --branch=main --depth=1 https://github.com/pocoproject/poco lib/poco
git clone --branch=v3.11.3 --depth=1 https://github.com/nlohmann/json lib/nlohmann_json
git clone --branch=main --depth=1 https://github.com/pboettch/json-schema-validator lib/nlohmann_json_schema_validator

cd lib/poco
mkdir build
cd build

cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_XML:BOOL="0" -DENABLE_JSON:BOOL="0" -DENABLE_NETSSL:BOOL="0" -DENABLE_CRYPTO:BOOL="0" -DENABLE_JWT:BOOL="0" -DENABLE_DATA:BOOL="0" -DENABLE_DATA_SQLITE:BOOL="0" -DENABLE_DATA_MYSQL:BOOL="0" -DENABLE_DATA_POSTGRESQL:BOOL="0" -DENABLE_DATA_ODBC:BOOL="0" -DENABLE_MONGODB:BOOL="0" -DENABLE_REDIS:BOOL="0" -DENABLE_PDF:BOOL="0" -DENABLE_ZIP:BOOL="0" -DENABLE_SEVENZIP:BOOL="0" -DENABLE_APACHECONNECTOR:BOOL="0" -DENABLE_CPPPARSER:BOOL="0" -DENABLE_ENCODINGS:BOOL="0" -DENABLE_ENCODINGS_COMPILER:BOOL="0" -DENABLE_PAGECOMPILER:BOOL="0" -DENABLE_PAGECOMPILER_FILE2PAGE:BOOL="0" -DENABLE_POCODOC:BOOL="0" -DENABLE_TESTS:BOOL="0" -DENABLE_SAMPLES:BOOL="0" -DENABLE_LONG_RUNNING_TESTS:BOOL="0" ..
cd ./../..

cd nlohmann_json
cmake -DCMAKE_CXX_STANDARD=17 -DCMAKE_BUILD_TYPE=Release -DJSON_BuildTests:BOOL="0" .
cd ../../

cd build
cmake -DCMAKE_BUILD_TYPE=Release -DJSON_BuildTests:BOOL="0" -DENABLE_XML:BOOL="0" -DENABLE_JSON:BOOL="0" -DENABLE_NETSSL:BOOL="0" -DENABLE_CRYPTO:BOOL="0" -DENABLE_JWT:BOOL="0" -DENABLE_DATA:BOOL="0" -DENABLE_DATA_SQLITE:BOOL="0" -DENABLE_DATA_MYSQL:BOOL="0" -DENABLE_DATA_POSTGRESQL:BOOL="0" -DENABLE_DATA_ODBC:BOOL="0" -DENABLE_MONGODB:BOOL="0" -DENABLE_REDIS:BOOL="0" -DENABLE_PDF:BOOL="0" -DENABLE_ZIP:BOOL="0" -DENABLE_SEVENZIP:BOOL="0" -DENABLE_APACHECONNECTOR:BOOL="0" -DENABLE_CPPPARSER:BOOL="0" -DENABLE_ENCODINGS:BOOL="0" -DENABLE_ENCODINGS_COMPILER:BOOL="0" -DENABLE_PAGECOMPILER:BOOL="0" -DENABLE_PAGECOMPILER_FILE2PAGE:BOOL="0" -DENABLE_POCODOC:BOOL="0" -DENABLE_TESTS:BOOL="0" -DENABLE_SAMPLES:BOOL="0" -DENABLE_LONG_RUNNING_TESTS:BOOL="0" ..
cmake --build .

cd ..

cp -r jsonSchema build/bin/
cp config.json build/bin/
rm build/bin/poco-arc
