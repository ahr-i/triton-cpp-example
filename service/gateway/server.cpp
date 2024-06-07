#include "server.h"
#include "gateway_request_handler.h"
#include <iostream>

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

// Constructor for the Server class, initializing the listener with the given address and request handler
Server::Server(const std::string& address, std::shared_ptr<GatewayRequestHandler> request_handler)
    : listener_(web::uri(utility::conversions::to_string_t(address))), request_handler_(request_handler) {}

// Function to start the server
void Server::start() {
    // Set the listener to support POST methods and bind the handlePost function from the request handler
    listener_.support(methods::POST, std::bind(&GatewayRequestHandler::handlePost, request_handler_.get(), std::placeholders::_1));
    // Open the listener
    listener_.open().wait();
    // Output the server start message with the address
    cout << "Server started at " << utility::conversions::to_utf8string(listener_.uri().to_string()) << endl;
}

// Function to stop the server
void Server::stop() {
    // Close the listener
    listener_.close().wait();
}
