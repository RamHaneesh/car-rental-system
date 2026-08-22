#ifndef DATE_H
#define DATE_H

#include <string>
#include <iostream>

class Date {
public:
    int year;
    int month;
    int day;

    Date();
    Date(int y, int m, int d);

    // Parses a date from YYYY-MM-DD format
    static Date parse(const std::string& str);

    // Returns date in YYYY-MM-DD format
    std::string toString() const;

    // Converts date to number of days since a fixed epoch (Year 1)
    int toDays() const;

    // Constructs a Date from days since epoch
    static Date fromDays(int totalDays);

    // Adds a number of days to this date
    Date addDays(int days) const;

    // Calculates difference (this - other) in days
    int difference(const Date& other) const;

    // Checks if the date is valid (correct month/day values)
    bool isValid() const;

    // Comparison operators
    bool operator<(const Date& other) const;
    bool operator==(const Date& other) const;
    bool operator<=(const Date& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Date& date);
};

#endif // DATE_H
