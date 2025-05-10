#ifndef PAYMENT_H
#define PAYMENT_H

#include <iostream>
#include <string>

#include "Address.h"
#include "Customer.h"
#include "Order.h"
#include "Product.h"

using namespace std;

class Order;

class Payment
{
public:
    Payment();
    virtual ~Payment();

    void processPayment();
    void printPayment();

    void setPayID(int id) { payID = id; }
    void setMethod(const std::string& method) { Method = method; }
    void setPaymentStatus(const std::string& status) { paymentStatus = status; }
    void setCardNumber(const std::string& number) { cardNumber = number; }
    void setExpirationDate(const std::string& date) { expirationDate = date; }
    void setCvv(const std::string& cvvCode) { cvv = cvvCode; }
    void setCardholderName(const std::string& name) { cardholderName = name; }

    int getPayID() const { return payID; }
    const std::string& getMethod() const { return Method; }
    const std::string& getPaymentStatus() const { return paymentStatus; }
    const std::string& getCardNumber() const { return cardNumber; }
    const std::string& getExpirationDate() const { return expirationDate; }
    const std::string& getCvv() const { return cvv; }
    const std::string& getCardholderName() const { return cardholderName; }

private:
    int payID;
    string Method;
    string paymentStatus;
    string cardNumber;
    string expirationDate;
    string cvv;
    string cardholderName;

    Order* orderInfo;
    Product* prodInfo;
};

#endif // PAYMENT_H
