# Object-Oriented C++ Car Rental System (CLI)

A modular, clean Object-Oriented C++ Command Line Interface (CLI) application for a Car Rental System. The system simulates databases for users and vehicles using flat text files, manages active rentals via STL containers (`std::map`, `std::set`), implements role-based authentication, and automates rental duration and fine calculations.

## Core Features
1. **Object-Oriented Design (OOP):** Modular architecture with distinct inheritance structures (Base `User` class with derived `Customer`, `Employee`, and `Admin` subclasses).
2. **Persistence Layer (File Databases):** Reads/writes user credentials, vehicle status, and transaction histories from and to separate text files:
   - `data/users.txt` (Contains registered user accounts, active rentals, and fines).
   - `data/vehicles.txt` (Contains car details, category, daily rent price, availability status, current renter ID, and due dates).
   - `data/transactions.txt` (Audit log recording all transactions: RENT, RETURN, PAY_FINE).
3. **STL-Based In-Memory Caches:** Leverages fast `std::map` lookups for in-memory collections of users and vehicles, and `std::set` to track rented items.
4. **Time & Fine Simulation:** Tracks rental durations and automates fine calculations if cars are returned late (users can pay fines via the client dashboard, and admins can fast-forward days to simulate dates passing).
5. **Interactive Help CLI:** Dynamic help screens for guests, logged-in customers/employees, and system administrators.

---

## Code Architecture
*   `src/Date.h` & `src/Date.cpp`: Custom calendar date utilities (leap years, parsing, difference calculation).
*   `src/User.h` & `src/User.cpp`: Base and derived user entities with data serialization.
*   `src/Vehicle.h` & `src/Vehicle.cpp`: Vehicle entity tracking availability and check-out data.
*   `src/RentalSystem.h` & `src/RentalSystem.cpp`: Core coordinator reading/writing database files and executing rental rules.
*   `src/main.cpp`: Entry point running the terminal dashboard menus.
*   `CMakeLists.txt`: Build configuration file.

---

## Build and Run Instructions

### Prerequisites
- A C++17 compatible compiler (e.g., GCC/MinGW, MSVC, Clang).
- CMake (optional but recommended).

### Building with CMake
From the project root directory, run:
```bash
# Configure the build directory
cmake -B build -S .

# Compile the executable
cmake --build build
```
Once built, run the executable:
- **Windows:** `build\Debug\CarRentalSystem.exe` (or `build/CarRentalSystem.exe` depending on compiler target).
- **macOS/Linux:** `./build/CarRentalSystem`

### Manual Building (without CMake)
You can compile the source files directly using your compiler:
```bash
g++ -std=c++17 src/*.cpp -Isrc -o CarRentalSystem
```
Then run `./CarRentalSystem` (or `CarRentalSystem.exe`).

---

## Default Administrator Credentials
- **User ID:** `admin`
- **Password:** `admin`
