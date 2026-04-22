#include "ServerUAVs.h"
#include <iostream>
#include <signal.h>

volatile sig_atomic_t g_running = 1;

void signalHandler(int) {
    g_running = 0;
}

int main() {
    signal(SIGINT, signalHandler);

    try {
        ServerCore server(8888, 8889);
        server.start();

        std::cout << "Server started. Press Ctrl+C to stop." << std::endl;

        // ѕример: мен€ем центр формации через 30 секунд
        std::thread([&server]() {
            std::this_thread::sleep_for(std::chrono::seconds(30));
            server.setFormationCenter(55.124, 37.457);
            std::cout << "[Server] Formation center changed!" << std::endl;
            }).detach();

            while (g_running) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            server.stop();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}