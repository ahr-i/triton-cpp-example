#ifndef __SERVER_H__
#define __SERVER_H__

#include <cpprest/http_listener.h>
#include <memory>
#include "ping_handler.h"

class RequestHandler;

class Server {
public:
    Server(const std::string& address);
    void start();
    void stop();

private:
    web::http::experimental::listener::http_listener listener;
    std::shared_ptr<RequestHandler> request_handler_;
    std::shared_ptr<PingHandler> ping_handler_;
};

#endif