#include "web_server.h"
#include "setting.h"
#include <iostream>
#include <thread>

using namespace std;

int main() {
    try {
        // Load settings from the configuration file
        Setting setting("setting.json");

        // Retrieve server port and addresses from the settings
        string address = setting.get("server_address");
        string gateway_address = setting.get("gateway_address");
        cout << "address: " << address << '\n';

        // Create and start the web server instance
        WebServer server_instance(address, gateway_address);
        server_instance.start();

        // Infinite loop to keep the server running
        cout << "Server is running. Press Ctrl+C to stop the server." << endl;
        while (true)
            this_thread::sleep_for(chrono::seconds(1));

        // Stop the server
        server_instance.stop();
    }
    catch (const std::exception& e) {
        // Catch and display any exceptions that occur
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
