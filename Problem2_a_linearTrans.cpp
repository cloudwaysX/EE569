// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=1;

	// Check for proper syntax
	if (argc < 6){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw height width slope" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
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

	//define the transfer funciton y=kx+b
	double k=atof(argv[5]);
	double b=128-128*k;

	cout<<height<<" "<<width<<endl;

	for(int r=0;r<height;r++){
		for(int c=0;c<width;c++){
			int temp=Imagedata[r][c][0];
			//cout<<"before:"<<temp<<endl;
			temp=k*(double)temp+b;
			//cout<<"after 1:"<<temp<<endl;
			temp=min(temp,255-(temp-255));
			//cout<<"after 2:"<<temp<<endl;
			temp=max(temp,-temp);
			Imagedata[r][c][0]=temp;
			//cout<<"after:"<<temp<<endl;
		}
	}


	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);

	return 0;
}

