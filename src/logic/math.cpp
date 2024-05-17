#include "math.h"

#include <cmath>

namespace Zen {

double Math::sine_lookup[lookup_table_size];
double Math::cosine_lookup[lookup_table_size];

void Math::GenerateTrigLookupTables() {
  for (int i = 0; i < lookup_table_size; i++) {	//For each value
    sine_lookup[i] = std::sin(PI * (((double) i / ((double) lookup_table_size / 2)) - 1));
    cosine_lookup[i] = std::cos(PI * (((double) i / ((double) lookup_table_size / 2)) - 1));
  }
}

double Math::FastSin(double Angle) {
  if (Angle > PI) {
    Angle -= PIt2;
  }
  if (Angle < -PI) {
    Angle += PIt2;
  }
  return sine_lookup[(int) ((Angle / PI) * (double) ((lookup_table_size - 1) / 2) + (lookup_table_size - 1) / 2)];
}

double Math::FastCos(double Angle) {
  if (Angle > PI) {
    Angle -= PIt2;
  }
  if (Angle < -PI) {
    Angle += PIt2;
  }
  return cosine_lookup[(int) ((Angle / PI) * (double) ((lookup_table_size - 1) / 2) + (lookup_table_size - 1) / 2)];
}

double Math::getRadiansPointsTo(Vector2 From, Vector2 To) {
  double DifX = To.get_x() - From.get_x();
  double DifY = To.get_y() - From.get_y();
  double Angle;
  if (DifY != 0) {
    Angle = std::atan2(DifY, DifX);
  } else {
    Angle = 0;
    if (DifX < 0) {
      Angle = PI;
    }
  }
  if (Angle > PI) {
    Angle -= PIt2;
  }
  return Angle;
}

double Math::getRadiansPointsTo(Vector2 To) {
  double DifX = To.get_x();
  double DifY = To.get_y();
  double Angle;
  if (DifY != 0) {
    Angle = std::atan2(DifY, DifX);
  } else {
    Angle = 0;
    if (DifX < 0) {
      Angle = PI;
    }
  }
  if (Angle > PI) {
    Angle -= PIt2;
  }
  return Angle;
}
}