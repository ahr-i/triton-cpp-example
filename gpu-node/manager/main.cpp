#include "server.h"
#include "setting.h"
#include "manager.h"
#include <iostream>
#include <thread>

using namespace std;

int main() {
    // Load settings from configuration file
    Setting setting("setting.json");

    // Get server port and address from settings
    string port = setting.get("server_port");
    string ip = setting.get("server_ip");
    string address = "http://" + ip + ":" + port;

    // Get health checker URL from settings
    string health_checker = setting.get("health_check_url");

    // Initialize and start the server
    Server server_instance(address);
    server_instance.start();

    // Initialize manager and register the server with health checker
    Manager manager(health_checker, port);
    manager.registerWithHealthChecker();

    // Infinite loop to keep the server running
    cout << "Server is running. Press Ctrl+C to stop the server." << endl;
    while (true)
        this_thread::sleep_for(chrono::seconds(1));

    // Stop the server
    server_instance.stop();
    return 0;
}
