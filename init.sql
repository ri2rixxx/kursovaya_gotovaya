-- Инициализация базы данных для Memory Game
CREATE TABLE IF NOT EXISTS games (
    id SERIAL PRIMARY KEY,
    player_name VARCHAR(100) NOT NULL,
    score INTEGER NOT NULL,
    moves INTEGER NOT NULL,
    pairs INTEGER NOT NULL,
    time DOUBLE PRECISION NOT NULL,
    date TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    difficulty VARCHAR(20) NOT NULL
);

-- Таблица пользователей
CREATE TABLE IF NOT EXISTS users (
    id SERIAL PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    password VARCHAR(100) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    registration_date TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    last_login TIMESTAMP
);

-- Статистика пользователей
CREATE TABLE IF NOT EXISTS user_stats (
    id SERIAL PRIMARY KEY,
    user_id INTEGER REFERENCES users(id) ON DELETE CASCADE,
    total_score INTEGER DEFAULT 0,
    games_played INTEGER DEFAULT 0,
    games_won INTEGER DEFAULT 0,
    total_play_time DOUBLE PRECISION DEFAULT 0.0
);

-- Создаем индексы для производительности
CREATE INDEX IF NOT EXISTS idx_games_player_name ON games(player_name);
CREATE INDEX IF NOT EXISTS idx_games_score ON games(score DESC);
CREATE INDEX IF NOT EXISTS idx_games_date ON games(date DESC);
CREATE INDEX IF NOT EXISTS idx_users_username ON users(username);
