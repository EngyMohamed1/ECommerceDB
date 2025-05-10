#include "Category.h"

Category::Category() { }
Category::Category(int id, string name, string desc) : ID(id), Name(name), Description(desc) {}

void Category::addProduct(Product* product)
{
    Products.push_back(product);
}

void Category::setCategory()
{
    cout << " Category ID: ";
    cin >> ID;
    cin.ignore();

    cout << " Category Name: ";
    getline(cin, Name);

    cout << " Category Description: ";
    getline(cin, Description);
}

void Category::setNameCategory()
{
    cout << " Category Name: ";
    cin >> Name;
}

void Category::setIDCategory()
{
    cout << " Category ID: ";
    cin >> ID;
}

void Category::viewProducts() const
{
    cout << "Category: " << Name << " contains:\n";
    if (Products.empty())
    {
        cout << "No products available in this category.\n";
    }
    else
    {
        for (vector<Product*>::const_iterator it = Products.begin(); it != Products.end(); ++it)
        {
            cout << "- " << (*it)->getName() << " (Price: $" << (*it)->getPrice() << ")\n";
        }
    }
}

void Category::viewCategory()
{
    cout << "\n\nCategory: " << Name << "\nDescription: " << Description<< "\nID: " << ID << endl;;
}
