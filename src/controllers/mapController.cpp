#include "mapController.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <memory>
#include <atomic>
#include <queue>
#include <sstream>
#include <thread>
#include "object/object.hpp"
#include "utils/logger.hpp"

// Grayscale color
#define ROAD 0
#define BRIDGE 50
#define ROADSIDE 70
#define OFF_ROAD 170
#define WATER 200
#define WALL 255

// Colors
#define ROAD_COLOR {ROAD, ROAD, ROAD}
#define BRIDGE_COLOR {BRIDGE, BRIDGE, BRIDGE}
#define ROADSIDE_COLOR {ROADSIDE, ROADSIDE, ROADSIDE}
#define OFF_ROAD_COLOR {OFF_ROAD, OFF_ROAD, OFF_ROAD}
#define WATER_COLOR {WATER, WATER, WATER}
#define WALL_COLOR {WALL, WALL, WALL}
#define CHECKPOINT_COLOR {0, 0, 255}
#define CURRENT_CHECKPOINT_COLOR {255, 0, 255}
#define OBJECT_COLOR {255, 0, 0}
#define OBSERVER_COLOR {255, 255, 0}
#define PATH_COLOR {0, 255, 0}

// Code
#define ROAD_CODE 0
#define BRIDGE_CODE 1
#define ROADSIDE_CODE 2
#define OFF_ROAD_CODE 3
#define WATER_CODE 4
#define WALL_CODE 5
#define CHECKPOINT_CODE 6
#define CURRENT_CHECKPOINT_CODE 7
#define OBJECT_CODE 8
#define OBSERVER_CODE 9
#define PATH_CODE 10
#define UKNOWN_CODE 11

// Cost
#define NO_COST 0.0
#define MIN_COST 1.0
#define ROAD_COST 1.0
#define BRIDGE_COST 1.3
#define ROADSIDE_COST 1.5
#define OFF_ROAD_COST 2.0
#define WATER_COST 5.0
#define WALL_COST 1000000
#define MAX_COST std::numeric_limits<double>::infinity()

MapController *MapController::instance = nullptr;
std::mutex MapController::mutex;

MapController *MapController::getInstance()
{
	std::lock_guard lock(mutex);
	if (instance == nullptr)
	{
		instance = new MapController();
	}
	return instance;
}

void MapController::configure(const std::string &ppmFilePath, const std::vector<Checkpoint> &checkpoints)
{
	this->checkpoints = checkpoints;
	loadPGM(ppmFilePath);
}

int MapController::encodeMapCoord(float num, int maxCoord) const
{
	if (map.empty())
	{
		return 0;
	}

	if (maxCoord == 0)
	{
		maxCoord = map[0].size();
	}
	return std::max(std::min(static_cast<int>(num * 4 + maxCoord / 2), maxCoord), 0);
}

float MapController::decodeMapCoord(int num, int maxCoord) const
{
	if (maxCoord == 0)
	{
		maxCoord = map[0].size();
	}
	return (num - maxCoord / 2) / 4;
}

std::vector<Object *> MapController::objectsTransformer(std::vector<Object *> objects)
{
	std::vector<Object *> transformedObjects;
	for (auto object : objects)
	{
		auto pos = object->getPos();
		auto newPos = Position(encodeMapCoord(pos.x), pos.y, encodeMapCoord(pos.z));
		object->move(newPos - pos);
		transformedObjects.push_back(object);
	}

	return transformedObjects;
}

void MapController::loadPGM(const std::string ppmFilePath)
{
	std::ifstream file(ppmFilePath);
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open PGM file");
	}

	std::string line;
	std::getline(file, line);
	if (line != "P2")
	{
		throw std::runtime_error("Invalid PGM file format");
	}

	int width, height;
	file >> width >> height;
	map.resize(height, std::vector<int>(width, 0));

	int maxColor;
	file >> maxColor;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int color;
			file >> color;
			switch (color)
			{
			case ROAD:
				map[y][x] = ROAD_CODE;
				break;
			case BRIDGE:
				map[y][x] = BRIDGE_CODE;
				break;
			case ROADSIDE:
				map[y][x] = ROADSIDE_CODE;
				break;
			case OFF_ROAD:
				map[y][x] = OFF_ROAD_CODE;
				break;
			case WATER:
				map[y][x] = WATER_CODE;
				break;
			case WALL:
				map[y][x] = WALL_CODE;
				break;
			default:
				map[y][x] = UKNOWN_CODE;
				break;
			}
		}
	}

	file.close();
}

struct Node
{
	int x, y;
	double cost, heuristic;
	std::unique_ptr<Node> parent;

	Node(int x, int y, double cost, double heuristic, std::unique_ptr<Node> parent = nullptr) :
		x(x), y(y), cost(cost), heuristic(heuristic), parent(std::move(parent))
	{
	}

	double totalCost() const { return cost + heuristic; }

	bool operator>(const Node &other) const { return totalCost() > other.totalCost(); }
};


double MapController::getCost(int x, int y, const std::vector<std::vector<int>> weightedMap) const
{
	if (y < 0 || y >= weightedMap.size() || x < 0 || x >= weightedMap[0].size())
		return WALL_COST; // Verificação de limites
	switch (weightedMap[y][x])
	{
	case ROAD_CODE:
		return ROAD_COST;
	case BRIDGE_CODE:
		return BRIDGE_COST;
	case ROADSIDE_CODE:
		return ROADSIDE_COST;
	case OFF_ROAD_CODE:
		return OFF_ROAD_COST;
	case WATER_CODE:
		return WATER_COST;
	case WALL_CODE:
		return WALL_COST;
	case OBSERVER_CODE:
		return getCost(x, y, map);
	case OBJECT_CODE:
		return MAX_COST;
	case CURRENT_CHECKPOINT_CODE:
		return NO_COST;
	default:
		return WALL_COST;
	}
}

extern std::atomic<bool> running;

std::pair<std::vector<std::pair<int, int>>, double> MapController::aStarSearch(int startX, int startY, int goalX, int goalY, Object *filteredObject) const
{
	auto heuristic = [goalX, goalY](int x, int y)
	{ return std::sqrt(std::pow(goalX - x, 2) + std::pow(goalY - y, 2)); };

	auto weightedMap = getWeightedMap(filteredObject, checkpoints.size() - 1);

	std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;
	std::vector<std::vector<bool>> closedSet(weightedMap.size(), std::vector<bool>(weightedMap[0].size(), false));

	openSet.emplace(startX, startY, 0, heuristic(startX, startY));

	while (!openSet.empty())
	{
		Node current = std::move(const_cast<Node &>(openSet.top()));
		openSet.pop();

		if (current.y < 0 || current.y >= weightedMap.size() || current.x < 0 || current.x >= weightedMap[0].size())
			continue;

		if (current.x == goalX && current.y == goalY)
		{
			std::vector<std::pair<int, int>> path;
			for (Node *node = &current; node != nullptr; node = node->parent.get())
			{
				path.emplace_back(node->x, node->y);
			}
			std::reverse(path.begin(), path.end());
			return {path, current.cost};
		}

		if (closedSet[current.y][current.x])
			continue;

		closedSet[current.y][current.x] = true;

		const std::vector<std::pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
		for (const auto &dir : directions)
		{
			int newX = current.x + dir.first;
			int newY = current.y + dir.second;

			if (newX >= 0 && newX < weightedMap[0].size() && newY >= 0 && newY < weightedMap.size() &&
				!closedSet[newY][newX])
			{
				double newCost = current.cost + getCost(newX, newY, weightedMap);

				if (newCost == 0)
				{
					std::vector<std::pair<int, int>> path;
					for (Node *node = &current; node != nullptr; node = node->parent.get())
					{
						path.emplace_back(node->x, node->y);
					}
					path.emplace_back(newX, newY);
					std::reverse(path.begin(), path.end());
					return {path, current.cost};
				}

				openSet.emplace(newX, newY, newCost, heuristic(newX, newY), std::make_unique<Node>(std::move(current)));
			}
		}
	}

	return {{}, std::numeric_limits<double>::infinity()};
}

std::pair<std::vector<std::pair<int, int>>, double> MapController::findPath(int x, int y, int checkpointIdx, Object *filteredObject) const
{
	auto checkpoint = checkpoints[checkpointIdx];
	int goalX = (checkpoint.first.first + checkpoint.second.first) / 2;
	int goalY = (checkpoint.first.second + checkpoint.second.second) / 2;

	return aStarSearch(x, y, goalX, goalY, filteredObject);
}

void MapController::fillPolygon(std::vector<std::vector<int>> &weightedMap, const Object object, int code) const
{
	int width = map[0].size();
	int height = map.size();
	auto box = object.getBox();
	auto projectedVertices = box.get2DProjectedVertices();
	std::vector<std::pair<int, int>> points;
	for (const auto &vertex : projectedVertices)
	{
		int x = encodeMapCoord(vertex.x, width);
		int y = encodeMapCoord(vertex.y, height);
		points.emplace_back(x, y);
	}

	// Find the bounding box of the polygon
	int minX =
		std::min_element(points.begin(), points.end(), [](const auto &a, const auto &b) { return a.first < b.first; })
			->first;
	int maxX =
		std::max_element(points.begin(), points.end(), [](const auto &a, const auto &b) { return a.first < b.first; })
			->first;
	int minY =
		std::min_element(points.begin(), points.end(), [](const auto &a, const auto &b) { return a.second < b.second; })
			->second;
	int maxY =
		std::max_element(points.begin(), points.end(), [](const auto &a, const auto &b) { return a.second < b.second; })
			->second;

	// Scanline fill algorithm
	for (int y = minY; y <= maxY; y++)
	{
		std::vector<int> intersections;
		for (size_t i = 0; i < points.size(); i++)
		{
			auto p1 = points[i];
			auto p2 = points[(i + 1) % points.size()];
			if ((p1.second <= y && p2.second > y) || (p1.second > y && p2.second <= y))
			{
				int x = p1.first + (y - p1.second) * (p2.first - p1.first) / (p2.second - p1.second);
				intersections.push_back(x);
			}
		}
		std::sort(intersections.begin(), intersections.end());
		for (size_t i = 0; i < intersections.size(); i += 2)
		{
			for (int x = intersections[i]; x <= intersections[i + 1]; x++)
			{
				if (x >= 0 && x < width && y >= 0 && y < height)
				{
					weightedMap[y][x] = code;
				}
			}
		}
	}
}

void MapController::fillLine(std::vector<std::vector<int>> &weightedMap, const Checkpoint checkpoint, int code) const
{
	int width = map[0].size();
	int height = map.size();

	int x1 = checkpoint.first.first;
	int y1 = checkpoint.first.second;
	int x2 = checkpoint.second.first;
	int y2 = checkpoint.second.second;

	int dx = std::abs(x2 - x1);
	int dy = std::abs(y2 - y1);
	int sx = (x1 < x2) ? 1 : -1;
	int sy = (y1 < y2) ? 1 : -1;
	int err = dx - dy;

	while (true)
	{
		if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height)
		{
			weightedMap[y1][x1] = code;
		}
		if (x1 == x2 && y1 == y2)
			break;
		int e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			x1 += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			y1 += sy;
		}
	}
}

std::vector<std::vector<int>> MapController::getWeightedMap(Object *observer, int checkpointIdx) const
{
	int width = map[0].size();
	int height = map.size();
	std::vector<std::vector<int>> weightedMap = map;

	for (const auto &object : *objects)
	{
		if (object != observer)
			fillPolygon(weightedMap, *object, OBJECT_CODE);
	}

	for (const auto &checkpoint : checkpoints)
	{
		if (checkpoint != checkpoints[checkpointIdx])
			fillLine(weightedMap, checkpoint, CHECKPOINT_CODE);
	}

	if (observer != nullptr)
		fillPolygon(weightedMap, *observer, OBSERVER_CODE);
	fillLine(weightedMap, checkpoints[checkpointIdx], CURRENT_CHECKPOINT_CODE);

	return weightedMap;
}

void MapController::saveMapPPM(const std::string &outputFilePath, const std::vector<std::vector<int>> &map) const
{
	int width = map[0].size();
	int height = map.size();

	std::ofstream file(outputFilePath, std::ios::binary);
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open output PPM file");
	}

	// Write the PPM header
	file << "P6\n";
	file << width << " " << height << "\n";
	file << "255\n";

	// Write the pixel data
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			std::tuple<unsigned char, unsigned char, unsigned char> color;
			switch (map[y][x])
			{
			case ROAD_CODE:
				color = ROAD_COLOR;
				break;
			case BRIDGE_CODE:
				color = BRIDGE_COLOR;
				break;
			case ROADSIDE_CODE:
				color = ROADSIDE_COLOR;
				break;
			case OFF_ROAD_CODE:
				color = OFF_ROAD_COLOR;
				break;
			case WATER_CODE:
				color = WATER_COLOR;
				break;
			case WALL_CODE:
				color = WALL_COLOR;
				break;
			case CHECKPOINT_CODE:
				color = CHECKPOINT_COLOR;
				break;
			case CURRENT_CHECKPOINT_CODE:
				color = CURRENT_CHECKPOINT_COLOR;
				break;
			case OBJECT_CODE:
				color = OBJECT_COLOR;
				break;
			case OBSERVER_CODE:
				color = OBSERVER_COLOR;
				break;
			case PATH_CODE:
				color = PATH_COLOR;
				break;
			default:
				color = ROAD_COLOR;
				break;
			}
			file << std::get<0>(color) << std::get<1>(color) << std::get<2>(color);
		}
	}

	file.close();
}

void MapController::saveObserverViewPPM(const std::string outputFilePath, int x, int y, int checkpointIdx, Object *observer) const
{
	// Create a snapshot of the map and objects
	auto mapSnapshot = map;
	std::vector<Object *> objectsSnapshot = *objects;

	const int convertedX = encodeMapCoord(x, map[0].size());
	const int convertedY = encodeMapCoord(y, map.size());

	// Run the save operation in a separate thread
	std::thread saveThread(
		[this, mapSnapshot, objectsSnapshot, outputFilePath, convertedX, convertedY, checkpointIdx, observer]()
		{
			auto weightedMap = getWeightedMap(observer, checkpointIdx);

			int width = weightedMap[0].size();
			int height = weightedMap.size();

			// Create a color map from the original PPM
			std::vector<std::vector<std::tuple<unsigned char, unsigned char, unsigned char>>> colorMap(
				height, std::vector<std::tuple<unsigned char, unsigned char, unsigned char>>(width));
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					switch (weightedMap[y][x])
					{
					case ROAD_CODE:
						colorMap[y][x] = ROAD_COLOR;
						break;
					case BRIDGE_CODE:
						colorMap[y][x] = BRIDGE_COLOR;
						break;
					case ROADSIDE_CODE:
						colorMap[y][x] = ROADSIDE_COLOR;
						break;
					case OFF_ROAD_CODE:
						colorMap[y][x] = OFF_ROAD_COLOR;
						break;
					case WATER_CODE:
						colorMap[y][x] = WATER_COLOR;
						break;
					case WALL_CODE:
						colorMap[y][x] = WALL_COLOR;
						break;
					case CHECKPOINT_CODE:
						colorMap[y][x] = CHECKPOINT_COLOR;
						break;
					case CURRENT_CHECKPOINT_CODE:
						colorMap[y][x] = CURRENT_CHECKPOINT_COLOR;
						break;
					case OBJECT_CODE:
						colorMap[y][x] = OBJECT_COLOR;
						break;
					case OBSERVER_CODE:
						colorMap[y][x] = OBSERVER_COLOR;
						break;
					case PATH_CODE:
						colorMap[y][x] = PATH_COLOR;
						break;
					default:
						colorMap[y][x] = ROAD_COLOR;
						break;
					}
				}
			}

			// Find the shortest path and paint it green
			auto path = findPath(convertedX, convertedY, checkpointIdx, observer).first;
			for (const auto &point : path)
			{
				int x = point.first;
				int y = point.second;
				if (x >= 0 && x < width && y >= 0 && y < height)
				{
					colorMap[y][x] = PATH_COLOR;
				}
			}

			saveMapPPM(outputFilePath, weightedMap);
		});
	saveThread.detach();
}
