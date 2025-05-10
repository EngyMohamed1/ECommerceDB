#include "Customer.h"
#include <iostream>

using namespace std;

Customer::Customer()
{
    custID = 0;
    Age = 0;
}

Customer::Customer(const string& name, int id)
{
    Name = name;
    custID = id;
}

Customer::Customer(int id, const std::string& name, const std::string& email, const std::string& password)
{
    custID = id;
    Name = name;
    EMail = email;
    Password = password;
}

Customer::~Customer() {}

const Address& Customer::getAddress() const
{
    return customerAddress;
}
void Customer::setAddress(const Address& address)
{
    customerAddress = address;
}

Customer Customer::Register() {
    string name, email, password, gender, phoneNumber;
    int age;

    cout << "Enter your name: ";
    cin.ignore();
    getline(cin, name);

    cout << "Enter your email: ";
    getline(cin, email);

    cout << "Enter your password: ";
    getline(cin, password);

    cout << "Enter your age: ";
    cin >> age;
    cin.ignore();

    cout << "Enter your gender: ";
    getline(cin, gender);

    cout << "Enter your phone number: ";
    getline(cin, phoneNumber);

    cout << "Enter your address:\n";
    Address address;
    address.setAddress();

    Customer newCustomer;
    DatabaseManager db("ecommerce.db");
    newCustomer.custID = db.getNextCustomerId();
    newCustomer.Name = name;
    newCustomer.EMail = email;
    newCustomer.Password = password;
    newCustomer.Age = age;
    newCustomer.Gender = gender;
    newCustomer.phoneNumber = phoneNumber;
    newCustomer.customerAddress = address;
    newCustomer.customerCart = Cart(&newCustomer);

    cout << "Customer registered successfully with ID: " << newCustomer.custID << endl;
    return newCustomer;
}

void Customer::Register(std::vector<Customer>& customers) {
    string name, email, password, gender, phoneNumber;
    int age;

    cout << "Enter your name: ";
    cin.ignore();
    getline(cin, name);

    cout << "Enter your email: ";
    getline(cin, email);

    cout << "Enter your password: ";
    getline(cin, password);

    cout << "Enter your age: ";
    cin >> age;
    cin.ignore();

    cout << "Enter your gender: ";
    getline(cin, gender);

    cout << "Enter your phone number: ";
    getline(cin, phoneNumber);

    cout << "Enter your address:\n";
    Address address;
    address.setAddress();

    Customer newCustomer;
    DatabaseManager db("ecommerce.db");
    newCustomer.custID = db.getNextCustomerId();
    newCustomer.Name = name;
    newCustomer.EMail = email;
    newCustomer.Password = password;
    newCustomer.Age = age;
    newCustomer.Gender = gender;
    newCustomer.phoneNumber = phoneNumber;
    newCustomer.customerAddress = address;
    newCustomer.customerCart = Cart(&newCustomer);

    customers.push_back(newCustomer);

    cout << "Customer registered successfully with ID: " << newCustomer.custID << endl;
}

bool Customer::logIn(const vector<Customer>& customers)
{
    string inputEmail, inputPassword;
    cout << "Enter email: ";
    cin.ignore();
    getline(cin, inputEmail);
    cout << "Enter password: ";
    getline(cin, inputPassword);

    for (const Customer& customer : customers)
    {
        if (customer.EMail == inputEmail && customer.Password == inputPassword)
        {
            cout << "Login successful!" << endl;
            return true;
        }
    }

    cout << "Invalid credentials!" << endl;
    return false;
}
void Customer::updateInfo()
{
    int key;
    do
    {
        cout << "\n===== Update Information =====\n";
        cout << "1: Update Password\n";
        cout << "2: Update Email\n";
        cout << "3: Update Address\n";
        cout << "4: Update Phone Number\n";
        cout << "5: Update Name\n";
        cout << "6: Update Age\n";
        cout << "7: Update Gender\n";
        cout << "8: Exit\n";
        cout << "Choose what to update: ";
        cin >> key;
        cin.ignore();

        switch (key)
        {
        case 1:
            cout << "New Password: ";
            getline(cin, Password);
            cout << "Password updated successfully!\n";
            break;
        case 2:
            cout << "New Email: ";
            getline(cin, EMail);
            cout << "Email updated successfully!\n";
            break;
        case 3:
            cout << "New Address: \n";
            customerAddress.setAddress();
            cout << "Address updated successfully!\n";
            break;
        case 4:
            cout << "New Phone Number: ";
            getline(cin, phoneNumber);
            cout << "Phone Number updated successfully!\n";
            break;
        case 5:
            cout << "New Name: ";
            getline(cin, Name);
            cout << "Name updated successfully!\n";
            break;
        case 6:
            cout << "New Age: ";
            cin >> Age;
            cin.ignore();
            cout << "Age updated successfully!\n";
            break;
        case 7:
            cout << "New Gender: ";
            getline(cin, Gender);
            cout << "Gender updated successfully!\n";
            break;
        case 8:
            cout << "Exiting update menu.\n";
            break;
        default:
            cout << "Invalid choice, please try again.\n";
        }
    }
    while (key != 8);
}

void Customer::printUserInfo()
{
    cout << "\n===== Customer Information =====\n";
    cout << "Customer ID: " << custID << endl;
    cout << "Name: " << Name << endl;
    cout << "Email: " << EMail << endl;
    cout << "Age: " << Age << endl;
    cout << "Gender: " << Gender << endl;
    cout << "Phone Number: " << phoneNumber << endl;
    cout << "Address: ";
    customerAddress.printAddress();
    cout << "\nShopping Cart: \n";
    customerCart.viewItems();
}

void Customer::printCustomerById(const vector<Customer>& customers, int customerId)
{
    for (const Customer& customer : customers)
    {
        if (customer.getId() == customerId)
        {
            cout << "ID: " << customer.getId()
                 << ", Name: " << customer.getName()
                 << ", Email: " << customer.getEmail() << endl;
            return;
        }
    }
    cout << "No customer found with ID: " << customerId << endl;
}

// Cart Functions
void Customer::addToCart(const Product& product)
{
    customerCart.addToCart(product);
}

void Customer::removeFromCart(const Product& product)
{
    customerCart.removeFromCart(product);
}

void Customer::viewCart() const
{
    customerCart.viewItems();
}

// Getters
int Customer::getId() const
{
    return custID;
}

const string& Customer::getName() const
{
    return Name;
}

const string& Customer::getEmail() const
{
    return EMail;
}

const string& Customer::getPassword() const
{
    return Password;
}

/*
bool Customer::checkCredentials(const std::string& email, const std::string& password) const {
    return this->EMail == email && this->Password == password;
}*/

bool Customer::checkCredentials(const std::string& email, const std::string& password) const
{
    return this->EMail == email && this->Password == password;
}
Cart& Customer::getCart(){
    return customerCart;
}
