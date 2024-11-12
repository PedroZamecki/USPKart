#ifndef MAP_CONTROLLER_HPP
#define MAP_CONTROLLER_HPP

#include <string>
#include <vector>

class Object;

class MapController
{
	void loadPPM(const std::string &ppmFilePath);
	std::vector<std::vector<int>> map;
	std::vector<std::string> trackObjects;

public:
	MapController(const std::string &ppmFilePath);
	void loadTrackObjects(const std::vector<std::string> &objects);
	std::vector<std::pair<int, int>> findShortestPath(int startX, int startY, int endX, int endY);
	std::vector<std::vector<int>> getWeightedMap(const std::vector<Object *> &objects);
	void saveModifiedPPM(const std::string &outputFilePath, const std::vector<Object *> &objects);
};

#endif // MAP_CONTROLLER_HPP
