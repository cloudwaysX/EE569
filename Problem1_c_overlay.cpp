// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;



using namespace std;


int main(int argc, char *argv[])

{

	int BytesPerPixel=3;
	FILE *file;
	FILE *file_back;

	// Check for proper syntax
	if (argc < 6){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw input_background_image.raw output_image.raw height width" << endl;
		return 0;
	}
	

	int height = atoi(argv[4]);
	int width =atoi(argv[5]);


	
	// Allocate image data array
	unsigned char Imagedata[height][width][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	unsigned char Imagedata_back[height][width][BytesPerPixel];
	if (!(file_back=fopen(argv[2],"rb"))) {
		cout << "Cannot open file: " << argv[2] <<endl;
		exit(1);
	}
	fread(Imagedata_back, sizeof(unsigned char), height*width*BytesPerPixel, file_back);
	fclose(file_back);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////	


	for(int r=0;r<height;r++){
		for(int c=0;c<width;c++){
			for(int k=0;k<BytesPerPixel;k++){
				double a=Imagedata_back[r][c][k]/255.0;
				double b=Imagedata[r][c][k]/255.0;
				double f;
				if(a<0.5){
					f=2.0*a*b;
				}
				else{
					f=1-2*(1-a)*(1-b);
				}
				Imagedata_back[r][c][k]=f*255;
			}
		}
	}


	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[3],"wb"))) {
		cout << "Cannot open file: " << argv[3] << endl;
		exit(1);
	}
	fwrite(Imagedata_back,sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	return 0;
}



