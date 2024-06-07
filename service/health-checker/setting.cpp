#include "setting.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cpprest/json.h>

using namespace std;
using namespace web;

// Constructor for the Setting class that loads settings from the given file path
Setting::Setting(const std::string& file_path) {
    loadSettings(file_path);
}

// Function to load settings from a JSON file
void Setting::loadSettings(const std::string& file_path) {
    ifstream file(file_path);
    if (!file.is_open()) {
        throw runtime_error("Could not open settings file.");
    }

    stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    json::value json = json::value::parse(buffer);
    // Iterate through the JSON object and store each key-value pair in the settings_ map
    for (const auto& item : json.as_object()) {
        settings_[utility::conversions::to_utf8string(item.first)] = utility::conversions::to_utf8string(item.second.as_string());
    }
}

// Function to retrieve a setting by key
std::string Setting::get(const std::string& key) const {
    auto it = settings_.find(key);
    if (it != settings_.end()) {
        return it->second;
    }
    else {
        throw runtime_error("Key not found in settings: " + key);
    }
}
