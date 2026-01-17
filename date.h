#pragma once
#ifndef DATE_H
#define DATE_H

#include <iosfwd>
#include <ctime>

namespace project {
    class Date {
    public:
        static constexpr int year_base = 1900;
        static constexpr int random_min_year = 1940;
        static constexpr int random_max_year = 2020;

        enum class Weekday {
            Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday
        };

        // Constructors
        Date();
        Date(int d, int m, int y);
        explicit Date(const char* p);
        explicit Date(std::time_t timer);

        // Getters
        [[nodiscard]] int get_month_day() const;
        [[nodiscard]] int get_month() const;
        [[nodiscard]] int get_year() const;
        [[nodiscard]] int get_year_day() const;
        [[nodiscard]] Weekday get_week_day() const;

        // Setters
        Date& set_month_day(int day);
        Date& set_month(int month);
        Date& set_year(int year);
        Date& set(int day, int mon, int year);

        // Arithmetic operators
        [[nodiscard]] Date operator-(int day) const;
        Date& operator+=(int day);
        Date& operator-=(int day);
        Date& operator++();
        Date operator++(int);
        Date& operator--();
        Date operator--(int);

        // Static functions
        [[nodiscard]] static Date random_date();
        static constexpr bool isleap(int y);

        // Friend declarations for comparison operators
        friend bool operator<(const Date&, const Date&);
        friend bool operator==(const Date&, const Date&);

        // Friend declarations for stream operators
        friend std::ostream& operator<<(std::ostream& os, const Date& date);
        friend std::istream& operator>>(std::istream& is, Date& date);

    private:
        int m_day;
        int m_month;
        int m_year;
    };

    // Global comparison operators
    [[nodiscard]] bool operator<=(const Date&, const Date&);
    [[nodiscard]] bool operator>(const Date&, const Date&);
    [[nodiscard]] bool operator>=(const Date&, const Date&);
    [[nodiscard]] bool operator!=(const Date&, const Date&);

    // Date arithmetic
    [[nodiscard]] int operator-(const Date& d1, const Date& d2);
    [[nodiscard]] Date operator+(const Date& date, int n);
    [[nodiscard]] Date operator+(int n, const Date&);

    // Weekday operators
    Date::Weekday& operator++(Date::Weekday& r);
    Date::Weekday operator++(Date::Weekday& r, int);
    Date::Weekday& operator--(Date::Weekday& r);
    Date::Weekday operator--(Date::Weekday& r, int);
}

#endif
