#ifndef __REQUEST_HANDLER_H__
#define __REQUEST_HANDLER_H__

#include <cpprest/http_listener.h>

class RequestHandler {
public:
    RequestHandler();
    void handleRequest(web::http::http_request request);
};

#endif
