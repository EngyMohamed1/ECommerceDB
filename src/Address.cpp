#include "Address.h"
#include <iostream>
#include <string>
using namespace std;

Address::Address()
{ }
Address::~Address()
{ }
void Address::setAddress()
{
    cout << " Governorate: ";
    //cin.ignore();
    getline(cin, Governorate);
    cout << " City: ";
    getline(cin, City);
    cout << " Street: ";
    getline(cin, Street);
    cout << " Building Number: ";
    cin >> buildingNumber;
    cout << " Apartment Number: ";
    cin >> apartmentNumber;

}
void Address::printAddress()
{
  cout << Governorate << ", " << City << ", " << Street << ", " << buildingNumber << ", " << apartmentNumber << endl;
}
