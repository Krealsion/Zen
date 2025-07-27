#pragma once
#include "custom_layout.h"

#include "zsignal.h"

namespace Zen {
class Text;
class TextBox : public CustomLayout {
public:
  struct TextBoxFilter {
    TextBoxFilterType type = TextBoxFilterType::ANY;
    DataType data_type = DataType::STRING; // Default to string, used when type is DATA_TYPE

    TextBoxFilter& operator =(TextBoxFilterType _type) {
      this->type = _type;
      return *this;
    }
    // Implicit conversion operator to TextBoxFilterType
    operator TextBoxFilterType() const {
      return type;
    }
  };

  Signal on_text_changed;
  Signal on_text_committed;

  TextBox();
  ~TextBox();

  void set_focused(bool focused);
  void set_text(const std::string& text);
  std::string get_text();

  void set_filter(TextBoxFilter filter) {
    _filter = filter;
  }
  void set_filter(TextBoxFilterType filter_type, DataType data_type = DataType::STRING) {
    _filter.type = filter_type;
    _filter.data_type = data_type;
  }

private:
  std::string _text_string;
  Text* _text = nullptr;
  bool _focused = false;
  TextBoxFilter _filter;

  void update_text();
  void _process_text(const std::string& text);
};
}
