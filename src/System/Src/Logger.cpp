#include <iostream>
#include "../Include/Logger.h"

void Logger::printError(const std::string &message) {
    std::cerr << std::endl << message << std::endl;
}

void Logger::printOut(const std::string &message) {
    std::cout << std::endl << message << std::endl;
}

