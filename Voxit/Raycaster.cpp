#include "Raycaster.h"

RaycastHit Raycaster::VoxelDetection(std::map<int, Voxel>* voxels) {
	glm::vec2 mousePos = Input::MousePosition();
	glm::vec2 screenSize = Window::GetSize();

	// Normalized Device Space
	glm::vec2 mouseNDS = glm::vec2((2.0f * mousePos.x) / screenSize.x - 1.0f, 1.0f - (2.0f * mousePos.y) / screenSize.y);

	// Eye Space														  * Clip Space
	glm::vec4 mouseEye = glm::inverse(Camera::ActiveCamera->Projection()) * glm::vec4(mouseNDS, -1.0f, 1.0f);
	mouseEye.z = -1.0f;
	mouseEye.w = 0.0f;

	// World Space
	glm::vec3 rayDirection = glm::inverse(Camera::ActiveCamera->View()) * mouseEye;
	rayDirection = glm::normalize(rayDirection);

	float cellSize = 1.0f;

	std::vector<glm::vec3> positions = std::vector<glm::vec3>();
	glm::vec3 startPoint = Camera::ActiveCamera->Position() + rayDirection;
	glm::vec3 endPoint = Camera::ActiveCamera->Position() + (rayDirection * 250.0f);    //IntersectPlane(rayDirection, Camera::ActiveCamera->Position(), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -0.5f, 0.0f));


	glm::vec3 startCell = VoxelBuilder::ConvertPosToClosestMapPos(startPoint);	//	glm::vec3(std::round(startPoint.x / cellSize), std::round(startPoint.y / cellSize), std::round(startPoint.z / cellSize));
	glm::vec3 endCell = VoxelBuilder::ConvertPosToClosestMapPos(endPoint);		//	glm::vec3(std::round(endPoint.x / cellSize), std::round(endPoint.y / cellSize), std::round(endPoint.z / cellSize));

	// Direction
	glm::vec3 direction = endPoint - startPoint;

	int step_X = (direction.x >= 0) ? 1 : -1;
	int step_Y = (direction.y >= 0) ? 1 : -1;
	int step_Z = (direction.z >= 0) ? 1 : -1;

	glm::vec3 norm_direction = glm::normalize(direction);

	// Distance to nearest square side
	double near_x = (step_X >= 0) ? ((startCell.x + 1) * cellSize - startPoint.x) + -0.5f : (startPoint.x - (startCell.x * cellSize)) + 0.5f;
	double near_y = (step_Y >= 0) ? ((startCell.y + 1) * cellSize - startPoint.y) + -0.5f : (startPoint.y - (startCell.y * cellSize)) + 0.5f;
	double near_z = (step_Z >= 0) ? ((startCell.z + 1) * cellSize - startPoint.z) + -0.5f : (startPoint.z - (startCell.z * cellSize)) + 0.5f;

	double maxX = (norm_direction.x != 0) ? near_x / norm_direction.x : DOUBLE_MAX; //std::numeric_limits<double>::max(); // X
	double maxY = (norm_direction.y != 0) ? near_y / norm_direction.y : DOUBLE_MAX; //std::numeric_limits<double>::max(); // Y
	double masZ = (norm_direction.z != 0) ? near_z / norm_direction.z : DOUBLE_MAX; //std::numeric_limits<double>::max(); // Z

	double dx = (norm_direction.x != 0) ? cellSize / norm_direction.x : DOUBLE_MAX; // std::numeric_limits<double>::max();
	double dy = (norm_direction.y != 0) ? cellSize / norm_direction.y : DOUBLE_MAX; // std::numeric_limits<double>::max();
	double dz = (norm_direction.z != 0) ? cellSize / norm_direction.z : DOUBLE_MAX; // std::numeric_limits<double>::max();

	glm::vec3 pos = startCell;
	positions.push_back(startCell);

	int gridBoundX = std::abs(endCell.x - startCell.x);
	int gridBoundY = std::abs(endCell.y - startCell.y);
	int gridBoundZ = std::abs(endCell.z - startCell.z);

	int counter = 0;

	while(counter != (gridBoundX + gridBoundY + gridBoundZ)) {
		if(std::abs(maxX) < std::abs(maxY)) {
			if(std::abs(maxX) < std::abs(masZ)) {
				maxX += dx;
				pos.x += step_X;
			} else {
				masZ += dz;
				pos.z += step_Z;
			}
		} else {
			if(std::abs(maxY) < std::abs(masZ)) {
				maxY += dy;
				pos.y += step_Y;
			} else {
				masZ += dz;
				pos.z += step_Z;
			}
		}

		int key = VoxelBuilder::PositionToKey(pos);
		if(VoxelBuilder::IsValidMapPos(pos)) {
			auto voxel = voxels->find(key);
			if(voxel != voxels->end()) {
				return RaycastHit(true, positions.back(), &(*voxel).second);
			}
		}

		if(pos.y < 0) {
			auto voxel = voxels->find(key);
			if(voxel != voxels->end())
				return RaycastHit(true, positions.back(), &(*voxel).second);
			else
				return RaycastHit(false, positions.back() , nullptr);
		}

		++counter;
		positions.push_back(pos);
	}

	return RaycastHit(false, VoxelBuilder::ConvertPosToClosestMapPos(endPoint), nullptr);
}

RaycastHit Raycaster::TEST_VoxelDetection() {
	glm::vec2 mousePos = Input::MousePosition();
	glm::vec2 screenSize = Window::GetSize();

	// Normalized Device Space
	glm::vec2 mouseNDS = glm::vec2((2.0f * mousePos.x) / screenSize.x - 1.0f, 1.0f - (2.0f * mousePos.y) / screenSize.y);

	// Eye Space														  * Clip Space
	glm::vec4 mouseEye = glm::inverse(Camera::ActiveCamera->Projection()) * glm::vec4(mouseNDS, -1.0f, 1.0f);
	mouseEye.z = -1.0f;
	mouseEye.w = 0.0f;

	// World Space
	glm::vec3 rayDirection = glm::inverse(Camera::ActiveCamera->View()) * mouseEye;
	rayDirection = glm::normalize(rayDirection);

	float cellSize = 1.0f;

	std::vector<glm::vec3> positions = std::vector<glm::vec3>();
	glm::vec3 startPoint = Camera::ActiveCamera->Position() + rayDirection;
	glm::vec3 endPoint = Camera::ActiveCamera->Position() + (rayDirection * 250.0f);    //IntersectPlane(rayDirection, Camera::ActiveCamera->Position(), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -0.5f, 0.0f));


	glm::vec3 startCell = VoxelBuilder::ConvertPosToClosestMapPos(startPoint);		//	glm::vec3(std::round(startPoint.x / cellSize), std::round(startPoint.y / cellSize), std::round(startPoint.z / cellSize));
	glm::vec3 endCell = VoxelBuilder::ConvertPosToClosestMapPos(endPoint);			//	glm::vec3(std::round(endPoint.x / cellSize), std::round(endPoint.y / cellSize), std::round(endPoint.z / cellSize));

	// Direction
	glm::vec3 direction = endPoint - startPoint;

	int step_X = (direction.x >= 0) ? 1 : -1;
	int step_Y = (direction.y >= 0) ? 1 : -1;
	int step_Z = (direction.z >= 0) ? 1 : -1;

	glm::vec3 norm_direction = glm::normalize(direction);

	// Distance to nearest square side
	double near_x = (step_X >= 0) ? ((startCell.x + 1) * cellSize - startPoint.x) + -0.5f : (startPoint.x - (startCell.x * cellSize)) + 0.5f;
	double near_y = (step_Y >= 0) ? ((startCell.y + 1) * cellSize - startPoint.y) + -0.5f : (startPoint.y - (startCell.y * cellSize)) + 0.5f;
	double near_z = (step_Z >= 0) ? ((startCell.z + 1) * cellSize - startPoint.z) + -0.5f : (startPoint.z - (startCell.z * cellSize)) + 0.5f;

	double maxX = (norm_direction.x != 0) ? near_x / norm_direction.x : DOUBLE_MAX; //std::numeric_limits<double>::max(); // X
	double maxY = (norm_direction.y != 0) ? near_y / norm_direction.y : DOUBLE_MAX; //std::numeric_limits<double>::max(); // Y
	double masZ = (norm_direction.z != 0) ? near_z / norm_direction.z : DOUBLE_MAX; //std::numeric_limits<double>::max(); // Z

	double dx = (norm_direction.x != 0) ? cellSize / norm_direction.x : DOUBLE_MAX; // std::numeric_limits<double>::max();
	double dy = (norm_direction.y != 0) ? cellSize / norm_direction.y : DOUBLE_MAX; // std::numeric_limits<double>::max();
	double dz = (norm_direction.z != 0) ? cellSize / norm_direction.z : DOUBLE_MAX; // std::numeric_limits<double>::max();

	glm::vec3 pos = startCell;
	positions.push_back(startCell);

	int gridBoundX = std::abs(endCell.x - startCell.x);
	int gridBoundY = std::abs(endCell.y - startCell.y);
	int gridBoundZ = std::abs(endCell.z - startCell.z);

	int counter = 0;

	while(counter != (gridBoundX + gridBoundY + gridBoundZ)) {
		if(std::abs(maxX) < std::abs(maxY)) {
			if(std::abs(maxX) < std::abs(masZ)) {
				maxX += dx;
				pos.x += step_X;
			} else {
				masZ += dz;
				pos.z += step_Z;
			}
		} else {
			if(std::abs(maxY) < std::abs(masZ)) {
				maxY += dy;
				pos.y += step_Y;
			} else {
				masZ += dz;
				pos.z += step_Z;
			}
		}

		Voxel* vox = World::GetVoxel(glm::round(pos));
		if(vox != nullptr) {
			return RaycastHit(true, positions.back(), vox);
		}

		if(pos.y < 0) {
			return RaycastHit(false, positions.back(), nullptr);
		}

		++counter;
		positions.push_back(pos);
	}

	return RaycastHit(false, VoxelBuilder::ConvertPosToClosestMapPos(endPoint), nullptr);
}
