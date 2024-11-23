#ifndef MAP_CONTROLLER_HPP
#define MAP_CONTROLLER_HPP

#include <string>
#include <vector>

class Object;

class MapController
{
	void loadPGM(const std::string &pgmFile);
	std::vector<std::vector<int>> map;
	std::vector<std::string> trackObjects;
	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> checkpoints;

public:
	MapController(const std::string &ppmFile,
				  const std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> &checkpoints);
	void loadTrackObjects(const std::vector<std::string> &objects);
	std::vector<std::pair<int, int>> findPath(int x, int y, int checkpointIdx, const std::vector<Object *> &objects);
	std::vector<std::vector<int>> getWeightedMap(const std::vector<Object *> &objects);
	void saveModifiedPPM(const std::string &outputFilePath, const std::vector<Object *> &objects, int x, int y,
						 int checkpointIdx);
};

#endif // MAP_CONTROLLER_HPP
