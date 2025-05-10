#include "Shipping.h"


Shipping::Shipping()
{
    ID = 0;
    method = "Standard";
    shipped = false;
    shipmentTracking = false;
    shipmentPrice = 0.0;
}
Shipping::Shipping(int id, string m, bool s, bool track, double price)
{

    ID = id;
    method = m;
    shipped = s;
    shipmentTracking = track;
    shipmentPrice = price;
}
Shipping::~Shipping()
{ }

void Shipping::showShipmentTrack()
{
    cout << "Shipment ID: " << ID << "\n"
         << "Method: " << method << "\n"
         << "Shipped: " << (shipped ? "Yes" : "No") << "\n"
         << "Tracking Available: " << (shipmentTracking ? "Yes" : "No") << "\n";
}

double Shipping::getShippingPrice() const
{
    return shipmentPrice;
}

void Shipping::selectShippingMethod(string newMethod)
{
    method = newMethod;
}
