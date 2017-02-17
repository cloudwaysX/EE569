// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>


using namespace std;

double CalcH(double M, double C, int mode, double diff);

double CalcL(double M, double m);

double CalcS(double L, double C);


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
	unsigned char out[height][width][1];
	int selectChannel=atoi(argv[5]);
	unsigned char normalizeParameter = 255;

	for(int r=0;r<height;r++){
		for(int c=0;c<width;c++){
			double R=Imagedata[r][c][0]/255.0;
			double G=Imagedata[r][c][1]/255.0;
			double B=Imagedata[r][c][2]/255.0;

			double M=max(max(R,G),B);
			double m=min(min(R,G),B);
			double C=M-m;

			double L=CalcL(M,m);
			double S=CalcS(L,C);

			int mode=0;
			double diff=0;

			if(M==R){
				mode=1; diff=G-B;
			}
			else if(M==G){
				mode=2; diff=B-R;
			}
			else{
				mode=3; diff=R-G;
			}
			double H=CalcH(M,C,mode,diff);

			unsigned char temp[3]={H/360.0*255,S*255,L*255};
			//unsigned char temp[3]={H/360.0*255,S*255,G*255};
			out[r][c][0]=temp[selectChannel];
			//out[r][c][0]=L*255;
			//cout<<(int)temp[selectChannel]<<endl;

			
		}
	}

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(out, sizeof(unsigned char), height*width, file);
	fclose(file);

	return 0;
}

//0 to 360
double CalcH(double M, double C, int mode, double diff){
	if(C==0) return 0;

	switch(mode){
		case 1 : return 60*((diff/C)-(int)(diff/C/6)*6);
		case 2 : return 60*(diff/C+2);
		case 3 : return 60*(diff/C+4);
	}		
}

//0 to 1
double CalcL(double M, double m){
	return (M+m)/2.0;
}

//0 to 1
double CalcS(double L, double C){
	if(L==0) return 0;

	if(L<0.5) return C/L/2.0;

	return C/(1.0-L)/2.0;	
}
