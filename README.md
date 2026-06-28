*This project has been created as part of the 42 curriculum by aunoguei.*

# Codexion

## Description

***Codexion*** is a concurrent programming project focused on designing and implementing a centralized scheduler that fairly arbitrates access to shared resources among competing threads.

The simulation models a group of coders competing for shared dongles required to compile their code. Each coder repeatedly:

1. Requests access to the required dongles.
2. Compiles.
3. Debugs.
4. Refactors.

If a coder is unable to start a new compilation before reaching its burnout limit, the simulation ends.

The project explores thread synchronization, resource arbitration, scheduling policies, and deadlock prevention using POSIX threads.

## Design Overview

Instead of allowing coders to compete directly for dongles, all compilation requests are centralized in a dedicated scheduler thread.

The scheduler becomes the single authority responsible for resource allocation, applies the selected scheduling policy (FIFO or EDF), reserves dongles atomically, and grants compilation permissions to eligible coders. This approach simplifies synchronization, prevents deadlocks, and provides a single point for enforcing fairness.

## Request Lifecycle

Each compilation follows the same sequence:

```text
Coder
   |
   | scheduler_enqueue()
   v
Global Priority Heap
   |
   | Scheduler Thread
   | (FIFO / EDF)
   v
Dongles Available?
   |
   +---- No ---> request remains in heap
   |
   +---- Yes
           |
           v
Reserve both dongles
           |
           v
Grant permission
           |
           v
Coder compiles
           |
           v
Release dongles
```

## Instructions

### Compilation
```
make
```

### Run
```
./codexion \ <number_of_coders> \ <time_to_burnout> \ <time_to_compile> \ <time_to_debug> \ <time_to_refactor> \ <number_of_compiles_required> \ <dongle_cooldown> \ <fifo|edf>
```

**Example:**
```
./codexion 3 800 200 100 150 3 50 edf
```

### Other available rules:

- make clean: removes object files
- make fclean: removes object files and the executable
- make re: recompiles the executable

## Simulation Rules

Each coder requires two dongles to compile.

After compiling:

- The coder releases both dongles.
- Performs debugging.
- Performs refactoring.
- Requests a new compilation.

A coder burns out if:
```
current_time - last_compile_start >= time_to_burnout
```

The simulation ends when:
- A coder burns out.
- Every coder has completed the required number of compilations.

## Architecture

```text
                          +-------------------+
                          |     Monitor       |
                          |-------------------|
                          | check_burnout()   |
                          | check_completion()|
                          +--------+----------+
                                   |
                                   | stop simulation
                                   v

+-------------------------------------------------------------+
|                      Scheduler Thread                       |
|-------------------------------------------------------------|
| Global request heap (FIFO / EDF)                            |
| Scheduler mutex                                             |
| Condition variable                                          |
| Grants compilation permissions                              |
+---------------------------+---------------------------------+
                            ^
                            |
                  compilation requests
                            |
                            |
        +-------------------+-------------------+
        |                                       |
+---------------+                       +---------------+
|   Coder #1    |                       |   Coder #N    |
|---------------|                       |---------------|
| request       |                       | request       |
| wait          |                       | wait          |
| compile       |                       | compile       |
| debug         |                       | debug         |
| refactor      |                       | refactor      |
+-------+-------+                       +-------+-------+
        |                                       |
        +---------------+-----------------------+
                        |
                        v
                +----------------+
                |    Dongles     |
                |----------------|
                | mutex          |
                | availability   |
                | release_time   |
                +----------------+
```

- Coders generate compilation requests.
- Requests are inserted into a global priority heap.
- The scheduler grants compilation permissions according to the selected scheduling policy.
- The monitor detects burnout and completion.
- Condition variables are used to put waiting coders to sleep until permission to compile is granted.

## Scheduling Policies

### FIFO

Requests are processed in arrival order.
Priority is determined by: ```request.order```
The oldest request is served first.

### EDF (Earliest Deadline First)

Requests are prioritized according to their burnout deadline.

Priority is determined by: ```request.deadline```
The request with the earliest deadline is served first.
Under heavy contention, EDF prioritizes coders that are closest to burning out.

## Request Arbitration

Coders submit compilation requests to the scheduler, where they are stored in a global priority heap.

During each scheduling cycle, the scheduler evaluates pending requests according to the selected policy (FIFO or EDF). A request is granted only if:

1. Both required dongles are available.
2. Both dongles have completed their cooldown period.
3. Neither dongle has already been reserved during the current scheduling cycle.

Once a request is accepted, the scheduler reserves both dongles and wakes the corresponding coder.

This centralized arbitration guarantees fair resource allocation while preventing race conditions.

## Blocking cases handled

### Single Coder

A single coder can never obtain two dongles.
The coder takes one dongle and eventually burns out.

### Deadlock Prevention

Dongle mutexes are always acquired in a deterministic order:

- the dongle with the lower memory address
- the dongle with the higher memory address

This removes circular wait conditions.

### Resource Contention

Compilation requests are stored in a single global priority heap.

During each scheduling cycle, the scheduler evaluates requests in priority order and reserves dongles for accepted coders, preventing multiple coders from acquiring the same dongle simultaneously.

### Cooldown Synchronization

After a dongle is released, it enters a cooldown period before it can be acquired again.

The scheduler checks cooldown expiration during each scheduling cycle and grants permissions only when both required dongles are ready.

## Thread synchronization mechanisms

### Mutexes

Used to protect:

- Coder state.
- Dongle state.
- Simulation state.
- Console output.
- Scheduler structures.

### Condition Variables

A single scheduler condition variable is shared by all coder threads.

Coders waiting for permission to compile sleep using:
```
pthread_cond_wait()
```

The scheduler wakes waiting coders whenever compilation permissions are granted or the simulation terminates.

### Monitor Thread

The monitor thread periodically checks:
- burnout detection
- simulation completion detection

## Data Structures

### Binary Heap

The scheduler maintains a single binary heap containing all pending compilation requests.

Priority is determined by the selected scheduling policy:

- FIFO: request arrival order.
- EDF: earliest burnout deadline.

Operations:
- heap_push()
- heap_pop()
- heap_peek()

Complexity:
- Insertion: O(log n)
- Removal:   O(log n)
- Peek:      O(1)

## Output

Example:
```
0   1 has taken a dongle
0   1 has taken a dongle
0   1 is compiling
205 1 is debugging
305 1 is refactoring
...
810 3 burned out
```

Timestamps are expressed in milliseconds since the beginning of the simulation.

## Concepts Practiced

- POSIX Threads
- Mutexes
- Condition Variables
- Race Condition Avoidance
- Deadlock Prevention
- Producer/Consumer Synchronization
- Centralized Scheduling
- Priority Queue Scheduling
- Concurrent Resource Arbitration
- Binary Heaps
- Monitor Pattern

## Resources

### Threads:

- https://medium.com/@akshatarhabib/understanding-threads-in-c-c9feb5e9372a
- https://www.geeksforgeeks.org/c/multithreading-in-c/

### POSIX Threads:

- https://code-vault.net/course/6q6s9eerd0:1609007479575

Topics covered:
- Introduction to threads (pthreads)
- Processes vs threads
- Race conditions
- Mutexes(pthread_mutex)
- Thread creation (pthread_create)
- Thread joining (pthread_join)
- Thread arguments
- Condition variables
- Signaling for condition variables (pthread_cond_signal vs pthread_cond_broadcast)

### Binary Heaps:

- https://www.geeksforgeeks.org/c/heap-in-c/
- https://www.youtube.com/watch?v=t0Cq6tVNRBA&t=494s

### AI usage

Artificial intelligence was used to assist with:

- Architecture discussions
- Documentation improvements
- Code review assistance