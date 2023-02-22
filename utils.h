#ifndef PROTOBUF_VS_LTTNG_UTILS_H
#define PROTOBUF_VS_LTTNG_UTILS_H

#include "trace_service.grpc.pb.h"

#define BINARY_TRACE_DATA_SIZE 400
#define EXPORT_BATCH_SIZE 512
#define EXPORT_N_BATCH 1300
//#define EXPORT_N_BATCH 100

timespec timespec_diff(timespec start, timespec end);

void populate_request(ExportTraceServiceRequest &request);

#endif //PROTOBUF_VS_LTTNG_UTILS_H
