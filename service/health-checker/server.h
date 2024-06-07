#ifndef __SERVER_H__
#define __SERVER_H__

#include <cpprest/http_listener.h>
#include <memory>
#include <string>

class HealthChecker;

class Server {
public:
    Server(const std::string& address, int interval);
    void start();
    void stop();
    std::vector<std::string> getAliveNodes() const;

private:
    void handlePost(web::http::http_request request);

    web::http::experimental::listener::http_listener listener;
    std::shared_ptr<HealthChecker> health_checker_;
};

#endif
