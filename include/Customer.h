#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <vector>

#include "Cart.h"
#include "Address.h"
#include "Product.h"
#include "DatabaseManager.h"

using namespace std;

class Customer {
private:
    int custID;
    int Age;

    string Name;
    string EMail;
    string Password;
    string Gender;
    string phoneNumber;

    Address customerAddress;
    Cart customerCart;

public:
    Customer();  // Default constructor
    Customer(const std::string& name, int id);  // Minimal constructor
    Customer(int id, const std::string& name, const std::string& email, const std::string& password);

    ~Customer();

    void userInfo();
    static Customer Register();
    void Register(std::vector<Customer>& customers);

    // overloaded for in-memory system
    bool logIn(const std::vector<Customer>& customers);
    void updateInfo();
    void printUserInfo();
    static void printCustomerById(const std::vector<Customer>& customers, int customerId);

    // Cart operations
    void addToCart(const Product& product);
    void removeFromCart(const Product& product);
    void viewCart() const;

    // Setters
    void setId(int id) { custID = id; }
    void setName(const std::string& name) { Name = name; }
    void setEmail(const std::string& email) { EMail = email; }
    void setPassword(const std::string& password) { Password = password; }
    void setAge(int age) { Age = age; }
    void setGender(const std::string& gender) { Gender = gender; }
    void setPhoneNumber(const std::string& number) { phoneNumber = number; }
    void setAddress(const Address& address);

    // Getters
    int getId() const;
    const string& getName() const;
    const string& getEmail() const;
    const string& getPassword() const;
    int getAge() const { return Age; }
    const string& getGender() const { return Gender; }
    const string& getPhoneNumber() const { return phoneNumber; }
    bool checkCredentials(const std::string& email, const std::string& password) const;
    const Address& getAddress() const;
    Cart& getCart();

};

#endif
