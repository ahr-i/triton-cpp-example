#ifndef __TRITON_CLIENT_H__
#define __TRITON_CLIENT_H__

#include <cpprest/json.h>
#include <string>

class TritonClient {
public:
    TritonClient();
    web::json::value sendInferenceRequest(const std::string& model, const std::string& version, const web::json::value& request_json);

private:
    std::string triton_address_;
};

#endif