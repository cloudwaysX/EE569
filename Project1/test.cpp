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

unsigned char RGBtoGRAY(unsigned char R, unsigned char G, unsigned char B);

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
	cout<<"d"<<endl;

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	//unsigned char out_gray[height][width];
	unsigned char*** out_gray= new unsigned char**[height];
	for(int i=0;i<height;i++){
		out_gray[i]=new unsigned char*[width];
	}
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			out_gray[i][j] = new unsigned char[BytesPerPixel];
		}
	}

	int selectChannel=atoi(argv[5]);
	unsigned char normalizeParameter = 255;

	for(int r=0;r<height;r++){
		for(int c=0;c<width;c++){

			unsigned char R=Imagedata[r][c][0];
			unsigned char G=Imagedata[r][c][1];
			unsigned char B=Imagedata[r][c][2];

			//unsigned char I=RGBtoGRAY(R,G,B);
			out_gray[r][c][0]=R;
			out_gray[r][c][1]=G;
			out_gray[r][c][2]=B;

		}
	}

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(out_gray, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	for()

	return 0;
}

unsigned char RGBtoGRAY(unsigned char R, unsigned char G, unsigned char B){
	return 0.21*R+0.72*G+0.07*B;
}
