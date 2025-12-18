#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <string>
#include <vector>
#include <memory>
#include "Database.h"

struct User {
    std::string username;
    std::string password;
    std::string email;
    int id;
    int totalScore;
    int gamesPlayed;
    int gamesWon;
    double totalPlayTime;
    std::string registrationDate;
    std::string lastLogin;
    
    User() : id(0), totalScore(0), gamesPlayed(0), gamesWon(0), totalPlayTime(0) {}
    User(const std::string& name, const std::string& pwd, const std::string& mail)
        : username(name), password(pwd), email(mail), id(0), 
          totalScore(0), gamesPlayed(0), gamesWon(0), totalPlayTime(0) {}
};

class UserManager {
private:
    std::unique_ptr<Database> database;
    User currentUser;
    bool isLoggedIn;
    
    std::string hashPassword(const std::string& password);
    bool verifyPassword(const std::string& password, const std::string& hash);
    
public:
    UserManager(const std::string& connStr = "");
    ~UserManager();
    
    bool initialize();
    
    // Регистрация и вход
    bool registerUser(const std::string& username, const std::string& password, 
                     const std::string& email, std::string& errorMsg);
    bool login(const std::string& username, const std::string& password, std::string& errorMsg);
    bool logout();
    
    // Статистика
    bool updateStats(int score, bool won, double playTime);
    std::vector<User> getLeaderboard(int limit = 10);
    
    // Текущий пользователь
    User getCurrentUser() const { return currentUser; }
    bool isUserLoggedIn() const { return isLoggedIn; }
    std::string getCurrentUsername() const { return currentUser.username; }
    std::string getCurrentEmail() const { return currentUser.email; }
    
    // Проверки
    bool usernameExists(const std::string& username);
    bool emailExists(const std::string& email);
};

#endif
