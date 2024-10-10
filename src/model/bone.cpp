//
// Created by pedro on 09/10/24.
//

#include "bone.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

Bone::Bone(std::string &name, int ID, const aiNodeAnim *channel) : m_LocalTransform(1.0f), m_Name(name), m_ID(ID)
{
	m_NumPositions = channel->mNumPositionKeys;

	for (int positionIndex = 0; positionIndex < m_NumPositions; ++positionIndex)
	{
		aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
		const float timeStamp = channel->mPositionKeys[positionIndex].mTime;
		KeyPosition data{};
		data.position = getGLMVec(aiPosition);
		data.timeStamp = timeStamp;
		m_Positions.push_back(data);
	}

	m_NumRotations = channel->mNumRotationKeys;
	for (int rotationIndex = 0; rotationIndex < m_NumRotations; ++rotationIndex)
	{
		aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
		float timeStamp = channel->mRotationKeys[rotationIndex].mTime;
		KeyRotation data{};
		data.orientation = getGLMQuat(aiOrientation);
		data.timeStamp = timeStamp;
		m_Rotations.push_back(data);
	}

	m_NumScalings = channel->mNumScalingKeys;
	for (int keyIndex = 0; keyIndex < m_NumScalings; ++keyIndex)
	{
		aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
		const float timeStamp = channel->mScalingKeys[keyIndex].mTime;
		KeyScale data{};
		data.scale = getGLMVec(scale);
		data.timeStamp = timeStamp;
		m_Scales.push_back(data);
	}
}

void Bone::update(const float animationTime)
{
	const glm::mat4 translation = interpolatePosition(animationTime);
	const glm::mat4 rotation = interpolateRotation(animationTime);
	const glm::mat4 scale = interpolateScaling(animationTime);
	m_LocalTransform = translation * rotation * scale;
}

int Bone::getPositionIndex(const float animationTime) const
{
	for (int index = 0; index < m_NumPositions - 1; ++index)
	{
		if (animationTime < m_Positions[index + 1].timeStamp)
			return index;
	}
	assert(0);
}

int Bone::getRotationIndex(const float animationTime) const
{
	for (int index = 0; index < m_NumRotations - 1; ++index)
	{
		if (animationTime < m_Rotations[index + 1].timeStamp)
			return index;
	}
	assert(0);
}

int Bone::getScaleIndex(const float animationTime) const
{
	for (int index = 0; index < m_NumScalings - 1; ++index)
	{
		if (animationTime < m_Scales[index + 1].timeStamp)
			return index;
	}
	assert(0);
}

float Bone::getScaleFactor(const float lastTimeStamp, const float nextTimeStamp, const float animationTime)
{
	float scaleFactor = 0.0f;
	const float midWayLength = animationTime - lastTimeStamp;
	const float framesDiff = nextTimeStamp - lastTimeStamp;
	scaleFactor = midWayLength / framesDiff;
	return scaleFactor;
}

glm::mat4 Bone::interpolatePosition(const float animationTime) const
{
	if (1 == m_NumPositions)
		return translate(glm::mat4(1.0f), m_Positions[0].position);

	const int p0Index = getPositionIndex(animationTime);
	const int p1Index = p0Index + 1;
	const float scaleFactor =
		getScaleFactor(m_Positions[p0Index].timeStamp, m_Positions[p1Index].timeStamp, animationTime);
	const glm::vec3 finalPosition = mix(m_Positions[p0Index].position, m_Positions[p1Index].position, scaleFactor);
	return translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Bone::interpolateRotation(const float animationTime) const
{
	if (1 == m_NumRotations)
	{
		const auto rotation = normalize(m_Rotations[0].orientation);
		return toMat4(rotation);
	}

	const int p0Index = getRotationIndex(animationTime);
	const int p1Index = p0Index + 1;
	const float scaleFactor =
		getScaleFactor(m_Rotations[p0Index].timeStamp, m_Rotations[p1Index].timeStamp, animationTime);
	glm::quat finalRotation = slerp(m_Rotations[p0Index].orientation, m_Rotations[p1Index].orientation, scaleFactor);
	finalRotation = normalize(finalRotation);
	return toMat4(finalRotation);
}

glm::mat4 Bone::interpolateScaling(const float animationTime) const
{
	if (1 == m_NumScalings)
		return scale(glm::mat4(1.0f), m_Scales[0].scale);

	const int p0Index = getScaleIndex(animationTime);
	const int p1Index = p0Index + 1;
	const float scaleFactor = getScaleFactor(m_Scales[p0Index].timeStamp, m_Scales[p1Index].timeStamp, animationTime);
	const glm::vec3 finalScale = mix(m_Scales[p0Index].scale, m_Scales[p1Index].scale, scaleFactor);
	return scale(glm::mat4(1.0f), finalScale);
}
