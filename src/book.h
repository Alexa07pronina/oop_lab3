#pragma once

#include <iostream>

class Book {
private:
    char* title_;
    int year_;

    void swap(Book& other) noexcept;

public:
    Book();
    Book(const char* title, int year);
    Book(const Book& other);
    Book(Book&& other) noexcept;
    Book& operator=(const Book& other);
    Book& operator=(Book&& other) noexcept;
    ~Book();

    const char* title() const;
    int year() const;

    bool operator==(const Book& other) const;
    bool operator<(const Book& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Book& book);
};
