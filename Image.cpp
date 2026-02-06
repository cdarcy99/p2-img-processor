#include <cassert>
#include "Image.hpp"

// REQUIRES: img points to an Image
//           0 < width && 0 < height
// MODIFIES: *img
// EFFECTS:  Initializes the Image with the given width and height, with
//           all pixels initialized to RGB values of 0.
void Image_init(Image* img, int width, int height) {
  assert(0 < width && 0 < height);
  img->width = width;
  img->height = height;
  // Access each channel in the image and set the width + height
  Matrix_init(&img->red_channel, width, height);
  Matrix_init(&img->green_channel, width, height);
  Matrix_init(&img->blue_channel, width, height);
}

// REQUIRES: img points to an Image
//           is contains an image in PPM format without comments
//           (any kind of whitespace is ok)
// MODIFIES: *img, is
// EFFECTS:  Initializes the Image by reading in an image in PPM format
//           from the given input stream.
// NOTE:     See the project spec for a discussion of PPM format.
void Image_init(Image* img, std::istream& is) {
  // Set the header, width, height, and intentsity first
  std::string header;
  is >> header;
  int width, height;
  is >> width >> height;
  int maxIntensity;
  is >> maxIntensity;

  Image_init(img, width, height);

  // For each element set each pixel value
  for (int row = 0; row < Image_height(img); ++row) {
    for (int col = 0; col < Image_width(img); ++col) {
      Pixel p;
      is >> p.r >> p.g >> p.b;
      Image_set_pixel(img, row, col, p);
    }
  }
}

// REQUIRES: img points to a valid Image
// MODIFIES: os
// EFFECTS:  Writes the image to the given output stream in PPM format.
//           You must use the kind of whitespace specified here.
//           First, prints out the header for the image like this:
//             P3 [newline]
//             WIDTH [space] HEIGHT [newline]
//             255 [newline]
//           Next, prints out the rows of the image, each followed by a
//           newline. Each pixel in a row is printed as three ints
//           for its red, green, and blue components, in that order. Each
//           int is followed by a space. This means that there will be an
//           "extra" space at the end of each line. See the project spec
//           for an example.
void Image_print(const Image* img, std::ostream& os) {
  // Output the first 3 lines, not pixels
  os << "P3\n";
  os << Image_width(img) << " " << Image_height(img) << "\n";
  os << 255 << "\n";
  
  // For each row set the r,g,b values and send to output
  for (int row = 0; row < Image_height(img); ++row) {
    for (int col = 0; col < Image_width(img); ++col) {
      int r = *Matrix_at(&img->red_channel, row, col);
      int g = *Matrix_at(&img->green_channel, row, col);
      int b = *Matrix_at(&img->blue_channel, row, col);

      os << r << " " << g << " " << b << " ";
    }
    os << "\n";
  }
}

// REQUIRES: img points to a valid Image
// EFFECTS:  Returns the width of the Image.
int Image_width(const Image* img) {
  return img->width;
}

// REQUIRES: img points to a valid Image
// EFFECTS:  Returns the height of the Image.
int Image_height(const Image* img) {
  return img->height;
}

// REQUIRES: img points to a valid Image
//           0 <= row && row < Image_height(img)
//           0 <= column && column < Image_width(img)
// EFFECTS:  Returns the pixel in the Image at the given row and column.
Pixel Image_get_pixel(const Image* img, int row, int column) {
  assert(0 <= row && row < Image_height(img));
  assert(0 <= column && column < Image_width(img));
  // set the specific pixel rgb values to match given values
  Pixel pixel;
  pixel.r = *Matrix_at(&img->red_channel,row, column);
  pixel.g = *Matrix_at(&img->green_channel,row, column);
  pixel.b = *Matrix_at(&img->blue_channel,row, column);
  return pixel;
}

// REQUIRES: img points to a valid Image
//           0 <= row && row < Image_height(img)
//           0 <= column && column < Image_width(img)
// MODIFIES: *img
// EFFECTS:  Sets the pixel in the Image at the given row and column
//           to the given color.
void Image_set_pixel(Image* img, int row, int column, Pixel color) {
  assert(0 <= row && row < Image_height(img) && 0 <= column && column < Image_width(img));
  // Change the value of each channels specific pixel to match
  *Matrix_at(&img->red_channel, row, column) = color.r;
  *Matrix_at(&img->blue_channel, row, column) = color.b;
  *Matrix_at(&img->green_channel, row, column) = color.g;
}

// REQUIRES: img points to a valid Image
// MODIFIES: *img
// EFFECTS:  Sets each pixel in the image to the given color.
void Image_fill(Image* img, Pixel color) {
  // For each value use Image_set_pixel() to change pixel
  for (int row = 0; row < Image_height(img); row++) {
    for (int col = 0; col < Image_width(img); col++) {
      Image_set_pixel(img, row, col, color);
    }
  }
}
