#include <iostream>
#include <ctime>
#include <BleCommunicator.h>;

int main()
{
    BleCommunicator* bleCommunicator = new BleCommunicator();
    if (!bleCommunicator->initialize()) {
        std::cout << "VR Shoe not initialized" << std::endl;
        return -1;
    }
    auto start = std::clock();
    double duration;
    while (true) {
        std::cout << bleCommunicator->read() << std::endl;
        duration = (std::clock() - (double)(start)) / (double)CLOCKS_PER_SEC;
        std::cout << "Duration: " << duration << std::endl;
        start = std::clock();
    }
}