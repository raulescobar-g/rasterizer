#include "Image.h"
#include <cassert>
#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

Image::Image(int w, int h)
    : width(w), height(h), comp(3), pixels(width * height * comp, 0) {}

Image::~Image() {}

void Image::setPixel(int x, int y, unsigned char r, unsigned char g,
                     unsigned char b) {
  // First check for bounds
  if (y < 0 || y >= height) {
    cout << "Row " << y << " is out of bounds" << endl;
    return;
  }
  if (x < 0 || x >= width) {
    cout << "Col " << x << " is out of bounds" << endl;
    return;
  }

  y = height - y - 1;
  int index = y * width + x;
  assert(index >= 0);
  assert(3 * index + 2 < (int)pixels.size());
  pixels[3 * index + 0] = r;
  pixels[3 * index + 1] = g;
  pixels[3 * index + 2] = b;
}

void Image::writeToFile(const string &filename) {
  int stride_in_bytes = width * comp * sizeof(unsigned char);
  int rc = stbi_write_png(filename.c_str(), width, height, comp, &pixels[0],
                          stride_in_bytes);
  if (rc) {
    cout << "Wrote to " << filename << endl;
  } else {
    cout << "Couldn't write to " << filename << endl;
  }
}
