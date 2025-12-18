#ifndef ACHIEVEMENT_H
#define ACHIEVEMENT_H

#include <string>
#include <functional>
#include <vector>
#include <SFML/Graphics.hpp>

enum class AchievementType {
    FIRST_GAME,          // Сыграть первую игру
    PERFECT_GAME,        // Найти все пары за минимальное количество ходов
    SPEED_RUNNER,        // Пройти игру менее чем за 60 секунд
    COMBO_MASTER,        // Найти 3 пары подряд без ошибок
    DIFFICULTY_MASTER,   // Пройти все уровни сложности
    THEME_COLLECTOR,     // Сыграть со всеми темами
    SCORE_MILESTONE_1000, // Набрать 1000 очков
    SCORE_MILESTONE_5000, // Набрать 5000 очков
    MOVES_EFFICIENT,     // Пройти игру с эффективностью 100%
    STREAK_5,            // Выиграть 5 игр подряд
    DAILY_PLAYER,        // Играть 7 дней подряд
    MATCH_FANATIC,       // Найти 100 пар карточек
    NO_MISTAKES,         // Пройти игру без ошибок
    ALL_DIFFICULTIES,    // Пройти все уровни сложности
    QUICK_THINKER        // Найти пару менее чем за 3 секунды
};

enum class AchievementRarity {
    COMMON,      // Обычное
    RARE,        // Редкое
    EPIC,        // Эпическое
    LEGENDARY    // Легендарное
};

struct Achievement {
    AchievementType type;
    std::string title;
    std::string description;
    AchievementRarity rarity;
    bool unlocked;
    std::string icon; // Эмодзи или символ для иконки
    std::string unlockDate;
    int progress;     // Текущий прогресс (например, 3/5 игр)
    int requirement;  // Требуемое значение для разблокировки
    
    Achievement(AchievementType t, const std::string& ti, const std::string& desc, 
                AchievementRarity r, const std::string& ic, int req = 1)
        : type(t), title(ti), description(desc), rarity(r), 
          unlocked(false), icon(ic), progress(0), requirement(req) {}
    
    void unlock() { 
        unlocked = true; 
        progress = requirement;
    }
    
    void addProgress(int amount = 1) { 
        progress += amount;
        if (progress >= requirement && !unlocked) {
            unlock();
        }
    }
    
    float getProgressPercent() const {
        return (float)progress / requirement * 100.0f;
    }
    
    std::string getRarityString() const {
        switch (rarity) {
            case AchievementRarity::COMMON: return "Common";
            case AchievementRarity::RARE: return "Rare";
            case AchievementRarity::EPIC: return "Epic";
            case AchievementRarity::LEGENDARY: return "Legendary";
            default: return "Unknown";
        }
    }
    
    sf::Color getRarityColor() const {
        switch (rarity) {
            case AchievementRarity::COMMON: return sf::Color(200, 200, 200);    // Светло-серый
            case AchievementRarity::RARE: return sf::Color(65, 105, 225);       // Королевский синий
            case AchievementRarity::EPIC: return sf::Color(147, 112, 219);      // Средний фиолетовый
            case AchievementRarity::LEGENDARY: return sf::Color(218, 165, 32);  // Золотой
            default: return sf::Color::White;
        }
    }
};

class AchievementManager {
private:
    std::vector<Achievement> achievements;
    std::vector<AchievementType> unlockedAchievements;
    int totalScore;
    std::string playerName;
    std::string saveFilePath;
    
    // Статистика для достижений
    std::map<std::string, bool> playedThemes;
    std::map<std::string, bool> playedDifficulties;
    int totalPairsFound;
    int perfectGamesCount;
    int quickMatchesCount;
    
    void initializeAchievements();
    std::string getCurrentDateTime() const;
    void loadAchievementStats();
    void saveAchievementStats();
    
public:
    AchievementManager();
    AchievementManager(const std::string& playerName);
    
    void setPlayerName(const std::string& name);
    
    void updateAchievement(AchievementType type, int progress = 1);
    void unlockAchievement(AchievementType type);
    void checkGameAchievements(int score, int moves, int totalPairs, 
                               double time, const std::string& difficulty, 
                               const std::string& theme);
    
    bool isAchievementUnlocked(AchievementType type) const;
    Achievement* getAchievement(AchievementType type);
    std::vector<Achievement> getUnlockedAchievements() const;
    std::vector<Achievement> getAllAchievements() const { return achievements; }
    std::string getPlayerName() const { return playerName; }
    
    int getTotalScore() const { return totalScore; }
    int getUnlockedCount() const;
    int getTotalCount() const { return achievements.size(); }
    
    void resetProgress();
    
    void saveToFile(); 
    void saveToFile(const std::string& filename);
    void loadFromFile(); 
    void loadFromFile(const std::string& filename);
    
    // Специальные методы для конкретных достижений
    void recordGameResult(int score, const std::string& difficulty, const std::string& theme);
    void recordDailyPlay();
    void recordPerfectMatch();
    void recordQuickMatch(double time);
    
    void addPlayedTheme(const std::string& theme);
    void addPlayedDifficulty(const std::string& difficulty);
    int getPlayedThemesCount() const { return playedThemes.size(); }
    int getPlayedDifficultiesCount() const { return playedDifficulties.size(); }
};

#endif
