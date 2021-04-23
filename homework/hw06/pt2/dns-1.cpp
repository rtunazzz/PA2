#ifndef __PROGTEST__
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "ipaddress.h"
using namespace std;
#endif /* __PROGTEST__ */

class CRecord {
   private:
    string m_name;
    string m_type;

   public:
    CRecord() = delete;
    CRecord(const string &name, const string &type) : m_name(name), m_type(type) {}
    virtual ~CRecord() = default;

    virtual CRecord *Clone() const {
        return new CRecord(*this);
    }

    const string &Type() const {
        return m_type;
    }

    const string &Name() const {
        return m_name;
    }

    virtual bool isEqual(const CRecord &other) const {
        return (other.Type() == Type() && other.Name() == Name());
    }

    virtual ostream &Print(ostream &os) const {
        os << Name() << " " << Type();
        return os;
    }

    /**
     * @brief Wrapper for displaying the class in a string format
     * @param os Stream
     * @param r Record
     * @return ostream& Stream passed in
     */
    friend ostream &operator<<(ostream &os, const CRecord &r) {
        return r.Print(os);
    }
};

class CSearchResult {
   private:
    vector<shared_ptr<CRecord>> m_data;

   public:
    CSearchResult() = default;
    int Count() const { return m_data.size(); }

    const CRecord &operator[](int index) const {
        // not needed since `at` will throw an out_of_range exception on it's own
        // if (index >= Count()) {
        //     throw std::out_of_range("Index is out of range - index >= Count()");
        // }
        return *m_data.at(index);
    }

    void Add(shared_ptr<CRecord> rec) {
        // m_data.push_back(rec->Clone());
        m_data.push_back(rec);
    }

    friend ostream &operator<<(ostream &os, const CSearchResult &s) {
        for (auto const &it : s.m_data) {
            it->Print(os);
            os << endl;
        }
        return os;
    }
};

class CRecA : public CRecord {
   private:
    CIPv4 m_ipv4;

   public:
    CRecA() = delete;
    CRecA(const string &name, const CIPv4 &ipv4) : CRecord(name, "A"), m_ipv4(ipv4) {}

    CRecord *Clone() const override {
        return new CRecA(*this);
    }

    const CIPv4 &IPv4() const { return m_ipv4; }

    bool isEqual(const CRecord &other) const override {
        const CRecA *rec = dynamic_cast<const CRecA *>(&other);
        if (rec == nullptr) {
            return CRecord::isEqual(other);
        }
        return (CRecord::isEqual(other) && IPv4() == rec->IPv4());
    }

    ostream &Print(ostream &os) const override {
        return os << Name() << " " << Type() << " " << IPv4();
    }
};

class CRecAAAA : public CRecord {
   private:
    CIPv6 m_ipv6;

   public:
    CRecAAAA() = delete;
    CRecAAAA(const string &name, const CIPv6 &ipv6) : CRecord(name, "AAAA"), m_ipv6(ipv6) {}
    CRecord *Clone() const override {
        return new CRecAAAA(*this);
    }

    const CIPv6 &IPv6() const { return m_ipv6; }

    bool isEqual(const CRecord &other) const override {
        const CRecAAAA *rec = dynamic_cast<const CRecAAAA *>(&other);
        if (rec == nullptr) {
            return CRecord::isEqual(other);
        }
        return (CRecord::isEqual(other) && IPv6() == rec->IPv6());
    }

    ostream &Print(ostream &os) const override {
        return os << Name() << " " << Type() << " " << IPv6();
    }
};

class CRecMX : public CRecord {
   private:
    string m_serverName;
    int m_priority;

   public:
    CRecMX() = delete;
    CRecMX(const string &name, const string &serverName, int priority) : CRecord(name, "MX"), m_serverName(serverName), m_priority(priority) {}
    CRecord *Clone() const override {
        return new CRecMX(*this);
    }

    int Priority() const { return m_priority; }
    const string &ServerName() const { return m_serverName; }

    bool isEqual(const CRecord &other) const override {
        const CRecMX *rec = dynamic_cast<const CRecMX *>(&other);
        if (rec == nullptr) {
            return CRecord::isEqual(other);
        }
        return (CRecord::isEqual(other) && ServerName() == rec->ServerName() && Priority() == rec->Priority());
    }

    ostream &Print(ostream &os) const override {
        return os << Name() << " " << Type() << " " << Priority() << " " << ServerName();
    }
};

class CZone {
   private:
    string m_name;
    vector<shared_ptr<CRecord>> m_data;

   public:
    CZone(const string &zoneName) : m_name(zoneName) {}

    const string &Name() const { return m_name; }

    bool Add(const CRecord &rec) {
        // check if there's a rec already in the m_data;
        auto it = find_if(m_data.begin(), m_data.end(), [&rec](const shared_ptr<CRecord> &other) { return other->isEqual(rec); });
        if (it != m_data.end()) {
            return false;
        }
        m_data.push_back(shared_ptr<CRecord>(rec.Clone()));
        return true;
    }

    bool Del(const CRecord &rec) {
        auto it = find_if(m_data.begin(), m_data.end(), [&rec](const shared_ptr<CRecord> &other) { return other->isEqual(rec); });
        if (it == m_data.end()) {
            return false;
        }
        m_data.erase(it);
        return true;
    }

    CSearchResult Search(const string &recordName) const {
        CSearchResult result;
        for (const auto &it : m_data) {
            if (it->Name() == recordName) {
                result.Add(it);
            }
        }
        return result;
    }

    friend ostream &operator<<(ostream &os, const CZone &z) {
        os << z.Name() << endl;
        for (const auto &it : z.m_data) {
            if (it == z.m_data.end()[-1]) {
                os << " \\- ";
            } else {
                os << " +- ";
            }
            it->Print(os);
            os << endl;
        }
        return os;
    }
};

#ifndef __PROGTEST__
int main(void) {
    ostringstream oss;

    CZone z0("fit.cvut.cz");
    assert(z0.Add(CRecA("progtest", CIPv4("147.32.232.142"))) == true);
    assert(z0.Add(CRecAAAA("progtest", CIPv6("2001:718:2:2902:0:1:2:3"))) == true);
    assert(z0.Add(CRecA("courses", CIPv4("147.32.232.158"))) == true);
    assert(z0.Add(CRecA("courses", CIPv4("147.32.232.160"))) == true);
    assert(z0.Add(CRecA("courses", CIPv4("147.32.232.159"))) == true);
    assert(z0.Add(CRecAAAA("progtest", CIPv6("2001:718:2:2902:1:2:3:4"))) == true);
    assert(z0.Add(CRecMX("courses", "relay.fit.cvut.cz.", 0)) == true);
    assert(z0.Add(CRecMX("courses", "relay2.fit.cvut.cz.", 10)) == true);
    oss.str("");
    oss << z0;
    assert(oss.str() ==
           "fit.cvut.cz\n"
           " +- progtest A 147.32.232.142\n"
           " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           " +- courses A 147.32.232.158\n"
           " +- courses A 147.32.232.160\n"
           " +- courses A 147.32.232.159\n"
           " +- progtest AAAA 2001:718:2:2902:1:2:3:4\n"
           " +- courses MX 0 relay.fit.cvut.cz.\n"
           " \\- courses MX 10 relay2.fit.cvut.cz.\n");
    assert(z0.Search("progtest").Count() == 3);
    oss.str("");
    oss << z0.Search("progtest");
    assert(oss.str() ==
           "progtest A 147.32.232.142\n"
           "progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           "progtest AAAA 2001:718:2:2902:1:2:3:4\n");
    assert(z0.Del(CRecA("courses", CIPv4("147.32.232.160"))) == true);
    assert(z0.Add(CRecA("courses", CIPv4("147.32.232.122"))) == true);
    oss.str("");
    oss << z0;
    assert(oss.str() ==
           "fit.cvut.cz\n"
           " +- progtest A 147.32.232.142\n"
           " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           " +- courses A 147.32.232.158\n"
           " +- courses A 147.32.232.159\n"
           " +- progtest AAAA 2001:718:2:2902:1:2:3:4\n"
           " +- courses MX 0 relay.fit.cvut.cz.\n"
           " +- courses MX 10 relay2.fit.cvut.cz.\n"
           " \\- courses A 147.32.232.122\n");
    assert(z0.Search("courses").Count() == 5);
    oss.str("");
    oss << z0.Search("courses");
    assert(oss.str() ==
           "courses A 147.32.232.158\n"
           "courses A 147.32.232.159\n"
           "courses MX 0 relay.fit.cvut.cz.\n"
           "courses MX 10 relay2.fit.cvut.cz.\n"
           "courses A 147.32.232.122\n");
    oss.str("");
    oss << z0.Search("courses")[0];
    assert(oss.str() == "courses A 147.32.232.158");
    assert(z0.Search("courses")[0].Name() == "courses");
    assert(z0.Search("courses")[0].Type() == "A");
    oss.str("");
    oss << z0.Search("courses")[1];
    assert(oss.str() == "courses A 147.32.232.159");
    assert(z0.Search("courses")[1].Name() == "courses");
    assert(z0.Search("courses")[1].Type() == "A");
    oss.str("");
    oss << z0.Search("courses")[2];
    assert(oss.str() == "courses MX 0 relay.fit.cvut.cz.");
    assert(z0.Search("courses")[2].Name() == "courses");
    assert(z0.Search("courses")[2].Type() == "MX");
    try {
        oss.str("");
        oss << z0.Search("courses")[10];
        assert("No exception thrown!" == nullptr);
    } catch (const out_of_range &e) {
    } catch (...) {
        assert("Invalid exception thrown!" == nullptr);
    }
    dynamic_cast<const CRecAAAA &>(z0.Search("progtest")[1]);
    CZone z1("fit2.cvut.cz");
    z1.Add(z0.Search("progtest")[2]);
    z1.Add(z0.Search("progtest")[0]);
    z1.Add(z0.Search("progtest")[1]);
    z1.Add(z0.Search("courses")[2]);
    oss.str("");
    oss << z1;
    assert(oss.str() ==
           "fit2.cvut.cz\n"
           " +- progtest AAAA 2001:718:2:2902:1:2:3:4\n"
           " +- progtest A 147.32.232.142\n"
           " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           " \\- courses MX 0 relay.fit.cvut.cz.\n");
    dynamic_cast<const CRecA &>(z1.Search("progtest")[1]);

    CZone z2("fit.cvut.cz");
    assert(z2.Add(CRecA("progtest", CIPv4("147.32.232.142"))) == true);
    assert(z2.Add(CRecAAAA("progtest", CIPv6("2001:718:2:2902:0:1:2:3"))) == true);
    assert(z2.Add(CRecA("progtest", CIPv4("147.32.232.144"))) == true);
    assert(z2.Add(CRecMX("progtest", "relay.fit.cvut.cz.", 10)) == true);
    assert(z2.Add(CRecA("progtest", CIPv4("147.32.232.142"))) == false);
    assert(z2.Del(CRecA("progtest", CIPv4("147.32.232.140"))) == false);
    assert(z2.Del(CRecA("progtest", CIPv4("147.32.232.142"))) == true);
    assert(z2.Del(CRecA("progtest", CIPv4("147.32.232.142"))) == false);
    assert(z2.Add(CRecMX("progtest", "relay.fit.cvut.cz.", 20)) == true);
    assert(z2.Add(CRecMX("progtest", "relay.fit.cvut.cz.", 10)) == false);
    oss.str("");
    oss << z2;
    assert(oss.str() ==
           "fit.cvut.cz\n"
           " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           " +- progtest A 147.32.232.144\n"
           " +- progtest MX 10 relay.fit.cvut.cz.\n"
           " \\- progtest MX 20 relay.fit.cvut.cz.\n");
    assert(z2.Search("progtest").Count() == 4);
    oss.str("");
    oss << z2.Search("progtest");
    assert(oss.str() ==
           "progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           "progtest A 147.32.232.144\n"
           "progtest MX 10 relay.fit.cvut.cz.\n"
           "progtest MX 20 relay.fit.cvut.cz.\n");
    assert(z2.Search("courses").Count() == 0);
    oss.str("");
    oss << z2.Search("courses");
    assert(oss.str() == "");
    try {
        dynamic_cast<const CRecMX &>(z2.Search("progtest")[0]);
        assert("Invalid type" == nullptr);
    } catch (const bad_cast &e) {
    }

    CZone z4("fit.cvut.cz");
    assert(z4.Add(CRecA("progtest", CIPv4("147.32.232.142"))) == true);
    assert(z4.Add(CRecA("courses", CIPv4("147.32.232.158"))) == true);
    assert(z4.Add(CRecA("courses", CIPv4("147.32.232.160"))) == true);
    assert(z4.Add(CRecA("courses", CIPv4("147.32.232.159"))) == true);
    CZone z5(z4);
    assert(z4.Add(CRecAAAA("progtest", CIPv6("2001:718:2:2902:0:1:2:3"))) == true);
    assert(z4.Add(CRecAAAA("progtest", CIPv6("2001:718:2:2902:1:2:3:4"))) == true);
    assert(z5.Del(CRecA("progtest", CIPv4("147.32.232.142"))) == true);
    oss.str("");
    oss << z4;
    assert(oss.str() ==
           "fit.cvut.cz\n"
           " +- progtest A 147.32.232.142\n"
           " +- courses A 147.32.232.158\n"
           " +- courses A 147.32.232.160\n"
           " +- courses A 147.32.232.159\n"
           " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           " \\- progtest AAAA 2001:718:2:2902:1:2:3:4\n");
    oss.str("");
    oss << z5;
    assert(oss.str() ==
           "fit.cvut.cz\n"
           " +- courses A 147.32.232.158\n"
           " +- courses A 147.32.232.160\n"
           " \\- courses A 147.32.232.159\n");
    z5 = z4;
    assert(z4.Add(CRecMX("courses", "relay.fit.cvut.cz.", 0)) == true);
    assert(z4.Add(CRecMX("courses", "relay2.fit.cvut.cz.", 10)) == true);
    oss.str("");
    oss << z4;
    assert(oss.str() ==
           "fit.cvut.cz\n"
           " +- progtest A 147.32.232.142\n"
           " +- courses A 147.32.232.158\n"
           " +- courses A 147.32.232.160\n"
           " +- courses A 147.32.232.159\n"
           " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           " +- progtest AAAA 2001:718:2:2902:1:2:3:4\n"
           " +- courses MX 0 relay.fit.cvut.cz.\n"
           " \\- courses MX 10 relay2.fit.cvut.cz.\n");
    oss.str("");
    oss << z5;
    assert(oss.str() ==
           "fit.cvut.cz\n"
           " +- progtest A 147.32.232.142\n"
           " +- courses A 147.32.232.158\n"
           " +- courses A 147.32.232.160\n"
           " +- courses A 147.32.232.159\n"
           " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           " \\- progtest AAAA 2001:718:2:2902:1:2:3:4\n");

    return 0;
}
#endif /* __PROGTEST__ */
