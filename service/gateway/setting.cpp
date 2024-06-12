#include "setting.h"
#include <cpprest/json.h>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace web;
using namespace std;

// Constructor for the Setting class, loads settings from the specified file
Setting::Setting(const std::string& filename) {
    loadSettings(filename);
}

// Function to load settings from a JSON file
void Setting::loadSettings(const std::string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open the file: " << filename << endl;
        return;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    // Parse the JSON content
    json::value json = json::value::parse(buffer.str());

    // Extract "server_port" from the JSON object
    if (json.has_field(U("server_address")) && json.at(U("server_address")).is_string()) {
        settings["server_address"] = utility::conversions::to_utf8string(json.at(U("server_address")).as_string());
    }

    // Extract "scheduler_address" from the JSON object
    if (json.has_field(U("scheduler_address")) && json.at(U("scheduler_address")).is_string()) {
        settings["scheduler_address"] = utility::conversions::to_utf8string(json.at(U("scheduler_address")).as_string());
    }

    // Extract "payment_address" from the JSON object
    if (json.has_field(U("payment_address")) && json.at(U("payment_address")).is_string()) {
        settings["payment_address"] = utility::conversions::to_utf8string(json.at(U("payment_address")).as_string());
    }
}

// Function to get a setting value by key
std::string Setting::get(const std::string& key) const {
    auto it = settings.find(key);
    if (it != settings.end()) {
        return it->second;
    }
    else {
        return "";
    }
}
