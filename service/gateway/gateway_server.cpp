#include "gateway_server.h"
#include <iostream>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
using namespace std;

// Constructor for GatewayServer
// Initializes the listener and request handler with the provided addresses
GatewayServer::GatewayServer(const std::string& address, const std::string& scheduler_address, const std::string& payment_address)
    : listener_(web::uri(utility::conversions::to_string_t(address))),
    request_handler_(make_shared<GatewayRequestHandler>(scheduler_address, payment_address)) {}

// Start the GatewayServer
void GatewayServer::start() {
    // Set up the listener to handle POST requests using the request handler
    listener_.support(methods::POST, std::bind(&GatewayRequestHandler::handlePost, request_handler_.get(), std::placeholders::_1));
    // Open the listener and wait for it to be ready
    listener_.open().wait();
    // Print a message indicating the server has started
    cout << "Server started at " << utility::conversions::to_utf8string(listener_.uri().to_string()) << endl;
}

// Stop the GatewayServer
void GatewayServer::stop() {
    // Close the listener and wait for it to finish
    listener_.close().wait();
}
