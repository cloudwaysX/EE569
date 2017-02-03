// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cmath>

using namespace std;

#define MIN 0.0000001


int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=3;

	// Check for proper syntax
	if (argc < 5){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw height width" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	int height = atoi(argv[3]);
	int width = atoi(argv[4]);
	double mean=70;
	double stdv=20;
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
	///////////Declare a pointer and do the image padding in the pointer

	//calcualte the histogram
	for(int k=0;k<BytesPerPixel;k++){
		vector<unsigned int> hist(256,0);
		vector<double> culhis(256,0);

		for(int r=0;r<height;r++){
			for(int c=0;c<width;c++){
				hist.at(Imagedata[r][c][k])+=1;
			}
		}

		culhis.at(0)=hist.at(0)/(double)(height*width);
		for(int i=1;i<256;i++){
			culhis.at(i)=culhis.at(i-1)+hist.at(i)/(double)(height*width);
			//cout<<culhis.at(i)<<endl;
		}

		///////////////////////////////////////////////////////////////////
		vector<double> transformedHist(256,0);
		double transformedSum=0;
		for(int i=0;i<256;i++){
			double temp=-pow(i-mean,2)/(2*pow(stdv,2));
			temp=exp(temp);
			temp=temp/sqrt(2*M_PI*stdv*stdv);
			transformedHist.at(i)=temp;
			transformedSum+=temp;
		}
		vector<double> culhis_trans(256,0);
		culhis_trans.at(0)=transformedHist.at(0)/transformedSum;
		for(int i=1;i<256;i++){
			culhis_trans.at(i)=culhis_trans.at(i-1)+transformedHist.at(i)/transformedSum;
			//cout<<culhis_trans.at(i)<<endl;
		}
		////////////////////////////////////////////
		vector<unsigned char> correspondingTensity(256,0);
		int begin=0;
		for(int i=0;i<256;i++){
			double target_cdf=culhis.at(i);
			cout<<"target_cdf: "<<target_cdf<<endl;
			for(int j=begin;j<256;j++){
				cout<<"culhis_trans: "<<culhis_trans.at(j)<<endl;
				cout<<i<<endl;
				if(culhis_trans.at(j)>=target_cdf-MIN){
					correspondingTensity.at(i)=j;
					//cout<<"i"<<i<<"j"<<j<<endl;
					begin=j;
					break;
				}
			}
		}

		//////////////////////////////////////
		for(int r=0;r<height;r++){
			for(int c=0;c<width;c++){	
				Imagedata[r][c][k]=(unsigned char)(correspondingTensity.at((int)Imagedata[r][c][k]));
				//cout<<(int)Imagedata[r][c][k]<<endl;
			}
		}


	}




	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);


	return 0;
}

