# whatsapp-api-webhook-server-cpp

# Building the library

### Requirements:
- CMake 3.5 or higher, accessible by ```cmake``` command
- C++ 17 or higher
- git client, accessible by ```git``` command

### Clone library repository

```
git clone --branch=main --depth=1 https://github.com/green-api/whatsapp-api-webhook-server-cpp
```

### Windows

```
cd whatsapp-api-webhook-server-cpp
build_Debug.bat
start build\Debug\whatsapp-api-webhook-server-cpp.exe
```

### Linux

```
cd whatsapp-api-webhook-server-cpp
./build_Debug.sh
./build/build/whatsapp-api-webhook-server-cpp
```

This will download Poco, nlohmann_json and nlohmann_json_schema_validator libraries and build them with the project. Launch the project by latest command.

Project build will be placed in ```whatsapp-api-webhook-server-cpp/build/bin/whatsapp-api-webhook-server-cpp```.

After script execution you can build your project from ```whatsapp-api-webhook-server-cpp/build``` directory:

```
cmake -- build .
```

or the other way from ```whatsapp-api-webhook-server-cpp```:

```
cmake --build build
```

### Running

After starting ```whatsapp-api-webhook-server-cpp``` a server in ```config.json``` port (default: 5000) will be started.

