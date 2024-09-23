#include <iostream>
#include <cstring>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <fstream>

void unsafeFunction(char* input) {
    char buffer[10];
    if (strlen(input) >= sizeof(buffer)) {
        std::cout << "Potential Buffer Overflow Detected!" << std::endl;
        logEvent("Buffer Overflow Attack Detected!");
        return;
    }
    strcpy(buffer, input);
    std::cout << "Buffer contents: " << buffer << std::endl;
}

void detectExploit() {
    volatile unsigned long canary = 0xdeadbeef;
    if (canary != 0xdeadbeef) {
        std::cout << "Exploit detected!" << std::endl;
        logEvent("Exploit Attempt Detected");
    }
}

void logEvent(const std::string& event) {
    std::ofstream logfile("security_log.txt", std::ios::app);
    if (logfile.is_open()) {
        logfile << event << std::endl;
        logfile.close();
    }
}

void randomDelayTraffic() {
    int delay = rand() % 1000 + 500;
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
}

int main() {
    char testInput[] = "ExploitAttempt";
    unsafeFunction(testInput);
    detectExploit();

    logEvent("System Monitoring Started");
    randomDelayTraffic();
    std::cout << "Traffic obfuscation complete." << std::endl;

    return 0;
}
