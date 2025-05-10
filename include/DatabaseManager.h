#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <sqlite3.h>
#include <string>
#include <vector>
#include "Customer.h"
#include "Product.h"
#include "Category.h"
#include "Cart.h"
#include "Order.h"
#include "Payment.h"
#include "Shipping.h"
#include "Address.h"
#include "cartItems.h"
#include "orderItems.h"

class DatabaseManager {
private:
    sqlite3* db;

    bool executeQuery(const std::string& sql);
    static int callback(void* data, int argc, char** argv, char** azColName);
    void logError(const std::string& message);

public:
    DatabaseManager(const std::string& dbName);
    ~DatabaseManager();

    bool connect();
    void disconnect();
    bool createTables();

    // Customer operations
    int getNextCustomerId();
    bool insertCustomer(const Customer& customer);
    bool updateCustomer(const Customer& customer);
    bool deleteCustomer(int custID);
    std::vector<Customer> getCustomers();
    Customer getCustomerById(int custID);
    Customer getCustomerByEmail(const std::string& email);

    // Product operations
    bool productExists(int productID);
    int getNextProductId();
    int insertProduct(const Product& product);
    bool updateProduct(const Product& product);
    bool deleteProduct(int productID);
    std::vector<Product> getProducts();
    Product getProductById(int productID);

    // Category operations
    bool categoryExists(int categoryID);
    int getNextCategoryId();
    int insertCategory(const Category& category);
    bool updateCategory(const Category& category);
    bool deleteCategory(int categoryID);
    std::vector<Category> getCategories();
    Category getCategoryById(int categoryID);

    // Order operations
    int getLastOrderId();
    bool insertOrder(int custID, double totalPrice, int paymentID, int shippingID);
    bool insertOrderItem(int orderID, int productID, int quantity);
    bool updateOrderStatus(int orderID, const std::string& status);
    std::vector<Order> getOrders();
    std::vector<orderItems> getOrderItems(int orderID);
    std::vector<Order> getOrdersByCustomer(int custID);

    // Cart operations
    bool insertCartItem(int custID, const cartItems& cartItem);
    bool deleteCartItem(int custID, int productID);
    bool updateCartItem(int custID, int productID, int newQuantity);
    bool cartItemExists(int custID, int productID);
    int getCartItemQuantity(int custID, int productID);
    bool clearCartItems(int custID);
    std::vector<cartItems> getCartItems(int custID);

    // Payment operations
    bool insertPayment(const Payment& payment);
    int getNextPaymentId();
    Payment getPaymentById(int payID);

    // Shipping operations
    bool insertShipping(const Shipping& shipping);
    int getNextShippingId();
    Shipping getShippingById(int shippingID);

    // Address operations
    bool insertAddress(int custID, const Address& address);
    Address getAddressByCustomerId(int custID);
};

#endif
