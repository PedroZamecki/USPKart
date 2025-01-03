#ifndef MAP_CONTROLLER_HPP
#define MAP_CONTROLLER_HPP

#include <mutex>
#include <string>
#include <vector>

class Object;
typedef std::pair<std::pair<int, int>, std::pair<int, int>> Checkpoint;

class MapController
{
	static std::mutex mutex;
	static MapController *instance;

	MapController() = default;

	void loadPGM(const std::string pgmFile);
	std::vector<std::vector<int>> map;
	std::vector<Checkpoint> checkpoints;
	std::vector<Object *> *objects;
	std::vector<std::vector<double>> pointsMap;
	std::pair<std::vector<std::pair<int, int>>, double> aStarSearch(int startX, int startY, int goalX, int goalY, Object *filteredObject) const;

public:
	static MapController *getInstance();
	void configure(const std::string &ppmFile, const std::vector<Checkpoint> &checkpoints);
	virtual ~MapController() = default;
	int encodeMapCoord(float num, int maxCoord = 0) const;
	float decodeMapCoord(int num, int maxCoord = 0) const;
	std::vector<Object *> objectsTransformer(std::vector<Object *> objects);
	double getCost(int x, int y, const std::vector<std::vector<int>> weightedMap) const;
	std::pair<std::vector<std::pair<int, int>>, double> findPath(int x, int y, int checkpointIdx,
																 Object *filteredObject) const;
	void fillPolygon(std::vector<std::vector<int>> &weightedMap, const Object object, int code) const;
	void fillLine(std::vector<std::vector<int>> &weightedMap, const Checkpoint checkpoint, int code) const;
	std::vector<std::vector<int>> getWeightedMap(Object *observer, int checkpointIdx) const;
	void saveMapPPM(const std::string &outputFilePath, const std::vector<std::vector<int>> &map) const;
	void saveObserverViewPPM(const std::string outputFilePath, int x, int y, int checkpointIdx, Object *observer) const;
	std::vector<Checkpoint> getCheckpoints() const { return checkpoints; }
	void setObjects(std::vector<Object *> &objects) { this->objects = &objects; }
};

#endif // MAP_CONTROLLER_HPP
