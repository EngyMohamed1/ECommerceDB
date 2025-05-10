#ifndef SHIPPING_H
#define SHIPPING_H

#include <iostream>
#include <vector>
using namespace std;

class Shipping
{
public:
    Shipping();
    Shipping(int id, string m, bool s, bool track, double price);

    virtual ~Shipping();

    void showShipmentTrack();
    void selectShippingMethod(string newMethod);

    double getShippingPrice() const;

    int getID() const { return ID; }
    const std::string& getMethod() const { return method; }
    bool isShipped() const { return shipped; }
    bool hasShipmentTracking() const { return shipmentTracking; }


private:
    int ID;
    double shipmentPrice;

    string method;

    bool shipped;
    bool shipmentTracking;
};

#endif // SHIPPING_H
