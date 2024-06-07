#include "setting.h"
#include <cpprest/json.h>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace web;
using namespace std;

// Constructor for the Setting class
Setting::Setting(const std::string& filename) {
    loadSettings(filename);
}

// Load settings from a JSON file
void Setting::loadSettings(const std::string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open the file: " << filename << endl;
        return;
    }

    stringstream buffer;
    buffer << file.rdbuf(); // Read the file content into the buffer
    file.close();

    json::value json = json::value::parse(buffer.str()); // Parse the JSON content

    // Extract the server port from the JSON content
    if (json.has_field(U("server_port")) && json.at(U("server_port")).is_string()) {
        settings["server_port"] = utility::conversions::to_utf8string(json.at(U("server_port")).as_string());
    }

    // Extract the Triton address from the JSON content
    if (json.has_field(U("triton_address")) && json.at(U("triton_address")).is_string()) {
        settings["triton_address"] = utility::conversions::to_utf8string(json.at(U("triton_address")).as_string());
    }

    // Extract the health check URL from the JSON content
    if (json.has_field(U("health_check_url")) && json.at(U("health_check_url")).is_string()) {
        settings["health_check_url"] = utility::conversions::to_utf8string(json.at(U("health_check_url")).as_string());
    }
}

// Get a setting value by key
std::string Setting::get(const std::string& key) const {
    auto it = settings.find(key); // Find the key in the settings map
    if (it != settings.end()) {
        return it->second; // Return the value if the key is found
    }
    else {
        return ""; // Return an empty string if the key is not found
    }
}
