#include <cmath>
#include <iostream>

#include "light.h"
#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>


using namespace std;

double DirectionalLight::distanceAttenuation(const glm::dvec3& P) const
{
	// distance to light is infinite, so f(di) goes to 0.  Return 1.
	return 1.0;
}


glm::dvec3 DirectionalLight::shadowAttenuation(const ray& r, const glm::dvec3& p) const
{
	// YOUR CODE HERE:
	// You should implement shadow-handling code here.
	glm::dvec3 light_dir = getDirection(p);
	isect light_ray_intersect;
	ray light_ray(glm::dvec3(0,0,0), glm::dvec3(0,0,0), glm::dvec3(1,1,1), ray::SHADOW);
	light_ray.setPosition(p + light_dir * RAY_EPSILON);
	light_ray.setDirection(light_dir);
	if(scene->intersect(light_ray, light_ray_intersect)){
			const Material& m = light_ray_intersect.getMaterial();
			return glm::dvec3(1.0,1.0,1.0) * m.kt(light_ray_intersect) *color;
	}
	return glm::dvec3(1.0, 1.0, 1.0) *color;
}

glm::dvec3 DirectionalLight::getColor() const
{
	return color;
}

glm::dvec3 DirectionalLight::getDirection(const glm::dvec3& P) const
{
	return -orientation;
}

double PointLight::distanceAttenuation(const glm::dvec3& P) const
{

	// YOUR CODE HERE

	// You'll need to modify this method to attenuate the intensity 
	// of the light based on the distance between the source and the 
	// point P.  For now, we assume no attenuation and just return 1.0
	double dist = glm::distance(P, position);
	return min(1.0, 1.0/ (constantTerm + linearTerm * dist + quadraticTerm*dist*dist));
}

glm::dvec3 PointLight::getColor() const
{
	return color;
}

glm::dvec3 PointLight::getDirection(const glm::dvec3& P) const
{
	return glm::normalize(position - P);
}


glm::dvec3 PointLight::shadowAttenuation(const ray& r, const glm::dvec3& p) const
{
	// YOUR CODE HERE:
	// You should implement shadow-handling code here.
	glm::dvec3 light_dir = getDirection(p);
	isect light_ray_intersect;
	ray light_ray(glm::dvec3(0,0,0), glm::dvec3(0,0,0), glm::dvec3(1,1,1), ray::SHADOW);
	light_ray.setPosition(p + light_dir * RAY_EPSILON);
	light_ray.setDirection(light_dir);
	if(getScene()->intersect(light_ray, light_ray_intersect)){
		double t = light_ray_intersect.getT();
		double light_dist = glm::length(position - p);
		double hit_dist = glm::length(light_dir * t);
		if(light_dist > hit_dist){
			const Material& m = light_ray_intersect.getMaterial();
			return glm::dvec3(1.0,1.0,1.0) *  m.kt(light_ray_intersect) * color;
		}
	}
	return glm::dvec3(1,1,1)*color;
}

#define VERBOSE 0

