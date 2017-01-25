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

// Define file pointer and variables
FILE *file;
int BytesPerPixel;
int Size = 256;


using namespace std;

unsigned char BiInterpolation(unsigned char tl, unsigned char tr,unsigned char bl,unsigned char br,double a,double b);

int main(int argc, char *argv[])

{

	int resizeScale=1;

	// Check for proper syntax
	if (argc < 4){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 5){
		BytesPerPixel = 1; // default is grey image
	} 
	else {
		BytesPerPixel = atoi(argv[4]);
		// Check if size is specified
		if (argc >= 6){
			Size = atoi(argv[5]);
		}
	}
	resizeScale=atoi(argv[3]);
	cout<<"resizeing into: "<<resizeScale<<" * "<<resizeScale<<endl;


	
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
	int height=resizeScale; int width=resizeScale;
	unsigned char resizedImg[height][width][BytesPerPixel];


	double wMultiply=(double)width/Size;
	double hMultiply=(double)height/Size;
	for(int r=0;r<height;r++){
		for(int c=0;c<width;c++){
			double originR = (double)(r+1)/hMultiply-1;
			double originC = (double)(c+1)/wMultiply-1;
			originC=max(originC,0.0);
			originR=max(originR,0.0);

			double a=originR-floor(originR);
			double b=originC-floor(originC);
			for(int i=0;i<BytesPerPixel;i++){
				unsigned char tl=Imagedata[(int)floor(originR)][(int)floor(originC)][i];
				unsigned char tr=Imagedata[(int)floor(originR)][(int)floor(originC)+1][i];
				unsigned char bl=Imagedata[(int)floor(originR)+1][(int)floor(originC)][i];
				unsigned char br=Imagedata[(int)floor(originR)+1][(int)floor(originC)+1][i];
				unsigned char originColor = BiInterpolation(tl,tr,bl,br,a,b);
				resizedImg[r][c][i]=originColor;

			}
		}			
	}

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(resizedImg,sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	return 0;
}



unsigned char BiInterpolation(unsigned char tl, unsigned char tr,unsigned char bl,unsigned char br,double a,double b){
	return (1-b)*(1-a)*tl+b*(1-a)*tr+a*(1-b)*bl+a*b*br;

}

