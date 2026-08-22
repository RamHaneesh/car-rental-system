#include "User.h"
#include <sstream>
#include <iomanip>

User::User() : id(""), name(""), password(""), role(""), fineAmount(0), maxCars(0), rentDays(0), fineRate(0) {}

User::User(const std::string& id, const std::string& name, const std::string& password, const std::string& role) 
    : id(id), name(name), password(password), role(role), fineAmount(0), maxCars(0), rentDays(0), fineRate(0) {}

User::~User() {}

std::string User::getId() const { return id; }
std::string User::getName() const { return name; }
std::string User::getRole() const { return role; }

bool User::checkPassword(const std::string& pwd) const { return password == pwd; }
void User::setPassword(const std::string& newPwd) { password = newPwd; }

int User::getFineAmount() const { return fineAmount; }
void User::addFine(int amount) { fineAmount += amount; }
void User::setFineAmount(int amount) { fineAmount = amount; }
void User::clearFine() { fineAmount = 0; }

int User::getMaxCars() const { return maxCars; }
int User::getRentDays() const { return rentDays; }
int User::getFineRate() const { return fineRate; }

const std::set<std::string>& User::getRentedVehicles() const { return rentedVehicleIds; }

void User::addRentedVehicle(const std::string& regNo) {
    rentedVehicleIds.insert(regNo);
}

void User::removeRentedVehicle(const std::string& regNo) {
    rentedVehicleIds.erase(regNo);
}

void User::clearRentedVehicles() {
    rentedVehicleIds.clear();
}

bool User::hasRentedVehicle(const std::string& regNo) const {
    return rentedVehicleIds.find(regNo) != rentedVehicleIds.end();
}

int User::getRentedCount() const {
    return rentedVehicleIds.size();
}

std::string User::serialize() const {
    std::stringstream ss;
    ss << id << "|" << name << "|" << password << "|" << role << "|" << fineAmount << "|" << maxCars << "|" << rentDays << "|" << fineRate << "|";
    
    bool first = true;
    for (const auto& regNo : rentedVehicleIds) {
        if (!first) ss << ",";
        ss << regNo;
        first = false;
    }
    return ss.str();
}

// Split utility function
static std::vector<std::string> splitString(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

User* User::deserialize(const std::string& line) {
    if (line.empty()) return nullptr;
    
    try {
        std::vector<std::string> parts = splitString(line, '|');
        if (parts.size() < 8) return nullptr;

        std::string id = parts[0];
        std::string name = parts[1];
        std::string password = parts[2];
        std::string role = parts[3];
        int fineAmount = std::stoi(parts[4]);
        int maxCars = std::stoi(parts[5]);
        int rentDays = std::stoi(parts[6]);
        int fineRate = std::stoi(parts[7]);

        User* user = nullptr;
        if (role == "admin") {
            user = new Admin(id, name, password);
        } else if (role == "employee") {
            user = new Employee(id, name, password);
        } else {
            user = new Customer(id, name, password);
        }

        user->setFineAmount(fineAmount);
        // Overwrite config values in case they were altered in file
        user->maxCars = maxCars;
        user->rentDays = rentDays;
        user->fineRate = fineRate;

        if (parts.size() > 8 && !parts[8].empty()) {
            std::vector<std::string> vehicles = splitString(parts[8], ',');
            for (const auto& regNo : vehicles) {
                if (!regNo.empty()) {
                    user->addRentedVehicle(regNo);
                }
            }
        }

        return user;
    } catch (...) {
        return nullptr;
    }
}

void User::display() const {
    std::cout << "User ID: " << id << "\n";
    std::cout << "Name:    " << name << "\n";
    std::cout << "Role:    " << role << "\n";
    std::cout << "Outstanding Fines: $" << fineAmount << "\n";
    std::cout << "Rented Vehicles Count: " << rentedVehicleIds.size() << "/" << (maxCars == 0 ? "Unlimited" : std::to_string(maxCars)) << "\n";
}

// Customer implementation
Customer::Customer() : User() {
    role = "customer";
    maxCars = 3;
    rentDays = 25;
    fineRate = 10;
}

Customer::Customer(const std::string& id, const std::string& name, const std::string& password)
    : User(id, name, password, "customer") {
    maxCars = 3;
    rentDays = 25;
    fineRate = 10;
}

// Employee implementation
Employee::Employee() : User() {
    role = "employee";
    maxCars = 5;
    rentDays = 25;
    fineRate = 10;
}

Employee::Employee(const std::string& id, const std::string& name, const std::string& password)
    : User(id, name, password, "employee") {
    maxCars = 5;
    rentDays = 25;
    fineRate = 10;
}

// Admin implementation
Admin::Admin() : User() {
    role = "admin";
    maxCars = 0; // Unlimited / Not applicable
    rentDays = 0;
    fineRate = 0;
}

Admin::Admin(const std::string& id, const std::string& name, const std::string& password)
    : User(id, name, password, "admin") {
    maxCars = 0;
    rentDays = 0;
    fineRate = 0;
}
