#include "web_server.h"
#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cpprest/uri.h>
#include <cpprest/base_uri.h>
#include <cpprest/uri_builder.h>
#include <cpprest/http_listener.h>
#include <cpprest/asyncrt_utils.h>
#include <cpprest/streams.h>
#include <iterator>
#include <iomanip>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::experimental::listener;
using namespace std;

// WebServer constructor: initializes the HTTP listener and sets up route handlers
WebServer::WebServer(const std::string& address, const std::string& gateway_address)
    : listener_(uri(utility::conversions::to_string_t(address))), gateway_address_(gateway_address) {
    // Bind the POST method to the handleGenerate function
    listener_.support(methods::POST, std::bind(&WebServer::handleGenerate, this, std::placeholders::_1));
    // Bind the GET method to the handleGet function
    listener_.support(methods::GET, std::bind(&WebServer::handleGet, this, std::placeholders::_1));
}

// Start the web server
void WebServer::start() {
    // Open the listener and wait for it to be ready
    listener_.open().wait();
    std::cout << "Server started at " << utility::conversions::to_utf8string(listener_.uri().to_string()) << std::endl;
}

// Stop the web server
void WebServer::stop() {
    // Close the listener and wait for it to be closed
    listener_.close().wait();
}

// Base64 encode a vector of unsigned char data
std::string base64_encode(const std::vector<unsigned char>& data) {
    static const char base64_chars[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

    std::string result;
    int val = 0;
    int valb = -6;

    for (unsigned char c : data) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            result.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) {
        result.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
    }
    while (result.size() % 4) {
        result.push_back('=');
    }
    return result;
}

// Handle POST requests for generating images
void WebServer::handleGenerate(http_request request) {
    // Check if the content type is application/json
    if (request.headers().content_type() != U("application/json")) {
        request.reply(status_codes::UnsupportedMediaType, U("Content-Type must be application/json"));
        return;
    }

    // Log the request body for debugging
    std::cout << "Request Body: " << utility::conversions::to_utf8string(request.to_string()) << std::endl;

    // Extract the JSON body from the request
    request.extract_json().then([=](json::value request_json) {
        try {
            // Extract the text field from the JSON request
            utility::string_t text = request_json[U("text")].as_string();
            int seed = rand() % 1000; // Generate a random seed for the image generation

            // Create a JSON request body for the inference server
            json::value json_request;
            json_request[U("inputs")][0][U("name")] = json::value::string(U("PROMPT"));
            json_request[U("inputs")][0][U("shape")] = json::value::array({ json::value::number(1) });
            json_request[U("inputs")][0][U("datatype")] = json::value::string(U("BYTES"));
            json_request[U("inputs")][0][U("data")] = json::value::array({ json::value::string(text) });

            json_request[U("inputs")][1][U("name")] = json::value::string(U("SAMPLES"));
            json_request[U("inputs")][1][U("shape")] = json::value::array({ json::value::number(1) });
            json_request[U("inputs")][1][U("datatype")] = json::value::string(U("INT32"));
            json_request[U("inputs")][1][U("data")] = json::value::array({ json::value::number(1) });

            json_request[U("inputs")][2][U("name")] = json::value::string(U("STEPS"));
            json_request[U("inputs")][2][U("shape")] = json::value::array({ json::value::number(1) });
            json_request[U("inputs")][2][U("datatype")] = json::value::string(U("INT32"));
            json_request[U("inputs")][2][U("data")] = json::value::array({ json::value::number(45) });

            json_request[U("inputs")][3][U("name")] = json::value::string(U("GUIDANCE_SCALE"));
            json_request[U("inputs")][3][U("shape")] = json::value::array({ json::value::number(1) });
            json_request[U("inputs")][3][U("datatype")] = json::value::string(U("FP32"));
            json_request[U("inputs")][3][U("data")] = json::value::array({ json::value::number(7.5) });

            json_request[U("inputs")][4][U("name")] = json::value::string(U("SEED"));
            json_request[U("inputs")][4][U("shape")] = json::value::array({ json::value::number(1) });
            json_request[U("inputs")][4][U("datatype")] = json::value::string(U("INT64"));
            json_request[U("inputs")][4][U("data")] = json::value::array({ json::value::number(seed) });

            json_request[U("outputs")][0][U("name")] = json::value::string(U("IMAGES"));
            json_request[U("outputs")][0][U("binary_data")] = json::value::boolean(false);

            // Build the complete URI for the inference request
            utility::string_t gateway_address_t = utility::conversions::to_string_t(gateway_address_);
            uri_builder builder(gateway_address_t);
            builder.append_path(U("models/stable_diffusion/1/inference"));

            uri complete_uri = builder.to_uri();
            std::cout << "Request URI: " << utility::conversions::to_utf8string(complete_uri.to_string()) << std::endl;

            // Send the inference request to the gateway server
            http_client client(complete_uri);
            http_request inference_request(methods::POST);
            inference_request.headers().add(U("Content-Type"), U("application/json"));
            inference_request.set_body(json_request);

            client.request(inference_request).then([=](http_response response) {
                if (response.status_code() == status_codes::OK) {
                    response.extract_json().then([=](json::value inference_response) {
                        try {
                            // Extract the image data from the response
                            auto outputs = inference_response[U("outputs")][0][U("data")].as_array();
                            std::vector<float> image_data;

                            for (auto& val : outputs) {
                                image_data.push_back(static_cast<float>(val.as_double()));
                            }

                            // Convert the image data to RGB format
                            int width = 512;
                            int height = 512;
                            std::vector<unsigned char> rgba_data(width * height * 3);

                            for (size_t i = 0; i < image_data.size(); i += 3) {
                                size_t index = i;
                                rgba_data[index] = static_cast<unsigned char>(image_data[i] * 255);
                                rgba_data[index + 1] = static_cast<unsigned char>(image_data[i + 1] * 255);
                                rgba_data[index + 2] = static_cast<unsigned char>(image_data[i + 2] * 255);
                            }

                            // Encode the image data to JPG format
                            std::vector<unsigned char> jpg_data;
                            stbi_write_jpg_to_func(
                                [](void* context, void* data, int size) {
                                    auto& jpg_data = *reinterpret_cast<std::vector<unsigned char>*>(context);
                                    jpg_data.insert(jpg_data.end(), (unsigned char*)data, (unsigned char*)data + size);
                                },
                                &jpg_data, width, height, 3, rgba_data.data(), 100
                            );

                            // Base64 encode the JPG image data
                            std::string base64_image = base64_encode(jpg_data);

                            // Create a JSON response with the Base64 encoded image
                            json::value json_response;
                            json_response[U("image_base64")] = json::value::string(utility::conversions::to_string_t(base64_image));

                            // Send the JSON response back to the client
                            request.reply(status_codes::OK, json_response);
                        }
                        catch (const std::exception& e) {
                            request.reply(status_codes::InternalError, U("Exception occurred while processing response: ") + utility::conversions::to_string_t(e.what()));
                        }
                        }).wait();
                }
                else {
                    request.reply(status_codes::InternalError, U("Failed to generate image."));
                }
                }).wait();
        }
        catch (const std::exception& e) {
            request.reply(status_codes::InternalError, U("Exception occurred: ") + utility::conversions::to_string_t(e.what()));
        }
        }).wait();
}

// Handle GET requests for serving files
void WebServer::handleGet(http_request request) {
    // Decode the requested path
    utility::string_t path = uri::decode(request.relative_uri().path());
    if (path == U("/")) {
        path = U("/index.html");
    }

    // Convert utility::string_t to std::string
    std::string full_path = "." + utility::conversions::to_utf8string(path);
    std::ifstream file(full_path, std::ios::binary);

    // Check if the file exists
    if (!file) {
        request.reply(status_codes::NotFound, U("File not found"));
        return;
    }

    // Read the file content into a string
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    std::string content = buffer.str();
    std::string content_type = "text/html";
    if (full_path.rfind(".css") == full_path.length() - 4) {
        content_type = "text/css";
    }
    else if (full_path.rfind(".js") == full_path.length() - 3) {
        content_type = "application/javascript";
    }
    // Send the file content as the response
    request.reply(status_codes::OK, content, content_type);
}
