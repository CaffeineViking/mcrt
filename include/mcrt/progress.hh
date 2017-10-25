#ifndef MCRT_PROGRESS_HH
#define MCRT_PROGRESS_HH

#include <string>

void printProgress(const std::string& task, double progress,
                   size_t characters = 50);

#endif
