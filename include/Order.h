#ifndef ORDER_H
#define ORDER_H

#include <iostream>
#include <string>
#include <vector>
#include "Customer.h"
#include "orderItems.h"
#include "Shipping.h"
#include "Payment.h"

using namespace std;

class Payment;
class Shipping;

class Order
{
public:
    Order();

    Order(int id, Customer* customer, vector<orderItems> items, Shipping shipping, Payment* payment, string stat, double price);
    virtual ~Order();

    void confirmOrder();
    void cancelOrder();
    void showOrderDetails();

    double getTotalPrice() const;

    int getID() const { return ID; }

    Customer* getCustomer() const { return customerInfo; }
    Shipping getShipping() const
    {
        return shippingInfo;
    }
    Payment* getPayment() const
    {
        return paymentMethod;
    }
    vector<orderItems> getItems() const
    {
        return itemInfo;
    }

    const string& getStatus() const { return status; }

private:
    int ID;
    double totalPrice;

    string status;

    vector<orderItems> itemInfo;

    Customer* customerInfo;
    Shipping shippingInfo;
    Payment* paymentMethod;
};

#endif // ORDER_H
