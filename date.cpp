#include "date.h"
#include <iostream>
#include <sstream>
#include <ctime>
#include <random>
#include <iomanip>

namespace project {

    // Lookup tables
    constexpr const char* month_names[] = {
        "Ocak", "Subat", "Mart", "Nisan", "Mayis", "Haziran",
        "Temmuz", "Agustos", "Eylul", "Ekim", "Kasim", "Aralik"
    };

    constexpr const char* day_names[] = {
        "Pazar", "Pazartesi", "Sali", "Carsamba",
        "Persembe", "Cuma", "Cumartesi"
    };

    constexpr int month_days[2][12] = {
        {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
    };

    // Helper functions
    static int days_in_month(int month, int year) {
        return month_days[Date::isleap(year)][month - 1];
    }

    static int year_to_days(int year) {
        int y = year - 1;
        return y * 365 + y / 4 - y / 100 + y / 400;
    }

    static int date_to_days(int day, int month, int year) {
        int total = year_to_days(year);
        for (int m = 1; m < month; ++m) {
            total += days_in_month(m, year);
        }
        total += day;
        return total;
    }

    static void days_to_date(int total_days, int& day, int& month, int& year) {
        year = Date::year_base;
        while (total_days > (Date::isleap(year) ? 366 : 365)) {
            total_days -= (Date::isleap(year) ? 366 : 365);
            ++year;
        }

        month = 1;
        while (total_days > days_in_month(month, year)) {
            total_days -= days_in_month(month, year);
            ++month;
        }
        day = total_days;
    }

    // Static member function implementations
    constexpr bool Date::isleap(int y) {
        return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    }

    Date Date::random_date() {
        static std::mt19937 eng{ std::random_device{}() };
        std::uniform_int_distribution<int> year_dist{ random_min_year, random_max_year };
        std::uniform_int_distribution<int> month_dist{ 1, 12 };

        int year = year_dist(eng);
        int month = month_dist(eng);
        std::uniform_int_distribution<int> day_dist{ 1, days_in_month(month, year) };
        int day = day_dist(eng);

        return Date{ day, month, year };
    }

    // Constructors
    Date::Date() : m_day(1), m_month(1), m_year(year_base) {}

    Date::Date(int d, int m, int y) : m_day(d), m_month(m), m_year(y) {}

    Date::Date(const char* p) {
        std::istringstream iss(p);
        char delim;
        iss >> m_day >> delim >> m_month >> delim >> m_year;
    }

    Date::Date(std::time_t timer) {

        std::tm time_info;
        localtime_s(&time_info, &timer);

        m_day = time_info.tm_mday;
        m_month = time_info.tm_mon + 1;
        m_year = time_info.tm_year + 1900;
    }

    // Getters
    int Date::get_month_day() const {
        return m_day;
    }

    int Date::get_month() const {
        return m_month;
    }

    int Date::get_year() const {
        return m_year;
    }

    int Date::get_year_day() const {
        int day_of_year = m_day;
        for (int m = 1; m < m_month; ++m) {
            day_of_year += days_in_month(m, m_year);
        }
        return day_of_year;
    }

    Date::Weekday Date::get_week_day() const {
        static constexpr int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
        int y = m_year;
        int m = m_month;
        int d = m_day;

        if (m < 3)
            y--;

        int day = (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
        return static_cast<Weekday>(day);
    }

    // Setters
    Date& Date::set_month_day(int day) {
        m_day = day;
        return *this;
    }

    Date& Date::set_month(int month) {
        m_month = month;
        return *this;
    }

    Date& Date::set_year(int year) {
        m_year = year;
        return *this;
    }

    Date& Date::set(int day, int mon, int year) {
        m_day = day;
        m_month = mon;
        m_year = year;
        return *this;
    }

    // Arithmetic operators
    Date Date::operator-(int day) const {
        int total = date_to_days(m_day, m_month, m_year) - date_to_days(1, 1, year_base);
        total -= day;

        int d, m, y;
        days_to_date(total, d, m, y);
        return Date{ d, m, y };
    }

    Date& Date::operator+=(int day) {
        int total = date_to_days(m_day, m_month, m_year) - date_to_days(1, 1, year_base);
        total += day;

        days_to_date(total, m_day, m_month, m_year);
        return *this;
    }

    Date& Date::operator-=(int day) {
        int total = date_to_days(m_day, m_month, m_year) - date_to_days(1, 1, year_base);
        total -= day;

        days_to_date(total, m_day, m_month, m_year);
        return *this;
    }

    Date& Date::operator++() {
        *this += 1;
        return *this;
    }

    Date Date::operator++(int) {
        Date temp{ *this };
        ++(*this);
        return temp;
    }

    Date& Date::operator--() {
        *this -= 1;
        return *this;
    }

    Date Date::operator--(int) {
        Date temp{ *this };
        --(*this);
        return temp;
    }

    // Comparison operators
    bool operator<(const Date& d1, const Date& d2) {
        if (d1.m_year != d2.m_year)
            return d1.m_year < d2.m_year;
        if (d1.m_month != d2.m_month)
            return d1.m_month < d2.m_month;
        return d1.m_day < d2.m_day;
    }

    bool operator==(const Date& d1, const Date& d2) {
        return d1.m_day == d2.m_day && d1.m_month == d2.m_month && d1.m_year == d2.m_year;
    }

    bool operator<=(const Date& d1, const Date& d2) {
        return d1 < d2 || d1 == d2;
    }

    bool operator>(const Date& d1, const Date& d2) {
        return !(d1 <= d2);
    }

    bool operator>=(const Date& d1, const Date& d2) {
        return !(d1 < d2);
    }

    bool operator!=(const Date& d1, const Date& d2) {
        return !(d1 == d2);
    }

    // Date difference
    int operator-(const Date& d1, const Date& d2) {
        int days1 = date_to_days(d1.get_month_day(), d1.get_month(), d1.get_year());
        int days2 = date_to_days(d2.get_month_day(), d2.get_month(), d2.get_year());
        return days1 - days2;
    }

    // Date + int
    Date operator+(const Date& date, int n) {
        Date temp{ date };
        temp += n;
        return temp;
    }

    Date operator+(int n, const Date& date) {
        return date + n;
    }

    // Weekday operators
    Date::Weekday& operator++(Date::Weekday& r) {
        r = static_cast<Date::Weekday>((static_cast<int>(r) + 1) % 7);
        return r;
    }

    Date::Weekday operator++(Date::Weekday& r, int) {
        Date::Weekday temp{ r };
        ++r;
        return temp;
    }

    Date::Weekday& operator--(Date::Weekday& r) {
        r = static_cast<Date::Weekday>((static_cast<int>(r) + 6) % 7);
        return r;
    }

    Date::Weekday operator--(Date::Weekday& r, int) {
        Date::Weekday temp{ r };
        --r;
        return temp;
    }

    // Stream operators
    std::ostream& operator<<(std::ostream& os, const Date& date) {
        os << date.m_day << " "
            << month_names[date.m_month - 1] << " "
            << date.m_year << " "
            << day_names[static_cast<int>(date.get_week_day())];
        return os;
    }

    std::istream& operator>>(std::istream& is, Date& date) {
        int d, m, y;
        char delim;
        is >> d >> delim >> m >> delim >> y;
        date.set(d, m, y);
        return is;
    }
}
