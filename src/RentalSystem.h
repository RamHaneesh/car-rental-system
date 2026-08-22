#ifndef RENTAL_SYSTEM_H
#define RENTAL_SYSTEM_H

#include <map>
#include <set>
#include <string>
#include <vector>
#include "User.h"
#include "Vehicle.h"
#include "Date.h"

class RentalSystem {
private:
    std::map<std::string, User*> users;       // ID -> User pointer
    std::map<std::string, Vehicle> vehicles;  // RegNo -> Vehicle
    Date systemDate;                          // Current simulated system date
    
    std::string usersFilePath;
    std::string vehiclesFilePath;
    std::string transactionsFilePath;

    void recordTransaction(const std::string& userId, const std::string& action, const std::string& regNo, int days, int cost, int fine);

public:
    RentalSystem();
    ~RentalSystem();

    // Database Load & Save
    void loadDatabase();
    void saveDatabase();

    // User Operations
    bool registerUser(const std::string& id, const std::string& name, const std::string& password, const std::string& role);
    User* loginUser(const std::string& id, const std::string& password);
    bool removeUser(const std::string& id);
    void displayAllUsers() const;
    User* getUser(const std::string& id);

    // Vehicle Operations
    bool addVehicle(const std::string& company, const std::string& model, const std::string& regNo, const std::string& type, int pricePerDay);
    bool removeVehicle(const std::string& regNo);
    void displayAllVehicles() const;
    void displayAvailableVehicles() const;
    Vehicle* getVehicle(const std::string& regNo);

    // Business Logic
    bool rentVehicle(const std::string& userId, const std::string& regNo, int rentDays);
    bool returnVehicle(const std::string& userId, const std::string& regNo);
    
    // Calculates fines for a single vehicle checked out by user
    int calculateFine(const Vehicle& vehicle) const;
    // Calculates total outstanding fines for a user based on all their rented vehicles
    int calculateTotalFines(const User* user) const;
    
    void payFines(const std::string& userId);

    // System Time Simulation
    Date getSystemDate() const;
    void advanceDays(int days);

    // Logs & Audit
    void displayTransactions() const;
    void displayDashboard() const;
};

#endif // RENTAL_SYSTEM_H
