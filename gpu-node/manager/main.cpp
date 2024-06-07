#include "server.h"
#include "setting.h"
#include "manager.h"
#include <iostream>

using namespace std;

int main() {
    // Load settings from configuration file
    Setting setting("setting.json");

    // Get server port and address from settings
    string port = setting.get("server_port");
    string address = "http://127.0.0.1:" + port;

    // Get health checker URL from settings
    string health_checker = setting.get("health_check_url");

    // Initialize and start the server
    Server server_instance(address);
    server_instance.start();

    // Initialize manager and register the server with health checker
    Manager manager(health_checker, port);
    manager.registerWithHealthChecker();

    // Wait for user input to stop the server
    cout << "Press Enter to stop the server." << endl;
    cin.get();

    // Stop the server
    server_instance.stop();
    return 0;
}
