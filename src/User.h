#ifndef USER_H
#define USER_H

#include <string>
#include <set>
#include <vector>
#include <iostream>

class User {
protected:
    std::string id;
    std::string name;
    std::string password;
    std::string role; // "customer", "employee", "admin"
    int fineAmount;   // accumulated unpaid fines
    int maxCars;      // maximum cars allowed to rent
    int rentDays;     // standard duration for rental in days
    int fineRate;     // fine rate per day late
    std::set<std::string> rentedVehicleIds; // set of vehicle registration numbers currently rented

public:
    User();
    User(const std::string& id, const std::string& name, const std::string& password, const std::string& role);
    virtual ~User();

    // Getters and Setters
    std::string getId() const;
    std::string getName() const;
    std::string getRole() const;
    bool checkPassword(const std::string& pwd) const;
    void setPassword(const std::string& newPwd);
    
    int getFineAmount() const;
    void addFine(int amount);
    void setFineAmount(int amount);
    void clearFine();

    int getMaxCars() const;
    int getRentDays() const;
    int getFineRate() const;

    const std::set<std::string>& getRentedVehicles() const;
    void addRentedVehicle(const std::string& regNo);
    void removeRentedVehicle(const std::string& regNo);
    void clearRentedVehicles();
    bool hasRentedVehicle(const std::string& regNo) const;
    int getRentedCount() const;

    // Serialization for database file
    virtual std::string serialize() const;
    static User* deserialize(const std::string& line);

    // Display user details
    virtual void display() const;
};

class Customer : public User {
public:
    Customer();
    Customer(const std::string& id, const std::string& name, const std::string& password);
};

class Employee : public User {
public:
    Employee();
    Employee(const std::string& id, const std::string& name, const std::string& password);
};

class Admin : public User {
public:
    Admin();
    Admin(const std::string& id, const std::string& name, const std::string& password);
};

#endif // USER_H
