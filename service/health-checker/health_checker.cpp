#include "health_checker.h"
#include <cpprest/http_client.h>
#include <cpprest/uri_builder.h>
#include <iostream>
#include <chrono>

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::experimental::listener;
using namespace std;

// Constructor for HealthChecker
HealthChecker::HealthChecker(int interval, http_listener& listener)
    : interval_(interval), running_(false), listener_(listener) {
    cout << "HealthChecker initialized" << endl;
    service_handler_ = make_shared<ServiceHandler>(alive_nodes_, mutex_);
    listener_.support(methods::GET, bind(&ServiceHandler::handleGet, service_handler_.get(), placeholders::_1));
}

// Destructor for HealthChecker
HealthChecker::~HealthChecker() {
    stop(); // Ensure the health checker is stopped before destruction
    cout << "HealthChecker destroyed" << endl;
}

// Start the health checker
void HealthChecker::start() {
    if (!running_) {
        running_ = true;
        checker_thread_ = thread(&HealthChecker::checkHealth, this); // Launch the health checking thread
        cout << "HealthChecker started." << endl;
    }
}

// Stop the health checker
void HealthChecker::stop() {
    if (running_) {
        running_ = false;
        if (checker_thread_.joinable()) {
            checker_thread_.join(); // Wait for the health checking thread to finish
        }
        cout << "HealthChecker stopped." << endl;
    }
}

// Get the list of alive nodes
vector<string> HealthChecker::getAliveNodes() const {
    lock_guard<mutex> lock(mutex_);
    return alive_nodes_;
}

// Handle POST requests to register nodes
void HealthChecker::handlePost(http_request request) {
    request.extract_json().then([this, request](pplx::task<json::value> task) {
        try {
            json::value json = task.get();
            if (json.has_field(U("port")) && json.at(U("port")).is_string()) {
                string port = utility::conversions::to_utf8string(json.at(U("port")).as_string());

                // Get the IP address of the request
                utility::string_t ip_address = request.remote_address();

                // Construct the node address
                string node = "http://" + utility::conversions::to_utf8string(ip_address) + ":" + port;
                {
                    lock_guard<mutex> lock(mutex_);
                    nodes_.push_back(node); // Add the node to the list
                }
                request.reply(status_codes::OK, U("Node registered"));
                cout << "Node registered: " << node << endl;
            }
            else {
                request.reply(status_codes::BadRequest, U("Invalid JSON"));
                cout << "Invalid JSON received in request" << endl;
            }
        }
        catch (const http_exception& e) {
            request.reply(status_codes::BadRequest, U("Invalid JSON"));
            cout << "Exception while processing request: " << e.what() << endl;
        }
        }).wait();
}

// Check the health of registered nodes
void HealthChecker::checkHealth() {
    while (running_) {
        vector<string> new_alive_nodes;
        for (vector<string>::const_iterator it = nodes_.begin(); it != nodes_.end(); ++it) {
            try {
                uri_builder builder(utility::conversions::to_string_t(*it));
                builder.append_path(U("ping"));

                cout << "Pinging node: " << *it << endl;

                http_client client(builder.to_uri());
                http_request request(methods::GET);

                http_response response = client.request(request).get();
                if (response.status_code() == status_codes::OK) {
                    cout << "Node " << *it << " is alive" << endl;
                    new_alive_nodes.push_back(*it); // Node is alive
                }
                else {
                    cout << "Node " << *it << " is not responding with 200 OK" << endl;
                }
            }
            catch (const exception& e) {
                cerr << "Health check failed for node " << *it << ": " << e.what() << endl;
            }
        }
        {
            lock_guard<mutex> lock(mutex_);
            nodes_ = new_alive_nodes; // Update the list of nodes
            alive_nodes_ = new_alive_nodes; // Update the list of alive nodes
        }
        this_thread::sleep_for(chrono::seconds(interval_)); // Wait before the next health check
    }
}
