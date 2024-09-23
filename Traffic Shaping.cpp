#include <chrono>
#include <thread>

void randomDelayTraffic() {
    int delay = rand() % 1000 + 500;  // Random delay between 500ms and 1500ms
    std::cout << "Traffic delayed by " << delay << " milliseconds." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
}
