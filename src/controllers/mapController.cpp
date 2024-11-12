#include "mapController.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <queue>
#include <sstream>
#include <thread>
#include "object/object.hpp"

int coordTransform(float num, int maxCoord) { return static_cast<int>(num * 4 + maxCoord / 2); }

MapController::MapController(const std::string &ppmFilePath) { loadPPM(ppmFilePath); }

void MapController::loadPPM(const std::string &ppmFilePath)
{
	std::ifstream file(ppmFilePath);
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open PPM file");
	}

	std::string line;
	std::getline(file, line);
	if (line != "P6")
	{
		throw std::runtime_error("Invalid PPM file format");
	}

	int width, height;
	file >> width >> height;
	map.resize(height, std::vector<int>(width, 0));

	int maxColor;
	file >> maxColor;
	file.get(); // Consume the newline character

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			char r, g, b;
			file.read(&r, 1);
			file.read(&g, 1);
			file.read(&b, 1);
			if (r == 255 && g == 255 && b == 255)
			{
				map[y][x] = 0; // White pixel
			}
			else if (r == 0 && g == 0 && b == 0)
			{
				map[y][x] = 1; // Black pixel
			}
			else
			{
				map[y][x] = (static_cast<int>(r) << 16) | (static_cast<int>(g) << 8) | static_cast<int>(b);
			}
		}
	}

	file.close();
}

void MapController::loadTrackObjects(const std::vector<std::string> &objects) { trackObjects = objects; }

// TODO: Implement this function properly
// Find the shortest path using A* algorithm
std::vector<std::pair<int, int>> MapController::findShortestPath(int startX, int startY, int endX, int endY)
{
	// A* algorithm implementation
	struct Node
	{
		int x, y;
		int g, h;
		Node *parent;
	};

	auto compare = [](const Node *a, const Node *b) { return a->g + a->h > b->g + b->h; };

	std::priority_queue<Node *, std::vector<Node *>, decltype(compare)> openSet(compare);
	std::vector<std::vector<bool>> closedSet(map.size(), std::vector<bool>(map[0].size(), false));
	std::vector<std::vector<Node *>> nodes(map.size(), std::vector<Node *>(map[0].size(), nullptr));

	Node *start = new Node{startX, startY, 0, 0, nullptr};
	start->h = std::abs(endX - startX) + std::abs(endY - startY);
	openSet.push(start);

	while (!openSet.empty())
	{
		Node *current = openSet.top();
		openSet.pop();

		if (current->x == endX && current->y == endY)
		{
			std::vector<std::pair<int, int>> path;
			while (current != nullptr)
			{
				path.emplace_back(current->x, current->y);
				current = current->parent;
			}
			return path;
		}

		closedSet[current->y][current->x] = true;

		for (int dy = -1; dy <= 1; dy++)
		{
			for (int dx = -1; dx <= 1; dx++)
			{
				if (dx == 0 && dy == 0)
				{
					continue;
				}

				int x = current->x + dx;
				int y = current->y + dy;
				if (x < 0 || x >= map[0].size() || y < 0 || y >= map.size() || map[y][x] == 1 || closedSet[y][x])
				{
					continue;
				}

				Node *neighbor = nodes[y][x];
				if (neighbor == nullptr)
				{
					neighbor = new Node{x, y, current->g + 1, 0, current};
					neighbor->h = std::abs(endX - x) + std::abs(endY - y);
					nodes[y][x] = neighbor;
					openSet.push(neighbor);
				}
				else if (current->g + 1 < neighbor->g)
				{
					neighbor->g = current->g + 1;
					neighbor->parent = current;
				}
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
		int minX = std::min_element(points.begin(), points.end(), [](const auto &a, const auto &b) { return a.first < b.first; })->first;
		int maxX = std::max_element(points.begin(), points.end(), [](const auto &a, const auto &b) { return a.first < b.first; })->first;
		int minY = std::min_element(points.begin(), points.end(), [](const auto &a, const auto &b) { return a.second < b.second; })->second;
		int maxY = std::max_element(points.begin(), points.end(), [](const auto &a, const auto &b) { return a.second < b.second; })->second;

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
						weightedMap[y][x] = 2;
					}
				}
			}
		}
	}

	return weightedMap;
}

void MapController::saveModifiedPPM(const std::string &outputFilePath, const std::vector<Object *> &objects)
{
	// Create a snapshot of the map and objects
	auto mapSnapshot = map;
	std::vector<Object *> objectsSnapshot = objects;

	// Run the save operation in a separate thread
	std::thread([this, mapSnapshot, objectsSnapshot, outputFilePath]() {
		auto weightedMap = getWeightedMap(objectsSnapshot);

		int width = weightedMap[0].size();
		int height = weightedMap.size();

		// Create a color map from the original PPM
		std::vector<std::vector<std::tuple<char, char, char>>> colorMap(height, std::vector<std::tuple<char, char, char>>(width));
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				int cell = mapSnapshot[y][x];
				if (cell == 0)
					colorMap[y][x] = std::make_tuple((char)255, (char)255, (char)255);
				else if (cell == 1)
					colorMap[y][x] = std::make_tuple((char)0, (char)0, (char)0);
				else
					colorMap[y][x] = std::make_tuple((char)(cell & 0xFF), (char)(cell & 0xFF), (char)(cell & 0xFF));
			}
		}

		// Modify the color map based on the weighted map
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				if (weightedMap[y][x] == 2)
				{
					colorMap[y][x] = std::make_tuple((char)255, (char)0, (char)0);
				}
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
	}).detach();
}
