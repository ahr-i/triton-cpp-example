#ifndef __HEALTH_CHECKER_H__
#define __HEALTH_CHECKER_H__

#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include "service_handler.h"

class HealthChecker {
public:
    HealthChecker(int interval, web::http::experimental::listener::http_listener& listener);
    ~HealthChecker();
    void start();
    void stop();
    std::vector<std::string> getAliveNodes() const;
    void handlePost(web::http::http_request request);

private:
    void checkHealth();

    std::vector<std::string> nodes_;
    std::vector<std::string> alive_nodes_;
    int interval_;
    std::thread checker_thread_;
    std::atomic<bool> running_;
    mutable std::mutex mutex_;
    web::http::experimental::listener::http_listener& listener_;
    std::shared_ptr<ServiceHandler> service_handler_;
};

#endif
