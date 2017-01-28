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

unsigned char SepiaFilter_gray(double p1,double p2,double p3,unsigned char I);

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
	//Mirror the Image acrross vertical axis
	cout<<"a"<<endl;
	unsigned char out_gray[height][width];
	cout<<"b"<<endl;
	//unsigned char out_Sephia[height][width][BytesPerPixel];
	cout<<"c"<<endl;
	int selectChannel=atoi(argv[5]);
	unsigned char normalizeParameter = 255;

	for(int r=0;r<height;r++){
		for(int c=0;c<width;c++){

			unsigned char R=Imagedata[r][c][0];
			unsigned char G=Imagedata[r][c][1];
			unsigned char B=Imagedata[r][c][2];

			unsigned char I=RGBtoGRAY(R,G,B);
			out_gray[r][c]=I;

			Imagedata[r][c][0]=SepiaFilter_gray(0.393,0.769,0.189,I);
			Imagedata[r][c][1]=SepiaFilter_gray(0.349,0.686,0.168,I);
			Imagedata[r][c][2]=SepiaFilter_gray(0.272,0.534,0.131,I);


		}
	}

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	if(selectChannel==0){
		fwrite(out_gray, sizeof(unsigned char), height*width, file);
	}
	else{
		fwrite(Imagedata, sizeof(unsigned char), height*width*BytesPerPixel, file);
	}
	fclose(file);

	return 0;
}

unsigned char RGBtoGRAY(unsigned char R, unsigned char G, unsigned char B){
	return 0.21*R+0.72*G+0.07*B;
}

unsigned char SepiaFilter_gray(double p1,double p2,double p3,unsigned char I){
	int result=min((p1+p2+p3)*(int)I,255.0);
	return result;
}