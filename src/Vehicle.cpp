#include "Vehicle.h"
#include <sstream>
#include <vector>

Vehicle::Vehicle() 
    : company(""), model(""), regNo(""), type(""), pricePerDay(0), rentedTo("manager"), dueDate(Date(1970, 1, 1)) {}

Vehicle::Vehicle(const std::string& company, const std::string& model, const std::string& regNo, const std::string& type, int pricePerDay)
    : company(company), model(model), regNo(regNo), type(type), pricePerDay(pricePerDay), rentedTo("manager"), dueDate(Date(1970, 1, 1)) {}

std::string Vehicle::getCompany() const { return company; }
std::string Vehicle::getModel() const { return model; }
std::string Vehicle::getRegNo() const { return regNo; }
std::string Vehicle::getType() const { return type; }
int Vehicle::getPricePerDay() const { return pricePerDay; }
std::string Vehicle::getRentedTo() const { return rentedTo; }
Date Vehicle::getDueDate() const { return dueDate; }

bool Vehicle::isAvailable() const {
    return rentedTo == "manager";
}

bool Vehicle::rentVehicle(const std::string& userId, const Date& returnDate) {
    if (!isAvailable()) return false;
    rentedTo = userId;
    dueDate = returnDate;
    return true;
}

void Vehicle::returnVehicle() {
    rentedTo = "manager";
    dueDate = Date(1970, 1, 1);
}

std::string Vehicle::serialize() const {
    std::stringstream ss;
    ss << company << "|" << model << "|" << regNo << "|" << type << "|" << pricePerDay << "|" << rentedTo << "|" << dueDate.toString();
    return ss.str();
}

static std::vector<std::string> splitString(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

Vehicle Vehicle::deserialize(const std::string& line) {
    try {
        std::vector<std::string> parts = splitString(line, '|');
        if (parts.size() < 7) {
            return Vehicle();
        }
        
        std::string company = parts[0];
        std::string model = parts[1];
        std::string regNo = parts[2];
        std::string type = parts[3];
        int pricePerDay = std::stoi(parts[4]);
        std::string rentedTo = parts[5];
        Date dueDate = Date::parse(parts[6]);

        Vehicle v(company, model, regNo, type, pricePerDay);
        v.rentedTo = rentedTo;
        v.dueDate = dueDate;
        return v;
    } catch (...) {
        return Vehicle();
    }
}

void Vehicle::display() const {
    std::cout << "Make/Model:   " << company << " " << model << " (" << type << ")\n";
    std::cout << "Reg No:       " << regNo << "\n";
    std::cout << "Price/Day:    $" << pricePerDay << "\n";
    std::cout << "Status:       ";
    if (isAvailable()) {
        std::cout << "Available\n";
    } else {
        std::cout << "Rented to user [" << rentedTo << "] (Due: " << dueDate << ")\n";
    }
}

bool Vehicle::operator<(const Vehicle& other) const {
    return regNo < other.regNo;
}

bool Vehicle::operator==(const Vehicle& other) const {
    return regNo == other.regNo;
}
