// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;



int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=1;

	// Check for proper syntax
	if (argc < 6){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw height width range" << endl;
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

	//define the range
	int range=atoi(argv[5]);

	//calcualte the histogram
	vector<unsigned int> hist(256,0);
	vector<double> culhis(256,0);

	for(int r=0;r<height;r++){
		for(int c=0;c<width;c++){
			hist.at(Imagedata[r][c][0])+=1;
		}
	}
	culhis.at(0)=hist.at(0)/(height*width);
	for(int i=1;i<256;i++){
		culhis.at(i)=culhis.at(i-1)+hist.at(i)/(double)(height*width);
	}

	for(int r=0;r<height;r++){
		for(int c=0;c<width;c++){
			
			Imagedata[r][c][0]=(unsigned char)(culhis.at((int)Imagedata[r][c][0])*range);
			cout<<(int)Imagedata[r][c][0]<<endl;
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

