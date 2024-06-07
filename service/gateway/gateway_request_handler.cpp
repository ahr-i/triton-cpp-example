#include "gateway_request_handler.h"
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <iostream>

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::client;

// Constructor for GatewayRequestHandler
// Initializes scheduler_client_ and payment_client_ with the provided addresses
GatewayRequestHandler::GatewayRequestHandler(const std::string& scheduler_address, const std::string& payment_address)
    : scheduler_client_(std::make_shared<SchedulerClient>(scheduler_address)),
    payment_client_(std::make_shared<PaymentClient>(payment_address)) {}

// Handle POST requests
void GatewayRequestHandler::handlePost(http_request request) {
    // Get the node address from the scheduler client
    scheduler_client_->getNodeAddress().then([=](json::value response) mutable {
        try {
            // Check if the response contains an "address" field and that it is a string
            if (response.has_field(U("address")) && response.at(U("address")).is_string()) {
                utility::string_t address = response.at(U("address")).as_string();  // Extract the address
                uri_builder builder(address);  // Build the URI with the address
                builder.append(request.relative_uri().to_string());  // Append the request URI to the builder

                http_client client(builder.to_uri());  // Create an HTTP client with the built URI
                http_request manager_request(methods::POST);  // Create a new POST request

                manager_request.headers() = request.headers();  // Copy headers from the incoming request
                // Extract the body of the incoming request as a string
                request.extract_string().then([=](utility::string_t body) mutable {
                    manager_request.set_body(body);  // Set the body of the manager request

                    // Send the manager request and process the response
                    client.request(manager_request).then([=](http_response manager_response) mutable {
                        // Extract the JSON response from the manager response
                        manager_response.extract_json().then([=](json::value jsonResponse) mutable {
                            // Reply to the original request with the manager response
                            request.reply(manager_response.status_code(), jsonResponse);

                            // After inference is done, send a POST request to /pay
                            payment_client_->sendPaymentRequest();
                            }).wait();
                        }).wait();
                    }).wait();
            }
            else {
                // If the scheduler response is invalid, reply with an InternalError
                request.reply(status_codes::InternalError, U("Invalid scheduler response"));
            }
        }
        catch (const std::exception& e) {
            // Handle any exceptions that occur
            cerr << "Exception: " << e.what() << endl;
            request.reply(status_codes::InternalError, U("Exception occurred"));
        }
        }).wait();
}
