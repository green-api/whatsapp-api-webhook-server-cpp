FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y \
    git \
    cmake \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

COPY ./examples/ /app/examples
COPY ./include/ /app/include
COPY ./jsonSchema/ /app/jsonSchema
COPY ./source/ /app/source
COPY ./build.sh /app/
COPY ./CMakeLists.txt /app/
COPY ./config.json /app/


WORKDIR /app

RUN sed -i 's/\r$//' build.sh
RUN bash build.sh

ENTRYPOINT ["./build/bin/whatsapp-api-webhook-server-cpp"]