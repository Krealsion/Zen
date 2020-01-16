//
// Created by jdemoss on 1/16/20.
//
#include "Camera.h"
#include <math.h>

Camera::Camera() {
    Position = Vector3(0, 0, 0);
    Pitch = 0;
    Yaw = 0;
    Roll = 0;
    FOV = M_PI * 2 / 3;
    TanHalfFOV = tan(FOV / 2);
}

Camera::Camera(Vector3 position, double pitch, double yaw, double roll) {
    Position = position;
    Pitch = pitch;
    Yaw = yaw;
    Roll = roll;
    FOV = M_PI * 2 / 3;
    TanHalfFOV = tan(FOV / 2);
}

void Camera::SetPosition(Vector3 position) {
    Position = position;
}

void Camera::SetPitch(double pitch) {
    Pitch = pitch;
}

void Camera::SetYaw(double yaw) {
    Yaw = yaw;
}

void Camera::SetRoll(double roll) {
    Roll = roll;
}

void Camera::SetFOV(double fov) {
    FOV = fov;
    TanHalfFOV = tan(FOV / 2);
}

Vector3 Camera::GetPosition() {
    return Position;
}

double Camera::GetPitch() {
    return Pitch;
}

double Camera::GetYaw() {
    return Yaw;
}

double Camera::GetRoll() {
    return Roll;
}

double Camera::GetFOV() {
    return FOV;
}

double Camera::GetTanHalfFOV() {
    return TanHalfFOV;
}
