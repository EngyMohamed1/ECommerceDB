#include <iostream>
#include <string>

#include "Payment.h"
#include "Address.h"
#include "Customer.h"

using namespace std;

Payment::Payment() {}
Payment::~Payment() {}

void Payment::processPayment()
{
    cout << " Payment ID: ";
    cin >> payID;

    /*cout << " Total Price: ";
    cin >> Price;*/

    cout << " Payment Method (Cash/Visa): ";
    cin >> Method;

    if(Method == "Cash")
    {
        paymentStatus = "Not Paid";
    }
    else if (Method == "Visa")
    {

        string cardNumber;
        string expiryDate;
        string CVV;

        cout << "Card Number: ";
        cin >> cardNumber;

        cout << "Expiry Date (MM/YY): ";
        cin >> expiryDate;

        cout << "CVV: ";
        cin >> CVV;

        if (cardNumber.length() == 16 && CVV.length() == 4 && expiryDate.size() == 5 && expiryDate[2] == '/')

        {
            cout << "Authorization successful for card ending in "
                 << cardNumber.substr(cardNumber.size() - 4) << endl;
            //cout << "Transaction of $" << Price << " has been settled.\n";
            paymentStatus = "Paid";
        }
        else
        {
            if (cardNumber.length() != 16)
            {
                cerr << "Invalid card number length.\n";
            }
            if (expiryDate.size() != 5 || expiryDate[2] != '/')
            {
                cerr << "Invalid expiry date format. Use MM/YY.\n";
            }
            if (CVV.length() != 4)
            {
                cerr << "Invalid CVV.\n";
            }
        }
    }
}
void Payment::printPayment() {
    cout << " Payment Id: " << payID << endl;
    cout << " Method: " << Method << endl;
    cout << " Payment Status: " << paymentStatus << endl;
    if (Method == "Visa") {
        cout << " Card Number: " << cardNumber << endl;
        cout << " Expiration Date: " << expirationDate << endl;
        cout << " CVV: " << cvv << endl;
        cout << " Cardholder Name: " << cardholderName << endl;
    }
}
