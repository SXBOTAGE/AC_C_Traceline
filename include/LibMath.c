#include "LibMath.h"


// Circle Stuffs //
inline float radiusToDiameter (float _radius) {return _radius * 2.00f;}
inline float diameterToRadius(float _diameter){return _diameter / 2.00f;}
inline float diameterToCircumference(float _diameter) {return _diameter * M_PI; }
inline float radiusToCircumference(float _radius) {return radiusToDiameter(_radius) * M_PI;}
inline float circumferenceToRadius(float _circumference) {return diameterToRadius(_circumference / M_PI);}
// Angle Stuffs //
inline float degreeToRadian(float _degree){ return _degree * M_PI / 180.00f;}
inline float radianToDegree(float _radian){return _radian * 180.00f / M_PI;}
inline float computeTanFromCosSin(float _cos, float _sin) {return _sin / _cos;}
inline float computeYaw(Vector3 _relativePosition){ return radianToDegree(ac_customPolarCoordinateTan(_relativePosition.y,_relativePosition.x));}
inline float computePitch(Vector3 _relativePosition){ return radianToDegree(asinf(_relativePosition.z / getDistance(_relativePosition)));}
inline float ac_customPolarCoordinateTan(float _y, float _x) {

    
/*
    This is the actual representation of the polar coordinate system used by assault cube. 
    This polar coordinate system is inclinated -π/2 from the standard one. 
    This is why you can achieve the same result adding 90.00f at the end of the atan2f computation : 
    This is the atan2f equation that can be used to find the same result : atan2f(_relativePosition.y, _relativePosition.x) + 90.00f;
    
    Polor Coordianates System (AC) : 
            - π
    - 3π/2        - π/2
            - 0

    Quadrant I : π/2 < α < π
    Quadrant II :  π < α < 3π/2
    Quadrant III : 3π/2 < α < 2π
    Quadrant IV : 0 < α < π/2
*/

    // get the relative angle 
    float angle = atanf(_x / _y);

    if(angle < 0) angle = -angle; // if the angle is negative, make it positive 

    if(_x > 0 && _y > 0) return  M_PI - angle; // Quadrant I 
    if(_x < 0 && _y > 0) return (M_PI + M_PI_2) - -(angle - M_PI_2); // Quadrant II
    if(_x < 0 && _y < 0) return M_PI_M2 - angle; // Quadrant III
    if(_x >= 0 && _y <= 0) return angle; // Quadrant IV

    return angle;
}
inline float calculateAngularDifference(float _ca,float _ta) { return getShortestAngleRotation(_ta - _ca);  } // Return shortest angular distance between two angles in range [-180, 180].
inline float getShortestAngleRotation(float _angle) {return fmodf((_angle + 540.0f), 360.0f) - 180.0f;} // if the the distance is -190 is it will return 170. 
inline float angleFromRatio(float _angle, float _ratio) { if(_ratio >= 1) return _angle; return _angle * _ratio; }
// 3D position Stuffs //
inline Vector3 getRelativePosition(Vector3 _src, Vector3  _dst){return (Vector3){_dst.x - _src.x,_dst.y - _src.y,_dst.z - _src.z};}
inline float getDistance(Vector3 _relativePosition){return sqrtf(powf(_relativePosition.x,2) + powf(_relativePosition.y,2) + powf(_relativePosition.z,2));}
inline float getDistanceFrom(Vector3 _src, Vector3  _dst){return getDistance(getRelativePosition(_src,_dst)) ;}
// 3D matrices Stuffs //
inline float transformX(const Vector3 _pos,const float _matrix[4][4]) { return _pos.x * _matrix[0][0] +  _pos.y * _matrix[1][0] + _pos.z * _matrix[2][0] + _matrix[3][0]; }
inline float transformY(const Vector3 _pos,const float _matrix[4][4]) { return _pos.x * _matrix[0][1] +  _pos.y * _matrix[1][1] + _pos.z * _matrix[2][1] + _matrix[3][1]; }
inline float transformZ(const Vector3 _pos,const float _matrix[4][4]) { return _pos.x * _matrix[0][2] +  _pos.y * _matrix[1][2] + _pos.z * _matrix[2][2] + _matrix[3][2]; }
inline float transformW(const Vector3 _pos,const float _matrix[4][4]) { return _pos.x * _matrix[0][3] +  _pos.y * _matrix[1][3] + _pos.z * _matrix[2][3] + _matrix[3][3]; }
inline Vector4 transform(const Vector3 _pos,const float _matrix[4][4]) { 
    Vector4 result;

    result.x = transformX(_pos,_matrix);
    result.y = transformY(_pos,_matrix);
    result.z = transformZ(_pos,_matrix);
    result.w = transformW(_pos,_matrix);

    return result;
}
inline bool worldToScreenGL(const Vector3 _pos,const ScreenDimension _screen, const float _matrix[4][4], Vector2 *out) {


    Vector4 clip = transform(_pos,_matrix);


    if(clip.w < 0.01f) return false;

    // Normalized Device Coordinates (NDC) (-1 to 1)
    clip.x /= clip.w;
    clip.y /= clip.w;

    // Window Coordinates : Scaling it to half the screen dimension
    out->x = clip.x * (_screen.width / 2);
    out->y = clip.y * (_screen.height / 2); 

    // Window Coordinates : Center the coordinates within the screen
    out->x =  (_screen.width / 2) + out->x;
    out->y =  (_screen.height / 2) - out->y;

    return true;
}
// 2D stuff //
inline Vector2 getRelativePosition2d(Vector2 _src, Vector2  _dst){ return (Vector2){_dst.x - _src.x, _dst.y - _src.y};}
inline float getDistance2d(Vector2 _relativePosition){ return sqrtf(powf(_relativePosition.x,2) + powf(_relativePosition.y,2));}
inline float scaleWidthFromDistance(float _distance) {
    
    float referenceDistance = 15.00f; 
    float referenceWidth = 50.00f; 
    float width = referenceWidth * referenceDistance / _distance; 

    return width;

}
inline float scaleHeightFromDistance(float _distance) {
    
    float referenceDistance = 40.00f; 
    float referenceHeight = 100.00f; 
    float width = referenceHeight * referenceDistance / _distance; 

    return width;

}
// Ratio stuff // 
inline float getRatio(float _b,float _a) { return _b / _a; }
inline float SmoothAngle(float _from, float _to, float _percent) { 

    float shAngle = calculateAngularDifference(_from,_to); // Return shortest angular distance between two angles in range [-180, 180].
    float shAngleR = angleFromRatio(shAngle, _percent); // retunr the angle from a given percentage or ratio
    
    
    return _from + shAngleR;
}
