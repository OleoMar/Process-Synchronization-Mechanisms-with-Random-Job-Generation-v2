#ifndef JOB_H
#define JOB_H
#include <string>

struct Job {
    int user_id;
    std::string job_type;
    int pages;
    int arrival_time;

    Job (int id, std::string type, int pg, int time);
};

#endif
