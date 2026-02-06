#include <cassert>
#include <vector>
#include "processing.hpp"

using namespace std;

// v DO NOT CHANGE v ------------------------------------------------
// The implementation of rotate_left is provided for you.
// REQUIRES: img points to a valid Image
// MODIFIES: *img
// EFFECTS:  The image is rotated 90 degrees to the left (counterclockwise).
void rotate_left(Image* img) {

  // for convenience
  int width = Image_width(img);
  int height = Image_height(img);

  // auxiliary image to temporarily store rotated image
  Image aux;
  Image_init(&aux, height, width); // width and height switched

  // iterate through pixels and place each where it goes in temp
  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      Image_set_pixel(&aux, width - 1 - c, r, Image_get_pixel(img, r, c));
    }
  }

  // Copy data back into original
  *img = aux;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------

// v DO NOT CHANGE v ------------------------------------------------
// The implementation of rotate_right is provided for you.
// REQUIRES: img points to a valid Image.
// MODIFIES: *img
// EFFECTS:  The image is rotated 90 degrees to the right (clockwise).
void rotate_right(Image* img){

  // for convenience
  int width = Image_width(img);
  int height = Image_height(img);

  // auxiliary image to temporarily store rotated image
  Image aux;
  Image_init(&aux, height, width); // width and height switched

  // iterate through pixels and place each where it goes in temp
  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      Image_set_pixel(&aux, c, height - 1 - r, Image_get_pixel(img, r, c));
    }
  }

  // Copy data back into original
  *img = aux;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------


// v DO NOT CHANGE v ------------------------------------------------
// The implementation of diff2 is provided for you.
static int squared_difference(Pixel p1, Pixel p2) {
  int dr = p2.r - p1.r;
  int dg = p2.g - p1.g;
  int db = p2.b - p1.b;
  // Divide by 100 is to avoid possible overflows
  // later on in the algorithm.
  return (dr*dr + dg*dg + db*db) / 100;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------


// ------------------------------------------------------------------
// You may change code below this line!



// REQUIRES: img points to a valid Image.
//           energy points to a Matrix.
// MODIFIES: *energy
// EFFECTS:  energy serves as an "output parameter".
//           The Matrix pointed to by energy is initialized to be the same
//           size as the given Image, and then the energy matrix for that
//           image is computed and written into it.
//           See the project spec for details on computing the energy matrix.
void compute_energy_matrix(const Image* img, Matrix* energy) {
  int height = Image_height(img);
  int width = Image_width(img);
  Matrix_init(energy, width, height);

  int maxEnergy = 0; 
  // For every value set the four coordinates
  // Then use the equation and set that energy level in the
  // new matrix (no borders)
  for(int row = 0; row < height; row++){
    for(int col = 0; col < width; col++){
        if (row > 0 && row < height - 1 && col > 0 && col < width - 1){
          Pixel N = Image_get_pixel(img, row-1, col);
          Pixel W = Image_get_pixel(img, row, col-1);
          Pixel E = Image_get_pixel(img, row, col+1);
          Pixel S = Image_get_pixel(img, row+1, col);
          int energyX = squared_difference(N, S) + squared_difference(W, E);
          *Matrix_at(energy, row, col) = energyX;
          if(energyX > maxEnergy) maxEnergy = energyX;
        } 
      }
  } 
  // Now do the borders
  for(int row = 0; row < height; row++){
    for(int col = 0; col < width; col++){
        if(col == 0 || col == width - 1 || row == 0 || row == height - 1){
          *Matrix_at(energy, row, col) = maxEnergy;
        } 
      }
    }
}


// REQUIRES: energy points to a valid Matrix.
//           cost points to a Matrix.
//           energy and cost aren't pointing to the same Matrix
// MODIFIES: *cost
// EFFECTS:  cost serves as an "output parameter".
//           The Matrix pointed to by cost is initialized to be the same
//           size as the given energy Matrix, and then the cost matrix is
//           computed and written into it.
//           See the project spec for details on computing the cost matrix.
void compute_vertical_cost_matrix(const Matrix* energy, Matrix *cost) {
  int height = Matrix_height(energy);
  int width = Matrix_width(energy);
  Matrix_init(cost, width, height);

  // Set the cost of the top
  for(int col = 0; col < width; col++){
    *Matrix_at(cost, 0, col) = *Matrix_at(energy, 0, col);
  }
  // go down each value to set the cost based
  // on the energy above it by adding
  for(int row = 1; row < height; row++){
    for(int col = 0; col < width; col++){  
      int start = max(0, col-1);
      int end = min(width, col+2);
      *Matrix_at(cost, row, col) = *Matrix_at(energy, row, col) 
        + Matrix_min_value_in_row(cost, row-1, start, end);
    }
  }
}


// REQUIRES: cost points to a valid Matrix
// EFFECTS:  Returns the vertical seam with the minimal cost according to the given
//           cost matrix, represented as a vector filled with the col numbers for
//           each pixel along the seam, with index 0 representing the lowest numbered
//           row (top of image). The length of the returned vector is equal to
//           Matrix_height(cost).
//           While determining the seam, if any pixels tie for lowest cost, the
//           leftmost one (i.e. with the lowest col number) is used.
//           See the project spec for details on computing the minimal seam.
//           Note: When implementing the algorithm, compute the seam starting at the
//           bottom row and work your way up.
vector<int> find_minimal_vertical_seam(const Matrix* cost) {
  int height = Matrix_height(cost);
  int width  = Matrix_width(cost);
  vector<int> seam(height);
  // set the last value in seam (end) using
  // the min value in that row
  seam[height-1] = Matrix_column_of_min_value_in_row(cost, height-1, 0, width);

  // start at the bottom and move up, finding the 
  // least costly path
  for (int row = height - 2; row >= 0; row--) {
    int prevCol = seam[row + 1];
    int start = max(0, prevCol - 1);
    int end = min(width, prevCol + 2); 
    seam[row] = Matrix_column_of_min_value_in_row(cost, row, start, end);
  }
  return seam;
}


// REQUIRES: img points to a valid Image with width >= 2
//           seam.size() == Image_height(img)
//           each element x in seam satisfies 0 <= x < Image_width(img)
// MODIFIES: *img
// EFFECTS:  Removes the given vertical seam from the Image. That is, one
//           pixel will be removed from every row in the image. The pixel
//           removed from row r will be the one with col equal to seam[r].
//           The width of the image will be one less than before.
//           See the project spec for details on removing a vertical seam.
// NOTE:     Declare a new variable to hold the smaller Image, and
//           then do an assignment at the end to copy it back into the
//           original image.
void remove_vertical_seam(Image *img, const vector<int> &seam) {
  int width = Image_width(img);
  int height = Image_height(img);
  assert(width >= 2 && seam.size() == height);
  Image removedImage;
  Image_init(&removedImage, width - 1, height);

  // for each element add a non-removed column to the new
  // picture, adjusting indexing
  for (int row = 0; row < height; row++) {
    int newCol = 0;   // column in smaller image
    for (int col = 0; col < width; col++) {
      if (col != seam[row]) {
        Image_set_pixel(&removedImage, row, newCol, Image_get_pixel(img, row, col));
        newCol++;  
      }
    }
  }
  // set the image to the new width image
  *img = removedImage;
}


// REQUIRES: img points to a valid Image
//           0 < newWidth && newWidth <= Image_width(img)
// MODIFIES: *img
// EFFECTS:  Reduces the width of the given Image to be newWidth by using
//           the seam carving algorithm. See the spec for details.
// NOTE:     Use a vector to hold the seam, and make sure that it has
//           the right size. You can use .data() on a vector to get
//           the underlying array.
void seam_carve_width(Image *img, int newWidth) {
  assert(0 < newWidth && newWidth <= Image_width(img)); 
  Matrix energy;
  Matrix cost;

// Run the remove seam until the new width is reached
  while(Image_width(img) != newWidth){
    compute_energy_matrix(img, &energy);
    compute_vertical_cost_matrix(&energy, &cost); 
    const std::vector<int> seam = find_minimal_vertical_seam(&cost);
    remove_vertical_seam(img, seam);
  }

}

// REQUIRES: img points to a valid Image
//           0 < newHeight && newHeight <= Image_height(img)
// MODIFIES: *img
// EFFECTS:  Reduces the height of the given Image to be newHeight.
// NOTE:     This is equivalent to first rotating the Image 90 degrees left,
//           then applying seam_carve_width(img, newHeight), then rotating
//           90 degrees right.
void seam_carve_height(Image *img, int newHeight) {
  assert(0 < newHeight && newHeight <= Image_height(img)); 
  // Rotate to remove height and then correct
  rotate_left(img);
  seam_carve_width(img, newHeight);
  rotate_right(img);
}

// REQUIRES: img points to a valid Image
//           0 < newWidth && newWidth <= Image_width(img)
//           0 < newHeight && newHeight <= Image_height(img)
// MODIFIES: *img
// EFFECTS:  Reduces the width and height of the given Image to be newWidth
//           and newHeight, respectively.
// NOTE:     This is equivalent to applying seam_carve_width(img, newWidth)
//           and then applying seam_carve_height(img, newHeight).
void seam_carve(Image *img, int newWidth, int newHeight) {
  assert(0 < newWidth && newWidth <= Image_width(img));
  assert(0 < newHeight && newHeight <= Image_height(img));
  seam_carve_width(img, newWidth);
  seam_carve_height(img, newHeight);
}
