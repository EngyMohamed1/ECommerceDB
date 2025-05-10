#include "Product.h"
#include <iostream>

Product::Product()
{ }
Product::Product(int id, string& name, string desc, int avail, double price, Category* category)
{
    ID = id;
    Name = name;
    Description = desc;
    Availability = avail;
    Price = price;
    CategoryType = category;
}
Product::Product(int id, const std::string& name, std::string desc, int avail, double price, Category* category)
{
    ID = id;
    Name = name;
    Description = desc;
    Availability = avail;
    Price = price;
    CategoryType = category;
}
/*
Product::~Product() {
    delete CategoryType;
}
*/
void Product::viewProduct() const
{
    cout << "\n\nProduct: " << Name
         << "\nDescription: " << Description
         << "\nPrice: $" << Price
         << "\nAvailable: ";

    if (Availability == 0)
        cout << "Not Available\n";
    else
        cout << Availability << " units\n";

    CategoryType->viewCategory();
}


void Product::enterProducts()
{
  //  cout << " Product ID: ";
  //  cin >> ID;
    cin.ignore();

    cout << " Product Name: ";
    getline(cin, Name);

    cout << " Product Description: ";
    getline(cin, Description);

    cout << " Product Price: ";
    cin >> Price;

    cout << " Availability: ";
    cin >> Availability;

}
void Product::addToCart()
{
    cout << Name << " added to cart.\n";
}

void Product::removeFromCart()
{
    cout << Name << " removed from cart.\n";
}

void Product::setProductID(int id) {
    this->ID = id;
}

int Product::getProductID() const
{
    return ID;
}
int Product::getAvailability() const
{
    return Availability;
}
double Product::getPrice() const
{
    return Price;
}

string Product::getName() const
{
    return Name;
}
