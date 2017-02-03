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

unsigned char MedianFilter(vector<unsigned char> Patch);
unsigned char GaussianFilter(vector<unsigned char> Patch,double sigma);

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=3;

	// Check for proper syntax
	if (argc < 7){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw height width size sigma" << endl;
		return 0;
	}
	
	int height = atoi(argv[3]);
	int width = atoi(argv[4]);
	int patchSize=atoi(argv[5]);
	int sigma=atoi(argv[6]);

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
	int*** out1 =new int**[height];
	int*** out2 =new int**[height];
	for(int i =0;i<height;i++){
		out1[i]=new int*[width];
		out2[i]=new int*[width];
	}
	for(int i =0;i<height;i++){
		for(int j=0;j<width;j++){
			out1[i][j]=new int[BytesPerPixel];
			out2[i][j]=new int[BytesPerPixel];
		}
	}


	/////////////////////////////////////////
	for(int k=0;k<BytesPerPixel;k++){
		for(int r=patchSize/2;r<height-patchSize/2;r++){
			for(int c=patchSize/2;c<width-patchSize/2;c++){

				vector<unsigned char> curPatch;
				for(int i=-patchSize/2;i<=patchSize/2;i++){
					for(int j=-patchSize/2;j<=patchSize/2;j++){
						curPatch.push_back(Imagedata[r+i][c+j][k]);
					}
				}
				out1[r][c][k]=MedianFilter(curPatch);
			}
		}
	}

	for(int k=0;k<BytesPerPixel;k++){
		for(int r=patchSize/2;r<height-patchSize/2;r++){
			for(int c=patchSize/2;c<width-patchSize/2;c++){

				vector<unsigned char> curPatch;
				for(int i=-patchSize/2;i<=patchSize/2;i++){
					for(int j=-patchSize/2;j<=patchSize/2;j++){
						curPatch.push_back(out1[r+i][c+j][k]);
					}
				}
				out2[r][c][k]=GaussianFilter(curPatch,sigma);
			}
			//cout<<r<<endl;
		}
	}

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
			delete out1[i][j];
			delete out2[i][j];
		}
	}
	for(int i =0;i<height;i++){
		delete out1[i];
		delete out2[i];
	}
	delete out1;
	delete out2;


	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);

	return 0;
}

unsigned char MedianFilter(vector<unsigned char> Patch){
	sort(Patch.begin(),Patch.end());
	return Patch.at(Patch.size()/2);
}

unsigned char GaussianFilter(vector<unsigned char> Patch,double sigma){
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