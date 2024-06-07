#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <string>

class WebServer {
public:
    WebServer(const std::string& address, const std::string& gateway_address);
    void start();
    void stop();

private:
    void handleGenerate(web::http::http_request request);
    void handleGet(web::http::http_request request);

    web::http::experimental::listener::http_listener listener_;
    std::string gateway_address_;
};

#endif // WEBSERVER_H
