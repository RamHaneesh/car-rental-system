# OOP C++ Car Rental CLI System

A modular, clean Object-Oriented C++ Command Line Interface (CLI) application for a Car Rental System. The system simulates databases for users and vehicles using flat text files, manages active rentals via STL containers (`std::map`, `std::set`), implements role-based authentication, and automates rental duration and fine calculations.

---

## 1. Core Features

1. **Object-Oriented Design (OOP):** Modular architecture with distinct inheritance structures (Base `User` class with derived `Customer`, `Employee`, and `Admin` subclasses).
2. **Persistence Layer (File Databases):** Reads/writes user credentials, vehicle status, and transaction histories from and to separate text files:
   - `data/users.txt` (Contains registered user accounts, active rentals, and fines).
   - `data/vehicles.txt` (Contains car details, category, daily rent price, availability status, current renter ID, and due dates).
   - `data/transactions.txt` (Audit log recording all transactions: RENT, RETURN, PAY_FINE).
3. **STL-Based In-Memory Caches:** Leverages fast `std::map` lookups for in-memory collections of users and vehicles, and `std::set` to track rented items.
4. **Time & Fine Simulation:** Tracks rental durations and automates fine calculations if cars are returned late (users can pay fines via the client dashboard, and admins can fast-forward days to simulate dates passing).
5. **Interactive Help CLI & Professional Enhancements:**
   - **Password Masking:** Password entries during login and registration are hidden behind `****` characters.
   - **Rental Return Receipt:** Prints a clean layout summarizing the return details (due date, return date, late days, fine rate, and total fees).
   - **Admin Revenue Dashboard:** Computes fleet utilization %, total rental earnings, fines collected, and total revenue from transaction logs.

---

## 2. Directory & Database Architecture

The application automatically creates a `data/` folder in the project root to store flat-file databases.

```text
Car Rental System/
├── data/
│   ├── users.txt         # User profile database (ID, Name, Password, Fines, Active Rentals)
│   ├── vehicles.txt      # Fleet inventory database (Make, Model, RegNo, Cost, Renter, DueDate)
│   └── transactions.txt  # Transaction history logs (Append-only audit trail)
├── src/
│   ├── Date.h/.cpp       # Date parsing, comparisons, and days calculations
│   ├── User.h/.cpp       # Polymorphic User, Customer, Employee, Admin structures
│   ├── Vehicle.h/.cpp    # Car specifications, rental operations, and serializations
│   ├── RentalSystem.h/.cpp# Database operations, transaction logs, and rental checkouts
│   └── main.cpp          # Command Line Interface (CLI) menu system
└── CMakeLists.txt        # CMake build configuration
```

### Seeding Logic
If the program detects that `users.txt` or `vehicles.txt` are **missing** or **empty (0 bytes)**, it will automatically populate them with default records:
- **Default Admin Account:** ID: `admin` | Password: `admin`
- **Default Customer Account:** ID: `cust1` | Password: `password`
- **Default Employee Account:** ID: `emp1` | Password: `password`
- **Pre-seeded Fleet:** Toyota Camry (`CAR-101`), Honda CR-V (`CAR-202`), Ford Mustang (`CAR-303`), Chevrolet Bolt (`CAR-404`).

---

## 3. Build and Run Instructions

### Prerequisites
- A C++17 compatible compiler (e.g., GCC/MinGW, MSVC, Clang).
- CMake (optional).

### Building with CMake
From the project root directory, run:
```bash
# Configure the build directory
cmake -B build -S .

# Compile the executable
cmake --build build
```
Once built, run the executable:
- **Windows:** `build\Debug\CarRentalSystem.exe`
- **macOS/Linux:** `./build/CarRentalSystem`

### Manual Building (without CMake)
You can compile the source files directly using your compiler:
```bash
g++ -std=c++17 src/*.cpp -Isrc -o CarRentalSystem
```
Then run `./CarRentalSystem` (or `.\CarRentalSystem.exe` on Windows).

---

## 4. User Roles & Permissions

The system operates with three distinct user roles, each having specific privileges:

| Privilege / Action | Customer | Employee | Administrator |
| :--- | :---: | :---: | :---: |
| Browse Available Fleet | Yes | Yes | Yes |
| Rent Vehicles | Yes (Max 3) | Yes (Max 5) | No |
| Maximum Rental Duration | 25 Days | 25 Days | N/A |
| Late Return Fines | Yes ($10/day) | Yes ($10/day) | No |
| View Rented Cars & Dues | Yes | Yes | Yes (Any User) |
| Manage Fleet (Add/Delete Cars) | No | No | Yes |
| Manage Users (Add/Delete Users) | No | No | Yes |
| Change System Simulated Time | No | No | Yes |
| Read System Transaction logs | No | No | Yes |

---

## 5. Detailed Usage Guide

### A. Guest / Public Flow
When starting the application, you enter the main menu.
1.  **Register as Customer (Option 2):** Enter a unique ID (e.g. `cust2`), your full name, and your password.
2.  **Log In (Option 1):** Enter your ID and password. As you type, the password is secure and hidden behind `****` characters.
3.  **Exit (Option 4):** Close the program and display the exit ASCII logo.

### B. Customer & Employee Flow
Once logged in, you can manage your rentals:
1.  **Display Available Vehicles (Option 1):** View all cars that are currently available to rent.
2.  **Display Your Rented Vehicles (Option 2):** View all cars currently issued to your ID, their due dates, and if any are marked **LATE!** with calculated overdue fines.
3.  **Rent a Vehicle (Option 3):** 
    *   Enter the registration number of the car (e.g., `CAR-101`).
    *   Enter the duration you wish to rent (e.g. `5` days).
    *   *Note: If you have outstanding unpaid fines, you will be blocked from renting new vehicles.*
4.  **Return a Vehicle (Option 4):**
    *   Enter the registration number of your rental.
    *   The console will print a detailed **Rental Return Receipt** showing the due date, return date, late days (if any), fine rate, and total fees added.
5.  **View & Pay Fines (Option 5 & 6):** 
    *   Option 5 lists your outstanding fine balance.
    *   Option 6 lets you pay all outstanding fines to reset your balance.
6.  **Logout (Option 8):** Return to the main menu.

### C. Administrator Flow
Admin credentials default to ID `admin` and Password `admin`.
1.  **View Full Fleet Inventory (Option 1):** List all cars, their categories, rates, availability, and the ID of the user who rented them.
2.  **View Registered Users (Option 2):** List details, roles, outstanding fines, and active rentals for all users in the system.
3.  **Check Vehicle Driver Status (Option 3):** Find who is currently driving a particular car using its registration plate.
4.  **Manage Vehicles (Option 4 & 5):** 
    *   Option 4 lets you add a new vehicle specifying the model, registration number, category, and daily rental rate.
    *   Option 5 lets you remove a vehicle from inventory (if it is currently rented, it will automatically force-return it first).
5.  **Manage Users (Option 6 & 7):**
    *   Option 6 registers a new Customer, Employee, or Admin.
    *   Option 7 deletes a user ID from the database (it will force-return any active rentals they hold).
6.  **View Transaction Logs (Option 8):** View the audit trail listing every action (`RENT`, `RETURN`, `PAY_FINE`) with timestamps, costs, and fine amounts.
7.  **View Revenue Dashboard (Option 9):** Get a summary of:
    *   Total fleet count
    *   Active rentals and fleet utilization %
    *   Total rental earnings
    *   Total collected fines
    *   Total system revenue
8.  **Simulate Passing of Time (Option 10 & 11):**
    *   Option 10 prints the current simulated system date.
    *   Option 11 allows you to fast-forward the system date by adding days (e.g. `10` days). This is the primary way to test late return fines on customer accounts!

---

## 6. Troubleshooting

### "No vehicles in inventory" appears on first run
This occurs if the database files existed on disk but were empty. 
- **Solution:** Delete the `data/` directory and restart the program. The system will detect the missing directory and seed the databases.

### "g++ is not recognized as an internal or external command"
This means the C++ compiler path is not added to your Windows environment variables.
- **Solution:** Add `C:\msys64\ucrt64\bin` (or your MinGW installation path) to the `Path` variable in your Windows system settings, and restart your terminal.
