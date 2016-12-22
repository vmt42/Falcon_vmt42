#ifndef FALCONLATINUM_LEVEL_H
#define FALCONLATINUM_LEVEL_H


#include <vector>
#include <string>
#include "../../2D/SpriteBatch/SpriteBatch.h"

const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;
class Level
{
public:
    Level(const std::string& filePath);
    ~Level() {};

    void init();
    void draw();

    unsigned long getWidth() const { return m_level[4].length(); };
    unsigned long getHeight() const { return m_level.size(); };
    const std::vector<std::string>& getLevelData() const { return m_level; };
    glm::vec2 getPlayerStartPos() const { return m_playerStartPos; };
    const std::vector<glm::vec2>& getEnemyStartPos() const { return m_enemyStartPos; };
    const std::vector<glm::vec2>& getHumanStartPos() const { return m_humanStartPos; };

private:
    int m_humans;

    std::vector<std::string> m_level;
    Falcon::SpriteBatch m_spriteBatch;
    glm::vec2 m_playerStartPos;
    std::vector<glm::vec2> m_enemyStartPos;
    std::vector<glm::vec2> m_humanStartPos;
};


#endif
