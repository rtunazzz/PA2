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

class CPerson {
   private:
    int m_Id;
    string m_Name;
    CDate m_DateBorn;

   protected:
    vector<weak_ptr<CPerson>> m_Parents;
    vector<weak_ptr<CPerson>> m_Kids;

   public:
    CPerson() = delete;
    virtual ~CPerson(){};

    // constructor
    CPerson(int id, const string &name, CDate dateBorn) : m_Id(id), m_Name(name), m_DateBorn(dateBorn) {}

    // GetID ()
    int GetID() const { return m_Id; }
    const string &Name() const { return m_Name; }
    const CDate &Born() const { return m_DateBorn; }

    virtual shared_ptr<CPerson> Clone() {
        return make_shared<CPerson>(*this);
    }

    virtual CDate RetireDate() {
        throw logic_error("RetireDate is not implemented");
    }

    virtual const string Type() const {
        throw logic_error("Type is not implemented");
    }

    void AddParent(shared_ptr<CPerson> person) {
        m_Parents.push_back(person);
    };

    void AddKid(shared_ptr<CPerson> person) {
        m_Kids.push_back(person);
    };

    virtual bool WasInMilitary() const { return false; }

    // set<string> ScanPedigree ()
    set<string> ScanPedigree() {
        set<string> result;
        for (auto const &it : m_Parents) {
            shared_ptr<CPerson> p = it.lock();
            if (p) {
                result.insert(p->Name());
                set<string> prevPedigrees = p->ScanPedigree();
                result.insert(prevPedigrees.begin(), prevPedigrees.end());
            }
        }
        return result;
    }

    ostream &Print(ostream &os) {
        return os << GetID() << ": " << Name()
                  << ", " << Type()
                  << ", born: " << Born()
                  << ", retires: " << RetireDate();
    }

    // operator <<
    friend ostream &operator<<(ostream &os, CPerson &p) {
        return p.Print(os);
    }
};

class CMan : public CPerson {
   private:
    vector<int> m_MilitaryLog;

   public:
    CMan() = delete;
    CMan(int id, const string &name, CDate dateBorn) : CPerson(id, name, dateBorn) {}
    ~CMan() = default;

    shared_ptr<CPerson> Clone() override {
        return make_shared<CMan>(*this);
    }

    const string Type() const override {
        return "man";
    }

    bool WasInMilitary() const override { return m_MilitaryLog.size() > 0; }

    CDate RetireDate() override {
        // TODO save this result into a variable and only recalculate when needed

        const CDate &bornDate = Born();

        // Sort the vector so it's sorted from the highest to the lowest
        sort(m_MilitaryLog.begin(), m_MilitaryLog.end(), greater<int>());

        // Calculate base retire date (+ 65 years for men)
        CDate retireDate = CDate(bornDate.Year() + 65, bornDate.Month(), bornDate.Day());
        for (size_t i = 0; i < m_MilitaryLog.size(); ++i) {
            int multiplicator = 1;
            // doba strávená na nejdelším vojenském cvičení se odečte 4x,
            if (i == 0)
                multiplicator = 4;
            // doba strávená na druhém nejdelším vojenském cvičení se odečte 3x,
            else if (i == 1)
                multiplicator = 3;
            // doba strávená na třetím nejdelším vojenském cvičení se odečte 2x,
            else if (i == 2)
                multiplicator = 2;
            // ostatní vojenská cvičení se odečítají podle své délky bez zvýhodnění,

            retireDate.SubDays(multiplicator * (m_MilitaryLog[i]));
        }

        // odečte se 10 dní za každého syna, který byl alespoň 1 den na vojenském cvičení,
        for (auto const &it : m_Kids) {
            shared_ptr<CPerson> p = it.lock();
            if (p && p->WasInMilitary())
                retireDate.SubDays(10);
        }

        // maximálně lze odečíst 20 let
        if (CDate(bornDate.Year() + 65 - 20, bornDate.Month(), bornDate.Day()) < retireDate)
            return retireDate;
        else
            return CDate(bornDate.Year() + 65 - 20, bornDate.Month(), bornDate.Day());
    }

    void Military(int days) {
        if (days > 0) {
            m_MilitaryLog.push_back(days);
        }
    }
};

class CWoman : public CPerson {
   public:
    CWoman() = delete;
    CWoman(int id, const string &name, CDate dateBorn) : CPerson(id, name, dateBorn) {}
    ~CWoman() = default;

    shared_ptr<CPerson> Clone() override {
        return make_shared<CWoman>(*this);
    }

    const string Type() const override {
        return "woman";
    }

    CDate RetireDate() override {
        // TODO save this result into a variable and only recalculate when needed

        const CDate &bornDate = Born();

        // Calculate base retire date (+ 60 years for women)
        CDate retireDate = CDate(bornDate.Year() + 60, bornDate.Month(), bornDate.Day());

        // za každé dítě se oba odchodu do důchodu zkracuje o 4 roky
        // odečte se 10 dní za každého syna, který byl alespoň 1 den na vojenském cvičení,
        for (auto const &it : m_Kids) {
            retireDate.SubYears(4);
            shared_ptr<CPerson> p = it.lock();

            if (p && p->WasInMilitary())
                retireDate.SubDays(10);
        }

        // maximálně lze odečíst 20 let
        if (CDate(bornDate.Year() + 60 - 20, bornDate.Month(), bornDate.Day()) < retireDate)
            return retireDate;
        else
            return CDate(bornDate.Year() + 60 - 20, bornDate.Month(), bornDate.Day());
    }
};

class CRegister {
   private:
    map<int, shared_ptr<CPerson>> m_Data;

   public:
    // default constructor
    CRegister() = default;
    ~CRegister() = default;
    // copy constructor (if needed)
    CRegister(const CRegister &old) {
        for (auto const &it : old.m_Data) {
            // m_Data.insert({it.first, it.second->Clone()});
            m_Data.insert({it.first, it.second->Clone()});
        }
    }

    CRegister &operator=(CRegister &old) = delete;

    bool Add(shared_ptr<CPerson> person, shared_ptr<CPerson> father, shared_ptr<CPerson> mother) {
        // check if person ID already exists in our database
        if (m_Data.find(person->GetID()) != m_Data.end()) {
            return false;
        }

        m_Data.insert({person->GetID(), person});
        if (father != nullptr) {
            person->AddParent(father);
            father->AddKid(person);
        }
        if (mother != nullptr) {
            person->AddParent(mother);
            mother->AddKid(person);
        }

        return true;
    }

    shared_ptr<CPerson> FindByID(int id) const {
        auto found = m_Data.find(id);
        if (found == m_Data.end()) {
            // cout << "Person with ID " << id << " not found!!!" << endl;
            return nullptr;
        }
        return found->second;
    }

    vector<shared_ptr<CPerson>> FindRetired(CDate from, CDate to) const {
        vector<shared_ptr<CPerson>> result;
        // cout << "Looking through " << m_Data.size() << " records." << endl;
        for (auto const &it : m_Data) {
            shared_ptr<CPerson> p = it.second;
            // cout << "Checking if\t" << *p << " is between " << from << " and " << to << endl;
            CDate retireDate = p->RetireDate();

            if (from > retireDate)
                continue;

            if (to < retireDate)
                continue;

            result.push_back(p);
        }

        return result;
    }
};

#ifndef __PROGTEST__
template <typename T_>
static bool vectorMatch(const vector<T_> &res,
                        const vector<string> &ref) {
    vector<string> tmp;
    for (const auto &x : res) {
        ostringstream oss;
        oss << *x;
        tmp.push_back(oss.str());
    }
    return tmp == ref;
}
int main(void) {
    ostringstream oss;
    vector<CRegister> r;
    r.emplace_back();
    assert(r[0].Add(make_shared<CMan>(1, "Peterson George", CDate(1900, 1, 1)),
                    nullptr, nullptr) == true);
    assert(r[0].Add(make_shared<CMan>(2, "Watson Paul", CDate(1900, 3, 12)),
                    nullptr, nullptr) == true);
    assert(r[0].Add(make_shared<CMan>(3, "Smith Peter", CDate(1900, 7, 14)),
                    nullptr, nullptr) == true);
    assert(r[0].Add(make_shared<CMan>(10, "Smith Samuel", CDate(1930, 11, 29)),
                    r[0].FindByID(3), nullptr) == true);
    assert(r[0].Add(make_shared<CWoman>(11, "Peterson Jane", CDate(1932, 6, 4)),
                    r[0].FindByID(1), nullptr) == true);
    assert(r[0].Add(make_shared<CWoman>(12, "Peterson Sue", CDate(1933, 10, 8)),
                    r[0].FindByID(1), nullptr) == true);
    assert(r[0].Add(make_shared<CMan>(13, "Pershing John", CDate(1929, 12, 11)),
                    nullptr, nullptr) == true);
    assert(r[0].Add(make_shared<CMan>(14, "Pearce Joe", CDate(1925, 7, 16)),
                    nullptr, nullptr) == true);
    assert(r[0].Add(make_shared<CMan>(15, "Peant Thomas", CDate(1937, 8, 15)),
                    nullptr, nullptr) == true);
    assert(r[0].FindByID(11)->GetID() == 11);
    oss.str("");
    oss << *r[0].FindByID(11);
    assert(oss.str() == "11: Peterson Jane, woman, born: 1932-06-04, retires: 1992-06-04");
    assert(r[0].Add(make_shared<CMan>(100, "Smith John", CDate(1961, 2, 12)),
                    r[0].FindByID(10), r[0].FindByID(11)) == true);
    assert(r[0].FindByID(11)->GetID() == 11);
    oss.str("");
    oss << *r[0].FindByID(11);
    assert(oss.str() == "11: Peterson Jane, woman, born: 1932-06-04, retires: 1988-06-04");
    assert(r[0].Add(make_shared<CMan>(101, "Smith Roger", CDate(1963, 4, 5)),
                    r[0].FindByID(10), r[0].FindByID(11)) == true);
    assert(r[0].FindByID(11)->GetID() == 11);
    oss.str("");
    oss << *r[0].FindByID(11);
    assert(oss.str() == "11: Peterson Jane, woman, born: 1932-06-04, retires: 1984-06-04");
    assert(r[0].Add(make_shared<CMan>(102, "Smith Daniel", CDate(1965, 9, 8)),
                    r[0].FindByID(10), r[0].FindByID(11)) == true);
    assert(r[0].FindByID(11)->GetID() == 11);
    oss.str("");
    oss << *r[0].FindByID(11);
    assert(oss.str() == "11: Peterson Jane, woman, born: 1932-06-04, retires: 1980-06-04");
    assert(r[0].Add(make_shared<CWoman>(103, "Smith Eve", CDate(1967, 1, 13)),
                    r[0].FindByID(10), r[0].FindByID(11)) == true);
    assert(r[0].FindByID(11)->GetID() == 11);
    oss.str("");
    oss << *r[0].FindByID(11);
    assert(oss.str() == "11: Peterson Jane, woman, born: 1932-06-04, retires: 1976-06-04");
    assert(r[0].Add(make_shared<CWoman>(103, "Smith Jane", CDate(1968, 6, 22)),
                    r[0].FindByID(10), r[0].FindByID(11)) == false);
    assert(r[0].FindByID(3)->GetID() == 3);
    oss.str("");
    oss << *r[0].FindByID(3);
    assert(oss.str() == "3: Smith Peter, man, born: 1900-07-14, retires: 1965-07-14");
    assert(r[0].FindByID(10)->GetID() == 10);
    oss.str("");
    oss << *r[0].FindByID(10);
    assert(oss.str() == "10: Smith Samuel, man, born: 1930-11-29, retires: 1995-11-29");
    dynamic_cast<CMan &>(*r[0].FindByID(10)).Military(380);
    assert(r[0].FindByID(3)->GetID() == 3);
    oss.str("");
    oss << *r[0].FindByID(3);
    assert(oss.str() == "3: Smith Peter, man, born: 1900-07-14, retires: 1965-07-04");
    assert(r[0].FindByID(10)->GetID() == 10);
    oss.str("");
    oss << *r[0].FindByID(10);
    assert(oss.str() == "10: Smith Samuel, man, born: 1930-11-29, retires: 1991-09-30");
    dynamic_cast<CMan &>(*r[0].FindByID(10)).Military(120);
    assert(r[0].FindByID(10)->GetID() == 10);
    oss.str("");
    oss << *r[0].FindByID(10);
    assert(oss.str() == "10: Smith Samuel, man, born: 1930-11-29, retires: 1990-10-05");
    dynamic_cast<CMan &>(*r[0].FindByID(10)).Military(57);
    assert(r[0].FindByID(10)->GetID() == 10);
    oss.str("");
    oss << *r[0].FindByID(10);
    assert(oss.str() == "10: Smith Samuel, man, born: 1930-11-29, retires: 1990-06-13");
    dynamic_cast<CMan &>(*r[0].FindByID(10)).Military(237);
    assert(r[0].FindByID(10)->GetID() == 10);
    oss.str("");
    oss << *r[0].FindByID(10);
    assert(oss.str() == "10: Smith Samuel, man, born: 1930-11-29, retires: 1988-12-26");
    dynamic_cast<CMan &>(*r[0].FindByID(10)).Military(380);
    assert(r[0].FindByID(10)->GetID() == 10);
    oss.str("");
    oss << *r[0].FindByID(10);
    assert(oss.str() == "10: Smith Samuel, man, born: 1930-11-29, retires: 1986-11-03");
    dynamic_cast<CMan &>(*r[0].FindByID(10)).Military(1380);
    assert(r[0].FindByID(3)->GetID() == 3);
    oss.str("");
    oss << *r[0].FindByID(3);
    assert(oss.str() == "3: Smith Peter, man, born: 1900-07-14, retires: 1965-07-04");
    assert(r[0].FindByID(10)->GetID() == 10);
    oss.str("");
    oss << *r[0].FindByID(10);
    assert(oss.str() == "10: Smith Samuel, man, born: 1930-11-29, retires: 1975-11-29");

    // vector<shared_ptr<CPerson>> tmp = r[0].FindRetired(CDate(1960, 1, 1), CDate(2000, 1, 1));
    // for (auto const &it : tmp) {
    //     cout << *it << endl;
    // }

    assert(vectorMatch(r[0].FindRetired(CDate(1960, 1, 1), CDate(2000, 1, 1)), vector<string>{
                                                                                   "1: Peterson George, man, born: 1900-01-01, retires: 1965-01-01",
                                                                                   "2: Watson Paul, man, born: 1900-03-12, retires: 1965-03-12",
                                                                                   "3: Smith Peter, man, born: 1900-07-14, retires: 1965-07-04",
                                                                                   "10: Smith Samuel, man, born: 1930-11-29, retires: 1975-11-29",
                                                                                   "11: Peterson Jane, woman, born: 1932-06-04, retires: 1976-06-04",
                                                                                   "12: Peterson Sue, woman, born: 1933-10-08, retires: 1993-10-08",
                                                                                   "13: Pershing John, man, born: 1929-12-11, retires: 1994-12-11",
                                                                                   "14: Pearce Joe, man, born: 1925-07-16, retires: 1990-07-16"}));
    assert(vectorMatch(r[0].FindRetired(CDate(1975, 11, 29), CDate(1976, 6, 4)), vector<string>{
                                                                                     "10: Smith Samuel, man, born: 1930-11-29, retires: 1975-11-29",
                                                                                     "11: Peterson Jane, woman, born: 1932-06-04, retires: 1976-06-04"}));
    assert(r[0].FindByID(103)->GetID() == 103);
    oss.str("");
    oss << *r[0].FindByID(103);
    assert(oss.str() == "103: Smith Eve, woman, born: 1967-01-13, retires: 2027-01-13");
    assert(r[0].FindByID(103)->ScanPedigree() == (set<string>{
                                                     "Peterson George",
                                                     "Peterson Jane",
                                                     "Smith Peter",
                                                     "Smith Samuel"}));
    assert(r[0].FindByID(2)->GetID() == 2);
    oss.str("");
    oss << *r[0].FindByID(2);
    assert(oss.str() == "2: Watson Paul, man, born: 1900-03-12, retires: 1965-03-12");
    assert(r[0].FindByID(2)->ScanPedigree() == (set<string>{}));
    r.emplace_back(r[0]);
    assert(r[0].FindByID(13)->GetID() == 13);
    oss.str("");
    oss << *r[0].FindByID(13);
    assert(oss.str() == "13: Pershing John, man, born: 1929-12-11, retires: 1994-12-11");
    assert(r[1].FindByID(13)->GetID() == 13);
    oss.str("");
    oss << *r[1].FindByID(13);
    assert(oss.str() == "13: Pershing John, man, born: 1929-12-11, retires: 1994-12-11");
    dynamic_cast<CMan &>(*r[1].FindByID(13)).Military(123);
    assert(r[0].FindByID(13)->GetID() == 13);
    oss.str("");
    oss << *r[0].FindByID(13);
    // cout << *r[0].FindByID(13) << endl;
    assert(oss.str() == "13: Pershing John, man, born: 1929-12-11, retires: 1994-12-11");
    assert(r[1].FindByID(13)->GetID() == 13);
    oss.str("");
    oss << *r[1].FindByID(13);
    assert(oss.str() == "13: Pershing John, man, born: 1929-12-11, retires: 1993-08-06");
    assert(r[0].FindByID(11)->GetID() == 11);
    oss.str("");
    oss << *r[0].FindByID(11);
    assert(oss.str() == "11: Peterson Jane, woman, born: 1932-06-04, retires: 1976-06-04");
    assert(r[1].FindByID(11)->GetID() == 11);
    oss.str("");
    oss << *r[1].FindByID(11);
    assert(oss.str() == "11: Peterson Jane, woman, born: 1932-06-04, retires: 1976-06-04");
    assert(r[0].Add(make_shared<CWoman>(104, "Smith Jane", CDate(1968, 6, 22)),
                    r[0].FindByID(10), r[0].FindByID(11)) == true);
    assert(r[0].FindByID(11)->GetID() == 11);
    oss.str("");
    oss << *r[0].FindByID(11);
    assert(oss.str() == "11: Peterson Jane, woman, born: 1932-06-04, retires: 1972-06-04");
    assert(r[1].FindByID(11)->GetID() == 11);
    oss.str("");
    oss << *r[1].FindByID(11);
    assert(oss.str() == "11: Peterson Jane, woman, born: 1932-06-04, retires: 1976-06-04");
    return 0;
}
#endif /* __PROGTEST__ */
