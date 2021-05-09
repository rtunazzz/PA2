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
    string m_Type;
    int m_Id;

   protected:
    string m_Name;
    vector<shared_ptr<CPerson>> m_Descendants;

   public:
    CPerson() = delete;
    CPerson(int id, const string &name, const string &type) : m_Type(type), m_Id(id), m_Name(name) {}
    virtual ~CPerson() = default;

    virtual bool prefixMatched(const string &prefix) const {
        if (prefix.length() > m_Name.length()) {
            return false;
        }
        // return m_Name.rfind(prefix, 0) == 0;
        return m_Name.find(prefix) == 0;
    }

    int GetID() const { return m_Id; }
    const string &Type() const { return m_Type; }

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
    CMan() = delete;
    CMan(int id, const string &name) : CPerson(id, name, "Man") {}

    ostream &Print(ostream &os) const override {
        CPerson::Print(os) << " (man)";
        return os;
    }

    friend ostream &operator<<(ostream &os, const CMan &p) {
        return p.Print(os);
    }
};

class CWoman : public CPerson {
   private:
    vector<string> m_PreviousNames;

   public:
    CWoman() = delete;
    CWoman(int id, const string &name) : CPerson(id, name, "Woman") {}

    bool prefixMatched(const string &prefix) const override {
        if (CPerson::prefixMatched(prefix)) {
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
        CPerson::Print(os);
        if (m_PreviousNames.size() < 1) {
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

    friend ostream &operator<<(ostream &os, const CWoman &p) {
        return p.Print(os);
    }
};

class CRegister {
   private:
    class CRecord {
       public:
        shared_ptr<CPerson> person;
        shared_ptr<CPerson> father;
        shared_ptr<CPerson> mother;

        CRecord() = delete;
        CRecord(shared_ptr<CPerson> _person, shared_ptr<CPerson> _father, shared_ptr<CPerson> _mother) : person(_person),
                                                                                                         father(_father),
                                                                                                         mother(_mother) {}
        CRecord(const CRecord &old) : person(old.person),
                                      father(old.father),
                                      mother(old.mother) {}
    };
    map<int, shared_ptr<CRecord>> m_Data;

   public:
    bool Add(shared_ptr<CPerson> person, shared_ptr<CPerson> father, shared_ptr<CPerson> mother) {
        // check if person ID already exists in database
        if (m_Data.find(person->GetID()) != m_Data.end()) {
            return false;
        }
        auto pair = m_Data.insert({person->GetID(), make_shared<CRecord>(CRecord(person, father, mother))});
        if (father != nullptr) {
            pair.first->second->father->AddDescendant(pair.first->second->person);
        }
        if (mother != nullptr) {
            pair.first->second->mother->AddDescendant(pair.first->second->person);
        }

        return true;
    }

    shared_ptr<CPerson> FindByID(int id) const {
        auto found = m_Data.find(id);
        if (found == m_Data.end()) {
            // cout << "Person with ID " << id << " not found!!!" << endl;
            return nullptr;
        }
        return found->second->person;
    }

    vector<shared_ptr<CPerson>> FindByName(const string &prefix) const {
        vector<shared_ptr<CPerson>> result;

        for (const auto &it : m_Data) {
            if (it.second->person->prefixMatched(prefix)) {
                result.push_back(it.second->person);
            }
        }

        return result;
    }

    list<pair<shared_ptr<CPerson>, ERel>> FindRelativesNoExep(int id1, int id2) const {
        list<pair<shared_ptr<CPerson>, ERel>> result;
        if (id1 == id2) {
            // cout << "id1 == id2" << endl;
            return result;
        }

        auto firstRecord = m_Data.find(id1);
        if (firstRecord == m_Data.end()) {
            // cout << "Person specified by the id1 parameter not found." << endl;
            return result;
        }

        auto secondRecord = m_Data.find(id2);
        if (secondRecord == m_Data.end()) {
            // cout << "Person specified by the id2 parameter not found." << endl;
            return result;
        }

        // check if ID corresponds for the first ID
        if (firstRecord->second->father != nullptr && firstRecord->second->father->GetID() == id2) {
            result.push_back({firstRecord->second->father, ERel::REL_FATHER});
        } else if (firstRecord->second->mother != nullptr && firstRecord->second->mother->GetID() == id2) {
            result.push_back({firstRecord->second->mother, ERel::REL_MOTHER});
        }

        // return a result if we have any
        if (result.size() > 0) {
            return result;
        }

        if (secondRecord->second->father != nullptr && secondRecord->second->father->GetID() == id1) {
            result.push_back({secondRecord->second->person, secondRecord->second->person->Type() == "Man" ? ERel::REL_SON : ERel::REL_DAUGHTER});
        } else if (secondRecord->second->mother != nullptr && secondRecord->second->mother->GetID() == id1) {
            result.push_back({secondRecord->second->person, secondRecord->second->person->Type() == "Man" ? ERel::REL_SON : ERel::REL_DAUGHTER});
        }

        // return a result if we have any
        if (result.size() > 0) {
            return result;
        }

        if (firstRecord->second->father != nullptr) {
            result = FindRelativesNoExep(id2, firstRecord->second->father->GetID());
            if (result.size() > 0) {
                result.push_back({secondRecord->second->person, ERel::REL_FATHER});
                return result;
            }
        }
        if (firstRecord->second->mother != nullptr) {
            result = FindRelativesNoExep(id2, firstRecord->second->mother->GetID());
            if (result.size() > 0) {
                result.push_back({secondRecord->second->person, ERel::REL_MOTHER});
                return result;
            }
        }

        if (secondRecord->second->father != nullptr) {
            result = FindRelativesNoExep(id1, secondRecord->second->father->GetID());
            if (result.size() > 0) {
                result.push_back({firstRecord->second->person, secondRecord->second->person->Type() == "Man" ? ERel::REL_SON : ERel::REL_DAUGHTER});
                return result;
            }
        }
        if (secondRecord->second->mother != nullptr) {
            result = FindRelativesNoExep(id1, secondRecord->second->mother->GetID());
            if (result.size() > 0) {
                result.push_back({firstRecord->second->person, secondRecord->second->person->Type() == "Man" ? ERel::REL_SON : ERel::REL_DAUGHTER});
                return result;
            }
        }
        return result;
    }

    list<pair<shared_ptr<CPerson>, ERel>> FindRelatives(int id1, int id2) const {
        if (id1 == id2) {
            throw std::invalid_argument("id1 == id2");
        }

        auto id1Found = m_Data.find(id1);
        if (id1Found == m_Data.end()) {
            throw std::invalid_argument("Person specified by the id1 parameter not found.");
        }

        auto id2Found = m_Data.find(id2);
        if (id2Found == m_Data.end()) {
            throw std::invalid_argument("Person specified by the id2 parameter not found.");
        }

        return FindRelativesNoExep(id1, id2);
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

    r.Add(make_shared<CWoman>(420, "S"), nullptr, nullptr);
    for (const auto &it : r.FindByName("S")) {
        cout << *it << endl;
    }

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

    assert(vectorMatch(r.FindByName("S"), vector<string>{
                                              "10: Smith Samuel (man)",
                                              "12: Peant Sue [Peterson Sue, Smith Sue, Pearce Sue] (woman)",
                                              "100: Smith John (man)",
                                              "101: Smith Roger (man)",
                                              "102: Smith Daniel (man)",
                                              "103: Smith Eve (woman)",
                                              "420: S (woman)"}));

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

    // r.FindByID(1)->PrintDescendants(cout);
    // for (const auto &it : r.FindRelatives(11, 1)) {
    //     cout << *it.first << " " << (int)it.second << endl;
    // }

    assert(listMatch(r.FindRelatives(11, 1), list<pair<string, ERel>>{
                                                 {"1: Peterson George (man)", ERel::REL_FATHER}}));

    assert(listMatch(r.FindRelatives(1, 11), list<pair<string, ERel>>{
                                                 {"11: Peterson Jane (woman)", ERel::REL_DAUGHTER}}));

    r.FindByID(1)->PrintDescendants(cout);
    for (const auto &it : r.FindRelatives(100, 1)) {
        cout << *it.first << " " << (int)it.second << endl;
    }

    cout << "-------------CORRECT:\n11: Peterson Jane (woman) 2\n1: Peterson George (man) 1" << endl;

    //  works here so far
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
