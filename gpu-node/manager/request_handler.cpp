#include "request_handler.h"
#include "triton_client.h"
#include <iostream>

using namespace std;
using namespace web;
using namespace web::http;

// Constructor to initialize the RequestHandler with a TritonClient instance
RequestHandler::RequestHandler()
    : triton_client_(make_shared<TritonClient>()) {}

// Function to handle POST requests
void RequestHandler::handlePost(http_request request) {
    // Split the request URI path
    std::vector<utility::string_t> paths = uri::split_path(uri::decode(request.relative_uri().path()));

    // Check if the request is for model inference
    if (paths.size() == 4 && paths[0] == U("models") && paths[3] == U("inference")) {
        // Extract model name and version from the URI
        std::string model = utility::conversions::to_utf8string(paths[1]);
        std::string version = utility::conversions::to_utf8string(paths[2]);

        // Log model and version
        cout << "Model: " << model << ", Version: " << version << endl;

        // Handle model inference
        handleModelInference(request, model, version);
    }
    else {
        // Reply with 404 Not Found if the request is not recognized
        request.reply(status_codes::NotFound, U("Unknown request"));
    }
}

// Function to handle model inference
void RequestHandler::handleModelInference(http_request request, const std::string& model, const std::string& version) {
    // Extract JSON from the request
    request.extract_json().then([this, request, model, version](pplx::task<json::value> task) {
        try {
            // Get the JSON from the task
            json::value json = task.get();

            // Log the request body
            cout << "Request Body: " << utility::conversions::to_utf8string(json.serialize()) << endl;

            // Send inference request to TritonClient and get the response
            json::value response = triton_client_->sendInferenceRequest(model, version, json);
            // Reply with the inference response
            request.reply(status_codes::OK, response);
        }
        catch (const http_exception& e) {
            // Reply with 400 Bad Request if there is an exception
            request.reply(status_codes::BadRequest, U("Invalid JSON"));
        }
        }).wait();
}
