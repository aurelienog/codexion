*This project has been created as part of the 42 curriculum by aunoguei.*

# Codexion

## Description

***Codexion*** is a concurrent programming project focused on designing fair and efficient protocols for sharing limited resources between multiple threads.

The simulation models a group of coders competing for access to shared dongles required to compile code. Each coder repeatedly:

1. Requests access to the required dongles.
2. Compiles.
3. Debugs.
4. Refactors.

If a coder is unable to start a new compilation before reaching its burnout limit, the simulation ends.

The project explores thread synchronization, resource arbitration, scheduling policies, and deadlock prevention using POSIX threads.


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
./codexion 5 800 200 100 150 3 50 edf
```

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
                    +------------------+
                    |     Monitor      |
                    |------------------|
                    | check_burnout()  |
                    | check_completion()|
                    | check_cooldowns()|
                    +--------+---------+
                             |
                             | pthread_cond_broadcast()
                             v

+--------------------------------------------------------+
|                    Scheduler                           |
|--------------------------------------------------------|
| scheduler_mutex                                        |
| scheduler_cond                                         |
+--------------------------------------------------------+

        ^                                   ^
        | request_compile()                 |
        |                                   |
        |                                   |

+---------------+                 +---------------+
|   Coder #1    |                 |   Coder #N    |
|---------------|                 |---------------|
| compile()     |                 | compile()     |
| debug()       |                 | debug()       |
| refactor()    |                 | refactor()    |
+-------+-------+                 +-------+-------+
        |                                 |
        | needs two dongles              |
        +---------------+----------------+
                        |
                        v

        +-----------------------------+
        |          Dongles            |
        |-----------------------------|
        | mutex                       |
        | is_available                |
        | release_time                |
        | cooldown_expired_notified   |
        | request_heap                |
        +-------------+---------------+
                      |
                      v

        +-----------------------------+
        |        Binary Heap          |
        |-----------------------------|
        | FIFO -> order               |
        | EDF  -> deadline            |
        +-----------------------------+
```

- Coders generate compilation requests.
- Requests are inserted into the two required dongle queues.
- Each dongle maintains its own priority heap.
- The scheduler grants access only when the coder is at the top of both queues.
- The monitor detects burnout, completion and cooldown expiration.
- Condition variables are used to put waiting coders to sleep until resources become available.

## Scheduling Policies

### FIFO

Requests are processed in arrival order.
Priority is determined by: ```request.order```
The oldest request is served first.

### EDF (Earliest Deadline First)

Requests are prioritized according to their burnout deadline.

Priority is determined by: ```request.deadline```
The request with the earliest deadline is served first.
EDF reduces starvation and improves survival under heavy contention.

## Request Arbitration

A coder can start compiling only if:

1. Both required dongles are available.
2. Both dongle cooldowns have expired.
3. The coder is at the top of both dongle request queues.

Formally:
```
left available
AND right available
AND left cooldown expired
AND right cooldown expired
AND first in left queue
AND first in right queue
```
This guarantees fair resource acquisition while preventing race conditions.

## Blocking cases handled

### Single Coder

A single coder can never obtain two dongles.
The coder takes one dongle and eventually burns out.

### Deadlock Prevention

Dongle mutexes are always acquired in a deterministic order:

- lowest address first
- highest address second

This removes circular wait conditions.

### Resource Contention

Requests are stored in priority queues and resolved by the selected scheduler (FIFO or EDF).

### Cooldown Synchronization

After a dongle is released, it cannot be reused until its cooldown period expires.

Coders sleep on a condition variable while waiting.

The monitor wakes all waiting coders when a cooldown expiration becomes available.

## Thread synchronization mechanisms

### Mutexes

Used to protect:

- Coder state.
- Dongle state.
- Simulation state.
- Console output.
- Scheduler structures.

### Condition Variables

A single scheduler condition variable is used.

Waiting coders sleep using:
```
pthread_cond_wait()
```
and are awakened when:
- A dongle cooldown expires.
- The simulation terminates.

### Monitor Thread

The monitor executes: ```watch()```
and periodically checks:
```
check_burnout()  
check_completion()  
check_cooldowns()
```

## Data Structures

### Binary Heap

Each dongle maintains a priority queue implemented as a binary heap.

Operations:
- heap_push
- heap_pop
- heap_peek

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
```

Timestamps are expressed in milliseconds since the beginning of the simulation.

## Concepts Practiced

- POSIX Threads
- Mutexes
- Condition Variables
- Producer/Consumer Synchronization
- Deadlock Prevention
- Race Condition Avoidance
- Priority Scheduling
- Binary Heaps
- Concurrent Resource Arbitration
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