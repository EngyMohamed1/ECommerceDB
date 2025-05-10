#ifndef CARTITEMS_H
#define CARTITEMS_H

#include <iostream>
#include <vector>

#include "Product.h"
using namespace std;

class cartItems
{
private:
    int ID;
    int quantity;

    Product products;

public:
    cartItems();
    //cartItems(int id, Product prod, int qty);
    cartItems(const Product& product, int quantity);

    void increaseQuantity();
    void decreaseQuantity();

    int getQuantity() const;
    int getCartItemsID() const;
    double getProductPrice() const;

    const Product& getProduct() const;

};

#endif // CARTITEMS_H
