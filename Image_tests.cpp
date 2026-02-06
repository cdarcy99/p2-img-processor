#include "Matrix.hpp"
#include "Image_test_helpers.hpp"
#include "unit_test_framework.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>

using namespace std;

// Here's a free test for you! Model yours after this one.
// Test functions have no interface and thus no RMEs, but
// add a comment like the one here to say what it is testing.
// -----
// Sets various pixels in a 2x2 Image and checks
// that Image_print produces the correct output.
TEST(test_print_basic) {
  Image img;
  const Pixel red = {255, 0, 0};
  const Pixel green = {0, 255, 0};
  const Pixel blue = {0, 0, 255};
  const Pixel white = {255, 255, 255};

  Image_init(&img, 2, 2);
  Image_set_pixel(&img, 0, 0, red);
  Image_set_pixel(&img, 0, 1, green);
  Image_set_pixel(&img, 1, 0, blue);
  Image_set_pixel(&img, 1, 1, white);

  // Capture our output
  ostringstream s;
  Image_print(&img, s);

  // Correct output
  ostringstream correct;
  correct << "P3\n2 2\n255\n";
  correct << "255 0 0 0 255 0 \n";
  correct << "0 0 255 255 255 255 \n";
  ASSERT_EQUAL(s.str(), correct.str());
}

// Test all the inputs from a stream are
// read in correctly
TEST(test_Image_stream_basic) {
  std::istringstream input(
    "P3\n"
    "2 2\n"
    "255\n"
    "10 20 30 40 50 60\n"
    "70 80 90 100 110 120\n"
  );

  Image img;
  Image_init(&img, input);

  ASSERT_EQUAL(Image_width(&img), 2);
  ASSERT_EQUAL(Image_height(&img), 2);

  // Row 0
  ASSERT_EQUAL(Image_get_pixel(&img, 0, 0).r, 10);
  ASSERT_EQUAL(Image_get_pixel(&img, 0, 0).g, 20);
  ASSERT_EQUAL(Image_get_pixel(&img, 0, 0).b, 30);

  ASSERT_EQUAL(Image_get_pixel(&img, 0, 1).r, 40);
  ASSERT_EQUAL(Image_get_pixel(&img, 0, 1).g, 50);
  ASSERT_EQUAL(Image_get_pixel(&img, 0, 1).b, 60);

  // Row 1
  ASSERT_EQUAL(Image_get_pixel(&img, 1, 0).r, 70);
  ASSERT_EQUAL(Image_get_pixel(&img, 1, 0).g, 80);
  ASSERT_EQUAL(Image_get_pixel(&img, 1, 0).b, 90);

  ASSERT_EQUAL(Image_get_pixel(&img, 1, 1).r, 100);
  ASSERT_EQUAL(Image_get_pixel(&img, 1, 1).g, 110);
  ASSERT_EQUAL(Image_get_pixel(&img, 1, 1).b, 120);
}

// Check a 1x1 works
TEST(test_init_min_size) {
  Image img;
  Image_init(&img, 1, 1);

  Pixel p = Image_get_pixel(&img, 0, 0);
  ASSERT_EQUAL(p.r, 0);
  ASSERT_EQUAL(p.g, 0);
  ASSERT_EQUAL(p.b, 0);

  ASSERT_EQUAL(Image_width(&img), 1);
  ASSERT_EQUAL(Image_height(&img), 1);
}

// Check a 1x1 gets properly filled
TEST(test_fill_min_size) {
  Image img;
  Image_init(&img, 1, 1);

  Pixel red = {255, 0, 0};
  Image_fill(&img, red);

  Pixel p = Image_get_pixel(&img, 0, 0);
  ASSERT_EQUAL(p.r, 255);
  ASSERT_EQUAL(p.g, 0);
  ASSERT_EQUAL(p.b, 0);
}

// Test a rectangle gets width and height correct
TEST(test_rectangle) {
  Image img;
  Image_init(&img, 2, 5);

  ASSERT_EQUAL(Image_width(&img), 2);
  ASSERT_EQUAL(Image_height(&img), 5);
}

// Test the corners are all reached in fill
TEST(test_fill_corners) {
  Image img;
  Image_init(&img, 3, 4);

  Pixel blue = {0,0,255};
  Image_fill(&img, blue);

  Pixel p = Image_get_pixel(&img, 3, 2); // last valid index's
  ASSERT_EQUAL(p.b, 255);
}

// Test changing 1 pixel seperatly
TEST(test_set_pixel_different) {
  Image img;
  Image_init(&img, 2, 2);

  Pixel different = {7, 128, 231};
  Image_set_pixel(&img, 1, 1, different);

  Pixel p = Image_get_pixel(&img, 1, 1);
  ASSERT_EQUAL(p.r, 7);
  ASSERT_EQUAL(p.g, 128);
  ASSERT_EQUAL(p.b, 231);
}

TEST_MAIN() // Do NOT put a semicolon here
