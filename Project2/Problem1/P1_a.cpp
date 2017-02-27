// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include "opencv2/core/mat.hpp"
using namespace std;
using namespace cv;
unsigned char BiInterpolation(unsigned char tl, unsigned char tr,unsigned char bl,unsigned char br,double a,double b);
int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=3;
	int Size = 500;
	
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw" << endl;
		return 0;
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
	//4 boundaries is x+1-y=0; -(x+y-1)=0; x+y+1=0; -(x-y-1)=0;
	double centerX=Size/2-0.5; double centerY=Size/2-0.5;
	int*** img=new int**[Size];
	for(int i =0;i<Size;i++){
		img[i]=new int*[Size];
	}
	for(int i =0;i<Size;i++){
		for(int j=0;j<Size;j++){
			img[i][j]=new int[BytesPerPixel];
		}
	}
	for(int k=0;k<BytesPerPixel;k++)
		for(int r=0;r<Size;r++){
			for(int c=0;c<Size;c++){
				int x=c- centerX;
				int y=-(r- centerY);
				int unit=Size/2-0.5;
				//cout<<x<<" "<<y<<endl;
				if((x+unit-y)>0&&(-(x+y-unit))>0&&(x+y+ unit)>0&&(-(x-y- unit))>0){
					double x_max=unit-abs(y);
					//cout<<"x: "<<x<<" "<<"y: "<<y<<"x_max:"<<x_max<<endl;
					double scale=(double)abs(x)/x_max;
					double sourceX;
					if(x<0){
						sourceX=(-Size/2+0.5)*scale;
					}
					else{
						sourceX=(Size/2-0.5)*scale;
					}
					double back_y=-y+centerY;
					double back_x=sourceX+centerX;
					double a=back_y-floor(back_y);
					double b=back_x-floor(back_x);
					if(floor(back_y)<0||floor(back_y)>=Size-1||floor(back_x)<0||floor(back_x)>=Size-1) continue;
					unsigned char tl=Imagedata[(int)floor(back_y)][(int)floor(back_x)][k];
					unsigned char tr=Imagedata[(int)floor(back_y)][(int)floor(back_x)+1][k];
					unsigned char bl=Imagedata[(int)floor(back_y)+1][(int)floor(back_x)][k];
					unsigned char br=Imagedata[(int)floor(back_y)+1][(int)floor(back_x)+1][k];
					//cout<<tl<<" "<<tr<<" "<<bl<<" "<<br<<endl;

				
					int temp=(int)BiInterpolation(tl,tr,bl,br,a,b);
					img[r][c][k]=temp;

				}
				else{
					img[r][c][k]=0;
				}
			}
		}

	// Write image data (filename specified by second argument) from image data matrix
	for(int r=0;r<Size;r++){
		for(int c=0;c<Size;c++){
			for(int k=0;k<BytesPerPixel;k++){
				Imagedata[r][c][k]=img[r][c][k];
			}
		}
	}

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

	return 0;
}

unsigned char BiInterpolation(unsigned char tl, unsigned char tr,unsigned char bl,unsigned char br,double a,double b){
	return (1-b)*(1-a)*tl+b*(1-a)*tr+a*(1-b)*bl+a*b*br;

}