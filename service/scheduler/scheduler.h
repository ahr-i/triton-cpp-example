#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <cpprest/http_listener.h>
#include <memory>
#include <vector>
#include <string>
#include "node_manager.h"
#include "request_handler.h"

class Scheduler {
public:
    Scheduler(const std::string& address, const std::string& health_check_url);
    void start();
    void stop();

private:
    void handleGet(web::http::http_request request);

    web::http::experimental::listener::http_listener listener_;
    std::shared_ptr<NodeManager> node_manager_;
    std::shared_ptr<RequestHandler> request_handler_;
    int request_count_;
};

#endif
