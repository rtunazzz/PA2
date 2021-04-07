
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

    virtual ~CEntity(){};
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
    CFile(const string& hash, int filesize) : m_Hash(hash), m_Filesize(filesize) {
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
    CFile& Change(const string& hash, int filesize) {
        m_Hash = hash;
        m_Filesize = filesize;
        return *this;
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
    CLink(const string& path) : m_Path(path) {
        m_Name = "";
    }
    // Size
    int Size() const {
        return m_Path.size() + 1;
    }

    // Change
    CLink& Change(const string& path) {
        m_Path = path;
        return *this;
    }
};

/**
 * @brief Represents a directory.
 */
class CDirectory : public CEntity {
   private:
    struct Buffer {
        /** @brief Vector of all directories in the directory */
        vector<CDirectory> directories;

        /** @brief Vector of all files in the directory */
        vector<CFile> files;

        /** @brief Vector of all links in the directory */
        vector<CLink> links;

        int reference_count = 0;
    };

   public:
    Buffer* m_Buffer;

    /** @brief Construct a new CDirectory object */
    CDirectory() {
        m_Name = "";
        m_Buffer = new Buffer();
        m_Buffer->reference_count = 1;
    }

    CDirectory(const CDirectory& oldDir) : m_Buffer(oldDir.m_Buffer) {
        m_Name = oldDir.m_Name;
        m_Buffer->reference_count++;
    }

    ~CDirectory() {
        if (--m_Buffer->reference_count < 1) {
            delete m_Buffer;
        }
    }

    /**
     * @brief Returns the size of a folder.
     * Size of the folder is equal to number of files (including directories) and the number of characters in their name.
     * @return int Size of the folder
     */
    int Size() const {
        int tmpSize = 0;

        // Iterate over the directories in this dirrectory and get their size
        for (auto const& it : m_Buffer->directories) {
            tmpSize += it.Size() + it.m_Name.size();
        }

        // Then iterate over files and add their size & the length of their name (hash)
        for (auto const& it : m_Buffer->files) {
            tmpSize += it.Size() + it.m_Name.size();
        }

        // Then iterate over links and add their size
        for (auto const& it : m_Buffer->links) {
            tmpSize += it.Size() + it.m_Name.size();
        }
        return tmpSize;
    }

    /**
     * @brief Adds or replaces in our directory file specified by the filename
     * @param filename Name of the file to add/ replace
     * @param file The actual file to add/ replace
     */
    CDirectory& Change(string filename, CFile file) {
        file.m_Name = filename;
        // Check if file exists
        auto it = find_if(m_Buffer->files.begin(), m_Buffer->files.end(), [&filename](const CFile& f) { return f.m_Name == filename; });
        if (it == m_Buffer->files.end()) {
            // file was not found = add it
            m_Buffer->files.push_back(file);
            return *this;
        } else {
            // otherwise replace the file with the new one
            (*it) = file;
        }
        return *this;
    }

    /**
     * @brief Adds or replaces in our directory link specified by the linkname
     * @param linkname Name of the link to add/ replace
     * @param link The actual link to add/ replace
     */
    CDirectory& Change(string linkname, CLink link) {
        link.m_Name = linkname;

        // Check if link exists
        auto it = find_if(m_Buffer->links.begin(), m_Buffer->links.end(), [&linkname](const CLink& l) { return l.m_Name == linkname; });
        if (it == m_Buffer->links.end()) {
            // link was not found = add it
            m_Buffer->links.push_back(link);
            return *this;
        } else {
            // otherwise replace the link with the new one
            (*it) = link;
        }
        return *this;
    }

    /**
     * @brief Adds or replaces a directory specified by the dirname in this directory
     * @param dirname Name of the directory to add/ replace
     * @param link The actual directory to add/ replace
     */
    CDirectory& Change(string dirname, CDirectory dir) {
        dir.m_Name = dirname;
        // Check if dir exists
        auto it = find_if(m_Buffer->directories.begin(), m_Buffer->directories.end(), [&dirname](const CDirectory& d) { return d.m_Name == dirname; });
        if (it == m_Buffer->directories.end()) {
            // link was not found = add it
            m_Buffer->directories.push_back(dir);
            return *this;
        } else {
            // otherwise replace the link with the new one
            (*it) = dir;
        }

        return *this;
    }

    /**
     * @brief Deletes a file from the directory specified by the filename.
     * @param filename Name of the file to delete.
     */
    CDirectory& Change(string& filename) {
        // Check if file exists via the find_if func and a lambda function that matches files with the name
        auto it = find_if(m_Buffer->files.begin(), m_Buffer->files.end(), [&filename](const CFile& file) { return file.m_Name == filename; });
        if (it == m_Buffer->files.end()) {
            // file was not found = we dont need to delete it
            return *this;
        } else {
            // otherwise delete the file form the vector
            m_Buffer->files.erase(it);
        }
        return *this;
    }

    // Get
    CEntity& Get(string filename) {
        return searchAll(filename);
    }

    const CEntity& Get(string filename) const {
        return searchAllConst(filename);
    }

    // operator<<
    friend ostream& operator<<(ostream& os, const CDirectory& dir) {
        for (auto const& it : dir.m_Buffer->links) {
            os << it.Size() << "\t" << it.m_Name << " -> " << it.m_Path << endl;
        }

        for (auto const& it : dir.m_Buffer->files) {
            os << it.Size() << "\t" << it.m_Name << " " << it.m_Hash << endl;
        }

        for (auto const& it : dir.m_Buffer->directories) {
            os << it.Size() << "\t" << it.m_Name << "/" << endl;
        }

        return os;
    }

    CDirectory& operator=(CDirectory tmp) {  // Copy & Swap z konzultace
        std::swap(tmp.m_Buffer, m_Buffer);
        return *this;
    }

   private:
    const CEntity& searchAllConst(string& name) const {
        // Search files
        auto const fileIt = find_if(m_Buffer->files.cbegin(), m_Buffer->files.cend(), [&name](const CFile& file) { return file.m_Name == name; });
        if (fileIt != m_Buffer->files.cend()) {
            return *fileIt;
        }
        // Search links
        auto const linkIt = find_if(m_Buffer->links.cbegin(), m_Buffer->links.cend(), [&name](const CLink& link) { return link.m_Name == name; });
        if (linkIt != m_Buffer->links.cend()) {
            return *linkIt;
        }

        // Search links
        auto const dirIt = find_if(m_Buffer->directories.cbegin(), m_Buffer->directories.cend(), [&name](const CDirectory& dir) { return dir.m_Name == name; });
        if (dirIt != m_Buffer->directories.cend()) {
            return *dirIt;
        }
        throw std::out_of_range("Resource not found.");
    }

    CEntity& searchAll(string& name) const {
        // Search files
        auto fileIt = find_if(m_Buffer->files.begin(), m_Buffer->files.end(), [&name](const CFile& file) { return file.m_Name == name; });
        if (fileIt != m_Buffer->files.end()) {
            return *fileIt;
        }
        // Search links
        auto linkIt = find_if(m_Buffer->links.begin(), m_Buffer->links.end(), [&name](const CLink& link) { return link.m_Name == name; });
        if (linkIt != m_Buffer->links.end()) {
            return *linkIt;
        }

        // Search links
        auto dirIt = find_if(m_Buffer->directories.begin(), m_Buffer->directories.end(), [&name](const CDirectory& dir) { return dir.m_Name == name; });
        if (dirIt != m_Buffer->directories.end()) {
            return *dirIt;
        }
        throw std::out_of_range("Resource not found.");
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
    // cout << root << endl;
    // cout << root.Size() << endl;
    // cout << endl;
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
    innerFile.Change("New hash mf", 10000);
    cout << innerFile.m_Hash << " size: " << innerFile.Size() << endl;
    return 0;
}

#endif /* __PROGTEST__ */
