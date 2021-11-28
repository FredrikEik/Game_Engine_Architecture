#include "TerrainSystem.h"
#include "../ECSManager.h"
#include "MeshSystem.h"
#include <fstream>
#include <chrono>
#include "CameraSystem.h"
#include "../Engine/Engine.h"
#include "../Shader.h"
void TerrainSystem::generateRegularGrid(uint32 entity, ECSManager* ECS)
{
	MeshComponent* mesh = ECS->getComponentManager<MeshComponent>()->getComponentChecked(entity);
	std::vector<Vertex>& vertices = (*new std::vector<Vertex>());
	std::vector<glm::vec3>& positions = (*new std::vector<glm::vec3>());
	std::vector<glm::vec3>& normals = (*new std::vector<glm::vec3>());
	std::vector<GLuint>& indices = (*new std::vector<GLuint>());
	int64 rows{ 300};
	int64 columns{ 300 };

	vertices.reserve(rows * columns);
	positions.reserve(rows * columns);

	for (uint64 i{}; i < rows; ++i)
	{
		for (uint64 j{}; j < columns; ++j)
		{
			positions.push_back(glm::vec3(i, 0, j));
			//positions.push_back(glm::vec3(i, std::rand()%10, j));
		}
	}
	int rowsLooped{ 1 };
	for (uint64 i{}; i < positions.size()-columns-1; i++)
	{
		// Don't push back indices outside the edge
		if (((i+1) % (columns*rowsLooped)) == 0 )
		{
			rowsLooped++;
			continue;
		}
		//indices.push_back(i);
		//indices.push_back(i+1);
		//indices.push_back(i+columns);

		//indices.push_back(i+1);
		//indices.push_back(i+1+columns);
		//indices.push_back(i+columns);		
		
		indices.push_back(i);
		indices.push_back(i+1+columns);
		indices.push_back(i+columns);

		indices.push_back(i);
		indices.push_back(i+1);
		indices.push_back(i+columns+1);
	}

	calculateNormals(positions, columns, 1, 1, normals);

	rowsLooped = 1;
	//int32 columnsLooped = 0;
	for (int32 i{}; i<positions.size(); ++i)
	{
		if (((i + 1) % (columns * rowsLooped)) == 0)
		{
			//columnsLooped = 0;
			rowsLooped++;
		}
		mesh->m_vertices.push_back(Vertex(positions[i], normals[i], glm::vec2((1-(rowsLooped % 2)), (i%2))));
		//mesh->m_vertices.push_back(Vertex(positions[i], normals[i], glm::vec2((1-(rowsLooped % 2)), (columnsLooped %2))));
		//++columnsLooped;
		//mesh->m_vertices.push_back(Vertex(it, glm::vec3(0, 1, 0), glm::vec2(0, 0)));
	}
	mesh->m_indices = indices;
	MeshSystem::initialize(*mesh);
	mesh->bDisregardedDuringFrustumCulling = true;
}

void TerrainSystem::generateGridFromLAS(uint32 entity, std::filesystem::path path, ECSManager* ECS)
{
	std::ifstream file(path);
	MeshComponent* mesh = ECS->getComponentManager<MeshComponent>()->getComponentChecked(entity);
	assert(file);
	assert(mesh);
	std::vector<glm::vec3>& lasPositions = (*new std::vector<glm::vec3>());
	std::vector<glm::vec3>& positions = (*new std::vector<glm::vec3>());
	std::vector<glm::vec3>& normals = (*new std::vector<glm::vec3>());
	std::vector<GLuint>& indices = (*new std::vector<GLuint>());

	terrainResolution = { 15 };

	glm::vec3 min{};
	glm::vec3 max{};
	bool initialized{ false };
	float x, y, z;
	int32 points{};

	file >> points; // The first line does not contain position info
	lasPositions.reserve(points);

	auto start = std::chrono::system_clock::now();
	while (file)
	{
		file >> x;
		file >> z;
		file >> y;

		if (!initialized)
		{
			// Making sure the min and max values are valid
			min = glm::vec3(x, y, z);
			max = glm::vec3(x, y, z);
			initialized = true;
		}
		// Calculating the lowest value in the file
		if (x < min.x) min.x = (x);
		if (y < min.y) min.y = (y);
		if (z < min.z) min.z = (z);

		// Calculating the highest value in the file
		if (x > max.x) max.x = (x);
		if (y > max.y) max.y = (y);
		if (z > max.z) max.z = (z);

		lasPositions.push_back(glm::vec3(x, y, z));
	}


	/* @brief A map that will contain the height values, sorted based on the position they will have
	 @param int position in array
	 @param float total height 
	 @param int number of that height*/
	std::map<int, std::pair<float, int>> averageHeightPerSquare;

	float tempTerrainResolution = terrainResolution; // Just so the lambda can capture it
	// Lambda that adds the positions to the averageHeightPerSquare container
	auto addPositionToAvgHeight =
		[tempTerrainResolution, min](std::map<int, std::pair<float, int>>& container, const glm::vec3& position)
	{
		float positionRow = (position.x - min.x + 1) / terrainResolution;
		float positionColumn = (position.z - min.z + 1) / terrainResolution;
		long positionInArray = std::lroundf(positionColumn * positionRow);
		if (container.find(positionInArray) != container.end())
		{
			container.at(positionInArray).first += position.y;
			container.at(positionInArray).second++;
		}
		else
		{
			container.insert(std::pair<int, std::pair<float, int>>(positionInArray, std::pair<float, int>(position.y, 1)));
		}
	};

	auto addPositionToAvgHeightNew =
		[tempTerrainResolution, min](std::map<int, std::pair<float, int>>& container, const glm::vec3& position)
	{
		float positionRow = (position.x - min.x + 1) / terrainResolution;
		float positionColumn = (position.z - min.z + 1) / terrainResolution;
		long positionInArray = std::lroundf(positionColumn * positionRow);
		if (container.find(positionInArray) != container.end())
		{
			container.at(positionInArray).first += position.y;
			container.at(positionInArray).second++;
		}
		else
		{
			container.insert(std::pair<int, std::pair<float, int>>(positionInArray, std::pair<float, int>(position.y, 1)));
		}
	};


	// Looping through all positions and gets the average height per vertex
	for (const auto& it : lasPositions)
	{
		addPositionToAvgHeight(averageHeightPerSquare, it);
	}
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	std::cout << "finished computation at " << std::ctime(&end_time)
		<< "elapsed time: " << elapsed_seconds.count() << "s\n";
	glm::vec3 center = (min + max) / 2.f;
	
	uint64 columns = std::sqrt(averageHeightPerSquare.size());
	normals.reserve(averageHeightPerSquare.size());
	indices.reserve(averageHeightPerSquare.size());
	positions.reserve(averageHeightPerSquare.size());
	std::cout << "Size of terrain: " << columns << "^2\n";
	// Using the for each loop as a regular nested for loop to ensure there are no gaps
	uint64 columnsLooped = 0;
	uint64 rowsLooped = 0;
	for (auto& it : averageHeightPerSquare)
	{
		if (columnsLooped >= columns)
		{
			columnsLooped = 0;
			rowsLooped++;
		}
		it.second.first /= (float)it.second.second;
		x = rowsLooped;
		x *= terrainResolution;
		z = columnsLooped;
		z *= terrainResolution;
		positions.push_back(glm::vec3(x, it.second.first - center.y, z));
		//positions.push_back(glm::vec3(x, 0, z));
		++columnsLooped;
	}

	rowsLooped = { 1 };
	for (uint64 i{}; i < positions.size() - columns - 1; i++)
	{
		// Don't push back indices outside the edge
		if (((i + 1) % (columns * rowsLooped)) == 0)
		{
			rowsLooped++;
			continue;
		}

		indices.push_back(i);
		indices.push_back(i + 1 + columns);
		indices.push_back(i + columns);

		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + columns + 1);
	}

	calculateNormals(positions, columns, terrainResolution, terrainResolution, normals);

	rowsLooped = 1;
	for (int32 i{}; i < positions.size(); ++i)
	{
		if (((i + 1) % (columns * rowsLooped)) == 0)
		{
			rowsLooped++;
		}
		mesh->m_vertices.push_back(Vertex(positions[i], normals[i], glm::vec2((1 - (rowsLooped % 2)), (i % 2))));
	}
	mesh->m_indices = indices;
	MeshSystem::initialize(*mesh);
	mesh->bDisregardedDuringFrustumCulling = true;

}

void TerrainSystem::generateContourLines(uint32 contourEntity, uint32 terrainEntity, ECSManager* ECS)
{
	MeshComponent* terrainMesh = ECS->getComponentManager<MeshComponent>()->getComponentChecked(terrainEntity);
	MeshComponent* contourMesh = ECS->getComponentManager<MeshComponent>()->getComponentChecked(contourEntity);
	assert(terrainMesh);
	assert(contourMesh);
	contourMesh->m_vertices.reserve(terrainMesh->m_indices.size());

	std::vector<glm::vec3>& positions = (*new std::vector<glm::vec3>());
	std::vector<GLuint>& indices = (*new std::vector<GLuint>());

	auto lerp = [](const glm::vec3& a, const glm::vec3& b, float alpha) -> glm::vec3
	{
		return glm::vec3(
			a.x + ((b.x-a.x)	* alpha), 
			a.y + ((b.y-a.y)	* alpha),
			a.z + ((b.z-a.z)	* alpha)
		);
	};

	auto getContourPosition = [](const glm::vec3& position, float height) -> glm::vec3
	{
		return glm::vec3(position.x, height, position.z);
	};


	glm::vec3 tempPosition[4]{};
	glm::vec3 tempLinePosA{};
	glm::vec3 tempLinePosB{};

	//int i{};
	int columns = std::sqrt(terrainMesh->m_vertices.size());
	int ekviDistance{ 4 };
	int yMax{ 50 };
	//for (auto& it : terrainMesh->m_indices)
	//{
	

	auto findCaseToDraw = [contourMesh, lerp](const glm::vec3* pos, float contourHeight, int& index)
	{

		/*
		 __c__
		|    /|	
		d  /  b
		|/_a__|
		*/

		bool a{}, b{}, c{}, d{};
		glm::vec3 n(0, 4, 0);
		glm::vec2 uv{};
		std::vector<glm::vec3> positions;
		
		//auto customPos = [contourHeight](const glm::vec3 pos) {return glm::vec3(pos.x, contourHeight, pos.z); };
		auto getLerpAlpha = [contourHeight](float lower, float upper) {
			float height = contourHeight;
			// Making sure the values are sorted by lowest to highest
			if (lower > upper)
			{
				float temp = lower;
				lower = upper;
				upper = temp;
			}
			// Finding out the lerp alpha of height between lower and upper
			float shift = lower;
			upper -= shift;
			height -= shift;
			return height / upper;
		};
		auto pushPoint = [contourMesh, contourHeight](int& index, const glm::vec3& positionToPush)
		{
			//contourMesh->m_vertices.push_back(Vertex(glm::vec3(positionToPush.x, contourHeight, positionToPush.z), glm::vec3(0, 4, 0), glm::vec2()));
			contourMesh->m_vertices.push_back(Vertex(positionToPush, glm::vec3(0, 4, 0), glm::vec2()));
			contourMesh->m_indices.push_back(index++);
		};
		if (pos[0].y <= contourHeight && pos[1].y > contourHeight ||
			pos[0].y > contourHeight && pos[1].y <= contourHeight)
		{
			// along edge A
			glm::vec3 lerpedPos = lerp(pos[0], pos[1], (getLerpAlpha(pos[0].y, pos[1].y)));
			//pushPoint(index, lerpedPos);
			positions.push_back(lerpedPos);
			if (lerpedPos.x < 0 || lerpedPos.z < 0)
			{
				std::cout << "x: " << lerpedPos.x << " y: " << lerpedPos.y << " z: " << lerpedPos.z << "\n";
				float lerpAlpha = getLerpAlpha(pos[0].y, pos[1].y);
				lerpedPos = lerp(pos[0], pos[1], (lerpAlpha));
			}
			a = true;
		}


		if (pos[1].y <= contourHeight && pos[3].y > contourHeight || 
			pos[1].y > contourHeight && pos[3].y <= contourHeight)
		{
			// along edge B
			glm::vec3 lerpedPos = lerp(pos[1], pos[3], (getLerpAlpha(pos[1].y, pos[3].y)));
			//pushPoint(index, lerpedPos);
			positions.push_back(lerpedPos);

			b = true;

			if (lerpedPos.x < 0 || lerpedPos.z < 0)
				std::cout << "x: " << lerpedPos.x << " y: " << lerpedPos.y << " z: " << lerpedPos.z << "\n";
		}

		if (pos[2].y <= contourHeight && pos[3].y > contourHeight || 
			pos[2].y > contourHeight && pos[3].y <= contourHeight)
		{
			// along edge C
			glm::vec3 lerpedPos = lerp(pos[2], pos[3], (getLerpAlpha(pos[2].y, pos[3].y)));
			//pushPoint(index, lerpedPos);
			positions.push_back(lerpedPos);

			//std::cout << "edge C\n";
			c = true;
			if (lerpedPos.x < 0 || lerpedPos.z < 0)
				std::cout << "x: " << lerpedPos.x << " y: " << lerpedPos.y << " z: " << lerpedPos.z << "\n";

		}

		if (pos[0].y <= contourHeight && pos[2].y > contourHeight || 
			pos[0].y > contourHeight && pos[2].y <= contourHeight)
		{
			// along edge D
			glm::vec3 lerpedPos = lerp(pos[0], pos[2], (getLerpAlpha(pos[0].y, pos[2].y)));
			positions.push_back(lerpedPos);

			d = true;
			if (lerpedPos.x < 0 || lerpedPos.z < 0)
				std::cout << "x: " << lerpedPos.x << " y: " << lerpedPos.y << " z: " << lerpedPos.z << "\n";
		}

		auto lerpFloat = [](float a, float b, float alpha) {return a + (b - a) * alpha; };
		auto getPositionOnTriangleLine = [pos, lerp](float alpha) {return lerp(pos[0], pos[3], alpha); };

		auto getTriangleAlpha = [lerp, lerpFloat, getLerpAlpha, pos](int a1, int a2, int b1, int b2)
		{
			float lerpAlphaA = getLerpAlpha(pos[a1].y, pos[a2].y);
			float lerpAlphaB = getLerpAlpha(pos[b1].y, pos[b2].y);
			//TESTING CODE
			

			// END OF TESTING CODE
			return lerpFloat(lerpAlphaA, lerpAlphaB, 0.5f);
		};

		auto getTriangleIntersectionAlpha = [pos, lerp](glm::vec3 point)
		{
			glm::vec3 a = pos[0];
			glm::vec3 b = pos[3];
			a.y = 0;
			b.y = 0;
			point.y = 0;
			if (glm::length(a) > glm::length(b))
			{
				glm::vec3 temp = a;
				a = b;
				b = temp;
			}
			b -= a;
			point -= a;
			float alpha{ (glm::length(point) / glm::length(b)) };
			return alpha;
		};

		auto getTriangleIntersectionPoint = [pos, lerp, getTriangleIntersectionAlpha](const glm::vec3& a1, const glm::vec3& a2)
		{

			const glm::vec3& b1 = pos[0];
			const glm::vec3& b2 = pos[3];

			float d1343{ glm::dot(a1 - b1, b2 - b1) };
			float d4321{ glm::dot(b2 - b1, a2 - a1) };
			float d1321{ glm::dot(a1 - b1, a2 - a1) };
			float d4343{ glm::dot(b2 - b1, b2 - b1) };
			float d2121{ glm::dot(a2 - a1, a2 - a1) };

			float denom = d2121 * d4343 - d4321 * d4321;
			float numer = d1343 * d4321 - d1321 * d4343;
			float mua = numer / denom;
			float mub = (d1343 + d4321 * (mua)) / d4343;

			glm::vec3 pointA = a1 + (mua * (a2 - a1));
			glm::vec3 pointB = b1 + (mub * (b2 - b1));
			


			return lerp(pointA, pointB, 0.5f);
			return pointB;
		};
		/*
__c__
|    /|
d  /  b
|/_a__|
*/
		if (positions.size() == 2)
		{
			
			//
			if (a && c);
			else if (a && d);
			else if (b && c);
			else if (b && d);
			else
			{
				pushPoint(index, positions[0]);
				pushPoint(index, positions[1]);
				return;
			}
			glm::vec3 triangleLinePos = lerp(pos[0], pos[3], getTriangleIntersectionAlpha(
				getTriangleIntersectionPoint(positions[0], positions[1])));

			pushPoint(index, positions[0]);
			pushPoint(index, triangleLinePos);
			pushPoint(index, triangleLinePos);
			pushPoint(index, positions[1]);
		}
		else if (positions.size() == 4)
		{

			glm::vec3 triangleLinePos{};
			// a - b
			pushPoint(index, positions[0]);
			pushPoint(index, positions[1]);

			// b - c
			pushPoint(index, positions[1]);
			triangleLinePos = getPositionOnTriangleLine(getTriangleAlpha(1, 3, 2, 3)); 
			triangleLinePos = lerp(pos[0], pos[3], getTriangleIntersectionAlpha(
				getTriangleIntersectionPoint(positions[1], positions[2])));
			pushPoint(index, triangleLinePos);
			pushPoint(index, triangleLinePos);
			pushPoint(index, positions[2]);

			// c - d
			pushPoint(index, positions[2]);
			pushPoint(index, positions[3]);

			// d - a
			pushPoint(index, positions[3]);
			triangleLinePos = getPositionOnTriangleLine(getTriangleAlpha(0, 1, 0, 2));
			triangleLinePos = lerp(pos[0], pos[3], getTriangleIntersectionAlpha(
				getTriangleIntersectionPoint(positions[3], positions[0])));
			pushPoint(index, triangleLinePos);
			pushPoint(index, triangleLinePos);
			pushPoint(index, positions[0]);

			//if (a && c)
			//	triangleLinePos = getPositionOnTriangleLine(getTriangleAlpha(0, 1, 2, 3));
			//else if (a && d)
			//	triangleLinePos = getPositionOnTriangleLine(getTriangleAlpha(0, 1, 0, 2));
			//else if (b && c)
			//else if (b && d)
			//	triangleLinePos = getPositionOnTriangleLine(getTriangleAlpha(1, 3, 0, 2));

		}
	};

	auto findDrawPoints = [lerp](glm::vec3* positions, float height, glm::vec3& a, glm::vec3& b, glm::vec3& c, glm::vec3& d)
	{
		/*	  c
			d	b
			  a
		*/
		a = lerp(positions[0], positions[1], 0.5f);
		//a.y = height;
		b = lerp(positions[3], positions[1], 0.5f);
		//b.y = height;
		c = lerp(positions[3], positions[2], 0.5f);
		//c.y = height;
		d = lerp(positions[2], positions[0], 0.5f);
		//d.y = height;
	};

	int index{ };
	auto drawCase = [contourMesh, findDrawPoints](int caseToDraw, glm::vec3* positions, float height, int& index)
	{
		/*	  c
			d	b
			  a
		*/
		glm::vec3 a, b, c, d;
		glm::vec3 normal{ 0,4,0 };
		glm::vec2 uv{};
		findDrawPoints(positions, height, a, b, c, d);
		switch (caseToDraw)
		{
		case 1: case 14:
			contourMesh->m_vertices.push_back(Vertex(d, normal, uv));
			contourMesh->m_indices.push_back(index++);
			contourMesh->m_vertices.push_back(Vertex(a, normal, uv));
			contourMesh->m_indices.push_back(index++);
			break;
		case 2: case 13:
			contourMesh->m_vertices.push_back(Vertex(a, normal, uv));
			contourMesh->m_indices.push_back(index++);
			contourMesh->m_vertices.push_back(Vertex(b, normal, uv));
			contourMesh->m_indices.push_back(index++);
			break;
		case 3: case 12:
			contourMesh->m_vertices.push_back(Vertex(d, normal, uv));
			contourMesh->m_indices.push_back(index++);
			contourMesh->m_vertices.push_back(Vertex(b, normal, uv));
			contourMesh->m_indices.push_back(index++);
			break;
		case 4: case 11:
			contourMesh->m_vertices.push_back(Vertex(c, normal, uv));
			contourMesh->m_indices.push_back(index++);
			contourMesh->m_vertices.push_back(Vertex(b, normal, uv));
			contourMesh->m_indices.push_back(index++);
			break;
		case 5:
			contourMesh->m_vertices.push_back(Vertex(a, normal, uv));
			contourMesh->m_indices.push_back(index++);
			contourMesh->m_vertices.push_back(Vertex(b, normal, uv));
			contourMesh->m_indices.push_back(index++);

			contourMesh->m_vertices.push_back(Vertex(d, normal, uv));
			contourMesh->m_indices.push_back(index++);
			contourMesh->m_vertices.push_back(Vertex(c, normal, uv));
			contourMesh->m_indices.push_back(index++);
			break;
		case 6: case 9:
			contourMesh->m_vertices.push_back(Vertex(a, normal, uv));
			contourMesh->m_indices.push_back(index++);
			contourMesh->m_vertices.push_back(Vertex(c, normal, uv));
			contourMesh->m_indices.push_back(index++);
			break;
		case 7: case 8:
			contourMesh->m_vertices.push_back(Vertex(d, normal, uv));
			contourMesh->m_indices.push_back(index++);
			contourMesh->m_vertices.push_back(Vertex(c, normal, uv));
			contourMesh->m_indices.push_back(index++);

			break;
		case 10:
			contourMesh->m_vertices.push_back(Vertex(d, normal, uv));
			contourMesh->m_indices.push_back(index++);
			contourMesh->m_vertices.push_back(Vertex(a, normal, uv));
			contourMesh->m_indices.push_back(index++);

			contourMesh->m_vertices.push_back(Vertex(c, normal, uv));
			contourMesh->m_indices.push_back(index++);
			contourMesh->m_vertices.push_back(Vertex(b, normal, uv));
			contourMesh->m_indices.push_back(index++);
			break;

		default:
			break;
		}
	};

	for (int i{-50}; i < yMax; i += ekviDistance)
	{
		for (int j{}; j < terrainMesh->m_vertices.size()-columns-1; ++j)
		{
			int caseToDraw{};
			contourMesh = ECS->getComponentManager<MeshComponent>()->getComponentChecked(contourEntity);

			if ((j + 1) % columns == 0)
				continue;

			//if((j + columns + 1) >= )

			tempPosition[0] = terrainMesh->m_vertices[j].getPosition();
			tempPosition[1] = terrainMesh->m_vertices[j + 1].getPosition();
			tempPosition[2] = terrainMesh->m_vertices[j + columns].getPosition();
			tempPosition[3] = terrainMesh->m_vertices[j + columns + 1].getPosition();
			findCaseToDraw(tempPosition, i, index);

			//if (tempPosition[0].y < j && tempPosition[2].y >= j)
			//{
			//	contourMesh->m_vertices.push_back(Vertex(getContourPosition(tempPosition[0], i), normal, uv));
			//	contourMesh->m_indices.push_back(index++);
			//}
			////////caseToDraw += withinEkviDistance(tempPosition[0], i);
			////////caseToDraw += withinEkviDistance(tempPosition[1], i) * 2;
			////////caseToDraw += withinEkviDistance(tempPosition[2], i) * 4;
			////////caseToDraw += withinEkviDistance(tempPosition[3], i) * 8;

			////////drawCase(caseToDraw, tempPosition, i, index);
			


			//if (caseToDraw != 15 && caseToDraw > 0)
			//	std::cout << "CaseToDraw: " << caseToDraw << "\n";
			//tempLinePosA = lerp(tempPosition[0], tempPosition[2], 0.5f);
			//tempLinePosB = lerp(tempPosition[1], tempPosition[3], 0.5f);

			//findLineToDraw(tempPosition, tempLinePosA, tempLinePosB);
			//contourMesh->m_vertices.push_back(Vertex(tempLinePosA, glm::vec3(0, 1, 0), glm::vec2()));
			//contourMesh->m_vertices.push_back(Vertex(tempLinePosB, glm::vec3(0, 1, 0), glm::vec2()));

			//contourMesh->m_indices.push_back(j);
			//contourMesh->m_indices.push_back(j+1);
			//if (j > 10)
			//	break;

		}
	/*	if (i > 10)
			break;*/
	}


	//contourMesh->m_vertices.push_back(Vertex(glm::vec3(0, 0, 0), glm::vec3(0, 3, 0), glm::vec2()));
	//contourMesh->m_vertices.push_back(Vertex(glm::vec3(0, 0, 10), glm::vec3(0, 3, 0), glm::vec2()));
	//contourMesh->m_vertices.push_back(Vertex(glm::vec3(0, 0.2, 0), glm::vec3(0, 3, 0), glm::vec2()));
	//contourMesh->m_vertices.push_back(Vertex(glm::vec3(0, 0.2, 10), glm::vec3(0, 3, 0), glm::vec2()));

	//contourMesh->m_vertices.push_back(Vertex(glm::vec3(0,10, 0), glm::vec3(0, 3, 0), glm::vec2()));
	//contourMesh->m_vertices.push_back(Vertex(glm::vec3(0, 10, 10), glm::vec3(0, 3, 0), glm::vec2()));
	//contourMesh->m_vertices.push_back(Vertex(glm::vec3(0, 10.2, 0), glm::vec3(0, 3, 0), glm::vec2()));
	//contourMesh->m_vertices.push_back(Vertex(glm::vec3(0, 10.2, 10), glm::vec3(0, 3, 0), glm::vec2()));
	//////contourMesh->m_indices.push_back(0);
	//////contourMesh->m_indices.push_back(3);
	//////contourMesh->m_indices.push_back(2);

	//////contourMesh->m_indices.push_back(0);
	//////contourMesh->m_indices.push_back(1);
	//////contourMesh->m_indices.push_back(3);

	//////contourMesh->m_indices.push_back(4);
	//////contourMesh->m_indices.push_back(7);
	//////contourMesh->m_indices.push_back(6);

	//////contourMesh->m_indices.push_back(4);
	//////contourMesh->m_indices.push_back(5);
	//////contourMesh->m_indices.push_back(7);

	//contourMesh->m_indices.push_back(0);
	//contourMesh->m_indices.push_back(1);
	//contourMesh->m_indices.push_back(4);
	//contourMesh->m_indices.push_back(5);


	//indices.push_back(i);
	//indices.push_back(i + 1 + columns);
	//indices.push_back(i + columns);

	//indices.push_back(i);
	//indices.push_back(i + 1);
	//indices.push_back(i + columns + 1);

	//contourMesh->m_indices.push_back(2);

	//}
	contourMesh = ECS->getComponentManager<MeshComponent>()->getComponentChecked(contourEntity);
	contourMesh->m_drawType = GL_LINES;
	MeshSystem::initialize(*contourMesh);
	contourMesh->bDisregardedDuringFrustumCulling = true;
	
}


glm::vec3 TerrainSystem::getNormal(const struct TransformComponent& entityTransform, 
	const MeshComponent& terrainMesh, const int32& index)
{
	if (index < 0)
		return glm::vec3(0, 1, 0); // not standing on any surface, assume flat ground

	glm::vec3 a(terrainMesh.m_vertices[terrainMesh.m_indices[index]].getPosition());
	glm::vec3 b(terrainMesh.m_vertices[terrainMesh.m_indices[index+1]].getPosition());
	glm::vec3 c(terrainMesh.m_vertices[terrainMesh.m_indices[index+2]].getPosition());

	glm::vec3 v(b - a);
	glm::vec3 w(c - a);

	glm::vec3 normal = glm::cross(v, w); // opposite of QT
	if (glm::length(normal) < 0.1f)
		return glm::vec3(0, 1, 0);
	else
		return glm::normalize(normal);

	//return glm::vec3();
}

void TerrainSystem::calculateNormals(const std::vector<glm::vec3>& positions, uint32 rowSize,
	float stepDistanceX, float stepDistanceY, std::vector<glm::vec3>& OUTNormals)
{
	uint32 rowsLooped{ 1 };

	float yLeft, yRight, yUp, yDown, yUpRight, yDownLeft;

	auto getHeight = [rowSize](const std::vector<glm::vec3>& positions, uint32 i, uint32& rowsLooped) -> float
	{
		if (i < (positions.size() - 1) && i > 0)
		{
			if (((i) % (rowSize * rowsLooped)) == 0)
			{
				return 0.f;
				//rowsLooped++;
			}
			else
				return positions[i].y;
		}
		else
			return 0;
	};

	for (uint32 i{}; i < positions.size() - 1; i++)
	{
		yLeft = getHeight(positions, i - 1, rowsLooped);
		yRight = getHeight(positions, i + 1, rowsLooped);
		yUp = getHeight(positions, i + rowSize, rowsLooped);
		yDown = getHeight(positions, i - rowSize, rowsLooped);
		yUpRight = getHeight(positions, i + rowSize + 1, rowsLooped);
		yDownLeft = getHeight(positions, i - rowSize - 1, rowsLooped);

		// Instead of doing cross product of 6 triangles
		// This is the same
		glm::vec3 normal ((2 * (yLeft - yRight) - yUpRight + yDownLeft + yUp - yDown ) / stepDistanceX,
			6,
			(2 * (yDown - yUp) + yUpRight + yDownLeft - yUp - yLeft) / stepDistanceY
		);
		normal = glm::normalize(normal);
		OUTNormals.push_back(normal);

		if (((i + 1) % (rowSize * rowsLooped)) == 0)
		{
			rowsLooped++;
		}
	}
}

float TerrainSystem::getHeight(uint32 entity, uint32 terrainEntity, class ECSManager* ECS)
{
	MeshComponent* mesh = ECS->getComponentManager<MeshComponent>()->getComponentChecked(terrainEntity);
	TransformComponent* transform = ECS->getComponentManager<TransformComponent>()->getComponentChecked(entity);
	assert(mesh);
	assert(transform);

	int32 tempIndex{};
	return getHeight(*transform, *mesh, tempIndex);

	glm::vec3 position = glm::vec3(transform->transform[3]);
	uint32 columns = std::sqrt(mesh->m_vertices.size());

	// Dividing the position by the terrain resolution and flooring the results
	int positionX = position.x / terrainResolution;
	int positionZ = position.z / terrainResolution;

	// Converting the worldposition to a position in the vertex array
	uint32 positionInArray = positionZ + (columns * positionX);

	// Converting the position in the vertex array to a position in the index array
	uint32 approximateStartIndex = positionInArray * 6;

	/**Because the index array stops one short on every row, a small error builds up
	* For now, a static variable errorMargin corrects this error and makes the guess very accurate
	* However, it remains to be tested if this is faster than an unchanging error margin when lots of calls are introduced
	* 1-2 percent is a safe margin, but will make the guess miss by a few houndred when the index gets large*/
	//approximateStartIndex = ((float)approximateStartIndex) * 0.99;
	approximateStartIndex = ((float)approximateStartIndex * errorMargin);


	// Makes sure we start at the start of a triangle
	while (approximateStartIndex % 3 != 0)
		--approximateStartIndex;

	glm::vec3 p, q, r, baryCoord;

	//std::cout << "Position in array: " << positionInArray << ". Guessed index: " << approximateStartIndex << '\n';
	for (uint32 i{ approximateStartIndex }; i < mesh->m_indices.size(); i+=3)
	{
		if (findTriangle(i, position, *mesh, baryCoord, p, q, r))
		{
			int32 error = approximateStartIndex - i;
			if (error <= -9)
				errorMargin += 0.00001f;
			else if (error >= -3 && approximateStartIndex > 12)
				errorMargin -= 0.00001f;

			//std::cout << "Found triangle after " << i - approximateStartIndex << " loops at index "<<i << " with error: " << error << " and error margin: " << errorMargin << '\n';
			//std::cout << "Found triangle at index" << i << ". Height is: " << baryCoord.x * p.y + baryCoord.y * q.y + baryCoord.z * r.y <<" .\n";
			return baryCoord.x * p.y + baryCoord.y * q.y + baryCoord.z * r.y;
		}
	}

	return 0.0f;
}

float TerrainSystem::getHeight(const TransformComponent& entityTransform, const MeshComponent& terrainMesh,
	int32& OUTIndex)
{

	glm::vec3 position = glm::vec3(entityTransform.transform[3]);

	if (position.x < 0 || position.z < 0)
	{
		OUTIndex = -1;
		return 0.0f;
	}

	uint32 columns = std::sqrt(terrainMesh.m_vertices.size());

	// Dividing the position by the terrain resolution and flooring the results
	int positionX = position.x / terrainResolution;
	int positionZ = position.z / terrainResolution;

	// Converting the worldposition to a position in the vertex array
	uint32 positionInArray = positionZ + (columns * positionX);

	// Converting the position in the vertex array to a position in the index array
	uint32 approximateStartIndex = positionInArray * 6;

	/**Because the index array stops one short on every row, a small error builds up
	* For now, a static variable errorMargin corrects this error and makes the guess very accurate
	* However, it remains to be tested if this is faster than an unchanging error margin when lots of calls are introduced
	* 1-2 percent is a safe margin, but will make the guess miss by a few houndred when the index gets large*/
	//approximateStartIndex = ((float)approximateStartIndex) * 0.99;
	approximateStartIndex = ((float)approximateStartIndex * errorMargin); // could be per entity
	//approximateStartIndex = ((float)approximateStartIndex * OUTErrorMargin); // per entity


	// Makes sure we start at the start of a triangle
	while (approximateStartIndex % 3 != 0)
		--approximateStartIndex;

	glm::vec3 p, q, r, baryCoord;

	//std::cout << "Position in array: " << positionInArray << ". Guessed index: " << approximateStartIndex << '\n';
	for (uint32 i{ approximateStartIndex }; i < terrainMesh.m_indices.size(); i += 3)
	{
		if (findTriangle(i, position, terrainMesh, baryCoord, p, q, r))
		{
			int32 error = approximateStartIndex - i;
			if (error <= -9)
				errorMargin += 0.00001f;
			else if (error >= -3 && approximateStartIndex > 12)
				errorMargin -= 0.00001f;

				//OUTErrorMargin += 0.00001f * (error <= -9);
				//OUTErrorMargin -= 0.00001f * (error >= -3 && approximateStartIndex > 12);

			OUTIndex = i;

			return baryCoord.x * p.y + baryCoord.y * q.y + baryCoord.z * r.y;
		}
	}

	return 0.0f;
}

float TerrainSystem::getHeightFast(const TransformComponent& entityTransform, const MeshComponent& terrainMesh, const int32& index)
{
	glm::vec3 p, q, r, baryCoord, position{entityTransform.transform[3]};

	findTriangle(index, position, terrainMesh, baryCoord, p, q, r);

	return baryCoord.x * p.y + baryCoord.y * q.y + baryCoord.z * r.y;
}

bool TerrainSystem::findTriangle(uint64 index, const glm::vec3& position, const MeshComponent& terrainMesh,
	glm::vec3& outBaryCoord, glm::vec3& outP, glm::vec3& outQ, glm::vec3& outR)
{
	glm::vec2 p, q, r;

	// first finding the triangle by searching with 2d vector
	// then get the height of all 3 vertices when the triangle is found
	p = glm::vec2(terrainMesh.m_vertices[terrainMesh.m_indices[index]].m_xyz[0], 
		terrainMesh.m_vertices[terrainMesh.m_indices[index]].m_xyz[2]);

	q = glm::vec2(terrainMesh.m_vertices[terrainMesh.m_indices[(index+1)]].m_xyz[0], 
		terrainMesh.m_vertices[terrainMesh.m_indices[(index+1)]].m_xyz[2]);

	r = glm::vec2(terrainMesh.m_vertices[terrainMesh.m_indices[(index+2)]].m_xyz[0],
		terrainMesh.m_vertices[terrainMesh.m_indices[(index+2)]].m_xyz[2]);

	glm::vec2 pos(position.x, position.z);

	outBaryCoord = baryCentricCoordinates(pos, p, q, r);
	if (outBaryCoord.x >= 0 && outBaryCoord.y >= 0 && outBaryCoord.z >= 0)
	{
		outP = glm::vec3(terrainMesh.m_vertices[terrainMesh.m_indices[index]].m_xyz[0],
			terrainMesh.m_vertices[terrainMesh.m_indices[index]].m_xyz[1],
			terrainMesh.m_vertices[terrainMesh.m_indices[index]].m_xyz[2]);

		outQ = glm::vec3(terrainMesh.m_vertices[terrainMesh.m_indices[(index + 1)]].m_xyz[0],
			terrainMesh.m_vertices[terrainMesh.m_indices[(index + 1)]].m_xyz[1],
			terrainMesh.m_vertices[terrainMesh.m_indices[(index + 1)]].m_xyz[2]);

		outR = glm::vec3(terrainMesh.m_vertices[terrainMesh.m_indices[(index + 2)]].m_xyz[0],
			terrainMesh.m_vertices[terrainMesh.m_indices[(index + 2)]].m_xyz[1],
			terrainMesh.m_vertices[terrainMesh.m_indices[(index + 2)]].m_xyz[2]);

		return true;
	}
	return false;
}

glm::vec3 TerrainSystem::baryCentricCoordinates(const glm::vec2& position, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3)
{
	glm::vec2 p12 = p2 - p1;
	glm::vec2 p13 = p3 - p1;
	glm::vec3 n = glm::cross(glm::vec3(p13, 0), glm::vec3(p12, 0));
	float areal_123 = glm::length(n); 

	glm::vec3 baryc; 
	glm::vec2 p = p2 - position;
	glm::vec2 q = p3 - position;
	n = glm::cross(glm::vec3(q,0), glm::vec3(p,0));
	baryc.x = (n.z / areal_123);// v

	p = p3 - position;
	q = p1 - position;
	n = glm::cross(glm::vec3(q,0), glm::vec3(p,0));
	baryc.y = (n.z / areal_123);// w

	p = p1 - position;
	q = p2 - position;
	n = glm::cross(glm::vec3(q,0), glm::vec3(p,0));
	baryc.z = (n.z / areal_123); // u?

	return baryc;
}
