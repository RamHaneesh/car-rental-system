#include "Date.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <chrono>

Date::Date() {
    // Initialize to current local date
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm* localTime = std::localtime(&t);
    year = localTime->tm_year + 1900;
    month = localTime->tm_mon + 1;
    day = localTime->tm_mday;
}

Date::Date(int y, int m, int d) : year(y), month(m), day(d) {}

Date Date::parse(const std::string& str) {
    std::stringstream ss(str);
    int y, m, d;
    char c1, c2;
    if (ss >> y >> c1 >> m >> c2 >> d && c1 == '-' && c2 == '-') {
        Date date(y, m, d);
        if (date.isValid()) {
            return date;
        }
    }
    throw std::invalid_argument("Invalid date format. Expected YYYY-MM-DD");
}

std::string Date::toString() const {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(4) << year << "-"
       << std::setw(2) << month << "-"
       << std::setw(2) << day;
    return ss.str();
}

int Date::toDays() const {
    int y = year;
    int m = month;
    int d = day;
    if (m <= 2) {
        y--;
        m += 12;
    }
    // Rata Die algorithm (days since 0001-01-01)
    return 365 * y + y / 4 - y / 100 + y / 400 + (153 * (m - 3) + 2) / 5 + d - 306;
}

Date Date::fromDays(int g) {
    int y = (10000 * g + 14780) / 3652425;
    int ddd = g - (365 * y + y / 4 - y / 100 + y / 400);
    if (ddd < 0) {
        y--;
        ddd = g - (365 * y + y / 4 - y / 100 + y / 400);
    }
    int mi = (52 + 100 * ddd) / 3060;
    int da = ddd - (306 * mi + 5) / 10;
    int mo = mi + 3;
    if (mo > 12) {
        mo -= 12;
        y++;
    }
    return Date(y, mo, da);
}

Date Date::addDays(int days) const {
    return fromDays(toDays() + days);
}

int Date::difference(const Date& other) const {
    return this->toDays() - other.toDays();
}

bool Date::isValid() const {
    if (year < 1 || month < 1 || month > 12 || day < 1) return false;
    
    int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    
    // Check leap year
    bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (isLeap) daysInMonth[2] = 29;
    
    return day <= daysInMonth[month];
}

bool Date::operator<(const Date& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}

bool Date::operator==(const Date& other) const {
    return year == other.year && month == other.month && day == other.day;
}

bool Date::operator<=(const Date& other) const {
    return (*this < other) || (*this == other);
}

bool Date::operator>(const Date& other) const {
    return other < *this;
}

bool Date::operator>=(const Date& other) const {
    return !(*this < other);
}

bool Date::operator!=(const Date& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Date& date) {
    os << date.toString();
    return os;
}
