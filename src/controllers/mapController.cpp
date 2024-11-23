#include "mapController.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
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
#define OBJECT_COLOR {255, 0, 0}
#define PATH_COLOR {0, 255, 0}

// Code
#define ROAD_CODE 0
#define BRIDGE_CODE 1
#define ROADSIDE_CODE 2
#define OFF_ROAD_CODE 3
#define WATER_CODE 4
#define WALL_CODE 5
#define CHECKPOINT_CODE 6
#define OBJECT_CODE 7
#define PATH_CODE 8
#define UKNOWN_CODE 9

// Cost
#define ROAD_COST 1.0
#define BRIDGE_COST 1.3
#define ROADSIDE_COST 1.5
#define OFF_ROAD_COST 2.0
#define WATER_COST 5.0
#define WALL_COST std::numeric_limits<double>::infinity()

int coordTransform(float num, int maxCoord)
{
	return std::max(std::min(static_cast<int>(num * 4 + maxCoord / 2), maxCoord), 0);
}

MapController::MapController(const std::string &ppmFilePath,
							 const std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> &checkpoints)
{
	this->checkpoints = checkpoints;
	loadPGM(ppmFilePath);
}

void MapController::loadPGM(const std::string &ppmFilePath)
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

void MapController::loadTrackObjects(const std::vector<std::string> &objects) { trackObjects = objects; }

struct Node
{
	int x, y;
	double cost, heuristic;
	Node *parent;

	Node(int x, int y, double cost, double heuristic, Node *parent = nullptr) :
		x(x), y(y), cost(cost), heuristic(heuristic), parent(parent)
	{
	}

	double totalCost() const { return cost + heuristic; }

	bool operator>(const Node &other) const { return totalCost() > other.totalCost(); }
};

std::vector<std::pair<int, int>> MapController::findPath(int x, int y, int lastCheckpointIndex,
														 const std::vector<Object *> &objects)
{
	auto checkpoint = checkpoints[lastCheckpointIndex];
	int goalX = (checkpoint.first.first + checkpoint.second.first) / 2;
	int goalY = (checkpoint.first.second + checkpoint.second.second) / 2;

	auto heuristic = [goalX, goalY](int x, int y)
	{ return std::sqrt(std::pow(goalX - x, 2) + std::pow(goalY - y, 2)); };

	auto getCost = [this](int x, int y)
	{
		switch (map[y][x])
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
		default:
			return WALL_COST;
		}
	};

	std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;
	std::vector<std::vector<bool>> closedSet(map.size(), std::vector<bool>(map[0].size(), false));

	openSet.emplace(x, y, 0, heuristic(x, y));

	while (!openSet.empty())
	{
		Node current = openSet.top();
		openSet.pop();

		if (current.x == goalX && current.y == goalY)
		{
			std::vector<std::pair<int, int>> path;
			for (Node *node = &current; node != nullptr; node = node->parent)
			{
				path.emplace_back(node->x, node->y);
			}
			std::reverse(path.begin(), path.end());
			return path;
		}

		if (closedSet[current.y][current.x])
			continue;

		closedSet[current.y][current.x] = true;

		const std::vector<std::pair<int, int>> directions = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
		for (const auto &dir : directions)
		{
			int newX = current.x + dir.first;
			int newY = current.y + dir.second;

			if (newX >= 0 && newX < map[0].size() && newY >= 0 && newY < map.size() && !closedSet[newY][newX])
			{
				double newCost = current.cost + getCost(newX, newY);
				openSet.emplace(newX, newY, newCost, heuristic(newX, newY), new Node(current));
			}
		}
	}

	return {};
}

std::vector<std::vector<int>> MapController::getWeightedMap(const std::vector<Object *> &objects)
{
	int width = map[0].size();
	int height = map.size();
	std::vector<std::vector<int>> weightedMap = map;

	for (const auto &object : objects)
	{
		auto box = object->getBox();
		auto projectedVertices = box.get2DProjectedVertices();
		std::vector<std::pair<int, int>> points;
		for (const auto &vertex : projectedVertices)
		{
			int x = coordTransform(vertex.x, width);
			int y = coordTransform(vertex.y, height);
			points.emplace_back(x, y);
		}

		// Find the bounding box of the polygon
		int minX = std::min_element(points.begin(), points.end(),
									[](const auto &a, const auto &b) { return a.first < b.first; })
					   ->first;
		int maxX = std::max_element(points.begin(), points.end(),
									[](const auto &a, const auto &b) { return a.first < b.first; })
					   ->first;
		int minY = std::min_element(points.begin(), points.end(),
									[](const auto &a, const auto &b) { return a.second < b.second; })
					   ->second;
		int maxY = std::max_element(points.begin(), points.end(),
									[](const auto &a, const auto &b) { return a.second < b.second; })
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
						weightedMap[y][x] = OBJECT_CODE;
					}
				}
			}
		}
	}

	for (const auto &checkpoint : checkpoints)
	{
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
				weightedMap[y1][x1] = CHECKPOINT_CODE;
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

	return weightedMap;
}

void MapController::saveModifiedPPM(const std::string &outputFilePath, const std::vector<Object *> &objects, int x,
									int y, int lastCheckpointIndex)
{
	// Create a snapshot of the map and objects
	auto mapSnapshot = map;
	std::vector<Object *> objectsSnapshot = objects;

	const int convertedX = coordTransform(x, map[0].size());
	const int convertedY = coordTransform(y, map.size());

	// Run the save operation in a separate thread
	std::thread saveThread(
		[this, mapSnapshot, objectsSnapshot, outputFilePath, convertedX, convertedY, lastCheckpointIndex]()
		{
			auto weightedMap = getWeightedMap(objectsSnapshot);

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
					case OBJECT_CODE:
						colorMap[y][x] = OBJECT_COLOR;
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
			auto path = findPath(convertedX, convertedY, lastCheckpointIndex, objectsSnapshot);
			for (const auto &point : path)
			{
				int x = point.first;
				int y = point.second;
				if (x >= 0 && x < width && y >= 0 && y < height)
				{
					colorMap[y][x] = PATH_COLOR;
				}
			}

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
			for (const auto &row : colorMap)
			{
				for (const auto &color : row)
				{
					file << std::get<0>(color) << std::get<1>(color) << std::get<2>(color);
				}
			}

			file.close();
		});
	saveThread.detach();
}
