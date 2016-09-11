Daniel Souza
dsouza@digipen.edu
50017506

March 17, 2010

CS 460
Project 3 - Inverse Kinematics


-------------------------------------------------

Contents
-------------------------------------------------

- Assignment 3 Notes
- Assignment 2 Notes
- Path Algorithms
- Models and Format
- Interpolation Algorithms
- Viewer Controls


-------------------------------------------------

Assignment 3 Notes
-------------------------------------------------

I Implemented the Cyclic Coordinate Descent (CCD) solution to simulate inverse kinematics.
Implementation code is in the file IKSolver.cpp and IKSolver.h

The wrist / palm joint is the end effector, CCD being applied 6 bones back, to the waist / spine.		

The bone update order is modified to simulate better motion as follows:
unsigned int bone_order[] = { 1, 3, 0, 4, 5, 2, 6 };	
1 - elbow			
3 - shoulder			
0 - wrist			
4 - upper spine			
5 - lower spine			
2 - "fake" arm bone (bone on the model between elbow and shoulder)	
6 - waist	

New Controls:

The arrow keys ( up, down, left, right ) are used to move the "bomber" figure around
on the X-Z plane.

Pressing the spacebar will cause the current bomber location to become the target for
the end effector, and the charecter will walk close enough before reaching out for it.

Standard camera controls are still in place, as described at the end of this document.
		


-------------------------------------------------

Assignment 2 Notes
-------------------------------------------------

- All path algorithms and code is contained in "PathController.h" and .cpp files
- Rendering and actual path location / velocity lookup are done in
  the GraphicsEngine.cpp update function right before rendering takes place

-------------------------------------------------

Path Algorithms
-------------------------------------------------

- Bezier Curve used to generate path from specified points
- Points are chosen inside GraphicsEngine.cpp constructor

- Arc length lookup table is generated using the adaptive approach discussed in class
- Inverse lookup is done by a binary search
- Final parameter return value is an interpolated value between two closest values
  based on path distance between two values

- Position-time / Velocity-time functions use constant acceleration / deceleration
  ease-in / ease-out function to return a distance value [0..1] along the path
  or velocity value [0..1] in the velocity-time function's case
- Model animation speed is based off of the velocity-time function's output
- The Center-of-Interest is determined as a look-ahead point on the path
  using a slightly larger distance value sent to the arc length lookup table

-------------------------------------------------

Models and Format
-------------------------------------------------

Models used in the project are from the FBX demo project found on GameCentral.
They are fully skinned and textured animated models:
 - Tad Studbody
 - Bomb Creature

Models are stored in a binary FBX format as follows:

FBX file type header

mesh_header		{ mesh tag, mesh chunk size } { uint, uint }
  vertex type		{ 0, 1 } { uint }

  number of indices	{ uint }
  index data		{ uint * number_of_indices }
	
  number of verts	{ uint }
  vert data		{ uint * number_of_verts }

skeleton_header		{ mesh tag, mesh chunk size } { uint, uint }
  number of bones 	{ uint }
  {
    bone_name		{ std::string }
    parent_bone		{ uint }
    bone_translation	{ float, float, float }
    bone_rotation	{ float, float, float, float }
    inverse_translation	{ float, float, float }
    inverse_rotation	{ float, float, float, float }
  } ( * number_of_bones )

( any number of animations )
animation_header	{ mesh tag, mesh chunk size } { uint, uint }
  animation_duration 	{ float }
  number_of_tracks 	{ uint }
  {
    number_of_frames	{ uint }
    {
      frame_time	{ float }
      translation	{ float, float, float }
      rotation		{ float, float, float, float }
    } ( * number_of_frames )
  } ( * number of tracks )

-------------------------------------------------

Interpolation Algorithms
-------------------------------------------------

Both rotations and translations for the model are interpolated between keyframes
using simple linear interpolation.	

-------------------------------------------------

Viewer Controls
-------------------------------------------------

  ESC - Exit the program

  1 - Load "Tad Studbody" model
  2 - Load "Bomber Creature" model
  Q - Toggle rendering of the model
  W - Toggle between animation and model's default bind pose
  E - Toggle rendering of the model's bones

  Camera Controls ( numpad )
  8, 2, 4, 6 - Move forwards, backwards, left and right along the camera's current axis
  0 - rotate counter-clockwise around the camera's y-axis
  . - rotate clockwise around the camera's y-axis
  5 - reset the camera to the default position




