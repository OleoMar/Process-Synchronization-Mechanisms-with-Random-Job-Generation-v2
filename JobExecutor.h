#pragma once
#ifndef JOBEXECUTOR_H
#define JOBEXECUTOR_H
#include <queue>
#include "Job.h"

void executeWithoutSync(std::queue<Job>& jobQueue);
void executeWithSync(std::queue<Job>& jobQueue);

#endif

