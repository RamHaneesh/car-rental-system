#include "RentalSystem.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <sstream>

RentalSystem::RentalSystem() {
    usersFilePath = "data/users.txt";
    vehiclesFilePath = "data/vehicles.txt";
    transactionsFilePath = "data/transactions.txt";

    // Ensure the data directory exists (C++17 filesystem)
    std::filesystem::create_directories("data");
    
    // Initialize system date to current real-time date
    systemDate = Date();

    loadDatabase();
}

RentalSystem::~RentalSystem() {
    saveDatabase();
    // Clean up dynamic memory
    for (auto& pair : users) {
        delete pair.second;
    }
}

void RentalSystem::loadDatabase() {
    // Load Users
    std::ifstream usersFile(usersFilePath);
    if (!usersFile.is_open()) {
        // Seed default admin and initial users if file doesn't exist
        std::cout << "Initializing database. Creating default admin...\n";
        registerUser("admin", "Admin Manager", "admin", "admin");
        registerUser("cust1", "John Doe", "password", "customer");
        registerUser("emp1", "Alice Smith", "password", "employee");
    } else {
        std::string line;
        while (std::getline(usersFile, line)) {
            if (line.empty()) continue;
            User* u = User::deserialize(line);
            if (u) {
                users[u->getId()] = u;
            }
        }
        usersFile.close();
    }

    // Load Vehicles
    std::ifstream vehiclesFile(vehiclesFilePath);
    if (!vehiclesFile.is_open()) {
        std::cout << "Initializing car database. Seeding initial fleet...\n";
        addVehicle("Toyota", "Camry", "CAR-101", "Sedan", 50);
        addVehicle("Honda", "CR-V", "CAR-202", "SUV", 75);
        addVehicle("Ford", "Mustang", "CAR-303", "Sports", 120);
        addVehicle("Chevrolet", "Bolt", "CAR-404", "Electric", 60);
    } else {
        std::string line;
        while (std::getline(vehiclesFile, line)) {
            if (line.empty()) continue;
            Vehicle v = Vehicle::deserialize(line);
            if (!v.getRegNo().empty()) {
                vehicles[v.getRegNo()] = v;
            }
        }
        vehiclesFile.close();
    }
}

void RentalSystem::saveDatabase() {
    // Save Users
    std::ofstream usersFile(usersFilePath);
    if (usersFile.is_open()) {
        for (const auto& pair : users) {
            usersFile << pair.second->serialize() << "\n";
        }
        usersFile.close();
    }

    // Save Vehicles
    std::ofstream vehiclesFile(vehiclesFilePath);
    if (vehiclesFile.is_open()) {
        for (const auto& pair : vehicles) {
            vehiclesFile << pair.second.serialize() << "\n";
        }
        vehiclesFile.close();
    }
}

bool RentalSystem::registerUser(const std::string& id, const std::string& name, const std::string& password, const std::string& role) {
    if (users.find(id) != users.end()) {
        return false; // User ID already exists
    }

    User* newUser = nullptr;
    if (role == "admin") {
        newUser = new Admin(id, name, password);
    } else if (role == "employee") {
        newUser = new Employee(id, name, password);
    } else {
        newUser = new Customer(id, name, password);
    }

    users[id] = newUser;
    saveDatabase();
    return true;
}

User* RentalSystem::loginUser(const std::string& id, const std::string& password) {
    auto it = users.find(id);
    if (it != users.end()) {
        if (it->second->checkPassword(password)) {
            return it->second;
        }
    }
    return nullptr;
}

bool RentalSystem::removeUser(const std::string& id) {
    auto it = users.find(id);
    if (it == users.end() || id == "admin") {
        return false; // Cannot remove admin or non-existent user
    }

    User* user = it->second;
    
    // Return all currently rented vehicles back to system
    if (user->getRentedCount() > 0) {
        std::vector<std::string> rentedRegs(user->getRentedVehicles().begin(), user->getRentedVehicles().end());
        for (const auto& regNo : rentedRegs) {
            returnVehicle(id, regNo);
        }
    }

    delete user;
    users.erase(it);
    saveDatabase();
    return true;
}

void RentalSystem::displayAllUsers() const {
    std::cout << "\n================ Registered Users ================\n";
    for (const auto& pair : users) {
        pair.second->display();
        std::cout << "--------------------------------------------------\n";
    }
}

User* RentalSystem::getUser(const std::string& id) {
    auto it = users.find(id);
    if (it != users.end()) {
        return it->second;
    }
    return nullptr;
}

bool RentalSystem::addVehicle(const std::string& company, const std::string& model, const std::string& regNo, const std::string& type, int pricePerDay) {
    if (vehicles.find(regNo) != vehicles.end()) {
        return false; // Duplicate Registration Number
    }
    vehicles[regNo] = Vehicle(company, model, regNo, type, pricePerDay);
    saveDatabase();
    return true;
}

bool RentalSystem::removeVehicle(const std::string& regNo) {
    auto it = vehicles.find(regNo);
    if (it == vehicles.end()) {
        return false; // Vehicle not found
    }

    Vehicle& v = it->second;
    if (!v.isAvailable()) {
        // If vehicle is rented, we must first clear it from user profile
        std::string userId = v.getRentedTo();
        User* user = getUser(userId);
        if (user) {
            user->removeRentedVehicle(regNo);
        }
    }

    vehicles.erase(it);
    saveDatabase();
    return true;
}

void RentalSystem::displayAllVehicles() const {
    std::cout << "\n================ Total Inventory ================\n";
    if (vehicles.empty()) {
        std::cout << "No vehicles in inventory.\n";
        return;
    }
    for (const auto& pair : vehicles) {
        pair.second.display();
        std::cout << "--------------------------------------------------\n";
    }
}

void RentalSystem::displayAvailableVehicles() const {
    std::cout << "\n================ Available Vehicles ================\n";
    bool found = false;
    for (const auto& pair : vehicles) {
        if (pair.second.isAvailable()) {
            pair.second.display();
            std::cout << "--------------------------------------------------\n";
            found = true;
        }
    }
    if (!found) {
        std::cout << "All vehicles are currently rented out.\n";
    }
}

Vehicle* RentalSystem::getVehicle(const std::string& regNo) {
    auto it = vehicles.find(regNo);
    if (it != vehicles.end()) {
        return &(it->second);
    }
    return nullptr;
}

bool RentalSystem::rentVehicle(const std::string& userId, const std::string& regNo, int rentDays) {
    User* user = getUser(userId);
    Vehicle* vehicle = getVehicle(regNo);

    if (!user || !vehicle) return false;
    if (user->getRole() == "admin") {
        std::cout << "Administrators cannot rent vehicles.\n";
        return false;
    }

    // Check outstanding fines
    if (user->getFineAmount() > 0) {
        std::cout << "Access Denied: You have unpaid fines ($" << user->getFineAmount() << "). Please clear them first.\n";
        return false;
    }

    // Check vehicle availability
    if (!vehicle->isAvailable()) {
        std::cout << "Vehicle " << regNo << " is not available.\n";
        return false;
    }

    // Check user rental limits
    if (user->getRentedCount() >= user->getMaxCars()) {
        std::cout << "Limit Exceeded: You cannot rent more than " << user->getMaxCars() << " vehicles.\n";
        return false;
    }

    // Rent
    Date returnDate = systemDate.addDays(rentDays);
    if (vehicle->rentVehicle(userId, returnDate)) {
        user->addRentedVehicle(regNo);
        int totalCost = vehicle->getPricePerDay() * rentDays;
        
        recordTransaction(userId, "RENT", regNo, rentDays, totalCost, 0);
        saveDatabase();
        return true;
    }
    return false;
}

bool RentalSystem::returnVehicle(const std::string& userId, const std::string& regNo) {
    User* user = getUser(userId);
    Vehicle* vehicle = getVehicle(regNo);

    if (!user || !vehicle) return false;
    if (!user->hasRentedVehicle(regNo)) {
        std::cout << "Error: User has not rented this vehicle.\n";
        return false;
    }

    // Calculate late fines
    int fine = calculateFine(*vehicle);
    if (fine > 0) {
        user->addFine(fine);
        std::cout << "Return Alert: Late return detected. A fine of $" << fine << " has been added to your account.\n";
    }

    // Perform return
    user->removeRentedVehicle(regNo);
    vehicle->returnVehicle();

    recordTransaction(userId, "RETURN", regNo, 0, 0, fine);
    saveDatabase();
    return true;
}

int RentalSystem::calculateFine(const Vehicle& vehicle) const {
    if (vehicle.isAvailable()) return 0;

    Date dueDate = vehicle.getDueDate();
    if (systemDate <= dueDate) return 0; // Not late

    int daysLate = systemDate.difference(dueDate);
    std::string userId = vehicle.getRentedTo();
    
    // Find the user's fine rate
    auto it = users.find(userId);
    if (it != users.end()) {
        return daysLate * it->second->getFineRate();
    }
    return daysLate * 10; // Default fallback fine rate
}

int RentalSystem::calculateTotalFines(const User* user) const {
    if (!user) return 0;
    
    int activeFines = user->getFineAmount();
    
    // Check if any currently rented vehicles are late and compile their potential fines
    for (const auto& regNo : user->getRentedVehicles()) {
        auto it = vehicles.find(regNo);
        if (it != vehicles.end()) {
            activeFines += calculateFine(it->second);
        }
    }
    return activeFines;
}

void RentalSystem::payFines(const std::string& userId) {
    User* user = getUser(userId);
    if (user) {
        int fineToPay = calculateTotalFines(user);
        if (fineToPay > 0) {
            std::cout << "Processing fine payment of $" << fineToPay << "...\n";
            user->clearFine();
            recordTransaction(userId, "PAY_FINE", "", 0, fineToPay, 0);
            saveDatabase();
            std::cout << "Fines successfully paid!\n";
        } else {
            std::cout << "You have no outstanding fines.\n";
        }
    }
}

Date RentalSystem::getSystemDate() const {
    return systemDate;
}

void RentalSystem::advanceDays(int days) {
    if (days <= 0) return;
    systemDate = systemDate.addDays(days);
    std::cout << "System clock advanced by " << days << " days. New Date: " << systemDate << "\n";
}

void RentalSystem::recordTransaction(const std::string& userId, const std::string& action, const std::string& regNo, int days, int cost, int fine) {
    std::ofstream txFile(transactionsFilePath, std::ios::app);
    if (txFile.is_open()) {
        txFile << systemDate.toString() << "|"
               << userId << "|"
               << action << "|"
               << regNo << "|"
               << days << "|"
               << cost << "|"
               << fine << "\n";
        txFile.close();
    }
}

void RentalSystem::displayTransactions() const {
    std::ifstream txFile(transactionsFilePath);
    std::cout << "\n================ Transaction Audit Log ================\n";
    if (!txFile.is_open()) {
        std::cout << "No transactions recorded yet.\n";
        return;
    }

    std::string line;
    std::cout << std::left 
              << std::setw(12) << "Date"
              << std::setw(10) << "User ID"
              << std::setw(10) << "Action"
              << std::setw(10) << "Reg No"
              << std::setw(8)  << "Duration"
              << std::setw(8)  << "Cost"
              << std::setw(8)  << "Fine" << "\n";
    std::cout << "----------------------------------------------------------------------\n";

    while (std::getline(txFile, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string date, uid, act, reg;
        std::string dur, cost, fine;
        
        std::getline(ss, date, '|');
        std::getline(ss, uid, '|');
        std::getline(ss, act, '|');
        std::getline(ss, reg, '|');
        std::getline(ss, dur, '|');
        std::getline(ss, cost, '|');
        std::getline(ss, fine, '|');

        std::cout << std::left 
                  << std::setw(12) << date
                  << std::setw(10) << uid
                  << std::setw(10) << act
                  << std::setw(10) << (reg.empty() ? "-" : reg)
                  << std::setw(8)  << (dur == "0" ? "-" : dur + " d")
                  << std::setw(8)  << (cost == "0" ? "-" : "$" + cost)
                  << std::setw(8)  << (fine == "0" ? "-" : "$" + fine) << "\n";
    }
    txFile.close();
    std::cout << "======================================================================\n";
}
