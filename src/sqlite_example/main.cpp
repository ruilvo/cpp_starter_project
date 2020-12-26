#include <sqlite3.h>

#include <iostream>

int main()
{
    std::cout << "Hello World!\n"
                 "SQLite3 version "
              << sqlite3_libversion() << '\n';
}
