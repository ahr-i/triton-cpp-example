#include "server.h"
#include "request_handler.h"
#include <iostream>

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

// Constructor for the Server class
Server::Server(const std::string& address)
    : listener(web::uri(utility::conversions::to_string_t(address))),
    request_handler_(make_shared<RequestHandler>()),
    ping_handler_(make_shared<PingHandler>(address + "/ping")) {

    // Bind the handlePost method of RequestHandler to the POST method of the listener
    listener.support(methods::POST, bind(&RequestHandler::handlePost, request_handler_, placeholders::_1));
}

// Start the server
void Server::start() {
    listener.open().wait(); // Open the listener
    ping_handler_->start(); // Start the PingHandler
    cout << "Server started at " << utility::conversions::to_utf8string(listener.uri().to_string()) << endl;
}

// Stop the server
void Server::stop() {
    listener.close().wait(); // Close the listener
    ping_handler_->stop();   // Stop the PingHandler
}
