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
    buffer << file.rdbuf();
    file.close();

    // Parse the JSON content from the file
    json::value json = json::value::parse(buffer.str());

    // Store the settings if they are found in the JSON
    if (json.has_field(U("server_port")) && json.at(U("server_port")).is_string()) {
        settings["server_port"] = utility::conversions::to_utf8string(json.at(U("server_port")).as_string());
    }

    if (json.has_field(U("gateway_address")) && json.at(U("gateway_address")).is_string()) {
        settings["gateway_address"] = utility::conversions::to_utf8string(json.at(U("gateway_address")).as_string());
    }
}

// Get the value for a given key from the settings
std::string Setting::get(const std::string& key) const {
    auto it = settings.find(key);
    if (it != settings.end()) {
        return it->second;
    }
    else {
        return "";
    }
}
