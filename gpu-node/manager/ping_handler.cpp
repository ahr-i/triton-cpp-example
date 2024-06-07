#include "ping_handler.h"
#include <iostream>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
using namespace std;

// Constructor to initialize the PingHandler with the given address
PingHandler::PingHandler(const std::string& address)
    : listener(web::uri(utility::conversions::to_string_t(address))) {
    // Support GET method and bind it to the handleGet function
    listener.support(methods::GET, std::bind(&PingHandler::handleGet, this, std::placeholders::_1));
}

// Function to start the PingHandler listener
void PingHandler::start() {
    listener.open().wait();
    cout << "PingHandler started at " << utility::conversions::to_utf8string(listener.uri().to_string()) << endl;
}

// Function to stop the PingHandler listener
void PingHandler::stop() {
    listener.close().wait();
}

// Function to handle GET requests
void PingHandler::handleGet(http_request request) {
    std::cout << "Ping received" << " - Health check" << '\n';
    request.reply(status_codes::OK, U("Pong"));
}
