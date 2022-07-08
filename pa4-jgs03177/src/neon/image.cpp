#include "image.hpp"
#include <cstring>
#include <iostream>
#include <lodepng/lodepng.h>

namespace ne {

void Image::resize(int width, int height) {
  size_ = glm::uvec2(width, height);
  pixels_.resize(width * height);
}

void Image::save(const char *filename) const {
  unsigned int byteSize = this->totalBytes();
  std::vector<unsigned char> data(byteSize);
  std::memcpy((void *)data.data(), pixels_.data(), byteSize);
  unsigned width = size_.x, height = size_.y;
  unsigned error = lodepng::encode(filename, data, width, height);
  if (error)
    std::cout << "PNG Encoding error " << error << ": "
              << lodepng_error_text(error) << std::endl;
}

void Image::load(const char *filename) {
  std::vector<unsigned char> data;
  unsigned width, height;

  // after this, data contains rgbargba....
  unsigned error = lodepng::decode(data, width, height, filename);
  if (error)
    std::cout << "PNG Decoding error " << error << ": "
              << lodepng_error_text(error) << std::endl;

  this->resize(width, height);
  std::memcpy((void *)pixels_.data(), data.data(), data.size());
}

// Inject small image
bool Image::inject(const ne::Image &other) {
  const glm::uvec2 offset = other.offset_;
  if (other.size_.x > size_.x || other.size_.y > size_.y)
    return false;

  for (int j = offset.y; j < offset.y + other.size_.y; ++j) {
    for (int i = offset.x; i < offset.x + other.size_.x; ++i) {
      if (i < size_.x && j < size_.y)
        pixels_[i + j * size_.x] = other(i - offset.x, j - offset.y);
    }
  }
  return true;
}

std::vector<ne::TileIterator> Image::toTiles(glm::uvec2 tileSize) const {
  glm::uvec2 numTiles = glm::uvec2((int)std::ceil(size_.x / float(tileSize.x)),
                                   (int)std::ceil(size_.y / float(tileSize.y)));

  std::vector<ne::TileIterator> tiles;

  // loop for tile index
  for (int j = 0; j < numTiles.y; ++j) {
    for (int i = 0; i < numTiles.x; ++i) {
      glm::uvec2 start_index = tileSize * glm::uvec2(i, j);
      glm::uvec2 end_index = glm::min(start_index + tileSize, size_);
      tiles.emplace_back(ne::TileIterator(start_index, end_index));
    }
  }

  return tiles;
};

Image::Image(glm::uvec2 size) : size_(size) {
  const unsigned int numPixels = size_.x * size_.y;
  pixels_.resize(numPixels);
}

Image::Image(unsigned int width, unsigned int height) : size_(width, height) {
  const unsigned int numPixels = size_.x * size_.y;
  pixels_.resize(numPixels);
}

} // namespace ne
