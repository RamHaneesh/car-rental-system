#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <iostream>
#include "Date.h"

class Vehicle {
private:
    std::string company;
    std::string model;
    std::string regNo;
    std::string type; // "SUV", "Sedan", "Hatchback" etc.
    int pricePerDay;
    std::string rentedTo; // ID of the user, or "manager" if available
    Date dueDate;

public:
    Vehicle();
    Vehicle(const std::string& company, const std::string& model, const std::string& regNo, const std::string& type, int pricePerDay);

    // Getters and Setters
    std::string getCompany() const;
    std::string getModel() const;
    std::string getRegNo() const;
    std::string getType() const;
    int getPricePerDay() const;
    std::string getRentedTo() const;
    Date getDueDate() const;

    bool isAvailable() const;
    bool rentVehicle(const std::string& userId, const Date& returnDate);
    void returnVehicle();

    // Serialization
    std::string serialize() const;
    static Vehicle deserialize(const std::string& line);

    void display() const;

    // Overloaded operators
    bool operator<(const Vehicle& other) const;
    bool operator==(const Vehicle& other) const;
};

#endif // VEHICLE_H
