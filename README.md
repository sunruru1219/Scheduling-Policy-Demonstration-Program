# Scheduling-Policy-Demonstration-Program
Implement a program called sched_demo, which lets a user run multiple threads with different scheduling policies and show the working status of each thread

The meanings of command-line arguments are:

-n <num_threads>: number of threads to run simultaneously
-t <time_wait>: duration of "busy" period
-s <policies>: scheduling policy for each thread, SCHED_FIFO or SCHED_NORMAL.
The example NORMAL,FIFO,NROMAL,FIFO shown above means to apply SCHED_NORMAL policy to the 1st and 3rd thread and SCHED_FIFO policy to the 2nd and 4nd thread.
-p <priorities>: real-time thread priority for real-time threads
The example -1,10,-1,30 shown above means to set the value 10 to the 2nd thread and value 30 to the 4nd thread.
You should specify the value -1 for threads with SCHED_NORMAL policy.
The program can be divided into two sections: the main thread section and worker thread section.
