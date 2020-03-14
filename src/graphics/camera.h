#pragma once

#include "vector3.h"

namespace Zen {
class Camera {
public:
  Camera();
  Camera(Vector3 position, double pitch, double yaw, double roll);

  Vector3 get_position();
  double get_pitch();
  double get_yaw();
  double get_roll();
  double get_fov();
  double get_tan_half_fov();

  void set_position(Vector3 position);
  void set_pitch(double pitch);
  void set_yaw(double yaw);
  void set_roll(double roll);
  void set_fov(double fov);

private:
  Vector3 _position;
  double _pitch;
  double _yaw;
  double _roll;
  double _fov;    //This is in Radians
  double tan_half_fov; //This is for speed
};
}
