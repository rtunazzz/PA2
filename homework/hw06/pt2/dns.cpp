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
        if (other.Type() == "CNAME" || other.Type() == "CZONE" || Type() == "CNAME" || Type() == "CZONE") {
            return other.Name() == Name();
        }
        return (other.Type() == Type() && other.Name() == Name());
    }

    virtual ostream &Print(ostream &os, const string &padding, bool isLast) const {
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
        return r.Print(os, "", true);
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

    CRecord &operator[](int index) {
        // not needed since `at` will throw an out_of_range exception on it's own
        // if (index >= Count()) {
        //     throw std::out_of_range("Index is out of range - index >= Count()");
        // }
        return *m_data.at(index);
    }

    void Add(shared_ptr<CRecord> rec) {
        // m_data.push_back(shared_ptr<CRecord>(rec->Clone()));
        m_data.push_back(rec);
    }

    friend ostream &operator<<(ostream &os, const CSearchResult &s) {
        for (auto const &it : s.m_data) {
            // it->Print(os, "", true);
            os << *it;
            if (it->Type() != "CZONE") {
                os << endl;
            }
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

    ostream &Print(ostream &os, const string &padding, bool isLast) const override {
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

    ostream &Print(ostream &os, const string &padding, bool isLast) const override {
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

    ostream &Print(ostream &os, const string &padding, bool isLast) const override {
        return os << Name() << " " << Type() << " " << Priority() << " " << ServerName();
    }
};

class CRecCNAME : public CRecord {
   private:
    string m_refName;

   public:
    CRecCNAME() = delete;
    CRecCNAME(const string &name, const string &referenceName) : CRecord(name, "CNAME"), m_refName(referenceName) {}
    CRecord *Clone() const override {
        return new CRecCNAME(*this);
    }

    const string &Reference() const { return m_refName; }

    bool isEqual(const CRecord &other) const override {
        // check just the name since there can't be the same name for a CName and any other record
        return other.Name() == Name();
    }

    ostream &Print(ostream &os, const string &padding, bool isLast) const override {
        return os << Name() << " " << Type() << " " << Reference();
    }
};

class CRecSPF : public CRecord {
   private:
    vector<string> m_addresses;

   public:
    CRecSPF() = delete;
    CRecSPF(const string &name) : CRecord(name, "SPF") {}
    CRecord *Clone() const override {
        return new CRecSPF(*this);
    }

    CRecSPF &Add(const string &address) {
        m_addresses.push_back(address);
        return *this;
    }

    bool isEqual(const CRecord &other) const override {
        return CRecord::isEqual(other);
    }

    ostream &Print(ostream &os, const string &padding, bool isLast) const override {
        os << Name() << " " << Type();
        for (auto const &it : m_addresses) {
            if (it != *m_addresses.begin()) {
                os << ",";
            }
            os << " " << it;
        }
        return os;
    }
};

class CZone : public CRecord {
   private:
    vector<shared_ptr<CRecord>> m_data;
    CSearchResult _regularSearch(const string &recordName) const {
        CSearchResult result;
        for (const auto &it : m_data) {
            if (it->Name() == recordName) {
                result.Add(it);
            }
        }
        return result;
    }

    CSearchResult _hierarchicSearch(const string &recordName, const char &separator) const {
        CSearchResult result;
        vector<shared_ptr<CRecord>> dataToGoThrough = m_data;

        // progtest.fit.cvut.cz
        istringstream recordStream(recordName);
        string recordPart;
        vector<string> toProcess;

        while (getline(recordStream, recordPart, separator)) {
            toProcess.push_back(recordPart);
        }

        auto processIt = toProcess.rbegin();
        while (processIt != toProcess.rend()) {
            vector<shared_ptr<CRecord>> tmp;
            // cout << "[" << (processIt - toProcess.rbegin()) << "] "
            //      << "Looking for: " << *processIt << " (" << recordName << ")" << endl;
            for (const auto &it : dataToGoThrough) {
                if (it->Name() == *processIt) {
                    if (it->Type() == "CZONE") {
                        // cout << "\tFound CZONE record for " << *processIt << endl;
                        // cout << "Checking if " << *processIt << " == " << *toProcess.begin() << " (" << ((*processIt == *toProcess.begin()) ? "True" : "False") << ")" << endl;
                        if (&*processIt == &*toProcess.begin()) {
                            // if we are at the last index
                            // cout << "\t\t(Result) Adding:" << endl
                            //      << *it << "to result" << endl;
                            result.Add(it);
                        } else {
                            for (const auto &it2 : dynamic_cast<CZone &>(*it).Data()) {
                                // cout << "\t\t(TMP) Adding:" << endl
                                //      << *it2 << "to tmp" << endl;
                                tmp.push_back(it2);
                            }
                        }
                    } else {
                        // cout << "\tFound a result (" << *it << ") for " << *processIt << endl;
                        result.Add(it);
                    }
                }
            }
            dataToGoThrough = tmp;
            // cout << "After looking for: " << *processIt << ", the dataToGoThrough size is: " << dataToGoThrough.size() << endl;
            processIt++;
        }
        // cout << "======================= (" << recordName << ") SEARCH RESULT IS: =======================" << endl;
        // cout << result << endl;
        // cout << "=====================================================================" << endl;
        return result;
    }

   public:
    CZone(const string &zoneName) : CRecord(zoneName, "CZONE") {}

    CRecord *Clone() const override {
        const CZone &old = *this;
        CZone *newZone = new CZone(old.Name());
        for (const auto &it : old.Data()) {
            newZone->Add(*it);
        }
        return newZone;
    }

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
        if (recordName.find('.') != std::string::npos) {
            // cout << recordName << ": running _hierarchicSearch" << endl;
            return _hierarchicSearch(recordName, '.');
        } else {
            // cout << recordName << ": running _regularSearch" << endl;
            return _regularSearch(recordName);
        }
    }

    const vector<shared_ptr<CRecord>> &Data() const {
        return m_data;
    }

    bool isEqual(const CRecord &other) const override {
        // check just the name since there can't be the same name for a CZone and any other record
        return other.Name() == Name();
    }

    ostream &Print(ostream &os, const string &padding, bool isLast) const override {
        os << Name();
        os << endl;

        const string &newPadding = padding + (!isLast ? "|  " : " ");
        for (const auto &it : m_data) {
            os << newPadding;
            if (it == m_data.end()[-1]) {
                os << "\\- ";
                it->Print(os, newPadding + "  ", true);
            } else {
                os << "+- ";
                it->Print(os, newPadding, false);
            }
            if (it->Type() != "CZONE") {
                os << endl;
            }
        }
        return os;
    }
};
#ifndef __PROGTEST__
int main(void) {
    ostringstream oss;

    CZone z0("fit");
    assert(z0.Add(CRecA("progtest", CIPv4("147.32.232.142"))) == true);
    assert(z0.Add(CRecAAAA("progtest", CIPv6("2001:718:2:2902:0:1:2:3"))) == true);
    assert(z0.Add(CRecA("courses", CIPv4("147.32.232.158"))) == true);
    assert(z0.Add(CRecA("courses", CIPv4("147.32.232.160"))) == true);
    assert(z0.Add(CRecA("courses", CIPv4("147.32.232.159"))) == true);
    assert(z0.Add(CRecCNAME("pririz", "sto.fit.cvut.cz.")) == true);
    assert(z0.Add(CRecSPF("courses").Add("ip4:147.32.232.128/25").Add("ip4:147.32.232.64/26")) == true);
    assert(z0.Add(CRecAAAA("progtest", CIPv6("2001:718:2:2902:1:2:3:4"))) == true);
    assert(z0.Add(CRecMX("courses", "relay.fit.cvut.cz.", 0)) == true);
    assert(z0.Add(CRecMX("courses", "relay2.fit.cvut.cz.", 10)) == true);
    oss.str("");
    oss << z0;
    // cout << z0;
    assert(oss.str() ==
           "fit\n"
           " +- progtest A 147.32.232.142\n"
           " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           " +- courses A 147.32.232.158\n"
           " +- courses A 147.32.232.160\n"
           " +- courses A 147.32.232.159\n"
           " +- pririz CNAME sto.fit.cvut.cz.\n"
           " +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
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
           "fit\n"
           " +- progtest A 147.32.232.142\n"
           " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           " +- courses A 147.32.232.158\n"
           " +- courses A 147.32.232.159\n"
           " +- pririz CNAME sto.fit.cvut.cz.\n"
           " +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           " +- progtest AAAA 2001:718:2:2902:1:2:3:4\n"
           " +- courses MX 0 relay.fit.cvut.cz.\n"
           " +- courses MX 10 relay2.fit.cvut.cz.\n"
           " \\- courses A 147.32.232.122\n");
    assert(z0.Search("courses").Count() == 6);
    oss.str("");
    oss << z0.Search("courses");
    assert(oss.str() ==
           "courses A 147.32.232.158\n"
           "courses A 147.32.232.159\n"
           "courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
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
    assert(oss.str() == "courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26");
    assert(z0.Search("courses")[2].Name() == "courses");
    assert(z0.Search("courses")[2].Type() == "SPF");
    try {
        oss.str("");
        oss << z0.Search("courses")[10];
        assert("No exception thrown!" == nullptr);
    } catch (const out_of_range &e) {
    } catch (...) {
        assert("Invalid exception thrown!" == nullptr);
    }
    dynamic_cast<const CRecAAAA &>(z0.Search("progtest")[1]);
    CZone z1("fit2");
    z1.Add(z0.Search("progtest")[2]);
    z1.Add(z0.Search("progtest")[0]);
    z1.Add(z0.Search("progtest")[1]);
    z1.Add(z0.Search("courses")[2]);
    oss.str("");
    oss << z1;
    assert(oss.str() ==
           "fit2\n"
           " +- progtest AAAA 2001:718:2:2902:1:2:3:4\n"
           " +- progtest A 147.32.232.142\n"
           " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           " \\- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n");
    dynamic_cast<const CRecA &>(z1.Search("progtest")[1]);

    CZone z10("fit");
    assert(z10.Add(CRecA("progtest", CIPv4("147.32.232.142"))) == true);
    assert(z10.Add(CRecAAAA("progtest", CIPv6("2001:718:2:2902:0:1:2:3"))) == true);
    assert(z10.Add(CRecA("progtest", CIPv4("147.32.232.144"))) == true);
    assert(z10.Add(CRecMX("progtest", "relay.fit.cvut.cz.", 10)) == true);
    assert(z10.Add(CRecA("progtest", CIPv4("147.32.232.142"))) == false);
    assert(z10.Del(CRecA("progtest", CIPv4("147.32.232.140"))) == false);
    assert(z10.Del(CRecA("progtest", CIPv4("147.32.232.142"))) == true);
    assert(z10.Del(CRecA("progtest", CIPv4("147.32.232.142"))) == false);
    assert(z10.Add(CRecMX("progtest", "relay.fit.cvut.cz.", 20)) == true);
    assert(z10.Add(CRecMX("progtest", "relay.fit.cvut.cz.", 10)) == false);
    assert(z10.Add(CRecCNAME("pririz", "sto.fit.cvut.cz.")) == true);
    assert(z10.Add(CRecCNAME("pririz", "stojedna.fit.cvut.cz.")) == false);
    assert(z10.Add(CRecA("pririz", CIPv4("147.32.232.111"))) == false);
    assert(z10.Add(CRecCNAME("progtest", "progtestbak.fit.cvut.cz.")) == false);
    assert(z10.Add(CZone("test")) == true);
    assert(z10.Add(CZone("pririz")) == false);
    assert(z10.Add(CRecA("test", CIPv4("147.32.232.232"))) == false);
    oss.str("");
    oss << z10;
    // cout << z10;
    assert(oss.str() ==
           "fit\n"
           " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           " +- progtest A 147.32.232.144\n"
           " +- progtest MX 10 relay.fit.cvut.cz.\n"
           " +- progtest MX 20 relay.fit.cvut.cz.\n"
           " +- pririz CNAME sto.fit.cvut.cz.\n"
           " \\- test\n");
    assert(z10.Search("progtest").Count() == 4);
    oss.str("");
    oss << z10.Search("progtest");
    assert(oss.str() ==
           "progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           "progtest A 147.32.232.144\n"
           "progtest MX 10 relay.fit.cvut.cz.\n"
           "progtest MX 20 relay.fit.cvut.cz.\n");
    assert(z10.Search("courses").Count() == 0);
    oss.str("");
    oss << z10.Search("courses");
    assert(oss.str() == "");

    CZone z20("<ROOT ZONE>");
    CZone z21("cz");
    CZone z22("cvut");
    CZone z23("fit");
    assert(z23.Add(CRecA("progtest", CIPv4("147.32.232.142"))) == true);
    assert(z23.Add(CRecAAAA("progtest", CIPv6("2001:718:2:2902:0:1:2:3"))) == true);
    assert(z23.Add(CRecA("courses", CIPv4("147.32.232.158"))) == true);
    assert(z23.Add(CRecA("courses", CIPv4("147.32.232.160"))) == true);
    assert(z23.Add(CRecA("courses", CIPv4("147.32.232.159"))) == true);
    assert(z23.Add(CRecCNAME("pririz", "sto.fit.cvut.cz.")) == true);
    assert(z23.Add(CRecSPF("courses").Add("ip4:147.32.232.128/25").Add("ip4:147.32.232.64/26")) == true);
    CZone z24("fel");
    assert(z24.Add(CRecA("www", CIPv4("147.32.80.2"))) == true);
    assert(z24.Add(CRecAAAA("www", CIPv6("1:2:3:4:5:6:7:8"))) == true);
    assert(z22.Add(z23) == true);
    assert(z22.Add(z24) == true);
    assert(z21.Add(z22) == true);
    assert(z20.Add(z21) == true);
    assert(z23.Add(CRecA("www", CIPv4("147.32.90.1"))) == true);
    oss.str("");
    // cout << z20;
    // cout << "==========================================" << endl;
    // cout << "<ROOT ZONE>\n"
    //         " \\- cz\n"
    //         "    \\- cvut\n"
    //         "       +- fit\n"
    //         "       |  +- progtest A 147.32.232.142\n"
    //         "       |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    //         "       |  +- courses A 147.32.232.158\n"
    //         "       |  +- courses A 147.32.232.160\n"
    //         "       |  +- courses A 147.32.232.159\n"
    //         "       |  +- pririz CNAME sto.fit.cvut.cz.\n"
    //         "       |  \\- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    //         "       \\- fel\n"
    //         "          +- www A 147.32.80.2\n"
    //         "          \\- www AAAA 1:2:3:4:5:6:7:8\n";
    oss << z20;
    assert(oss.str() ==
           "<ROOT ZONE>\n"
           " \\- cz\n"
           "    \\- cvut\n"
           "       +- fit\n"
           "       |  +- progtest A 147.32.232.142\n"
           "       |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           "       |  +- courses A 147.32.232.158\n"
           "       |  +- courses A 147.32.232.160\n"
           "       |  +- courses A 147.32.232.159\n"
           "       |  +- pririz CNAME sto.fit.cvut.cz.\n"
           "       |  \\- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           "       \\- fel\n"
           "          +- www A 147.32.80.2\n"
           "          \\- www AAAA 1:2:3:4:5:6:7:8\n");
    oss.str("");
    oss << z21;
    assert(oss.str() ==
           "cz\n"
           " \\- cvut\n"
           "    +- fit\n"
           "    |  +- progtest A 147.32.232.142\n"
           "    |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           "    |  +- courses A 147.32.232.158\n"
           "    |  +- courses A 147.32.232.160\n"
           "    |  +- courses A 147.32.232.159\n"
           "    |  +- pririz CNAME sto.fit.cvut.cz.\n"
           "    |  \\- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           "    \\- fel\n"
           "       +- www A 147.32.80.2\n"
           "       \\- www AAAA 1:2:3:4:5:6:7:8\n");
    oss.str("");
    oss << z22;
    assert(oss.str() ==
           "cvut\n"
           " +- fit\n"
           " |  +- progtest A 147.32.232.142\n"
           " |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           " |  +- courses A 147.32.232.158\n"
           " |  +- courses A 147.32.232.160\n"
           " |  +- courses A 147.32.232.159\n"
           " |  +- pririz CNAME sto.fit.cvut.cz.\n"
           " |  \\- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           " \\- fel\n"
           "    +- www A 147.32.80.2\n"
           "    \\- www AAAA 1:2:3:4:5:6:7:8\n");
    oss.str("");
    oss << z23;
    assert(oss.str() ==
           "fit\n"
           " +- progtest A 147.32.232.142\n"
           " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           " +- courses A 147.32.232.158\n"
           " +- courses A 147.32.232.160\n"
           " +- courses A 147.32.232.159\n"
           " +- pririz CNAME sto.fit.cvut.cz.\n"
           " +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           " \\- www A 147.32.90.1\n");
    oss.str("");
    oss << z24;
    assert(oss.str() ==
           "fel\n"
           " +- www A 147.32.80.2\n"
           " \\- www AAAA 1:2:3:4:5:6:7:8\n");
    // cout << "================================" << endl;
    // cout << z20;
    // cout << z20.Search("progtest.fit.cvut.cz").Count() << endl;
    assert(z20.Search("progtest.fit.cvut.cz").Count() == 2);
    oss.str("");
    oss << z20.Search("progtest.fit.cvut.cz");
    assert(oss.str() ==
           "progtest A 147.32.232.142\n"
           "progtest AAAA 2001:718:2:2902:0:1:2:3\n");
    // cout << "================================" << endl;
    assert(z20.Search("fit.cvut.cz").Count() == 1);
    oss.str("");
    // cout << z20.Search("fit.cvut.cz");
    oss << z20.Search("fit.cvut.cz");
    assert(oss.str() ==
           "fit\n"
           " +- progtest A 147.32.232.142\n"
           " +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           " +- courses A 147.32.232.158\n"
           " +- courses A 147.32.232.160\n"
           " +- courses A 147.32.232.159\n"
           " +- pririz CNAME sto.fit.cvut.cz.\n"
           " \\- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n");
    assert(dynamic_cast<CZone &>(z20.Search("fit.cvut.cz")[0]).Add(z20.Search("fel.cvut.cz")[0]) == true);
    oss.str("");
    oss << z20;
    // cout << z20;
    // cout << "<ROOT ZONE>\n"
    //         " \\- cz\n"
    //         "    \\- cvut\n"
    //         "       +- fit\n"
    //         "       |  +- progtest A 147.32.232.142\n"
    //         "       |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
    //         "       |  +- courses A 147.32.232.158\n"
    //         "       |  +- courses A 147.32.232.160\n"
    //         "       |  +- courses A 147.32.232.159\n"
    //         "       |  +- pririz CNAME sto.fit.cvut.cz.\n"
    //         "       |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
    //         "       |  \\- fel\n"
    //         "       |     +- www A 147.32.80.2\n"
    //         "       |     \\- www AAAA 1:2:3:4:5:6:7:8\n"
    //         "       \\- fel\n"
    //         "          +- www A 147.32.80.2\n"
    //         "          \\- www AAAA 1:2:3:4:5:6:7:8\n";
    assert(oss.str() ==
           "<ROOT ZONE>\n"
           " \\- cz\n"
           "    \\- cvut\n"
           "       +- fit\n"
           "       |  +- progtest A 147.32.232.142\n"
           "       |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           "       |  +- courses A 147.32.232.158\n"
           "       |  +- courses A 147.32.232.160\n"
           "       |  +- courses A 147.32.232.159\n"
           "       |  +- pririz CNAME sto.fit.cvut.cz.\n"
           "       |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           "       |  \\- fel\n"
           "       |     +- www A 147.32.80.2\n"
           "       |     \\- www AAAA 1:2:3:4:5:6:7:8\n"
           "       \\- fel\n"
           "          +- www A 147.32.80.2\n"
           "          \\- www AAAA 1:2:3:4:5:6:7:8\n");
    // cout << z20.Search("cz")[0];
    assert(dynamic_cast<CZone &>(z20.Search("fit.cvut.cz")[0]).Add(z20.Search("cz")[0]) == true);
    oss.str("");
    oss << z20;
    assert(oss.str() ==
           "<ROOT ZONE>\n"
           " \\- cz\n"
           "    \\- cvut\n"
           "       +- fit\n"
           "       |  +- progtest A 147.32.232.142\n"
           "       |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           "       |  +- courses A 147.32.232.158\n"
           "       |  +- courses A 147.32.232.160\n"
           "       |  +- courses A 147.32.232.159\n"
           "       |  +- pririz CNAME sto.fit.cvut.cz.\n"
           "       |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           "       |  +- fel\n"
           "       |  |  +- www A 147.32.80.2\n"
           "       |  |  \\- www AAAA 1:2:3:4:5:6:7:8\n"
           "       |  \\- cz\n"
           "       |     \\- cvut\n"
           "       |        +- fit\n"
           "       |        |  +- progtest A 147.32.232.142\n"
           "       |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           "       |        |  +- courses A 147.32.232.158\n"
           "       |        |  +- courses A 147.32.232.160\n"
           "       |        |  +- courses A 147.32.232.159\n"
           "       |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
           "       |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           "       |        |  \\- fel\n"
           "       |        |     +- www A 147.32.80.2\n"
           "       |        |     \\- www AAAA 1:2:3:4:5:6:7:8\n"
           "       |        \\- fel\n"
           "       |           +- www A 147.32.80.2\n"
           "       |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
           "       \\- fel\n"
           "          +- www A 147.32.80.2\n"
           "          \\- www AAAA 1:2:3:4:5:6:7:8\n");
    // cout << z20;
    // cout << z20.Search("fit.cvut.cz.fit.cvut.cz")[0];
    // cout << "-------------------------------------------------------------------------------------" << endl;
    assert(dynamic_cast<CZone &>(z20.Search("fit.cvut.cz.fit.cvut.cz")[0]).Add(z20.Search("cz")[0]) == true);
    oss.str("");
    oss << z20;
    assert(oss.str() ==
           "<ROOT ZONE>\n"
           " \\- cz\n"
           "    \\- cvut\n"
           "       +- fit\n"
           "       |  +- progtest A 147.32.232.142\n"
           "       |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           "       |  +- courses A 147.32.232.158\n"
           "       |  +- courses A 147.32.232.160\n"
           "       |  +- courses A 147.32.232.159\n"
           "       |  +- pririz CNAME sto.fit.cvut.cz.\n"
           "       |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           "       |  +- fel\n"
           "       |  |  +- www A 147.32.80.2\n"
           "       |  |  \\- www AAAA 1:2:3:4:5:6:7:8\n"
           "       |  \\- cz\n"
           "       |     \\- cvut\n"
           "       |        +- fit\n"
           "       |        |  +- progtest A 147.32.232.142\n"
           "       |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           "       |        |  +- courses A 147.32.232.158\n"
           "       |        |  +- courses A 147.32.232.160\n"
           "       |        |  +- courses A 147.32.232.159\n"
           "       |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
           "       |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           "       |        |  +- fel\n"
           "       |        |  |  +- www A 147.32.80.2\n"
           "       |        |  |  \\- www AAAA 1:2:3:4:5:6:7:8\n"
           "       |        |  \\- cz\n"
           "       |        |     \\- cvut\n"
           "       |        |        +- fit\n"
           "       |        |        |  +- progtest A 147.32.232.142\n"
           "       |        |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           "       |        |        |  +- courses A 147.32.232.158\n"
           "       |        |        |  +- courses A 147.32.232.160\n"
           "       |        |        |  +- courses A 147.32.232.159\n"
           "       |        |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
           "       |        |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           "       |        |        |  +- fel\n"
           "       |        |        |  |  +- www A 147.32.80.2\n"
           "       |        |        |  |  \\- www AAAA 1:2:3:4:5:6:7:8\n"
           "       |        |        |  \\- cz\n"
           "       |        |        |     \\- cvut\n"
           "       |        |        |        +- fit\n"
           "       |        |        |        |  +- progtest A 147.32.232.142\n"
           "       |        |        |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           "       |        |        |        |  +- courses A 147.32.232.158\n"
           "       |        |        |        |  +- courses A 147.32.232.160\n"
           "       |        |        |        |  +- courses A 147.32.232.159\n"
           "       |        |        |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
           "       |        |        |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           "       |        |        |        |  \\- fel\n"
           "       |        |        |        |     +- www A 147.32.80.2\n"
           "       |        |        |        |     \\- www AAAA 1:2:3:4:5:6:7:8\n"
           "       |        |        |        \\- fel\n"
           "       |        |        |           +- www A 147.32.80.2\n"
           "       |        |        |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
           "       |        |        \\- fel\n"
           "       |        |           +- www A 147.32.80.2\n"
           "       |        |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
           "       |        \\- fel\n"
           "       |           +- www A 147.32.80.2\n"
           "       |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
           "       \\- fel\n"
           "          +- www A 147.32.80.2\n"
           "          \\- www AAAA 1:2:3:4:5:6:7:8\n");
    assert(dynamic_cast<CZone &>(z20.Search("fit.cvut.cz.fit.cvut.cz")[0]).Del(CZone("fel")) == true);
    oss.str("");
    oss << z20;
    assert(oss.str() ==
           "<ROOT ZONE>\n"
           " \\- cz\n"
           "    \\- cvut\n"
           "       +- fit\n"
           "       |  +- progtest A 147.32.232.142\n"
           "       |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           "       |  +- courses A 147.32.232.158\n"
           "       |  +- courses A 147.32.232.160\n"
           "       |  +- courses A 147.32.232.159\n"
           "       |  +- pririz CNAME sto.fit.cvut.cz.\n"
           "       |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           "       |  +- fel\n"
           "       |  |  +- www A 147.32.80.2\n"
           "       |  |  \\- www AAAA 1:2:3:4:5:6:7:8\n"
           "       |  \\- cz\n"
           "       |     \\- cvut\n"
           "       |        +- fit\n"
           "       |        |  +- progtest A 147.32.232.142\n"
           "       |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           "       |        |  +- courses A 147.32.232.158\n"
           "       |        |  +- courses A 147.32.232.160\n"
           "       |        |  +- courses A 147.32.232.159\n"
           "       |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
           "       |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           "       |        |  \\- cz\n"
           "       |        |     \\- cvut\n"
           "       |        |        +- fit\n"
           "       |        |        |  +- progtest A 147.32.232.142\n"
           "       |        |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           "       |        |        |  +- courses A 147.32.232.158\n"
           "       |        |        |  +- courses A 147.32.232.160\n"
           "       |        |        |  +- courses A 147.32.232.159\n"
           "       |        |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
           "       |        |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           "       |        |        |  +- fel\n"
           "       |        |        |  |  +- www A 147.32.80.2\n"
           "       |        |        |  |  \\- www AAAA 1:2:3:4:5:6:7:8\n"
           "       |        |        |  \\- cz\n"
           "       |        |        |     \\- cvut\n"
           "       |        |        |        +- fit\n"
           "       |        |        |        |  +- progtest A 147.32.232.142\n"
           "       |        |        |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           "       |        |        |        |  +- courses A 147.32.232.158\n"
           "       |        |        |        |  +- courses A 147.32.232.160\n"
           "       |        |        |        |  +- courses A 147.32.232.159\n"
           "       |        |        |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
           "       |        |        |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           "       |        |        |        |  \\- fel\n"
           "       |        |        |        |     +- www A 147.32.80.2\n"
           "       |        |        |        |     \\- www AAAA 1:2:3:4:5:6:7:8\n"
           "       |        |        |        \\- fel\n"
           "       |        |        |           +- www A 147.32.80.2\n"
           "       |        |        |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
           "       |        |        \\- fel\n"
           "       |        |           +- www A 147.32.80.2\n"
           "       |        |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
           "       |        \\- fel\n"
           "       |           +- www A 147.32.80.2\n"
           "       |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
           "       \\- fel\n"
           "          +- www A 147.32.80.2\n"
           "          \\- www AAAA 1:2:3:4:5:6:7:8\n");
    CZone z25(z20);
    z22 = z20;
    assert(z20.Add(CZone("sk")) == true);
    assert(z25.Add(CZone("au")) == true);
    assert(z22.Add(CZone("de")) == true);
    oss.str("");
    oss << z20;
    assert(oss.str() ==
           "<ROOT ZONE>\n"
           " +- cz\n"
           " |  \\- cvut\n"
           " |     +- fit\n"
           " |     |  +- progtest A 147.32.232.142\n"
           " |     |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           " |     |  +- courses A 147.32.232.158\n"
           " |     |  +- courses A 147.32.232.160\n"
           " |     |  +- courses A 147.32.232.159\n"
           " |     |  +- pririz CNAME sto.fit.cvut.cz.\n"
           " |     |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           " |     |  +- fel\n"
           " |     |  |  +- www A 147.32.80.2\n"
           " |     |  |  \\- www AAAA 1:2:3:4:5:6:7:8\n"
           " |     |  \\- cz\n"
           " |     |     \\- cvut\n"
           " |     |        +- fit\n"
           " |     |        |  +- progtest A 147.32.232.142\n"
           " |     |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           " |     |        |  +- courses A 147.32.232.158\n"
           " |     |        |  +- courses A 147.32.232.160\n"
           " |     |        |  +- courses A 147.32.232.159\n"
           " |     |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
           " |     |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           " |     |        |  \\- cz\n"
           " |     |        |     \\- cvut\n"
           " |     |        |        +- fit\n"
           " |     |        |        |  +- progtest A 147.32.232.142\n"
           " |     |        |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           " |     |        |        |  +- courses A 147.32.232.158\n"
           " |     |        |        |  +- courses A 147.32.232.160\n"
           " |     |        |        |  +- courses A 147.32.232.159\n"
           " |     |        |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
           " |     |        |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           " |     |        |        |  +- fel\n"
           " |     |        |        |  |  +- www A 147.32.80.2\n"
           " |     |        |        |  |  \\- www AAAA 1:2:3:4:5:6:7:8\n"
           " |     |        |        |  \\- cz\n"
           " |     |        |        |     \\- cvut\n"
           " |     |        |        |        +- fit\n"
           " |     |        |        |        |  +- progtest A 147.32.232.142\n"
           " |     |        |        |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           " |     |        |        |        |  +- courses A 147.32.232.158\n"
           " |     |        |        |        |  +- courses A 147.32.232.160\n"
           " |     |        |        |        |  +- courses A 147.32.232.159\n"
           " |     |        |        |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
           " |     |        |        |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           " |     |        |        |        |  \\- fel\n"
           " |     |        |        |        |     +- www A 147.32.80.2\n"
           " |     |        |        |        |     \\- www AAAA 1:2:3:4:5:6:7:8\n"
           " |     |        |        |        \\- fel\n"
           " |     |        |        |           +- www A 147.32.80.2\n"
           " |     |        |        |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
           " |     |        |        \\- fel\n"
           " |     |        |           +- www A 147.32.80.2\n"
           " |     |        |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
           " |     |        \\- fel\n"
           " |     |           +- www A 147.32.80.2\n"
           " |     |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
           " |     \\- fel\n"
           " |        +- www A 147.32.80.2\n"
           " |        \\- www AAAA 1:2:3:4:5:6:7:8\n"
           " \\- sk\n");
    oss.str("");
    oss << z22;
    assert(oss.str() ==
           "<ROOT ZONE>\n"
           " +- cz\n"
           " |  \\- cvut\n"
           " |     +- fit\n"
           " |     |  +- progtest A 147.32.232.142\n"
           " |     |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           " |     |  +- courses A 147.32.232.158\n"
           " |     |  +- courses A 147.32.232.160\n"
           " |     |  +- courses A 147.32.232.159\n"
           " |     |  +- pririz CNAME sto.fit.cvut.cz.\n"
           " |     |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           " |     |  +- fel\n"
           " |     |  |  +- www A 147.32.80.2\n"
           " |     |  |  \\- www AAAA 1:2:3:4:5:6:7:8\n"
           " |     |  \\- cz\n"
           " |     |     \\- cvut\n"
           " |     |        +- fit\n"
           " |     |        |  +- progtest A 147.32.232.142\n"
           " |     |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           " |     |        |  +- courses A 147.32.232.158\n"
           " |     |        |  +- courses A 147.32.232.160\n"
           " |     |        |  +- courses A 147.32.232.159\n"
           " |     |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
           " |     |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           " |     |        |  \\- cz\n"
           " |     |        |     \\- cvut\n"
           " |     |        |        +- fit\n"
           " |     |        |        |  +- progtest A 147.32.232.142\n"
           " |     |        |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           " |     |        |        |  +- courses A 147.32.232.158\n"
           " |     |        |        |  +- courses A 147.32.232.160\n"
           " |     |        |        |  +- courses A 147.32.232.159\n"
           " |     |        |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
           " |     |        |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           " |     |        |        |  +- fel\n"
           " |     |        |        |  |  +- www A 147.32.80.2\n"
           " |     |        |        |  |  \\- www AAAA 1:2:3:4:5:6:7:8\n"
           " |     |        |        |  \\- cz\n"
           " |     |        |        |     \\- cvut\n"
           " |     |        |        |        +- fit\n"
           " |     |        |        |        |  +- progtest A 147.32.232.142\n"
           " |     |        |        |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           " |     |        |        |        |  +- courses A 147.32.232.158\n"
           " |     |        |        |        |  +- courses A 147.32.232.160\n"
           " |     |        |        |        |  +- courses A 147.32.232.159\n"
           " |     |        |        |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
           " |     |        |        |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           " |     |        |        |        |  \\- fel\n"
           " |     |        |        |        |     +- www A 147.32.80.2\n"
           " |     |        |        |        |     \\- www AAAA 1:2:3:4:5:6:7:8\n"
           " |     |        |        |        \\- fel\n"
           " |     |        |        |           +- www A 147.32.80.2\n"
           " |     |        |        |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
           " |     |        |        \\- fel\n"
           " |     |        |           +- www A 147.32.80.2\n"
           " |     |        |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
           " |     |        \\- fel\n"
           " |     |           +- www A 147.32.80.2\n"
           " |     |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
           " |     \\- fel\n"
           " |        +- www A 147.32.80.2\n"
           " |        \\- www AAAA 1:2:3:4:5:6:7:8\n"
           " \\- de\n");
    oss.str("");
    oss << z25;
    assert(oss.str() ==
           "<ROOT ZONE>\n"
           " +- cz\n"
           " |  \\- cvut\n"
           " |     +- fit\n"
           " |     |  +- progtest A 147.32.232.142\n"
           " |     |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           " |     |  +- courses A 147.32.232.158\n"
           " |     |  +- courses A 147.32.232.160\n"
           " |     |  +- courses A 147.32.232.159\n"
           " |     |  +- pririz CNAME sto.fit.cvut.cz.\n"
           " |     |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           " |     |  +- fel\n"
           " |     |  |  +- www A 147.32.80.2\n"
           " |     |  |  \\- www AAAA 1:2:3:4:5:6:7:8\n"
           " |     |  \\- cz\n"
           " |     |     \\- cvut\n"
           " |     |        +- fit\n"
           " |     |        |  +- progtest A 147.32.232.142\n"
           " |     |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           " |     |        |  +- courses A 147.32.232.158\n"
           " |     |        |  +- courses A 147.32.232.160\n"
           " |     |        |  +- courses A 147.32.232.159\n"
           " |     |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
           " |     |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           " |     |        |  \\- cz\n"
           " |     |        |     \\- cvut\n"
           " |     |        |        +- fit\n"
           " |     |        |        |  +- progtest A 147.32.232.142\n"
           " |     |        |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           " |     |        |        |  +- courses A 147.32.232.158\n"
           " |     |        |        |  +- courses A 147.32.232.160\n"
           " |     |        |        |  +- courses A 147.32.232.159\n"
           " |     |        |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
           " |     |        |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           " |     |        |        |  +- fel\n"
           " |     |        |        |  |  +- www A 147.32.80.2\n"
           " |     |        |        |  |  \\- www AAAA 1:2:3:4:5:6:7:8\n"
           " |     |        |        |  \\- cz\n"
           " |     |        |        |     \\- cvut\n"
           " |     |        |        |        +- fit\n"
           " |     |        |        |        |  +- progtest A 147.32.232.142\n"
           " |     |        |        |        |  +- progtest AAAA 2001:718:2:2902:0:1:2:3\n"
           " |     |        |        |        |  +- courses A 147.32.232.158\n"
           " |     |        |        |        |  +- courses A 147.32.232.160\n"
           " |     |        |        |        |  +- courses A 147.32.232.159\n"
           " |     |        |        |        |  +- pririz CNAME sto.fit.cvut.cz.\n"
           " |     |        |        |        |  +- courses SPF ip4:147.32.232.128/25, ip4:147.32.232.64/26\n"
           " |     |        |        |        |  \\- fel\n"
           " |     |        |        |        |     +- www A 147.32.80.2\n"
           " |     |        |        |        |     \\- www AAAA 1:2:3:4:5:6:7:8\n"
           " |     |        |        |        \\- fel\n"
           " |     |        |        |           +- www A 147.32.80.2\n"
           " |     |        |        |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
           " |     |        |        \\- fel\n"
           " |     |        |           +- www A 147.32.80.2\n"
           " |     |        |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
           " |     |        \\- fel\n"
           " |     |           +- www A 147.32.80.2\n"
           " |     |           \\- www AAAA 1:2:3:4:5:6:7:8\n"
           " |     \\- fel\n"
           " |        +- www A 147.32.80.2\n"
           " |        \\- www AAAA 1:2:3:4:5:6:7:8\n"
           " \\- au\n");

    return 0;
}
#endif /* __PROGTEST__ */
