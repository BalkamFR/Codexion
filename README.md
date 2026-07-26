*This project has been created as part of the 42 curriculum by papilaz.*

# Codexion — Master the race for resources before the deadline masters you

## Description

Codexion is a multithreaded simulation inspired by the classic **Dining Philosophers Problem**, reframed in a software development context. Each **coder** (thread) needs two **dongles** (mutexes) to compile their code. If a coder goes too long without compiling, they **burn out** and the simulation ends.

The goal of the project is to implement fair resource arbitration using thread synchronization primitives, while preventing deadlocks, starvation, and data races.

### Parameters

```
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

- `number_of_coders` — number of coder threads
- `time_to_burnout` — max time (ms) a coder can go without compiling before burning out
- `time_to_compile` — time (ms) it takes to compile
- `time_to_debug` — time (ms) spent debugging after compiling
- `time_to_refactor` — time (ms) spent refactoring after debugging
- `number_of_compiles_required` — how many times each coder must compile before the simulation ends successfully
- `dongle_cooldown` — time (ms) a dongle must wait after being released before it can be taken again
- `scheduler` — `fifo` (first-in, first-out) or `edf` (earliest deadline first)

## Instructions

### Compilation

```bash
make        # Build the binary
make re     # Clean and rebuild
make clean  # Remove object files
make fclean # Remove object files and binary
```

### Execution

```bash
./codexion 5 800 200 100 100 5 20 fifo
./codexion 4 800 100 50 50 3 10 edf
```

### Expected log format

```
timestamp_in_ms coder_id has taken a dongle
timestamp_in_ms coder_id is compiling
timestamp_in_ms coder_id is debugging
timestamp_in_ms coder_id is refactoring
timestamp_in_ms coder_id burned out
```

## Blocking Cases Handled

### Deadlock Prevention (Coffman's Conditions)

The classic deadlock risk occurs when all coders hold one dongle and wait for the second — a **circular wait**. This is prevented by assigning dongles asymmetrically:
- **Even-indexed coders** lock `right_dongle` first, then `left_dongle`
- **Odd-indexed coders** lock `left_dongle` first, then `right_dongle`

This breaks the circular wait condition, one of Coffman's four necessary conditions for deadlock.

### Starvation Prevention

The **priority queue** (linked list ordered by scheduler policy) guarantees that every coder eventually gets access to dongles:
- **FIFO**: coders are served in arrival order — no coder waits forever
- **EDF** (Earliest Deadline First): coders closest to burning out are prioritized, preventing unfair starvation under pressure

### Dongle Cooldown

After a coder releases a dongle, the dongle cannot be taken again for `dongle_cooldown` milliseconds. This is tracked via `dongle_last_released[]` and checked in `is_blocked()`. Without this, a single coder could repeatedly grab the same dongle and starve its neighbors.

### Precise Burnout Detection

A dedicated **monitor thread** checks each coder's `last_compile_start` every millisecond (`usleep(1000)`). When `get_time() - last_compile_start > time_to_burnout`, the monitor immediately sets `stop_sim = 1`, prints the burnout message, and broadcasts to wake all waiting threads. This ensures burnout is logged within 10ms of the actual event.

### Log Serialization

All `printf` calls go through `print_status()`, which locks `write_mutex` before writing and unlocks after. This guarantees that no two log lines ever interleave on the same output.

## Thread Synchronization Mechanisms

### `pthread_mutex_t *dongles`

An array of mutexes, one per dongle. Each coder locks two adjacent dongles before compiling and unlocks them after. Locking order is asymmetric (even/odd) to prevent deadlock.

### `pthread_mutex_t write_mutex`

Protects all `printf` calls. Ensures log output is never interleaved between threads.

### `pthread_mutex_t queue_mutex`

Protects access to the shared priority queue. All push/pop operations and the `cond_wait` loop are done under this mutex.

### `pthread_mutex_t stop_mutex`

Protects reads and writes to `stop_sim`, `compile_count`, and `last_compile_start`. These variables are accessed by both coder threads and the monitor thread, making them susceptible to data races without protection.

**Example — how stop_sim is protected:**
```c
// Writer (monitor thread):
pthread_mutex_lock(&data->stop_mutex);
data->stop_sim = 1;
pthread_mutex_unlock(&data->stop_mutex);

// Reader (coder thread):
pthread_mutex_lock(&data->stop_mutex);
stopped = data->stop_sim;
pthread_mutex_unlock(&data->stop_mutex);
while (!stopped) { ... }
```

### `pthread_cond_t queue_cond`

A condition variable used to put coders to sleep while they wait in the queue. When a dongle becomes available (or the simulation stops), `pthread_cond_broadcast()` wakes all waiting coders so they can re-evaluate `is_blocked()`.

`broadcast` is used instead of `signal` because multiple coders may become unblocked at the same time (e.g., after a dongle is released).

## Resources

- [POSIX Threads Programming — Lawrence Livermore](https://hpc-tutorials.llnl.gov/posix/)
- [Dining Philosophers Problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [Earliest Deadline First Scheduling — Wikipedia](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)
- [pthread_cond_wait man page](https://man7.org/linux/man-pages/man3/pthread_cond_wait.3p.html)
