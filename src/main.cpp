#include "book_set.h"
#include <iostream>

int main() {
    BookSet first;
    first += Book("War and Peace", 1869);
    first += Book("The Hobbit", 1937);
    first += Book("1984", 1949);

    BookSet second;
    second += Book("The Hobbit", 1937);
    second += Book("Dune", 1965);

    std::cout << "A = " << first << '\n';
    std::cout << "B = " << second << '\n';
    std::cout << "A | B = " << (first | second) << '\n';
    std::cout << "A & B = " << (first & second) << '\n';
    std::cout << "A - B = " << (first - second) << '\n';
    std::cout << "First book in A: " << first[0] << '\n';
    return 0;
}
