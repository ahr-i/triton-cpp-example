#include "gateway_server.h"
#include "setting.h"
#include <iostream>
#include <thread>

using namespace std;

int main() {
    try {
        // Load settings from the configuration file
        Setting setting("setting.json");

        // Retrieve the server port, scheduler address, and payment address from the settings
        string address = setting.get("server_address");
        string scheduler_address = setting.get("scheduler_address");
        string payment_address = setting.get("payment_address");

        // Create an instance of the GatewayServer with the retrieved addresses
        GatewayServer server_instance(address, scheduler_address, payment_address);

        // Start the server
        server_instance.start();

        // Infinite loop to keep the server running
        cout << "Server is running. Press Ctrl+C to stop the server." << endl;
        while (true)
            this_thread::sleep_for(chrono::seconds(1));

        // Stop the server
        server_instance.stop();
    }
    catch (const std::exception& e) {
        // Catch and print any exceptions that occur
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
