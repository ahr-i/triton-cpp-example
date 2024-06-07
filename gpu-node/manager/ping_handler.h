#ifndef __PING_HANDLER_H__
#define __PING_HANDLER_H__

#include <cpprest/http_listener.h>

class PingHandler {
public:
    PingHandler(const std::string& address);
    void start();
    void stop();

private:
    web::http::experimental::listener::http_listener listener;
    void handleGet(web::http::http_request request);
};

#endif