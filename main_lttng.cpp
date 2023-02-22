#include "lttng_tracepoints.h"
#include "utils.h"

#include <iostream>

#define N_EXPERIMENTS 10

int main() {
    srand(0); // NOLINT

    // Create the trace data
    ExportTraceServiceRequest request;
    populate_request(request);

    timespec start_cpu{}, end_cpu{};
    timespec start_real{}, end_real{};
    for (int64_t i = 0; i < N_EXPERIMENTS; i++) {
        clock_gettime(CLOCK_MONOTONIC_RAW, &start_real);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_cpu);
        for(auto j = 0; j < EXPORT_N_BATCH; j++) {
            for (auto k = 0; k < request.trace_data_size(); k++) {
                std::string serialized = request.trace_data(k).SerializeAsString();
                lttng_ust_tracepoint(protobuf_vs_lttng, trace_data, (uint8_t *) serialized.data(), serialized.size());
            }
        }
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_cpu);
        clock_gettime(CLOCK_MONOTONIC_RAW, &end_real);
        timespec cpu_duration = timespec_diff(start_cpu, end_cpu);
        timespec real_duration = timespec_diff(start_real, end_real);
        std::cout << "Run no " << i << " cpu_time     " << 1000000000LL * cpu_duration.tv_sec + cpu_duration.tv_nsec
                  << std::endl;
        std::cout << "Run no " << i << " elapsed_time " << 1000000000LL * real_duration.tv_sec + real_duration.tv_nsec
                  << std::endl;
    }

    return 0;
}
