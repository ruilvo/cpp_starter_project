#include <sqlite3.h>

#include <iostream>

int main()
{
    std::cout << "Hello World!" << std::endl;
    std::cout << "SQLite3 version " << sqlite3_libversion() << std::endl;

    return 0; // Success
}
