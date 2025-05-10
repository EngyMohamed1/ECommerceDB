#include "DatabaseManager.h"
#include <iostream>
#include <sstream>


DatabaseManager::DatabaseManager(const std::string& dbName) : db(nullptr) {
    int rc = sqlite3_open(dbName.c_str(), &db);
    if (rc != SQLITE_OK) {
        logError("Cannot open database: " + std::string(sqlite3_errmsg(db)));
        db = nullptr;
    } else {
        std::cout << "Database opened successfully!" << std::endl;
    }
}

DatabaseManager::~DatabaseManager() {
    disconnect();
}

bool DatabaseManager::connect() {
    return db != nullptr;
}

void DatabaseManager::disconnect() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
        std::cout << "Database closed." << std::endl;
    }
}

void DatabaseManager::logError(const std::string& message) {
    std::cerr << "Database Error: " << message << std::endl;
}

bool DatabaseManager::executeQuery(const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        logError("SQL error: " + std::string(errMsg));
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

int DatabaseManager::callback(void* data, int argc, char** argv, char** azColName) {
    return 0; // Used for queries if needed
}

bool DatabaseManager::createTables() {
    std::string queries[] = {
        "CREATE TABLE IF NOT EXISTS Customers ("
        "custID INTEGER PRIMARY KEY, "
        "Name TEXT NOT NULL, "
        "EMail TEXT UNIQUE NOT NULL, "
        "Password TEXT NOT NULL, "
        "Age INTEGER, "
        "Gender TEXT, "
        "phoneNumber TEXT);",

        "CREATE TABLE IF NOT EXISTS Addresses ("
        "custID INTEGER PRIMARY KEY, "
        "Governorate TEXT, "
        "City TEXT, "
        "Street TEXT, "
        "buildingNumber INTEGER, "
        "apartmentNumber INTEGER, "
        "FOREIGN KEY(custID) REFERENCES Customers(custID));",

        "CREATE TABLE IF NOT EXISTS Categories ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "Name TEXT NOT NULL, "
        "Description TEXT);",

        "CREATE TABLE IF NOT EXISTS Products ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "Name TEXT NOT NULL, "
        "Description TEXT, "
        "Availability INTEGER, "
        "Price REAL, "
        "CategoryID INTEGER, "
        "FOREIGN KEY(CategoryID) REFERENCES Categories(ID));",

        "CREATE TABLE IF NOT EXISTS CartItems ("
        "custID INTEGER, "
        "productID INTEGER, "
        "quantity INTEGER, "
        "PRIMARY KEY(custID, productID), "
        "FOREIGN KEY(custID) REFERENCES Customers(custID), "
        "FOREIGN KEY(productID) REFERENCES Products(ID));",

        "CREATE TABLE IF NOT EXISTS Orders ("
        "ID INTEGER PRIMARY KEY, "
        "custID INTEGER, "
        "status TEXT, "
        "totalPrice REAL, "
        "paymentID INTEGER, "
        "shippingID INTEGER, "
        "FOREIGN KEY(custID) REFERENCES Customers(custID), "
        "FOREIGN KEY(paymentID) REFERENCES Payments(payID), "
        "FOREIGN KEY(shippingID) REFERENCES Shipments(ID));",

        "CREATE TABLE IF NOT EXISTS OrderItems ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "orderID INTEGER, "
        "productID INTEGER, "
        "quantity INTEGER, "
        "FOREIGN KEY(orderID) REFERENCES Orders(ID), "
        "FOREIGN KEY(productID) REFERENCES Products(ID));"

        "CREATE TABLE IF NOT EXISTS Payments ("
        "payID INTEGER PRIMARY KEY, "
        "Method TEXT, "
        "paymentStatus TEXT, "
        "cardNumber TEXT, "
        "expirationDate TEXT, "
        "cvv TEXT, "
        "cardholderName TEXT);"

        "CREATE TABLE IF NOT EXISTS Shipments ("
        "ID INTEGER PRIMARY KEY, "
        "method TEXT, "
        "shipped INTEGER, "
        "shipmentTracking INTEGER, "
        "shipmentPrice REAL);"
    };

    for (const auto& query : queries) {
        if (!executeQuery(query)) {
            return false;
        }
    }
    return true;
}

// Customer operations
int DatabaseManager::getNextCustomerId() {
    int maxId = 0;
    std::string query = "SELECT MAX(custID) FROM Customers;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            maxId = sqlite3_column_int(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);
    return maxId + 1;
}

bool DatabaseManager::insertCustomer(const Customer& customer) {
    std::stringstream ss;
    ss << "INSERT INTO Customers (custID, Name, EMail, Password, Age, Gender, phoneNumber) VALUES ("
       << customer.getId() << ", '"
       << customer.getName() << "', '"
       << customer.getEmail() << "', '"
       << customer.getPassword() << "', "
       << customer.getAge() << ", '"
       << customer.getGender() << "', '"
       << customer.getPhoneNumber() << "');";
    return executeQuery(ss.str());
}

bool DatabaseManager::insertAddress(int custID, const Address& address) {
    std::stringstream ss;
    ss << "INSERT INTO Addresses (custID, Governorate, City, Street, buildingNumber, apartmentNumber) VALUES ("
       << custID << ", '"
       << address.getGovernorate() << "', '"
       << address.getCity() << "', '"
       << address.getStreet() << "', "
       << address.getBuildingNumber() << ", "
       << address.getApartmentNumber() << ");";
    return executeQuery(ss.str());
}

bool DatabaseManager::updateCustomer(const Customer& customer) {
    std::stringstream ss;
    ss << "UPDATE Customers SET "
       << "Name = '" << customer.getName() << "', "
       << "EMail = '" << customer.getEmail() << "', "
       << "Password = '" << customer.getPassword() << "', "
       << "Age = " << customer.getAge() << ", "
       << "Gender = '" << customer.getGender() << "', "
       << "phoneNumber = '" << customer.getPhoneNumber() << "' "
       << "WHERE custID = " << customer.getId() << ";";
    return executeQuery(ss.str());
}

bool DatabaseManager::deleteCustomer(int custID) {
    std::string query = "DELETE FROM Customers WHERE custID = " + std::to_string(custID) + ";";
    return executeQuery(query);
}

std::vector<Customer> DatabaseManager::getCustomers() {
    std::vector<Customer> customers;
    std::string query = "SELECT * FROM Customers;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Customer customer;
            customer.setId(sqlite3_column_int(stmt, 0));
            customer.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            customer.setEmail(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            customer.setPassword(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
            customer.setAge(sqlite3_column_int(stmt, 4));
            customer.setGender(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
            customer.setPhoneNumber(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)));
            customers.push_back(customer);
        }
    } else {
        logError("Failed to prepare statement: " + std::string(sqlite3_errmsg(db)));
    }
    sqlite3_finalize(stmt);
    return customers;
}

Customer DatabaseManager::getCustomerById(int custID) {
    Customer customer;
    std::string query = "SELECT * FROM Customers WHERE custID = " + std::to_string(custID) + ";";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            customer.setId(sqlite3_column_int(stmt, 0));
            customer.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            customer.setEmail(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            customer.setPassword(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
            customer.setAge(sqlite3_column_int(stmt, 4));
            customer.setGender(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
            customer.setPhoneNumber(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)));
        }
    }
    sqlite3_finalize(stmt);
    return customer;
}

Customer DatabaseManager::getCustomerByEmail(const std::string& email) {
    Customer customer;
    std::string query = "SELECT * FROM Customers WHERE EMail = '" + email + "';";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            customer.setId(sqlite3_column_int(stmt, 0));
            customer.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            customer.setEmail(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            customer.setPassword(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
            customer.setAge(sqlite3_column_int(stmt, 4));
            customer.setGender(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
            customer.setPhoneNumber(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)));
        }
    }
    sqlite3_finalize(stmt);
    return customer;
}

Address DatabaseManager::getAddressByCustomerId(int custID) {
    Address address;
    std::string query = "SELECT * FROM Addresses WHERE custID = " + std::to_string(custID) + ";";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            address.setGovernorate(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            address.setCity(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            address.setStreet(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
            address.setBuildingNumber(sqlite3_column_int(stmt, 4));
            address.setApartmentNumber(sqlite3_column_int(stmt, 5));
        }
    }
    sqlite3_finalize(stmt);
    return address;
}

// Product operations
bool DatabaseManager::productExists(int productID) {
    std::string query = "SELECT 1 FROM Products WHERE ID = " + std::to_string(productID) + " LIMIT 1;";
    sqlite3_stmt* stmt;
    bool exists = false;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            exists = true;
        }
    }
    sqlite3_finalize(stmt);
    return exists;
}

int DatabaseManager::getNextProductId() {
    int maxId = 0;
    std::string query = "SELECT MAX(ID) FROM Products;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            maxId = sqlite3_column_int(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);
    return maxId + 1;
}

int DatabaseManager::insertProduct(const Product& product) {
    std::stringstream ss;
    ss << "INSERT INTO Products (Name, Description, Availability, Price, CategoryID) VALUES ('"
       << product.getName() << "', '"
       << product.getDescription() << "', "
       << product.getAvailability() << ", "
       << product.getPrice() << ", "
       << product.getCategory()->getID() << ");";
    if (executeQuery(ss.str())) {
        return static_cast<int>(sqlite3_last_insert_rowid(db));
    }
    return -1;
}

bool DatabaseManager::updateProduct(const Product& product) {
    std::stringstream ss;
    ss << "UPDATE Products SET "
       << "Name = '" << product.getName() << "', "
       << "Description = '" << product.getDescription() << "', "
       << "Availability = " << product.getAvailability() << ", "
       << "Price = " << product.getPrice() << ", "
       << "CategoryID = " << product.getCategory()->getID() << " "
       << "WHERE ID = " << product.getProductID() << ";";
    return executeQuery(ss.str());
}

bool DatabaseManager::deleteProduct(int productID) {
    std::string query = "DELETE FROM Products WHERE ID = " + std::to_string(productID) + ";";
    return executeQuery(query);
}

std::vector<Product> DatabaseManager::getProducts() {
    std::vector<Product> products;
    std::string query = "SELECT p.*, c.ID, c.Name, c.Description FROM Products p "
                        "JOIN Categories c ON p.CategoryID = c.ID;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Product product;
            product.setProductID(sqlite3_column_int(stmt, 0));
            product.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            product.setDescription(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            product.setAvailability(sqlite3_column_int(stmt, 3));
            product.setPrice(sqlite3_column_double(stmt, 4));
            Category* category = new Category(
                sqlite3_column_int(stmt, 6),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8))
            );
            product.setCategory(category);
            products.push_back(product);
        }
    }
    sqlite3_finalize(stmt);
    return products;
}

Product DatabaseManager::getProductById(int productID) {
    Product product;
    std::string query = "SELECT p.*, c.ID, c.Name, c.Description FROM Products p "
                        "JOIN Categories c ON p.CategoryID = c.ID WHERE p.ID = " + std::to_string(productID) + ";";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            product.setProductID(sqlite3_column_int(stmt, 0));
            product.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            product.setDescription(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            product.setAvailability(sqlite3_column_int(stmt, 3));
            product.setPrice(sqlite3_column_double(stmt, 4));
            Category* category = new Category(
                sqlite3_column_int(stmt, 6),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8))
            );
            product.setCategory(category);
        }
    }
    sqlite3_finalize(stmt);
    return product;
}

// Category operations
bool DatabaseManager::categoryExists(int categoryID) {
    std::string query = "SELECT 1 FROM Categories WHERE ID = " + std::to_string(categoryID) + " LIMIT 1;";
    sqlite3_stmt* stmt;
    bool exists = false;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            exists = true;
        }
    }
    sqlite3_finalize(stmt);
    return exists;
}

int DatabaseManager::getNextCategoryId() {
    int maxId = 0;
    std::string query = "SELECT MAX(ID) FROM Categories;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            maxId = sqlite3_column_int(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);
    return maxId + 1;
}

int DatabaseManager::insertCategory(const Category& category) {
    std::stringstream ss;
    ss << "INSERT INTO Categories (Name, Description) VALUES ('"
       << category.getName() << "', '"
       << category.getDescription() << "');";
    if (executeQuery(ss.str())) {
        return static_cast<int>(sqlite3_last_insert_rowid(db));
    }
    return -1;
}

bool DatabaseManager::updateCategory(const Category& category) {
    std::stringstream ss;
    ss << "UPDATE Categories SET "
       << "Name = '" << category.getName() << "', "
       << "Description = '" << category.getDescription() << "' "
       << "WHERE ID = " << category.getID() << ";";
    return executeQuery(ss.str());
}

bool DatabaseManager::deleteCategory(int categoryID) {
    std::string query = "DELETE FROM Categories WHERE ID = " + std::to_string(categoryID) + ";";
    return executeQuery(query);
}

std::vector<Category> DatabaseManager::getCategories() {
    std::vector<Category> categories;
    std::string query = "SELECT * FROM Categories;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Category category(
                sqlite3_column_int(stmt, 0),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))
            );
            categories.push_back(category);
        }
    }
    sqlite3_finalize(stmt);
    return categories;
}

Category DatabaseManager::getCategoryById(int categoryID) {
    Category category;
    std::string query = "SELECT * FROM Categories WHERE ID = " + std::to_string(categoryID) + ";";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            category = Category(
                sqlite3_column_int(stmt, 0),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))
            );
        }
    }
    sqlite3_finalize(stmt);
    return category;
}

// Order operations
int DatabaseManager::getLastOrderId() {
    int orderId = 0;
    std::string query = "SELECT MAX(ID) FROM Orders;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            orderId = sqlite3_column_int(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);
    return orderId;
}

bool DatabaseManager::insertOrder(int custID, double totalPrice, int paymentID, int shippingID) {
    std::stringstream ss;
    ss << "INSERT INTO Orders (custID, totalPrice, status, paymentID, shippingID) VALUES ("
       << custID << ", "
       << totalPrice << ", 'Pending', "
       << paymentID << ", "
       << shippingID << ");";
    return executeQuery(ss.str());
}

bool DatabaseManager::insertOrderItem(int orderID, int productID, int quantity) {
    std::stringstream ss;
    ss << "INSERT INTO OrderItems (orderID, productID, quantity) VALUES ("
       << orderID << ", "
       << productID << ", "
       << quantity << ");";
    return executeQuery(ss.str());
}

bool DatabaseManager::updateOrderStatus(int orderID, const std::string& status) {
    std::string query = "UPDATE Orders SET status = '" + status + "' WHERE ID = " + std::to_string(orderID) + ";";
    return executeQuery(query);
}

std::vector<Order> DatabaseManager::getOrders() {
    std::vector<Order> orders;
    std::string query = "SELECT o.*, c.custID, c.Name, c.EMail, c.Age, c.Gender, c.phoneNumber, "
                        "p.payID, p.Method, p.paymentStatus, p.cardNumber, p.expirationDate, p.cvv, p.cardholderName, "
                        "s.ID, s.method, s.shipped, s.shipmentTracking, s.shipmentPrice "
                        "FROM Orders o "
                        "JOIN Customers c ON o.custID = c.custID "
                        "JOIN Payments p ON o.paymentID = p.payID "
                        "JOIN Shipments s ON o.shippingID = s.ID;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Customer* customer = new Customer(
                sqlite3_column_int(stmt, 6),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8)),
                ""
            );
            customer->setAge(sqlite3_column_int(stmt, 9));
            customer->setGender(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10)));
            customer->setPhoneNumber(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 11)));
            customer->setAddress(getAddressByCustomerId(sqlite3_column_int(stmt, 6)));

            Payment* payment = new Payment();
            payment->setPayID(sqlite3_column_int(stmt, 12));
            payment->setMethod(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 13)));
            payment->setPaymentStatus(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 14)));
            payment->setCardNumber(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 15)));
            payment->setExpirationDate(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 16)));
            payment->setCvv(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 17)));
            payment->setCardholderName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 18)));

            Shipping shipping(
                sqlite3_column_int(stmt, 19),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 20)),
                sqlite3_column_int(stmt, 21),
                sqlite3_column_int(stmt, 22),
                sqlite3_column_double(stmt, 23)
            );

            std::vector<orderItems> items = getOrderItems(sqlite3_column_int(stmt, 0));

            Order order(
                sqlite3_column_int(stmt, 0),
                customer,
                items,
                shipping,
                payment,
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
                sqlite3_column_double(stmt, 3)
            );
            orders.push_back(order);
        }
    }
    sqlite3_finalize(stmt);
    return orders;
}

std::vector<orderItems> DatabaseManager::getOrderItems(int orderID) {
    std::vector<orderItems> items;
    std::string query = "SELECT oi.ID, oi.quantity, p.ID, p.Name, p.Description, p.Availability, p.Price, "
                        "c.ID, c.Name, c.Description "
                        "FROM OrderItems oi "
                        "JOIN Products p ON oi.productID = p.ID "
                        "JOIN Categories c ON p.CategoryID = c.ID "
                        "WHERE oi.orderID = " + std::to_string(orderID) + ";";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Category* category = new Category(
                sqlite3_column_int(stmt, 7),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9))
            );
            Product* product = new Product(
                sqlite3_column_int(stmt, 2),
                std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)),
                sqlite3_column_int(stmt, 5),
                sqlite3_column_double(stmt, 6),
                category
            );
            double itemPrice = sqlite3_column_double(stmt, 6) * sqlite3_column_int(stmt, 1); // Price * Quantity
            orderItems item(
                sqlite3_column_int(stmt, 0), // ID
                sqlite3_column_int(stmt, 1), // Quantity
                product,                     // Product
                itemPrice                   // Total Price
            );
            items.push_back(item);
        }
    }
    sqlite3_finalize(stmt);
    return items;
}

std::vector<Order> DatabaseManager::getOrdersByCustomer(int custID) {
    std::vector<Order> orders;
    std::string query = "SELECT o.*, c.custID, c.Name, c.EMail, p.payID, p.Method, p.paymentStatus, "
                        "s.ID, s.method, s.shipped, s.shipmentTracking, s.shipmentPrice "
                        "FROM Orders o "
                        "JOIN Customers c ON o.custID = c.custID "
                        "JOIN Payments p ON o.paymentID = p.payID "
                        "JOIN Shipments s ON o.shippingID = s.ID "
                        "WHERE o.custID = " + std::to_string(custID) + ";";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Customer* customer = new Customer(
                sqlite3_column_int(stmt, 6),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8)),
                ""
            );
            Payment* payment = new Payment();
            payment->setPayID(sqlite3_column_int(stmt, 9));
            payment->setMethod(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10)));
            payment->setPaymentStatus(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 11)));
            Shipping shipping(
                sqlite3_column_int(stmt, 12),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 13)),
                sqlite3_column_int(stmt, 14),
                sqlite3_column_int(stmt, 15),
                sqlite3_column_double(stmt, 16)
            );
            std::vector<orderItems> items; // Fetch order items separately if needed
            Order order(
                sqlite3_column_int(stmt, 0),
                customer,
                items,
                shipping,
                payment,
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
                sqlite3_column_double(stmt, 3)
            );
            orders.push_back(order);
        }
    }
    sqlite3_finalize(stmt);
    return orders;
}

// Cart operations
bool DatabaseManager::insertCartItem(int custID, const cartItems& cartItem) {
    std::stringstream ss;
    ss << "INSERT INTO CartItems (custID, productID, quantity) VALUES ("
       << custID << ", "
       << cartItem.getProduct().getProductID() << ", "
       << cartItem.getQuantity() << ");";
    return executeQuery(ss.str());
}

bool DatabaseManager::deleteCartItem(int custID, int productID) {
    std::string query = "DELETE FROM CartItems WHERE custID = " + std::to_string(custID) +
                        " AND productID = " + std::to_string(productID) + ";";
    return executeQuery(query);
}

bool DatabaseManager::updateCartItem(int custID, int productID, int newQuantity) {
    std::stringstream ss;
    ss << "UPDATE CartItems SET quantity = " << newQuantity
       << " WHERE custID = " << custID
       << " AND productID = " << productID << ";";
    return executeQuery(ss.str());
}

bool DatabaseManager::cartItemExists(int custID, int productID) {
    std::string query = "SELECT 1 FROM CartItems WHERE custID = " + std::to_string(custID) +
                        " AND productID = " + std::to_string(productID) + " LIMIT 1;";
    sqlite3_stmt* stmt;
    bool exists = false;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            exists = true;
        }
    }
    sqlite3_finalize(stmt);
    return exists;
}

int DatabaseManager::getCartItemQuantity(int custID, int productID) {
    int quantity = 0;
    std::string query = "SELECT quantity FROM CartItems WHERE custID = " + std::to_string(custID) +
                        " AND productID = " + std::to_string(productID) + ";";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            quantity = sqlite3_column_int(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);
    return quantity;
}

bool DatabaseManager::clearCartItems(int custID) {
    std::string query = "DELETE FROM CartItems WHERE custID = " + std::to_string(custID) + ";";
    return executeQuery(query);
}

std::vector<cartItems> DatabaseManager::getCartItems(int custID) {
    std::vector<cartItems> items;
    std::string query = "SELECT ci.productID, ci.quantity, p.ID, p.Name, p.Description, p.Availability, p.Price, "
                        "c.ID, c.Name, c.Description "
                        "FROM CartItems ci "
                        "JOIN Products p ON ci.productID = p.ID "
                        "JOIN Categories c ON p.CategoryID = c.ID "
                        "WHERE ci.custID = " + std::to_string(custID) + ";";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Category* category = new Category(
                sqlite3_column_int(stmt, 7),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9))
            );
            Product product(
                sqlite3_column_int(stmt, 2),
                std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)),
                sqlite3_column_int(stmt, 5),
                sqlite3_column_double(stmt, 6),
                category
            );
            cartItems item(product, sqlite3_column_int(stmt, 1));
            items.push_back(item);
        }
    }
    sqlite3_finalize(stmt);
    return items;
}

// Payment operations
bool DatabaseManager::insertPayment(const Payment& payment) {
    std::stringstream ss;
    ss << "INSERT INTO Payments (payID, Method, paymentStatus, cardNumber, expirationDate, cvv, cardholderName) VALUES ("
       << payment.getPayID() << ", '"
       << payment.getMethod() << "', '"
       << payment.getPaymentStatus() << "', '"
       << payment.getCardNumber() << "', '"
       << payment.getExpirationDate() << "', '"
       << payment.getCvv() << "', '"
       << payment.getCardholderName() << "');";
    return executeQuery(ss.str());
}

int DatabaseManager::getNextPaymentId() {
    int maxId = 0;
    std::string query = "SELECT MAX(payID) FROM Payments;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            maxId = sqlite3_column_int(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);
    return maxId + 1;
}

Payment DatabaseManager::getPaymentById(int payID) {
    Payment payment;
    std::string query = "SELECT * FROM Payments WHERE payID = " + std::to_string(payID) + ";";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            payment.setPayID(sqlite3_column_int(stmt, 0));
            payment.setMethod(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            payment.setPaymentStatus(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        }
    }
    sqlite3_finalize(stmt);
    return payment;
}

// Shipping operations
bool DatabaseManager::insertShipping(const Shipping& shipping) {
    std::stringstream ss;
    ss << "INSERT INTO Shipments (ID, method, shipped, shipmentTracking, shipmentPrice) VALUES ("
       << shipping.getID() << ", '"
       << shipping.getMethod() << "', "
       << (shipping.isShipped() ? 1 : 0) << ", "
       << (shipping.hasShipmentTracking() ? 1 : 0) << ", "
       << shipping.getShippingPrice() << ");";
    return executeQuery(ss.str());
}

int DatabaseManager::getNextShippingId() {
    int maxId = 0;
    std::string query = "SELECT MAX(ID) FROM Shipments;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            maxId = sqlite3_column_int(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);
    return maxId + 1;
}

Shipping DatabaseManager::getShippingById(int shippingID) {
    Shipping shipping;
    std::string query = "SELECT * FROM Shipments WHERE ID = " + std::to_string(shippingID) + ";";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            shipping = Shipping(
                sqlite3_column_int(stmt, 0),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                sqlite3_column_int(stmt, 2),
                sqlite3_column_int(stmt, 3),
                sqlite3_column_double(stmt, 4)
            );
        }
    }
    sqlite3_finalize(stmt);
    return shipping;
}
