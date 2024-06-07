#include "server.h"
#include "setting.h"
#include <iostream>

using namespace std;

int main() {
    try {
        // Load settings from the configuration file
        Setting setting("setting.json");

        // Get the server address, port, and health check interval from the settings
        std::string server_address = setting.get("server_address");
        std::string server_port_str = setting.get("server_port");
        int server_port = std::stoi(server_port_str);
        int health_check_interval = std::stoi(setting.get("health_check_interval"));

        // Construct the full address of the server
        std::string address = server_address + ":" + std::to_string(server_port);

        // Create and start the server instance
        Server server_instance(address, health_check_interval);
        server_instance.start();

        cout << "Press Enter to stop the server." << endl;
        cin.get(); // Wait for user input to stop the server

        // Stop the server instance
        server_instance.stop();
    }
    catch (const std::invalid_argument& e) {
        // Handle invalid argument exceptions
        std::cerr << "Invalid argument: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        // Handle other exceptions
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0; // Return 0 to indicate successful execution
}
