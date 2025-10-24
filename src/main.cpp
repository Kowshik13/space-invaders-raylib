#include "raylib.h"
#include "resource_dir.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

namespace
{
constexpr int kWindowWidth = 800;
constexpr int kWindowHeight = 800;
constexpr int kObstacleBlockSize = 3;
constexpr int kObstacleCols = 23;
constexpr int kObstacleRows = 13;
constexpr int kMaxLeaderboardEntries = 10;

Color GreyBackground()
{
    return {29, 29, 27, 255};
}
} // namespace

struct DifficultySettings
{
    std::string name;
    float alienMoveInterval;  // Seconds between horizontal steps
    float alienHorizontalStep;
    float alienLaserInterval; // Seconds between alien shots
    float alienLaserSpeed;
    float alienDownStep;
    float spaceshipMoveSpeed;
    float spaceshipLaserSpeed;
    double spaceshipFireCooldown;
    int startingLives;
};

struct LeaderboardEntry
{
    std::string name;
    int score = 0;
    std::string difficulty;
};

class Leaderboard
{
  public:
    explicit Leaderboard(std::string filePath) : m_filePath(std::move(filePath))
    {
        Load();
    }

    void AddEntry(const LeaderboardEntry &entry)
    {
        m_entries.push_back(entry);
        SortEntries();
        Save();
    }

    const std::vector<LeaderboardEntry> &GetEntries() const
    {
        return m_entries;
    }

  private:
    void Load()
    {
        m_entries.clear();

        std::ifstream file(m_filePath);
        if (!file.is_open())
        {
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            if (line.empty())
            {
                continue;
            }

            std::stringstream stream(line);
            std::string scoreString;
            std::string difficulty;
            std::string name;

            if (!std::getline(stream, scoreString, ','))
            {
                continue;
            }

            if (!std::getline(stream, difficulty, ','))
            {
                continue;
            }

            if (!std::getline(stream, name))
            {
                name = "Player";
            }

            try
            {
                LeaderboardEntry entry{name, std::stoi(scoreString), difficulty};
                m_entries.push_back(entry);
            }
            catch (const std::exception &)
            {
                std::cerr << "Ignoring invalid leaderboard line: " << line << std::endl;
            }
        }

        SortEntries();
    }

    void Save() const
    {
        std::ofstream file(m_filePath, std::ios::trunc);
        if (!file.is_open())
        {
            std::cerr << "Unable to write leaderboard to " << m_filePath << std::endl;
            return;
        }

        for (const auto &entry : m_entries)
        {
            file << entry.score << "," << entry.difficulty << "," << entry.name << "\n";
        }
    }

    void SortEntries()
    {
        std::sort(m_entries.begin(), m_entries.end(), [](const LeaderboardEntry &lhs, const LeaderboardEntry &rhs) {
            return lhs.score > rhs.score;
        });

        if (m_entries.size() > kMaxLeaderboardEntries)
        {
            m_entries.resize(kMaxLeaderboardEntries);
        }
    }

    std::vector<LeaderboardEntry> m_entries;
    std::string m_filePath;
};

class Laser
{
  public:
    Laser(Vector2 position, float speed, Color color) : m_position(position), m_speed(speed), m_color(color)
    {
    }

    void Update()
    {
        m_position.y += m_speed;
        if (m_position.y < -20 || m_position.y > GetScreenHeight() + 20)
        {
            m_active = false;
        }
    }

    void Draw() const
    {
        if (m_active)
        {
            DrawRectangleV(m_position, {4, 15}, m_color);
        }
    }

    Rectangle GetRect() const
    {
        return {m_position.x, m_position.y, 4, 15};
    }

    bool IsActive() const
    {
        return m_active;
    }

    void Deactivate()
    {
        m_active = false;
    }

  private:
    Vector2 m_position{};
    float m_speed = 0.0f;
    Color m_color{};
    bool m_active = true;
};

class Block
{
  public:
    explicit Block(Vector2 position) : m_position(position) {}

    Rectangle GetRect() const
    {
        return {m_position.x, m_position.y, static_cast<float>(kObstacleBlockSize),
                static_cast<float>(kObstacleBlockSize)};
    }

    void Draw() const
    {
        DrawRectangleV(m_position, {static_cast<float>(kObstacleBlockSize), static_cast<float>(kObstacleBlockSize)},
                        {243, 216, 63, 255});
    }

  private:
    Vector2 m_position{};
};

class Obstacle
{
  public:
    explicit Obstacle(Vector2 position)
    {
        for (int row = 0; row < static_cast<int>(m_grid.size()); ++row)
        {
            for (int column = 0; column < static_cast<int>(m_grid[row].size()); ++column)
            {
                if (m_grid[row][column] == 1)
                {
                    Vector2 blockPosition = {position.x + static_cast<float>(column * kObstacleBlockSize),
                                             position.y + static_cast<float>(row * kObstacleBlockSize)};
                    m_blocks.emplace_back(blockPosition);
                }
            }
        }
    }

    void Draw() const
    {
        for (const auto &block : m_blocks)
        {
            block.Draw();
        }
    }

    std::vector<Block> &Blocks()
    {
        return m_blocks;
    }

  private:
    std::vector<Block> m_blocks;
    static const std::array<std::array<int, kObstacleCols>, kObstacleRows> m_grid;
};

const std::array<std::array<int, kObstacleCols>, kObstacleRows> Obstacle::m_grid = {
    std::array<int, kObstacleCols>{0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    std::array<int, kObstacleCols>{0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    std::array<int, kObstacleCols>{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    std::array<int, kObstacleCols>{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    std::array<int, kObstacleCols>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    std::array<int, kObstacleCols>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    std::array<int, kObstacleCols>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    std::array<int, kObstacleCols>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    std::array<int, kObstacleCols>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    std::array<int, kObstacleCols>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    std::array<int, kObstacleCols>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    std::array<int, kObstacleCols>{1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
    std::array<int, kObstacleCols>{1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
    std::array<int, kObstacleCols>{1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1}};

class Alien
{
  public:
    Alien() = default;

    Alien(int type, Vector2 position) : m_type(type), m_position(position)
    {
        EnsureTexturesLoaded();

        switch (m_type)
        {
        case 1:
            m_scoreValue = 150;
            break;
        case 2:
            m_scoreValue = 100;
            break;
        default:
            m_scoreValue = 50;
            break;
        }
    }

    Rectangle GetRect() const
    {
        const Texture2D &image = m_alienImages[m_type - 1];
        return {m_position.x, m_position.y, static_cast<float>(image.width), static_cast<float>(image.height)};
    }

    void Draw() const
    {
        DrawTextureV(m_alienImages[m_type - 1], m_position, WHITE);
    }

    void Update(float xOffset, float yOffset)
    {
        m_position.x += xOffset;
        m_position.y += yOffset;
    }

    float GetPositionX() const
    {
        return m_position.x;
    }

    float GetPositionY() const
    {
        return m_position.y;
    }

    float GetWidth() const
    {
        return static_cast<float>(m_alienImages[m_type - 1].width);
    }

    float GetHeight() const
    {
        return static_cast<float>(m_alienImages[m_type - 1].height);
    }

    int GetScoreValue() const
    {
        return m_scoreValue;
    }

    static void UnloadTextures()
    {
        if (!m_texturesLoaded)
        {
            return;
        }

        for (auto &texture : m_alienImages)
        {
            if (texture.id != 0)
            {
                UnloadTexture(texture);
                texture = {};
            }
        }

        m_texturesLoaded = false;
    }

  private:
    static void EnsureTexturesLoaded()
    {
        if (m_texturesLoaded)
        {
            return;
        }

        m_alienImages[0] = LoadTexture("alien_1.png");
        m_alienImages[1] = LoadTexture("alien_2.png");
        m_alienImages[2] = LoadTexture("alien_3.png");
        m_texturesLoaded = true;
    }

    Vector2 m_position{};
    int m_type = 1;
    int m_scoreValue = 50;

    static std::array<Texture2D, 3> m_alienImages;
    static bool m_texturesLoaded;
};

std::array<Texture2D, 3> Alien::m_alienImages = {};
bool Alien::m_texturesLoaded = false;

class Spaceship
{
  public:
    Spaceship()
    {
        m_image = LoadTexture("spaceship.png");
        Reset();
    }

    ~Spaceship()
    {
        if (m_image.id != 0)
        {
            UnloadTexture(m_image);
        }
    }

    void Reset()
    {
        m_position.x = (GetScreenWidth() - static_cast<float>(m_image.width)) / 2.0f;
        m_position.y = static_cast<float>(GetScreenHeight() - m_image.height - 10);
        m_lasers.clear();
        m_lastFireTime = 0.0;
    }

    void Draw() const
    {
        DrawTextureV(m_image, m_position, WHITE);
    }

    void UpdateLasers()
    {
        for (auto &laser : m_lasers)
        {
            laser.Update();
        }
    }

    void DrawLasers() const
    {
        for (const auto &laser : m_lasers)
        {
            laser.Draw();
        }
    }

    void MoveLeft()
    {
        if (m_position.x > 0)
        {
            m_position.x = std::max(0.0f, m_position.x - m_moveSpeed);
        }
    }

    void MoveRight()
    {
        float maxX = static_cast<float>(GetScreenWidth() - m_image.width);
        if (m_position.x < maxX)
        {
            m_position.x = std::min(maxX, m_position.x + m_moveSpeed);
        }
    }

    void FireLaser()
    {
        double currentTime = GetTime();
        if (currentTime - m_lastFireTime < m_fireCooldown)
        {
            return;
        }

        Vector2 laserPosition = {m_position.x + static_cast<float>(m_image.width) / 2.0f - 2.0f, m_position.y - 15};
        m_lasers.emplace_back(laserPosition, m_laserSpeed, m_laserColor);
        m_lastFireTime = currentTime;
    }

    Rectangle GetRect() const
    {
        return {m_position.x, m_position.y, static_cast<float>(m_image.width), static_cast<float>(m_image.height)};
    }

    std::vector<Laser> &Lasers()
    {
        return m_lasers;
    }

    const Texture2D &GetTexture() const
    {
        return m_image;
    }

    void SetMoveSpeed(float speed)
    {
        m_moveSpeed = speed;
    }

    void SetLaserSpeed(float speed)
    {
        m_laserSpeed = speed;
    }

    void SetFireCooldown(double cooldown)
    {
        m_fireCooldown = cooldown;
    }

    void ClearLasers()
    {
        m_lasers.clear();
    }

  private:
    Texture2D m_image{};
    Vector2 m_position{};
    std::vector<Laser> m_lasers;
    double m_lastFireTime = 0.0;
    float m_moveSpeed = 7.0f;
    float m_laserSpeed = -6.0f;
    double m_fireCooldown = 0.35;
    Color m_laserColor{243, 216, 63, 255};
};

class Game
{
  public:
    Game()
        : m_leaderboard("leaderboard.txt"),
          m_difficulties({DifficultySettings{"Easy", 0.65f, 8.0f, 1.2f, 4.0f, 10.0f, 6.0f, -6.0f, 0.30, 5},
                          DifficultySettings{"Normal", 0.45f, 10.0f, 0.85f, 5.0f, 12.0f, 7.0f, -7.5f, 0.28, 4},
                          DifficultySettings{"Hard", 0.28f, 12.0f, 0.55f, 6.5f, 14.0f, 8.5f, -9.0f, 0.23, 3}})
    {
        m_selectedDifficultyIndex = 1;
        m_currentDifficulty = m_difficulties[m_selectedDifficultyIndex];
        SetupNewSession();
    }

    ~Game()
    {
        Alien::UnloadTextures();
    }

    void HandleInput()
    {
        switch (m_state)
        {
        case GameState::Title:
            HandleTitleInput();
            break;
        case GameState::Playing:
            HandleGameplayInput();
            break;
        case GameState::Paused:
            if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ENTER))
            {
                m_state = GameState::Playing;
            }
            break;
        case GameState::GameOver:
            HandleGameOverInput();
            break;
        }
    }

    void Update()
    {
        if (m_state != GameState::Playing)
        {
            return;
        }

        m_spaceship.UpdateLasers();
        for (auto &laser : m_alienLasers)
        {
            laser.Update();
        }

        RemoveInactiveLasers();

        double currentTime = GetTime();
        if (currentTime - m_lastAlienMoveTime >= m_currentDifficulty.alienMoveInterval)
        {
            MoveAliens();
            m_lastAlienMoveTime = currentTime;
        }

        if (currentTime - m_timeLastAlienFired >= m_currentDifficulty.alienLaserInterval)
        {
            AlienShootLaser();
            m_timeLastAlienFired = currentTime;
        }

        CheckForCollisions();

        if (m_aliens.empty())
        {
            StartNextWave();
        }
    }

    void Draw() const
    {
        switch (m_state)
        {
        case GameState::Title:
            DrawTitleScreen();
            break;
        case GameState::Playing:
            DrawPlaying();
            break;
        case GameState::Paused:
            DrawPlaying();
            DrawPausedOverlay();
            break;
        case GameState::GameOver:
            DrawPlaying();
            DrawGameOverOverlay();
            break;
        }
    }

  private:
    enum class GameState
    {
        Title,
        Playing,
        Paused,
        GameOver
    };

    void SetupNewSession()
    {
        m_spaceship.Reset();
        m_spaceship.SetMoveSpeed(m_currentDifficulty.spaceshipMoveSpeed);
        m_spaceship.SetLaserSpeed(m_currentDifficulty.spaceshipLaserSpeed);
        m_spaceship.SetFireCooldown(m_currentDifficulty.spaceshipFireCooldown);
        m_spaceship.ClearLasers();

        m_obstacles = CreateObstacles();
        m_aliens = CreateAliens();
        m_alienLasers.clear();
        m_lives = m_currentDifficulty.startingLives;
        m_score = 0;
        m_wave = 1;
        m_aliensDirection = 1;
        m_lastAlienMoveTime = GetTime();
        m_timeLastAlienFired = GetTime();
        m_state = GameState::Title;
        m_playerName.clear();
        m_nameSubmitted = false;
        m_comboCount = 0;
        m_lastAlienDefeat = 0.0;
    }

    void HandleTitleInput()
    {
        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
        {
            m_selectedDifficultyIndex = (m_selectedDifficultyIndex + 1) % m_difficulties.size();
        }
        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
        {
            m_selectedDifficultyIndex = (m_selectedDifficultyIndex + m_difficulties.size() - 1) % m_difficulties.size();
        }

        if (IsKeyPressed(KEY_ENTER))
        {
            m_currentDifficulty = m_difficulties[m_selectedDifficultyIndex];
            SetupNewRun();
        }
    }

    void HandleGameplayInput()
    {
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
        {
            m_spaceship.MoveLeft();
        }
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
        {
            m_spaceship.MoveRight();
        }
        if (IsKeyDown(KEY_SPACE))
        {
            m_spaceship.FireLaser();
        }
        if (IsKeyPressed(KEY_P))
        {
            m_state = GameState::Paused;
        }
    }

    void HandleGameOverInput()
    {
        int key = GetCharPressed();
        while (key > 0)
        {
            if (key >= 32 && key <= 125 && m_playerName.length() < 12)
            {
                m_playerName.push_back(static_cast<char>(key));
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && !m_playerName.empty())
        {
            m_playerName.pop_back();
        }

        if (IsKeyPressed(KEY_ENTER))
        {
            if (m_playerName.empty())
            {
                m_playerName = "Player";
            }

            if (!m_nameSubmitted)
            {
                LeaderboardEntry entry{m_playerName, m_score, m_currentDifficulty.name};
                m_leaderboard.AddEntry(entry);
                m_nameSubmitted = true;
            }

            m_currentDifficulty = m_difficulties[m_selectedDifficultyIndex];
            SetupNewSession();
        }
    }

    void SetupNewRun()
    {
        m_spaceship.Reset();
        m_spaceship.SetMoveSpeed(m_currentDifficulty.spaceshipMoveSpeed);
        m_spaceship.SetLaserSpeed(m_currentDifficulty.spaceshipLaserSpeed);
        m_spaceship.SetFireCooldown(m_currentDifficulty.spaceshipFireCooldown);
        m_spaceship.ClearLasers();

        m_obstacles = CreateObstacles();
        m_aliens = CreateAliens();
        m_alienLasers.clear();
        m_lives = m_currentDifficulty.startingLives;
        m_score = 0;
        m_wave = 1;
        m_aliensDirection = 1;
        m_lastAlienMoveTime = GetTime();
        m_timeLastAlienFired = GetTime();
        m_state = GameState::Playing;
        m_comboCount = 0;
        m_lastAlienDefeat = 0.0;
        m_playerName.clear();
        m_nameSubmitted = false;
    }

    std::vector<Obstacle> CreateObstacles() const
    {
        std::vector<Obstacle> result;
        int obstacleWidth = kObstacleCols * kObstacleBlockSize;
        float gap = (GetScreenWidth() - 4.0f * obstacleWidth) / 5.0f;
        for (int i = 0; i < 4; ++i)
        {
            float offsetX = (i + 1) * gap + i * obstacleWidth;
            float offsetY = static_cast<float>(GetScreenHeight() - 120);
            result.emplace_back(Vector2{offsetX, offsetY});
        }
        return result;
    }

    std::vector<Alien> CreateAliens() const
    {
        std::vector<Alien> aliens;
        aliens.reserve(55);
        for (int row = 0; row < 5; ++row)
        {
            int type = 3;
            if (row == 0)
            {
                type = 1;
            }
            else if (row == 1 || row == 2)
            {
                type = 2;
            }

            for (int column = 0; column < 11; ++column)
            {
                float x = 75.0f + column * 55.0f;
                float y = 110.0f + row * 55.0f;
                aliens.emplace_back(type, Vector2{x, y});
            }
        }
        return aliens;
    }

    void MoveAliens()
    {
        if (m_aliens.empty())
        {
            return;
        }

        bool shouldChangeDirection = false;
        for (const auto &alien : m_aliens)
        {
            if (m_aliensDirection > 0 && alien.GetPositionX() + alien.GetWidth() + m_currentDifficulty.alienHorizontalStep >=
                                             static_cast<float>(GetScreenWidth()))
            {
                shouldChangeDirection = true;
                break;
            }
            if (m_aliensDirection < 0 && alien.GetPositionX() - m_currentDifficulty.alienHorizontalStep <= 0.0f)
            {
                shouldChangeDirection = true;
                break;
            }
        }

        if (shouldChangeDirection)
        {
            m_aliensDirection *= -1;
            MoveDownAliens(m_currentDifficulty.alienDownStep);
        }

        for (auto &alien : m_aliens)
        {
            alien.Update(m_currentDifficulty.alienHorizontalStep * static_cast<float>(m_aliensDirection), 0.0f);
        }
    }

    void MoveDownAliens(float distance)
    {
        for (auto &alien : m_aliens)
        {
            alien.Update(0.0f, distance);
            if (alien.GetPositionY() + alien.GetHeight() >= static_cast<float>(GetScreenHeight()) - 80.0f)
            {
                TriggerGameOver();
            }
        }
    }

    void AlienShootLaser()
    {
        if (m_aliens.empty())
        {
            return;
        }

        int randomIndex = GetRandomValue(0, static_cast<int>(m_aliens.size()) - 1);
        const Alien &alien = m_aliens[randomIndex];
        Vector2 position = {alien.GetPositionX() + alien.GetWidth() / 2.0f - 2.0f,
                            alien.GetPositionY() + alien.GetHeight()};
        m_alienLasers.emplace_back(position, m_currentDifficulty.alienLaserSpeed, RED);
    }

    void RemoveInactiveLasers()
    {
        auto &playerLasers = m_spaceship.Lasers();
        for (auto it = playerLasers.begin(); it != playerLasers.end();)
        {
            if (!it->IsActive())
            {
                it = playerLasers.erase(it);
            }
            else
            {
                ++it;
            }
        }

        for (auto it = m_alienLasers.begin(); it != m_alienLasers.end();)
        {
            if (!it->IsActive())
            {
                it = m_alienLasers.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    void CheckForCollisions()
    {
        auto &playerLasers = m_spaceship.Lasers();

        for (auto &laser : playerLasers)
        {
            if (!laser.IsActive())
            {
                continue;
            }

            for (auto alienIt = m_aliens.begin(); alienIt != m_aliens.end();)
            {
                if (CheckCollisionRecs(alienIt->GetRect(), laser.GetRect()))
                {
                    m_score += CalculateScore(*alienIt);
                    alienIt = m_aliens.erase(alienIt);
                    laser.Deactivate();
                    break;
                }
                else
                {
                    ++alienIt;
                }
            }

            if (!laser.IsActive())
            {
                continue;
            }

            for (auto &obstacle : m_obstacles)
            {
                auto &blocks = obstacle.Blocks();
                for (auto blockIt = blocks.begin(); blockIt != blocks.end();) {
                    if (CheckCollisionRecs(blockIt->GetRect(), laser.GetRect()))
                    {
                        blockIt = blocks.erase(blockIt);
                        laser.Deactivate();
                        break;
                    }
                    else
                    {
                        ++blockIt;
                    }
                }

                if (!laser.IsActive())
                {
                    break;
                }
            }
        }

        for (auto &laser : m_alienLasers)
        {
            if (!laser.IsActive())
            {
                continue;
            }

            if (CheckCollisionRecs(m_spaceship.GetRect(), laser.GetRect()))
            {
                laser.Deactivate();
                LoseLife();
            }

            for (auto &obstacle : m_obstacles)
            {
                auto &blocks = obstacle.Blocks();
                for (auto blockIt = blocks.begin(); blockIt != blocks.end();) {
                    if (CheckCollisionRecs(blockIt->GetRect(), laser.GetRect()))
                    {
                        blockIt = blocks.erase(blockIt);
                        laser.Deactivate();
                        break;
                    }
                    else
                    {
                        ++blockIt;
                    }
                }

                if (!laser.IsActive())
                {
                    break;
                }
            }
        }

        for (const auto &alien : m_aliens)
        {
            if (CheckCollisionRecs(alien.GetRect(), m_spaceship.GetRect()))
            {
                TriggerGameOver();
            }

            if (alien.GetPositionY() + alien.GetHeight() >= static_cast<float>(GetScreenHeight()) - 80.0f)
            {
                TriggerGameOver();
            }
        }
    }

    int CalculateScore(const Alien &alien)
    {
        double now = GetTime();
        if (m_lastAlienDefeat > 0.0 && now - m_lastAlienDefeat < 3.0)
        {
            m_comboCount++;
        }
        else
        {
            m_comboCount = 1;
        }
        m_lastAlienDefeat = now;
        int comboBonus = static_cast<int>(std::round(std::min(5, m_comboCount) * 0.2 * alien.GetScoreValue()));
        return alien.GetScoreValue() + comboBonus + (m_wave - 1) * 10;
    }

    void LoseLife()
    {
        if (m_lives == 0)
        {
            return;
        }
        --m_lives;
        if (m_lives <= 0)
        {
            TriggerGameOver();
        }
        else
        {
            m_spaceship.Reset();
            m_spaceship.SetMoveSpeed(m_currentDifficulty.spaceshipMoveSpeed);
            m_spaceship.SetLaserSpeed(m_currentDifficulty.spaceshipLaserSpeed);
            m_spaceship.SetFireCooldown(m_currentDifficulty.spaceshipFireCooldown);
            m_comboCount = 0;
            m_lastAlienDefeat = 0.0;
        }
    }

    void TriggerGameOver()
    {
        m_state = GameState::GameOver;
        m_spaceship.ClearLasers();
        m_alienLasers.clear();
        m_playerName.clear();
        m_nameSubmitted = false;
    }

    void StartNextWave()
    {
        ++m_wave;
        m_obstacles = CreateObstacles();
        m_aliens = CreateAliens();
        m_alienLasers.clear();
        m_aliensDirection = 1;
        m_lastAlienMoveTime = GetTime();
        m_timeLastAlienFired = GetTime();
        m_comboCount = 0;

        // Slight difficulty ramp inside the selected difficulty.
        m_currentDifficulty.alienMoveInterval = std::max(0.18f, m_currentDifficulty.alienMoveInterval * 0.92f);
        m_currentDifficulty.alienLaserInterval = std::max(0.4f, m_currentDifficulty.alienLaserInterval * 0.95f);
    }

    void DrawTitleScreen() const
    {
        DrawText("Space Invaders Deluxe", 110, 110, 50, YELLOW);
        DrawText("Select a difficulty", 210, 220, 24, LIGHTGRAY);

        for (size_t i = 0; i < m_difficulties.size(); ++i)
        {
            Color color = (i == m_selectedDifficultyIndex) ? WHITE : GRAY;
            DrawRectangle(200 + static_cast<int>(i) * 180, 260, 160, 80, {40, 40, 40, 200});
            DrawRectangleLines(200 + static_cast<int>(i) * 180, 260, 160, 80,
                               (i == m_selectedDifficultyIndex) ? YELLOW : DARKGRAY);
            DrawText(m_difficulties[i].name.c_str(), 225 + static_cast<int>(i) * 180, 288, 28, color);
        }

        DrawText("Press ENTER to launch", 240, 380, 22, WHITE);
        DrawText("Controls: Arrow Keys or A/D to move, SPACE to shoot, P to pause", 70, 430, 20, LIGHTGRAY);

        DrawLeaderboard(70, 480);
    }

    void DrawPlaying() const
    {
        m_spaceship.Draw();
        m_spaceship.DrawLasers();

        for (const auto &obstacle : m_obstacles)
        {
            obstacle.Draw();
        }

        for (const auto &alien : m_aliens)
        {
            alien.Draw();
        }

        for (const auto &laser : m_alienLasers)
        {
            laser.Draw();
        }

        DrawHud();
    }

    void DrawHud() const
    {
        DrawText(TextFormat("Score: %06i", m_score), 20, 20, 24, WHITE);
        DrawText(TextFormat("Wave: %i", m_wave), 20, 52, 20, LIGHTGRAY);
        DrawText(TextFormat("Difficulty: %s", m_currentDifficulty.name.c_str()), 20, 76, 20, LIGHTGRAY);

        const Texture2D &icon = m_spaceship.GetTexture();
        for (int i = 0; i < m_lives; ++i)
        {
            DrawTextureEx(icon, {20.0f + i * (icon.width + 10.0f), 110.0f}, 0.0f, 0.4f, WHITE);
        }

        if (m_comboCount > 1 && m_state == GameState::Playing)
        {
            DrawText(TextFormat("Combo x%i!", m_comboCount), GetScreenWidth() - 200, 20, 22, {255, 140, 0, 255});
        }
    }

    void DrawPausedOverlay() const
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 160});
        DrawText("Paused", GetScreenWidth() / 2 - 60, GetScreenHeight() / 2 - 20, 40, YELLOW);
        DrawText("Press P or ENTER to resume", GetScreenWidth() / 2 - 160, GetScreenHeight() / 2 + 30, 22, WHITE);
    }

    void DrawGameOverOverlay() const
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 160});
        DrawText("Game Over", GetScreenWidth() / 2 - 110, 220, 48, RED);
        DrawText(TextFormat("Final Score: %06i", m_score), GetScreenWidth() / 2 - 160, 280, 28, WHITE);

        if (!m_nameSubmitted)
        {
            DrawText("Enter your name:", GetScreenWidth() / 2 - 150, 340, 24, LIGHTGRAY);
            DrawRectangleLines(GetScreenWidth() / 2 - 180, 370, 360, 40, YELLOW);
            DrawText(m_playerName.c_str(), GetScreenWidth() / 2 - 170, 380, 28, WHITE);
            DrawText("Press ENTER to submit", GetScreenWidth() / 2 - 140, 430, 20, WHITE);
        }
        else
        {
            DrawText("Score saved! Press ENTER to continue", GetScreenWidth() / 2 - 210, 360, 22, WHITE);
        }

        DrawLeaderboard(GetScreenWidth() / 2 - 170, 470);
    }

    void DrawLeaderboard(int x, int y) const
    {
        DrawText("Leaderboard", x, y, 26, YELLOW);
        int lineHeight = 24;
        int offset = y + 36;
        const auto &entries = m_leaderboard.GetEntries();
        if (entries.empty())
        {
            DrawText("No scores yet. Be the first!", x, offset, 20, LIGHTGRAY);
            return;
        }

        for (size_t i = 0; i < entries.size(); ++i)
        {
            const auto &entry = entries[i];
            DrawText(TextFormat("%d. %s", static_cast<int>(i) + 1, entry.name.c_str()), x,
                     offset + static_cast<int>(i) * lineHeight, 20, WHITE);
            DrawText(TextFormat("%s - %i", entry.difficulty.c_str(), entry.score), x + 220,
                     offset + static_cast<int>(i) * lineHeight, 20, LIGHTGRAY);
        }
    }

    Spaceship m_spaceship;
    std::vector<Obstacle> m_obstacles;
    std::vector<Alien> m_aliens;
    std::vector<Laser> m_alienLasers;

    Leaderboard m_leaderboard;

    std::array<DifficultySettings, 3> m_difficulties;
    size_t m_selectedDifficultyIndex = 0;
    DifficultySettings m_currentDifficulty{};

    int m_lives = 3;
    int m_score = 0;
    int m_wave = 1;
    int m_aliensDirection = 1;

    double m_lastAlienMoveTime = 0.0;
    double m_timeLastAlienFired = 0.0;

    GameState m_state = GameState::Title;

    std::string m_playerName;
    bool m_nameSubmitted = false;

    int m_comboCount = 0;
    double m_lastAlienDefeat = 0.0;
};

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(kWindowWidth, kWindowHeight, "Space Invaders Deluxe");
    SetTargetFPS(60);

    SearchAndSetResourceDir("resources");

    Game game;

    while (!WindowShouldClose())
    {
        game.HandleInput();
        game.Update();

        BeginDrawing();
        ClearBackground(GreyBackground());
        game.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
