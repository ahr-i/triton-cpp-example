#include "scheduler.h"
#include "node_manager.h"
#include "request_handler.h"
#include <cpprest/uri.h>
#include <cpprest/http_client.h>
#include <iostream>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
using namespace std;

// Constructor for Scheduler class
Scheduler::Scheduler(const std::string& address, const std::string& health_check_url)
    : listener_(web::uri(utility::conversions::to_string_t(address))),
    node_manager_(std::make_shared<NodeManager>(health_check_url)),
    request_handler_(std::make_shared<RequestHandler>()),
    request_count_(0) {
    listener_.support(methods::GET, std::bind(&Scheduler::handleGet, this, std::placeholders::_1));
}

// Start the scheduler server
void Scheduler::start() {
    try {
        listener_.open().wait();
        cout << "Scheduler server started at " << utility::conversions::to_utf8string(listener_.uri().to_string()) << endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Listener failed to start: " << e.what() << std::endl;
    }
}

// Stop the scheduler server
void Scheduler::stop() {
    try {
        listener_.close().wait();
    }
    catch (const std::exception& e) {
        std::cerr << "Listener failed to stop: " << e.what() << std::endl;
    }
}

// Handle GET requests
void Scheduler::handleGet(http_request request) {
    // Fetch node list from the health-check server
    std::vector<std::string> node_list = node_manager_->getNodeList();

    if (node_list.empty()) {
        request.reply(status_codes::ServiceUnavailable, U("No available nodes"));
        return;
    }

    // Select node using round-robin approach
    int node_index = request_count_ % node_list.size();
    request_count_++;

    cout << request_count_ << " - " << node_list[node_index] << '\n';

    json::value response_json;
    response_json[U("address")] = json::value::string(utility::conversions::to_string_t(node_list[node_index]));

    request.reply(status_codes::OK, response_json);
}
