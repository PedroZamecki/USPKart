#ifndef CAMERA_STATE_HPP
#define CAMERA_STATE_HPP

#include "object/character/player.hpp"

class Camera;
class Position;

class CameraState
{
	bool inverted = false;

public:
	[[nodiscard]] bool getInverted() const { return inverted; }
	explicit CameraState(const bool inverted = false) : inverted(inverted) {}
	virtual ~CameraState() = default;
	virtual void update(Camera *camera, const Player *player) = 0;
	void setInverted(const bool value) { inverted = value; }
};

class CameraBehindState final : public CameraState
{
public:
	explicit CameraBehindState(const bool inverted = false) : CameraState(inverted) {}
	void update(Camera *camera, const Player *player) override;
};

class CameraSideState final : public CameraState
{
public:
	explicit CameraSideState(const bool inverted = false) : CameraState(inverted) {}
	void update(Camera *camera, const Player *player) override;
};


#endif // CAMERA_STATE_HPP
