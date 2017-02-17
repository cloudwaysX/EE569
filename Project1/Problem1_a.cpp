// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

// Define file pointer and variables
FILE *file;
int BytesPerPixel;
int Size = 256;

//void Mirror(unsigned char*** Imagedata, unsigned char*** mirroredImg);
	


using namespace std;
int main(int argc, char *argv[])

{
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 4){
		BytesPerPixel = 1; // default is grey image
	} 
	else {
		BytesPerPixel = atoi(argv[3]);
		// Check if size is specified
		if (argc >= 5){
			Size = atoi(argv[4]);
		}
	}
	
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
	//Mirror the Image acrross vertical axis

	//Initilize the pointer for mirroredImg here
	//cout<<BytesPerPixel<<endl;
	unsigned char*** mirroredImg= new unsigned char**[Size];
	for(int i=0;i<Size;i++){
		mirroredImg[i]=new unsigned char*[Size];
		for(int j=0;j<Size;j++){
			mirroredImg[i][j]=new unsigned char[BytesPerPixel];
		}
	}

	//Initilize the pointer for original here
	unsigned char*** originalImg= new unsigned char**[Size];
	for(int i=0;i<Size;i++){
		originalImg[i]=new unsigned char*[Size];
		for(int j=0;j<Size;j++){
			originalImg[i][j]=new unsigned char[BytesPerPixel];
			for(int k=0;k<BytesPerPixel;k++){
				originalImg[i][j][k]=Imagedata[i][j][k];
			}
		}
	}

	/*unsigned char originalImg[Size][Size][BytesPerPixel];
	for(int i=0;i<Size;i++){
		for(int j=0;j<Size;j++){
			for(int k=0;k<BytesPerPixel;k++){
				originalImg[i][j][k]=Imagedata[i][j][k];
			}
		}
	}*/


	//Mirror(originalImg,mirroredImg);
	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(originalImg, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

	return 0;
}

/*void Mirror(unsigned char*** Imagedata, unsigned char*** mirroredImg){
	for(int r=0;r<Size;r++){
		for(int c=0;c<Size;c++){
			for(int i=0;i<BytesPerPixel;i++){
				mirroredImg[r][Size-c-1][i]=Imagedata[r][c][i];
			}
		}
	}
}*/

/*
//Resizing the image
unsigned char Resize(int width, int height){
	unsigned char resizedImg[height][width][BytesPerPixel];

	int wMultiply=width/Size;
	int hMultiply=height/Size;

	for(int r=0;r<height;r++){
		for(int c=0;c<width;c++){
			for(int i=0;i<BytesPerPixel;i++){
				double originR = r/hMultiply;
				double originc = c/wMultiply;

				unsigned char originColor = BiInterpolation()

				resizedImg[r][c][i]=originColor;
			}
		}			
	}

	return resizedImg;
}

unsigned char BiInterpolation(unsigned char tl, unsigned char tr,unsigned char bl,unsigned char br,double a,double b){

}
*/
