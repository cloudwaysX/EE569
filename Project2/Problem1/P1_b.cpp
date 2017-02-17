// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#include "opencv2/core/mat.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.hpp"

using namespace std;
//using namespace cv;

cv::Mat FindConer(int*** img, int mode);
bool isNoise(int*** img,int r, int c);

int Size = 512;
int Size_piece=500;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=3;
	
	// Check for proper syntax
	if (argc < 4){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw mode=matching/transform" << endl;
		return 0;
	}
	
	
	// Allocate image data array
	unsigned char Imagedata2[Size][Size][BytesPerPixel];
	unsigned char Imagedata[Size_piece][Size_piece][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen("../images/P1/piece.raw","rb"))) {
		cout << "Cannot open file: " <<"piece.raw" <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata2, sizeof(unsigned char), Size_piece*Size_piece*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	int*** original=new int**[Size];
	for(int i =0;i<Size;i++){
		original[i]=new int*[Size];
	}
	for(int i =0;i<Size;i++){
		for(int j=0;j<Size;j++){
			original[i][j]=new int[BytesPerPixel];
		}
	}
	for(int i =0;i<Size;i++){
		for(int j=0;j<Size;j++){
			for(int k=0;k<3;k++){
				original[i][j][k]=Imagedata2[i][j][k];
			}
		}
	}

	int*** piece=new int**[Size_piece];
	for(int i =0;i<Size_piece;i++){
		piece[i]=new int*[Size_piece];
	}
	for(int i =0;i<Size_piece;i++){
		for(int j=0;j<Size_piece;j++){
			piece[i][j]=new int[BytesPerPixel];
		}
	}
	for(int i =0;i<Size_piece;i++){
		for(int j=0;j<Size_piece;j++){
			for(int k=0;k<3;k++){
				piece[i][j][k]=Imagedata[i][j][k];
			}
		}
	}



	cv::Mat corner_orginal=FindConer(original,1);
	cv::Mat corner_piece=FindConer(piece,0);
	


	// Write image data (filename specified by second argument) from image data matrix

	/*if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);*/

	return 0;
}


cv::Mat FindConer(int*** img, int mode){
	//mode 0 means this is the piece, mode 1 means this is original image
	int most_left = 1000; int paired_most_left=1000;
	int most_right=0; int paired_most_right=0;
	int most_up=1000; int paired_most_up=1000;
	int most_bt=0; int paired_most_bt=0;
	if(mode==0){
		cout<<"this is the piece"<<endl;
		for(int r=0;r<Size_piece;r++){
			for(int c=0;c<Size_piece/2;c++){
				//cout<<img[r][c][0]<<" "<<img[r][c][1]<<" "<<img[r][c][2]<<endl;
				if(img[r][c][0]!=255||img[r][c][1]!=255||img[r][c][2]!=255){
					if(c<most_left){most_left=c; paired_most_left=r;}
					if(c>most_right){most_right=c; paired_most_right=r;}
					if(r<most_up){most_up=r;paired_most_up=c;}
					if(r>most_bt){most_bt=r;paired_most_bt=c;}
				}

			}
		}

	}
	else{
		cout<<"this is the original"<<endl;
		for(int r=0;r<Size;r++){
			for(int c=0;c<Size;c++){
				//cout<<img[r][c][0]<<" "<<img[r][c][1]<<" "<<img[r][c][2]<<endl;
				if(img[r][c][0]==255&&img[r][c][1]==255&&img[r][c][2]==255){
					if(c<most_left && !isNoise(img,r,c)){most_left=c; paired_most_left=r;continue;}
					if(r<most_up && !isNoise(img,r,c)){most_up=r;paired_most_up=c;continue;}
					//if(c<most_left){most_left=c; paired_most_left=r;}
					if(c>most_right&& !isNoise(img,r,c)){most_right=c; paired_most_right=r;continue;}
					//if(r<most_up&& !isNoise(img,r,c)){most_up=r;paired_most_up=c;}
					if(r>most_bt && !isNoise(img,r,c)){most_bt=r;paired_most_bt=c;continue;}
				}

			}
		}
	}
	//four points are always clock-wise
	cout<<most_left<<" "<<paired_most_left<<endl;
	cout<<most_up<<" "<<paired_most_up<<endl;
	cout<<most_right<<" "<<paired_most_right<<endl;
	cout<<most_bt<<" "<<paired_most_bt<<endl;
	cout<<img[paired_most_left][most_left][0]<<" "<<img[paired_most_left][most_left][1]<<" "<<img[paired_most_left][most_left][2]<<endl;
	//int points[]={most_left, paired_most_left, paired_most_right, most_up, most_right, paired_most_right, paired_most_bt, most_bt};
	int points[3][4]={
		{most_left,paired_most_up, most_right,paired_most_bt}, 
		{paired_most_left,most_up,paired_most_right,most_bt},
		{1,1,1,1}
	};
	cv::Mat result(3,4,CV_8UC1);
	for(int i=0;i<3;i++){
		for(int j=0;j<4;j++){
			result.at<int>(i,j)=points[i][j];
		}
	}
	//temp.at<int>()
	return result;
}

bool isNoise(int*** img,int r, int c){
	bool isNoise=true;
	for(int i=-1;i<=1;i++){
		for(int j=-1;j<1;j++){
			if(i==0&&j==0) continue;
			if(r+i<0 || r+i>=512 || c+j<0 || c+j>=512)  continue;
			if( img[r+i][c+j][0]==255 && img[r+i][c+j][1]==255 &&img[r+i][c+j][2]==255){
				isNoise=false;
				break;
			}
		}
	}
	return isNoise;
}