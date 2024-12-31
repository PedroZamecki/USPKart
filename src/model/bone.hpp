#ifndef BONE_HPP
#define BONE_HPP

#include <assimp/scene.h>
#include <vector>
#include "helper.hpp"

class Bone
{
	static float getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

	[[nodiscard]] glm::mat4 interpolatePosition(float animationTime) const;

	[[nodiscard]] glm::mat4 interpolateRotation(float animationTime) const;

	[[nodiscard]] glm::mat4 interpolateScaling(float animationTime) const;

	std::vector<KeyPosition> m_Positions;
	std::vector<KeyRotation> m_Rotations;
	std::vector<KeyScale> m_Scales;
	int m_NumPositions;
	int m_NumRotations;
	int m_NumScalings;

	glm::mat4 m_LocalTransform;
	std::string m_Name;
	int m_ID;

public:
	Bone(std::string &name, int ID, const aiNodeAnim *channel);

	void update(float animationTime);
	[[nodiscard]] glm::mat4 getLocalTransform() const { return m_LocalTransform; }
	[[nodiscard]] std::string getBoneName() const { return m_Name; }
	[[nodiscard]] int getBoneID() const { return m_ID; }

	[[nodiscard]] int getPositionIndex(float animationTime) const;

	[[nodiscard]] int getRotationIndex(float animationTime) const;

	[[nodiscard]] int getScaleIndex(float animationTime) const;
};

#endif // BONE_HPP
