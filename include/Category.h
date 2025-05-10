#ifndef CATEGORY_H
#define CATEGORY_H

#include <iostream>
#include <vector>

#include "Product.h"
using namespace std;

class Product;

class Category {
private:
    int ID;

    string Name;
    string Description;
    vector<Product*> Products;

public:
    Category();
    Category(int id, string name, string desc);
    Category(const Category& other) : ID(other.ID), Name(other.Name), Description(other.Description), Products(other.Products) {}

    int getID() const { return ID; }
    string getName() const { return Name; }
    string getDescription() const { return Description; }

    void addProduct(Product* product);
    void viewProducts() const;
    void viewCategory();

    void setNameCategory();
    void setIDCategory();
    void setCategory();

    vector<Product*> getProducts() const { return Products; }

};

#endif // CATEGORY_H
