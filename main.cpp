#include "JobGenerator.h"
#include "JobExecutor.h"
#include <iostream>
#include <thread>
using namespace std;

int main() {

    ::cout << "Generating jobs...\n"; //generate the job queue
    queue<Job> jobQueue = generateJobs();

    ::cout << "\nExecuting jobs without synchronization:\n"; //execute jobs without synchronization 
    thread withoutSyncThread(executeWithoutSync, std::ref(jobQueue));


    withoutSyncThread.join(); //wait to complete(without synchronization)


    jobQueue = generateJobs();//regenerate the job queue for synchronized
    ::cout << "\nExecuting jobs with synchronization:\n";
    thread withSyncThread(executeWithSync, std::ref(jobQueue));


    withSyncThread.join();//wait to complete(with synchronization)

    return 0;
}
