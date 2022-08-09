# TIMED
> A simple timer to measure **Wall and CPU Time**. Shipped with a basic benchmark framework.

## Timer
> The timer provides easy access to `std::chrono::steady_clock` (Wall Time) and `ctime` (CPU Time).

### Usage

The Syntax for both Wall Timer and CPU Timer is the same:

```c++
WallTimer wall_timer;
wall_timer.start();
// Do some operations
wall_timer.pause();  // optional: pause timing
// Do some operations that should not be considered by wall_timer
wall_timer.start()  // continue timing
wall_timer.stop()
std::cout << wall_timer.elapsedNanoseconds() << std::endl;
```
```c++
CPUTimer cpu_timer;
cpu_timer.start();
// Do some operations
cpu_timer.pause();  // optional: pause timing
// Do some operations that should not be considered by wall_timer
cpu_timer.start()  // continue timing
cpu_timer.stop()
std::cout << cpu_timer.elapsedNanoseconds() << std::endl;
```

## Benchmark

The `Benchmark` class provides a relatively powerful API for benchmarking a function. The class takes a
`std::function<void()>` which works as a wrapper for the operations that should be benchmarked.

Example:

```c++
int uselessCounter(unsigned i, unsigned j, unsigned k) {
  int counter = 0;
  for (i; i > 0; --i) {
    for (j; j > 0; --j) {
      for (k; k > 0; --k) {
        if (i % j == k) {
          counter++;
        }
      }
    }
  }
  return counter
}

int main(int argc, char** argv) {
  unsigned i = 100;
  unsigned j = 100;
  unsigned k = 100;
  std::function<void()> wrapper = [&]() {
    int count = uselessCounter(i, j, k);
  };
  Benchmark benchmark(wrapper);
  benchmark.run(true)  // run benchmark with verbosity
  std::cout << benchmark << std::endl;  // print benchmark results
}
```

# Build