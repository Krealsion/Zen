#pragma once

#include <string>

namespace Zen {


enum class Layout {
  CHILD_CONTROLLED,
  HORIZONTAL,
  VERTICAL
};

enum class PositionTo {
  RELATIVE, // Values are treated as a normal position on the x, y plane relative to parent
  // Used with Vertical Layouts
  LEFT,
  RIGHT,
  // Used with Horizontal Layouts
  TOP,
  BOTTOM,
  // Used with both
  CENTER,
  PARENT_CONTROLLED,
};

enum DataType {
  BIT,
  STRING,
  NUMBER,
  BOOLEAN
};

enum TextBoxFilterType {
  ANY,
  DATA_TYPE, // Data type is used to determine the type of data that can be entered into the textbox TODO:: This will require a variable data storage system to implement, think about using varius try and catch statements to try the conversion and display an (todo) issue that can be resolved before the data will be reattempted
  INTEGER, // TODO consider a regex for this and make it plugin based so that it can be extended
  EMAIL, // TODO consider a regex for this and make it plugin based so that it can be extended
  PLUGIN
};

enum class SizeTo {
  STATIC = 0,
  PARENT, // Any additional value is treated as a static addition
  PARENT_PERCENT, // Any additional value is treated as a percentage with 1 being 100%
  PARENT_STATIC, // Any additional value is subtracted from the size of the parent
  CHILDREN, // Any additional value is treated as a static addition
  CHILDREN_PERCENT, // Any additional value is treated as a percentage with 1 being 100%
  FILL
};

enum class PaddingTo {
  STATIC,
  PERCENT
};

// Unused
//enum class ScrollDirection {
//  NONE,
//  HORIZONTAL,
//  VERTICAL,
//  BOTH
//};
}
