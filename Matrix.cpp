#include <cassert>
#include "Matrix.hpp"

// REQUIRES: mat points to a Matrix
//           0 < width && 0 < height
// MODIFIES: *mat
// EFFECTS:  Initializes *mat as a Matrix with the given width and height,
//           with all elements initialized to 0.
void Matrix_init(Matrix* mat, int width, int height) {
  assert(width>0 && height > 0);
  mat->data.assign(width*height, 0);
  mat->width = width;
  mat->height = height;
}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: os
// EFFECTS:  First, prints the width and height for the Matrix to os:
//             WIDTH [space] HEIGHT [newline]
//           Then prints the rows of the Matrix to os with one row per line.
//           Each element is followed by a space and each row is followed
//           by a newline. This means there will be an "extra" space at
//           the end of each line.
void Matrix_print(const Matrix* mat, std::ostream& os) {
  os << Matrix_width(mat) << " " << Matrix_height(mat) << "\n";
  for(int row = 0; row < Matrix_height(mat); row++){
    for(int column = 0; column < Matrix_width(mat); column++){
      os << *Matrix_at(mat, row, column) << " ";
    }
    os << "\n";
  }
}

// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the width of the Matrix.
int Matrix_width(const Matrix* mat) {
  return mat-> width;
}

// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the height of the Matrix.
int Matrix_height(const Matrix* mat) {
  return mat-> height;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column && column < Matrix_width(mat)
//
// MODIFIES: (The returned pointer may be used to modify an
//            element in the Matrix.)
// EFFECTS:  Returns a pointer to the element in the Matrix
//           at the given row and column.
int* Matrix_at(Matrix* mat, int row, int column) {
  assert(0 <= row && row < Matrix_height(mat));
  assert(0 <= column && column < Matrix_width(mat));
  // Index into the 2d vector like a 3d one
  return &mat->data[row * Matrix_width(mat) + column];
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column && column < Matrix_width(mat)
//
// EFFECTS:  Returns a pointer-to-const to the element in
//           the Matrix at the given row and column.
const int* Matrix_at(const Matrix* mat, int row, int column) {
  assert(0 <= row && row < Matrix_height(mat));
  assert(0 <= column && column < Matrix_width(mat));
  return &mat->data[row * Matrix_width(mat) + column];
}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: *mat
// EFFECTS:  Sets each element of the Matrix to the given value.
void Matrix_fill(Matrix* mat, int value) {
  // loop through all values and set new value
  for(int row = 0; row < Matrix_height(mat); row++){
    for(int column = 0; column < Matrix_width(mat); column++){
      *Matrix_at(mat, row, column) = value;
    }
  }
}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: *mat
// EFFECTS:  Sets each element on the border of the Matrix to
//           the given value. These are all elements in the first/last
//           row or the first/last column.
void Matrix_fill_border(Matrix* mat, int value) {
  int height = Matrix_height(mat);
  int width = Matrix_width(mat);
  // for each vector not on the edges change the value
  for(int row = 0; row < height; row++){
    for(int column = 0; column < width; column++){
        if(column == 0 || column == width - 1 ||
          row == 0 || row == height - 1){
            *Matrix_at(mat, row, column) = value;
          }
        } 
      }
 } 


// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the value of the maximum element in the Matrix
int Matrix_max(const Matrix* mat) {
  int max = *Matrix_at(mat, 0, 0);
  // search each value if its bigger than the first
  for(int row = 0; row < Matrix_height(mat); row++){
    for(int column = 0; column < Matrix_width(mat); column++){
      if(*Matrix_at(mat, row, column) > max){
        max = *Matrix_at(mat, row, column);
      }
    }
  }
  return max;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column_start && column_end <= Matrix_width(mat)
//           column_start < column_end
// EFFECTS:  Returns the column of the element with the minimal value
//           in a particular region. The region is defined as elements
//           in the given row and between column_start (inclusive) and
//           column_end (exclusive).
//           If multiple elements are minimal, returns the column of
//           the leftmost one.
int Matrix_column_of_min_value_in_row(const Matrix* mat, int row, 
  int column_start, int column_end) {
  assert(0 <= row && row < Matrix_height(mat) && 0 <= column_start);
  assert(column_end <= Matrix_width(mat) && column_start < column_end);
  int min = *Matrix_at(mat, row, column_start);
  int minColumn = column_start;
  // for each value check its value isn't smaller than first
  // if it is replace value and column
  for(int column = column_start + 1; column < column_end; column++){ 
    if(*Matrix_at(mat, row, column) < min){
      min = *Matrix_at(mat, row, column);
      minColumn = column;
    }
  }
  return minColumn;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column_start && column_end <= Matrix_width(mat)
//           column_start < column_end
// EFFECTS:  Returns the minimal value in a particular region. The region
//           is defined as elements in the given row and between
//           column_start (inclusive) and column_end (exclusive).
int Matrix_min_value_in_row(const Matrix* mat, int row, 
  int column_start, int column_end) {
  assert(0 <= row && row < Matrix_height(mat));
  assert(0 <= column_start && column_end <= Matrix_width(mat));
  assert(column_start < column_end);

  // for each value from the start to end given using function
  // check if the min is smaller than the first
  int minColumn = Matrix_column_of_min_value_in_row(mat, row, 
    column_start, column_end);

  return *Matrix_at(mat, row, minColumn);
}
