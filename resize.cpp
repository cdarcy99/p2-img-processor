#include <iostream>
#include <fstream>
#include "Image.hpp"
#include "processing.hpp"
using namespace std;

int main(int argc, char **argv) {
    // take in image one, output it to a file, then new width and height
    // argc is the number of elements, argv is the name of them
    string inputFileName = argv[1];
    string outputFileName = argv[2];
    ifstream fin(inputFileName);

    // Image variables
    Image img;
    Image_init(&img, fin);
    int newWidth = atoi(argv[3]);
    int newHeight;

    // Check if the input file is open
    if (!fin.is_open()) {
        cout << "Error opening file: " << inputFileName << endl;
        return 1;
    }
    
    // Set height based on how many inputs
    if(argc == 5){
        newHeight = atoi(argv[4]);
    } else {
        newHeight = Image_height(&img);
    }

    // Check the right number of arguments
    if(argc != 4 && argc != 5){
        cout << "Usage: resize.exe " << inputFileName << " " << outputFileName 
        << " " << newWidth << " " << newHeight << "\n" << endl;
        return 1;
    }

    // Check that the heights and widths are valid
    if(newHeight < 1 || newHeight > Image_height(&img)){
        cout << "Usage: resize.exe " << inputFileName << " " << outputFileName 
        << " " << newWidth << " " << newHeight << "\n" 
        << "WIDTH and HEIGHT must be less than or equal to original" << endl;
        return 1;
    }
    if(newWidth < 1 || newWidth > Image_width(&img)){
        cout << "Usage: resize.exe " << inputFileName << " " << outputFileName 
        << " " << newWidth << " " << newHeight << "\n" 
        << "WIDTH and HEIGHT must be less than or equal to original" << endl;
        return 1;
    }

    // Run the cutting
    if(argc == 5){
        seam_carve(&img, newWidth, newHeight);
    } else {
        seam_carve_width(&img, newWidth);
    }
    
    ofstream fout(outputFileName);
    if (!fout.is_open()) {
        cout << "Error opening file: " << outputFileName << endl;
        return 1;
    }

    Image_print(&img, fout);
    fout.close();

}