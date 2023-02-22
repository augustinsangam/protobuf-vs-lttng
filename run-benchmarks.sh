#!/usr/bin/env bash
set -e

build_dir_protobuf_server=/tmp/build_dir_protobuf_server
mkdir -p "$build_dir_protobuf_server"
cmake -B "$build_dir_protobuf_server" -S . \
  -D CMAKE_BUILD_TYPE=Release \
  -D TRACING_ENABLED=OFF
cmake --build "$build_dir_protobuf_server" --target all --

echo "###############################################"
echo "# lttng"
echo "###############################################"
lttng create
lttng enable-channel --userspace userspace-channel --subbuf-size=134217728
lttng enable-event --channel=userspace-channel -u 'protobuf_vs_lttng:*'
lttng start
time "$build_dir_protobuf_server/main_lttng"
lttng stop
lttng destroy

echo "###############################################"
echo "# protobuf"
echo "###############################################"
time "$build_dir_protobuf_server/main_protobuf"
