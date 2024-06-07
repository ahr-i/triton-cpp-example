#include "payment_client.h"
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <iostream>

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace std;

// Constructor to initialize the PaymentClient with the server address
PaymentClient::PaymentClient(const std::string& address) : address_(address) {}

// Function to send a payment request to the server
void PaymentClient::sendPaymentRequest() const {
    try {
        // Combine the address and path to form the complete URI
        uri_builder builder(utility::conversions::to_string_t(address_));
        builder.append_path(U("/pay"));

        // Convert the URI builder to a URI object
        uri complete_uri = builder.to_uri();

        // Log the complete URI to the console
        std::cout << "Request URI: " << utility::conversions::to_utf8string(complete_uri.to_string()) << std::endl;

        // Create an HTTP client with the complete URI
        http_client client(complete_uri);

        // Create an HTTP POST request
        http_request request(methods::POST);

        // Send the request and handle the response
        client.request(request).then([](http_response response) {
            if (response.status_code() == status_codes::OK) {
                // Log success message if the request was successful
                cout << "Payment request sent successfully." << endl;
            }
            else {
                // Log error message if the request failed
                cerr << "Failed to send payment request: " << response.status_code() << endl;
            }
            }).wait();
    }
    catch (const std::exception& e) {
        // Log any exceptions that occur
        cerr << "Exception while sending payment request: " << e.what() << endl;
    }
}
