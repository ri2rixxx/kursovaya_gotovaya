#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <iostream>
#include <set>
#include "Card.h"
#include "Player.h"
#include "Database.h"
#include "GUI/Button.h"
#include "GUI/CardSprite.h"
#include "GUI/Menu.h"
#include "Audio/SoundManager.h"
#include "Audio/MusicPlayer.h"
#include "ContactForm.h"
#include "Achievement.h"
#include "UserManager.h"
#include <iomanip>

enum class GameState {
    LOGIN_SCREEN,
    REGISTER_SCREEN,
    MAIN_MENU,
    ENTER_NAME,
    SETUP,
    PLAYING,
    PAUSED,
    GAME_OVER_WIN,
    GAME_OVER_LOSE,
    LEADERBOARD,
    SETTINGS,
    ACHIEVEMENTS,
    CONTACT_FORM,
    EXIT
};

enum class Difficulty {
    EASY,    // 3x4 = 12 cards (6 pairs)
    MEDIUM,  // 4x4 = 16 cards (8 pairs)
    HARD,    // 4x6 = 24 cards (12 pairs)
    EXPERT   // 6x6 = 36 cards (18 pairs)
};

class Game {
private:
    sf::RenderWindow window;
    sf::Font mainFont;
    sf::Clock gameClock;
    sf::Time elapsedTime;

    Database db;

    std::unique_ptr<UserManager> userManager;

    std::vector<std::unique_ptr<CardSprite>> cards;
    std::unique_ptr<Player> player;
    std::unique_ptr<Database> database;
    std::unique_ptr<SoundManager> soundManager;
    std::unique_ptr<MusicPlayer> musicPlayer;
    std::unique_ptr<AchievementManager> achievementManager;
    std::vector<Card> gameCards;

    std::string usernameInput;
    std::string passwordInput;
    std::string emailInput;
    std::string confirmPasswordInput;
    std::string playerNameInput;
    bool isEnteringName;

    enum class InputField { USERNAME, PASSWORD, EMAIL, CONFIRM_PASSWORD, NONE };
    InputField activeInputField;

    sf::Text nameInputText;
    sf::RectangleShape nameInputBox;
    sf::Text loginErrorText;
    sf::Text registerErrorText;
    sf::Text welcomeText;
    sf::Text userStatsText;

    sf::Texture menuBackgroundTexture;
    sf::Texture gameBackgroundTexture;
    sf::Sprite background;
    sf::Color menuBackgroundColor;
    sf::Color gameBackgroundColor;

    std::vector<Button> loginButtons;
    std::vector<Button> registerButtons;
    std::vector<Button> mainMenuButtons;
    std::vector<Button> gameButtons;
    std::vector<Button> pauseButtons;
    std::vector<Button> setupButtons;
    std::vector<Button> leaderboardButtons;
    std::vector<Button> settingsButtons;
    std::vector<Button> achievementsButtons;
    Button surrenderButton;

    float brightness;
    sf::VideoMode currentVideoMode;
    std::vector<sf::VideoMode> availableVideoModes;
    int currentVideoModeIndex;

    sf::Text titleText;
    sf::Text statsText;
    sf::Text timerText;
    sf::Text scoreText;
    sf::Text difficultyText;
    sf::Text settingsTitle;
    
    GameState currentState;
    GameState previousState;
    Difficulty difficulty;
    CardTheme currentTheme;
    int rows, cols, totalPairs;
    int matchedPairs;
    int moves;
    bool isGameActive;
    bool firstCardSelected;
    int selectedCard1, selectedCard2;

    float cardFlipTime;
    float cardFlipProgress;
    bool isFlipping;

    CardSprite* firstCard;
    CardSprite* secondCard;
    bool isChecking;
    
    bool hasWon;
    
    // Для достижений
    sf::Clock pairTimer;
    sf::Clock gameTimer;
    int consecutiveMatches;
    int totalGamesPlayed;
    int winStreak;
    
    // Для скроллинга достижений
    float achievementsScrollOffset;
    float achievementsTotalHeight;
    
    // Resource paths
    std::map<CardTheme, std::string> themeImagePaths;
    
    // Форма обратной связи
    ContactForm contactForm;

    void updateBackgrounds();
    void loadResources();
    void setupLoginUI();
    void setupRegisterUI();
    void setupMainMenu();
    void setupGameUI();
    void setupPauseMenu();
    void setupSetupMenu();
    void setupLeaderboardUI();
    void setupSettingsMenu();
    void setupAchievementsUI();
    void setupContactForm();
    void initializeCards();
    void createCardSprites();
    void resetGame();
    void updateStats();
    void handleCardClick(int cardIndex);
    void processCardMatch();
    void saveGameResult();
    void checkAchievements();

    void renderLoginScreen();
    void renderRegisterScreen();
    void renderNameInput();
    void renderContactForm();
    void renderAchievements();
    void renderGame();
    void renderMainMenu();
    void renderPauseMenu();
    void renderSetupMenu();
    void renderGameOverWin();
    void renderGameOverLose();
    void renderLeaderboard();
    void renderSettings();

    std::string getDifficultyString() const;
    std::string getCurrentDate() const;
    sf::Color getDifficultyColor() const;
    void getImagePathsForTheme(CardTheme theme, std::vector<std::string>& imagePaths);

    void handleLoginInput(sf::Event event);
    void handleRegisterInput(sf::Event event);
    void updateUserStatsDisplay();
    void logoutUser();

    bool isRunningInDockerInternal();

public:
    Game();
    ~Game();

    void run();
    void handleEvents();
    void update(float deltaTime);
    void render();

    void startNewGame();
    void pauseGame();
    void resumeGame();
    void showLeaderboard();
    void showAchievements();
    void showSettings();
    void exitGame();
    void surrenderGame();
    void setDifficulty(Difficulty diff);
    void setTheme(CardTheme theme);

    GameState getState() const { return currentState; }
    int getScore() const { return player ? player->getScore() : 0; }
    sf::Font& getMainFont() { return mainFont; }
};

#endif
