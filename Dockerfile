FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Используем зеркало
RUN sed -i 's/archive.ubuntu.com/mirrors.ustc.edu.cn/g' /etc/apt/sources.list

# Устанавливаем ВСЕ необходимые зависимости для звука
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    libsfml-dev \
    libpq-dev \
    postgresql-client \
    fonts-dejavu \
    wget \
    pkg-config \
    file \
    # Звуковые зависимости
    alsa-utils \
    pulseaudio \
    libasound2 \
    libasound2-plugins \
    libpulse0 \
    libopenal-dev \
    libflac-dev \
    libvorbis-dev \
    libogg-dev \
    sox \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Создаем пользователя для звука (важно для PulseAudio)
RUN useradd -m -u 1000 gameuser && \
    usermod -a -G audio gameuser && \
    chown -R gameuser:gameuser /app

# Копируем исходный код и ресурсы
COPY --chown=gameuser:gameuser . .

# Проверяем ресурсы
RUN echo "=== Resource Check ===" && \
    echo "Sound files:" && \
    find . -name "*.wav" -type f 2>/dev/null && \
    echo "=== File check ===" && \
    if [ -f assets/sounds/flip.wav ]; then \
        echo "WAV file found, checking format:"; \
        file assets/sounds/flip.wav; \
    else \
        echo "ERROR: flip.wav not found!"; \
    fi

# Собираем проект
RUN mkdir -p build && cd build && cmake .. && make -j$(nproc)

# Создаем папки для данных с правильными правами
RUN mkdir -p /app/saves /app/feedback /app/database && \
    chown -R gameuser:gameuser /app/saves /app/feedback /app/database && \
    chmod 777 /app/saves /app/feedback /app/database

# Копируем тестовые звуковые файлы если их нет
RUN if [ ! -f "assets/sounds/flip.wav" ]; then \
    echo "Creating placeholder sound files..."; \
    mkdir -p assets/sounds; \
    for sound in flip match mismatch click win lose; do \
        echo "Creating $sound.wav..."; \
        sox -n -r 44100 -b 16 -c 1 "assets/sounds/$sound.wav" synth 0.3 sine 440 gain -3 2>/dev/null || true; \
    done; \
    fi

USER gameuser

# Создаем правильный стартовый скрипт
RUN echo '#!/bin/bash' > /app/start.sh && \
    echo 'echo "=== DOCKER STARTUP ==="' >> /app/start.sh && \
    echo 'echo "User: $(whoami)"' >> /app/start.sh && \
    echo 'echo "UID: $(id -u)"' >> /app/start.sh && \
    echo 'echo "GID: $(id -g)"' >> /app/start.sh && \
    echo 'echo ""' >> /app/start.sh && \
    echo 'echo "=== Checking directories ==="' >> /app/start.sh && \
    echo 'ls -la /app/' >> /app/start.sh && \
    echo 'echo ""' >> /app/start.sh && \
    echo 'echo "=== Checking data directories ==="' >> /app/start.sh && \
    echo 'mkdir -p /app/saves /app/feedback /app/database' >> /app/start.sh && \
    echo 'chmod 777 /app/saves /app/feedback /app/database' >> /app/start.sh && \
    echo 'ls -la /app/saves /app/feedback /app/database' >> /app/start.sh && \
    echo 'echo ""' >> /app/start.sh && \
    echo 'echo "=== Sound check ==="' >> /app/start.sh && \
    echo 'pulseaudio --check 2>&1 || echo "PulseAudio not running (may be OK)"' >> /app/start.sh && \
    echo 'echo ""' >> /app/start.sh && \
    echo 'echo "=== Starting Game ==="' >> /app/start.sh && \
    echo 'exec ./build/memory_game' >> /app/start.sh && \
    chmod +x /app/start.sh

CMD ["/app/start.sh"]
