#pragma once

#include <string>
#include <iostream>
#include <mutex>

class Book {

private:
    int bookId;
    std::string name;
    std::string author;
    std::string genre;
    int year;
    bool available;

    int borrowedById;
    std::string borrowedByName;

public:
    Book(int _bookId, std::string _name, std::string _author, std::string _genre, int _year);
    ~Book();

    void DisplayInfo();
    // finish this later(update details)
    void UpdateDetails(std::string _name, std::string _author, std::string _genre, int _year);
    void UpdateId(int id);

    bool IsAvailable();
    std::string GetName();
    int GetId();

    int Borrowed(int id, std::string userName);
    void Returned();

};