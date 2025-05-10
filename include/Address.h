#ifndef ADDRESS_H
#define ADDRESS_H

#include <iostream>
#include <string>

using namespace std;

class Address{
public:
    Address();
    virtual ~Address();
    void setAddress();
    void printAddress();

    void setGovernorate(const std::string& gov) { Governorate = gov; }
    void setCity(const std::string& city) { City = city; }
    void setStreet(const std::string& street) { Street = street; }
    void setBuildingNumber(int num) { buildingNumber = num; }
    void setApartmentNumber(int num) { apartmentNumber = num; }

    const string& getGovernorate() const { return Governorate; }
    const string& getCity() const { return City; }
    const string& getStreet() const { return Street; }
    int getBuildingNumber() const { return buildingNumber; }
    int getApartmentNumber() const { return apartmentNumber; }

private:
    string Governorate;
    string City;
    string Street;

    int buildingNumber;
    int apartmentNumber;
};

#endif // ADDRESS_H
