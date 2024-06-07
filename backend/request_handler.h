#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <cpprest/http_listener.h>
#include <cpprest/http_msg.h>

class RequestHandler {
public:
    void handlePost(web::http::http_request request);
};

#endif // REQUEST_HANDLER_H
