#include "node_manager.h"
#include <cpprest/uri_builder.h>
#include <iostream>

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace std;

// Constructor for NodeManager class, initializes health_check_url_
NodeManager::NodeManager(const std::string& health_check_url)
    : health_check_url_(health_check_url) {
}

// Public method to get the list of nodes, ensures thread safety
std::vector<std::string> NodeManager::getNodeList() {
    fetchNodeList(); // Fetch the latest node list
    std::lock_guard<std::mutex> lock(mutex_); // Lock the mutex to ensure thread safety
    return node_list_; // Return the node list
}

// Private method to fetch the list of nodes from the health check service
void NodeManager::fetchNodeList() {
    try {
        // Build the URI for the health check request
        uri_builder builder(utility::conversions::to_string_t(health_check_url_));
        builder.append_path(U("service/get/list"));

        // Create an HTTP client and send a GET request
        http_client client(builder.to_uri());
        http_request request(methods::GET);

        // Get the response and check its status code
        http_response response = client.request(request).get();
        if (response.status_code() == status_codes::OK) {
            // Extract JSON from the response
            json::value json = response.extract_json().get();
            if (json.has_field(U("service_list")) && json.at(U("service_list")).is_array()) {
                // Parse the JSON array and update the node list
                std::vector<std::string> new_node_list;
                for (auto& item : json.at(U("service_list")).as_array()) {
                    new_node_list.push_back(utility::conversions::to_utf8string(item.as_string()));
                }
                // Lock the mutex and update the node list
                std::lock_guard<std::mutex> lock(mutex_);
                node_list_ = new_node_list;
            }
        }
    }
    catch (const std::exception& e) {
        // Handle exceptions and print error message
        std::cerr << "Failed to fetch node list: " << e.what() << std::endl;
    }
}
