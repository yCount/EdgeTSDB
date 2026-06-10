#ifndef ETSDB_INTERNAL_H
#define ETSDB_INTERNAL_H

#include "etsdb.h"

#include <stdint.h>

#define ETSDB_FILE_MAGIC "ETSDB001"
#define ETSDB_PAGE_MAGIC "ETPAGE01"

enum {
    ETSDB_FILE_MAGIC_SIZE = 8,
    ETSDB_PAGE_MAGIC_SIZE = 8,
    ETSDB_FORMAT_MAJOR = 1,
    ETSDB_FORMAT_MINOR = 0,
    ETSDB_FILE_HEADER_SIZE = 128,
    ETSDB_PAGE_HEADER_SIZE = 64
};

enum {
    ETSDB_FILE_HDR_MAGIC = 0,
    ETSDB_FILE_HDR_FORMAT_MAJOR = 8,
    ETSDB_FILE_HDR_FORMAT_MINOR = 10,
    ETSDB_FILE_HDR_HEADER_SIZE = 12,
    ETSDB_FILE_HDR_PAGE_TARGET_SAMPLES = 16,
    ETSDB_FILE_HDR_MAX_PAGE_BYTES = 20,
    ETSDB_FILE_HDR_INDEX_GRANULARITY_PAGES = 24,
    ETSDB_FILE_HDR_CREATED_UNIX_NS = 32,
    ETSDB_FILE_HDR_FLAGS = 40,
    ETSDB_FILE_HDR_RESERVED = 48,
    ETSDB_FILE_HDR_RESERVED_SIZE = 80
};

enum {
    ETSDB_PAGE_HDR_MAGIC = 0,
    ETSDB_PAGE_HDR_VERSION_MAJOR = 8,
    ETSDB_PAGE_HDR_VERSION_MINOR = 10,
    ETSDB_PAGE_HDR_HEADER_SIZE = 12,
    ETSDB_PAGE_HDR_FLAGS = 14,
    ETSDB_PAGE_HDR_FIRST_TIMESTAMP = 16,
    ETSDB_PAGE_HDR_LAST_TIMESTAMP = 24,
    ETSDB_PAGE_HDR_FIRST_VALUE_BITS = 32,
    ETSDB_PAGE_HDR_SAMPLE_COUNT = 40,
    ETSDB_PAGE_HDR_TIMESTAMP_STREAM_BYTES = 44,
    ETSDB_PAGE_HDR_VALUE_STREAM_BYTES = 48,
    ETSDB_PAGE_HDR_RESERVED32 = 52,
    ETSDB_PAGE_HDR_RESERVED64 = 56
};

typedef struct etsdb_index_entry {
    int64_t first_timestamp;
    uint64_t page_offset;
    uint64_t sample_ordinal;
} etsdb_index_entry_t;

typedef struct etsdb_page_meta {
    uint64_t offset;
    int64_t first_timestamp;
    int64_t last_timestamp;
    uint64_t first_value_bits;
    uint32_t sample_count;
    uint32_t timestamp_stream_bytes;
    uint32_t value_stream_bytes;
} etsdb_page_meta_t;

#endif

