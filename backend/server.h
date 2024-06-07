#ifndef SERVER_H
#define SERVER_H

#include <cpprest/http_listener.h>
#include <string>

class Server {
public:
    Server(const std::string& address);
    void start();
    void stop();

private:
    void handleGet(web::http::http_request request);

    web::http::experimental::listener::http_listener listener_;
};

#endif // SERVER_H
