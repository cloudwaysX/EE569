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

cv::Mat FindCorner(int*** img, int mode,int range_begin, int range_end);
bool isNoise(int*** img,int r, int c);
unsigned char BiInterpolation(unsigned char tl, unsigned char tr,unsigned char bl,unsigned char br,double a,double b);


int Size = 512;
int Size_piece=500;
int range_begin;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=3;
	
	// Check for proper syntax
	if (argc < 5){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw roughRange" << endl;
		return 0;
	}

	//here is the rough range to find the pieces
	range_begin=atoi(argv[3]);
	int range_end=atoi(argv[4]);
	
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
			for(int k=0;k<BytesPerPixel;k++){
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
			for(int k=0;k<BytesPerPixel;k++){
				piece[i][j][k]=Imagedata[i][j][k];
			}
		}
	}



	cv::Mat corner_orginal=FindCorner(original,1,range_begin,range_end);
	cv::Mat corner_piece=FindCorner(piece,0,range_begin,range_end);
	cv::Mat backM;

	int overlay;
	if(range_begin<250){
		//Adjust the edge
		overlay=3;
		corner_orginal.colRange(0,3).at<float>(0,0)-=overlay;
		corner_orginal.colRange(0,3).at<float>(0,1)+=overlay;
		corner_orginal.colRange(0,3).at<float>(0,2)+=overlay;
		corner_orginal.colRange(0,3).at<float>(1,0)-=overlay;
		corner_orginal.colRange(0,3).at<float>(1,1)-=overlay;
		corner_orginal.colRange(0,3).at<float>(1,2)+=overlay;
		cout<<"overlay is:"<<overlay<<endl;
		//calculate transform matrix and display it
		cout<<"this is back transform matrix"<<endl;
		backM = corner_piece.colRange(1,4)*(corner_orginal.colRange(0,3)).inv();
	}
	else{

		overlay=1;
		cout<<"overlay is:"<<overlay<<endl;
		corner_orginal.colRange(1,4).at<float>(0,0)+=overlay;
		corner_orginal.colRange(1,4).at<float>(0,1)-=overlay;
		corner_orginal.colRange(1,4).at<float>(0,2)+=overlay;
		corner_orginal.colRange(1,4).at<float>(1,0)+=overlay;
		corner_orginal.colRange(1,4).at<float>(1,1)-=overlay;
		corner_orginal.colRange(1,4).at<float>(1,2)+=overlay;
		//calculate transform matrix and display it
		cout<<"this is back transform matrix"<<endl;
		backM = corner_piece.colRange(0,3)*(corner_orginal.colRange(1,4)).inv();
	}


	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			cout<<backM.at<float>(i,j)<<" ";
		}
		cout<<endl;
	}

	for(int y=corner_orginal.at<float>(1,0)-overlay;y<=corner_orginal.at<float>(1,0)+99+overlay;y++){
		for(int x=corner_orginal.at<float>(0,0)-overlay;x<=corner_orginal.at<float>(0,0)+99+overlay;x++){
			double back_x=backM.at<float>(0,0)*x+backM.at<float>(0,1)*y+backM.at<float>(0,2);

			double back_y=backM.at<float>(1,0)*x+backM.at<float>(1,1)*y+backM.at<float>(1,2);
			double a=back_y-floor(back_y);
			double b=back_x-floor(back_x);

			for(int i=0;i<BytesPerPixel;i++){

				unsigned char tl=piece[(int)floor(back_y)][(int)floor(back_x)][i];
								//cout<<"a1"<<endl;
				unsigned char tr=piece[(int)floor(back_y)][(int)floor(back_x)+1][i];
								//cout<<"a2"<<endl;
				unsigned char bl=piece[(int)floor(back_y)+1][(int)floor(back_x)][i];
								//cout<<"a3"<<endl;
				unsigned char br=piece[(int)floor(back_y)+1][(int)floor(back_x)+1][i];
				
				//cout<<"a"<<endl;
				int temp=(int)BiInterpolation(tl,tr,bl,br,a,b);
				if(original[y][x][i]==255){
					original[y][x][i]=(int)BiInterpolation(tl,tr,bl,br,a,b);
				}
			}
		}
	}
	for(int y=corner_orginal.at<float>(1,0)-overlay;y<=corner_orginal.at<float>(1,0)+99+overlay;y++){
		for(int x=corner_orginal.at<float>(0,0)-overlay;x<=corner_orginal.at<float>(0,0)+99+overlay;x++){
			if(abs(x-corner_orginal.at<float>(0,0))<=3||abs(x-corner_orginal.at<float>(0,0)-99)<=3||abs(y-corner_orginal.at<float>(1,0))<=3||abs(y-corner_orginal.at<float>(1,0)-99)<=3){
				for(int k=0;k<BytesPerPixel;k++){
					int temp=0;
					for(int i=-2;i<=2;i++){
						for(int j=-2;j<=2;j++){
							temp+=original[y+i][x+j][k];
						}
					}
					original[y][x][k]=temp/25;
				}
			}
		}
	}

	for(int i =0;i<Size;i++){
		for(int j=0;j<Size;j++){
			for(int k=0;k<BytesPerPixel;k++){
				Imagedata2[i][j][k]=original[i][j][k];
			}
		}
	}

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata2, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

	return 0;
}


cv::Mat FindCorner(int*** img, int mode,int range_begin, int range_end){
	//mode 0 means this is the piece, mode 1 means this is original image
	int most_left = 1000; int paired_most_left=1000;
	int most_right=0; int paired_most_right=0;
	int most_up=1000; int paired_most_up=1000;
	int most_bt=0; int paired_most_bt=0;
	if(mode==0){
		cout<<"this is the piece"<<endl;
		for(int r=0;r<Size_piece;r++){
			for(int c=range_begin;c<range_end;c++){

				if(range_begin>0){
					if(img[r][c][0]<=210&&img[r][c][1]<=145&&img[r][c][2]<=110){

						if(c<most_left){most_left=c; paired_most_left=r;}
						if(c>most_right){most_right=c; paired_most_right=r;}
						if(r<most_up){most_up=r;paired_most_up=c;}
						if(r>most_bt){most_bt=r;paired_most_bt=c;}
					}
				}
				else{
					if(img[r][c][0]!=255||img[r][c][1]!=255||img[r][c][2]!=255){
						if(c<most_left){most_left=c; paired_most_left=r;}
						if(c>most_right){most_right=c; paired_most_right=r;}
						if(r<most_up){most_up=r;paired_most_up=c;}
						if(r>most_bt){most_bt=r;paired_most_bt=c;}
					}

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
					if(r<=most_up && !isNoise(img,r,c)){most_up=r;paired_most_up=c;continue;}
					if(c>=most_right&&!isNoise(img,r,c)){most_right=c; paired_most_right=r;continue;}
					if(r>most_bt && !isNoise(img,r,c)){most_bt=r;paired_most_bt=c;continue;}
				}

			}
		}
	}



	cout<<"y    x"<<endl;
	cout<<paired_most_left<<" "<<most_left<<endl;
	cout<<most_up<<" "<<paired_most_up<<endl;
	cout<<paired_most_right<<" "<<most_right<<endl;
	cout<<most_bt<<" "<<paired_most_bt<<endl;

	cv::Mat result = (cv::Mat_<float>(3, 4) <<
		most_left,paired_most_up,most_right,paired_most_bt,
		paired_most_left, most_up,paired_most_right,most_bt,
		1,1,1,1);
	return result;
}

bool isNoise(int*** img,int r, int c){
	bool isNoise=true;
	int cout=0;
	for(int i=-1;i<=1;i++){
		for(int j=-1;j<=1;j++){
			if(i==0&&j==0) continue;
			if(r+i<0 || r+i>=512 || c+j<0 || c+j>=512)  continue;
			if( img[r+i][c+j][0]==255 && img[r+i][c+j][1]==255 &&img[r+i][c+j][2]==255){
				if(++cout>=3){
					isNoise=false;
					break;
				}
			}
		}
	}
	return isNoise;
}

unsigned char BiInterpolation(unsigned char tl, unsigned char tr,unsigned char bl,unsigned char br,double a,double b){
	return (1-b)*(1-a)*tl+b*(1-a)*tr+a*(1-b)*bl+a*b*br;

}