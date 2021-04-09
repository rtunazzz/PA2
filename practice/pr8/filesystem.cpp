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
    /** @brief Name of the entity */
    string m_Name;
    // not needed since we hold names in the folder map anyway but this way
    // I can overload the << operators on the class' children

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

    /**
     * @brief Sends file's string representation to the stream specified
     * @param os stream to send the string to
     * @param file file to be sent
     * @return ostream& 
     */
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
    /** @brief Path of the link */
    string m_Path;

    /**
     * @brief Construct a new CLink object
     * @param path Path of the link
     */
    CLink(const string path) : m_Path(path) {
        m_Name = "";
    }

    /**
     * @brief Returns the size of the link
     * @return int 
     */
    int Size() const {
        return m_Path.size() + 1;
    }

    /**
     * @brief Changes the link's file to the new path passed in
     * @param path new path
     * @return CLink& 
     */
    CLink& Change(const string path) {
        m_Path = path;
        return *this;
    }

    /**
     * @brief Sends link's string representation to the stream specified
     * @param os stream to send the string to
     * @param link link to be sent
     * @return ostream& 
     */
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
    /** @brief Holds data about our entities */
    map<string, CEntity*> data;

    CDirectory(const CDirectory& dir) : data(dir.data) {
        auto oldData = dir.data;

        for (auto it : dir.data) {
            const CFile* f = dynamic_cast<const CFile*>(it.second);
            if (f) data.insert(make_pair(it.first, new CFile(*f)));

            const CLink* l = dynamic_cast<const CLink*>(it.second);
            if (l) data.insert(make_pair(it.first, new CLink(*l)));

            const CDirectory* d = dynamic_cast<const CDirectory*>(it.second);
            if (d) data.insert(make_pair(it.first, new CDirectory(*d)));
        }
    }

    CDirectory() {
        m_Name = "";
    }

    ~CDirectory() {
        for (auto it : data) {
            delete it.second;
        }
    }

    /**
     * @brief Returns the size of a directory.
     * Size of the directory is equal to number of files (including directories) and the number of characters in their name.
     * @return int Size of the directory
     */
    int Size() const {
        int tmpSize = 0;

        // Iterate over all the files
        for (auto it : data) {
            tmpSize += it.second->Size() + it.first.size();
        }

        return tmpSize;
    }

    /**
     * @brief Deletes a file from the directory specified by the filename.
     * @param filename Name of the file to delete.
     * @param ent Pointer to the entity to be changed (= deleted)
     * @return CDirectory& 
     */
    CDirectory& Change(string filename, CEntity* ent = nullptr) {
        if (ent) {
            return Change(filename, *ent);
        } else {
            auto it = find_if(data.begin(), data.end(), [&filename](const pair<string, CEntity*>& p) { return p.first == filename; });
            if (it != data.end()) {
                // delete the file from the map
                // delete it->second;
                data.erase(it);
                return *this;
            }
        }

        return *this;
    }

    /**
     * @brief Adds or replaces in our directory file specified by the filename
     * @param filename Name of the file to add/ replace
     * @param entity const reference to the entity to add/ replace
     */
    CDirectory& Change(string filename, const CEntity& entity) {
        const CFile* f = dynamic_cast<const CFile*>(&entity);
        const CLink* l = dynamic_cast<const CLink*>(&entity);
        const CDirectory* d = dynamic_cast<const CDirectory*>(&entity);

        auto it = find_if(data.begin(), data.end(), [&filename](const pair<string, CEntity*>& p) { return p.first == filename; });

        if (f) {
            // It's a file
            CFile* newFile = new CFile(*f);  // TODO handle deleting
            newFile->m_Name = filename;
            if (it == data.end()) {
                // Insert the new file
                auto p = make_pair(filename, newFile);
                data.insert(p);
            } else {
                // Otherwise just change the pointer to the new file
                delete it->second;
                it->second = newFile;
            }
            return *this;
        }

        if (l) {
            // It's a link
            CLink* newLink = new CLink(*l);  // TODO handle deleting
            newLink->m_Name = filename;

            if (it == data.end()) {
                // Insert the new link
                auto p = make_pair(filename, newLink);
                data.insert(p);
            } else {
                // Otherwise just change the pointer to the new link
                delete it->second;
                it->second = newLink;
            }
            return *this;
        }

        if (d) {
            // It's a directory
            CDirectory* newDir = new CDirectory(*d);  // TODO handle deleting
            newDir->m_Name = filename;

            if (it == data.end()) {
                // Insert the new directory
                auto p = make_pair(filename, newDir);
                data.insert(p);
            } else {
                // Otherwise just change the pointer to the new directory
                delete it->second;
                it->second = newDir;
            }
            return *this;
        }

        return *this;
    }

    /**
     * @brief Finds an entity by name. Throws out_of_range if not found.
     * Returns it by reference to prevent "cutting" while converting to CEntity
     * @throws std::out_of_range
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

    /**
     * @brief Sends directory's string representation to the stream specified
     * @param os stream to send the string to
     * @param dir directory to be sent
     * @return ostream& 
     */
    friend ostream& operator<<(ostream& os, const CDirectory& dir) {
        for (auto it : dir.data) {
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

    CDirectory& operator=(CDirectory tmp) {  // copy & swap
        std::swap(tmp.m_Name, m_Name);
        std::swap(tmp.data, data);
        return *this;
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
    cout << "================== [DIRECTORY] BEFORE COPY ==================" << endl;
    cout << root << endl;

    CDirectory root2 = root;
    root2.Change("TestFile.txt", CFile("yesyes=", 123));
    root2.Change("TestFile.ln", CLink("test/path"));
    root.Change("file.txt");

    cout << "================== [DIRECTORY] ROOT1: ==================" << endl;
    cout << root << endl;

    cout << "================== [DIRECTORY] ROOT2: ==================" << endl;
    cout << root2 << endl;

    CFile file1 = CFile("copytest", 111);
    cout << "================== [FILE] BEFORE COPY ==================" << endl;
    cout << file1 << endl;
    CFile file2 = file1;
    cout << "================== [FILE] COPY1 ==================" << endl;
    cout << file1 << endl;
    cout << "================== [FILE] COPY2 ==================" << endl;
    cout << file2 << endl;

    return 0;
}

#endif /* __PROGTEST__ */
