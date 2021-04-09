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
    /** @brief Buffer (pointer array) which holds our data */
    T* m_buffer;

    /** @brief Maximum capacity of our buffer (pointer array) */
    int m_max_capacity;

    /** @brief Size of our buffer (pointer array) */
    int m_size;

    /** @brief Increases (2x) the capacity of our container. */
    void increase_capacity() {
        // Initialize the new capactity
        int new_max_capacity = 2 * m_max_capacity;
        if (new_max_capacity == 0) new_max_capacity = 10;

        // Initialize a new buffer and allocate memory
        T* new_buffer = new T[new_max_capacity];

        // Copy the contents in the current buffer to the new one
        for (int i = 0; i < m_size; ++i) {
            new_buffer[i] = m_buffer[i];
        }

        // Delete the old buffer & assign to member variables
        if (m_buffer != nullptr)
            delete[](m_buffer);

        m_buffer = new_buffer;
        m_max_capacity = new_max_capacity;
    }

   public:
    // ====================================== CONSTRUCTOR / DESTRUCTOR / CCONSTRUCTOR ======================================
    /**
    * @brief Construct a new My Vector object
    * with maximum capacity set to 0 and size 0
    */
    MyVector() : m_buffer(nullptr), m_max_capacity(0), m_size(0) {}

    /**
     * @brief Copies a My Vector object
     * @param old source to be copied form
     */
    MyVector(const MyVector<T>& old) : m_buffer(new T[old.m_max_capacity]), m_max_capacity(old.m_max_capacity), m_size(old.m_size) {
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
        std::swap(m_buffer, old.m_buffer);
        std::swap(m_max_capacity, old.m_max_capacity);
        std::swap(m_size, old.m_size);
        return *this;
    }

    /**
     * @brief Overloads the [] operator
     * Returns the T& located at the index passed in.
     * @param index 
     * @return T& 
     */
    T& operator[](int index) {
        if (index >= m_size) {
            throw std::out_of_range("index is greater or equal to the container's size!");
            exit(0);
        }
        return m_buffer[index];
    };

    /**
     * @brief Overloads the [] operator
     * Returns the T& located at the index passed in.
     * @param index 
     * @return T& 
     */
    const T& operator[](int index) const {
        if (index >= m_size) {
            throw std::out_of_range("index is greater or equal to the container's size!");
            exit(0);
        }
        return m_buffer[index];
    };

    // ====================================== METHODS ======================================
    /**
     * @brief Adds a value to our container.
     * @param var Variable to be added
     */
    MyVector& push_back(const T& var) {
        // if we're at max capacity, increase it
        if (m_size >= m_max_capacity) {
            increase_capacity();
        }
        // and add the variable to the buffer
        m_buffer[m_size] = var;
        m_size += 1;
        return *this;
    }

    /** @brief Removes and the last element from our container.*/
    void pop_back() {
        if (m_size > 0) {
            m_size -= 1;
        }
    }

    /**
     * @brief Returns the size of our container.
     * @return  int 
     */
    int size() const { return m_size; };

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
    void insert(const int position, const T& var) {
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
        m_size += 1;
    }

    void erase(const int position) {
        // fill the "deleted" position
        m_size -= 1;
        for (int i = (int)position; i < m_size; i++) {
            m_buffer[i] = m_buffer[i + 1];
        }
    }

    /** @brief Utility method to print the vector on the standard output. */
    void print() const {
        std::cout << "[ " << endl;
        for (int i = 0; i < m_size; ++i) {
            std::cout << "\t" << m_buffer[i] << std::endl;
        }
        std::cout << "] " << endl;
    }
};

/** @brief Represents a Car record in a car registry */
class CarRecord {
   private:
    /** @brief Whether or not is the card record archived */
    bool m_archived;

   public:
    // TODO make these private & add getters and setters
    /** @brief License Plate number of the car */
    char* rz;
    /** @brief Name of the car's owner */
    char* name;
    /** @brief Surname of the car's owner */
    char* surname;

    /** @brief Construct a new CarRecord object without any parameters */
    CarRecord() : m_archived(false), rz(nullptr), name(nullptr), surname(nullptr) {}

    /**
     * @brief Construct a new CarRecord object with the parameters specified
     * @param _rz License Plate number of the car
     * @param _name Name of the car's owner
     * @param _surname Surname of the car's owner
     */
    CarRecord(const char* _rz, const char* _name, const char* _surname) : m_archived(false) {
        rz = new char[strlen(_rz) + 1];
        strcpy(rz, _rz);

        name = new char[strlen(_name) + 1];
        strcpy(name, _name);

        surname = new char[strlen(_surname) + 1];
        strcpy(surname, _surname);
    }

    /** @brief Destroy the Car Record object and dealocates memory */
    ~CarRecord() {
        delete rz;
        delete name;
        delete surname;
    }

    /**
     * @brief Construct a new Car Record object from the old car provided
     * @param old CarRecord to copy
     */
    CarRecord(const CarRecord& old) : m_archived(old.IsArchived()) {
        rz = new char[strlen(old.rz) + 1];
        strcpy(rz, old.rz);

        name = new char[strlen(old.name) + 1];
        strcpy(name, old.name);

        surname = new char[strlen(old.surname) + 1];
        strcpy(surname, old.surname);
    }

    CarRecord& operator=(CarRecord old) {
        std::swap(rz, old.rz);
        std::swap(name, old.name);
        std::swap(surname, old.surname);

        m_archived = old.IsArchived();
        return *this;
    }

    /** @brief Archives the CarRecord */
    void Archive() {
        m_archived = true;
    }

    /**
     * @brief Checks whether or not is the current car record archived
     * @return true When it is archived
     * @return false When it isn't archived
     */
    bool IsArchived() const {
        return m_archived;
    }

    friend ostream& operator<<(ostream& os, const CarRecord& c) {
        os << "[" << c.rz << "] - owned by: " << c.name << " " << c.surname << " (archived: " << (c.IsArchived() ? "true" : "false")
           << ")";
        return os;
    }
};

/** @brief Represents an iterator for cars in a car registry */
class CCarList {
   private:
    MyVector<CarRecord> m_registry;
    int m_index;

    /**
     * @brief Filters through the _registry passed in
     * and adds to our list all occurances that match the provided name & surname
     * @param _registry Registry to filter through
     * @param _name Name to filter
     * @param _surname Surname to filter
     */
    void filterAndAdd(const MyVector<CarRecord>& _registry, const char* _name, const char* _surname) {
        for (int i = _registry.size() - 1; i > -1; i--) {
            if (!_registry[i].IsArchived() && strcmp(_name, _registry[i].name) == 0 && strcmp(_surname, _registry[i].surname) == 0) {
                m_registry.push_back(CarRecord(_registry[i]));
            }
        }
    }

   public:
    CCarList() = delete;
    /** @brief Destroys the CCarList object. */
    ~CCarList() = default;

    /**
    * @brief Construct a new CCarList object.
    * The object then includes cars which where previously owned by the person specified
    * by the name & surname passed in.
    * @param _registry Registry to construst the CCarList over
    * @param _name Name of the person to construct the CCarList over
    * @param _surname Surname of the person to construct the CCarList over
    */
    CCarList(const MyVector<CarRecord>& _registry, const char* _name, const char* _surname) : m_index(0) {
        filterAndAdd(_registry, _name, _surname);
    }

    /**
     * @brief Copyconstructor which constructs a new CCarList object.
     * Creates a deep copy of the old CCarList passed in.
     * @param old CCarList to copy
     */
    CCarList(const CCarList& old) {
        for (int i = 0; i < old.m_registry.size(); i++) {
            m_registry.push_back(old.m_registry[i]);
        }
    }

    CCarList& operator=(CCarList old) {
        std::swap(m_registry, old.m_registry);
        return *this;
    }

    /**
     * @brief Returns the license plate number of the current car.
     * @return const char* License plate number
     */
    const char* RZ(void) const {
        if (!AtEnd())
            return m_registry[m_index].rz;
        return nullptr;
    };

    /**
     * @brief Checks whether or not we are in the end of the list.
     * @return true When we are in the end of the list
     * @return false When we aren't in the end of the list)
     */
    bool AtEnd(void) const { return m_registry.size() <= m_index; }

    /** @brief Moves the current position in the list one forward */
    void Next(void) { m_index++; }
};

/** @brief Represents an iterator for owners in a car registry */
class COwnerList {
   private:
    MyVector<CarRecord> m_registry;
    int m_index;

    /**
     * @brief Filters through the _registry passed in
     * and adds to our list all occurances that match the provided name & surname
     * @param _registry Registry to filter through
    * @param _rz License plate number to construct the COwnerList over
     */
    void filterAndAdd(const MyVector<CarRecord>& _registry, const char* _rz) {
        for (int i = _registry.size() - 1; i > -1; i--) {
            if (strcmp(_rz, _registry[i].rz) == 0) {
                // TODO keep or remove previous owners?
                m_registry.push_back(CarRecord(_registry[i]));
            }
        }
    }

   public:
    COwnerList() = delete;
    /** @brief Destroys the COwnerList object. */
    ~COwnerList() = default;

    /**
    * @brief Construct a new COwnerList object.
    * The object then includes people which own or previously owned the car specified
    * by the license place number passed in.
    * @param _registry Registry to construst the COwnerList over
    * @param _rz License plate number to construct the COwnerList over
    */
    COwnerList(const MyVector<CarRecord>& _registry, const char* _rz) : m_index(0) {
        filterAndAdd(_registry, _rz);
    }

    /**
     * @brief Copyconstructor which constructs a new COwnerList object.
     * Creates a deep copy of the old COwnerList passed in.
     * @param old COwnerList to copy
     */
    COwnerList(const COwnerList& old) {
        for (int i = 0; i < old.m_registry.size(); i++) {
            m_registry.push_back(old.m_registry[i]);
        }
    }

    COwnerList& operator=(COwnerList old) {
        std::swap(m_registry, old.m_registry);
        return *this;
    }
    /**
     * @brief Returns the name of the owner we're currently on.
     * @return const char* Name
     */
    const char* Name(void) const {
        if (!AtEnd())
            return m_registry[m_index].name;
        return nullptr;
    }
    /**
     * @brief Returns the surname of the owner we're currently on.
     * @return const char* Surname
     */
    const char* Surname(void) const {
        if (!AtEnd())
            return m_registry[m_index].surname;
        return nullptr;
    }
    /**
     * @brief Checks whether or not we are in the end of the list.
     * @return true When we are in the end of the list
     * @return false When we aren't in the end of the list)
     */
    bool AtEnd(void) const { return m_registry.size() <= m_index; }
    /** @brief Moves the current position in the list one forward */
    void Next(void) { m_index++; }
};

/** @brief Represents a car registry */
class CRegister {
   private:
    MyVector<CarRecord> m_data;

   public:
    CRegister() = default;
    ~CRegister() = default;

    CRegister(const CRegister& old) {
        m_data = old.m_data;
    }

    CRegister& operator=(CRegister old) {
        std::swap(m_data, old.m_data);
        return *this;
    }

    /**
     * @brief Adds a car to the registry.
     * @param rz license plate number
     * @param name first name of the owner
     * @param surname last name of the owner
     * @return true When added successfully
     * @return false When failed to add
     */
    bool AddCar(const char* rz, const char* name, const char* surname) {
        // check if car with the same rz exists
        for (int i = 0; i < m_data.size(); i++) {
            if (strcmp(rz, m_data[i].rz) == 0) {
                // if it does exist, return false
                return false;
            }
        }
        // otherwise add it

        // cout << "============================================================" << endl;
        // cout << "Before adding:" << endl;
        // m_data.print();
        // cout << "=============== Adding rz: '" << rz << "' =====================" << endl;

        m_data.push_back(CarRecord(rz, name, surname));

        // cout << "After adding:" << endl;
        // m_data.print();
        // cout << "============================================================" << endl;

        return true;
    }

    /**
     * @brief Deletes a car from the registry.
     * @param rz license plate number
     * @return true When found and deleted
     * @return false When not found
     */
    bool DelCar(const char* rz) {
        bool deleted = false;
        // TODO unsure if this should be deleting just one record or all records with the matching rz
        // find the car with the same rz
        for (int i = 0; i < m_data.size(); i++) {
            if (strcmp(rz, m_data[i].rz) == 0) {
                // when found, remove it
                // cout << "============================================================" << endl;
                // cout << "Before delete:" << endl;
                // m_data.print();

                // cout << "=============== DELETING rz: '" << m_data[i].rz << "' ===============" << endl;
                m_data.erase(i);

                // cout << "After delete:" << endl;
                // m_data.print();
                // cout << "============================================================" << endl;

                deleted = true;
            }
        }
        return deleted;
    }

    /**
     * @brief Counts how many cars an a person has.
     * Person is idenfitifed by the name & surname parameters
     * @param name
     * @param surname 
     * @return int 
     */
    int CountCars(const char* name, const char* surname) const {
        int count = 0;

        // Iterate over our container and increase count if the name and surname match.
        for (int i = 0; i < m_data.size(); i++) {
            if (!m_data[i].IsArchived() && strcmp(name, m_data[i].name) == 0 && strcmp(surname, m_data[i].surname) == 0) {
                count++;
            }
        }

        // cout << "============================================================" << endl;
        // cout << name << " " << surname << " has " << count << " cars." << endl;
        // cout << "============================================================" << endl;

        return count;
    }

    /**
     * @brief Counts how many owners a car had in the history of our register.
     * @param RZ License plate of the car we're looking for
     * @return int Number of owners
     */
    int CountOwners(const char* RZ) const {
        int count = 0;
        // Iterate over our container and increase count if the RZ.
        for (int i = 0; i < m_data.size(); i++) {
            if (strcmp(RZ, m_data[i].rz) != 0)
                continue;

            bool duplicate = false;
            // iterate over the previously checked names
            for (int j = 0; j < i; j++) {
                if (strcmp(RZ, m_data[j].rz) != 0)
                    continue;

                // if any match, it's a duplicate name
                if (strcmp(m_data[i].name, m_data[j].name) == 0 && strcmp(m_data[i].surname, m_data[j].surname) == 0) {
                    duplicate = true;
                    break;
                }
            }
            if (!duplicate) {
                count++;
            }
        }

        // cout << "============================================================" << endl;
        // cout << RZ << " was owned by " << count << " people." << endl;
        // cout << "============================================================" << endl;

        return count;
    }

    /**
     * @brief Transfers a car to the person identified by the nName and nSurname parameters.
     * @param rz License plate of the car to transfer
     * @param nName First name of the new owner
     * @param nSurname Last name of the new owner
     * @return true When transfer was successfully completed
     * @return false When car doesn't exist or the current owner & the new owner are the same.
     */
    bool Transfer(const char* rz, const char* nName, const char* nSurname) {
        // find the car we are trying to transfer
        for (int i = 0; i < m_data.size(); i++) {
            // check if the car isn't archived and if RZ matches
            if (!m_data[i].IsArchived() && strcmp(rz, m_data[i].rz) == 0) {
                // we found a car with a matching rz that isn't archived

                // check if the owners are different
                if (strcmp(nSurname, m_data[i].surname) == 0 && strcmp(nName, m_data[i].name) == 0) {
                    // owners are the same
                    return false;
                }
                // cout << "============================================================" << endl;
                // cout << "Before transfer:" << endl;
                // m_data.print();

                // cout << "=============== TRANSFERRING rz: '" << m_data[i].rz << "' ===============" << endl;

                m_data[i].Archive();
                m_data.push_back(CarRecord(rz, nName, nSurname));

                // cout << "After transfer:" << endl;
                // m_data.print();
                // cout << "============================================================" << endl;

                return true;
            }
        }
        // not found
        return false;
    }

    /**
     * @brief Searches through the registry and returns an object
     * which can be used to list all cars a person specified by the name
     * and surname owns.
     * @param _name Name to list
     * @param _surname Surname to list
     * @return CCarList Object to iterate over
     */
    CCarList ListCars(const char* name, const char* surname) const {
        // m_data.print();
        return CCarList(m_data, name, surname);
    }

    /**
     * @brief Searches through the registry and returns an object
     * which can be used to list all owners of the RZ specified..
     * @param _name Name to list
     * @param _surname Surname to list
     * @return COwnerList Object to iterate over
     */
    COwnerList ListOwners(const char* RZ) const {
        // m_data.print();
        return COwnerList(m_data, RZ);
    }
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

    // CUSTOM TESTS
    CRegister b3;
    b3.AddCar("ABC-12-34", "John", "Smith");
    b3.Transfer("ABC-12-34", "Jeremy", "Black");
    b3.Transfer("ABC-12-34", "John", "Smith");
    b3.Transfer("ABC-12-34", "Jeremy", "Black");
    assert(b3.CountOwners("ABC-12-34") == 2);

    CRegister b4 = b3;
    b4.AddCar("ABC-12-34", "John", "Smith");
    b4.AddCar("ABC-32-22", "John", "Hacker");
    b4.AddCar("XYZ-11-22", "Peter", "Smith");
    b4.AddCar("XYZ-11-22", "Jane", "Black");
    b4.AddCar("XYZ-11-22XXXXX22XXXXX22XXXXX22XXXXX22XXXXX22XXXXX22XXXXX22XXXXXXYZ-11-22XXXXX22XXXXX22XXXXX22XXXXX22XXXXX22XXXXX22XXXXX22XXXXX", "XYZ-11-22XXXXX22XXXXX22XXXXX22XXXXX22XXXXX22XXXXX22XXXXX22XXXXX", "XYZ-11-22XXXXX22XXXXX22XXXXX22XXXXX22XXXXX22XXXXX22XXXXX22XXXXX");
    b4.AddCar("", "", "");
    b4.DelCar("AAA-11-11");
    b4.DelCar("XYZ-11-22");
    b4.Transfer("BBB-99-99", "John", "Smith");
    b4.Transfer("ABC-12-34", "John", "Smith");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "John", "Smith");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "John", "Smith");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "John", "Smith");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "John", "Smith");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "John", "Smith");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "John", "Smith");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "John", "Smith");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "John", "Smith");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "John", "Smith");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "John", "Smith");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "John", "Smith");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "John", "Smith");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "John", "Smith");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "John", "Smith");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "John", "Smith");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "John", "Smith");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.Transfer("ABC-12-34", "John", "Smith");
    b4.Transfer("ABC-12-34", "Jeremy", "Black");
    b4.CountCars("George", "White");
    return 0;
}
#endif /* __PROGTEST__ */
