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
	contourMesh->m_indices.reserve(terrainMesh->m_indices.size());

	glm::vec3 tempPosition[4]{};

	int columns = std::sqrt(terrainMesh->m_vertices.size());
	int ekviDistance{ 4 };
	int yMin{ -50 };
	int yMax{ 50 };
	
#pragma region MarchingSquares
	auto drawContourOnSquare = [contourMesh](const glm::vec3* pos, float contourHeight, int& index)
	{
		/*
		 __c__
		|    /|	
		d  /  b
		|/_a__|
		*/

		bool a{}, b{}, c{}, d{};
		std::vector<glm::vec3> positions;
		
	#pragma region ConvenienceLambdas
		auto lerp = [](const glm::vec3& a, const glm::vec3& b, float alpha) -> glm::vec3
		{
			return glm::vec3(
				a.x + ((b.x - a.x) * alpha),
				a.y + ((b.y - a.y) * alpha),
				a.z + ((b.z - a.z) * alpha)
			);
		};

		auto getLerpAlpha = [contourHeight](float lower, float upper) {
			float height = contourHeight;
			// Making sure the values are sorted by lowest to highest
			if (lower > upper)
			{
				float temp = lower;
				lower = upper;
				upper = temp;
			}
			// Subtracts the lowest value to find the difference between the height and max value
			// The result is the alpha we would use when linear interpolating lower and upper to get height.
			float shift = lower;
			upper -= shift;
			height -= shift;
			return height / upper;
		};

		// Convenience lambda to push a point to the mesh.
		auto pushPoint = [contourMesh, contourHeight](int& index, const glm::vec3& positionToPush)
		{
			contourMesh->m_vertices.push_back(Vertex(positionToPush, glm::vec3(0, 4, 0), glm::vec2()));
			contourMesh->m_indices.push_back(index++);
		};
		//ConvenienceLambdas
	#pragma endregion

		// This is essentially where the marching squares are happening
	#pragma region ContourLineTesting
		// Tests if the contour line crosses edge A
		if (pos[0].y <= contourHeight && pos[1].y > contourHeight ||
			pos[0].y > contourHeight && pos[1].y <= contourHeight)
		{
			// along edge A
			glm::vec3 lerpedPos = lerp(pos[0], pos[1], (getLerpAlpha(pos[0].y, pos[1].y)));
			positions.push_back(lerpedPos);
			a = true;
		}

		// Tests if the contour line crosses edge B
		if (pos[1].y <= contourHeight && pos[3].y > contourHeight || 
			pos[1].y > contourHeight && pos[3].y <= contourHeight)
		{
			// along edge B
			glm::vec3 lerpedPos = lerp(pos[1], pos[3], (getLerpAlpha(pos[1].y, pos[3].y)));
			positions.push_back(lerpedPos);

			b = true;
		}

		// Tests if the contour line crosses edge C
		if (pos[2].y <= contourHeight && pos[3].y > contourHeight || 
			pos[2].y > contourHeight && pos[3].y <= contourHeight)
		{
			// along edge C
			glm::vec3 lerpedPos = lerp(pos[2], pos[3], (getLerpAlpha(pos[2].y, pos[3].y)));
			positions.push_back(lerpedPos);

			c = true;
		}

		// Tests if the contour line crosses edge D
		if (pos[0].y <= contourHeight && pos[2].y > contourHeight || 
			pos[0].y > contourHeight && pos[2].y <= contourHeight)
		{
			// along edge D
			glm::vec3 lerpedPos = lerp(pos[0], pos[2], (getLerpAlpha(pos[0].y, pos[2].y)));
			positions.push_back(lerpedPos);

			d = true;
		}
		//ContourLineTesting
	#pragma endregion 

		// Helper lambdas to find where the contour line intersects the quad diagonal
		// The goal is to triangulate the result after marching the squares. 
	#pragma region TriangleIntersection
		/*
		* Because the lambda getTriangleIntersectionPoint only finds the closest point to intersection
		* and not the exact point, the lambda getTriangleIntersectionAlpha reverse engineers the 
		* approximated point to find the alpha it would have if it were a point 
		* linear interpolated on the diagonal. 
		
		* This can then be used to lerp on the actual diagonal to achieve a much more accurate result.
		*/
		auto getTriangleIntersectionAlpha = [pos, lerp](glm::vec3 point)
		{
			glm::vec3 a = pos[0];
			glm::vec3 b = pos[3];
			// Because Y is where the biggest error in the approximation lies, we flatten the quad by removing Y
			a.y = 0;
			b.y = 0;
			point.y = 0;

			// Making sure a is the shortest vector
			if (glm::length(a) > glm::length(b))
			{
				glm::vec3 temp = a;
				a = b;
				b = temp;
			}
			// Subtracts a to find out where the point is relative to b
			b -= a;
			point -= a;
			return (glm::length(point) / glm::length(b));
		};

		// Finds an approximate point where the line intersects with the diagonal of the quad
		// http://paulbourke.net/geometry/pointlineplane/
		auto getTriangleIntersectionPoint = [pos, lerp](const glm::vec3& p0, const glm::vec3& p1)
		{
			const glm::vec3& p2 = pos[0];
			const glm::vec3& p3 = pos[3];

			float dot0232{ glm::dot(p0 - p2, p3 - p2) };
			float dot3210{ glm::dot(p3 - p2, p1 - p0) };
			float dot0210{ glm::dot(p0 - p2, p1 - p0) };
			float dot3232{ glm::dot(p3 - p2, p3 - p2) };
			float dot2121{ glm::dot(p1 - p0, p1 - p0) };

			float numerator = dot0232 * dot3210 - dot0210 * dot3232;
			float denominator = dot2121 * dot3232 - dot3210 * dot3210;
			float mua = numerator / denominator;
			float mub = (dot0232 + dot3210 * (mua)) / dot3232;

			// The line between pointA and pointB is the shortest line segment
			// and can be considered as the intersection
			glm::vec3 pointA = p0 + (mua * (p1 - p0));
			glm::vec3 pointB = p2 + (mub * (p3 - p2));

			// returns a point on the middle of the shortest line segment separating the lines.
			return lerp(pointA, pointB, 0.5f);
		};
		//TriangleIntersection
	#pragma endregion

	#pragma region PushingVertices
		/*
		 __c__
		|    /|
		d  /  b
		|/_a__|
		*/
		// Tests to see if every edge is intersected or only 2 are
		if (positions.size() == 2)
		{	
			if (a && c ||
				a && d ||
				b && c ||
				b && d)
			{
				// Finds the point where the contour line intersects with the quad triangulation diagonal
				glm::vec3 diagonalIntersectionPoint = lerp(pos[0], pos[3], getTriangleIntersectionAlpha(
					getTriangleIntersectionPoint(positions[0], positions[1])));

				pushPoint(index, positions[0]);
				pushPoint(index, diagonalIntersectionPoint);
				pushPoint(index, diagonalIntersectionPoint);
				pushPoint(index, positions[1]);
			}
			else
			{
				// The line does not cross the diagonal where the quad is triangulated so we just draw a straight line 
				pushPoint(index, positions[0]);
				pushPoint(index, positions[1]);
			}
		}
		else if (positions.size() == 4)
		{
			/*
			 __c__
			|    /|
			d  /  b
			|/_a__|
			*/
			glm::vec3 diagonalIntersectionPoint{};
			// a - b
			pushPoint(index, positions[0]);
			pushPoint(index, positions[1]);

			// b - c   The line crosses the quad diagonal, so we draw a point where the lines intersect
			pushPoint(index, positions[1]);
			diagonalIntersectionPoint = lerp(pos[0], pos[3], getTriangleIntersectionAlpha(
				getTriangleIntersectionPoint(positions[1], positions[2])));
			pushPoint(index, diagonalIntersectionPoint);
			pushPoint(index, diagonalIntersectionPoint);
			pushPoint(index, positions[2]);

			// c - d
			pushPoint(index, positions[2]);
			pushPoint(index, positions[3]);

			// d - a  The line crosses the quad diagonal, so we draw a point where the lines intersect
			pushPoint(index, positions[3]);
			diagonalIntersectionPoint = lerp(pos[0], pos[3], getTriangleIntersectionAlpha(
				getTriangleIntersectionPoint(positions[3], positions[0])));
			pushPoint(index, diagonalIntersectionPoint);
			pushPoint(index, diagonalIntersectionPoint);
			pushPoint(index, positions[0]);
		}
		//PushingVertices
	#pragma endregion
	};
	//MarchingSquares
#pragma endregion

#pragma region MainLoop
	int index{ };
	for (int i{ yMin }; i < yMax; i += ekviDistance)
	{
		for (int j{}; j < terrainMesh->m_vertices.size()-columns-1; ++j)
		{
			int caseToDraw{};

			if ((j + 1) % columns == 0)
				continue;

			tempPosition[0] = terrainMesh->m_vertices[j].getPosition();
			tempPosition[1] = terrainMesh->m_vertices[j + 1].getPosition();
			tempPosition[2] = terrainMesh->m_vertices[j + columns].getPosition();
			tempPosition[3] = terrainMesh->m_vertices[j + columns + 1].getPosition();
			drawContourOnSquare(tempPosition, i, index);
		}

	}
	//MainLoop
#pragma endregion
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
