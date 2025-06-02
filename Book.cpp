#include "Book.h"

Book::Book(int _bookId, std::string _name, std::string _author, std::string _genre, int _year) {
    bookId = _bookId;
    name = _name;
    author = _author;
    genre = _genre;
    year = _year;

    available = true;
    borrowedByName = "No one";
    borrowedById = 0;
}

Book::~Book() {}

void Book::DisplayInfo() {
    std::cout << "[";
    std::cout << "ID: " << bookId << "| ";
    std::cout << "Title: " << name << "| ";
    std::cout << "Author: " << author << "| ";
    std::cout << "Genre: " << genre << "| ";
    std::cout << "Year: " << year << "| ";
    std::cout << "Borrowed by: " << borrowedByName;
    std::cout << "]" << std::endl;

}

// finish this later
void Book::UpdateDetails(std::string _name, std::string _author, std::string _genre, int _year) {
    name = _name;
    author = _author;
    genre = _genre;
    year = _year;
}

void Book::UpdateId(int id) {
    bookId = id;
}

bool Book::IsAvailable() {
    return available;
}

std::string Book::GetName() {
    return name;
}

int Book::GetId() {
    return bookId;
}

int Book::Borrowed(int id, std::string userName) {
    borrowedById = id;
    borrowedByName = userName;
    available = false;

    return bookId;
}

void Book::Returned() {
    borrowedById = 0;
    borrowedByName = "No one"; // segmentation fault
    available = true;
}