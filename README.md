# Scheduling-Policy-Demonstration-Program
Implement a program called schedemo, which lets a user run multiple threads with different scheduling policies and show the working status of each thread.

## Background
This project was originally developed as an assignment for the **Operating System** course at **National Yang Ming Chiao Tung University (NYCU)** during the Fall 2023 semester.


## Features
* **CPU Affinity:** Binds all worker threads to a specific CPU core (CPU 2) to eliminate multi-core interference and clearly observe scheduling behaviors.
* **Synchronization Barrier:** Utilizes `pthread_barrier` to synchronize the launch of all threads, ensuring a fair, simultaneous start.
* **Dynamic Configuration:** Supports command-line arguments to customize thread counts, sleep durations, scheduling policies, and priorities.


## Argument Format
The meanings of command-line arguments are:

* `-n` : number of threads to run simultaneously

* `-t` : duration of "busy" period

* `-s` : scheduling policy for each thread (`FIFO` or `NORMAL`).

* `-p` : real-time thread priority for real-time threads

## 🚀 Usage & Example
To observe thread preemption, scheduling policies, and starvation on a single CPU core, run the program with `sudo` (required for real-time `SCHED_FIFO` privileges).

### Example 1
This testcase demonstrates how higher-priority real-time threads preempt lower-priority ones, causing regular threads to starve.

```bash
sudo ./schedemo -n 3 -t 1.0 -s NORMAL,FIFO,FIFO -p -1,10,30
```
### Expected Output:
```text
Thread 2 is running
Thread 2 is running
Thread 2 is running
Thread 1 is running
Thread 1 is running
Thread 1 is running
Thread 0 is running
Thread 0 is running
Thread 0 is running
```
* **Explanation:**
    * **Thread 2 (FIFO, Prio 30)** will completely preempt other threads and finish its execution first.
    * **Thread 1 (FIFO, Prio 10)** will run after Thread 2 finishes.
    * **Thread 0 (NORMAL)** has the lowest priority and will experience CPU starvation, executing only after all real-time threads have completed their tasks.

---

### Example 2
This testcase demonstrates both real-time preemption and how Linux time-slices multiple standard (`NORMAL`) threads once the real-time threads finish.

```bash
sudo schedemo -n 4 -t 0.5 -s NORMAL,FIFO,NORMAL,FIFO -p -1,10,-1,30
```
### Expected Output:
```text
Thread 3 is running
Thread 3 is running
Thread 3 is running
Thread 1 is running
Thread 1 is running
Thread 1 is running
Thread 2 is running
Thread 0 is running
Thread 2 is running
Thread 0 is running
Thread 2 is running
Thread 0 is running
```
* **Explanation:**
    * **Thread 3 (FIFO, Prio 30)** will completely preempt other threads and finish its execution first.
    * **Thread 1 (FIFO, Prio 10)** will run after Thread 3 finishes.
    * **Thread 0 & Thread 2 (NORMAL)** both experience CPU starvation initially.
       * Once all real-time threads complete, the remaining NORMAL threads can compete for CPU time under Linux's normal scheduling policy.
       * Results in the alternating `2 -> 0 -> 2 -> 0 -> 2 -> 0` output patterns.
