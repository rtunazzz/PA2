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
    map<string, CResult*> m_tests;

   public:
    CStudent() = delete;
    CStudent(const string& line) {
        // cout << "[CSTUDENT] Adding: " << line << endl;
        istringstream line_stream(line);
        string field;
        int index = 0;  // save index so we know which field we're reading
        // process the line
        while (getline(line_stream, field, ':')) {
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
                    // cout << "[CSTUDENT] Added ID: " << m_id << endl;
                    break;
                }
                case 1: {
                    // we're parsing the full name
                    m_fullname = field;
                    index += 1;
                    // cout << "[CSTUDENT] Added name: " << m_fullname << endl;
                    break;
                }
                case 2: {
                    // we're parsing card ids, which is a list separated by commas
                    istringstream field_stream(field);
                    string card;
                    while (getline(field_stream, card, ',')) {
                        // remove spaces
                        card.erase(remove_if(card.begin(), card.end(), ::isspace), card.end());

                        // add it
                        m_cards.push_back(card);
                        // cout << "[CSTUDENT] Added card: " << card << endl;
                    }
                    index += 1;
                    break;
                }
            }
        }
    }
    ~CStudent() {
        for (auto it : m_tests) {
            delete it.second;
        }
    }
    const unsigned int Id() const { return m_id; }
    const string& Name() const { return m_fullname; }
    const vector<string>& Cards() const { return m_cards; }

    bool WroteTest(const string& test) const {
        auto it = m_tests.find(test);
        if (it == m_tests.end()) {
            // cout << m_id << " does not have a test " << test << endl;
            return false;
        }
        return true;
    }

    bool IsTestAssessed(const string& test) const {
        auto it = m_tests.find(test);
        if (it == m_tests.end()) {
            // cout << m_id << " does not have a test " << test << endl;
            return false;
        }
        return (it->second != nullptr);
    }

    CResult Result(const string& test) const {
        auto it = m_tests.find(test);
        if (it == m_tests.end()) {
            // cout << m_id << " does not have a test " << test << endl;
        }
        return *it->second;
    }

    /**
     * @brief Registers student for the test specified.
     * @param test Test to register for
     * @return true When successfully registered
     * @return false When failed to register
     */
    bool Register(const string& test) {
        // check if student isn't registered for this test already
        if (m_tests.count(test) > 0) {
            // cout << m_id << " is already registered for test " << test << endl;
            return false;
        }
        m_tests.insert(make_pair(test, nullptr));
        return true;
    }

    /**
     * @brief Assesses the student the result provided from the test specified.
     * @param test Test to which to assess the result
     * @param result Result to be assessed
     * @return true When successfully assesses the result for the test specified
     * @return false When failed to assess the result for the test specified
     */
    bool Assess(const string& test, int result) {
        auto it = m_tests.find(test);
        if (it == m_tests.end()) {
            // cout << m_id << " is NOT registered for test " << test << endl;
            return false;
        }
        if (it->second != nullptr) {
            // cout << m_id << " already has a grade from test " << test << endl;
            return false;
        }
        it->second = new CResult(m_fullname, m_id, test, result);
        return true;
    }

    /**
     * @brief Utility function for printing the student details.
     */
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

    multimap<string, CStudent*> m_assessed_students_by_test;

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

    /**
     * @brief Loads students from the stream provided.
     * @param cardMap Stream to add students from
     * @return true When successfully loaded students
     * @return false When failed to load students
     */
    bool Load(istream& cardMap) {
        vector<CStudent*> student_buffer;
        string line;
        // read stream line by line
        while (getline(cardMap, line)) {
            CStudent* student = new CStudent(line);
            // check if students id isn't already present
            if (m_students_by_id.count(student->Id()) > 0) {
                // cout << "Student ID " << student->Id() << " is already present in this exam!";
                delete student;
                for (auto otherStudent : student_buffer) {
                    delete otherStudent;
                }
                return false;
            }

            // check if any of student's cards aren't already present
            auto cards = student->Cards();
            for (auto const& it : cards) {
                if (m_students_by_card.count(it) > 0) {
                    // cout << "Student card " << it << " is already present in this exam!";
                    delete student;
                    for (auto otherStudent : student_buffer) {
                        delete otherStudent;
                    }
                    return false;
                }
            }
            // cout << "Loaded student " << student->Name() << " with the ID: " << student->Id() << endl;
            student_buffer.push_back(student);
        }
        for (const auto& student : student_buffer) {
            m_students.push_back(student);
            m_students_by_id.insert(make_pair(student->Id(), student));
            for (auto const& it : student->Cards()) {
                m_students_by_card.insert(make_pair(it, student));
            }
        }
        return true;
    }

    /**
     * @brief Registers student specified by the CardID for the test specified.
     * @param cardID Student identifies
     * @param test Test to register for
     * @return true When successfully registered
     * @return false When failed to register
     */
    bool Register(const string& cardID, const string& test) {
        // search student up by his cardID
        auto it = m_students_by_card.find(cardID);
        if (it == m_students_by_card.end()) {
            // student wasn't found = card isn't recognized
            // cout << "Student card " << cardID << " was not found for the test " << test << endl;
            return false;
        }
        return it->second->Register(test);
    }

    /**
     * @brief Assesses the student specified by his studentID the result provided for the test specified.
     * @param studentID Identification of the student
     * @param test Test to which to assess the result
     * @param result Result to be assessed
     * @return true When successfully assesses the result for the test specified
     * @return false When failed to assess the result for the test specified
     */
    bool Assess(unsigned int studentID, const string& test, int result) {
        // find the student specified by his ID
        auto it = m_students_by_id.find(studentID);
        if (it == m_students_by_id.end()) {
            // student wasn't found = card isn't recognized
            // cout << "Student with ID " << studentID << " was not found!" << endl;
            return false;
        }
        if (it->second->Assess(test, result)) {
            // cout << "[" << test << "]"
            //      << " Assessed Student with ID " << studentID << " for " << result << endl;
            m_assessed_students_by_test.insert(make_pair(test, it->second));
            return true;
        } else {
            return false;
        }
    }

    /**
     * @brief Lists and sorts results of students that took the test specified by the testName.
     * @param testName Name of the test to list
     * @param sortBy Based on what to sort
     * @return list<CResult> List with the results
     */
    list<CResult> ListTest(const string& testName, int sortBy) const {
        list<CResult> result_list;

        if (sortBy == SORT_ID) {
            for (const auto& it : m_students_by_id) {
                if (it.second->WroteTest(testName) && it.second->IsTestAssessed(testName)) {
                    result_list.push_back(it.second->Result(testName));
                }
            }
            return result_list;
        }

        for (const auto& it : m_assessed_students_by_test) {
            // we gotta make sure there aren't duplicates
            if (it.first != testName) {
                continue;
            }
            if (it.second->WroteTest(testName)) {
                result_list.push_back(it.second->Result(testName));
            }
        }

        if (sortBy == SORT_NAME) {
            result_list.sort([](const CResult& lhs, const CResult& rhs) { return lhs.m_Name < rhs.m_Name; });
            // cout << "Sorted by name:" << endl;
        } else if (sortBy == SORT_RESULT) {
            result_list.sort([](const CResult& lhs, const CResult& rhs) { return lhs.m_Result > rhs.m_Result; });
            // cout << "Sorted by result:" << endl;
        } else {
            // cout << "Sorted by time assessed:" << endl;
        }
        // for (const auto& res : result_list) {
        //     cout << "\t" << res.m_StudentID << ", " << res.m_Name << ", " << res.m_Result << endl;
        // }
        return result_list;
    }

    set<unsigned int> ListMissing(const string& testName) const {
        set<unsigned int> s;
        for (const auto& it : m_students) {
            // check if student wrote the test and if it wasnt assessed
            if (it->WroteTest(testName) && !(it->IsTestAssessed(testName))) {
                s.insert(it->Id());
            }
        }
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
    CExam m2;
    return 0;
}
#endif /* __PROGTEST__ */
