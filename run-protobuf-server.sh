#!/usr/bin/env bash
set -e

build_dir_protobuf_server=/tmp/build_dir_protobuf_server
mkdir -p "$build_dir_protobuf_server"
cmake -B "$build_dir_protobuf_server" -S . \
  -D CMAKE_BUILD_TYPE=Release \
  -D TRACING_ENABLED=OFF
cmake --build "$build_dir_protobuf_server" --target main_protobuf_server --

"$build_dir_protobuf_server/main_protobuf_server"
