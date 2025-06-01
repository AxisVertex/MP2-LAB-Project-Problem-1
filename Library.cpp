#include "Library.h"

Library::Library() {
    userIdCount = 1;
    bookIdCount = 1;

    mainLoggedInUser = nullptr;
    mainUserIsLoggedIn = false;
    continueRunning = true;

    users.reserve(11);

    InitializeBooks();

    // testing purposes
    users.push_back(User(userIdCount, "testAdmin1", "123", true));
    userIdCount += 1;

    // how many simulated users to generate
    int amountOfSimulatedUsers = 10;

    for (int simUserCount = 0; simUserCount < amountOfSimulatedUsers; simUserCount++) {
        std::string name = "Simulated User " + std::to_string((simUserCount + 1));
        std::string pass = "123";
        
        simulatedUsers.emplace_back(std::thread(&Library::UserSimulation, this, name, pass));
    }

    MainLoop();
}

Library::~Library() {}

void Library::MainLoop() {

    while (continueRunning) {
        if (mainUserIsLoggedIn == false)
            MainMenu();
        else {
            if (mainLoggedInUser->IsAdmin()) 
                UserAdminMenu();
            else
                UserMenu();
        }
    }

    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    std::cout << "Bye bye" << std::endl;
}

// testing purposes
void Library::InitializeBooks() {
    std::cout << "Books iniliatized" << std::endl;
    books.push_back(Book(bookIdCount, "Test title 1", "Unknown author", "Unknown genre", 2025));
    bookIdCount++;
    books.push_back(Book(bookIdCount, "Test title 2", "Unknown author", "Unknown genre", 2025));
    bookIdCount++;
    books.push_back(Book(bookIdCount, "Test title 3", "Unknown author", "Unknown genre", 2025));
    bookIdCount++;
}

// User methods //
void Library::RegisterUser() {
    std::lock_guard<std::mutex> lock(createSimUserMutex);

    std::cout << "[LOCK STATUS: LOCKED] Registering a new account" << std::endl;

    std::string userName;
    std::string password;
    bool registrationSuccess = true;

    while (true) {
        std::cout << "Enter username: ";
        std::getline(std::cin, userName);

        std::cout << "Enter password: ";
        std::getline(std::cin, password);
        if (users.size() > 0) {
            for (int userIndex = 0; userIndex < users.size(); userIndex++) {
                if (userName == users[userIndex].GetName()) {
                    registrationSuccess = false;
                    break;
                }
            }
        }

        if (registrationSuccess == false) {
            std::cout <<"Username already exists" << std::endl;
            registrationSuccess = true;
            continue;
        }
        else {
            break;
        }
    }

    users.push_back(User(userIdCount, userName, password));
    std::cout << "Account Registered" << std::endl;
    userIdCount++;

    std::cout << "[LOCK STATUS: UNLOCKED] Registering a new account" << std::endl;
}

void Library::UserLogIn() {
    std::lock_guard<std::mutex> lock(createSimUserMutex);

    std::cout << "[LOCK STATUS: LOCKED] user logging in" << std::endl;

    std::string userName;
    std::string password;
    bool accountFound = false;

    std::cout << "Enter username: ";
    std::getline(std::cin, userName);

    std::cout << "Enter password: ";
    std::getline(std::cin, password);

    if (users.size() > 0) {
        for (int userIndex = 0; userIndex <= users.size(); userIndex++) {
            if (users[userIndex].GetName() == userName && users[userIndex].GetPassword() == password) {
                accountFound = true;
                mainLoggedInUser = &users[userIndex];
                mainLoggedInUser->LogIn();
                mainUserIsLoggedIn = true;
                break;
            }
        }
    }

    if (accountFound == false) {
        std::cout << "Invalid username or password" << std::endl;
    }

    std::cout << "[LOCK STATUS: UNLOCKED] user logging in" << std::endl;
}

void Library::UserLogOut() {
    mainUserIsLoggedIn = false;
    mainLoggedInUser->LogOut();
    mainLoggedInUser = nullptr;
}

void Library::DisplayAllUsers() {
    std::cout << "Users registered: \n";
    if (users.size() > 0) {
        for (int userIndex = 0; userIndex <= users.size(); userIndex++) {
        std::cout << users[userIndex].GetName() << "\n";
        } 
    }
}

// methods for the books
void Library::BorrowBook() {
    std::lock_guard<std::mutex> lock(simUserBorrowBookMutex);

    std::cout << "[LOCK STATUS: LOCKED] user borrowing a book" << std::endl;

    if (mainLoggedInUser->HasAlreadyBorrowed() == false) {

        if (books.size() > 0) {
            int enteredBookId;

            std::cout << "What book would you like to borrow? [Enter the ID of the book]" << std::endl;
            DisplayAllBooks();

            while (true) {
                std::cout << ">> ";
                std::cin >> enteredBookId;
                std::cin.ignore();
                
                if (std::cin.fail()) {
                    // Clear the failbit and ignore the remaining
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                    std::cout << "Invalid input, Try again." << std::endl;
                    continue;
                }
                else if (enteredBookId > books.size() || enteredBookId < 1) { 
                    std::cin.clear();
                    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                    std::cout << "Book does not exist." << std::endl;
                    continue;
                }

                break;
            }

            if (books[enteredBookId - 1].IsAvailable()) {
                mainLoggedInUser->BorrowBook(books[enteredBookId - 1].Borrowed(mainLoggedInUser->GetId(), mainLoggedInUser->GetName()), books[enteredBookId - 1].GetName());
            }
            else {
                std::cout << "Book is already being borrowed by someone else" << std::endl;
            }

        }
        else {
            std::cout << "No books to borrow." << std::endl;
        }
    }
    else {
        std::cout << "You can only borrow one book at a time" << std::endl;
    }

    std::cout << "[LOCK STATUS: UNLOCKED] user borrowing a book" << std::endl;

}

void Library::ReturnBook() {
    std::lock_guard<std::mutex> lock(simUserReturnBookMutex);

    std::cout << "[LOCK STATUS: LOCKED] user returning a book" << std::endl;

    if (mainLoggedInUser->HasAlreadyBorrowed()) {
        books[mainLoggedInUser->ReturnBook() - 1].Returned();
        std::cout << mainLoggedInUser->GetBorrowedBookName() << " has been returned" << std::endl;
    }
    else {
        std::cout << "You have not borrowed any books" << std::endl;
    }

    std::cout << "[LOCK STATUS: UNLOCKED] user returning a book" << std::endl;

}

void Library::DisplayAllBooks() {
    std::cout << "List of books: " << std::endl;
    if (books.size() > 0) {
        for (int bookIndex = 0; bookIndex < books.size(); bookIndex++)
            books[bookIndex].DisplayInfo();
    }
    else {
        std::cout << "There are no current books available" << std::endl;
    }
}

void Library::AddBook() {

    std::lock_guard<std::mutex> lock(simUserBorrowBookMutex);
    std::cout << "[LOCK STATUS: LOCKED] Admin adding new a book" << std::endl;

    std::string bookName;
    std::string bookAuthor;
    std::string bookGenre;
    int yearPublished;

    std::cout << "Adding new book: " << std::endl;

    while (true) {
        
        std::cout << "Book name: ";
        std::getline(std::cin, bookName);

        std::cout << "Enter author name: ";
        std::getline(std::cin, bookAuthor);

        std::cout << "Enter genre: ";
        std::getline(std::cin, bookGenre);
        
        std::cout << "Enter year published: ";
        std::cin >> yearPublished;
        std::cin.ignore();

        if (std::cin.fail()) {
            // Clear the failbit and ignore the remaining
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            std::cout << "Invalid input, Try again." << std::endl;
            continue;
        }

        break;

    }

    books.push_back(Book(bookIdCount, bookName, bookAuthor, bookGenre, yearPublished));
    bookIdCount++;

    std::cout << "[LOCK STATUS: UNLOCKED] Admin adding new a book" << std::endl;
}

void Library::RemoveBook() {

    std::lock_guard<std::mutex> lock(simUserBorrowBookMutex);

    std::cout << "[LOCK STATUS: LOCKED] Admin removing a book" << std::endl;

    if (books.size() > 0) {
        int enteredBookId;

        std::cout << "What book would you like to remove? [Enter the ID of the book]" << std::endl;
        DisplayAllBooks();

        while (true) {
            std::cout << ">> ";
            std::cin >> enteredBookId;
            std::cin.ignore();
            
            if (std::cin.fail()) {
                // Clear the failbit and ignore the remaining
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                std::cout << "Invalid input, Try again." << std::endl;
                continue;
            }
            else if (enteredBookId > books.size() || enteredBookId < 1) { 
                std::cin.clear();
                // std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                std::cout << "Book does not exist." << std::endl;
                continue;
            }

            break;
        }

        if (books[enteredBookId - 1].IsAvailable()) {
            books.erase(books.begin() + (enteredBookId - 1));

            for (int bookIndex = (enteredBookId - 1); bookIndex < books.size(); bookIndex++)
                books[bookIndex].UpdateId(books[bookIndex].GetId() - 1);

            std::cout << "Book removed" << std::endl;
        }
        else {
            std::cout << "Cannot remove book, it is borrowed by someone else" << std::endl;
        }

    }
    else {
        std::cout << "No books to remove." << std::endl;
    }

    std::cout << "[LOCK STATUS: UNLOCKED] Admin removing a book" << std::endl;

}

void Library::UpdateBook() {

    std::lock_guard<std::mutex> lock(simUserBorrowBookMutex);

    std::cout << "[LOCK STATUS: LOCKED] Admin updating a book" << std::endl;
    
   if (books.size() > 0) {
        int enteredBookId;

        std::cout << "What book would you like to update? [Enter the ID of the book]" << std::endl;
        DisplayAllBooks();

        while (true) {
            std::cout << ">> ";
            std::cin >> enteredBookId;
            std::cin.ignore();
            
            if (std::cin.fail()) {
                // Clear the failbit and ignore the remaining
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                std::cout << "Invalid input, Try again." << std::endl;
                continue;
            }
            else if (enteredBookId > books.size() || enteredBookId < 1) { 
                std::cin.clear();
                // std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                std::cout << "Book does not exist." << std::endl;
                continue;
            }

            break;
        }

        if (books[enteredBookId - 1].IsAvailable()) {
            
            std::string newbookName;
            std::string newbookAuthor;
            std::string newbookGenre;
            int newYearPublished;

            std::cout << "Update book details: " << std::endl;

            while (true) {
                
                std::cout << "Book name: ";
                std::getline(std::cin, newbookName);

                std::cout << "Enter author name: ";
                std::getline(std::cin, newbookAuthor);

                std::cout << "Enter genre: ";
                std::getline(std::cin, newbookGenre);
                
                std::cout << "Enter year published: ";
                std::cin >> newYearPublished;
                std::cin.ignore();

                if (std::cin.fail()) {
                    // Clear the failbit and ignore the remaining
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                    std::cout << "Invalid input, Try again." << std::endl;
                    continue;
                }

                break;
 
            }

            std::cout << "Book has been updated." << std::endl;

            books[enteredBookId - 1].UpdateDetails(newbookName, newbookAuthor, newbookGenre, newYearPublished);

        }
        else {
            std::cout << "Cannot update book, it is borrowed by someone else" << std::endl;
        }

    }
    else {
        std::cout << "No books to update." << std::endl;
    }

    std::cout << "[LOCK STATUS: UNLOCKED] Admin updating a book" << std::endl;
}

void Library::MainMenu() {
    int chosenOption;

    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "\tLibrary Managemet System" << std::endl;
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "\t\t[Main Menu]\n";
    std::cout << "[1] Log in\n";
    std::cout << "[2] Register\n";
    std::cout << "[3] Exit program\n\n";

    while (true) {
        std::cout << ">> ";
        std::cin >> chosenOption;
        std::cin.ignore();

        if (std::cin.fail()) {
            // Clear the failbit and ignore the remaining
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            std::cout << "Invalid input, Try again." << std::endl;
            continue;
        }
        else if (chosenOption > 3 || chosenOption < 1) { 
            std::cin.clear();
            // std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            std::cout << "Invalid option." << std::endl;
            continue;
        }

        std::cin.clear();
        break;

    }

    switch(chosenOption) {

        case 1: {
            UserLogIn();
            break;
        }
        
        case 2: {
            RegisterUser();
            break;
        }

        case 3: {
            continueRunning = false;
            break;
        }

    }
}

void Library::UserMenu() {
    int chosenOption;

    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "\tLibrary Managemet System" << std::endl;
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "\t[Welcome back, " << mainLoggedInUser->GetName() << "]\n";
    std::cout << "\t[Borrowed book: " << mainLoggedInUser->GetBorrowedBookName() << "]\n";
    std::cout << "[1] Borrow book\n";
    std::cout << "[2] Return book\n";
    std::cout << "[3] View all books\n";
    std::cout << "[4] Logout\n\n";

    while (true) {
        std::cout << ">> ";
        std::cin >> chosenOption;
        std::cin.ignore();
        
        if (std::cin.fail()) {
            // Clear the failbit and ignore the remaining
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            std::cout << "Invalid input, Try again." << std::endl;
            continue;
        }
        else if (chosenOption > 4 || chosenOption < 1) { 
            std::cin.clear();
            // std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            std::cout << "Invalid option." << std::endl;
            continue;
        }

        break;
    }

    switch(chosenOption) {
        case 1: {
            BorrowBook();
            break;
        }

        case 2: {
            ReturnBook();
            break;
        }

        case 3: {
            DisplayAllBooks();
            break;
        }

        case 4: {
            std::cout << "Logged out" << std::endl;
            UserLogOut();
            break;
        }
    }

}

void Library::UserAdminMenu() {
    int chosenOption;

    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "\tLibrary Managemet System" << std::endl;
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "\t[Admin Menu]\n";
    std::cout << "\t[Welcome back, " << mainLoggedInUser->GetName() << "]\n";
    std::cout << "[1] Add book\n";
    std::cout << "[2] Remove book\n";
    std::cout << "[3] Update book\n";
    std::cout << "[4] View all books\n";
    std::cout << "[5] Display all users\n";
    std::cout << "[6] Logout\n\n";

    while (true) {
        std::cout << ">> ";
        std::cin >> chosenOption;
        std::cin.ignore();
        
        if (std::cin.fail()) {
            // Clear the failbit and ignore the remaining
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            std::cout << "Invalid input, Try again." << std::endl;
            continue;
        }
        else if (chosenOption > 5 || chosenOption < 1) { 
            std::cin.clear();
            // std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            std::cout << "Invalid option." << std::endl;
            continue;
        }

        break;
    }

    switch(chosenOption) {
        case 1: {
            AddBook();
            break;
        }

        case 2: {
            RemoveBook();
            break;
        }

        case 3: {
            UpdateBook();
            break;
        }

        case 4: {
            DisplayAllBooks();
            break;
        }

        case 5: {
            DisplayAllUsers();
            break;
        }

        case 6: {
            std::cout << "Logged out" << std::endl;
            UserLogOut();
            break;
        }
    }

}

// user simulation methods
int Library::RegisterSimulatedUser(std::string name, std::string pass) {

    std::lock_guard<std::mutex> lock(createSimUserMutex);
    int currentIndex = userIdCount;

    users.push_back(User(userIdCount, name, pass));
    userIdCount++;

    return currentIndex;

}

void Library::SimulatedUserBorrowBook(int id, User& simUser) {
    std::lock_guard<std::mutex> lock(simUserBorrowBookMutex);

    if (simUser.HasAlreadyBorrowed() == false) {
        if (books[id].IsAvailable()) {
            simUser.BorrowBook(books[id].Borrowed(simUser.GetId(), simUser.GetName()), books[id].GetName());
        } 
    }

}

void Library::SimulatedUserReturnBook(User& simUser) {
    std::lock_guard<std::mutex> lock(simUserReturnBookMutex);

    if (simUser.HasAlreadyBorrowed()) {
        books[simUser.ReturnBook() - 1].Returned();
    }

}

// void Library::

void Library::UserSimulation(std::string testName, std::string testPass) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); 

    User& simUserAccount = users[RegisterSimulatedUser(testName, testPass)];

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> actionDist(0, 1); // 0=borrow, 1=return
    std::uniform_int_distribution<> bookDist(0, books.size());
    std::uniform_int_distribution<> timeDist(0, 1); // 0 = 5 sec, 1 = 10 sec

    for (int i = 0; i < 10; ++i) { // simulate 10 actions per user
        
        int action = actionDist(gen);

        if (simUserAccount.HasAlreadyBorrowed()) 
            action = 1;
        

        switch (action) {
            case 0: {
                int bookId = bookDist(gen);

                SimulatedUserBorrowBook(bookId, simUserAccount);
                
                break;
            }
                
            case 1: {

                SimulatedUserReturnBook(simUserAccount);
                
                break;
            }
                
        }

        int timeDelay = timeDist(gen);

        if (timeDelay == 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // simulate delay 5 sec
        else   
            std::this_thread::sleep_for(std::chrono::milliseconds(10000)); // simulate delay 5 sec
    }

}


