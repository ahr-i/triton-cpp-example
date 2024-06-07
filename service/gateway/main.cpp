#include "gateway_server.h"
#include "setting.h"
#include <iostream>

using namespace std;

int main() {
    try {
        // Load settings from the configuration file
        Setting setting("setting.json");

        // Retrieve the server port, scheduler address, and payment address from the settings
        string port = setting.get("server_port");
        string address = "http://127.0.0.1:" + port;
        string scheduler_address = setting.get("scheduler_address");
        string payment_address = setting.get("payment_address");

        // Create an instance of the GatewayServer with the retrieved addresses
        GatewayServer server_instance(address, scheduler_address, payment_address);

        // Start the server
        server_instance.start();

        // Inform the user to press Enter to stop the server
        cout << "Press Enter to stop the server." << endl;
        cin.get();

        // Stop the server
        server_instance.stop();
    }
    catch (const std::exception& e) {
        // Catch and print any exceptions that occur
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
