#include "Camera2D.h"

namespace Falcon
{
    void Camera2D::init(int screenWidth, int screenHeight)
    {
        m_screenWidth = screenWidth;
        m_screenHeight = screenHeight;
        m_orthoMatrix = glm::ortho(0.0f, static_cast<float>(m_screenWidth), 0.0f, static_cast<float>(m_screenHeight));
    }

    void Camera2D::update()
    {
        if (m_shouldUpdate)
        {
            //Camera translation
            glm::vec3 translate(-m_position.x + m_screenWidth / 2, -m_position.y + m_screenHeight / 2, 0.0f);
            m_cameraMatrix = glm::translate(m_orthoMatrix, translate);

            //Camera scaling
            glm::vec3 scale(m_scale, m_scale, 0.0f);
            m_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * m_cameraMatrix;

            m_shouldUpdate = false;
        }
    }

    glm::vec2 Camera2D::screenToWorld(glm::vec2 screenCoords)
    {
        // Invert Y dir
        screenCoords.y = m_screenHeight - screenCoords.y;
        // Make 0 as center
        screenCoords -= glm::vec2(m_screenWidth / 2, m_screenHeight / 2);
        // Scale coords
        screenCoords /= m_scale;
        // Translate with camera position
        screenCoords += m_position;

        return screenCoords;
    }

    bool Camera2D::isBoxVisible(const glm::vec2 &position, const glm::vec2 &dimensions)
    {
        glm::vec2 scaledScreen = glm::vec2(m_screenWidth, m_screenHeight) / m_scale;

        const float MIN_DISTANCE_X = dimensions.x / 2.0f + scaledScreen.x / 2.0f;
        const float MIN_DISTANCE_Y = dimensions.y / 2.0f + scaledScreen.y / 2.0f;

        //Input position
        glm::vec2 centerPos = position + dimensions / 2.0f;
        glm::vec2 centerCameraPos = m_position;

        //Vector from input to the camera
        glm::vec2 distVec = centerPos - centerCameraPos ;

        float x_depth = MIN_DISTANCE_X - std::abs(distVec.x);
        float y_depth = MIN_DISTANCE_Y - std::abs(distVec.y);


        if (x_depth > 0 && y_depth > 0)
        {
            // Was a collision
            return true;
        }

        return false;
    }


}