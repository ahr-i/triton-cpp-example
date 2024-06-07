#include "request_handler.h"
#include <cpprest/json.h>
#include <iostream>

using namespace web;
using namespace web::http;

void RequestHandler::handlePost(http_request request) {
    // Extract JSON from the request
    request.extract_json().then([=](json::value json) {
        // Handle the JSON request here by printing it to the console
        std::cout << utility::conversions::to_utf8string(json.serialize()) << std::endl;

        // For demonstration purposes, send back a static response
        json::value response;
        response[U("message")] = json::value::string(U("Received JSON"));

        // Reply to the client with the response
        request.reply(status_codes::OK, response);
        }).wait();
}
