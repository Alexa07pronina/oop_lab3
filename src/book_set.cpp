#include "book_set.h"

#include <stdexcept>
#include <utility>

BookSet::BookSet() : data_(nullptr), size_(0), capacity_(0) {}

BookSet::BookSet(const BookSet& other) : data_(nullptr), size_(0), capacity_(0) {
    if (other.capacity_ == 0) {
        return;
    }

    Book* new_data = new Book[other.capacity_];
    try {
        for (std::size_t i = 0; i < other.size_; ++i) {
            new_data[i] = other.data_[i];
        }
    } catch (...) {
        delete[] new_data;
        throw;
    }

    data_ = new_data;
    size_ = other.size_;
    capacity_ = other.capacity_;
}

BookSet::BookSet(BookSet&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

BookSet& BookSet::operator=(const BookSet& other) {
    if (this != &other) {
        BookSet temp(other);
        swap(temp);
    }
    return *this;
}

BookSet& BookSet::operator=(BookSet&& other) noexcept {
    if (this != &other) {
        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

BookSet::~BookSet() {
    delete[] data_;
}

void BookSet::swap(BookSet& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

void BookSet::reserve(std::size_t new_capacity) {
    if (new_capacity <= capacity_) {
        return;
    }

    Book* new_data = new Book[new_capacity];
    try {
        for (std::size_t i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }
    } catch (...) {
        delete[] new_data;
        throw;
    }

    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
}

std::size_t BookSet::findPosition(const Book& book) const {
    std::size_t left = 0;
    std::size_t right = size_;

    while (left < right) {
        std::size_t middle = left + (right - left) / 2;
        if (data_[middle] < book) {
            left = middle + 1;
        } else {
            right = middle;
        }
    }
    return left;
}

void BookSet::appendSorted(const Book& book) {
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    data_[size_] = book;
    ++size_;
}

bool BookSet::add(const Book& book) {
    std::size_t position = findPosition(book);
    if (position < size_ && data_[position] == book) {
        return false;
    }
    Book new_book(book);
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }

    for (std::size_t i = size_; i > position; --i) {
        data_[i] = std::move(data_[i - 1]);
    }
    data_[position] = std::move(new_book);
    ++size_;
    return true;
}

bool BookSet::remove(const Book& book) {
    std::size_t position = findPosition(book);
    if (position == size_ || !(data_[position] == book)) {
        return false;
    }

    for (std::size_t i = position; i + 1 < size_; ++i) {
        data_[i] = std::move(data_[i + 1]);
    }
    --size_;
    return true;
}

bool BookSet::contains(const Book& book) const {
    std::size_t position = findPosition(book);
    return position < size_ && data_[position] == book;
}

bool BookSet::operator()(const Book& book) const {
    return contains(book);
}

std::size_t BookSet::size() const {
    return size_;
}

bool BookSet::empty() const {
    return size_ == 0;
}

BookSet& BookSet::operator+=(const Book& book) {
    add(book);
    return *this;
}

BookSet& BookSet::operator-=(const Book& book) {
    remove(book);
    return *this;
}

BookSet BookSet::operator|(const BookSet& other) const {
    BookSet result;
    std::size_t i = 0;
    std::size_t j = 0;

    while (i < size_ && j < other.size_) {
        if (data_[i] < other.data_[j]) {
            result.appendSorted(data_[i++]);
        } else if (other.data_[j] < data_[i]) {
            result.appendSorted(other.data_[j++]);
        } else {
            result.appendSorted(data_[i]);
            ++i;
            ++j;
        }
    }
    while (i < size_) {
        result.appendSorted(data_[i++]);
    }
    while (j < other.size_) {
        result.appendSorted(other.data_[j++]);
    }
    return result;
}

BookSet BookSet::operator&(const BookSet& other) const {
    BookSet result;
    std::size_t i = 0;
    std::size_t j = 0;

    while (i < size_ && j < other.size_) {
        if (data_[i] < other.data_[j]) {
            ++i;
        } else if (other.data_[j] < data_[i]) {
            ++j;
        } else {
            result.appendSorted(data_[i]);
            ++i;
            ++j;
        }
    }
    return result;
}

BookSet BookSet::operator-(const BookSet& other) const {
    BookSet result;
    std::size_t i = 0;
    std::size_t j = 0;

    while (i < size_ && j < other.size_) {
        if (data_[i] < other.data_[j]) {
            result.appendSorted(data_[i++]);
        } else if (other.data_[j] < data_[i]) {
            ++j;
        } else {
            ++i;
            ++j;
        }
    }
    while (i < size_) {
        result.appendSorted(data_[i++]);
    }
    return result;
}

bool BookSet::operator==(const BookSet& other) const {
    if (size_ != other.size_) {
        return false;
    }
    for (std::size_t i = 0; i < size_; ++i) {
        if (!(data_[i] == other.data_[i])) {
            return false;
        }
    }
    return true;
}

bool BookSet::operator!=(const BookSet& other) const {
    return !(*this == other);
}

const Book& BookSet::operator[](std::size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("BookSet index out of range");
    }
    return data_[index];
}

std::ostream& operator<<(std::ostream& os, const BookSet& set) {
    os << "{";
    if (!set.empty()) {
        os << " ";
    }
    for (std::size_t i = 0; i < set.size_; ++i) {
        if (i != 0) {
            os << ", ";
        }
        os << set.data_[i];
    }
    if (!set.empty()) {
        os << " ";
    }
    os << "}";
    return os;
}
