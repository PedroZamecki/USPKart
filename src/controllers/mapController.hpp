#ifndef MAP_CONTROLLER_HPP
#define MAP_CONTROLLER_HPP

#include <string>
#include <vector>

class Object;
typedef std::pair<std::pair<int, int>, std::pair<int, int>> Checkpoint ;

class MapController
{
	void loadPGM(const std::string &pgmFile);
	std::vector<std::vector<int>> map;
	std::vector<Checkpoint> checkpoints;

public:
	MapController(const std::string &ppmFile,
				  const std::vector<Checkpoint> &checkpoints);
	std::vector<std::pair<int, int>> findPath(int x, int y, std::vector<std::vector<int>> map, int checkpointIdx);
	void fillPolygon(std::vector<std::vector<int>> *weightedMap, Object *object, int code);
	void fillLine(std::vector<std::vector<int>> *weightedMap, Checkpoint checkpoint, int code);
	std::vector<std::vector<int>> getWeightedMap(std::vector<Object *> trackObjects, Object *observer, int checkpointIdx);
	void saveModifiedPPM(const std::string &outputFilePath, int x, int y,
						 int checkpointIdx, std::vector<Object *> trackObjects, Object *observer);
	std::vector<Checkpoint> getCheckpoints() { return checkpoints; }
};

#endif // MAP_CONTROLLER_HPP
