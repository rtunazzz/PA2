#ifndef __PROGTEST__
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
using namespace std;
#endif /* __PROGTEST__ */

/**
 * @brief Simple template implementation of the std::vector container.
 * @tparam T Type of the values we plan on storing in our container
 */
template <typename T>
class MyVector {
   private:
    /** @brief Buffer (array) which holds our data */
    T* m_buffer;

    /** @brief Maximum capacity of our buffer (data) */
    unsigned int m_max_capacity;

    /** @brief Size of our buffer (data) */
    unsigned int m_size;

    /**
     * @brief Swaps the vector values passed in with our current vector.
     * Expected to be used with the copy-and-swap idiom
     * @param old 
     */
    void swap(MyVector<T>& old) {
        m_buffer = old.m_buffer;
        m_max_capacity = old.m_max_capacity;
        m_size = old.m_size;
        old.m_buffer = nullptr;
    }

    /** @brief Increases (2x) the capacity of our container. */
    void increase_capacity() {
        // Initialize the new capactity
        unsigned int new_max_capacity = 2 * (this->m_max_capacity);
        if (new_max_capacity == 0) new_max_capacity = 1;  // TODO Increase for better performance

        // Initialize a new buffer and allocate memory
        T* new_buffer = new T[new_max_capacity];

        // Copy the contents in the current buffer to the new one
        for (int i = 0; i < this->m_size; ++i) {
            new_buffer[i] = (this->m_buffer)[i];
        }

        // Delete the old buffer & assign to member variables
        delete[](this->m_buffer);
        this->m_buffer = new_buffer;
        this->m_max_capacity = new_max_capacity;
    }

   public:
    // ====================================== CONSTRUCTOR / DESTRUCTOR / CCONSTRUCTOR ======================================
    /**
    * @brief Construct a new My Vector object
    * with maximum capacity set to 0 and size 0
    */
    MyVector() : m_max_capacity(0), m_size(0), m_buffer(nullptr) {}

    /**
     * @brief Copies a My Vector object
     * @param old source to be copied form
     */
    MyVector(const MyVector<T>& old) : m_max_capacity(old.m_size), m_size(old.m_size), m_buffer(new T[old.m_size]) {
        // copy contents of the old buffer to the new one
        for (int i = 0; i < old.m_size; ++i) {
            m_buffer[i] = old.m_buffer[i];
        }
    }

    /**
     * @brief Destroys the My Vector object and frees alocated memory.
     */
    ~MyVector() {
        if (m_buffer != nullptr)
            delete[] m_buffer;
    }

    // ====================================== OPERATOR OVERLOADING ======================================
    /**
     * @brief Overloads the = operator.
     * Uses the copy-and-swap idiom
     * @param old 
     * @return MyVector<T>&
     */
    MyVector<T>& operator=(MyVector<T> old) {
        swap(old);
        return *this;
    }

    /**
     * @brief Overloads the [] operator
     * Returns the T& located at the index passed in.
     * @param index 
     * @return T& 
     */
    T& operator[](unsigned int index) {
        return m_buffer[index];
    };

    // ====================================== METHODS ======================================
    /**
     * @brief Adds a value to our container.
     * @param var Variable to be added
     */
    void push_back(const T& var) {
        // if we're at max capacity, increase it
        if (m_size >= m_max_capacity) {
            increase_capacity();
        }
        // and add the variable to the buffer
        m_buffer[m_size] = var;
        m_size++;
    }

    /**
     * @brief Removes and returns the last element from the container.
     * !!! This method is different than the default implementation of the std::vector::pop_back method.
     */
    T pop_back() {
        T& to_return = m_buffer[--m_size];
        return to_return;
    }

    /**
     * @brief Returns the size of our container.
     * @return unsigned int 
     */
    unsigned int size() { return m_size; };

    /**
     * @brief Returns the last element from our container.
     * @return T& 
     */
    T& back() { return m_buffer[m_size - 1]; };

    /**
     * @brief Inserts an element onto the position passed in.
     * @param position index of where the element should be placed
     * @param var value which is to be placed at the possition passed in
     */
    void insert(const unsigned int position, const T& var) {
        // increase capacity if needed
        if (m_size >= m_max_capacity) {
            increase_capacity();
        }
        // move elements starting from the position passed in one index further,
        // which leaves the position index empty
        for (int i = (int)m_size; i > position; --i) {
            m_buffer[i] = m_buffer[i - 1];
        }
        m_buffer[position] = var;
        m_size++;
    }

    /** @brief Utility method to print the vector on the standard output. */
    void print() {
        for (int i = 0; i < m_size; ++i) {
            std::cout << m_buffer[i] << std::endl;
        }
    }
};

class CCarList {
   public:
    // copy cons, op=, dtor ...
    const char* RZ(void) const;
    bool AtEnd(void) const;
    void Next(void);

   private:
    // todo
};

class COwnerList {
   public:
    // copy cons, op=, dtor ...
    const char* Name(void) const;
    const char* Surname(void) const;
    bool AtEnd(void) const;
    void Next(void);

   private:
    // todo
};

class CRegister {
   public:
    // copy cons, op=, dtor ...
    bool AddCar(const char* rz,
                const char* name,
                const char* surname);
    bool DelCar(const char* rz);
    bool Transfer(const char* rz,
                  const char* nName,
                  const char* nSurname);
    CCarList ListCars(const char* name,
                      const char* surname) const;
    int CountCars(const char* name,
                  const char* surname) const;
    COwnerList ListOwners(const char* RZ) const;
    int CountOwners(const char* RZ) const;

   private:
    // todo
};

#ifndef __PROGTEST__
static bool matchList(CCarList&& l,
                      const char* rz1 = nullptr,
                      const char* rz2 = nullptr) {
    for (; !l.AtEnd(); l.Next())
        if (rz1 && !strcmp(l.RZ(), rz1))
            rz1 = nullptr;
        else if (rz2 && !strcmp(l.RZ(), rz2))
            rz2 = nullptr;
        else
            return false;
    return rz1 == nullptr && rz2 == nullptr;
}
int main(void) {
    char name[50], surname[50];
    CRegister b0;
    assert(b0.AddCar("ABC-12-34", "John", "Smith") == true);
    strncpy(name, "John", sizeof(name));
    strncpy(surname, "Hacker", sizeof(surname));
    assert(b0.AddCar("ABC-32-22", name, surname) == true);
    strncpy(name, "Peter", sizeof(name));
    strncpy(surname, "Smith", sizeof(surname));
    assert(b0.AddCar("XYZ-11-22", name, surname) == true);
    assert(b0.CountCars("John", "Hacker") == 1);
    assert(matchList(b0.ListCars("John", "Hacker"), "ABC-32-22"));
    assert(b0.CountOwners("ABC-12-34") == 1);
    COwnerList ol0 = b0.ListOwners("ABC-12-34");
    assert(!ol0.AtEnd() && !strcmp(ol0.Name(), "John") && !strcmp(ol0.Surname(), "Smith"));
    ol0.Next();
    assert(ol0.AtEnd());
    CRegister b1(b0);
    assert(b0.Transfer("XYZ-11-22", "John", "Hacker") == true);
    assert(b0.Transfer("XYZ-11-22", "Will", "Smith") == true);
    assert(b1.Transfer("XYZ-11-22", "John", "Smith") == true);
    assert(b0.CountOwners("XYZ-11-22") == 3);
    COwnerList ol1 = b0.ListOwners("XYZ-11-22");
    assert(!ol1.AtEnd() && !strcmp(ol1.Name(), "Will") && !strcmp(ol1.Surname(), "Smith"));
    ol1.Next();
    assert(!ol1.AtEnd() && !strcmp(ol1.Name(), "John") && !strcmp(ol1.Surname(), "Hacker"));
    ol1.Next();
    assert(!ol1.AtEnd() && !strcmp(ol1.Name(), "Peter") && !strcmp(ol1.Surname(), "Smith"));
    ol1.Next();
    assert(ol1.AtEnd());
    assert(b1.CountOwners("XYZ-11-22") == 2);
    COwnerList ol2 = b1.ListOwners("XYZ-11-22");
    assert(!ol2.AtEnd() && !strcmp(ol2.Name(), "John") && !strcmp(ol2.Surname(), "Smith"));
    ol2.Next();
    assert(!ol2.AtEnd() && !strcmp(ol2.Name(), "Peter") && !strcmp(ol2.Surname(), "Smith"));
    ol2.Next();
    assert(ol2.AtEnd());
    b1 = b0;
    assert(b0.DelCar("XYZ-11-22") == true);
    assert(b1.DelCar("ABC-12-34") == true);
    assert(b0.CountCars("John", "Smith") == 1);
    assert(matchList(b0.ListCars("John", "Smith"), "ABC-12-34"));
    assert(b1.CountCars("John", "Smith") == 0);
    assert(matchList(b1.ListCars("John", "Smith")));

    CRegister b2;
    assert(b2.AddCar("ABC-12-34", "John", "Smith") == true);
    assert(b2.AddCar("ABC-32-22", "John", "Hacker") == true);
    assert(b2.AddCar("XYZ-11-22", "Peter", "Smith") == true);
    assert(b2.AddCar("XYZ-11-22", "Jane", "Black") == false);
    assert(b2.DelCar("AAA-11-11") == false);
    assert(b2.DelCar("XYZ-11-22") == true);
    assert(b2.Transfer("BBB-99-99", "John", "Smith") == false);
    assert(b2.Transfer("ABC-12-34", "John", "Smith") == false);
    assert(b2.Transfer("XYZ-11-22", "John", "Smith") == false);
    assert(b2.CountCars("George", "White") == 0);
    assert(matchList(b2.ListCars("George", "White")));
    assert(b2.CountOwners("AAA-AA-AA") == 0);
    COwnerList ol3 = b2.ListOwners("AAA-AA-AA");
    assert(ol3.AtEnd());

    return 0;
}
#endif /* __PROGTEST__ */
