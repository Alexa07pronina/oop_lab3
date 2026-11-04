#include "book.h"

#include <cstring>
#include <utility>

Book::Book() : Book("", 0) {}

Book::Book(const char* title, int year) : title_(nullptr), year_(year) {
    if (title == nullptr) {
        title = "";
    }
    title_ = new char[std::strlen(title) + 1];
    std::strcpy(title_, title);
}

Book::Book(const Book& other) : Book(other.title(), other.year_) {}

Book::Book(Book&& other) noexcept : title_(other.title_), year_(other.year_) {
    other.title_ = nullptr;
    other.year_ = 0;
}

Book& Book::operator=(const Book& other) {
    if (this != &other) {
        Book temp(other);
        swap(temp);
    }
    return *this;
}

Book& Book::operator=(Book&& other) noexcept {
    if (this != &other) {
        delete[] title_;
        title_ = other.title_;
        year_ = other.year_;
        other.title_ = nullptr;
        other.year_ = 0;
    }
    return *this;
}

Book::~Book() {
    delete[] title_;
}

void Book::swap(Book& other) noexcept {
    std::swap(title_, other.title_);
    std::swap(year_, other.year_);
}

const char* Book::title() const {
    return title_ == nullptr ? "" : title_;
}

int Book::year() const {
    return year_;
}

bool Book::operator==(const Book& other) const {
    return std::strcmp(title(), other.title()) == 0 && year_ == other.year_;
}

bool Book::operator<(const Book& other) const {
    int comparison = std::strcmp(title(), other.title());
    if (comparison != 0) {
        return comparison < 0;
    }
    return year_ < other.year_;
}

std::ostream& operator<<(std::ostream& os, const Book& book) {
    os << "(" << book.title() << ", " << book.year_ << ")";
    return os;
}
