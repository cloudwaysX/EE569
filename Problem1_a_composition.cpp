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
	if (argc < 8){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name threshold input_image.raw input_background_image.raw output_image.raw size1 size2's height+width" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color

	int height = atoi(argv[6]);
	int width =atoi(argv[7]);
	int Size=atoi(argv[5]);
	unsigned char threshold = atoi(argv[1]);

	cout<<"current threshold on the B channel is: "<<(int)threshold<<endl;


	
	// Allocate image data array
	unsigned char Imagedata[Size][Size][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[2],"rb"))) {
		cout << "Cannot open file: " << argv[2] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

	unsigned char Imagedata_back[height][width][BytesPerPixel];
	if (!(file_back=fopen(argv[3],"rb"))) {
		cout << "Cannot open file: " << argv[3] <<endl;
		exit(1);
	}
	fread(Imagedata_back, sizeof(unsigned char), height*width*BytesPerPixel, file_back);
	fclose(file_back);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////	



	int default_r=400;
	int default_c=1100;

	for(int r=0;r<Size;r++){
		for(int c=0;c<Size;c++){
			for(int i=0;i<BytesPerPixel;i++){
				if(Imagedata[r][c][2]<threshold)
					Imagedata_back[r+default_r][c+default_c][i] = Imagedata[r][c][i];

			}
		}
	}


	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[4],"wb"))) {
		cout << "Cannot open file: " << argv[4] << endl;
		exit(1);
	}
	fwrite(Imagedata_back,sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	return 0;
}



