#ifndef LIBMATHS_H_INCLUDED
#define LIBMATHS_H_INCLUDED

// tan(α) = sin(α) / cos(α)
// cos²(aˆ) + sin²(aˆ) = 1 
// Hyponetuse = √(a² + b²)
// cos(aˆ) = Adjacent/Hyponetuse
// sin(aˆ) = Opposite/Hyponetuse
// tan(aˆ) = Opposite/adjacent

#define M_PI_M2    					6.283185307179586476925286766559
#define BAD_DISTANCE                -1

#include <math.h> // Maths library for angles and matrices calculation
#include <stdbool.h>

typedef struct ScreenDimension               ScreenDimension;
typedef struct Vector4                       Vector4;
typedef struct Vector3                       Vector3;
typedef struct Vector2                       Vector2;


struct ScreenDimension {
	int width;
	int height;
};

struct Vector2 {
	float x;
	float y;
};

struct Vector3 {
	float x;
	float y;
	float z;
};

struct Vector4 {
	float x;
	float y;
	float z;
	float w;
};


// Circle Stuffs //
extern  float radiusToDiameter (float _radius);
extern  float diameterToRadius(float _diameter);
extern  float diameterToCircumference(float _diameter);
extern  float radiusToCircumference(float _radius);
extern  float circumferenceToRadius(float _circumference);
// Angle Stuffs // 
extern  float degreeToRadian(float _degree);
extern  float radianToDegree(float _radian);
extern  float computeTanFromCosSin(float _cos, float _sin);
extern  float computeYaw(Vector3 _relativePosition);
extern  float computePitch(Vector3 _relativePosition);
extern  float ac_customPolarCoordinateTan(float _y, float _x); // Based on the game polar coordinate system for the calculate of the yaw angle instead of using atan2f with an offset
extern  float calculateAngularDifference(float _ca,float _ta);
extern float getShortestAngleRotation(float _angle);

// 3D Stuffs //
extern Vector3 getRelativePosition(Vector3 _src, Vector3  _dst);
extern float getDistance(Vector3 _relativePosition);
extern float getDistanceFrom(Vector3 _src, Vector3  _dst);
extern float transformX(const Vector3 _pos,const float _matrix[4][4]);
extern float transformY(const Vector3 _pos,const float _matrix[4][4]);
extern float transformZ(const Vector3 _pos,const float _matrix[4][4]);
extern float transformW(const Vector3 _pos,const float _matrix[4][4]);
extern Vector4 transform(const Vector3 _pos,const float _matrix[4][4]);
extern bool worldToScreenGL(const Vector3 _pos,const ScreenDimension _screenDimension, const float _matrix[4][4], Vector2 *out);
// 2D Stuffs //
extern Vector2 getRelativePosition2d(Vector2 _src, Vector2  _dst);
extern float getDistance2d(Vector2 _relativePosition);
extern float scaleWidthFromDistance(float _distance);
extern float scaleHeightFromDistance(float _distance);
// ratio stuff //
extern float getRatio(float _a,float _b);
extern float angleFromRatio(float _angle, float _ratio);
extern float SmoothAngle(float _from, float _to, float _percent);

#endif
