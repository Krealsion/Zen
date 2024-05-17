#pragma once

#include "src/graphics/user_interface/custom_layout.h"
#include "texture.h"
#include "types/vector2.h"
#include "types/rectangle.h"

namespace Zen {

/*
 * A drawable object that can do the following
 * - Be resized
 * - Be rotated around an arbitrary point
 */
class Sprite : public CustomLayout {
public:
  Sprite() = default;
  Sprite(std::shared_ptr<Texture> texture, Vector2 position) : _texture(std::move(texture)) {}
  Sprite(const std::string& texture_path, Vector2 position) : _texture(std::make_shared<Texture>(texture_path)) {}

  virtual ~Sprite() = default;

  void draw(GameGraphics& game_graphics) override {
    game_graphics.draw_texture(_texture, _destination, _flips, _origin, _rotation, _clipping);
  }

  virtual void set_texture(std::shared_ptr<Texture> texture) {
    _texture = std::move(texture);
  }
  virtual void set_clipping(const Rectangle& clipping) {
    _clipping = clipping;
  }
  virtual void set_position(const Vector2& position) {
    _destination.set_position(position);
  }
  virtual void set_origin(const Vector2& origin) {
    _origin = origin;
  }
  virtual void set_angle(float angle) {
    _rotation = angle;
  }

  virtual void set_scale(float scale) {
    auto base_size = _texture->get_size();
    _destination.set_size(base_size * scale);
  }
  virtual void set_scale(float scale_x, float scale_y) {
    auto base_size = _texture->get_size();
    _destination.set_width(base_size.get_x() * scale_x);
    _destination.set_height(base_size.get_y() * scale_y);
  }
  virtual void set_scale(const Vector2& scale) {
    auto base_size = _texture->get_size();
    _destination.set_size(base_size * scale);
  }
  virtual void set_scale_x(float scale) {
    auto base_size = _texture->get_size();
    _destination.set_width(base_size.get_x() * scale);
  }
  virtual void set_scale_y(float scale) {
    auto base_size = _texture->get_size();
    _destination.set_height(base_size.get_y() * scale);
  }

  bool is_flipped_x() {
    return _flips.first;
  }
  void set_flip_x(bool should_flip) {
    _flips.first = should_flip;
  }
  void flip_x() {
    set_flip_x(!is_flipped_x());
  }

  bool is_flipped_y() {
    return _flips.second;
  }
  void set_flip_y(bool should_flip) {
    _flips.second = should_flip;
  }
  void flip_y() {
    set_flip_y(!is_flipped_y());
  }

private:
  void _init_texture() {
    _destination.set_size(_texture->get_size());
    _clipping.set_size(_destination.get_size());
    _clipping.set_position(Vector2());
    _origin = _destination.get_size() * 0.5f;
  }

  std::shared_ptr<Texture> _texture;

  Rectangle _clipping = Rectangle();
  Rectangle _destination = Rectangle();
  Vector2 _origin = Vector2();
  float _rotation = 0;
  float _scale = 1;
  std::pair<bool, bool> _flips;
};
}
