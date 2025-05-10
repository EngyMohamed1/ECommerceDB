#ifndef ORDERITEMS_H
#define ORDERITEMS_H

#include <iostream>
#include <string>
#include <vector>

#include "Product.h"
using namespace std;

class orderItems
{
public:
    orderItems();
    orderItems(int id, int num, const Product* prod, double price);

    virtual ~orderItems();

    void showOrderItems();

    int getID() const { return ID; }
    int getOrderID() const { return ID; }
    int getQuantity() const { return numItems; }
    const Product* getProduct() const { return product; }

    void setOrderID(int id) { ID = id; }
    double getTotalPrice() const;

private:
    int ID;
    int numItems;
    double totalPrice;

    const Product* product;

};

#endif // ORDERITEMS_H
