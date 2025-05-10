#include "Cart.h"
#include "Customer.h"

#include <iostream>
#include <vector>

using namespace std;

Cart::Cart()
{
    ID = 0;
    customer = nullptr;    // Initialize with null if no customer
}
Cart::Cart(const Customer* customer)
{
    customer = customer;
};
Cart::Cart(int id, const Customer* cust)
{
    ID = id;
    customer = cust;
}
void Cart::addToCart(const Product& product) {
    cout << "addToCart called for product ID: " << product.getProductID() << endl;
    for (auto& item : Items) {
        if (item.getProduct().getProductID() == product.getProductID()) {
            if (item.getQuantity() >= product.getAvailability()) {
                cout << "Cannot add more. Only " << product.getAvailability() << " available.\n";
                return;
            }
            item.increaseQuantity();
            cout << "Increased quantity of " << product.getName() << "\n";
            return;
        }
    }
    if (product.getAvailability() > 0) {
        Items.push_back(cartItems(product, 1));
        cout << product.getName() << " added to cart!" << endl;
    } else {
        cout << "Product not available to add to cart.\n";
    }
}
void Cart::removeFromCart(const Product& product)
{
    for (vector<cartItems>::iterator it = Items.begin(); it != Items.end(); ++it)
    {
        if (it->getProduct().getProductID() == product.getProductID())
        {
            if (it->getQuantity() == 0)
            {
                cout << "No quantity of " << product.getName() << " to remove.\n";
                return;
            }

            it->decreaseQuantity();

            if (it->getQuantity() == 0)
            {
                Items.erase(it);
                cout << product.getName() << " removed from cart.\n";
            }
            else
            {
                cout << "Reduced quantity of " << product.getName() << " to " << it->getQuantity() << ".\n";
            }
            return;
        }
    }
    cout << product.getName() << " not found in cart.\n";
}


void Cart::viewItems() const
{
    cout << "Cart contains:\n";
    for (vector<cartItems>::const_iterator item = Items.begin(); item != Items.end(); ++item)
    {
        cout << "- " << item->getProduct().getName() << " (x" << item->getQuantity() << ")\n";
    }
}

void Cart::checkout()
{
    cout << "Checking out...\n";
}

double Cart::getTotalPrice() const
{
    double total = 0.0;
    for (const cartItems& item : Items)
    {
        total += item.getProduct().getPrice() * item.getQuantity();
    }
    return total;
}

const Customer* Cart::getCustomer() const
{
    return customer;
}
bool Cart::isEmpty() const { return Items.empty(); }
void Cart::clear() { Items.clear(); }
vector<cartItems> Cart::getItems() const { return Items; }
