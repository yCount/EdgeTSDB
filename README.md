# EdgeTSDB
Embedded time-series storage engine with Gorilla-style compression.

## MVP Scope

- One `.etsdb` file containing one ordered time series.
- Append-only compressed page records.
- Gorilla-style timestamp and `double` compression.
- Sparse in-memory index rebuilt from page headers at open time.
- Range queries that seek to the nearest indexed page and decode forward.
- Small C API and a CLI.

## Build

```sh
make all
make test
make bench
```

