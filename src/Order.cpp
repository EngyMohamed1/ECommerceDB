#include "Order.h"


Order::Order()
{
    ID = 0, customerInfo = nullptr, status= "Pending", totalPrice = 0.0;
}

Order::Order(int id, Customer* customer, vector<orderItems> items, Shipping shipping, Payment* payment, string stat, double price)
{
    ID = id;
    customerInfo = customer;
    itemInfo= items;
    shippingInfo = shipping;
    paymentMethod = payment;
    status = stat;
    totalPrice = price;
}
Order::~Order()
{ }

void Order::confirmOrder()
{
    status = "Confirmed";
    cout << "Order " << ID << " confirmed.\n";
}

void Order::cancelOrder()
{
    status = "Cancelled";
    cout << "Order " << ID << " cancelled.\n";
}

void Order::showOrderDetails()
{
    cout << "Order ID: " << ID << "\n"
         << "Status: " << status << "\n"
         << "Total Price: " << totalPrice << "\n";
}

double Order::getTotalPrice() const
{

    return totalPrice;
}
