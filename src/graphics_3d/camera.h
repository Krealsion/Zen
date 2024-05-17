#pragma once

#include "vector3.h"

namespace Zen {

class Camera {
public:
	Camera();
	Camera(Vector3 Pos, double Pitch, double Yaw, double Roll);

	void set_position(Vector3 pos);
	void set_pitch(double pitch);
	void set_yaw(double yaw);
	void set_roll(double roll);
	void set_fov(double fov);

	[[nodiscard]] Vector3 get_pos() const;
	[[nodiscard]] double get_pitch() const;
	[[nodiscard]] double get_yaw() const;
	[[nodiscard]] double get_roll() const;
	[[nodiscard]] double get_fov() const;
	[[nodiscard]] double get_tan_half_fov() const;

private:
  Vector3 _pos;
  double _pitch;
  double _yaw;
  double _roll;
  double _fov;	//This is in Rad
  double _tan_half_fov;
};
}