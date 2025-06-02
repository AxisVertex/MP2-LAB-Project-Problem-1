#include "User.h"

User::User(int _userId, std::string _name, std::string _password, bool _makeAdmin=false) {
    userId = _userId;
    name = _name;
    password = _password;
    isAdmin = _makeAdmin;

    loggedIn = false;
    hasBorrowedBook = false;
    borrowedBookId = 0;
    borrowedBookName = "None";
}

User::~User() {}

void User::LogIn() {
    loggedIn = true;
}

void User::LogOut() {
    loggedIn = false;
}

void User::BorrowBook(int bookId, std::string bookName) {
    borrowedBookName = bookName;
    borrowedBookId = bookId;
    hasBorrowedBook = true;
}

int User::ReturnBook() {
    hasBorrowedBook = false;
    borrowedBookName = "None";
    return borrowedBookId;
}

int User::GetId() {
    return userId;
}

 std::string User::GetName() {
    return name;
 }

std::string User::GetPassword() {
    return password;
}

bool User::IsLoggedIn() {
    return loggedIn;
}

bool User::HasAlreadyBorrowed() {
    return hasBorrowedBook;
}

std::string User::GetBorrowedBookName() {
    return borrowedBookName;
}

bool User::IsAdmin() {
    return isAdmin;
}