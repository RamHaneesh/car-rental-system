#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include <conio.h>
#include "RentalSystem.h"

void showWelcomeArt() {
    std::cout << "\n";
    std::cout << "      __      __       .__                                \n";
    std::cout << "     /  \\    /  \\ ____ |  |   ____  ____   _____   ____   \n";
    std::cout << "     \\   \\/\\/   // __ \\|  | _/ ___\\/  _ \\ /     \\_/ __ \\  \n";
    std::cout << "      \\        /\\  ___/|  |_\\  \\__(  <_> )  Y Y  \\  ___/  \n";
    std::cout << "       \\__/\\__/  \\____ >____/\\____ >____/|__|_|__/\\____ > \n";
    std::cout << "                   CAR RENTAL CLI SYSTEM                  \n";
}

void showExitArt() {
    std::cout << "\n";
    std::cout << "  ____                 _ _                 \n";
    std::cout << " / ___| ___   ___   __| | |__  _   _  ___  \n";
    std::cout << "| |  _ / _ \\ / _ \\ / _` | '_ \\| | | |/ _ \\ \n";
    std::cout << "| |_| | (_) | (_) | (_| | |_) | |_| |  __/ \n";
    std::cout << " \\____|\\___/ \\___/ \\__,_|_.__/ \\__, |\\___| \n";
    std::cout << "                               |___/       \n";
}

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string getMaskedPassword() {
    std::string password = "";
    char ch;
    while (true) {
        ch = _getch();
        if (ch == '\r' || ch == '\n') { // Enter key
            break;
        } else if (ch == '\b') { // Backspace
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b";
            }
        } else if (ch >= 32 && ch <= 126) { // Printable chars
            password.push_back(ch);
            std::cout << '*';
        }
    }
    std::cout << "\n";
    return password;
}

bool containsInvalidChars(const std::string& str) {
    return str.find('|') != std::string::npos || str.find(',') != std::string::npos;
}

void displayGuestHelp() {
    std::cout << "\n================ GUEST HELP OPTIONS ================\n";
    std::cout << "1. Login     : Log into your account using ID and password.\n";
    std::cout << "2. Register  : Create a new Customer account.\n";
    std::cout << "3. Exit      : Close and terminate the program.\n";
    std::cout << "====================================================\n";
}

void displayUserHelp(const User* user) {
    std::cout << "\n================ CUSTOMER/EMPLOYEE HELP ================\n";
    std::cout << "1. Display Available Vehicles : View all cars ready for renting.\n";
    std::cout << "2. Display Rented Vehicles    : View cars currently issued to you.\n";
    std::cout << "3. Rent a Vehicle             : Checkout a car (requires ID & duration).\n";
    std::cout << "4. Return a Vehicle           : Return a checked-out car back to inventory.\n";
    std::cout << "5. View Outstanding Fines     : Check calculated fine fees.\n";
    std::cout << "6. Pay Outstanding Fines      : Pay current account balance.\n";
    std::cout << "7. View Current Date          : Print system date context.\n";
    std::cout << "8. Logout                     : Sign out of your profile.\n";
    std::cout << "9. Exit                       : Terminate the program.\n";
    std::cout << "========================================================\n";
}

void displayAdminHelp() {
    std::cout << "\n================ ADMIN HELP OPTIONS ================\n";
    std::cout << "1. Display All Vehicles       : List full fleet inventory details.\n";
    std::cout << "2. Display Registered Users    : List profile details of system users.\n";
    std::cout << "3. Check Vehicle Rental User  : Find who is currently driving a specific car.\n";
    std::cout << "4. Add a Vehicle              : Create a new vehicle in database.\n";
    std::cout << "5. Delete a Vehicle           : Remove a vehicle from registry.\n";
    std::cout << "6. Add a User                 : Register Customer, Employee, or Admin.\n";
    std::cout << "7. Delete a User              : Remove user and force checkout returns.\n";
    std::cout << "8. View Transaction Log       : Open system transaction auditing trail.\n";
    std::cout << "9. View Revenue Dashboard     : View fleet utilization and total system earnings.\n";
    std::cout << "10. Show System Date          : View current simulated calendar date.\n";
    std::cout << "11. Advance System Date       : Shift time forward to test late return fines.\n";
    std::cout << "12. Logout                    : Sign out of Administrator mode.\n";
    std::cout << "13. Exit                      : Terminate the program.\n";
    std::cout << "====================================================\n";
}

void handleCustomerMenu(RentalSystem& system, User* user) {
    bool logout = false;
    while (!logout) {
        std::cout << "\n[" << user->getName() << " (" << user->getRole() << ")] - Menu (Type h for Help)\n";
        if (!(std::cin >> input)) {
            std::cout << "\nInput stream closed. Exiting...\n";
            exit(0);
        }

        if (input == "h" || input == "H") {
            displayUserHelp(user);
            continue;
        }

        int choice = 0;
        try {
            choice = std::stoi(input);
        } catch (...) {
            std::cout << "Invalid command. Enter 'h' for help.\n";
            continue;
        }

        switch (choice) {
            case 1:
                system.displayAvailableVehicles();
                break;
            case 2: {
                const auto& rented = user->getRentedVehicles();
                if (rented.empty()) {
                    std::cout << "You currently have no rented vehicles.\n";
                } else {
                    std::cout << "\nCurrently Rented Vehicles:\n";
                    for (const auto& regNo : rented) {
                        Vehicle* v = system.getVehicle(regNo);
                        if (v) {
                            int potentialFine = system.calculateFine(*v);
                            std::cout << "- " << v->getCompany() << " " << v->getModel() 
                                      << " [" << regNo << "] | Due: " << v->getDueDate();
                            if (potentialFine > 0) {
                                std::cout << " (LATE! Current Fine: $" << potentialFine << ")";
                            }
                            std::cout << "\n";
                        }
                    }
                }
                break;
            }
            case 3: {
                std::string regNo;
                int days;
                std::cout << "Enter vehicle registration number to rent: ";
                std::cin >> regNo;
                std::cout << "Enter rental duration in days (max limit depends on role): ";
                if (!(std::cin >> days) || days <= 0) {
                    std::cout << "Invalid duration.\n";
                    clearInput();
                    break;
                }
                if (days > user->getRentDays()) {
                    std::cout << "Error: Your role (" << user->getRole() << ") allows a maximum checkout of " << user->getRentDays() << " days.\n";
                    break;
                }
                if (system.rentVehicle(user->getId(), regNo, days)) {
                    std::cout << "Successfully rented vehicle " << regNo << "!\n";
                }
                break;
            }
            case 4: {
                std::string regNo;
                std::cout << "Enter vehicle registration number to return: ";
                std::cin >> regNo;
                if (system.returnVehicle(user->getId(), regNo)) {
                    std::cout << "Vehicle returned successfully.\n";
                }
                break;
            }
            case 5: {
                int totalFines = system.calculateTotalFines(user);
                std::cout << "Your total outstanding fine balance: $" << totalFines << "\n";
                break;
            }
            case 6:
                system.payFines(user->getId());
                break;
            case 7:
                std::cout << "Current System Date: " << system.getSystemDate() << "\n";
                break;
            case 8:
                logout = true;
                std::cout << "Logged out.\n";
                break;
            case 9:
                showExitArt();
                exit(0);
            default:
                std::cout << "Invalid choice. Enter 'h' for help.\n";
        }
    }
}

void handleAdminMenu(RentalSystem& system, User* admin) {
    bool logout = false;
    while (!logout) {
        std::cout << "\n[Admin Menu] - (Type h for Help)\n";
        if (!(std::cin >> input)) {
            std::cout << "\nInput stream closed. Exiting...\n";
            exit(0);
        }

        if (input == "h" || input == "H") {
            displayAdminHelp();
            continue;
        }

        int choice = 0;
        try {
            choice = std::stoi(input);
        } catch (...) {
            std::cout << "Invalid command. Enter 'h' for help.\n";
            continue;
        }

        switch (choice) {
            case 1:
                system.displayAllVehicles();
                break;
            case 2:
                system.displayAllUsers();
                break;
            case 3: {
                std::string regNo;
                std::cout << "Enter vehicle registration number: ";
                std::cin >> regNo;
                Vehicle* v = system.getVehicle(regNo);
                if (v) {
                    if (v->isAvailable()) {
                        std::cout << "Vehicle " << regNo << " is currently available.\n";
                    } else {
                        std::cout << "Vehicle " << regNo << " is rented to user: " << v->getRentedTo() 
                                  << " (Due: " << v->getDueDate() << ")\n";
                    }
                } else {
                    std::cout << "Vehicle not found.\n";
                }
                break;
            }
            case 4: {
                std::string comp, model, reg, type;
                int price;
                std::cout << "Enter vehicle company: ";
                if (!(std::cin >> comp)) { exit(0); }
                std::cout << "Enter vehicle model: ";
                if (!(std::cin >> model)) { exit(0); }
                std::cout << "Enter registration plate (unique): ";
                if (!(std::cin >> reg)) { exit(0); }
                std::cout << "Enter category type (SUV/Sedan/Hatchback): ";
                if (!(std::cin >> type)) { exit(0); }
                if (containsInvalidChars(comp) || containsInvalidChars(model) || containsInvalidChars(reg) || containsInvalidChars(type)) {
                    std::cout << "Error: Input fields cannot contain '|' or ',' characters.\n";
                    break;
                }
                std::cout << "Enter daily rent rate ($): ";
                if (!(std::cin >> price) || price <= 0) {
                    std::cout << "Invalid price.\n";
                    clearInput();
                    break;
                }
                if (system.addVehicle(comp, model, reg, type, price)) {
                    std::cout << "Vehicle added successfully!\n";
                } else {
                    std::cout << "Error: Vehicle registration plate already exists.\n";
                }
                break;
            }
            case 5: {
                std::string reg;
                std::cout << "Enter registration plate of vehicle to remove: ";
                std::cin >> reg;
                if (system.removeVehicle(reg)) {
                    std::cout << "Vehicle deleted successfully.\n";
                } else {
                    std::cout << "Vehicle not found.\n";
                }
                break;
            }
            case 6: {
                std::string id, name, pass, role;
                std::cout << "Enter user ID (unique): ";
                if (!(std::cin >> id)) { exit(0); }
                if (containsInvalidChars(id)) {
                    std::cout << "Error: ID cannot contain '|' or ',' characters.\n";
                    break;
                }
                std::cout << "Enter user Full Name: ";
                std::cin.ignore();
                if (!std::getline(std::cin, name)) { exit(0); }
                if (containsInvalidChars(name)) {
                    std::cout << "Error: Name cannot contain '|' or ',' characters.\n";
                    break;
                }
                std::cout << "Enter password: ";
                pass = getMaskedPassword();
                if (containsInvalidChars(pass)) {
                    std::cout << "Error: Password cannot contain '|' or ',' characters.\n";
                    break;
                }
                std::cout << "Enter role (customer/employee/admin): ";
                if (!(std::cin >> role)) { exit(0); }
                if (role != "customer" && role != "employee" && role != "admin") {
                    std::cout << "Invalid role type.\n";
                    break;
                }
                if (system.registerUser(id, name, pass, role)) {
                    std::cout << "User registered successfully!\n";
                } else {
                    std::cout << "Error: User ID already exists.\n";
                }
                break;
            }
            case 7: {
                std::string id;
                std::cout << "Enter user ID to delete: ";
                std::cin >> id;
                if (system.removeUser(id)) {
                    std::cout << "User deleted successfully.\n";
                } else {
                    std::cout << "Cannot delete user (ID may be admin, or non-existent).\n";
                }
                break;
            }
            case 8:
                system.displayTransactions();
                break;
            case 9:
                system.displayDashboard();
                break;
            case 10:
                std::cout << "Current Simulated Date: " << system.getSystemDate() << "\n";
                break;
            case 11: {
                int days;
                std::cout << "Enter number of days to shift system time forward: ";
                if (std::cin >> days && days > 0) {
                    system.advanceDays(days);
                } else {
                    std::cout << "Invalid number of days.\n";
                    clearInput();
                }
                break;
            }
            case 12:
                logout = true;
                std::cout << "Logged out.\n";
                break;
            case 13:
                showExitArt();
                exit(0);
            default:
                std::cout << "Invalid choice. Enter 'h' for help.\n";
        }
    }
}

int main() {
    showWelcomeArt();
    RentalSystem system;

    std::cout << "System initialized. Default admin credentials - ID: admin | Password: admin\n";

    while (true) {
        std::cout << "\n================ MAIN MENU ================\n";
        std::cout << "Current Date: " << system.getSystemDate() << "\n";
        std::cout << "1. Login\n";
        std::cout << "2. Register Customer\n";
        std::cout << "3. Show Help\n";
        std::cout << "4. Exit\n";
        std::cout << "===========================================\n";
        if (!(std::cin >> input)) {
            std::cout << "\nInput stream closed. Exiting...\n";
            break;
        }
        int choice = 0;
        try {
            choice = std::stoi(input);
        } catch (...) {
            std::cout << "Invalid input. Enter 1-4.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                std::string id, pass;
                std::cout << "Enter ID: ";
                if (!(std::cin >> id)) { exit(0); }
                std::cout << "Enter Password: ";
                pass = getMaskedPassword();
                
                User* user = system.loginUser(id, pass);
                if (user) {
                    std::cout << "\nLogin Successful! Welcome, " << user->getName() << ".\n";
                    if (user->getRole() == "admin") {
                        handleAdminMenu(system, user);
                    } else {
                        handleCustomerMenu(system, user);
                    }
                } else {
                    std::cout << "Error: Invalid ID or Password.\n";
                }
                break;
            }
            case 2: {
                std::string id, name, pass;
                std::cout << "Enter new customer ID (unique): ";
                if (!(std::cin >> id)) { exit(0); }
                if (containsInvalidChars(id)) {
                    std::cout << "Error: ID cannot contain '|' or ',' characters.\n";
                    break;
                }
                std::cout << "Enter Full Name: ";
                std::cin.ignore();
                if (!std::getline(std::cin, name)) { exit(0); }
                if (containsInvalidChars(name)) {
                    std::cout << "Error: Name cannot contain '|' or ',' characters.\n";
                    break;
                }
                std::cout << "Enter password: ";
                pass = getMaskedPassword();
                if (containsInvalidChars(pass)) {
                    std::cout << "Error: Password cannot contain '|' or ',' characters.\n";
                    break;
                }
                
                if (system.registerUser(id, name, pass, "customer")) {
                    std::cout << "Customer registered successfully! You can now log in.\n";
                } else {
                    std::cout << "Error: User ID already exists.\n";
                }
                break;
            }
            case 3:
                displayGuestHelp();
                break;
            case 4:
                showExitArt();
                return 0;
            default:
                std::cout << "Invalid choice. Please enter a value between 1 and 4.\n";
        }
    }

    return 0;
}
