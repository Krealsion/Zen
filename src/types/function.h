#pragma once

#include <string>
#include <timer.h>
#include <anything_storage.h>
#include <vector>
#include "vector2.h"
#include <random>
//#include "anything_storage.h"

namespace Zen {
/*
 * An input needs to be a specific type
 * Output also needs to be a specific type
 */

/*
 * Functions need to be able to be implicitly converted to a pointer of their value, which will
 *
 *
 * Maybe functions themselves explicitly convert to pointers to their output type
 * but then how do they get updated?
 *
 * noise_function.set_x(Timer::get_current_time);
 *
 * linear_function.set_intercept(noise_function);
 *
 * When the function is dereferenced is when the value will get updated!!!!
 *
 *
 */

/*
 * Findings:
 * All functions only have one output
 * This output can be anything
 * All functions can be directly converted into a pointer of "FunctionOutput<output_type>"
 */

/*
 * EmissionField emission;
 *
 * Function::Linear flicker_increase;
 * flicker_increase.set_intercept(1);
 * flicker_increase.set_slope(.001);
 *
 * Function::Noise emission_flicker;
 * emission_flicker.set_scale(flicker_increase);
 * emission_flicker.set_strength(10);
 *
 * so, when the game goes to draw, it will try and get the value from emission_flicker to see how bright the emission should be
 * emission_flicker is storing a pointer to a memory location of the data of a type that can be implicitly converted to the data
 *
 * two capture points
 * when the data is dereferenced, which is unable to be captured once it is a double or such
 * When the FunctionOutput is implicitly converted
 * Why cant this just be the function?
 * it can
 * When the function is passed, it is passed as
 * As long as it implicitly converts to a type, will it still pass?
 * If its a double pointer will it still pass as like a barrier?
 * int** val = function;
 *
 * You must be able to pass a single type as a setter and have it go through, but it is a reference
 *
 */
template<typename T>
class SmartIterator : public T {
  // Interesting
  //Possibly used to keep the objects as are, and then adding a next and previous to the iterator?
};

class Noise {
public:
  AnythingStorage<double> scale;
  AnythingStorage<double> strength;

  std::vector<Vector2> points;

  AnythingStorage<double> last_x;


  Noise() {
    last_x.set_c(0);
  }

  operator double() {
    return get(last_x);
  }

  void gen_to(double x) {
    if (points.empty()) {
      points.emplace_back(0, ((double) rand() / (RAND_MAX) - .5) * strength * 2);
    }
    double last = points[points.size() - 1].get_x();
    while (last < x) {
      double x_increase = ((double) rand() / (RAND_MAX)) * scale;
      double y = ((double) rand() / (RAND_MAX) - .5) * strength * 2;
      Vector2 p(last + x_increase, y);
      points.push_back(p);
      last = p.get_x();
    }
  }

  double get(double x) {
    gen_to(x);
    if (points.size() > 2) {
      for (long i = points.size() - 1; i > 0; i--) {
        if (points[i].get_x() < x && points[i + 1].get_x() > x) {
          Vector2 p1 = points[i];
          Vector2 p2 = points[i + 1];
          double slope = (p1.get_y() - p2.get_y()) / (p1.get_x() - p2.get_x());
          double intercept = p1.get_y() - slope * p1.get_x();
          return slope * x + intercept;
        }
      }
    }
    return 69; // Should never be reached
  }
};
}
