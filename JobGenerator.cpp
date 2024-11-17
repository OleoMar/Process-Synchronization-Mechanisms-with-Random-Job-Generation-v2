#include "JobGenerator.h"
#include "Job.h"
#include <queue>
#include <random>
#include <chrono>
#include <thread>
#include <iostream>

std::queue<Job> generateJobs() {
    std::queue<Job> jobQueue;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> type_dist(0, 1);
    std::uniform_int_distribution<> page_dist(1, 3);
    std::uniform_int_distribution<> interval_dist(1, 5);

    int accumulatedTime = 0;

    //generate "Jobs"  for users
    for (int i = 0; i < 50; ++i) {
        int user = (i % 5) + 1; //round-robin for users 1 to 5
        std::string type = type_dist(gen) == 0 ? "Print" : "Scan";
        int pages;

        // generate number of pages
        int job_length_type = page_dist(gen);
        if (job_length_type == 1) {
            pages = 1 + (rand() % 5);
        }
        else if (job_length_type == 2) {
            pages = 6 + (rand() % 10);
        }
        else {
            pages = 16 + (rand() % 35);
        }

        int interval = interval_dist(gen);
        accumulatedTime += interval;

        //add new job to the queue
        jobQueue.push(Job(user, type, pages, accumulatedTime));

        // display info
        std::cout << "User P" << user << ": " << type << " Job, "
            << pages << " pages, Arrival Time: " << accumulatedTime << " seconds\n";
    }

    return jobQueue;
}