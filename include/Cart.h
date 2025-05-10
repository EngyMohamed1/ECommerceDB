#ifndef CART_H
#define CART_H

#include <iostream>
#include <vector>

#include "cartItems.h"

using namespace std;

class Customer;
class cartItems;

class Cart
{
private:
    int ID;

    vector<cartItems> Items;

    const Customer* customer;

public:
    Cart();
    Cart(int id, const Customer* cust);
    Cart(const Customer* customer);

    void addToCart(const Product& product);
    void removeFromCart(const Product& product);
    void viewItems() const;
    void checkout();

    double getTotalPrice() const;
    const Customer* getCustomer() const;
    bool isEmpty() const;
    void clear();
    vector<cartItems> getItems() const;
};

#endif // CART_H
