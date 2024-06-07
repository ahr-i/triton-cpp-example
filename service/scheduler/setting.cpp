#include "setting.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cpprest/json.h>

using namespace std;
using namespace web;

// Constructor that initializes the settings by loading from a specified file
Setting::Setting(const std::string& file_path) {
    loadSettings(file_path);
}

// Load settings from a given file path
void Setting::loadSettings(const std::string& file_path) {
    // Open the file
    ifstream file(file_path);
    if (!file.is_open()) {
        // If the file can't be opened, throw an exception
        throw runtime_error("Could not open settings file.");
    }

    // Read the entire file into a stringstream
    stringstream buffer;
    buffer << file.rdbuf();
    file.close();  // Close the file after reading

    // Parse the JSON from the buffer
    json::value json = json::value::parse(buffer);
    // Iterate over each item in the JSON object
    for (const auto& item : json.as_object()) {
        // Convert the key and value from utility::string_t to std::string and store in the settings map
        settings_[utility::conversions::to_utf8string(item.first)] = utility::conversions::to_utf8string(item.second.as_string());
    }
}

// Retrieve the value associated with a key from the settings
std::string Setting::get(const std::string& key) const {
    // Find the key in the settings map
    auto it = settings_.find(key);
    if (it != settings_.end()) {
        // If the key is found, return the corresponding value
        return it->second;
    }
    else {
        // If the key is not found, throw an exception
        throw runtime_error("Key not found in settings: " + key);
    }
}
