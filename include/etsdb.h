#ifndef ETSDB_H
#define ETSDB_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ETSDB_DEFAULT_PAGE_TARGET_SAMPLES 1024u
#define ETSDB_DEFAULT_MAX_PAGE_BYTES (64u * 1024u)
#define ETSDB_DEFAULT_INDEX_GRANULARITY_PAGES 1u

typedef struct etsdb etsdb_t;

/* Stable status values returned by every public API function. */
typedef enum etsdb_status {
    ETSDB_OK = 0,
    ETSDB_ERR_INVALID_ARGUMENT,
    ETSDB_ERR_INVALID_CONFIG,
    ETSDB_ERR_IO,
    ETSDB_ERR_NO_MEMORY,
    ETSDB_ERR_CORRUPT_FILE,
    ETSDB_ERR_UNSUPPORTED_FORMAT,
    ETSDB_ERR_NON_MONOTONIC_TIMESTAMP,
    ETSDB_ERR_CALLBACK_ABORTED
} etsdb_status_t;

/*
 * READ_ONLY opens an existing file for queries.
 * READ_WRITE opens an existing file for appends and queries.
 * CREATE replaces any existing file with a new database.
 * CREATE_OR_OPEN opens an existing file or creates one if it is missing.
 */
typedef enum etsdb_open_mode {
    ETSDB_OPEN_READ_ONLY = 0,
    ETSDB_OPEN_READ_WRITE = 1,
    ETSDB_OPEN_CREATE = 2,
    ETSDB_OPEN_CREATE_OR_OPEN = 3
} etsdb_open_mode_t;

/* Writer configuration persisted in the file header for later inspection. */
typedef struct etsdb_config {
    etsdb_open_mode_t mode;
    uint32_t page_target_samples;
    uint32_t max_page_bytes;
    uint64_t index_granularity_pages;
} etsdb_config_t;

/* One logical telemetry sample. Values round-trip as exact double bits. */
typedef struct etsdb_sample {
    int64_t timestamp;
    double value;
} etsdb_sample_t;

/* Aggregated metadata collected from the file header and page scan. */
typedef struct etsdb_stats {
    uint64_t sample_count;
    uint64_t page_count;
    uint64_t index_entry_count;
    uint64_t file_size_bytes;
    uint64_t compressed_bytes;
    uint64_t uncompressed_sample_bytes;
    int64_t first_timestamp;
    int64_t last_timestamp;
    int has_samples;
    uint32_t page_target_samples;
    uint32_t max_page_bytes;
    uint64_t index_granularity_pages;
} etsdb_stats_t;

/* Return 0 to continue a query; return nonzero to abort the scan. */
typedef int (*etsdb_sample_callback_t)(const etsdb_sample_t *sample,
                                       void *user_data);

/* Initialize config with CREATE_OR_OPEN and conservative page defaults. */
void etsdb_config_init(etsdb_config_t *config);

const char *etsdb_status_string(etsdb_status_t status);

/* Open or create a database. Passing NULL config uses etsdb_config_init. */
etsdb_status_t etsdb_open(const char *path, const etsdb_config_t *config,
                          etsdb_t **out_db);

/* Close flushes any active page before releasing resources. */
etsdb_status_t etsdb_close(etsdb_t *db);

/* Force the active in-memory page to be appended to the file. */
etsdb_status_t etsdb_flush(etsdb_t *db);

/* Append timestamps in strictly increasing order. */
etsdb_status_t etsdb_append(etsdb_t *db, int64_t timestamp, double value);

etsdb_status_t etsdb_append_sample(etsdb_t *db,
                                   const etsdb_sample_t *sample);

/* Stream samples in [start_timestamp, end_timestamp] through callback. */
etsdb_status_t etsdb_query_range(etsdb_t *db, int64_t start_timestamp,
                                 int64_t end_timestamp,
                                 etsdb_sample_callback_t callback,
                                 void *user_data);

/* Copy current file/index statistics into out_stats. */
etsdb_status_t etsdb_get_stats(etsdb_t *db, etsdb_stats_t *out_stats);

#ifdef __cplusplus
}
#endif

#endif

