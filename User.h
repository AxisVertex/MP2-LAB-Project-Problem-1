#pragma once

#include <string>

class User {

private:
    int userId;
    std::string name;
    std::string password;
    bool loggedIn;
    bool isAdmin;

    bool hasBorrowedBook;
    std::string borrowedBookName;
    int borrowedBookId;


public:
    User(int _userId, std::string _name, std::string _password, bool _makeAdmin);
    ~User();

    void LogIn();
    void LogOut();

    void BorrowBook(int bookId, std::string bookName);
    int ReturnBook();

    int GetId();
    std::string GetName();
    std::string GetPassword();
    bool IsLoggedIn();
    bool HasAlreadyBorrowed();
    std::string GetBorrowedBookName();
    bool IsAdmin();

};