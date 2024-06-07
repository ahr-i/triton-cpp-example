#ifndef __REQUEST_HANDLER_H__
#define __REQUEST_HANDLER_H__

#include <cpprest/http_listener.h>
#include <memory>

class TritonClient;

class RequestHandler {
public:
    RequestHandler();
    void handlePost(web::http::http_request request);

private:
    std::shared_ptr<TritonClient> triton_client_;
    void handleModelInference(web::http::http_request request, const std::string& model, const std::string& version);
};

#endif