#include "server.h"
#include "health_checker.h"
#include <cpprest/uri.h>
#include <cpprest/http_client.h>
#include <iostream>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
using namespace std;

// Constructor for the Server class
Server::Server(const std::string& address, int interval)
    : listener(web::uri(utility::conversions::to_string_t(address))),
    health_checker_(std::make_shared<HealthChecker>(interval, listener)) {
    // Set up the listener to handle POST requests
    listener.support(methods::POST, std::bind(&Server::handlePost, this, std::placeholders::_1));
}

// Start the server and health checker
void Server::start() {
    try {
        // Open the listener
        listener.open().wait();
        // Start the health checker
        health_checker_->start();
        cout << "Server started at " << utility::conversions::to_utf8string(listener.uri().to_string()) << endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Listener failed to start: " << e.what() << std::endl;
    }
}

// Stop the server and health checker
void Server::stop() {
    try {
        // Close the listener
        listener.close().wait();
        // Stop the health checker
        health_checker_->stop();
    }
    catch (const std::exception& e) {
        std::cerr << "Listener failed to stop: " << e.what() << std::endl;
    }
}

// Get the list of alive nodes
std::vector<std::string> Server::getAliveNodes() const {
    return health_checker_->getAliveNodes();
}

// Handle POST requests
void Server::handlePost(http_request request) {
    // Delegate the request handling to the health checker
    health_checker_->handlePost(request);
}
