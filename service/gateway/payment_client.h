#ifndef PAYMENT_CLIENT_H
#define PAYMENT_CLIENT_H

#include <cpprest/json.h>
#include <string>

class PaymentClient {
public:
    PaymentClient(const std::string& address);
    void sendPaymentRequest() const;

private:
    std::string address_;
};

#endif // PAYMENT_CLIENT_H
