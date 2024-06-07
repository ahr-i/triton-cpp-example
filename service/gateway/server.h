#ifndef __SERVER_H__
#define __SERVER_H__

#include <cpprest/http_listener.h>
#include <memory>
#include <string>

class GatewayRequestHandler; // forward declaration

class Server {
public:
    Server(const std::string& address, std::shared_ptr<GatewayRequestHandler> request_handler);
    virtual ~Server() = default;
    void start();
    void stop();

protected:
    web::http::experimental::listener::http_listener listener_;
    std::shared_ptr<GatewayRequestHandler> request_handler_;
};

#endif
