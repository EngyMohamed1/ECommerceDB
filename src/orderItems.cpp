#include "orderItems.h"


orderItems::orderItems()
{
    ID = 0;
    totalPrice = 0;
    product = nullptr;
}
orderItems::orderItems(int id, int num, const Product* prod, double price)
{
    ID = id;
    numItems = num;
    product = prod;
    totalPrice = price;

}

orderItems::~orderItems()
{ }

void orderItems::showOrderItems()
{
    cout << "Order Item ID: " << ID << "\n"
         << "Number of Items: " << numItems << "\n"
         << "Price of Item: " << totalPrice << "\n";
}

double orderItems::getTotalPrice() const
{
    return totalPrice;
}
