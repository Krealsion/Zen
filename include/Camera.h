//
// Created by jdemoss on 1/16/20.
//

#ifndef LEAVINGTERRA_CAMERA_H
#define LEAVINGTERRA_CAMERA_H

#include "Vector3.h"

class Camera {
public:
    Camera();
    Camera(Vector3 position, double pitch, double yaw, double roll);

    Vector3 GetPosition();
    double GetPitch();
    double GetYaw();
    double GetRoll();
    double GetFOV();
    double GetTanHalfFOV();

    void SetPosition(Vector3 position);
    void SetPitch(double pitch);
    void SetYaw(double yaw);
    void SetRoll(double roll);
    void SetFOV(double fov);

private:
    Vector3 Position;
    double Pitch;
    double Yaw;
    double Roll;
    double FOV;    //This is in Radians
    double TanHalfFOV;
};

#endif //LEAVINGTERRA_CAMERA_H
