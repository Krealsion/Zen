#pragma once

#include "vector2.h"

namespace Zen {

class Math {
public:
	constexpr static const double PI   = 3.141592653589793238462643383279502884197;
	constexpr static const double PIo2 = 1.570796326794896619231321691639751442098;
	constexpr static const double PIo4 = 0.785398163397448309615660845819875721049;
	constexpr static const double PIt2 = 6.283185307179586476925286766559005768394;

	static void GenerateTrigLookupTables();
	static double FastSin(double Angle);
	static double FastCos(double Angle);
	static double getRadiansPointsTo(Vector2 From, Vector2 To);
	static double getRadiansPointsTo(Vector2 To);
private:
  constexpr static const int lookup_table_size = 10001;

  static double sine_lookup[lookup_table_size];
  static double cosine_lookup[lookup_table_size];
};
}
