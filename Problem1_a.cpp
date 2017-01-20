// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

unsigned char Mirror();

unsigned char Resize(int width, int height);

int BiInterpolation(unsigned char tl, unsigned char tr,unsigned char bl,unsigned char br );

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int Size = 256;
	
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 4){
		BytesPerPixel = 1; // default is grey image
	} 
	else {
		BytesPerPixel = atoi(argv[3]);
		// Check if size is specified
		if (argc >= 5){
			Size = atoi(argv[4]);
		}
	}
	
	// Allocate image data array
	unsigned char Imagedata[Size][Size][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

	return 0;
}

//Mirror the Image acrross vertical axis
unsigned char Mirror(){

	unsigned char mirroredImg[Size][Size][BytesPerPixel];

	for(int r=0;r<Size;r++){
		for(int c=0;c<Size;c++){
			for(int i=0;i<BytesPerPixel;i++){
				mirroredImg[r][Size-c][i];
			}
		}
	}

	return mirroredImg;

}

//Resizing the image
unsigned char Resize(int width, int height){
	unsigned char resizedImg[height][width][BytesPerPixel];

	int wMultiply=width/Size;
	int hMultiply=height/Size;

	for(int r=0;r<height;r++){
		for(int c=0;c<width;c++){
			for(int i=0;i<BytesPerPixel;i++){
				double originR = r/hMultiply;
				double originc = c/wMultiply;

				unsigned char originColor = BiInterpolation()

				resizedImg[r][c][i]=originColor;
			}
		}			
	}

	return resizedImg;
}

unsigned char BiInterpolation(unsigned char tl, unsigned char tr,unsigned char bl,unsigned char br,double a,double b){

}
