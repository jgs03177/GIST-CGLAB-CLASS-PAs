#ifndef __IMAGE_H_
#define __IMAGE_H_

#include <glm/glm.hpp>
#include <vector>

#include <glm/gtx/string_cast.hpp>
#include <iostream>

namespace ne {

// this is just simple 2D forward iterator for iterate indices
class TileIterator {
  glm::uvec2 start_;
  glm::uvec2 end_;
  glm::uvec2 current_;

public:
  TileIterator() {}
  TileIterator(const glm::uvec2 &s, const glm::uvec2 e)
      : current_(s), start_(s), end_(e) {}

  TileIterator(const glm::uvec2 &c, const glm::uvec2 &s, const glm::uvec2 e)
      : current_(c), start_(s), end_(e) {}

  TileIterator begin() const { return TileIterator{start_, end_}; };
  TileIterator end() const { return TileIterator{end_, start_, end_}; };

  TileIterator &operator++() {
    increment();
    return *this;
  }

  bool operator!=(const TileIterator &other) const {
    return !(current_ == other.current_);
  }

  const glm::uvec2 &operator*() const { return current_; }

  inline void increment() {
    if (++current_.x == end_.x && ++current_.y != end_.y)
      current_.x = start_.x;
  }
};

class Image {
  // RGBA Pixel Type
  static constexpr unsigned int RGBASize = sizeof(glm::u8vec4);

public:
  Image(const glm::uvec2 size);
  Image(unsigned int width = 0, unsigned int height = 0);
  Image(const char *filename);
  ~Image() {}

  // this will clear pixel data and overwrite the size variable
  void load(const char *);
  void save(const char *) const;
  void resize(int width, int height);

  glm::uvec2 size() const { return size_; }
  unsigned int width() const { return size_.x; }
  unsigned int height() const { return size_.y; }
  unsigned int numPixels() const { return pixels_.size(); }
  unsigned int pixelByte() const { return RGBASize; }
  unsigned int totalBytes() const { return numPixels() * RGBASize; }

  // for writing
  inline glm::u8vec4 &operator()(int i, int j) {
    return pixels_[i + j * size_.x];
  }
  // for reading
  inline glm::u8vec4 operator()(int i, int j) const {
    return pixels_[i + j * size_.x];
  }

  // for writing (reversed y)
  inline glm::u8vec4 &operator()(const glm::uvec2 index) {
    return pixels_[index.x + (size_.y - index.y - 1) * size_.x];
  }
  // for reading (reversed y)
  inline glm::u8vec4 operator()(const glm::uvec2 index) const {
    return pixels_[index.x + (size_.y - index.y - 1) * size_.x];
  }

  std::vector<ne::TileIterator> toTiles(glm::uvec2 tileSize) const;

  // Inject small image
  bool inject(const ne::Image &other);

private:
  glm::uvec2 offset_;
  glm::uvec2 size_; // size.x = width, size.y = height
  std::vector<glm::u8vec4> pixels_;
};

} // namespace ne

#endif // __IMAGE_H_
