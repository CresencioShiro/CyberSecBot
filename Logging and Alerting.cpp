#include <fstream>

void logEvent(const std::string& event) {
    std::ofstream logfile("security_log.txt", std::ios::app);
    if (logfile.is_open()) {
        logfile << event << std::endl;
        logfile.close();
    }
}
