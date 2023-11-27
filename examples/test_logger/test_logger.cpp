#include "logger.hpp"

int main() {
    Logger logger("test", "./");
    logger.Log(Logger::LOG_LEVEL::DEBUG, __FILE__, __LINE__, "%d", 12345);
    logger.Log(Logger::LOG_LEVEL::INFO, __FILE__, __LINE__, "%s", "fdsafdsafsd");
    logger.Log(Logger::LOG_LEVEL::ERROR, __FILE__, __LINE__, "%f", 123.456);
}