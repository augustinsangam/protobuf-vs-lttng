#undef LTTNG_UST_TRACEPOINT_PROVIDER
#define LTTNG_UST_TRACEPOINT_PROVIDER protobuf_vs_lttng

#undef LTTNG_UST_TRACEPOINT_INCLUDE
#define LTTNG_UST_TRACEPOINT_INCLUDE "./lttng_tracepoints.h"

#if !defined(_LTTNG_TRACEPOINTS_H) || \
    defined(LTTNG_UST_TRACEPOINT_HEADER_MULTI_READ)
#define _LTTNG_TRACEPOINTS_H

#include <lttng/tracepoint.h>

LTTNG_UST_TRACEPOINT_EVENT(
    /* Tracepoint provider name */
    protobuf_vs_lttng,
    /* Tracepoint class name */
    trace_data,
    /* Input arguments */
    LTTNG_UST_TP_ARGS(const uint8_t *, trace_data, unsigned int,
            trace_data_size),
    /* Output event fields */
    LTTNG_UST_TP_FIELDS(lttng_ust_field_sequence(uint8_t, trace_data,
                                                 trace_data, unsigned int,
                                                 trace_data_size)))

LTTNG_UST_TRACEPOINT_EVENT(
    /* Tracepoint provider name */
    protobuf_vs_lttng,
    /* Tracepoint class name */
    start_event,
    /* Input arguments */
    LTTNG_UST_TP_ARGS(),
    /* Output event fields */
    LTTNG_UST_TP_FIELDS())

LTTNG_UST_TRACEPOINT_EVENT(
    /* Tracepoint provider name */
    protobuf_vs_lttng,
    /* Tracepoint class name */
    end_event,
    /* Input arguments */
    LTTNG_UST_TP_ARGS(
            int64_t, duration_ns
    ),
    /* Output event fields */
    LTTNG_UST_TP_FIELDS(
            lttng_ust_field_integer(int64_t, duration_ns, duration_ns)
    )
)

#endif /* _LTTNG_TRACEPOINTS_H */

#include <lttng/tracepoint-event.h>
