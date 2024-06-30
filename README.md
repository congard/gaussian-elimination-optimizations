# GaussianElimination - Optimizations

This repository contains several low-level optimizations of the Gaussian Elimination algorithm.

Made as a part of Code Optimizations on Different Architectures course
at the AGH University of Science and Technology.

## Optimizations

Optimizations are located in the `src` directory and named `ge#.c`, where `#` - optimization number.

### ge1

Basic Gaussian Elimination implementation.

### ge2

Counters are moved to registers.

### ge3

Multiplier, i.e.

```c
multiplier = A[i][k] / A[k][k]
```

is moved to register and extracted from the most inner loop.

### ge4

The third loop is unrolled by 8.

### ge5

Matrix representation was changed to 1D array.

### ge6

SSE instructions are used.

### ge7

AVX-256 instructions are used.

### ge8

AVX-512 instructions are used.

## Dependencies & Requirements

The project requires **PAPI** to be installed (tested with PAPI 7.0.1.0).
PAPI provides a programmer interface to monitor the performance of running programs.

On Fedora, PAPI can be installed by executing the following command:

```bash
sudo dnf install papi-devel
```

> [!IMPORTANT]
> In order to run the prepared benchmarks, your CPU must at least support the following counters:
> 
> | Counter        | Description                                  |
> |----------------|----------------------------------------------|
> | `PAPI_FP_OPS`  | Floating point operations                    |
> | `PAPI_TOT_INS` | Instructions completed                       |
> | `PAPI_TOT_CYC` | Total cycles                                 |
> | `PAPI_BR_TKN`  | Conditional branch instructions taken        |
> | `PAPI_BR_MSP`  | Conditional branch instructions mispredicted |
> 
> Supported counters can be checked by executing
> ```bash
> papi_avail | grep Yes
> ```
> 
> Moreover, in order to run `ge6`, `ge7` and `ge8` optimizations,
> your CPU must support SSE, AVX-256 and AVX-512 accordingly.
> It can be checked by executing `lscpu` command.

## Bibliografy

1. Materiały do laboratorium 2 – dr hab. inż. Maciej Woźniak
2. Materiały do laboratorium 3 – dr hab. inż. Maciej Woźniak
3. Materiały do laboratorium 4 – dr hab. inż. Maciej Woźniak
4. How To Optimize Gemm – flame: [https://github.com/flame/how-to-optimize-gemm](https://github.com/flame/how-to-optimize-gemm)
