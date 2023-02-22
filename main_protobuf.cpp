#include "utils.h"

#include "trace_service.grpc.pb.h"

#include <grpcpp/grpcpp.h>

#include <iostream>

#define N_EXPERIMENTS 10

int main() {
    srand(0); // NOLINT

    auto stub = TraceService::NewStub(
            grpc::CreateChannel(
                    "132.207.72.28:50051",
                    grpc::InsecureChannelCredentials()));

    timespec start_cpu{}, end_cpu{};
    timespec start_real{}, end_real{};
    for (auto i = 0; i < N_EXPERIMENTS; i++) {
        clock_gettime(CLOCK_MONOTONIC_RAW, &start_real);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_cpu);
        for (auto j = 0; j < EXPORT_N_BATCH; j++) {
            ExportTraceServiceRequest request;
            populate_request(request);
            grpc::ClientContext context;
            ExportTraceServiceResponse response;
            grpc::Status status = stub->Export(&context, request, &response);
            if (!status.ok()) {
                std::cout << status.error_code() << ": " << status.error_message()
                          << std::endl;
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
