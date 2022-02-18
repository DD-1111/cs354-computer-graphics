#include "material.h"
#include "../ui/TraceUI.h"
#include "light.h"
#include "ray.h"
extern TraceUI* traceUI;

#include <glm/gtx/io.hpp>
#include <iostream>
#include "../fileio/images.h"

using namespace std;
extern bool debugMode;

Material::~Material()
{
}

// Apply the phong model to this point on the surface of the object, returning
// the color of that point.
glm::dvec3 Material::shade(Scene* scene, const ray& r, const isect& i) const
{
	// YOUR CODE HERE

	// For now, this method just returns the diffuse color of the object.
	// This gives a single matte color for every distinct surface in the
	// scene, and that's it.  Simple, but enough to get you started.
	// (It's also inconsistent with the phong model...)

	// Your mission is to fill in this method with the rest of the phong
	// shading model, including the contributions of all the light sources.
	// You will need to call both distanceAttenuation() and
	// shadowAttenuation()
	// somewhere in your code in order to compute shadows and light falloff.
	//	if( debugMode )
	//		std::cout << "Debugging Phong code..." << std::endl;

	// When you're iterating through the lights,
	// you'll want to use code that looks something
	// like this:
	// 
	// for ( const auto& pLight : scene->getAllLights() )
	// {
	//              // pLight has type unique_ptr<Light>
	// 		.
	// 		.
	// 		.
	// }
	glm::dvec3 P = r.getPosition();
	glm::dvec3 d = r.getDirection();
	glm::dvec3 N = i.getN();
	double t = i.getT();
	double sn = shininess(i);
	glm::dvec3 Q = P + d*t;
	glm::dvec3 I = ke(i) + ka(i) * scene -> ambient();
	for ( const auto& pLight : scene->getAllLights() ){
		auto dist_att = pLight -> distanceAttenuation(Q);
		auto shadow_att = pLight -> shadowAttenuation(r,Q);
		auto direction = pLight -> getDirection(Q);
		glm::dvec3 atten = dist_att * shadow_att;
		glm::dvec3 R_light = 2 * glm::dot(direction, N) * N - direction;
		glm::dvec3 kt = kd(i) * max(glm::dot(direction, N) ,0.0);
		kt += ks(i) * pow(max(glm::dot(R_light,glm::normalize(P-Q)), 0.0), sn);
		I += atten * kt;
	}
	return I;
}

TextureMap::TextureMap(string filename)
{
	data = readImage(filename.c_str(), width, height);
	if (data.empty()) {
		width = 0;
		height = 0;
		string error("Unable to load texture map '");
		error.append(filename);
		error.append("'.");
		throw TextureMapException(error);
	}
}

glm::dvec3 TextureMap::getMappedValue(const glm::dvec2& coord) const
{
	// YOUR CODE HERE
	//
	// In order to add texture mapping support to the
	// raytracer, you need to implement this function.
	// What this function should do is convert from
	// parametric space which is the unit square
	// [0, 1] x [0, 1] in 2-space to bitmap coordinates,
	// and use these to perform bilinear interpolation
	// of the values.
	if(coord[0] == floor(coord[0]) && coord[1] == floor(coord[1])){
		return getPixelAt(coord[0],coord[1]);
	}
	int x = (int)(coord[0] * width);
	int y = (int)(coord[1] * height);
	// bilinear interpolation to get the pixel value, code here is less elegant but straight forward
	if( x >= width )
       x = width - 1;
    if( y >= height )
       y = height - 1;  
   	int pos = (y * width + x) * 3;
   	glm::dvec3 interp_result(double(data[pos])/255.0, double(data[pos+1])/255.0, double(data[pos+2])/255.0);
	return interp_result;
}

glm::dvec3 TextureMap::getPixelAt(int x, int y) const
{
	// YOUR CODE HERE
	//
	// In order to add texture mapping support to the
	// raytracer, you need to implement this function.

	if(x > height){
		x = height;
	}
	if(y > width){
		y = width;
	}
	// calculate the color pixel from the image information
	int index_cord = ( x + y * width)*3;
	return glm::dvec3((double)(data[index_cord])/255.0, 
					(double)(data[1 + index_cord] )/255.0, 
					(double)(data[2 + index_cord]) /255.0);
}

glm::dvec3 MaterialParameter::value(const isect& is) const
{
	if (0 != _textureMap)
		return _textureMap->getMappedValue(is.getUVCoordinates());
	else
		return _value;
}

double MaterialParameter::intensityValue(const isect& is) const
{
	if (0 != _textureMap) {
		glm::dvec3 value(
		        _textureMap->getMappedValue(is.getUVCoordinates()));
		return (0.299 * value[0]) + (0.587 * value[1]) +
		       (0.114 * value[2]);
	} else
		return (0.299 * _value[0]) + (0.587 * _value[1]) +
		       (0.114 * _value[2]);
}
