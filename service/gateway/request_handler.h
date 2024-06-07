#ifndef GATEWAY_REQUEST_HANDLER_H
#define GATEWAY_REQUEST_HANDLER_H

#include <cpprest/http_listener.h>
#include <memory>
#include "scheduler_client.h"
#include "payment_client.h"

class GatewayRequestHandler {
public:
    GatewayRequestHandler(const std::string& scheduler_address);
    void handlePost(web::http::http_request request);

private:
    std::shared_ptr<SchedulerClient> scheduler_client_;
    std::shared_ptr<PaymentClient> payment_client_; // Add payment client member
};

#endif // GATEWAY_REQUEST_HANDLER_H
