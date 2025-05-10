#include "System.h"

System::System() : dbManager("ecommerce.db"), loggedInCustomer(nullptr)
{
    if (dbManager.connect())
    {
        dbManager.createTables();
    }
    else
    {
        std::cerr << "Failed to connect to database!" << std::endl;
    }
}

System::~System()
{
    delete loggedInCustomer;
}

void System::run()
{
    mainMenu();
}
void System::login()
{
    string inputEmail, inputPassword;
    cout << "Enter email: ";
    cin.ignore();
    getline(cin, inputEmail);
    cout << "Enter password: ";
    getline(cin, inputPassword);

    Customer customer = dbManager.getCustomerByEmail(inputEmail);
    if (customer.getId() != 0 && customer.checkCredentials(inputEmail, inputPassword))
    {
        loggedInCustomer = new Customer(customer);
        loggedInCustomer->setAddress(dbManager.getAddressByCustomerId(customer.getId()));
        cout << "Login successful!" << endl;
    }
    else
    {
        cout << "Invalid credentials!" << endl;
    }
}

void System::mainMenu()
{
    int choice;
    do
    {
        cout << "\n===== Main Menu =====\n";
        cout << "[1] Customer Management\n";
        cout << "[2] Product Management\n";
        cout << "[3] Category Management\n";
        cout << "[4] Place Order\n";
        cout << "[0] Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            customerMenu();
            break;
        case 2:
            productMenu();
            break;
        case 3:
            categoryMenu();
            break;
        case 4:
            placeOrderMenu();
            break;
        case 0:
            cout << "Exiting program...\n";
            break;
        default:
            cout << "Invalid choice, please try again!\n";
        }
    }
    while (choice != 0);
}
void System::customerMenu()
{
    int choice;
    do
    {
        cout << "\n===== Customer Menu =====\n";
        cout << "[1] Register New Customer\n";
        cout << "[2] Login\n";
        if (loggedInCustomer)
        {
            cout << "[3] Logout\n";
            cout << "[4] Update Customer Info\n";
            cout << "[5] View Customer Info\n";
        }
        else
        {
            cout << "[3] Update Customer Info (Login Required)\n";
            cout << "[4] View Customer Info (Login Required)\n";
        }
        cout << "[0] Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            Customer newCustomer = Customer::Register();
            dbManager.insertCustomer(newCustomer);
            dbManager.insertAddress(newCustomer.getId(), newCustomer.getAddress());
            cout << "Customer registered successfully with ID: " << newCustomer.getId() << endl;
            break;
        }
        case 2:
        {
            if (loggedInCustomer)
            {
                cout << "You're already logged in as: " << loggedInCustomer->getEmail() << endl;
                break;
            }
            string email, password;
            cin.ignore();
            cout << "Enter email: ";
            getline(cin, email);
            cout << "Enter password: ";
            getline(cin, password);
            Customer customer = dbManager.getCustomerByEmail(email);
            if (customer.getId() != 0 && customer.checkCredentials(email, password))
            {
                loggedInCustomer = new Customer(customer);
                loggedInCustomer->setAddress(dbManager.getAddressByCustomerId(customer.getId()));
                cout << "Login successful!\n";
            }
            else
            {
                cout << "Invalid credentials!\n";
            }
            break;
        }
        case 3:
        {
            if (loggedInCustomer)
            {
                cout << "Logging out " << loggedInCustomer->getName() << "...\n";
                delete loggedInCustomer;
                loggedInCustomer = nullptr;
                cout << "You have been logged out.\n";
            }
            else
            {
                cout << "You must log in first!\n";
            }
            break;
        }
        case 4:
        {
            if (loggedInCustomer)
            {
                loggedInCustomer->updateInfo();
                dbManager.updateCustomer(*loggedInCustomer);
                dbManager.insertAddress(loggedInCustomer->getId(), loggedInCustomer->getAddress());
                cout << "Customer info updated in database.\n";
            }
            else
            {
                cout << "You must log in first!\n";
            }
            break;
        }
        case 5:
        {
            if (loggedInCustomer)
            {
                loggedInCustomer->printUserInfo();
            }
            else
            {
                cout << "You must log in first!\n";
            }
            break;
        }
        case 0:
            cout << "Returning to Main Menu...\n";
            break;
        default:
            cout << "Invalid choice, please try again!\n";
        }
    }
    while (choice != 0);
}



void System::productMenu()
{
    int choice;
    do
    {
        cout << "\n===== Product Menu =====\n";
        cout << "[1] View Products\n";
        cout << "[2] Add Product to Cart\n";
        cout << "[3] Remove Product from Cart\n";
        cout << "[4] View Cart Items\n";
        cout << "[5] Enter Product to System\n";
        cout << "[0] Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            auto products = dbManager.getProducts();
            if (products.empty())
            {
                cout << "No products available.\n";
            }
            else
            {
                for (auto& product : products)
                {
                    product.viewProduct();
                }
            }
            break;
        }
        case 2:
        {
            if (!loggedInCustomer)
            {
                cout << "You must be logged in to add products to cart.\n";
                break;
            }
            int prodID;
            cout << "Enter Product ID: ";
            cin >> prodID;
            Product product = dbManager.getProductById(prodID);
            if (product.getProductID() != 0)
            {
                if (product.getAvailability() > 0)
                {
                    if (dbManager.cartItemExists(loggedInCustomer->getId(), prodID))
                    {
                        int currentQuantity = dbManager.getCartItemQuantity(loggedInCustomer->getId(), prodID);
                        if (currentQuantity >= product.getAvailability())
                        {
                            cout << "Cannot add more. Only " << product.getAvailability() << " available.\n";
                        }
                        else
                        {
                            dbManager.updateCartItem(loggedInCustomer->getId(), prodID, currentQuantity + 1);
                            cout << "Increased quantity of " << product.getName() << " to " << (currentQuantity + 1) << ".\n";
                        }
                    }
                    else
                    {
                        cartItems item(product, 1);
                        if (dbManager.insertCartItem(loggedInCustomer->getId(), item))
                        {
                            cout << product.getName() << " added to cart.\n";
                        }
                        else
                        {
                            cout << "Failed to add " << product.getName() << " to cart.\n";
                        }
                    }
                }
                else
                {
                    cout << product.getName() << " is out of stock.\n";
                }
            }
            else
            {
                cout << "Product not found.\n";
            }
            break;
        }

        case 3:
        {
            if (!loggedInCustomer)
            {
                cout << "You must be logged in to remove products from cart.\n";
                break;
            }
            int prodID;
            cout << "Enter Product ID to remove: ";
            cin >> prodID;
            if (dbManager.cartItemExists(loggedInCustomer->getId(), prodID))
            {
                int currentQuantity = dbManager.getCartItemQuantity(loggedInCustomer->getId(), prodID);
                if (currentQuantity > 1)
                {
                    dbManager.updateCartItem(loggedInCustomer->getId(), prodID, currentQuantity - 1);
                    cout << "Quantity of product ID " << prodID << " reduced to " << (currentQuantity - 1) << ".\n";
                }
                else
                {
                    dbManager.deleteCartItem(loggedInCustomer->getId(), prodID);
                    cout << "Product removed from cart.\n";
                }
            }
            else
            {
                cout << "Product not found in cart.\n";
            }
            break;
        }
        case 4:
        {
            if (!loggedInCustomer)
            {
                cout << "You must be logged in to view cart.\n";
                break;
            }
            auto items = dbManager.getCartItems(loggedInCustomer->getId());
            if (items.empty())
            {
                cout << "Cart is empty.\n";
            }
            else
            {
                cout << "Cart contains:\n";
                double total = 0;
                for (const auto& item : items)
                {
                    cout << "- " << item.getProduct().getName() << " (x" << item.getQuantity() << ")\n";
                    total += item.getProduct().getPrice() * item.getQuantity();
                }
                cout << "Total Price: $" << total << "\n";
            }
            break;
        }
        case 5:
        {
            auto categories = dbManager.getCategories();
            if (categories.empty())
            {
                cout << "No categories available. Add a category first.\n";
                break;
            }
            Product product;
            product.enterProducts();
            cout << "Available Categories:\n";
            for (const auto& c : categories)
            {
                cout << "ID: " << c.getID() << " - " << c.getName() << endl;
            }
            int catID;
            cout << "Enter Category ID: ";
            cin >> catID;
            Category category = dbManager.getCategoryById(catID);
            if (category.getID() != 0)
            {
                product.setCategory(new Category(category));
                int newId = dbManager.insertProduct(product);
                if (newId != -1)
                {
                    product.setProductID(newId);
                    cout << "Product added successfully with ID: " << newId << ".\n";
                }
                else
                {
                    cout << "Failed to add product.\n";
                }
            }
            else
            {
                cout << "Invalid category ID.\n";
            }
            break;
        }
        case 0:
            break;
        default:
            cout << "Invalid choice. Try again!" << endl;
        }
    }
    while (choice != 0);
}


void System::categoryMenu()
{
    int choice;
    do
    {
        cout << "\n===== Category Menu =====\n";
        cout << "[1] View Categories\n";
        cout << "[2] Add New Category\n";
        cout << "[3] View Products in Category\n";
        cout << "[0] Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            auto categories = dbManager.getCategories();
            if (categories.empty())
            {
                cout << "No categories added yet.\n";
            }
            else
            {
                cout << "Available Categories:\n";
                for (const auto& c : categories)
                {
                    cout << "- " << c.getName() << "\n";
                }
            }
            break;
        }
        case 2:
        {
            string name, description;
            cout << "Enter category name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter category description: ";
            getline(cin, description);
            Category category(0, name, description);
            int newId = dbManager.insertCategory(category);
            if (newId != -1)
            {
                cout << "Category added successfully with ID: " << newId << "!\n";
            }
            else
            {
                cout << "Failed to add category.\n";
            }
            break;
        }
        case 3:
        {
            int catID;
            cout << "Enter category ID: ";
            cin >> catID;
            Category category = dbManager.getCategoryById(catID);
            if (category.getID() != 0)
            {
                cout << "Products in category: " << category.getName() << "\n";
                auto products = dbManager.getProducts();
                bool hasProducts = false;
                for (const auto& p : products)
                {
                    if (p.getCategory()->getID() == catID)
                    {
                        hasProducts = true;
                        cout << "- " << p.getName() << " | Price: $" << p.getPrice()
                             << " | Available: " << p.getAvailability() << "\n";
                    }
                }
                if (!hasProducts)
                {
                    cout << "No products in this category.\n";
                }
            }
            else
            {
                cout << "Invalid category ID.\n";
            }
            break;
        }
        case 0:
            cout << "Returning to Main Menu...\n";
            break;
        default:
            cout << "Invalid choice, please try again!\n";
        }
    }
    while (choice != 0);
}


void System::placeOrderMenu()
{
    int choice;
    do
    {
        cout << "\n===== Place Order Menu =====\n";
        cout << "[1] Place Order\n";
        cout << "[2] View All Orders\n";
        cout << "[3] Cancel Order\n";
        cout << "[0] Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            if (!loggedInCustomer)
            {
                cout << "You must be logged in to place an order!\n";
                break;
            }
            auto cartItems = dbManager.getCartItems(loggedInCustomer->getId());
            if (cartItems.empty())
            {
                cout << "Your cart is empty.\n";
                break;
            }

            string shippingMethod;
            cout << "Choose Shipping Method (Standard / Express): ";
            cin >> shippingMethod;
            double shippingCost = (shippingMethod == "Express") ? 50.0 : 20.0;

            string paymentMethod;
            cout << "Choose Payment Method:\n";
            cout << "[1] Cash\n";
            cout << "[2] Visa\n";
            cout << "Enter choice (1 or 2): ";
            int paymentChoice;
            cin >> paymentChoice;
            if (paymentChoice == 1)
            {
                paymentMethod = "Cash";
            }
            else if (paymentChoice == 2)
            {
                paymentMethod = "Visa";
            }
            else
            {
                cout << "Invalid payment method.\n";
                break;
            }

            double totalPrice = shippingCost;
            for (const auto& item : cartItems)
            {
                totalPrice += item.getProduct().getPrice() * item.getQuantity();
            }

            int paymentID = dbManager.getNextPaymentId();
            Payment payment;
            payment.setPayID(paymentID);
            payment.setMethod(paymentMethod);
            payment.setPaymentStatus("Pending");

            if (paymentMethod == "Visa")
            {
                string cardNumber, expirationDate, cvv, cardholderName;
                cin.ignore();
                cout << "Enter Card Number: ";
                getline(cin, cardNumber);
                cout << "Enter Expiration Date (MM/YY): ";
                getline(cin, expirationDate);
                cout << "Enter CVV: ";
                getline(cin, cvv);
                cout << "Enter Cardholder Name: ";
                getline(cin, cardholderName);

                payment.setCardNumber(cardNumber);
                payment.setExpirationDate(expirationDate);
                payment.setCvv(cvv);
                payment.setCardholderName(cardholderName);
            }

            if (!dbManager.insertPayment(payment))
            {
                cout << "Failed to insert payment.\n";
                break;
            }

            int shippingID = dbManager.getNextShippingId();
            Shipping shipping(shippingID, shippingMethod, true, true, shippingCost);
            if (!dbManager.insertShipping(shipping))
            {
                cout << "Failed to insert shipping.\n";
                break;
            }

            if (dbManager.insertOrder(loggedInCustomer->getId(), totalPrice, paymentID, shippingID))
            {
                int orderID = dbManager.getLastOrderId();

                for (const auto& item : cartItems)
                {
                    dbManager.insertOrderItem(orderID, item.getProduct().getProductID(), item.getQuantity());
                }

                dbManager.clearCartItems(loggedInCustomer->getId());
                loggedInCustomer->getCart().clear();

                cout << "Order placed successfully!\n";
            }
            else
            {
                cout << "Failed to place order.\n";
            }
            break;
        }
        case 2:
        {
            auto orders = dbManager.getOrders();
            if (orders.empty())
            {
                cout << "No orders found.\n";
                break;
            }
            for (auto& ord : orders)
            {
                ord.showOrderDetails();
                ord.getCustomer()->printUserInfo();
                ord.getShipping().showShipmentTrack();
                ord.getPayment()->printPayment();
                cout << "Items:\n";
                auto items = ord.getItems();
                if (items.empty())
                {
                    cout << "No items in this order.\n";
                }
                else
                {
                    for (auto& item : items)
                    {
                        cout << "- " << item.getProduct()->getName() << " (x" << item.getQuantity() << ")\n";
                    }
                }
                cout << "----------------------\n";
            }
            break;
        }
        case 3:
        {
            if (!loggedInCustomer)
            {
                cout << "You must be logged in to cancel an order!\n";
                break;
            }
            auto orders = dbManager.getOrdersByCustomer(loggedInCustomer->getId());
            if (orders.empty())
            {
                cout << "You have no orders to cancel.\n";
                break;
            }
            cout << "\n===== Your Orders =====\n";
            for (size_t i = 0; i < orders.size(); ++i)
            {
                cout << "[" << i + 1 << "] ";
                orders[i].showOrderDetails();
                cout << "----------------------\n";
            }
            cout << "Enter the number of the order to cancel: ";
            int selection;
            cin >> selection;
            if (selection >= 1 && selection <= static_cast<int>(orders.size()))
            {
                dbManager.updateOrderStatus(orders[selection - 1].getID(), "Cancelled");
                cout << "Order cancelled successfully.\n";
            }
            else
            {
                cout << "Invalid selection.\n";
            }
            break;
        }
        case 0:
            cout << "Returning to Main Menu...\n";
            break;
        default:
            cout << "Invalid choice, please try again!\n";
        }
    }
    while (choice != 0);
}
