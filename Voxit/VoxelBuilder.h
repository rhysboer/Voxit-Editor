#pragma once
#include <Vector>
#include <map>
#include "CircularBuffer.h"

#include "imgui.h"
#include "imgui_glm.h"

#include "Raycaster.h"
#include "Input.h"

#include "Selector.h"
#include "Voxel.h"
#include "Grid.h"

#include "FileHandler.h"
#include "Icons.h"

#include "Settings.h"

#define VOXEL_VERTICES_SIZE 24



#include "World.h"
#include "ShadowMapping.h"
#include "Renderer.h"

struct HistoryNode {
	enum class UndoType { ADD, REMOVE };
	std::vector<Voxel> voxels = std::vector<Voxel>();
	UndoType type = UndoType::ADD;
};

class VoxelBuilder {
public:
	VoxelBuilder();
	~VoxelBuilder();

	void OnUpdate();
	void Render(Camera* camera);

	static glm::vec3 ConvertPosToClosestMapPos(glm::vec3 pos);	// Converts position to a position in the grid
	static glm::vec3 ClosestPosInsideMap(glm::vec3 pos);		// Converts position to the closest position inside the map
	static int PositionToKey(glm::vec3 position);				// Converts grid position to a map key
	static bool IsValidMapPos(glm::vec3 pos);					// Returns true if grid position is inside map 

private:
	enum class ToolIDs {
		PLACE,
		COLOUR,
		ERASE,
		GRADIENT,

		COUNT
	};

	// FUNCTIONS
	void DrawGUI();			// Draw UI

	// Tool Functions
	void ToolPlace();
	void ToolEyedropper();
	void ToolEraser();
	void ToolGradient();

	//void AddVoxels(Voxel* voxels, int size);					// Add voxels to renderer
	//Voxel CreateVoxel(glm::vec3 pos, glm::vec3 colour);			// Create a voxel

	void ClearAndSetButtonState(ToolIDs set);

	//void ClearVoxelData();

	//void AddToHistory(Voxel* voxels, int size, UndoHistory::UndoType type);
	void AddToHistory(std::vector<Voxel>& voxels, HistoryNode::UndoType type);

	// VARIABLES
	static const int mapSize = 75;
	const unsigned int historySize = 20;

	Selector selector;
	Grid grid;

	// Voxel Stuff
	std::map<int, Voxel> voxelMap = std::map<int, Voxel>();
	std::vector<Voxel> loadedVoxelData = std::vector<Voxel>();	// Loaded data from a file
	glm::vec3 voxelColour;			// base voxel colour
	glm::vec3 voxelColourSecondary; // Secondary voxel colour
	unsigned int voxelPositionVBO;		// buffer to voxel data
	int voxelMapSize;				// amount of voxels in map

	CircularBuffer<std::shared_ptr<HistoryNode>> history;

	bool isSelecting;

	FileHandler fileHandler;
	ToolIDs currentTool;

	// ICONS
	bool buttonStates[(unsigned int)ToolIDs::COUNT] = { false };

	// TESTING SHIT

	int offset = 0;
	int offsetZ = 0;
	int offsetY = 0;

	glm::mat4 d_rotation = glm::mat4(1.0f);
	unsigned int VBO, VAO, EBO;
};


// TODO
/*
	Bugs:
	- FileHandler: find illegal characters in save filename (https://gist.github.com/doctaphred/d01d05291546186941e1b7ddc02034d3)
	- Look for memory leaks

	Tasks:
	- Change rendering to chunk based meshes			[DONE]
	- Finish GUI, (add coordinates, 'file' buttons)
	- Hotkeys for drawing			[DONE]
	- Change Enable Lighting into the W component of the vec4 of the light direction
	- Working Shadow maps
	- Adjustible Camera Speed
	- Change shader so it isnt using "Use Program" each time the function is called. Instead just force the user to call it before hand
	- ShadowMapping: Change _End, add a Clear Depth Buffer and use End to resize the resolution after all the depth buffers have been drawn too
*/