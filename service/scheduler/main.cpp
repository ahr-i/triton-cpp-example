#include "scheduler.h"
#include "setting.h"
#include <iostream>
#include <thread>

using namespace std;

int main() {
    try {
        // Load settings from the "setting.json" file
        Setting setting("setting.json");

        // Retrieve necessary settings
        std::string server_address = setting.get("server_address");
        std::string server_port_str = setting.get("server_port");
        int server_port = std::stoi(server_port_str); // Convert the server port to an integer
        std::string health_check_url = setting.get("health_check_url");

        // Construct the server address with the port and endpoint
        std::string address = server_address + ":" + std::to_string(server_port) + "/gpu-node/scheduling";

        // Initialize the Scheduler with the server address and health check URL
        Scheduler scheduler(address, health_check_url);
        scheduler.start(); // Start the scheduler

        // Infinite loop to keep the server running
        cout << "Server is running. Press Ctrl+C to stop the server." << endl;
        while (true)
            this_thread::sleep_for(chrono::seconds(1));

        // Stop the scheduler
        scheduler.stop();
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
