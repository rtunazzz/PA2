
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
#include <list>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;
#endif /* __PROGTEST__ */

class CEntity {
   public:
    /** @brief Name of the file */
    string m_Name;

    virtual ~CEntity() {}
    virtual int Size() const { return -1; }
    friend ostream& operator<<(ostream& os, const CEntity& e) { return os; }
};

/**
 * @brief Represents a basic file.
 */
class CFile : public CEntity {
   public:
    /** @brief Hash of the file's data */
    string m_Hash;

    /** @brief Size of the file */
    int m_Filesize;

    /**
     * @brief Construct a new CFile object
     * @param hash Hash of the file's data
     * @param filesize Size of the file
     */
    CFile(const string hash, int filesize) : m_Hash(hash), m_Filesize(filesize) {
        m_Name = "";
    }

    /**
     * @brief Returns the current size of the file.
     * @return int Size of the file
     */
    int Size() const {
        return m_Filesize;
    }

    /**
     * @brief Sets a new hash and size to the file
     * @param hash Hash of the file's data
     * @param filesize Size of the file
     * @return CFile& Reference to self
     */
    CFile& Change(const string hash, int filesize) {
        m_Hash = hash;
        m_Filesize = filesize;
        return *this;
    }

    friend ostream& operator<<(ostream& os, const CFile& file) {
        os << file.Size() << "\t" << file.m_Name << " " << file.m_Hash;
        return os;
    }
};

/**
 * @brief Represents a link.
 */
class CLink : public CEntity {
   public:
    /** @brief Path to the link */
    string m_Path;

    // Constructor
    CLink(const string path) : m_Path(path) {
        m_Name = "";
    }

    // Size
    int Size() const {
        return m_Path.size() + 1;
    }

    // Change
    CLink& Change(const string path) {
        m_Path = path;
        return *this;
    }

    friend ostream& operator<<(ostream& os, const CLink& link) {
        os << link.Size() << "\t" << link.m_Name << " -> " << link.m_Path;
        return os;
    }
};

/**
 * @brief Represents a directory.
 */
class CDirectory : public CEntity {
   public:
    map<string, CEntity*> data;

    /**
     * @brief Returns the size of a directory.
     * Size of the directory is equal to number of files (including directories) and the number of characters in their name.
     * @return int Size of the directory
     */
    int Size() const {
        int tmpSize = 0;

        // Iterate over all the files
        for (auto const& it : data) {
            tmpSize += it.second->Size() + it.first.size();
        }

        return tmpSize;
    }

    /**
     * @brief Deletes a file from the directory specified by the filename.
     * @param filename Name of the file to delete.
     */
    CDirectory& Change(string filename, CEntity* ent = nullptr) {
        if (ent) {
            return Change(filename, *ent);
        } else {
            auto it = find_if(data.begin(), data.end(), [&filename](const pair<string, CEntity*>& p) { return p.first == filename; });
            if (it != data.end()) {
                // delete the file form the map
                delete (it->second);
                data.erase(it);
                return *this;
            }
        }

        return *this;
    }

    /**
     * @brief Adds or replaces in our directory file specified by the filename
     * @param filename Name of the file to add/ replace
     * @param file The actual file to add/ replace
     */
    CDirectory& Change(string filename, const CEntity& entity) {
        const CFile* f = dynamic_cast<const CFile*>(&entity);
        const CLink* l = dynamic_cast<const CLink*>(&entity);
        const CDirectory* d = dynamic_cast<const CDirectory*>(&entity);

        auto it = find_if(data.begin(), data.end(), [&filename](const pair<string, CEntity*>& p) { return p.first == filename; });

        if (f) {
            // It's a file
            CFile* newFile = new CFile(*f);
            newFile->m_Name = filename;
            if (it == data.end()) {
                // Insert the new file
                auto p = make_pair(filename, newFile);
                data.insert(p);
            } else {
                // Otherwise just change the pointer to the new file
                it->second = newFile;
            }
            return *this;
        }

        if (l) {
            // It's a link
            CLink* newLink = new CLink(*l);
            newLink->m_Name = filename;

            if (it == data.end()) {
                // Insert the new link
                auto p = make_pair(filename, newLink);
                data.insert(p);
            } else {
                // Otherwise just change the pointer to the new link
                it->second = newLink;
            }
            return *this;
        }

        if (d) {
            // It's a directory
            CDirectory* newDir = new CDirectory(*d);
            newDir->m_Name = filename;

            if (it == data.end()) {
                // Insert the new directory
                auto p = make_pair(filename, newDir);
                data.insert(p);
            } else {
                // Otherwise just change the pointer to the new directory
                it->second = newDir;
            }
            return *this;
        }

        return *this;
    }

    /**
     * @brief Finds an entity by name.
     * Returns it by reference to prevent "cutting" while converting to CEntity
     * @param filename Name of the file
     * @return CEntity& 
     */
    CEntity& Get(string filename) {
        // Search files
        auto it = find_if(data.begin(), data.end(), [&filename](const pair<string, CEntity*>& p) { return p.first == filename; });
        if (it != data.end()) {
            return *(it->second);
        }

        throw std::out_of_range("Resource not found.");
    }

    /**
     * @brief Finds an entity by name.
     * Returns it by reference to prevent "cutting" while converting to CEntity
     * @param filename Name of the file
     * @return CEntity& which is constant
     */
    const CEntity& Get(string filename) const {
        // Search files
        auto it = find_if(data.cbegin(), data.cend(), [&filename](const pair<string, CEntity*>& p) { return p.first == filename; });
        if (it != data.cend()) {
            return *(it->second);
        }

        throw std::out_of_range("Resource not found.");
    }

    // operator<<
    friend ostream& operator<<(ostream& os, const CDirectory& dir) {
        for (auto const& it : dir.data) {
            const CFile* f = dynamic_cast<const CFile*>(it.second);
            const CLink* l = dynamic_cast<const CLink*>(it.second);
            const CDirectory* d = dynamic_cast<const CDirectory*>(it.second);
            if (f) {
                os << *f << endl;
            }
            if (l) {
                os << *l << endl;
            }
            if (d) {
                os << d->Size() << "\t" << d->m_Name << "/" << endl;
            }
        }

        return os;
    }
};

#ifndef __PROGTEST__

// int main() {
//     CFile f = CFile("hash", 10);
//     cout << "File named: " << f.m_Hash << " with a size: " << f.Size() << endl;
//     return 0;
// }

int main() {
    CDirectory root;
    stringstream sout;

    root.Change("file.txt", CFile("jhwadkhawkdhajwdhawhdaw=", 1623))
        .Change("file.ln", CLink("").Change("file.txt"))
        .Change("folder", CDirectory()
                              .Change("fileA.txt", CFile("", 0).Change("skjdajdakljdljadkjwaljdlaw=", 1713))
                              .Change("fileB.txt", CFile("kadwjkwajdwhoiwhduqwdqwuhd=", 71313))
                              .Change("fileC.txt", CFile("aihdqhdqudqdiuwqhdquwdqhdi=", 8193)));

    sout.str("");
    cout << root << endl;
    cout << root.Size() << endl;
    cout << endl;
    sout << root;
    assert(sout.str() ==
           "9\tfile.ln -> file.txt\n"
           "1623\tfile.txt jhwadkhawkdhajwdhawhdaw=\n"
           "81246\tfolder/\n");
    assert(root.Size() == 82899);

    string filename = "folder";
    const CDirectory& inner = dynamic_cast<const CDirectory&>(root.Get("folder"));
    const CDirectory& inner2 = dynamic_cast<const CDirectory&>(root.Get(filename));
    CDirectory& inner3 = dynamic_cast<CDirectory&>(root.Get("folder"));
    CDirectory& inner4 = dynamic_cast<CDirectory&>(root.Get(filename));

    sout.str("");
    sout << inner;
    assert(sout.str() ==
           "1713\tfileA.txt skjdajdakljdljadkjwaljdlaw=\n"
           "71313\tfileB.txt kadwjkwajdwhoiwhduqwdqwuhd=\n"
           "8193\tfileC.txt aihdqhdqudqdiuwqhdquwdqhdi=\n");
    assert(inner.Size() == 81246);

    sout.str("");
    sout << inner2;
    assert(sout.str() ==
           "1713\tfileA.txt skjdajdakljdljadkjwaljdlaw=\n"
           "71313\tfileB.txt kadwjkwajdwhoiwhduqwdqwuhd=\n"
           "8193\tfileC.txt aihdqhdqudqdiuwqhdquwdqhdi=\n");
    assert(inner2.Size() == 81246);

    sout.str("");
    sout << inner3;
    assert(sout.str() ==
           "1713\tfileA.txt skjdajdakljdljadkjwaljdlaw=\n"
           "71313\tfileB.txt kadwjkwajdwhoiwhduqwdqwuhd=\n"
           "8193\tfileC.txt aihdqhdqudqdiuwqhdquwdqhdi=\n");
    assert(inner3.Size() == 81246);

    sout.str("");
    sout << inner4;
    assert(sout.str() ==
           "1713\tfileA.txt skjdajdakljdljadkjwaljdlaw=\n"
           "71313\tfileB.txt kadwjkwajdwhoiwhduqwdqwuhd=\n"
           "8193\tfileC.txt aihdqhdqudqdiuwqhdquwdqhdi=\n");
    assert(inner4.Size() == 81246);

    CFile& innerFile = dynamic_cast<CFile&>(root.Get("file.txt"));
    cout << innerFile.m_Hash << " size: " << innerFile.Size() << endl;
    innerFile.Change("New hash!!", 10000);
    cout << innerFile.m_Hash << " size: " << innerFile.Size() << endl;

    CLink& innerLink = dynamic_cast<CLink&>(root.Get("file.ln"));
    cout << innerLink.m_Name << " -> " << innerLink.m_Path << " size:" << innerLink.Size() << endl;
    innerLink.Change("/path/to/something");
    string p = "/path/to/something/new";
    innerLink.Change(p).Change(p);
    cout << innerLink.m_Name << " -> " << innerLink.m_Path << " size:" << innerLink.Size() << endl;

    // ================== COPY TEST ==================
    cout << "================== BEFORE COPY ==================" << endl;
    cout << root << endl;

    CDirectory root2 = root;
    root2.Change("TestFile.txt", CFile("yesyes=", 123));
    root2.Change("TestFile.ln", CLink("test/path"));

    cout << "================== ROOT1: ==================" << endl;
    cout << root << endl;

    cout << "================== ROOT2: ==================" << endl;
    cout << root2 << endl;
    return 0;
}

#endif /* __PROGTEST__ */
