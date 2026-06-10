#include "etsdb.h"
#include "etsdb_internal.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

static int count_sample(const etsdb_sample_t *sample, void *user_data)
{
    uint64_t *count = (uint64_t *)user_data;

    assert(sample != NULL);
    assert(count != NULL);
    *count += 1u;
    return 0;
}

int main(void)
{
    _Static_assert(ETSDB_FILE_HEADER_SIZE == 128, "file header size");
    _Static_assert(ETSDB_PAGE_HEADER_SIZE == 64, "page header size");
    _Static_assert(ETSDB_FILE_MAGIC_SIZE == 8, "file magic size");
    _Static_assert(ETSDB_PAGE_MAGIC_SIZE == 8, "page magic size");
    _Static_assert(ETSDB_FILE_HDR_RESERVED + ETSDB_FILE_HDR_RESERVED_SIZE ==
                       ETSDB_FILE_HEADER_SIZE,
                   "file reserved span");
    _Static_assert(ETSDB_PAGE_HDR_RESERVED64 + sizeof(uint64_t) ==
                       ETSDB_PAGE_HEADER_SIZE,
                   "page header span");

    etsdb_config_t config = {
        .mode = ETSDB_OPEN_CREATE_OR_OPEN,
        .page_target_samples = ETSDB_DEFAULT_PAGE_TARGET_SAMPLES,
        .max_page_bytes = ETSDB_DEFAULT_MAX_PAGE_BYTES,
        .index_granularity_pages = ETSDB_DEFAULT_INDEX_GRANULARITY_PAGES,
    };
    etsdb_sample_t sample = {
        .timestamp = 123456789,
        .value = 42.25,
    };
    etsdb_stats_t stats = {0};
    etsdb_index_entry_t index_entry = {
        .first_timestamp = sample.timestamp,
        .page_offset = ETSDB_FILE_HEADER_SIZE,
        .sample_ordinal = 0,
    };
    etsdb_page_meta_t page = {
        .offset = index_entry.page_offset,
        .first_timestamp = sample.timestamp,
        .last_timestamp = sample.timestamp,
        .first_value_bits = 0,
        .sample_count = 1,
        .timestamp_stream_bytes = 0,
        .value_stream_bytes = 0,
    };
    uint64_t callback_count = 0;
    etsdb_sample_callback_t callback = count_sample;

    assert(ETSDB_OK == 0);
    assert(config.page_target_samples > 0u);
    assert(config.max_page_bytes >= ETSDB_PAGE_HEADER_SIZE);
    assert(index_entry.first_timestamp == page.first_timestamp);
    assert(stats.sample_count == 0u);
    assert(callback(&sample, &callback_count) == 0);
    assert(callback_count == 1u);

    return 0;
}

