#include "cubeMap.h"
#include "ray.h"
#include "../ui/TraceUI.h"
#include "../scene/material.h"
extern TraceUI* traceUI;

glm::dvec3 CubeMap::getColor(ray r) const
{
	// YOUR CODE HERE
	// FIXME: Implement Cube Map here
	glm::dvec3 ray_out = r.getDirection();
	glm::dvec3 input_position = r.getPosition();
	ray_out /= std::max(std::max(abs(ray_out.x),abs(ray_out.y)),abs(ray_out.z));
	int resultIndex;
	glm::dvec2 proj;
	// chose the face of the map the ray is pointing to
	if(ray_out.x == 1.0){
		proj = glm::dvec2(input_position.y, -input_position.z);
		return tMap[0] -> getMappedValue(proj);
	}else if(ray_out.x == -1.0){
		proj = glm::dvec2(input_position.y, input_position.z);
		return tMap[1] -> getMappedValue(proj);
	}else if(ray_out.y == 1.0){
		proj = glm::dvec2(-input_position.z, input_position.x);
		return tMap[2] -> getMappedValue(proj);
	}else if(ray_out.y == -1.0){
		proj = glm::dvec2(input_position.z, input_position.x);
		return tMap[3] -> getMappedValue(proj);
	}else if(ray_out.z == 1.0){
		proj = glm::dvec2(input_position.y, input_position.x);
		return tMap[4] -> getMappedValue(proj);
	}else{
		proj = glm::dvec2(input_position.y, -input_position.x);
		return tMap[5] -> getMappedValue(proj);
	}
}

CubeMap::CubeMap()
{
}

CubeMap::~CubeMap()
{
}

void CubeMap::setNthMap(int n, TextureMap* m)
{
	if (m != tMap[n].get())
		tMap[n].reset(m);
}
