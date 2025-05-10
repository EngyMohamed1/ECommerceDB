#ifndef SYSTEM_H
#define SYSTEM_H

#include "Customer.h"
#include "Product.h"
#include "Category.h"
#include "Order.h"
#include "DatabaseManager.h"


class System {
private:
    DatabaseManager dbManager;
    Customer* loggedInCustomer;

    void customerMenu();
    void productMenu();
    void categoryMenu();
    void placeOrderMenu();

public:
    System();
    ~System();
    void run();
    void login();
    void mainMenu();
};

#endif
