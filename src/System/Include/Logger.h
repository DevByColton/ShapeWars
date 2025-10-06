#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger {
private:
    Logger() = default;
    ~Logger() = default;

public:
    static Logger &instance() {
        static auto *instance = new Logger;
        return *instance;
    }

    static void printError(const std::string &);
    static void printOut(const std::string &);
};



#endif //LOGGER_H
