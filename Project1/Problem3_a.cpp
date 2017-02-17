// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>    
#include <vector>      
#include <cmath>


using namespace std;

unsigned char MedianFilter_Pixel(vector<unsigned char> Patch);
unsigned char GaussianFilter_Pixel(vector<unsigned char> Patch,double sigma);
int*** MedianFilter(int*** in,int BytesPerPixel,int patchSize,int height,int width);
int*** GaussianFilter(int*** in,int BytesPerPixel,int patchSize,int height,int width,double sigma);

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=3;

	// Check for proper syntax
	if (argc < 8){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw height width size sigma mode" << endl;
		return 0;
	}
	
	cout<<"mode 1: median only; mode 2: gaussian only; mode 3:gaussian+median 4:median+gaussian"<<endl;

	int height = atoi(argv[3]);
	int width = atoi(argv[4]);
	int patchSize=atoi(argv[5]);
	double sigma=atof(argv[6]);
	int mode=atoi(argv[7]);

	cout<<"now are running mode: "<<mode<<endl;

	// Allocate image data array
	unsigned char Imagedata[height][width][BytesPerPixel];
	unsigned char out[height][width][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	////////////Declare a pointer
	int*** out1;
	int*** out2;
	int*** original=new int**[height];
	for(int i =0;i<height;i++){
		original[i]=new int*[width];
	}
	for(int i =0;i<height;i++){
		for(int j=0;j<width;j++){
			original[i][j]=new int[BytesPerPixel];
		}
	}

	//copy the original data into pointer in order to pass into the function
	for(int i =0;i<height;i++){
		for(int j=0;j<width;j++){
			for(int k=0;k<BytesPerPixel;k++){
				original[i][j][k]=Imagedata[i][j][k];
			}
		}
	}

	if(mode==1){
		out2=MedianFilter(original,BytesPerPixel,patchSize,height,width);
	}
	else if(mode==2){
		out2=GaussianFilter(original,BytesPerPixel,patchSize,height,width,sigma);
	}
	else if(mode==3){
		out1=GaussianFilter(original,BytesPerPixel,patchSize,height,width,sigma);
		out2=MedianFilter(out1,BytesPerPixel,patchSize,height,width);
	}
	else{
		out1=MedianFilter(original,BytesPerPixel,patchSize,height,width);
		out2=GaussianFilter(out1,BytesPerPixel,patchSize,height,width,sigma);
	}
	/////////////////////////////////////////

	// Write image data (filename specified by second argument) from image data matrix

	//put the image data back in buff
	for(int i =0;i<height;i++){
		for(int j=0;j<width;j++){
			for(int k=0;k<BytesPerPixel;k++){
				Imagedata[i][j][k]=out2[i][j][k];
			}
		}
	}

	//delete pointers
	for(int i =0;i<height;i++){
		for(int j=0;j<width;j++){
			if(mode==3 ||mode==4) delete out1[i][j];
			delete out2[i][j];
			delete original[i][j];
		}
	}
	for(int i =0;i<height;i++){
		if(mode==3 ||mode==4) delete out1[i];
		delete out2[i];
		delete original[i];
	}
	if(mode==3 ||mode==4) delete out1;
	delete out2;
	delete original;


	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);

	return 0;
}

unsigned char MedianFilter_Pixel(vector<unsigned char> Patch){
	sort(Patch.begin(),Patch.end());
	return Patch.at(Patch.size()/2);
}

unsigned char GaussianFilter_Pixel(vector<unsigned char> Patch,double sigma){
	//cout<<sigma<<endl;
	double sum=0;
	double weight_sum=0;
	int filterSize=sqrt(Patch.size());
	double weight[Patch.size()];
	int center=filterSize/2; //both for row and column
	for (int r=0;r<filterSize;r++){
		for (int c=0;c<filterSize;c++){
			int index=r*filterSize+c;
			weight[index]=exp(-(pow(r-center,2)+pow(c-center,2))/(2*pow((double)sigma,2)));
			weight_sum+=weight[index];
		}
	}
	for (int i=0;i<Patch.size();i++){
		sum+=weight[i]*Patch.at(i);
	}
	sum = sum/weight_sum;
	return sum;
}

int*** MedianFilter(int*** in,int BytesPerPixel,int patchSize,int height,int width){
	int*** out =new int**[height];
	for(int i =0;i<height;i++){
		out[i]=new int*[width];
	}
	for(int i =0;i<height;i++){
		for(int j=0;j<width;j++){
			out[i][j]=new int[BytesPerPixel];
		}
	}
	cout<<height<<endl;
	cout<<width<<endl;
	//out[510][510][0]=1;
	for(int k=0;k<BytesPerPixel;k++){
		for(int r=0;r<height;r++){
			for(int c=0;c<width;c++){
				vector<unsigned char> curPatch;
				for(int i=-patchSize/2;i<=patchSize/2;i++){
					for(int j=-patchSize/2;j<=patchSize/2;j++){
						int r2=max(0,r+i); r2=min(r2,height-1);
						int c2=max(0,c+j); c2=min(c2,width-1);
						curPatch.push_back(in[r2][c2][k]);
					}
				}
				int tmp=MedianFilter_Pixel(curPatch);
				out[r][c][k]=tmp;
			}
		}
	}

	return out;
}

int*** GaussianFilter(int*** in,int BytesPerPixel,int patchSize,int height,int width,double sigma){
	int*** out =new int**[height];
	for(int i =0;i<height;i++){
		out[i]=new int*[width];
	}
	for(int i =0;i<height;i++){
		for(int j=0;j<width;j++){
			out[i][j]=new int[BytesPerPixel];
		}
	}
	for(int k=0;k<BytesPerPixel;k++){
		for(int r=0;r<height;r++){
			for(int c=0;c<width;c++){
				vector<unsigned char> curPatch;
				for(int i=-patchSize/2;i<=patchSize/2;i++){
					for(int j=-patchSize/2;j<=patchSize/2;j++){
						int r2=max(0,r+i); r2=min(r2,height-1);
						int c2=max(0,c+j); c2=min(c2,width-1);
						curPatch.push_back(in[r2][c2][k]);
					}
				}
				int tmp=GaussianFilter_Pixel(curPatch,sigma);
				out[r][c][k]=tmp;
			}
		}
	}

	return out;
}