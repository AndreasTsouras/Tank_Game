
#ifndef TANKGAME_SPHERICALCAMERAMANIPULATOR_H
#define TANKGAME_SPHERICALCAMERAMANIPULATOR_H


#include "Matrix.h"
#include "Vector.h"


class SphericalCameraManipulator
{
public:

    float pan;
    float tilt;
    float radius;
    float azimuth;  // Angle of rotation around the vertical axis (pan)
    float zenith;   // Angle measured from the vertical axis (tilt)

    //!
    SphericalCameraManipulator();

    //!
    ~SphericalCameraManipulator();

    //!
    void setPanTiltRadius(float pan, float tilt, float radius);
    
    //!
    Matrix4x4 apply(Matrix4x4 matrix);

    //!
    void handleMouse(int button, int state, int x, int y);
    
    //!
    void handleMouseMotion(int x, int y);    

    //!
    void handleKeyboard(char key, int x, int y);

    //! 
    void setFocus(Vector3f focus);
      
    //!
    float getPan();

    //!
    float getTilt();
    
    //!
    float getRadius();

    // Get the azimuth angle (horizontal rotation or pan)
    float getAzimuth() const {
        return azimuth;
    }

    // Get the zenith angle (vertical rotation or tilt)
    float getZenith() const {
        return zenith;
    }

    Matrix4x4 getMVMatrix();

    Matrix4x4 getProjectionMatrix() const;
private:

    //!
    void enforceRanges();    
    
    //!
    Matrix4x4 transform();

    Matrix4x4 projectionMatrix;

private:

    Vector3f focus;

    float panTiltStep;
    float panTiltMouseStep;
    float tiltRange[2];


    float radiusStep;
    float radiusMouseStep;
    float radiusWheelStep;
    float radiusRange[2];
    
    int currentButton;
    int currentState;
    float previousMousePosition[2];
    bool reset;

};


#endif //TANKGAME_SPHERICALCAMERAMANIPULATOR_H

