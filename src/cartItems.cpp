#include "cartItems.h"
#include <iostream>

cartItems::cartItems() { }
/*cartItems::cartItems(int id,const Product& prod, int qty)
{
    ID = id;
    product = prod;
    quantity = qty;
}*/

cartItems::cartItems(const Product& prod, int qty)
{
    products = prod;
    quantity = qty;
}

void cartItems::increaseQuantity()
{
    quantity++;
}

void cartItems::decreaseQuantity()
{
    if (quantity > 0)
    {
        quantity--;
    }
}

int cartItems::getQuantity() const
{
    return quantity;
}

int cartItems::getCartItemsID() const
{
    return ID;
}
const Product& cartItems::getProduct() const
{
    return products;
}
double cartItems::getProductPrice() const
{
    return products.getPrice();
}
