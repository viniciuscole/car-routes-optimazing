# Car routes optimazing

## What it does

Finds the fastest route between two nodes on a directed road graph whose
speeds change at known instants. It runs Dijkstra with a binary-heap
priority queue, and re-plans from the current node whenever an update's
instant passes.

## Input format

```
N;E                      nodes and edges
src;dst
speed                    km/h, one value for every road at instant 0
from;to;metres           × E   (directed edges)
instant;from;to;kmh      × any number of updates, sorted by instant
```

Edge weight is travel time in seconds: `metres / (km/h ÷ 3.6)`.

## Build and run

```
make
./trab2 input.txt output.txt
```

`output.txt` gets three lines: the path (node ids separated by `;`), the
distance in km, and the time as `hh:mm:s.ffffff` — hours and minutes are
zero-padded, but seconds are not (e.g. `00:03:0.000000`).

## Trace mode

```
make trace
./trace < input.txt
```

Reads the same input from stdin and prints one event per line: `graph N
E`, `edge FROM TO METERS KMH`, `replan NODE CLOCK`, `pop NODE TIME`,
`relax FROM TO TIME`, `plan N1 ... Nk ETA` (`ETA` is `unreachable` when
there is no path), `move FROM TO T0 T1`, `update FROM TO KMH CLOCK`,
`unreachable NODE CLOCK`, `done CLOCK KM`. `pop`/`relax`/`plan` times are
relative to the node in the preceding `replan`; `move`/`update`/`done` use
the absolute clock. First lines of `test/with_update.trace`:

```
graph 6 9
edge 1 2 1000 60.000
edge 1 3 1500 60.000
edge 2 4 1000 60.000
edge 2 5 2500 60.000
edge 3 4 800 60.000
edge 3 5 3000 60.000
edge 4 6 1000 60.000
edge 4 5 600 60.000
edge 5 6 500 60.000
replan 1 0.000
pop 1 0.000
```

## Tests

```
test/run.sh
```

Compiles `trab2` and `trace` under AddressSanitizer/UBSan and compares six
scenarios byte-exact against the expected output. Needs a gcc that
supports `-fsanitize=address,undefined`.

## Authors

- Vinicius Cole ([viniciuscole](https://github.com/viniciuscole))
- João ([vortex2jm](https://github.com/vortex2jm))
- Gabriel Gatti ([gabrielgatti7](https://github.com/gabrielgatti7))

*Técnicas de Busca e Ordenação, UFES, 2023*

## See it running

https://viniciuscole.dev/projects/car-routes/ compiles this code to
WebAssembly and animates the trace step by step.

## Patterns
 - Functions must be in snake_case
 - Types must be in UpperCamelCase
 - Variables must be in snake_case
 - All code will be written in English

## Remarks
 - All header files must be into *include* folder
 - All ".c" files must be into *src* folder
