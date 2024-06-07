#include "server.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

// Constructor for the Server class
Server::Server(const std::string& address)
    : listener_(uri(utility::conversions::to_string_t(address))) {
    // Bind the GET method to the handleGet function
    listener_.support(methods::GET, std::bind(&Server::handleGet, this, std::placeholders::_1));
}

// Start the server
void Server::start() {
    listener_.open().wait();
    std::cout << "Server started at " << utility::conversions::to_utf8string(listener_.uri().to_string()) << std::endl;
}

// Stop the server
void Server::stop() {
    listener_.close().wait();
}

// Handle GET requests
void Server::handleGet(http_request request) {
    auto path = uri::decode(request.relative_uri().path());
    if (path == U("/")) {
        path = U("/index.html");
    }

    std::string full_path = "." + utility::conversions::to_utf8string(path);
    std::ifstream file(full_path, std::ios::binary);

    if (!file) {
        // If the file is not found, reply with a 404 Not Found status
        request.reply(status_codes::NotFound, U("File not found"));
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    auto content = buffer.str();
    // Reply with the file content and a status of 200 OK
    request.reply(status_codes::OK, content, "text/html");
}
