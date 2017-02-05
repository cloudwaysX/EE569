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
	if (argc < 5){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw height width" << endl;
		return 0;
	}
	if(argc==6){
		BytesPerPixel=atoi(argv[5]);
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
	double k=3;
	double b1=10;
	double b2=-540;

	cout<<height<<" "<<width<<endl;
	for(int i=0;i<BytesPerPixel;i++){
		cout<<i<<endl;
		for(int r=0;r<height;r++){
			for(int c=0;c<width;c++){
				int temp=Imagedata[r][c][i];
				double b;
				if(temp<80){
					b=b1;
				}
				else{
					b=b2;
				}
				temp=k*(double)temp+b;
				temp=min(temp,255-(temp-255));
				temp=max(temp,-temp);
				Imagedata[r][c][i]=temp;
			}
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

