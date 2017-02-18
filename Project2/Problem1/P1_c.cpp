
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

unsigned char BiInterpolation(unsigned char tl, unsigned char tr,unsigned char bl,unsigned char br,double a,double b);




int main(int argc, char *argv[])

{
	if (argc < 2){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name mode[tartan=0/trojan=1]" << endl;
		return 0;
	}

	int mode=atoi(argv[1]);

	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=3;

	int width_original=972;
	int height_original=648;
	int height_word,width_word;
	unsigned char Imagedata[height_original][width_original][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen("../images/P1/field.raw","rb"))) {
		cout << "Cannot open file: " <<"Field.raw" <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), height_original*width_original*BytesPerPixel, file);
	fclose(file);
	cout<<mode<<endl;
	if(mode==0){
		if (!(file=fopen("../images/P1/tartans.raw","rb"))) {
			cout << "Cannot open file: " << "tartan.raw" <<endl;
			exit(1);
		}
			width_word=350;
			height_word=146;
	}
	else{
		if (!(file=fopen("../images/P1/trojans.raw","rb"))) {
			cout << "Cannot open file: " << "Trojan.raw" <<endl;
			exit(1);
		}
		width_word=350;
		height_word=197;
	}
	unsigned char Imagedata2[height_word][width_word][BytesPerPixel];

	fread(Imagedata2, sizeof(unsigned char), height_word*width_word*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	//initilize the point
	int*** original=new int**[height_original];
	for(int i =0;i<height_original;i++){
		original[i]=new int*[width_original];
	}
	for(int i =0;i<height_original;i++){
		for(int j=0;j<width_original;j++){
			original[i][j]=new int[BytesPerPixel];
		}
	}
	for(int i =0;i<height_original;i++){
		for(int j=0;j<width_original;j++){
			for(int k=0;k<BytesPerPixel;k++){
				original[i][j][k]=Imagedata[i][j][k];
			}
		}
	}

	int*** word=new int**[height_word];
	for(int i =0;i<height_word;i++){
		word[i]=new int*[width_word];
	}
	for(int i =0;i<height_word;i++){
		for(int j=0;j<width_word;j++){
			word[i][j]=new int[BytesPerPixel];
		}
	}
	for(int i =0;i<height_word;i++){
		for(int j=0;j<width_word;j++){
			for(int k=0;k<BytesPerPixel;k++){
				word[i][j][k]=Imagedata2[i][j][k];
			}
		}
	}


	//select the control point
	int x1_tl=320;int y1_tl=600; int x1_tr=542; int y1_tr=428;int x1_br=652;int y1_br=430;int x1_bl=528;int y1_bl=612;//field image
	int x2_tl,y2_tl=0,x2_tr,y2_tr,x2_br,y2_br,x2_bl,y2_bl;//word image
	if(mode==0){
		x2_tl=0;y2_tl=0; x2_tr=349; y2_tr=0;x2_br=349; y2_br=145; x2_bl=0; y2_bl=145;//word image
	}
	else{
		x2_tl=0;y2_tl=0; x2_tr=349; y2_tr=0;x2_br=349; y2_br=196; x2_bl=0; y2_bl=196;//word image
	}
	//calculate transform matrix and display it
	cv::Mat Backtarget = (cv::Mat_<float>(1, 8) <<
		x2_tl,y2_tl, x2_tr, y2_tr,x2_br,y2_br,x2_bl,y2_bl);
	cv::Mat Backsource =(cv::Mat_<float>(8, 8) <<
		x1_tl,y1_tl,1,0,0,0,-x1_tl*x2_tl,-y1_tl*x2_tl,
		0,0,0,x1_tl,y1_tl,1,-x1_tl*y2_tl,-y1_tl*y2_tl,
		x1_tr,y1_tr,1,0,0,0,-x1_tr*x2_tr,-y1_tr*x2_tr,
		0,0,0,x1_tr,y1_tr,1,-x1_tr*y2_tr,-y1_tr*y2_tr,
		x1_br,y1_br,1,0,0,0,-x1_br*x2_br,-y1_br*x2_br,
		0,0,0,x1_br,y1_br,1,-x1_br*y2_br,-y1_br*y2_br,
		x1_bl,y1_bl,1,0,0,0,-x1_bl*x2_bl,-y1_bl*x2_bl,
		0,0,0,x1_bl,y1_bl,1,-x1_bl*y2_bl,-y1_bl*y2_bl);
	//cv::transpose(Backsource,Backsource);
	cout<<"this is back transform matrix"<<endl;
	cv::Mat backM = Backtarget*(Backsource.t()).inv();


	for(int i=0;i<8;i++){
		cout<<backM.at<float>(0,i)<<" ";
	}
	cout<<endl;
	for(int y=min(min(y1_tl,y1_bl),min(y1_tr,y1_br));y<max(max(y1_tl,y1_bl),max(y1_tr,y1_br));y++){
		for(int x=min(min(x1_tl,x1_bl),min(x1_tr,x1_br));x<max(max(x1_tl,x1_bl),max(x1_tr,x1_br));x++){
			double back_x=backM.at<float>(0,0)*x+backM.at<float>(0,1)*y+backM.at<float>(0,2);
			double back_y=backM.at<float>(0,3)*x+backM.at<float>(0,4)*y+backM.at<float>(0,5);
			double w=backM.at<float>(0,6)*x+backM.at<float>(0,7)*y+1;
			back_x=back_x/w;
			back_y=back_y/w;
			if(back_x<0||back_x>=x2_tr-1||back_y<0||back_y>=y2_bl-1) continue;
			double a=back_y-floor(back_y);
			double b=back_x-floor(back_x);
			int temp[3];

			for(int i=0;i<BytesPerPixel;i++){
				unsigned char tl=word[(int)floor(back_y)][(int)floor(back_x)][i];
				unsigned char tr=word[(int)floor(back_y)][(int)floor(back_x)+1][i];
				unsigned char bl=word[(int)floor(back_y)+1][(int)floor(back_x)][i];
				unsigned char br=word[(int)floor(back_y)+1][(int)floor(back_x)+1][i];

				temp[i]=(int)BiInterpolation(tl,tr,bl,br,a,b);
			}
			if((mode==0&&temp[0]<=10&&temp[1]<=10&&temp[2]<=10)||(mode==1&&temp[0]>=230&&temp[1]>=230&&temp[2]>=230)){
				continue;
			}
			original[y][x][0]=temp[0];
			original[y][x][1]=temp[1];
			original[y][x][2]=temp[2];
		}
	}

	for(int i=0;i<height_original;i++){
		for(int j=0;j<width_original;j++){
			for(int k=0;k<BytesPerPixel;k++){
				Imagedata[i][j][k]=original[i][j][k];
			}
		}
	}

	// Write image data (filename specified by second argument) from image data matrix

	//if (!(file=fopen(argv[2],"wb"))) {
	if (mode==0) {
		if (!(file=fopen("../result/P1/tartanOnField.raw","wb"))) {
			cout << "Cannot open file: " << "tartanOnField.raw" << endl;
			exit(1);
		}
	}
	else{
		if (!(file=fopen("../result/P1/trojanOnField.raw","wb"))) {
			cout << "Cannot open file: " << "tartanOnField.raw" << endl;
			exit(1);
		}
	}
	fwrite(Imagedata, sizeof(unsigned char), height_original*width_original*BytesPerPixel, file);
	fclose(file);

	return 0;
}


unsigned char BiInterpolation(unsigned char tl, unsigned char tr,unsigned char bl,unsigned char br,double a,double b){
	return (1-b)*(1-a)*tl+b*(1-a)*tr+a*(1-b)*bl+a*b*br;

}