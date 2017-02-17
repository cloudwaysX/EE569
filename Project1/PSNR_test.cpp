// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>

using namespace std;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=3;
	
	// Check for proper syntax
	if (argc < 5){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image_noise.raw input_image_noisefree.raw height width" << endl;
		return 0;
	}

	int height=atoi(argv[3]);
	int width=atoi(argv[4]);

	
	// Allocate image data array
	unsigned char Imagedata_noise[height][width][BytesPerPixel];
	unsigned char Imagedata[height][width][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata_noise, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);
	if (!(file=fopen(argv[2],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	double MSE2=0;
	for(int k=0;k<BytesPerPixel;k++){
		double MSE=0;
		for(int r=0;r<height;r++){
			for(int c=0;c<width;c++){
				MSE+=pow(Imagedata_noise[r][c][k]-Imagedata[r][c][k],2)/(double)(width*height);
				MSE2+=pow(Imagedata_noise[r][c][k]-Imagedata[r][c][k],2)/(double)(width*height*3);
			}
		}
		cout<<"the PSNR of channel: "<<k<<"is "<<10*log10(255*255/MSE)<<endl;
	}
	cout<<"total PSNR is: "<<10*log10(255*255/MSE2)<<endl;
	return 0;
}
