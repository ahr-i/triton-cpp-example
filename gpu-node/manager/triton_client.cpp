#include "triton_client.h"
#include "setting.h"
#include <cpprest/http_client.h>
#include <cpprest/uri.h>
#include <iostream>

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::client;

// Constructor for the TritonClient class
TritonClient::TritonClient() {
    Setting setting("setting.json"); // Load settings from the configuration file
    triton_address_ = setting.get("triton_address"); // Get the Triton address from the settings
}

// Send an inference request to the Triton server
json::value TritonClient::sendInferenceRequest(const std::string& model, const std::string& version, const json::value& request_json) {
    // Construct the URL for the Triton inference request
    std::string triton_url = triton_address_ + "/v2/models/" + model + "/versions/" + version + "/infer";
    http_client client(utility::conversions::to_string_t(triton_address_)); // Create an HTTP client
    http_request request(methods::POST); // Create an HTTP POST request
    request.headers().add(U("Content-Type"), U("application/json")); // Set the Content-Type header
    request.set_request_uri(utility::conversions::to_string_t("/v2/models/" + model + "/versions/" + version + "/infer")); // Set the request URI
    request.set_body(request_json); // Set the request body

    // Log the Triton URL
    cout << "Triton URL: " << triton_url << endl;

    try {
        // Send the request and get the response
        http_response response = client.request(request).get();
        if (response.status_code() == status_codes::OK) {
            return response.extract_json().get(); // Extract and return the JSON response if the request is successful
        }
        else {
            // Log the error if the request fails
            cerr << "Request failed with status code: " << response.status_code() << endl;
            return json::value();
        }
    }
    catch (const http_exception& e) {
        // Log the exception if an HTTP error occurs
        cerr << "HTTP exception occurred: " << e.what() << endl;
        return json::value();
    }
}
