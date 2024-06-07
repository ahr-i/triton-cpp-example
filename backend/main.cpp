#include "web_server.h"
#include "setting.h"
#include <iostream>

using namespace std;

int main() {
    try {
        // Load settings from the configuration file
        Setting setting("setting.json");

        // Retrieve server port and addresses from the settings
        string port = setting.get("server_port");
        string address = "http://127.0.0.1:" + port;
        string gateway_address = setting.get("gateway_address");

        // Create and start the web server instance
        WebServer server_instance(address, gateway_address);
        server_instance.start();

        // Wait for user input to stop the server
        cout << "Press Enter to stop the server." << endl;
        cin.get();

        // Stop the server
        server_instance.stop();
    }
    catch (const std::exception& e) {
        // Catch and display any exceptions that occur
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
