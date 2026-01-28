# Quick Start Guide

## Prerequisites

1. **Clang++ with C++14 support**

   ```bash
   clang++ --version  # Should show version 3.4+ for C++14
   ```

2. **Catch2 library**
   - Install via package manager or download header-only version
   - Place `catch.hpp` in `tools/include/catch2/` directory

## Setup

1. **Initialize the project**

   ```bash
   make setup
   ```

2. **Build all curriculum modules**

   ```bash
   make all
   ```

3. **List available content**
   ```bash
   make list-days    # See all daily lessons
   make list-weeks   # See all weeks
   ```

## Daily Workflow

### Run Today's Lesson

```bash
make run-day DAY=week01/day01_auto_decltype
```

### Benchmark Performance

```bash
make benchmark-day DAY=week01/day01_auto_decltype
```

### Run Entire Week

```bash
make run-week WEEK=week01
```

## Build Modes

- `make debug` - Debug build with sanitizers
- `make release` - Optimized build (default)
- `make benchmark` - Benchmark-optimized build
- `make analysis` - Analysis build with debug symbols

## Example Session

```bash
# Day 1: Auto and Decltype
make run-day DAY=week01/day01_auto_decltype
make benchmark-day DAY=week01/day01_auto_decltype
make notes DAY=week01/day01_auto_decltype

# Day 2: Lambda Expressions
make run-day DAY=week01/day02_lambda_expressions
make benchmark-day DAY=week01/day02_lambda_expressions

# Day 3: Move Semantics
make run-day DAY=week01/day03_move_semantics
make benchmark-day DAY=week01/day03_move_semantics
```

## Troubleshooting

### Catch2 Not Found

Download the single-header version:

```bash
mkdir -p tools/include/catch2
curl -o tools/include/catch2/catch.hpp https://raw.githubusercontent.com/catchorg/Catch2/v2.13.10/single_include/catch2/catch.hpp
```

### Compilation Errors

- Ensure C++14 support: `clang++ -std=c++14 -dM -E - < /dev/null | grep __cplusplus`
- Check include paths: `make debug` for detailed error messages

### Performance Issues

- Use `make benchmark` for optimized builds
- Run benchmarks multiple times for consistent results
- Consider system load when benchmarking

## Next Steps

1. Complete Week 1 (Days 1-7) - C++14 Language Features
2. Move to Week 2 (Days 8-14) - Standard Library Deep Dive
3. Continue through the 12-week curriculum
4. Build projects in the `projects/` directory
5. Document insights in `notes/` directory
