#include "text_box.h"

#include "input.h"
#include "text.h"

namespace Zen {

TextBox::TextBox() {
  CustomLayout::set_background_color(Color(0,0,0));
  auto* inner = new CustomLayout();
  inner->set_size(SizeTo::PARENT, 2, SizeTo::PARENT, 2);
  inner->center();
  inner->set_on_click_callback(std::function<void()>([&]() {
    set_focused(true);
  }));
  this->add_child(inner);

  _text = new Text();
  _text->set_position(PositionTo::LEFT, PositionTo::CENTER);
  _text->set_wrap(false);
  inner->add_child(_text);
}

TextBox::~TextBox() {
}

void TextBox::set_focused(bool focused) {
  _focused = focused;
  if (_focused) {
    Input::start_text_input(Action<const std::string&>([this](const std::string& text) {
      _process_text(text);
    }));
  } else {
    Input::end_text_input();
    on_text_committed();
  }
}

void TextBox::_process_text(const std::string& text) {
  if (text == "\0") {
    set_focused(false);
    return;
  }
  if (_filter.type == TextBoxFilterType::ANY) {
    _text_string += text;
  } else if (_filter == TextBoxFilterType::DATA_TYPE && _filter.data_type == DataType::NUMBER ) {
    // Only allow numbers
    for (char c : text) {
      if (c == '-') {
        // Swap sign if the first character is a minus sign
        if (_text_string.empty() || (_text_string.size() == 1 && _text_string[0] == '-')) {
          _text_string = "-" + _text_string;
        } else if (!_text_string.empty() && _text_string[0] == '-') {
          _text_string.erase(0, 1);
        }
      } else if (c == '.') {
        // only allow one decimal point
        if (_text_string.find('.') == std::string::npos) {
          _text_string += c;
        }
      } else if (isdigit(c)) {
        _text_string += c;
      }
    }
  } else if (_filter == TextBoxFilterType::DATA_TYPE && _filter.data_type == DataType::BIT) {
    // Only allow 0 or 1
    for (char c : text) {
      if (c == '0' || c == '1') {
        _text_string += c;
      } else if (c == '-') {
        // Swap sign if the first character is a minus sign
        if (_text_string.empty() || (_text_string.size() == 1 && _text_string[0] == '-')) {
          _text_string = "-" + _text_string;
        } else if (!_text_string.empty() && _text_string[0] == '-') {
          _text_string.erase(0, 1);
        }
      }
    }
  } else if (_filter == TextBoxFilterType::INTEGER) {
    for (char c : text) {
      if (c == '-') {
        // Swap sign if the first character is a minus sign
        if (_text_string.empty() || (_text_string.size() == 1 && _text_string[0] == '-')) {
          _text_string = "-" + _text_string;
        } else if (!_text_string.empty() && _text_string[0] == '-') {
          _text_string.erase(0, 1);
        }
      } else if (isdigit(c)) {
        _text_string += c;
      }
    }
  }
  on_text_changed();
  update_text();
}

void TextBox::set_text(const std::string& text) {
  _text_string = text;
  update_text();
}

std::string TextBox::get_text() const {
  return _text_string;
}

void TextBox::update_text() {
  _text->set_text(_text_string);
}

}
