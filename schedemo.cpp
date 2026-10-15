#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sched.h>
#include <pthread.h>
#include <chrono>
using namespace std;

double time_wait;
pthread_barrier_t barrier;

void busyWait(double t) {
    auto startTime = chrono::high_resolution_clock::now();
    long long durationNanoSeconds = static_cast<long long>(t * 1e9);

    while(true) {
        auto currentTime = chrono::high_resolution_clock::now();
        auto elapsedTime = chrono::duration_cast<chrono::nanoseconds>(currentTime-startTime).count();
        if (elapsedTime >= durationNanoSeconds) {
            break;
        }

        int x = 0;
        for (int i = 0; i < 10000; ++i) x+=i*i;
    }
}

void * worker(void *arg) {

    pthread_barrier_wait(&barrier);
    int *n = (int *)arg;
    for(int i = 0; i < 3; ++i) {
        cout << "Thread " << *n << " is running" << endl;
        busyWait(time_wait);
    }
    return NULL;
}


int main(int argc, char* argv[]) {
    int num_threads;// = 4;
    //double time_wait;// = 0.5;
    vector<string> policies;//{"FIFO", "NORMAL", "FIFO", "NORMAL"};
    vector<int> prior;//{-1,10,-1,30};


    /* 1. Parse program arguments */
    int opt;
    string token;
    istringstream iss;

    while ((opt = getopt(argc,argv, "n:t:s:p:")) != -1) {
        switch(opt) {
            case 'n':
                num_threads = stoi(optarg);
                break;
            case 't':
                time_wait = stod(optarg);
                break;
            case 's':
                iss.clear();
                iss.str(optarg);
                while (getline(iss, token, ',')) {
                    policies.push_back(token);
                }
                break;
            case 'p':
                iss.clear();
                iss.str(optarg);
                while (getline(iss, token, ',')) {
                    prior.push_back(stoi(token));
                }
                break;
            default:
               break;
        } // end switch
    } // end opt while

    pthread_barrier_init(&barrier, NULL, num_threads+1);

    /* 2. Create <num_threads> worker threads */
    vector<int> thread_id;
    for (int i = 0; i < num_threads; ++i) thread_id.push_back(i);
    vector<pthread_t> threads(num_threads);

    /* 3. Set CPU affinity */
    int cpu_id = 2;
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu_id, &cpuset);

    for (int i = 0; i < num_threads; i++) {
        /* 4. Set the attributes to each thread */
        pthread_attr_t attr;
        struct sched_param param;

        pthread_attr_init(&attr);

        int policy;
        if (policies[i] == "FIFO") {
            policy = SCHED_FIFO;
        }
        else {
            policy = SCHED_OTHER;
        }

        pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
        pthread_attr_setschedpolicy(&attr, policy);

        param.sched_priority = prior[i];
        pthread_attr_setschedparam(&attr, &param);

        if (pthread_create(&threads[i], &attr, worker, &thread_id[i]) != 0) {
            cerr <<  "Error!\n";
        }

        pthread_setaffinity_np(threads[i], sizeof(cpu_set_t), &cpuset);
        pthread_attr_destroy(&attr);

    }

    /* 5. Start all threads at once */
    pthread_barrier_wait(&barrier);
    
    /* 6. Wait for all threads to finish  */
    sleep(10);
    pthread_barrier_destroy(&barrier);

    return 0;
}

