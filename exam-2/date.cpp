#ifndef __PROGTEST__
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <vector>
using namespace std;
#endif /* __PROGTEST__ */

class CDate {
   private:
    int m_Year;
    int m_Month;
    int m_Day;

   public:
    CDate(int year, int month, int day) : m_Year(year), m_Month(month), m_Day(day) {}

    int Year() const { return m_Year; }
    int Month() const { return m_Month; }
    int Day() const { return m_Day; }

    void SubYears(int years) {
        if (years < 1) return;
        m_Year -= years;
    }

    void SubMonths(int months) {
        if (months < 1) return;

        if (months < 12)
            if (m_Month > months)
                m_Month -= months;
            else {
                SubYears(1);
                m_Month = 12 - (months - m_Month);
            }
        else {
            int years = months / 12;
            SubYears(years);
            m_Month -= months - (years * 12);
        }
    }

    void SubDays(int days) {
        if (days < 1) return;

        static int monthDays[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        if (days < monthDays[Month()])
            if (m_Day > days)
                m_Day -= days;
            else {
                SubMonths(1);
                m_Day = monthDays[Month()] - (days - m_Day);
            }
        else {
            SubMonths(1);
            if (days - monthDays[Month()] > 0)
                SubDays(days - monthDays[Month()]);
            else if (days - monthDays[Month()] < 0) {
                m_Day += monthDays[Month()] - days;
            }
        }
    }

    friend bool operator<(const CDate &first, const CDate &second) {
        if (first.Year() < second.Year()) {
            return true;
        } else if (first.Year() != second.Year()) {
            return false;
        }

        if (first.Month() < second.Month()) {
            return true;
        } else if (first.Month() != second.Month()) {
            return false;
        }

        if (first.Day() < second.Day()) {
            return true;
        }

        return false;
    }

    friend bool operator>(const CDate &first, const CDate &second) {
        if (first.Year() > second.Year()) {
            return true;
        } else if (first.Year() != second.Year()) {
            return false;
        }

        if (first.Month() > second.Month()) {
            return true;
        } else if (first.Month() != second.Month()) {
            return false;
        }

        if (first.Day() > second.Day()) {
            return true;
        }

        return false;
    }

    friend ostream &operator<<(ostream &os, const CDate &d) {
        os << setw(4) << setfill('0') << d.Year() << "-";
        os << setw(2) << setfill('0') << d.Month() << "-";
        return os << setw(2) << setfill('0') << d.Day();
    }
};

void compareTest1() {
    {
        cout << "8) -----------------" << endl;
        CDate d = CDate(2000, 1, 1);
        CDate d2 = CDate(2001, 1, 1);
        cout << "Is " << d << " < " << d2 << " ?" << endl;
        cout << "\t -> " << ((d < d2) ? "True" : "False") << endl;
        assert((d < d2) == true);
    }
    {
        cout << "8) -----------------" << endl;
        CDate d = CDate(2001, 1, 1);
        CDate d2 = CDate(2000, 1, 1);
        cout << "Is " << d << " < " << d2 << " ?" << endl;
        cout << "\t -> " << ((d < d2) ? "True" : "False") << endl;
        assert((d < d2) == false);
    }
    {
        cout << "9) -----------------" << endl;
        CDate d = CDate(2000, 1, 1);
        CDate d2 = CDate(2000, 1, 1);
        cout << "Is " << d << " < " << d2 << " ?" << endl;
        cout << "\t -> " << ((d < d2) ? "True" : "False") << endl;
        assert((d < d2) == false);
    }
    {
        cout << "10) ----------------" << endl;
        CDate d = CDate(2000, 10, 1);
        CDate d2 = CDate(2000, 1, 1);
        cout << "Is " << d << " < " << d2 << " ?" << endl;
        cout << "\t -> " << ((d < d2) ? "True" : "False") << endl;
        assert((d < d2) == false);
    }
    {
        cout << "11) ----------------" << endl;
        CDate d = CDate(2000, 10, 1);
        CDate d2 = CDate(2000, 10, 11);
        cout << "Is " << d << " < " << d2 << " ?" << endl;
        cout << "\t -> " << ((d < d2) ? "True" : "False") << endl;
        assert((d < d2) == true);
    }
    {
        cout << "12) ----------------" << endl;
        CDate d = CDate(2000, 10, 11);
        CDate d2 = CDate(2000, 10, 12);
        cout << "Is " << d << " < " << d2 << " ?" << endl;
        cout << "\t -> " << ((d < d2) ? "True" : "False") << endl;
        assert((d < d2) == true);
    }
    {
        cout << "13) ----------------" << endl;
        CDate d = CDate(2001, 10, 11);
        CDate d2 = CDate(2000, 10, 12);
        cout << "Is " << d << " < " << d2 << " ?" << endl;
        cout << "\t -> " << ((d < d2) ? "True" : "False") << endl;
        assert((d < d2) == false);
    }
}

void compareTest2() {
    {
        cout << "8) -----------------" << endl;
        CDate d = CDate(2000, 1, 1);
        CDate d2 = CDate(2001, 1, 1);
        cout << "Is " << d << " > " << d2 << " ?" << endl;
        cout << "\t -> " << ((d > d2) ? "True" : "False") << endl;
        assert((d > d2) == false);
    }
    {
        cout << "8) -----------------" << endl;
        CDate d = CDate(2001, 1, 1);
        CDate d2 = CDate(2000, 1, 1);
        cout << "Is " << d << " > " << d2 << " ?" << endl;
        cout << "\t -> " << ((d > d2) ? "True" : "False") << endl;
        assert((d > d2) == true);
    }
    {
        cout << "9) -----------------" << endl;
        CDate d = CDate(2000, 1, 1);
        CDate d2 = CDate(2000, 1, 1);
        cout << "Is " << d << " > " << d2 << " ?" << endl;
        cout << "\t -> " << ((d > d2) ? "True" : "False") << endl;
        assert((d > d2) == false);
    }
    {
        cout << "10) ----------------" << endl;
        CDate d = CDate(2000, 10, 1);
        CDate d2 = CDate(2000, 1, 1);
        cout << "Is " << d << " > " << d2 << " ?" << endl;
        cout << "\t -> " << ((d > d2) ? "True" : "False") << endl;
        assert((d > d2) == true);
    }
    {
        cout << "11) ----------------" << endl;
        CDate d = CDate(2000, 10, 1);
        CDate d2 = CDate(2000, 10, 11);
        cout << "Is " << d << " > " << d2 << " ?" << endl;
        cout << "\t -> " << ((d > d2) ? "True" : "False") << endl;
        assert((d > d2) == false);
    }
    {
        cout << "12) ----------------" << endl;
        CDate d = CDate(2000, 10, 11);
        CDate d2 = CDate(2000, 10, 12);
        cout << "Is " << d << " > " << d2 << " ?" << endl;
        cout << "\t -> " << ((d > d2) ? "True" : "False") << endl;
        assert((d > d2) == false);
    }
    {
        cout << "13) ----------------" << endl;
        CDate d = CDate(2001, 10, 11);
        CDate d2 = CDate(2000, 10, 12);
        cout << "Is " << d << " > " << d2 << " ?" << endl;
        cout << "\t -> " << ((d > d2) ? "True" : "False") << endl;
        assert((d > d2) == true);
    }
}

void subTest() {
    {
        cout << "1) -----------------" << endl;
        CDate d = CDate(2000, 12, 10);
        cout << "Substracting 10 days from: " << d << endl;
        d.SubDays(10);
        cout << "After substracting: " << d << endl;
    }

    {
        cout << "2) -----------------" << endl;
        CDate d = CDate(2000, 12, 10);
        cout << "Substracting 20 days from: " << d << endl;
        d.SubDays(20);
        cout << "After substracting: " << d << endl;
    }

    {
        cout << "2) -----------------" << endl;
        CDate d = CDate(2000, 12, 10);
        cout << "Substracting 20 days from: " << d << endl;
        d.SubDays(20);
        cout << "Substracting 20 days from: " << d << endl;
        d.SubDays(20);
        cout << "Substracting 20 days from: " << d << endl;
        d.SubDays(20);
        cout << "After substracting: " << d << endl;
    }

    {
        cout << "3) -----------------" << endl;
        CDate d = CDate(2000, 12, 10);
        cout << "Substracting 40 days from: " << d << endl;
        d.SubDays(40);
        cout << "Substracting 20 days from: " << d << endl;
        d.SubDays(20);
        cout << "After substracting: " << d << endl;
    }
    {
        cout << "4) -----------------" << endl;
        CDate d = CDate(2000, 12, 10);
        cout << "Substracting 60 days from: " << d << endl;
        d.SubDays(60);
        cout << "After substracting: " << d << endl;
    }
    {
        cout << "5) -----------------" << endl;
        CDate d = CDate(2000, 1, 10);
        cout << "Substracting 10 days from: " << d << endl;
        d.SubDays(10);
        cout << "After substracting: " << d << endl;
    }
    {
        cout << "6) -----------------" << endl;
        CDate d = CDate(2000, 1, 10);
        cout << "Substracting 100 days from: " << d << endl;
        d.SubDays(100);
        cout << "After substracting: " << d << endl;
    }
    {
        cout << "7) -----------------" << endl;
        CDate d = CDate(2000, 1, 1);
        cout << "Substracting 1 day from: " << d << endl;
        d.SubDays(1);
        cout << "After substracting: " << d << endl;
    }
    {
        cout << "14) ----------------" << endl;
        CDate d = CDate(1929, 12, 11);
        cout << "Substracting 123 days from: " << d << endl;
        d.SubDays(123);
        cout << "After substracting: " << d << endl;
    }
    {
        cout << "15) ----------------" << endl;
        CDate d = CDate(1929, 12, 11);
        cout << "Substracting 0 days from: " << d << endl;
        d.SubDays(0);
        cout << "After substracting: " << d << endl;
    }
}
int main() {
    cout << "********************" << endl;
    subTest();
    compareTest1();
    compareTest2();

    // {
    //     cout << "2) -----------------" << endl;
    //     CDate d = CDate(2000, 12, 10);
    //     cout << "Substracting 20 days from: " << d << endl;
    //     d.SubDays(20);
    //     cout << "Substracting 20 days from: " << d << endl;
    //     d.SubDays(20);
    //     cout << "Substracting 20 days from: " << d << endl;
    //     d.SubDays(20);
    //     cout << "After substracting: " << d << endl;
    // }

    {
        cout << "4) -----------------" << endl;
        CDate d = CDate(2000, 12, 10);
        cout << "Substracting 60 days from: " << d << endl;
        d.SubDays(60);
        cout << "After substracting: " << d << endl;
    }

    // ********************
    // 2) -----------------
    // Substracting 20 days from: 2000-12-10
    // Substracting 20 days from: 2000-11-20
    // Substracting 20 days from: 2000-10-31
    // After substracting: 2000-10-11
    // 4) -----------------
    // Substracting 60 days from: 2000-12-10
    // After substracting: 2000-10-10
    // ********************

    cout << "********************" << endl;

    return 0;
}
