#pragma once

#include "book.h"

#include <cstddef>
#include <iostream>

class BookSet {
private:
    Book* data_;
    std::size_t size_;
    std::size_t capacity_;

    void swap(BookSet& other) noexcept;
    void reserve(std::size_t new_capacity);
    std::size_t findPosition(const Book& book) const;
    void appendSorted(const Book& book);

public:
    BookSet();
    BookSet(const BookSet& other);
    BookSet(BookSet&& other) noexcept;
    BookSet& operator=(const BookSet& other);
    BookSet& operator=(BookSet&& other) noexcept;
    ~BookSet();

    bool add(const Book& book);
    bool remove(const Book& book);
    bool contains(const Book& book) const;
    bool operator()(const Book& book) const;

    std::size_t size() const;
    bool empty() const;

    BookSet& operator+=(const Book& book);
    BookSet& operator-=(const Book& book);

    BookSet operator|(const BookSet& other) const;
    BookSet operator&(const BookSet& other) const;
    BookSet operator-(const BookSet& other) const;

    bool operator==(const BookSet& other) const;
    bool operator!=(const BookSet& other) const;

    const Book& operator[](std::size_t index) const;

    friend std::ostream& operator<<(std::ostream& os, const BookSet& set);
};
