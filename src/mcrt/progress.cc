#include "mcrt/progress.hh"

#include <iostream>

void printProgress(const std::string& task, double progress, size_t characters) {
    std::cout << task << "[";
    size_t position = progress * characters;
    for (size_t i { 0 }; i < characters; ++i) {
        if (i < position) std::cout << "=";
        else if (i > position) std::cout << " ";
        else std::cout << ">";
    } std::cout << "] ";

    size_t percent = progress * 100.0;
    std::cout << percent << " %\r";
    std::cout.flush();
}
