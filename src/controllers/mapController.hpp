#ifndef MAP_CONTROLLER_HPP
#define MAP_CONTROLLER_HPP

#include <string>
#include <vector>

class Object;
typedef std::pair<std::pair<int, int>, std::pair<int, int>> Checkpoint ;

class MapController
{
	void loadPGM(const std::string pgmFile);
	std::vector<std::vector<int>> map;
	std::vector<Checkpoint> checkpoints;

public:
	MapController(const std::string ppmFile, const std::vector<Checkpoint> checkpoints);
	int coordTransform(float num, int maxCoord = 0) const;
	std::vector<Object *> objectsTransformer(std::vector<Object *> objects);
	std::pair<std::vector<std::pair<int, int>>, double> findPath(int x, int y, std::vector<std::vector<int>> map,
																 int checkpointIdx) const;
	void fillPolygon(std::vector<std::vector<int>> &weightedMap, const Object object, int code) const;
	void fillLine(std::vector<std::vector<int>> &weightedMap, const Checkpoint checkpoint, int code) const;
	std::vector<std::vector<int>> getWeightedMap(const std::vector<Object *> trackObjects, const Object *observer, int checkpointIdx) const;
	void saveModifiedPPM(const std::string outputFilePath, int x, int y,
						 int checkpointIdx, const std::vector<Object *> trackObjects, const Object *observer) const;
	std::vector<Checkpoint> getCheckpoints() const { return checkpoints; }
};

#endif // MAP_CONTROLLER_HPP
