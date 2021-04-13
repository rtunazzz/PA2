#ifndef __PROGTEST__
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
using namespace std;
#endif /* __PROGTEST__ */

template <typename T>
class CSet {
   private:
    class CNode {
       public:
        CNode* m_next;
        T m_val;
        const T& Value() const { return m_val; }
        CNode() = delete;
        CNode(const T& val) : m_next(nullptr), m_val(val) {}
    };

    CNode* m_begin;
    size_t m_size;

    /**
     * @brief Checks if values passed are equal.
     * @param _first First value
     * @param _second Second value
     * @return true When they are equal
     * @return false Otherwise (= they aren't equal)
     */
    bool _isEqual(const T& _first, const T& _second) const {
        // we only have the < operator available for comparing the node values
        // To get determine whether or not are the node values equal, we need to:
        //  - Check if fist value is lower than the second
        //      -> if it is, then they aren't equal
        //  - Check if second value is lower than the first
        //      -> if it is, then they aren't equal
        // Then if both checks are false, the values are equal

        return (!(_first < _second) && !(_second < _first));
    }

    bool _isInSet(const T& _item) const {
        CNode* tmp = m_begin;
        while (tmp != nullptr) {
            if (_isEqual(tmp->Value(), _item)) {
                return true;
            }
            tmp = tmp->m_next;
        }
        return false;
    }

    CNode* _getLastNode() const {
        CNode* tmp = m_begin;
        while (tmp->m_next != nullptr) {
            tmp = tmp->m_next;
        }
        return tmp;
    }

   public:
    /** @brief Construct a new CSet object */
    CSet() : m_begin(nullptr), m_size(0) {}

    /**
     * @brief Construct a new CSet object based off the one passed in.
     * @param _old Container to base the copy on
     */
    CSet(const CSet& _old) : m_begin(nullptr), m_size(0) {
        m_size = _old.m_size;
        CNode* oldNode = _old.m_begin;
        CNode* next = m_begin;
        while (oldNode != nullptr) {
            next = new CNode(oldNode->m_val);

            next = next->m_next;
            oldNode = oldNode->m_next;
        }
    }

    /**
     * @brief Overloads the = operator
     * and constructs a deep copy of the CSet passed in.
     * @param _old CSet to base the deep copy of
     * @return CSet& Reference to the newly created CSet
     */
    CSet& operator=(CSet _old) {
        std::swap(m_begin, _old.m_begin);
        std::swap(m_size, _old.m_size);
        return *this;
    }

    /** @brief Destroy the CSet object */
    ~CSet() {
        CNode* next = m_begin;
        while (next != nullptr) {
            CNode* tmp = next->m_next;
            delete next;
            next = tmp;
        }
    }

    /**
     * @brief Inserts a new item into our container.
     * @param _item Item to insert
     * @return true When inserting was successfull
     * @return false When inserting failed (= item was already in our container)
     */
    bool Insert(const T& _item) {
        if (_isInSet(_item)) {
            return false;
        }

        CNode* newNode = new CNode(_item);

        if (m_begin != nullptr) {
            CNode* lastNode = _getLastNode();
            lastNode->m_next = newNode;
        } else {
            m_begin = newNode;
        }
        m_size += 1;
        return true;
    }

    /**
     * @brief Removes an item from our container.
     * @param _item Item to be removed
     * @return true When removed successfully
     * @return false When failed to remove (= item wasn't in our container)
     */
    bool Remove(const T& _item) {
        if (m_begin == nullptr) {
            return false;
        }
        CNode* curr = m_begin;
        CNode* prev = nullptr;
        // traverse the nodes until the end/ until we found one which values are equal
        while ((curr != nullptr) && (!_isEqual(curr->Value(), _item))) {
            prev = curr;
            curr = curr->m_next;
        }

        if ((curr != nullptr) && _isEqual(curr->Value(), _item)) {
            // we found the item to delete
            if (prev != nullptr) prev->m_next = curr->m_next;
            // delete curr;
            m_size -= 1;
            return true;
        }
        return false;
    }

    /**
     * @brief Checks if the item passed in is contained in our container.
     * @param _item Item to check for
     * @return true When item is in our container
     * @return false When item isn't in our container
     */
    bool Contains(const T& _item) const {
        return _isInSet(_item);
    }

    /**
     * @brief Returns the size of our container.
     * @return int Size of the container
     */
    int Size() const { return (int)m_size; }
};

#ifndef __PROGTEST__
#include <cassert>

struct CSetTester {
    static void test0() {
        CSet<string> x0;
        assert(x0.Insert("Jerabek Michal"));
        assert(x0.Insert("Pavlik Ales"));
        assert(x0.Insert("Dusek Zikmund"));
        assert(x0.Size() == 3);
        assert(x0.Contains("Dusek Zikmund"));
        assert(!x0.Contains("Pavlik Jan"));
        assert(x0.Remove("Jerabek Michal"));
        assert(!x0.Remove("Pavlik Jan"));
    }

    static void test1() {
        CSet<string> x0;
        assert(x0.Insert("Jerabek Michal"));
        assert(x0.Insert("Pavlik Ales"));
        assert(x0.Insert("Dusek Zikmund"));
        assert(x0.Size() == 3);
        assert(x0.Contains("Dusek Zikmund"));
        assert(!x0.Contains("Pavlik Jan"));
        assert(x0.Remove("Jerabek Michal"));
        assert(!x0.Remove("Pavlik Jan"));
        CSet<string> x1(x0);
        assert(x0.Insert("Vodickova Saskie"));
        assert(x1.Insert("Kadlecova Kvetslava"));
        assert(x0.Size() == 3);
        assert(x1.Size() == 3);
        assert(x0.Contains("Vodickova Saskie"));
        assert(!x1.Contains("Vodickova Saskie"));
        assert(!x0.Contains("Kadlecova Kvetslava"));
        assert(x1.Contains("Kadlecova Kvetslava"));
    }

    static void test2() {
        CSet<string> x0;
        CSet<string> x1;
        assert(x0.Insert("Jerabek Michal"));
        assert(x0.Insert("Pavlik Ales"));
        assert(x0.Insert("Dusek Zikmund"));
        assert(x0.Size() == 3);
        assert(x0.Contains("Dusek Zikmund"));
        assert(!x0.Contains("Pavlik Jan"));
        assert(x0.Remove("Jerabek Michal"));
        assert(!x0.Remove("Pavlik Jan"));
        x1 = x0;
        assert(x0.Insert("Vodickova Saskie"));
        assert(x1.Insert("Kadlecova Kvetslava"));
        assert(x0.Size() == 3);
        assert(x1.Size() == 3);
        assert(x0.Contains("Vodickova Saskie"));
        assert(!x1.Contains("Vodickova Saskie"));
        assert(!x0.Contains("Kadlecova Kvetslava"));
        assert(x1.Contains("Kadlecova Kvetslava"));
    }

    static void test4() {
        CSet<int> x0;
        assert(x0.Insert(4));
        assert(x0.Insert(8));
        assert(x0.Insert(1));
        assert(x0.Size() == 3);
        assert(x0.Contains(4));
        assert(!x0.Contains(5));
        assert(!x0.Remove(5));
        assert(x0.Remove(4));
    }
};

int main() {
    // TODO pridat vlastni testy (~28:00)
    CSetTester::test0();
    CSetTester::test1();
    CSetTester::test2();
    CSetTester::test4();
    return 0;
}
#endif /* __PROGTEST__ */
