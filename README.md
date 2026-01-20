# st-paths (s-t path counting, TdZdd)

## Build
Assumes TdZdd is placed under `./tdzdd/include`. If it is elsewhere, set `INCLUDES`.

```sh
make
```

Example:

```sh
make INCLUDES=-I/path/to/tdzdd/include
```

## Run
```sh
./path_count grid11x11.grh
```

The output is only the total number of s-t paths.

## Output all s-t paths (for small graphs)
```sh
./path_count grid4x4.grh --all
```

Outputs one path per line with vertex numbers separated by spaces.
