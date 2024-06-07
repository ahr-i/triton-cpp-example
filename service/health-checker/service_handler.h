#ifndef __SERVICE_HANDLER_H__
#define __SERVICE_HANDLER_H__

#include <cpprest/http_listener.h>
#include <vector>
#include <string>
#include <mutex>

class ServiceHandler {
public:
    ServiceHandler(std::vector<std::string>& alive_nodes, std::mutex& mutex);
    void handleGet(web::http::http_request request);

private:
    std::vector<std::string>& alive_nodes_;
    std::mutex& mutex_;
};

#endif
