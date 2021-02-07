#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "functions.h"

using namespace std;

Pixel** createImage(int width, int height) {
  cout << "Start createImage... " << endl;
  
  // Create a one dimensional array on the heap of pointers to Pixels 
  //    that has width elements (i.e. the number of columns)
  Pixel** image = new Pixel*[width];
  
  bool fail = false;
  
  for (int i=0; i < width; ++i) { // loop through each column
    // assign that column to a one dimensional array on the heap of Pixels
    //  that has height elements (i.e. the number of rows)
    image[i] = new Pixel[height];
    
    if (image[i] == nullptr) { // failed to allocate
      fail = true;
    }
  }
  
  if (fail) { // if any allocation fails, clean up and avoid memory leak
    // deallocate any arrays created in for loop
    for (int i=0; i < width; ++i) {
      delete [] image[i]; // deleting nullptr is not a problem
    }
    delete [] image; // dlete array of pointers
    return nullptr;
  }
  
  // initialize cells
  //cout << "Initializing cells..." << endl;
  for (int row=0; row<height; ++row) {
    for (int col=0; col<width; ++col) {
      //cout << "(" << col << ", " << row << ")" << endl;
      image[col][row] = { 0, 0, 0 };
    }
  }
  cout << "End createImage... " << endl;
  return image;
}

void deleteImage(Pixel** image, int width) {
  cout << "Start deleteImage..." << endl;
  // avoid memory leak by deleting the array
  for (int i=0; i<width; ++i) {
    delete [] image[i]; // delete each individual array placed on the heap
  }
  delete [] image;
  image = nullptr;
}

// implement for part 1

int* createSeam(int length) {
  int *seam;
  seam = new int [length];
  return seam;
}

void deleteSeam(int* seam) {
	cout << "Start deleteSeam..." << endl;
	delete [] seam;
}

bool loadImage(string filename, Pixel** image, int width, int height) {
  // declare/define and open input file stream
  ifstream ifs (filename);

  // check if input stream opened successfully
  if (!ifs.is_open()) {
    cout << "Error: failed to open input file - " << filename << endl;
    return false;
  }
  
  // get type from preamble
  char type[3];
  ifs >> type; // should be P3
  if (!isdigit(type[1])) {
	  cout << "Error: read non-integer value" << endl;
	  return false;
  }
  if ((toupper(type[0]) != 'P') || (type[1] != '3')) { // check that type is correct
    cout << "Error: type is " << type << " instead of P3" << endl; 
    return false;
  }
  
  // get width (w) and height (h) from preamble
  int w = 0, h = 0;
  ifs >> w >> h;
  if (ifs.fail()) {
	cout << "Error: read non-integer value" << endl;
	return false;
  }
  if (w != width) { // check that width matches what was passed into the function
    cout << "Error: input width (" << width << ") does not match value in file (" << w << ")" << endl;
    return false;
  }
  if (ifs.fail()) {
	cout << "Error: read non-integer value" << endl;
	return false;
  }
  if (h != height) { // check that height matches what was passed into the function
    cout << "Error: input height (" << height << ") does not match value in file (" << h << ")" << endl;
    return false;
  }
  
  int max_color_value = 0;

  ifs >> max_color_value;
  if (ifs.fail()) {
	cout << "Error: read non-integer value" << endl;
	return false;
  }
  if (max_color_value < 0) {
	cout << "Error: color value is less than 0" << endl;
	return false;
  }  
  if (max_color_value > 255) {
	cout << "Error: invalid color value " << max_color_value << endl;
	return false;
  }  
  

  // get RGB pixel values
  int red = 0;
  int green = 0;
  int blue = 0;
  for (int row = 0; row < height; ++row) {
	//cout << endl;
	for (int col = 0; col < width; ++col) {
		ifs >> ws >> red;
		if (ifs.fail()) {
			if (ifs.eof()) {
				cout << "Error: not enough color values" << endl;
			}
			else {
				cout << "Error: read non-integer value" << endl;
			}
			return false;
		}
		ifs >> ws >> green;
		if (ifs.fail()) {
			if (ifs.eof()) {
				cout << "Error: not enough color values" << endl;
			}
			else {
				cout << "Error: read non-integer value" << endl;
			}
			return false;
		}
		ifs >> ws >> blue;
		if (ifs.fail()) {
			if (ifs.eof()) {
				cout << "Error: not enough color values" << endl;
			}
			else {
				cout << "Error: read non-integer value" << endl;
			}
			return false;
		}
		
		if (red < 0 || red > 255) {
			cout << "Error: invalid color value " << red << endl;
			return false;
		}
		else if (green < 0 || green > 255) {
			cout << "Error: invalid color value " << green << endl;
			return false;
		}
		else if (blue < 0 || blue > 255) {
			cout << "Error: invalid color value " << blue << endl;
			return false;
		}
		image[col][row].r = red;
		image[col][row].g = green;
		image[col][row].b = blue;
		//cout << red << " " << green << " " << blue << " ";
	}
  }
  int line = -9000;
  while (ifs >> line) {
	if (line != -9000) {
		cout << "Error: too many color values" << endl;
		return false;
	}
  }
  return true;
}

bool outputImage(string filename, Pixel** image, int width, int height) {
  ofstream ofs (filename);
  
    // check if output stream opened successfully
  if (!ofs.is_open()) {
	cout << "Error: failed to open output file - " << filename << endl;
	return false;
  }
  
	// output preamble
  ofs << "P3" << endl << width << " " << height << endl << 255 << endl;

  // output pixels
  int red = 0;
  int green = 0;
  int blue = 0;
  for (int row = 0; row < height; ++row) {
	  //cout << endl;
	for (int col = 0; col < width; ++col) {
		red = image[col][row].r;
		green = image[col][row].g;
		blue = image[col][row].b;
		ofs << red << " " << green << " " << blue << endl;
		//cout << red << " " << green << " " << blue << endl;
	}
  }
  return true;
}

int energy(Pixel** image, int x, int y, int width, int height) { 
  int x_energy = 0;
  int y_energy = 0;
  int r_energy = 0;
  int g_energy = 0;
  int b_energy = 0;
  if (x == 0 || x == width - 1) {
	if (x == 0) {
		r_energy = pow(((image[width-1][y].r) - (image[1][y].r)), 2);
		g_energy = pow(((image[width-1][y].g) - (image[1][y].g)), 2);
		b_energy = pow(((image[width-1][y].b) - (image[1][y].b)), 2);
	}
	else {
		r_energy = pow(((image[x-1][y].r) - (image[0][y].r)), 2);
		g_energy = pow(((image[x-1][y].g) - (image[0][y].g)), 2);
		b_energy = pow(((image[x-1][y].b) - (image[0][y].b)), 2);
	}
  }	
  else {
	r_energy = pow(((image[x-1][y].r) - (image[x+1][y].r)), 2);
	g_energy = pow(((image[x-1][y].g) - (image[x+1][y].g)), 2);
	b_energy = pow(((image[x-1][y].b) - (image[x+1][y].b)), 2);
  }
  x_energy = r_energy + g_energy + b_energy;
  
  if (y == 0 || y == height - 1) {
	if (y == 0) {
		r_energy = pow(((image[x][height-1].r) - (image[x][1].r)), 2);
		g_energy = pow(((image[x][height-1].g) - (image[x][1].g)), 2);
		b_energy = pow(((image[x][height-1].b) - (image[x][1].b)), 2);
	}
	else {
		r_energy = pow(((image[x][y-1].r) - (image[x][0].r)), 2);
		g_energy = pow(((image[x][y-1].g) - (image[x][0].g)), 2);
		b_energy = pow(((image[x][y-1].b) - (image[x][0].b)), 2);
	}
  }	
  else {
	r_energy = pow(((image[x][y-1].r) - (image[x][y+1].r)), 2);
	g_energy = pow(((image[x][y-1].g) - (image[x][y+1].g)), 2);
	b_energy = pow(((image[x][y-1].b) - (image[x][y+1].b)), 2);
  }
  y_energy = r_energy + g_energy + b_energy;
  int energy = x_energy + y_energy;
  return energy;
}

// implement for part 2 

int loadVerticalSeam(Pixel** image, int start_col, int width, int height, int* seam) {
  int total_energy = energy(image, start_col, 0, width, height);
  seam[0] = start_col;
  int col = start_col;
  
  int left_col = 0;
  int forward_col = 0;
  int right_col = 0;
  
  for (int row = 1; row < height; ++row) {
	  if (col == 0) { 												// if column is far right
		forward_col = energy(image, 0, row, width, height);
		left_col = energy(image, 1, row, width, height);
		
		if (forward_col <= left_col) {
			total_energy += forward_col;
			seam[row] = 0;
		}
		else {
			total_energy += left_col;
			col += 1;
			seam[row] = 1;
			
		}
	  }
	  
	  else if (col == width-1) { 										// if column is far left
		right_col = energy(image, col-1, row, width, height);
		forward_col = energy(image, col, row, width, height);
		
		if (right_col < forward_col) {
			total_energy += right_col;
			col -= 1;
			seam[row] = col;
		}
		else {
			total_energy += forward_col;
			seam[row] = col;
			
		}
		
	  }
	  
	  else {														// if column is in the middle
		right_col = energy(image, col-1, row, width, height);
		forward_col = energy(image, col, row, width, height);
		left_col = energy(image, col+1, row, width, height);
		
		if ((right_col < left_col) && (right_col < forward_col)) {
			total_energy += right_col;
			col -= 1;
			seam[row] = col;
		}
		else if (left_col < forward_col) {
			total_energy += left_col;
			col += 1;
			seam[row] = col;
		}
		
		else {
			total_energy += forward_col;
			seam[row] = col;
		}
	  }
  }
  return total_energy;
}

int* findMinVerticalSeam(Pixel** image, int width, int height) {
  int* min_seam;
  min_seam = new int [height];
  int min_col = 0;
  
  int min_energy = loadVerticalSeam(image, 0, width, height, min_seam);
  int energy = min_energy;
  
  for (int col = 1; col < width; ++col) {
	energy = loadVerticalSeam(image, col, width, height, min_seam);
	if (energy < min_energy) {
		min_energy = energy;
		min_col = col;
	}
  }
  min_energy = loadVerticalSeam(image, min_col, width, height, min_seam);
  return min_seam;
}

void removeVerticalSeam(Pixel** image, int width, int height, int* verticalSeam) {
	for (int row = 0; row < height; ++row) {
		for (int col = verticalSeam[row]; col < width-1; ++col) {
			image[col][row].r = image[col+1][row].r;
			image[col][row].g = image[col+1][row].g;
			image[col][row].b = image[col+1][row].b;
		}
	}
	delete [] verticalSeam;
}

// implement for extra credit

int loadHorizontalSeam(Pixel** image, int start_row, int width, int height, int* seam) {
  int total_energy = energy(image, 0, start_row, width, height);
  seam[0] = start_row;
  int row = start_row;
  
  int top_row = 0;
  int forward_row = 0;
  int bottom_row = 0;
  
  for (int col = 1; col < width; ++col) {
	  if (row == 0) { 													// if row is far top
		forward_row = energy(image, col, 0, width, height);
		bottom_row = energy(image, col, 1, width, height);
		
		if (forward_row <= bottom_row) {
			total_energy += forward_row;
			seam[col] = 0;
		}
		else {
			total_energy += bottom_row;
			row += 1;
			seam[col] = 1;
			
		}
	  }
	  
	  else if (row == height-1) { 										// if row is far bottom
		top_row = energy(image, col, row-1, width, height);
		forward_row = energy(image, col, row, width, height);
		
		if (top_row < forward_row) {
			total_energy += top_row;
			row -= 1;
			seam[col] = row;
		}
		else {
			total_energy += forward_row;
			seam[col] = row;
			
		}
		
	  }
	  
	  else {															// if row is middle
		top_row = energy(image, col, row-1, width, height);
		forward_row = energy(image, col, row, width, height);
		bottom_row = energy(image, col, row+1, width, height);
		
		if ((bottom_row < top_row) && (bottom_row < forward_row)) {
			total_energy += bottom_row;
			row += 1;
			seam[col] = row;
		}
		else if (top_row < forward_row) {
			total_energy += top_row;
			row -= 1;
			seam[col] = row;
		}
		
		else {
			total_energy += forward_row;
			seam[col] = row;
		}
	  }
  }
  return total_energy;
}

int* findMinHorizontalSeam(Pixel** image, int width, int height) {
  int* min_seam;
  min_seam = new int [width];
  int min_row = 0;
  
  int min_energy = loadHorizontalSeam(image, 0, width, height, min_seam);
  int energy = min_energy;
  
  for (int row = 1; row < height; ++row) {
	energy = loadHorizontalSeam(image, row, width, height, min_seam);
	if (energy < min_energy) {
		min_energy = energy;
		min_row = row;
	}
  }
  min_energy = loadHorizontalSeam(image, min_row, width, height, min_seam);
  return min_seam;
}

void removeHorizontalSeam(Pixel** image, int width, int height, int* horizontalSeam) {
	for (int col = 0; col < width; ++col) {
		for (int row = horizontalSeam[col]; row < height-1; ++row) {
			image[col][row].r = image[col][row+1].r;
			image[col][row].g = image[col][row+1].g;
			image[col][row].b = image[col][row+1].b;
		}
	}
	delete [] horizontalSeam;
}
