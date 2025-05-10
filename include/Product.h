#ifndef PRODUCT_H
#define PRODUCT_H

#include <iostream>
#include <string>
#include "Category.h"
using namespace std;

class Category;

class Product
{
private:
    int catID;
    int ID;
    int Availability;
    double Price;

    string Name;
    string Description;

    Category* CategoryType;


public:
    Product();
    Product(int id, string& name, string desc, int avail, double price, Category* cat);
    Product(int id, const std::string& name, std::string desc, int avail, double price, Category* category);
    //~Product();

    void viewProduct() const;
    void addToCart();
    void removeFromCart();
    void enterProducts();

    void setProductID(int id);
    void setCategory(Category* cat) { CategoryType = cat; }
    void setName(const string& name) { Name = name; }
    void setDescription(const string& desc) { Description = desc; }
    void setAvailability(int avail) { Availability = avail; }
    void setPrice(double price) { Price = price; }

    int getProductID() const;
    int getAvailability() const;
    double getPrice() const;
    const string& getDescription() const { return Description; }

    string getName() const ;

    Category* getCategory() const { return CategoryType; }

};

#endif // PRODUCT_H
