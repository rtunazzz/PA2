#ifndef __PROGTEST__
#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class CResult {
   public:
    CResult(const string& name, unsigned int studentID, const string& test, int result)
        : m_Name(name), m_StudentID(studentID), m_Test(test), m_Result(result) {}

    bool operator==(const CResult& other) const {
        return m_Name == other.m_Name && m_StudentID == other.m_StudentID && m_Test == other.m_Test && m_Result == other.m_Result;
    }

    string m_Name;
    unsigned int m_StudentID;
    string m_Test;
    int m_Result;
};

#endif /* __PROGTEST__ */

class CStudent {
   private:
    unsigned int m_id;
    string m_fullname;
    vector<string> m_cards;
    CResult* m_result;

   public:
    CStudent() = delete;
    CStudent(const string& line) : m_result(nullptr) {
        istringstream s(line);
        string field;
        int index = 0;  // save index so we know which field we're reading
        // process the line
        while (getline(s, field, ':')) {
            // process the field
            switch (index) {
                case 0: {
                    // we're parsing the student_id
                    size_t* check = nullptr;
                    m_id = stoul(field, check, 10);
                    if (check != nullptr) {
                        cout << "Failed to parse Student ID! '" << *check << "' is not an unsinged int.";
                    }
                    index += 1;
                    break;
                }
                case 1: {
                    // we're parsing the full name
                    m_fullname = field;
                    index += 1;
                    break;
                }
                default: {
                    // we're parsing card id
                    m_cards.push_back(field);
                    index += 1;
                    break;
                }
            }
        }
    }

    const unsigned int Id() const { return m_id; }
    const string& Name() const { return m_fullname; }
    const vector<string>& Cards() const { return m_cards; }

    void Print() {
        cout << m_fullname << " (" << m_id << ") ";
        cout << "Cards: [";
        for (const auto& it : m_cards) {
            cout << it << ", ";
        }
        cout << "]" << endl;
    }
};

class CExam {
   private:
    vector<CStudent*> m_students;

    /** @brief Map with ids as keys and pointer to the students the id belongs as the value */
    map<unsigned int, CStudent*> m_students_by_id;

    /** @brief Map with cards as keys and pointer to the students the card belongs as the value */
    map<string, CStudent*> m_students_by_card;

    /**
     * @brief Container to keep data about the current exam results.
     * Time is the time when the student's result was assessed
     */
    map<time_t, CStudent*> m_students_by_assess;

   public:
    static const int SORT_NONE = 0;
    static const int SORT_ID = 1;
    static const int SORT_NAME = 2;
    static const int SORT_RESULT = 3;

    /**
     * @brief Construct a new CExam object
     * Supposed to be implicit, so we keep it set to default.
     */
    CExam() = default;
    ~CExam() {
        for (auto it : m_students) {
            delete it;
        }
    }
    bool Load(istream& cardMap) {
        // TODO do i have to load students into TMP and drop them when one in the stream fails?

        string line;
        // read stream line by line
        while (getline(cardMap, line)) {
            CStudent* student = new CStudent(line);
            // check if students id isn't already present
            if (m_students_by_id.count(student->Id()) > 0) {
                cout << "Student ID " << student->Id() << " is already present in this exam!";
                delete student;
                return false;
            }

            // check if any of student's cards aren't already present
            auto cards = student->Cards();
            for (auto const& it : cards) {
                if (m_students_by_card.count(it) > 0) {
                    cout << "Student card " << it << " is already present in this exam!";
                    delete student;
                    return false;
                }
            }
            m_students.push_back(student);
            m_students_by_id.insert(make_pair(student->Id(), student));
            for (auto const& it : cards) {
                m_students_by_card.insert(make_pair(it, student));
            }
        }
        return true;
    }

    bool Register(const string& cardID, const string& test) { return true; }

    bool Assess(unsigned int studentID, const string& test, int result) { return true; }

    list<CResult> ListTest(const string& testName, int sortBy) const {
        list<CResult> l;
        return l;
    }

    set<unsigned int> ListMissing(const string& testName) const {
        set<unsigned int> s;
        return s;
    }
};

#ifndef __PROGTEST__
int main(void) {
    istringstream iss;
    CExam m;
    iss.clear();
    iss.str(
        "123456:Smith John:er34252456hjsd2451451, 1234151asdfe5123416, asdjklfhq3458235\n"
        "654321:Nowak Jane: 62wtsergtsdfg34\n"
        "456789:Nowak Jane: okjer834d34\n"
        "987:West Peter Thomas:sdswertcvsgncse\n");
    assert(m.Load(iss));

    assert(m.Register("62wtsergtsdfg34", "PA2 - #1"));
    assert(m.Register("62wtsergtsdfg34", "PA2 - #2"));
    assert(m.Register("er34252456hjsd2451451", "PA2 - #1"));
    assert(m.Register("er34252456hjsd2451451", "PA2 - #3"));
    assert(m.Register("sdswertcvsgncse", "PA2 - #1"));
    assert(!m.Register("1234151asdfe5123416", "PA2 - #1"));
    assert(!m.Register("aaaaaaaaaaaa", "PA2 - #1"));
    assert(m.Assess(123456, "PA2 - #1", 50));
    assert(m.Assess(654321, "PA2 - #1", 30));
    assert(m.Assess(654321, "PA2 - #2", 40));
    assert(m.Assess(987, "PA2 - #1", 100));
    assert(!m.Assess(654321, "PA2 - #1", 35));
    assert(!m.Assess(654321, "PA2 - #3", 35));
    assert(!m.Assess(999999, "PA2 - #1", 35));
    assert(m.ListTest("PA2 - #1", CExam::SORT_RESULT) == (list<CResult>{
                                                             CResult("West Peter Thomas", 987, "PA2 - #1", 100),
                                                             CResult("Smith John", 123456, "PA2 - #1", 50),
                                                             CResult("Nowak Jane", 654321, "PA2 - #1", 30)}));
    assert(m.ListTest("PA2 - #1", CExam::SORT_NAME) == (list<CResult>{
                                                           CResult("Nowak Jane", 654321, "PA2 - #1", 30),
                                                           CResult("Smith John", 123456, "PA2 - #1", 50),
                                                           CResult("West Peter Thomas", 987, "PA2 - #1", 100)}));
    assert(m.ListTest("PA2 - #1", CExam::SORT_NONE) == (list<CResult>{
                                                           CResult("Smith John", 123456, "PA2 - #1", 50),
                                                           CResult("Nowak Jane", 654321, "PA2 - #1", 30),
                                                           CResult("West Peter Thomas", 987, "PA2 - #1", 100)}));
    assert(m.ListTest("PA2 - #1", CExam::SORT_ID) == (list<CResult>{
                                                         CResult("West Peter Thomas", 987, "PA2 - #1", 100),
                                                         CResult("Smith John", 123456, "PA2 - #1", 50),
                                                         CResult("Nowak Jane", 654321, "PA2 - #1", 30)}));
    assert(m.ListMissing("PA2 - #3") == (set<unsigned int>{123456}));
    iss.clear();
    iss.str("888:Watson Joe:25234sdfgwer52, 234523uio, asdf234235we, 234234234\n");
    assert(m.Load(iss));

    assert(m.Register("234523uio", "PA2 - #1"));
    assert(m.Assess(888, "PA2 - #1", 75));
    iss.clear();
    iss.str(
        "555:Gates Bill:ui2345234sdf\n"
        "888:Watson Joe:2345234634\n");
    assert(!m.Load(iss));

    assert(!m.Register("ui2345234sdf", "PA2 - #1"));
    iss.clear();
    iss.str(
        "555:Gates Bill:ui2345234sdf\n"
        "666:Watson Thomas:okjer834d34\n");
    assert(!m.Load(iss));

    assert(!m.Register("ui2345234sdf", "PA2 - #3"));
    iss.clear();
    iss.str(
        "555:Gates Bill:ui2345234sdf\n"
        "666:Watson Thomas:jer834d3sdf4\n");
    assert(m.Load(iss));

    assert(m.Register("ui2345234sdf", "PA2 - #3"));
    assert(m.ListMissing("PA2 - #3") == (set<unsigned int>{555, 123456}));

    return 0;
}
#endif /* __PROGTEST__ */
