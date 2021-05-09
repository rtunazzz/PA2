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

enum class ERel {
    REL_NONE,
    REL_FATHER,
    REL_MOTHER,
    REL_SON,
    REL_DAUGHTER
};
#endif /* __PROGTEST__ */

class CPerson {
   private:
    int m_Id;

   protected:
    string m_Name;
    vector<shared_ptr<CPerson>> m_Descendants;

   public:
    CPerson(const CPerson &old) : m_Id(old.m_Id), m_Name(old.m_Name), m_Descendants(old.m_Descendants) {}
    CPerson(int id, const string &name) : m_Id(id), m_Name(name) {}
    virtual ~CPerson() = default;

    virtual bool prefixMatched(const string &prefix) const {
        if (prefix.length() > m_Name.length()) {
            return false;
        }
        // return m_Name.rfind(prefix, 0) == 0;
        return m_Name.find(prefix) == 0;
    }

    int GetID() const { return m_Id; }

    virtual ostream &Print(ostream &os) const {
        os << m_Id << ": " << m_Name;
        return os;
    }

    void AddDescendant(shared_ptr<CPerson> descendant) {
        m_Descendants.push_back(descendant);
    }

    ostream &_printDescendants(ostream &os, const string &padding, bool isLast) const {
        Print(os) << endl;
        // cout << "Printing '" << m_Descendants.size() << "' descendants" << endl;
        const string &newPadding = padding + (!isLast ? "|  " : " ");
        for (const auto &it : m_Descendants) {
            os << newPadding;
            if (it == m_Descendants.end()[-1]) {
                os << "\\- ";
                it->_printDescendants(os, newPadding + "  ", true);
            } else {
                os << "+- ";
                it->_printDescendants(os, newPadding, false);
            }
        }
        return os;
    }

    ostream &PrintDescendants(ostream &os) const {
        return _printDescendants(os, "", true);
    }

    friend ostream &operator<<(ostream &os, const CPerson &p) {
        return p.Print(os);
    }
};

class CMan : public CPerson {
   public:
    CMan(int id, const string &name) : CPerson(id, name) {}

    ostream &Print(ostream &os) const override {
        os << GetID() << ": " << m_Name << " (man)";
        // CPerson::Print(os) << " (man)";
        return os;
    }
};

class CWoman : public CPerson {
   private:
    vector<string> m_PreviousNames;

   public:
    CWoman(int id, const string &name) : CPerson(id, name) {}

    bool prefixMatched(const string &prefix) const override {
        if ((prefix.length() <= m_Name.length()) && m_Name.find(prefix) == 0) {
            return true;
        }
        for (const auto &it : m_PreviousNames) {
            // if ((prefix.length() <= it.length()) && it.rfind(prefix, 0) == 0) {
            //     return true;
            // }
            if ((prefix.length() <= it.length()) && it.find(prefix) == 0) {
                return true;
            }
        }
        return false;
    }

    void Wedding(const string &newName) {
        m_PreviousNames.push_back(m_Name);
        m_Name = newName;
    }

    ostream &Print(ostream &os) const override {
        // CPerson::Print(os);
        os << GetID() << ": " << m_Name;
        if (m_PreviousNames.size() == 0) {
            os << " (woman)";
        } else {
            os << " [";
            for (const auto &it : m_PreviousNames) {
                os << it;
                // if it's not the last name
                if (it != (m_PreviousNames.end()[-1])) {
                    os << ", ";
                }
            }
            os << "] (woman)";
        }
        return os;
    }
};

class CRegister {
   private:
    map<int, shared_ptr<CPerson>> m_Data;

   public:
    // default constructor
    bool Add(shared_ptr<CPerson> person, shared_ptr<CPerson> father, shared_ptr<CPerson> mother) {
        // check if person ID already exists in database
        if (m_Data.find(person->GetID()) != m_Data.end()) {
            return false;
        }
        m_Data.insert({person->GetID(), person});

        if (father && m_Data.find(father->GetID()) != m_Data.end()) {
            m_Data.find(father->GetID())->second->AddDescendant(person);
        }

        if (mother && m_Data.find(mother->GetID()) != m_Data.end()) {
            m_Data.find(mother->GetID())->second->AddDescendant(person);
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

    vector<shared_ptr<CPerson>> FindByName(const string &prefix) const {
        vector<shared_ptr<CPerson>> result;

        for (const auto &it : m_Data) {
            if (it.second->prefixMatched(prefix)) {
                result.push_back(it.second);
            }
        }

        return result;
    }

    list<pair<unique_ptr<CPerson>, ERel>> FindRelatives(int id1, int id2) const {
        list<pair<unique_ptr<CPerson>, ERel>> result;
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
template <typename T_>
static bool listMatch(const list<pair<T_, ERel>> &res,
                      const list<pair<string, ERel>> &ref) {
    list<pair<string, ERel>> tmp;
    for (const auto &x : res) {
        ostringstream oss;
        oss << *x.first;
        tmp.emplace_back(oss.str(), x.second);
    }
    return tmp == ref;
}
int main(void) {
    ostringstream oss;
    CRegister r;
    assert(r.Add(make_shared<CMan>(1, "Peterson George"),
                 nullptr, nullptr) == true);
    assert(r.Add(make_shared<CMan>(2, "Watson Paul"),
                 nullptr, nullptr) == true);
    assert(r.Add(make_shared<CMan>(10, "Smith Samuel"),
                 nullptr, nullptr) == true);
    assert(r.Add(make_shared<CWoman>(11, "Peterson Jane"),
                 r.FindByID(1), nullptr) == true);
    assert(r.Add(make_shared<CWoman>(12, "Peterson Sue"),
                 r.FindByID(1), nullptr) == true);
    assert(r.Add(make_shared<CMan>(13, "Pershing John"),
                 nullptr, nullptr) == true);
    assert(r.Add(make_shared<CMan>(14, "Pearce Joe"),
                 nullptr, nullptr) == true);
    assert(r.Add(make_shared<CMan>(15, "Peant Thomas"),
                 nullptr, nullptr) == true);
    assert(r.Add(make_shared<CMan>(100, "Smith John"),
                 r.FindByID(10), r.FindByID(11)) == true);
    assert(r.Add(make_shared<CMan>(101, "Smith Roger"),
                 r.FindByID(10), r.FindByID(11)) == true);
    assert(r.Add(make_shared<CMan>(102, "Smith Daniel"),
                 r.FindByID(10), r.FindByID(11)) == true);
    assert(r.Add(make_shared<CWoman>(103, "Smith Eve"),
                 r.FindByID(10), r.FindByID(11)) == true);
    assert(r.Add(make_shared<CWoman>(103, "Smith Jane"),
                 r.FindByID(10), r.FindByID(11)) == false);
    dynamic_cast<CWoman &>(*r.FindByID(12)).Wedding("Smith Sue");
    assert(r.Add(make_shared<CMan>(150, "Pershing Joe"),
                 r.FindByID(13), r.FindByID(12)) == true);
    dynamic_cast<CWoman &>(*r.FindByID(12)).Wedding("Pearce Sue");
    assert(r.Add(make_shared<CMan>(151, "Pearce Phillip"),
                 r.FindByID(14), r.FindByID(12)) == true);
    dynamic_cast<CWoman &>(*r.FindByID(12)).Wedding("Peant Sue");
    assert(r.Add(make_shared<CMan>(152, "Peant Harry"),
                 r.FindByID(15), r.FindByID(12)) == true);
    assert(r.Add(make_shared<CMan>(200, "Pershing Peter"),
                 r.FindByID(150), r.FindByID(103)) == true);
    assert(r.Add(make_shared<CWoman>(201, "Pershing Julia"),
                 r.FindByID(150), r.FindByID(103)) == true);
    assert(r.Add(make_shared<CWoman>(202, "Pershing Anne"),
                 r.FindByID(150), r.FindByID(103)) == true);
    assert(vectorMatch(r.FindByName("Peterson"), vector<string>{
                                                     "1: Peterson George (man)",
                                                     "11: Peterson Jane (woman)",
                                                     "12: Peant Sue [Peterson Sue, Smith Sue, Pearce Sue] (woman)"}));
    assert(vectorMatch(r.FindByName("Pe"), vector<string>{
                                               "1: Peterson George (man)",
                                               "11: Peterson Jane (woman)",
                                               "12: Peant Sue [Peterson Sue, Smith Sue, Pearce Sue] (woman)",
                                               "13: Pershing John (man)",
                                               "14: Pearce Joe (man)",
                                               "15: Peant Thomas (man)",
                                               "150: Pershing Joe (man)",
                                               "151: Pearce Phillip (man)",
                                               "152: Peant Harry (man)",
                                               "200: Pershing Peter (man)",
                                               "201: Pershing Julia (woman)",
                                               "202: Pershing Anne (woman)"}));
    assert(vectorMatch(r.FindByName("Smith"), vector<string>{
                                                  "10: Smith Samuel (man)",
                                                  "12: Peant Sue [Peterson Sue, Smith Sue, Pearce Sue] (woman)",
                                                  "100: Smith John (man)",
                                                  "101: Smith Roger (man)",
                                                  "102: Smith Daniel (man)",
                                                  "103: Smith Eve (woman)"}));
    assert(r.FindByID(1)->GetID() == 1);
    oss.str("");
    oss << *r.FindByID(1);
    assert(oss.str() == "1: Peterson George (man)");
    oss.str("");
    r.FindByID(1)->PrintDescendants(oss);
    assert(oss.str() ==
           "1: Peterson George (man)\n"
           " +- 11: Peterson Jane (woman)\n"
           " |  +- 100: Smith John (man)\n"
           " |  +- 101: Smith Roger (man)\n"
           " |  +- 102: Smith Daniel (man)\n"
           " |  \\- 103: Smith Eve (woman)\n"
           " |     +- 200: Pershing Peter (man)\n"
           " |     +- 201: Pershing Julia (woman)\n"
           " |     \\- 202: Pershing Anne (woman)\n"
           " \\- 12: Peant Sue [Peterson Sue, Smith Sue, Pearce Sue] (woman)\n"
           "    +- 150: Pershing Joe (man)\n"
           "    |  +- 200: Pershing Peter (man)\n"
           "    |  +- 201: Pershing Julia (woman)\n"
           "    |  \\- 202: Pershing Anne (woman)\n"
           "    +- 151: Pearce Phillip (man)\n"
           "    \\- 152: Peant Harry (man)\n");
    assert(r.FindByID(2)->GetID() == 2);
    oss.str("");
    oss << *r.FindByID(2);
    assert(oss.str() == "2: Watson Paul (man)");
    oss.str("");
    r.FindByID(2)->PrintDescendants(oss);
    assert(oss.str() ==
           "2: Watson Paul (man)\n");
    assert(listMatch(r.FindRelatives(100, 1), list<pair<string, ERel>>{
                                                  {"11: Peterson Jane (woman)", ERel::REL_MOTHER},
                                                  {"1: Peterson George (man)", ERel::REL_FATHER}}));
    assert(listMatch(r.FindRelatives(100, 13), list<pair<string, ERel>>{
                                                   {"10: Smith Samuel (man)", ERel::REL_FATHER},
                                                   {"103: Smith Eve (woman)", ERel::REL_DAUGHTER},
                                                   {"200: Pershing Peter (man)", ERel::REL_SON},
                                                   {"150: Pershing Joe (man)", ERel::REL_FATHER},
                                                   {"13: Pershing John (man)", ERel::REL_FATHER}}));
    assert(listMatch(r.FindRelatives(100, 2), list<pair<string, ERel>>{}));
    try {
        r.FindRelatives(100, 3);
        assert("Missing an exception" == nullptr);
    } catch (const invalid_argument &e) {
    } catch (...) {
        assert("An unexpected exception thrown");
    }
    try {
        r.FindRelatives(100, 100);
        assert("Missing an exception" == nullptr);
    } catch (const invalid_argument &e) {
    } catch (...) {
        assert("An unexpected exception thrown");
    }
    return 0;
}
#endif /* __PROGTEST__ */
