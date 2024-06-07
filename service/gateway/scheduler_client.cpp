#include "scheduler_client.h"
#include <cpprest/uri_builder.h>

using namespace web;
using namespace web::http;
using namespace web::http::client;

// Constructor for SchedulerClient which initializes the HTTP client with the given address
SchedulerClient::SchedulerClient(const std::string& address)
    : client_(uri(utility::conversions::to_string_t(address))) {}

// Function to get the node address by sending a GET request to the scheduler
pplx::task<json::value> SchedulerClient::getNodeAddress() const {
    // Build the URI for the scheduling endpoint
    uri_builder builder(U("/gpu-node/scheduling"));

    // Send the GET request to the scheduler
    return const_cast<http_client&>(client_).request(methods::GET, builder.to_string())
        .then([](http_response response) -> pplx::task<json::value> {
        // If the response status is OK, extract the JSON body
        if (response.status_code() == status_codes::OK) {
            return response.extract_json();
        }
        // Otherwise, return an empty JSON value
        return pplx::task_from_result(json::value());
            });
}
