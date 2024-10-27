#ifndef TRACK_HPP
#define TRACK_HPP
#include "object.hpp"

class Track final : public Object
{
public:
	Track() : Object("assets/models/track.obj", {0, 0, 0}, 2048, 0, 2048, 0, 0, 0, 2) {}
};

#endif // TRACK_HPP
