#pragma once
#include <windows.h>
#include <string>
#include <set>
#include <map>
#include "DataManager.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "Icons.h"

#include "World.h"

class FileHandler {
public:
	FileHandler();
	~FileHandler();

	enum class Window {
		FILE_OPEN,
		FILE_SAVE,
		FILE_EXPORT,
		NONE,
	};

	void OpenWindow(FileHandler::Window window);
	void ShowOpenWindow(World& world);

	bool IsWindowOpen() const;
private:
	struct value_compare {
		bool operator() (const std::string& a, const std::string& b) const {
			return ToLower(a) < ToLower(b);
		}

		std::string ToLower(const std::string& string) const {
			std::string temp = string;
			for(int i = 0; i < string.size(); i++)
				if(temp[i] >= 'A' && temp[i] <= 'Z')
					temp[i] = temp[i] + 32;
			return temp;
		}
	};

	enum MessageBoxType {
		ERROR_,
		SUCCESS
	};

	// Returns true if file was opened
	bool LoadFileDialog(World& world); // std::vector<Voxel>& data
	// Returns true if file was successfully saved
	bool SaveFileDialog(World& world); // const std::map<int, Voxel>& data
	// Returns true if file was exported to OBJ successfully
	bool ExportFileDialog(World& world); // const std::map<int, Voxel>& data

	void Explorer(const char* filter);

	// Parameters:
	// directory - Next directory down to search, send ".." to go back a directory if possible.
	// set - send true if you want to hard set the directory, returns false if directory isnt found
	bool NextDirectory(std::string directory, bool set);

	// Updates 'folders' and 'files' variables, and returns false if directory isn't found
	bool SearchDirectory(const char* filter = nullptr);

	bool HasFileExtension(const char* name, const char* extension) const;

	void SetPopupMessage(const char* message, MessageBoxType type);
	void ShowMessageBox();

	// Variables
	Window currentOpen;

	std::string currentDirectory;
	std::string directoryBar;
	std::string selectedFileName;
	bool hasSearched;

	bool hasMessage;
	MessageBoxType messageType;
	std::string message;

	std::set<std::string, value_compare> folders = std::set<std::string, value_compare>();
	std::set<std::string, value_compare> files	= std::set<std::string, value_compare>();
};