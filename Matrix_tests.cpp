#include "Matrix.hpp"
#include "Matrix_test_helpers.hpp"
#include "unit_test_framework.hpp"

using namespace std;

// Here's a free test for you! Model yours after this one.
// Test functions have no interface and thus no RMEs, but
// add a comment like the one here to say what it is testing.
// -----
// Fills a 3x5 Matrix with a value and checks
// that Matrix_at returns that value for each element.
TEST(test_fill_basic) {
  Matrix mat;
  const int width = 3;
  const int height = 5;
  const int value = 42;
  Matrix_init(&mat, 3, 5);
  Matrix_fill(&mat, value);

  for(int r = 0; r < height; ++r){
    for(int c = 0; c < width; ++c){
      ASSERT_EQUAL(*Matrix_at(&mat, r, c), value);
    }
  }
  ASSERT_EQUAL(Matrix_max(&mat),42)
}

// Test a 1x1 matrix intilizes correctly and fills
TEST(test_one_by_one){
  Matrix mat;
  Matrix_init(&mat, 1, 1);
  Matrix_fill_border(&mat, 7);

  ASSERT_EQUAL(Matrix_width(&mat), 1);
  ASSERT_EQUAL(Matrix_height(&mat), 1);
  ASSERT_EQUAL(*Matrix_at(&mat,0,0), 7);
}

// Test a variety of values for the max
TEST(test_not_same){
  Matrix mat;
  Matrix_init(&mat, 3, 3);

  *Matrix_at(&mat,0,0) = 1;
  *Matrix_at(&mat,0,1) = 2;
  *Matrix_at(&mat,0,2) = 5;
  *Matrix_at(&mat,1,0) = 1;
  *Matrix_at(&mat,2,0) = 10;

  ASSERT_EQUAL(Matrix_max(&mat), 10);
}

// Test a rectange init's correctly and fills just border
// and the minimum value row
TEST(test_border_rectangular){
  Matrix mat;
  Matrix_init(&mat, 5, 3); // width != height
  Matrix_fill(&mat, 0);
  Matrix_fill_border(&mat, 9);

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 5; j++){
      if(i == 0 || i == 2 || j == 0 || j == 4){
        ASSERT_EQUAL(*Matrix_at(&mat,i,j), 9);
      }
      else{
        ASSERT_EQUAL(*Matrix_at(&mat,i,j), 0);
      }
    }
  }
  *Matrix_at(&mat,2,1) = 10;
  ASSERT_EQUAL(Matrix_column_of_min_value_in_row(&mat, 2, 1, 2),1);
  // check width and height are properly assigned
  ASSERT_EQUAL(Matrix_width(&mat), 5);  
  ASSERT_EQUAL(Matrix_height(&mat), 3);

}
// Test minimum chooses leftmost column when tied
TEST(test_column_min_left){
  Matrix mat;
  Matrix_init(&mat, 4, 1);

  *Matrix_at(&mat,0,0) = 5;
  *Matrix_at(&mat,0,1) = 2;
  *Matrix_at(&mat,0,2) = 2;
  *Matrix_at(&mat,0,3) = 9;

  ASSERT_EQUAL(Matrix_column_of_min_value_in_row(&mat,0,0,4),1);
}

// tests the prints for os matches
TEST(test_print_multi){
  Matrix mat;
  Matrix_init(&mat, 2, 2);

  *Matrix_at(&mat,0,0) = 1;
  *Matrix_at(&mat,0,1) = 2;
  *Matrix_at(&mat,1,0) = 3;
  *Matrix_at(&mat,1,1) = 4;

  ostringstream expected;
  expected << "2 2\n"
           << "1 2 \n"
           << "3 4 \n";

  ostringstream actual;
  Matrix_print(&mat, actual);

  ASSERT_EQUAL(expected.str(), actual.str());
}

// Test the max number and for all negatives,
// all 0's, and a mix negatives and positive
TEST(test_max){
  Matrix mat;
  Matrix_init(&mat, 5, 1);

  *Matrix_at(&mat,0,0) = 5;
  *Matrix_at(&mat,0,1) = 2;
  *Matrix_at(&mat,0,2) = 2;
  *Matrix_at(&mat,0,3) = 8;
  *Matrix_at(&mat,0,4) = 9;

  ASSERT_EQUAL(Matrix_max(&mat),9);

  Matrix mat1;
  Matrix_init(&mat1, 5, 1);

  *Matrix_at(&mat1,0,0) = -5;
  *Matrix_at(&mat1,0,1) = -2;
  *Matrix_at(&mat1,0,2) = -2;
  *Matrix_at(&mat1,0,3) = -8;
  *Matrix_at(&mat1,0,4) = -9;

  ASSERT_EQUAL(Matrix_max(&mat1),-2);

  Matrix mat2;
  Matrix_init(&mat2, 5, 1);

  *Matrix_at(&mat2,0,0) = 0;
  *Matrix_at(&mat2,0,1) = 0;
  *Matrix_at(&mat2,0,2) = 0;
  *Matrix_at(&mat2,0,3) = 0;
  *Matrix_at(&mat2,0,4) = 0;

  ASSERT_EQUAL(Matrix_max(&mat2),0);

  Matrix mat3;
  Matrix_init(&mat3, 5, 1);

  *Matrix_at(&mat3,0,0) = -2;
  *Matrix_at(&mat3,0,1) = 4;
  *Matrix_at(&mat3,0,2) = -6;
  *Matrix_at(&mat3,0,3) = 8;
  *Matrix_at(&mat3,0,4) = -1;

  ASSERT_EQUAL(Matrix_max(&mat3),8);

}

TEST_MAIN() 
