# Voxit Editor

Voxit Editor is a 3D voxel modeling software developed with a goal to export models to work with Unity3D.
Developed in c++ and OpenGL.

## Description

Voxit is a little modeling software I developed for fun and a refresher for 3D graphics and C++, after spending so much time in Unity3D and C#. A lot of stuff was added just for learning and my own interest, for example the Cascading Shadow Maps (CSM), a custom made File Explorer and the Model Exporting.
This sofware wasn't designed to be used in actual game development and more just for fun and for other people who wanted too see how I implemented some of the features.  

I've added a few example models in the 'Example' folder, load any .vxt file in Voxit to view.

## Features

* Saving & Loading Models
* Export To Work With Unity3D & Other COLLADA .dae Combatible Software/Engines.
* Useful Drawing Tools (Eyedropper, Gradient, Eraser)
* Lighting & Shadowing
* Handful of Options
* Key Shortcuts


## Controls & Shortcuts

Movement: `W, A, S, D`  
Up & Down: `E, Q`  
Fast Movement: `Left Shift`
 
Undo: `Ctrl + Z`   
Pencil: `B`  
Eye Dropper: `I`  
Eraser: `F`  
Gradient: `G`

## Future Features & Goals

* More Export Types (OBJ, FBX e.g.)
* New Drawing Tools (Different Gradient Patterns, Selecting & Dragging e.g.)
* Multiplayer (Build a model with a friend, for fun?)
* Improved Shadow Mapping
* More Lighting Settings
* Changeable Grid Size
* Clean Up Code
* Optimisations

## Download

You can download and compile the source code for Voxit here or download a precompiled executable on my site [here](http://www.rhysboer.com/project/voxit).

## Libraries Used
* [Stb](https://github.com/nothings/stb) - Image Loading
* [Imgui](https://github.com/ocornut/imgui) - GUI
* [Glm](https://glm.g-truc.net/0.9.9/index.html) - 3D Math Library
* [TinyXML-2](https://github.com/leethomason/tinyxml2) - XML parser
