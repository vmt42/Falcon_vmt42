#include "Level.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include "../../Errors/Errors.h"
#include "../../ResourceManager/ResourceManager.h"




Level::Level(const std::string &filePath)
{
    std::ifstream file;
    file.open(filePath);
    if (file.fail())
        Falcon::fatalError("Failed to open" + filePath);

    std::string empty;

    while(std::getline(file, empty))
    {
        m_level.push_back(empty);
    }
}

void Level::init()
{
    glm::vec4 uvVec(0.0f, 0.0f, 1.0f, 1.0f);

    m_spriteBatch.init();
    m_spriteBatch.begin();

    std::reverse(m_level.begin(), m_level.end());

    for (int i = 0; i < m_level.size(); i++)
    {
        for (int j = 0; j < m_level[i].size(); j++)
        {
            char tile = m_level[i][j];

            glm::vec4 posVec(j * TILE_HEIGHT, i * TILE_WIDTH, TILE_WIDTH, TILE_HEIGHT);
            switch (tile)
            {
                case '.':
                    m_spriteBatch.draw(posVec, uvVec, Falcon::ResourceManager::getTexture("images/Textures/floor_tile.png").id,
                                       0.0f, Falcon::Color{255, 255, 255, 255});
                    break;
                case 'W':
                    m_spriteBatch.draw(posVec, uvVec, Falcon::ResourceManager::getTexture("images/Textures/wall_tile.png").id,
                                       0.0f, Falcon::Color{255, 255, 255, 255});
                    break;
                case 'C':
                    m_spriteBatch.draw(posVec, uvVec, Falcon::ResourceManager::getTexture("images/Textures/ceiling_tile.png").id,
                                       0.0f, Falcon::Color{255, 255, 255, 255});
                    break;
                case '@': //A player position
                    m_level[i][j] = '.';
                    m_playerStartPos.x = j * TILE_HEIGHT;
                    m_playerStartPos.y = i * TILE_WIDTH;
                    m_spriteBatch.draw(posVec, uvVec, Falcon::ResourceManager::getTexture("images/Textures/floor_tile.png").id,
                                       0.0f, Falcon::Color{255, 255, 255, 255});
                    break;
                case '%': //A human position
                    m_level[i][j] = '.';
                    m_humanStartPos.emplace_back(j * TILE_HEIGHT, i * TILE_WIDTH);
                    m_spriteBatch.draw(posVec, uvVec, Falcon::ResourceManager::getTexture("images/Textures/floor_tile.png").id,
                                       0.0f, Falcon::Color{255, 255, 255, 255});
                    break;
                case '&': //An enemy position
                    m_level[i][j] = '.';
                    m_enemyStartPos.emplace_back(j * TILE_HEIGHT, i * TILE_WIDTH);
                    m_spriteBatch.draw(posVec, uvVec, Falcon::ResourceManager::getTexture("images/Textures/floor_tile.png").id,
                                       0.0f, Falcon::Color{255, 255, 255, 255});
                    break;
                default:
                    std::printf("Unexpected symbol %c at %d, %d", tile, i, j);
                    break;
            }
        }
    }

    m_spriteBatch.end();
}

void Level::draw()
{
    m_spriteBatch.renderBatch();
}


