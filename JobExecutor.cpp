#include <iostream>
#include <unordered_map>
#include <queue>
#include <thread>
#include <chrono>
#include <random>
#include <mutex>
#include "Job.h"

std::mutex printerMutex;

void executeWithoutSync(std::queue<Job>& jobQueue) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 5); // random interruption generator

    std::unordered_map<int, std::queue<Job>> userJobQueues;

    // group jobs by user
    while (!jobQueue.empty()) {
        Job job = jobQueue.front();
        jobQueue.pop();
        userJobQueues[job.user_id].push(job);
    }

    auto userIt = userJobQueues.begin();
    while (!userJobQueues.empty()) {
        // wrap around if iterator reaches the end
        if (userIt == userJobQueues.end()) {
            userIt = userJobQueues.begin();
        }

        int userId = userIt->first;
        std::queue<Job>& userQueue = userIt->second;

        // ff the users queue is empty it remove user and move to the next
        if (userQueue.empty()) {
            userIt = userJobQueues.erase(userIt); //erase and update iterator
            continue;
        }

        Job job = userQueue.front();
        userQueue.pop();

        for (int i = 1; i <= job.pages; ++i) {
            {
                //synchronise console output using mutex
                std::lock_guard<std::mutex> lock(printerMutex);
                std::cout << "Processing " << job.job_type << " for User P" << userId
                    << " - Page " << i << " of " << job.pages << "...\n";
            }
            std::this_thread::sleep_for(std::chrono::seconds(1)); //simulate processing time

            // deal with random interruption
            if (dist(gen) == 1) {
                {
                    std::lock_guard<std::mutex> lock(printerMutex);
                    std::cout << "Random interruption in processing for User P" << userId << "...\n";
                }

                // save remaining pages and requeue
                if (i < job.pages) {
                    job.pages -= i; // Update remaining pages
                    userQueue.push(job);
                }

                //move to the next user
                if (userQueue.empty()) {
                    userIt = userJobQueues.erase(userIt); //erase and update iterator
                }
                else {
                    ++userIt;
                }
                break; // exit and switch to the next user
            }
        }

        //check if the users queue is empty then remove it
        if (userQueue.empty()) {
            userIt = userJobQueues.erase(userIt); //erase and update iterator
        }
        else if (userIt != userJobQueues.end()) {
            ++userIt; // move to the next user
        }
    }
}

void executeWithSync(std::queue<Job>& jobQueue) {
    std::unordered_map<int, std::vector<Job>> userJobs;

    // group jobs by user ID
    while (!jobQueue.empty()) {
        Job job = jobQueue.front();
        jobQueue.pop();
        userJobs[job.user_id].push_back(job);
    }

    // process jobs for each user
    for (const auto& user : userJobs) {
        int userId = user.first;
        const auto& jobs = user.second;

        {
            std::lock_guard<std::mutex> lock(printerMutex);
            std::cout << "Processing jobs for User P" << userId << ":\n";
        }

        for (const auto& job : jobs) {
            for (int i = 1; i <= job.pages; ++i) {
                {
                    std::lock_guard<std::mutex> lock(printerMutex);
                    std::cout << "Processing " << job.job_type << " for User P" << userId
                        << " - Page " << i << " of " << job.pages << "...\n";
                }
                std::this_thread::sleep_for(std::chrono::seconds(1)); //simulate processing time
            }
        }

            {
                std::lock_guard<std::mutex> lock(printerMutex);
                std::cout << "Completed all jobs for User P" << userId << "\n\n";
            }
    }
}