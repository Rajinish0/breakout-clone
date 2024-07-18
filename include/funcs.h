#ifndef FUNCS_GUARD
#define FUNCS_GUARD
#include <iostream>
#include <string>
#include <tuple>
#include "gameobjects/gameobject.h"
#include "gameobjects/ball.h"
#include "vectorconsts.h"
#include "mesh.h"
	//collided,collided normal vector, the push back vector
typedef std::tuple<bool, glm::vec2, glm::vec2> Collision;
			 /*
			 in the future i can use this third vector to apply an 
			 outward force to the object that is proportional to the length
			 of this vector

			reflecting the ball's vel around this normal vector could
			also work. But since square (or rects) are axis aligned
			(they aren't rotated) reflections are basically equivalent
			to flipping the coordinates --> this comes from conservation of momentum
			when a elastic body collides with an object of infinite mass 
			 */

namespace funcs {
	unsigned int TextureFromFile(const std::string& path, std::string directory);
	unsigned int loadCubeMap(std::vector<std::string> faces);
	Mesh genSphere();
	Mesh genTorus(); // or donut?
	template<typename T>
	T clamp(T val, T lowerb, T upperb);
	bool rectToRectCollision(GameObject& g1, GameObject& g2);
	Collision rectToCircleCollision(GameObject& rect, Ball& circle);
}

#endif