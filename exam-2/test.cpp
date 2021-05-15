#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <deque>
#include <algorithm>
#include <memory>
#include <functional>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

class CDate
{
  public:
    // constructor ( y, m, d )
  private:
    // todo
};
class CMan
{
  public:
    // constructor
    // GetID ()
    // operator <<
    // Military ( days )
    // set<string> ScanPedigree ()
  private:
    // todo
};

class CWoman
{
  public:
    // constructor
    // GetID ()
    // operator <<
    // set<string> ScanPedigree ()
  private:
    // todo
};

class CRegister
{
  public:
    // default constructor
    // copy constructor (if needed)
    bool               Add           ( person, father, mother );
    ?                  FindByID      ( id ) const;
    vector<?>          FindRetired   ( from, to ) const;
  private:
    // todo
};

#ifndef __PROGTEST__
template <typename T_>
static bool        vectorMatch ( const vector<T_>     & res,
                                 const vector<string> & ref )
{
  vector<string> tmp;
  for ( const auto & x : res )
  {
    ostringstream oss;
    oss << *x;
    tmp . push_back ( oss . str () );
  }
  return tmp == ref;
}
int main ( void )
{
  ostringstream oss;
  vector<CRegister> r;
  r . emplace_back ();
  assert ( r[0] . Add ( make_shared<CMan> ( 1, "Peterson George", CDate ( 1900, 1, 1 ) ),
                     nullptr, nullptr ) == true );
  assert ( r[0] . Add ( make_shared<CMan> ( 2, "Watson Paul", CDate ( 1900, 3, 12 ) ),
                     nullptr, nullptr ) == true );
  assert ( r[0] . Add ( make_shared<CMan> ( 3, "Smith Peter", CDate ( 1900, 7, 14 ) ),
                     nullptr, nullptr ) == true );
  assert ( r[0] . Add ( make_shared<CMan> ( 10, "Smith Samuel", CDate ( 1930, 11, 29 ) ),
                     r[0] . FindByID ( 3 ), nullptr ) == true );
  assert ( r[0] . Add ( make_shared<CWoman> ( 11, "Peterson Jane", CDate ( 1932, 6, 4 ) ),
                     r[0] . FindByID ( 1 ), nullptr ) == true );
  assert ( r[0] . Add ( make_shared<CWoman> ( 12, "Peterson Sue", CDate ( 1933, 10, 8 ) ),
                     r[0] . FindByID ( 1 ), nullptr ) == true );
  assert ( r[0] . Add ( make_shared<CMan> ( 13, "Pershing John", CDate ( 1929, 12, 11 ) ),
                     nullptr, nullptr ) == true );
  assert ( r[0] . Add ( make_shared<CMan> ( 14, "Pearce Joe", CDate ( 1925, 7, 16 ) ),
                     nullptr, nullptr ) == true );
  assert ( r[0] . Add ( make_shared<CMan> ( 15, "Peant Thomas", CDate ( 1937, 8, 15 ) ),
                     nullptr, nullptr ) == true );
  assert ( r[0] . FindByID ( 11 ) -> GetID () == 11 );
  oss . str ( "" );
  oss << * r[0] . FindByID ( 11 );
  assert ( oss . str () == "11: Peterson Jane, woman, born: 1932-06-04, retires: 1992-06-04" );
  assert ( r[0] . Add ( make_shared<CMan> ( 100, "Smith John", CDate ( 1961, 2, 12 ) ),
                     r[0] . FindByID ( 10 ), r[0] . FindByID ( 11 ) ) == true );
  assert ( r[0] . FindByID ( 11 ) -> GetID () == 11 );
  oss . str ( "" );
  oss << * r[0] . FindByID ( 11 );
  assert ( oss . str () == "11: Peterson Jane, woman, born: 1932-06-04, retires: 1988-06-04" );
  assert ( r[0] . Add ( make_shared<CMan> ( 101, "Smith Roger", CDate ( 1963, 4, 5 ) ),
                     r[0] . FindByID ( 10 ), r[0] . FindByID ( 11 ) ) == true );
  assert ( r[0] . FindByID ( 11 ) -> GetID () == 11 );
  oss . str ( "" );
  oss << * r[0] . FindByID ( 11 );
  assert ( oss . str () == "11: Peterson Jane, woman, born: 1932-06-04, retires: 1984-06-04" );
  assert ( r[0] . Add ( make_shared<CMan> ( 102, "Smith Daniel", CDate ( 1965, 9, 8 ) ),
                     r[0] . FindByID ( 10 ), r[0] . FindByID ( 11 ) ) == true );
  assert ( r[0] . FindByID ( 11 ) -> GetID () == 11 );
  oss . str ( "" );
  oss << * r[0] . FindByID ( 11 );
  assert ( oss . str () == "11: Peterson Jane, woman, born: 1932-06-04, retires: 1980-06-04" );
  assert ( r[0] . Add ( make_shared<CWoman> ( 103, "Smith Eve", CDate ( 1967, 1, 13 ) ),
                     r[0] . FindByID ( 10 ), r[0] . FindByID ( 11 ) ) == true );
  assert ( r[0] . FindByID ( 11 ) -> GetID () == 11 );
  oss . str ( "" );
  oss << * r[0] . FindByID ( 11 );
  assert ( oss . str () == "11: Peterson Jane, woman, born: 1932-06-04, retires: 1976-06-04" );
  assert ( r[0] . Add ( make_shared<CWoman> ( 103, "Smith Jane", CDate ( 1968, 6, 22 ) ),
                     r[0] . FindByID ( 10 ), r[0] . FindByID ( 11 ) ) == false );
  assert ( r[0] . FindByID ( 3 ) -> GetID () == 3 );
  oss . str ( "" );
  oss << * r[0] . FindByID ( 3 );
  assert ( oss . str () == "3: Smith Peter, man, born: 1900-07-14, retires: 1965-07-14" );
  assert ( r[0] . FindByID ( 10 ) -> GetID () == 10 );
  oss . str ( "" );
  oss << * r[0] . FindByID ( 10 );
  assert ( oss . str () == "10: Smith Samuel, man, born: 1930-11-29, retires: 1995-11-29" );
  dynamic_cast<CMan &> ( *r[0] . FindByID ( 10 ) ) . Military ( 380 );
  assert ( r[0] . FindByID ( 3 ) -> GetID () == 3 );
  oss . str ( "" );
  oss << * r[0] . FindByID ( 3 );
  assert ( oss . str () == "3: Smith Peter, man, born: 1900-07-14, retires: 1965-07-04" );
  assert ( r[0] . FindByID ( 10 ) -> GetID () == 10 );
  oss . str ( "" );
  oss << * r[0] . FindByID ( 10 );
  assert ( oss . str () == "10: Smith Samuel, man, born: 1930-11-29, retires: 1991-09-30" );
  dynamic_cast<CMan &> ( *r[0] . FindByID ( 10 ) ) . Military ( 120 );
  assert ( r[0] . FindByID ( 10 ) -> GetID () == 10 );
  oss . str ( "" );
  oss << * r[0] . FindByID ( 10 );
  assert ( oss . str () == "10: Smith Samuel, man, born: 1930-11-29, retires: 1990-10-05" );
  dynamic_cast<CMan &> ( *r[0] . FindByID ( 10 ) ) . Military ( 57 );
  assert ( r[0] . FindByID ( 10 ) -> GetID () == 10 );
  oss . str ( "" );
  oss << * r[0] . FindByID ( 10 );
  assert ( oss . str () == "10: Smith Samuel, man, born: 1930-11-29, retires: 1990-06-13" );
  dynamic_cast<CMan &> ( *r[0] . FindByID ( 10 ) ) . Military ( 237 );
  assert ( r[0] . FindByID ( 10 ) -> GetID () == 10 );
  oss . str ( "" );
  oss << * r[0] . FindByID ( 10 );
  assert ( oss . str () == "10: Smith Samuel, man, born: 1930-11-29, retires: 1988-12-26" );
  dynamic_cast<CMan &> ( *r[0] . FindByID ( 10 ) ) . Military ( 380 );
  assert ( r[0] . FindByID ( 10 ) -> GetID () == 10 );
  oss . str ( "" );
  oss << * r[0] . FindByID ( 10 );
  assert ( oss . str () == "10: Smith Samuel, man, born: 1930-11-29, retires: 1986-11-03" );
  dynamic_cast<CMan &> ( *r[0] . FindByID ( 10 ) ) . Military ( 1380 );
  assert ( r[0] . FindByID ( 3 ) -> GetID () == 3 );
  oss . str ( "" );
  oss << * r[0] . FindByID ( 3 );
  assert ( oss . str () == "3: Smith Peter, man, born: 1900-07-14, retires: 1965-07-04" );
  assert ( r[0] . FindByID ( 10 ) -> GetID () == 10 );
  oss . str ( "" );
  oss << * r[0] . FindByID ( 10 );
  assert ( oss . str () == "10: Smith Samuel, man, born: 1930-11-29, retires: 1975-11-29" );
  assert ( vectorMatch ( r[0] . FindRetired( CDate ( 1960, 1, 1 ), CDate ( 2000, 1, 1 ) ), vector<string>
           {
             "1: Peterson George, man, born: 1900-01-01, retires: 1965-01-01",
             "2: Watson Paul, man, born: 1900-03-12, retires: 1965-03-12",
             "3: Smith Peter, man, born: 1900-07-14, retires: 1965-07-04",
             "10: Smith Samuel, man, born: 1930-11-29, retires: 1975-11-29",
             "11: Peterson Jane, woman, born: 1932-06-04, retires: 1976-06-04",
             "12: Peterson Sue, woman, born: 1933-10-08, retires: 1993-10-08",
             "13: Pershing John, man, born: 1929-12-11, retires: 1994-12-11",
             "14: Pearce Joe, man, born: 1925-07-16, retires: 1990-07-16"
           } ) );
  assert ( vectorMatch ( r[0] . FindRetired( CDate ( 1975, 11, 29 ), CDate ( 1976, 6, 4 ) ), vector<string>
           {
             "10: Smith Samuel, man, born: 1930-11-29, retires: 1975-11-29",
             "11: Peterson Jane, woman, born: 1932-06-04, retires: 1976-06-04"
           } ) );
  assert ( r[0] . FindByID ( 103 ) -> GetID () == 103 );
  oss . str ( "" );
  oss << * r[0] . FindByID ( 103 );
  assert ( oss . str () == "103: Smith Eve, woman, born: 1967-01-13, retires: 2027-01-13" );
  assert ( r[0] . FindByID ( 103 ) -> ScanPedigree () == ( set<string>
           {
             "Peterson George",
             "Peterson Jane",
             "Smith Peter",
             "Smith Samuel"
           } ) );
  assert ( r[0] . FindByID ( 2 ) -> GetID () == 2 );
  oss . str ( "" );
  oss << * r[0] . FindByID ( 2 );
  assert ( oss . str () == "2: Watson Paul, man, born: 1900-03-12, retires: 1965-03-12" );
  assert ( r[0] . FindByID ( 2 ) -> ScanPedigree () == ( set<string>
           {
           } ) );
  r . emplace_back ( r[0] );
  assert ( r[0] . FindByID ( 13 ) -> GetID () == 13 );
  oss . str ( "" );
  oss << * r[0] . FindByID ( 13 );
  assert ( oss . str () == "13: Pershing John, man, born: 1929-12-11, retires: 1994-12-11" );
  assert ( r[1] . FindByID ( 13 ) -> GetID () == 13 );
  oss . str ( "" );
  oss << * r[1] . FindByID ( 13 );
  assert ( oss . str () == "13: Pershing John, man, born: 1929-12-11, retires: 1994-12-11" );
  dynamic_cast<CMan &> ( *r[1] . FindByID ( 13 ) ) . Military ( 123 );
  assert ( r[0] . FindByID ( 13 ) -> GetID () == 13 );
  oss . str ( "" );
  oss << * r[0] . FindByID ( 13 );
  assert ( oss . str () == "13: Pershing John, man, born: 1929-12-11, retires: 1994-12-11" );
  assert ( r[1] . FindByID ( 13 ) -> GetID () == 13 );
  oss . str ( "" );
  oss << * r[1] . FindByID ( 13 );
  assert ( oss . str () == "13: Pershing John, man, born: 1929-12-11, retires: 1993-08-06" );
  assert ( r[0] . FindByID ( 11 ) -> GetID () == 11 );
  oss . str ( "" );
  oss << * r[0] . FindByID ( 11 );
  assert ( oss . str () == "11: Peterson Jane, woman, born: 1932-06-04, retires: 1976-06-04" );
  assert ( r[1] . FindByID ( 11 ) -> GetID () == 11 );
  oss . str ( "" );
  oss << * r[1] . FindByID ( 11 );
  assert ( oss . str () == "11: Peterson Jane, woman, born: 1932-06-04, retires: 1976-06-04" );
  assert ( r[0] . Add ( make_shared<CWoman> ( 104, "Smith Jane", CDate ( 1968, 6, 22 ) ),
                     r[0] . FindByID ( 10 ), r[0] . FindByID ( 11 ) ) == true );
  assert ( r[0] . FindByID ( 11 ) -> GetID () == 11 );
  oss . str ( "" );
  oss << * r[0] . FindByID ( 11 );
  assert ( oss . str () == "11: Peterson Jane, woman, born: 1932-06-04, retires: 1972-06-04" );
  assert ( r[1] . FindByID ( 11 ) -> GetID () == 11 );
  oss . str ( "" );
  oss << * r[1] . FindByID ( 11 );
  assert ( oss . str () == "11: Peterson Jane, woman, born: 1932-06-04, retires: 1976-06-04" );
  return 0;
}
#endif /* __PROGTEST__ */
