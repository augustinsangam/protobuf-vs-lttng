#include "utils.h"

timespec timespec_diff(timespec start, timespec end) {
    timespec temp{};
    if ((end.tv_nsec - start.tv_nsec) < 0) {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp;
}

void populate_request(ExportTraceServiceRequest &request) {
    uint8_t trace_bytes[BINARY_TRACE_DATA_SIZE];
    for (int64_t i = 0; i < EXPORT_BATCH_SIZE; ++i) {
        auto trace = request.mutable_trace_data()->Add();
        // Create the trace data
        for (uint8_t &byte: trace_bytes) {
            byte = rand(); // NOLINT
        }
        trace->set_data(trace_bytes, BINARY_TRACE_DATA_SIZE);
    }
}
