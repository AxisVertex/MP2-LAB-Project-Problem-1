#pragma once

#include <vector>
#include <iostream>
#include <limits>
#include <thread>
#include <chrono>
#include <random>
#include <mutex>

#include "Book.cpp"
#include "User.cpp"

class Library
{
private:
    
    std::vector<User> users;
    std::vector<Book> books;
    std::vector<std::thread> simulatedUsers;

    int userIdCount;
    int bookIdCount;

    User* mainLoggedInUser;
    bool mainUserIsLoggedIn;

    bool continueRunning;

    std::mutex createSimUserMutex;
    std::mutex simUserBorrowBookMutex;
    std::mutex simUserReturnBookMutex;

public:
    Library();
    ~Library();

    void MainLoop();

    void RegisterUser();
    void UserLogIn();
    void UserLogOut();

    void DisplayAllBooks();
    void InitializeBooks();

    void BorrowBook();
    void ReturnBook();

    // admin only
    void DisplayAllUsers();
    void AddBook();
    void RemoveBook();
    void UpdateBook();

    // testing purposes
    void SeeLoggedInUser();

    void MainMenu();
    void UserMenu();
    void UserAdminMenu();

    // for simulations
    void UserSimulation(std::string testName, std::string testPass);

    int RegisterSimulatedUser(std::string name, std::string pass);
    void SimulatedUserBorrowBook(int id, User& simUser);
    void SimulatedUserReturnBook(User& simUser);
};