#ifndef __SCHEDULER_CLIENT_H__
#define __SCHEDULER_CLIENT_H__

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <string>

class SchedulerClient {
public:
    SchedulerClient(const std::string& address);
    pplx::task<web::json::value> getNodeAddress() const;

private:
    web::http::client::http_client client_;
};

#endif
