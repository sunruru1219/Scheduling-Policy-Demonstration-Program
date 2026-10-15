# Scheduling-Policy-Demonstration-Program
Implement a program called sched_demo, which lets a user run multiple threads with different scheduling policies and show the working status of each thread

The meanings of command-line arguments are:

-n <num_threads>: number of threads to run simultaneously

-t <time_wait>: duration of "busy" period

-s <policies>: scheduling policy for each thread, SCHED_FIFO or SCHED_NORMAL.

-p <priorities>: real-time thread priority for real-time threads
