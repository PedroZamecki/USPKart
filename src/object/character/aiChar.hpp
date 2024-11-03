#ifndef AI_CHAR_HPP
#define AI_CHAR_HPP

#include "character.hpp"

class AIChar final : public Character
{
    double actionTime = 0;
public:
    AIChar(const Position &pos = {0, 0.5, 0}, glm::vec3 angle = glm::vec3{0}, float scale = 1): Character(pos, angle, scale) {
    }

    void draw(const Shader &shader, const float deltaTime, const glm::mat4 baseModel, const bool drawBoxes,
			  const Shader &boxShader) override
	{
		actionTime += deltaTime;
        if (actionTime > 3) {
            actionTime -= 3;
            steerState = static_cast<CharacterSteeringState>(rand() % 3 - 1);
            acceleratingState = static_cast<CharacterAcceleratingState>(0);
            breakingState = static_cast<CharacterBreakingState>(rand() % 2);
        }
		Character::draw(shader, deltaTime, baseModel, drawBoxes, boxShader);
	}
};

#endif // AI_CHAR_HPP
