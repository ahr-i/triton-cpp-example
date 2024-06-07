#ifndef GATEWAY_SERVER_H
#define GATEWAY_SERVER_H

#include <cpprest/http_listener.h>
#include <memory>
#include "gateway_request_handler.h"

class GatewayServer {
public:
    GatewayServer(const std::string& address, const std::string& scheduler_address, const std::string& payment_address);
    void start();
    void stop();

private:
    web::http::experimental::listener::http_listener listener_;
    std::shared_ptr<GatewayRequestHandler> request_handler_;
};

#endif // GATEWAY_SERVER_H
