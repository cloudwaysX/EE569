// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>


using namespace std;

int main(int argc, char *argv[])

{
	// Check for proper syntax
	if (argc < 6){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image(no raw!) height width option_channel" << endl;
		return 0;
	}


	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=3;
	int height = atoi(argv[3]);
	int width = atoi(argv[4]);

	
	// Allocate image data array
	unsigned char Imagedata[height][width][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	//Mirror the Image acrross vertical axis
	double out[height][width][1];
	int selectChannel=atoi(argv[5]);
	double normalizeParameter = 255;

	for(int r=0;r<height;r++){
		for(int c=0;c<width;c++){
			out[r][c][0]=(1-Imagedata[r][c][selectChannel]/normalizeParameter);
		}
	}

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(out, sizeof(double), height*width, file);
	fclose(file);

	return 0;
}
