cmake_minimum_required(VERSION 3.1)

project(protobuf-vs-lttng)

find_package(LTTngUST REQUIRED)

# Find Protobuf installation
# Looks for protobuf-config.cmake file installed by Protobuf's cmake installation.
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
if (CMAKE_CROSSCOMPILING)
    find_program(_PROTOBUF_PROTOC protoc)
else ()
    set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif ()

# Find gRPC installation
# Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP gRPC::grpc++)
if (CMAKE_CROSSCOMPILING)
    find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else ()
    set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif ()

# Proto file
get_filename_component(td_proto "./protos/trace_service.proto" ABSOLUTE)
get_filename_component(td_proto_path "${td_proto}" PATH)

# Generated sources
set(td_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/trace_service.pb.cc")
set(td_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/trace_service.pb.h")
set(td_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/trace_service.grpc.pb.cc")
set(td_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/trace_service.grpc.pb.h")
add_custom_command(
        OUTPUT "${td_proto_srcs}" "${td_proto_hdrs}" "${td_grpc_srcs}" "${td_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${td_proto_path}"
        --plugin "protoc-gen-grpc=${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${td_proto}"
        DEPENDS "${td_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# td_grpc_proto
add_library(td_grpc_proto
        ${td_grpc_srcs}
        ${td_grpc_hdrs}
        ${td_proto_srcs}
        ${td_proto_hdrs})
target_link_libraries(td_grpc_proto
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF})

# target = main_lttng
add_executable(main_lttng)
target_link_libraries(main_lttng PRIVATE
        dl
        lttng-ust
        td_grpc_proto
        )
target_include_directories(main_lttng PRIVATE .)
target_sources(main_lttng PRIVATE
        lttng_tracepoints.cpp
        main_lttng.cpp
        utils.cpp
        )

# target = main_protobuf
add_executable(main_protobuf)
target_link_libraries(main_protobuf PRIVATE
        td_grpc_proto
        )
#target_include_directories(main_protobuf PRIVATE .)
target_sources(main_protobuf PRIVATE
        main_protobuf.cpp
        utils.cpp
        )

# target = main_protobuf_server
add_executable(main_protobuf_server)
target_link_libraries(main_protobuf_server PRIVATE
        td_grpc_proto
        )
#target_include_directories(main_protobuf_server PRIVATE .)
target_sources(main_protobuf_server PRIVATE
        main_protobuf_server.cpp
        )