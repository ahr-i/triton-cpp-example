#include "service_handler.h"
#include <cpprest/json.h>

using namespace web;
using namespace web::http;
using namespace std;

// Constructor for the ServiceHandler class
ServiceHandler::ServiceHandler(vector<string>& alive_nodes, mutex& mutex)
    : alive_nodes_(alive_nodes), mutex_(mutex) {
}

// Handle GET requests
void ServiceHandler::handleGet(http_request request) {
    json::value response_json;
    {
        // Lock the mutex to ensure thread-safe access to the alive_nodes_ vector
        lock_guard<mutex> lock(mutex_);

        // Create a JSON array to hold the list of alive nodes
        json::value service_list = json::value::array(alive_nodes_.size());
        for (size_t i = 0; i < alive_nodes_.size(); ++i) {
            service_list[i] = json::value::string(utility::conversions::to_string_t(alive_nodes_[i]));
        }

        // Add the service list to the response JSON
        response_json[U("service_list")] = service_list;
    }

    // Send the response with the list of alive nodes
    request.reply(status_codes::OK, response_json);
}
