#pragma once

namespace Zen {


enum class Layout {
  NONE,
  HORIZONTAL,
  VERTICAL
};

enum class PositionTo {
  NONE,
  // Used with Vertical Layouts
  LEFT,
  RIGHT,
  // Used with Horizontal Layouts
  TOP,
  TOP_LEFT,
  TOP_RIGHT,
  BOTTOM,
  BOTTOM_LEFT,
  BOTTOM_RIGHT,
  // Used with both
  CENTER,
  PARENT_CONTROLLED,
  RELATIVE // Values are treated as a normal position on the x, y plane relative to parent
};

enum class SizeTo {
  NONE,
  PARENT, // Any additional value is treated as a static addition
  PARENT_PERCENT, // Any additional value is treated as a percentage with 1 being 100%
  CHILDREN, // Any additional value is treated as a static addition
  CHILDREN_PERCENT, // Any additional value is treated as a percentage with 1 being 100%
  STATIC
};

enum class PaddingTo {
  NONE,
  STATIC,
  PERCENT
};

// Unused
//enum class Direction {
//  NONE,
//  HORIZONTAL,
//  VERTICAL,
//  BOTH
//};
}
