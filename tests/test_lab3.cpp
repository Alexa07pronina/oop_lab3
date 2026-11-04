#include <gtest/gtest.h>

#include "book_set.h"

#include <cstdio>
#include <sstream>
#include <stdexcept>
#include <utility>

TEST(BookTest, DefaultConstructorAndComparison) {
    Book empty;
    EXPECT_STREQ(empty.title(), "");
    EXPECT_EQ(empty.year(), 0);

    Book a("Alice", 2000);
    Book b("Alice", 2000);
    Book c("Alice", 2001);
    Book d("Bob", 1990);

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a < c);
    EXPECT_TRUE(c < d);
}

TEST(BookTest, CopiesOwnTitle) {
    char name[] = "Original";
    Book a(name, 2020);
    name[0] = 'X';
    EXPECT_STREQ(a.title(), "Original");

    Book b(a);
    EXPECT_TRUE(a == b);
    EXPECT_NE(a.title(), b.title());

    Book c("Different", 2001);
    c = a;
    EXPECT_TRUE(c == a);
    EXPECT_NE(c.title(), a.title());

    c = c;
    EXPECT_STREQ(c.title(), "Original");
}

TEST(BookTest, MoveAndSelfMove) {
    Book a("Move", 2021);
    Book b(std::move(a));
    EXPECT_STREQ(b.title(), "Move");
    EXPECT_STREQ(a.title(), "");

    Book c;
    c = std::move(b);
    EXPECT_STREQ(c.title(), "Move");
    EXPECT_STREQ(b.title(), "");

    Book& same_book = c;
    c = std::move(same_book);
    EXPECT_STREQ(c.title(), "Move");
}

TEST(BookTest, StreamOutput) {
    std::ostringstream out;
    out << Book("ABC", 2005);
    EXPECT_EQ(out.str(), "(ABC, 2005)");
}

TEST(BookSetTest, EmptySet) {
    BookSet books;
    EXPECT_EQ(books.size(), 0u);
    EXPECT_TRUE(books.empty());
    EXPECT_FALSE(books.contains(Book("A", 2000)));
    EXPECT_FALSE(books(Book("A", 2000)));
}

TEST(BookSetTest, AddSortedAndNoDuplicates) {
    BookSet books;
    EXPECT_TRUE(books.add(Book("Zoo", 2000)));
    EXPECT_TRUE(books.add(Book("Alpha", 2020)));
    EXPECT_TRUE(books.add(Book("Alpha", 2010)));
    EXPECT_FALSE(books.add(Book("Alpha", 2010)));
    EXPECT_EQ(books.size(), 3u);
    EXPECT_STREQ(books[0].title(), "Alpha");
    EXPECT_EQ(books[0].year(), 2010);
    EXPECT_EQ(books[1].year(), 2020);
    EXPECT_STREQ(books[2].title(), "Zoo");
}

TEST(BookSetTest, RemoveShiftsElements) {
    BookSet books;
    books += Book("A", 1);
    books += Book("B", 2);
    books += Book("C", 3);

    EXPECT_TRUE(books.remove(Book("B", 2)));
    EXPECT_FALSE(books.remove(Book("B", 2)));
    EXPECT_EQ(books.size(), 2u);
    EXPECT_STREQ(books[1].title(), "C");
    EXPECT_TRUE(books.remove(Book("A", 1)));
    EXPECT_TRUE(books.remove(Book("C", 3)));
    EXPECT_TRUE(books.empty());
}

TEST(BookSetTest, ContainsAndCallOperator) {
    BookSet books;
    books.add(Book("A", 2020));
    EXPECT_TRUE(books.contains(Book("A", 2020)));
    EXPECT_TRUE(books(Book("A", 2020)));
    EXPECT_FALSE(books.contains(Book("A", 2021)));
    EXPECT_FALSE(books(Book("Z", 2020)));
}

TEST(BookSetTest, PlusAndMinusAssignment) {
    BookSet books;
    (books += Book("A", 1)) += Book("B", 2);
    EXPECT_EQ(books.size(), 2u);
    (books -= Book("A", 1)) -= Book("C", 3);
    EXPECT_EQ(books.size(), 1u);
    EXPECT_TRUE(books.contains(Book("B", 2)));
}

TEST(BookSetTest, CopyConstructorAndAssignmentIndependent) {
    BookSet original;
    original += Book("A", 1);
    original += Book("B", 2);

    BookSet copied(original);
    copied.remove(Book("A", 1));
    EXPECT_EQ(original.size(), 2u);
    EXPECT_EQ(copied.size(), 1u);

    BookSet assigned;
    assigned = original;
    EXPECT_TRUE(assigned == original);
    EXPECT_NE(assigned[0].title(), original[0].title());
    assigned.add(Book("C", 3));
    EXPECT_FALSE(original.contains(Book("C", 3)));

    assigned = assigned;
    EXPECT_EQ(assigned.size(), 3u);
}

TEST(BookSetTest, MoveConstructorAndAssignment) {
    BookSet a;
    a += Book("A", 1);
    BookSet b(std::move(a));
    EXPECT_TRUE(a.empty());
    EXPECT_TRUE(b.contains(Book("A", 1)));
    EXPECT_TRUE(a.add(Book("New", 2020))); 

    BookSet c;
    c += Book("Old", 0);
    c = std::move(b);
    EXPECT_TRUE(b.empty());
    EXPECT_TRUE(c.contains(Book("A", 1)));
    EXPECT_FALSE(c.contains(Book("Old", 0)));

    BookSet& same_set = c;
    c = std::move(same_set);
    EXPECT_EQ(c.size(), 1u);
}

TEST(BookSetTest, UnionIntersectionDifference) {
    BookSet a;
    a += Book("A", 1);
    a += Book("C", 3);
    a += Book("E", 5);

    BookSet b;
    b += Book("B", 2);
    b += Book("C", 3);
    b += Book("F", 6);

    BookSet un = a | b;
    BookSet in = a & b;
    BookSet diff = a - b;

    EXPECT_EQ(un.size(), 5u);
    EXPECT_TRUE(un.contains(Book("F", 6)));
    EXPECT_EQ(in.size(), 1u);
    EXPECT_TRUE(in.contains(Book("C", 3)));
    EXPECT_EQ(diff.size(), 2u);
    EXPECT_TRUE(diff.contains(Book("A", 1)));
    EXPECT_TRUE(diff.contains(Book("E", 5)));

    EXPECT_EQ(a.size(), 3u);
    EXPECT_EQ(b.size(), 3u);
}

TEST(BookSetTest, OperationsWithEmptyAndSameSet) {
    BookSet a;
    a.add(Book("A", 1));
    BookSet empty;

    EXPECT_TRUE((a | empty) == a);
    EXPECT_TRUE((a & empty).empty());
    EXPECT_TRUE((a - empty) == a);
    EXPECT_TRUE((a - a).empty());
    EXPECT_TRUE((a | a) == a);
    EXPECT_TRUE((a & a) == a);
}

TEST(BookSetTest, Equality) {
    BookSet a;
    a += Book("B", 2);
    a += Book("A", 1);

    BookSet b;
    b += Book("A", 1);
    b += Book("B", 2);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);

    b += Book("C", 3);
    EXPECT_TRUE(a != b);
}

TEST(BookSetTest, IndexIsConstAndBoundsChecked) {

    BookSet books;
    EXPECT_THROW(books[0], std::out_of_range);
    books += Book("A", 1);
    EXPECT_STREQ(books[0].title(), "A");
    EXPECT_THROW(books[1], std::out_of_range);
}

TEST(BookSetTest, StreamOutputSorted) {
    BookSet books;
    books += Book("B", 2);
    books += Book("A", 1);
    std::ostringstream out;
    out << books;
    EXPECT_EQ(out.str(), "{ (A, 1), (B, 2) }");

    std::ostringstream empty_out;
    empty_out << BookSet();
    EXPECT_EQ(empty_out.str(), "{}");
}

TEST(BookSetTest, Stress1000Books) {
    BookSet books;
    for (int i = 999; i >= 0; --i) {
        char title[32];
        std::snprintf(title, sizeof(title), "Book_%04d", i);
        EXPECT_TRUE(books.add(Book(title, 2000)));
    }
    EXPECT_EQ(books.size(), 1000u);

    for (int i = 0; i < 1000; ++i) {
        char title[32];
        std::snprintf(title, sizeof(title), "Book_%04d", i);
        EXPECT_FALSE(books.add(Book(title, 2000)));
        EXPECT_STREQ(books[i].title(), title);
        EXPECT_TRUE(books.contains(Book(title, 2000)));
    }
    EXPECT_EQ(books.size(), 1000u);
}

TEST(BookSetTest, AliasingAndManyRemovals) {
    BookSet books;
    books += Book("A", 1);
    books += Book("B", 2);
    books += Book("C", 3);
    EXPECT_FALSE(books.add(books[0]));
    EXPECT_TRUE(books.remove(books[1]));
    EXPECT_STREQ(books[1].title(), "C");

    for (int i = 0; i < 100; ++i) {
        books += Book("D", i);
    }
    for (int i = 0; i < 100; ++i) {
        EXPECT_TRUE(books.remove(Book("D", i)));
    }
    EXPECT_EQ(books.size(), 2u);
}
