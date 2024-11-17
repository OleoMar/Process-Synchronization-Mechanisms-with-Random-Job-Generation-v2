#include "Job.h"
//constructor definition
Job::Job(int id, std::string type, int pg, int time)
    : user_id(id), job_type(type), pages(pg), arrival_time(time) {}
