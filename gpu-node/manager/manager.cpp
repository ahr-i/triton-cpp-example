#include "manager.h"
#include <cpprest/uri.h>
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <iostream>

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::client;

// Constructor to initialize Manager with health checker address and port
Manager::Manager(const string& address, const string& port)
    : health_checker_address_(address), port_(port) {}

// Function to register the service with the health checker
void Manager::registerWithHealthChecker() {
    try {
        // Build the URI for the health checker registration endpoint
        uri_builder builder(utility::conversions::to_string_t(health_checker_address_));
        builder.append_path(U("/service/register"));

        // Create HTTP client and POST request
        http_client client(builder.to_uri());
        http_request request(methods::POST);

        // Prepare the JSON body with the port information
        utility::string_t port = utility::conversions::to_string_t(port_);
        json::value body;
        body[U("port")] = json::value::string(port);

        // Set the request body
        request.set_body(body);
        // Uncomment the line below to set the Content-Type header to application/json
        // request.headers().add(U("Content-Type"), U("application/json"));

        // Send the request and get the response
        http_response response = client.request(request).get();
        if (response.status_code() == status_codes::OK) {
            cout << "Successfully registered with Health-Checker" << endl;
        }
        else {
            cerr << "Failed to register with Health-Checker: " << response.status_code() << endl;
        }
    }
    catch (const std::exception& e) {
        cerr << "Exception while registering with Health-Checker: " << e.what() << endl;
    }
}
