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
    // TODO

   public:
    /** @brief Construct a new CSet object */
    CSet() {}

    /**
     * @brief Construct a new CSet object based off the one passed in.
     * @param _old Container to base the copy on
     */
    CSet(const CSet& _old) {}

    // operator=
    CSet& operator=(const CSet& _old) {}

    /** @brief Destroy the CSet object */
    ~CSet() {}

    /**
     * @brief Inserts a new item into our container.
     * @param _item Item to insert
     * @return true When inserting was successfull
     * @return false When inserting failed (= item was already in our container)
     */
    bool Insert(const T& _item) {
    }

    /**
     * @brief Removes an item from our container.
     * @param _item Item to be removed
     * @return true When removed successfully
     * @return false When failed to remove (= item wasn't in our container)
     */
    bool Remove(const T& _item) {
    }

    /**
     * @brief Checks if the item passed in is contained in our container.
     * @param _item Item to check for
     * @return true When item is in our container
     * @return false When item isn't in our container
     */
    bool Contains(const T& _item) {
    }

    /**
     * @brief Returns the size of our container.
     * @return int Size of the container
     */
    int Size() const {
    }
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
