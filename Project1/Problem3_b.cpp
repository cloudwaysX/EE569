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

double CalcEuclideanD(int*** img, int r_i, int c_i, int r_j, int c_j,int k,vector<double> gaussianFilter);
vector<double> GaussianFilter(int filterSize,double sigma);

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=3;

	// Check for proper syntax
	if (argc < 8){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw height width size h sigma" << endl;
		return 0;
	}
	
	int height = atoi(argv[3]);
	int width = atoi(argv[4]);
	int patchSize=atoi(argv[5]);
	double h=atof(argv[6]);
	double sigma=atof(argv[7]);

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
	////////////Declare a pointer and do the image padding in the pointer
	int Paddingnum=patchSize/2;
	int*** out1 =new int**[height+Paddingnum*2];
	for(int i =0;i<height+Paddingnum*2;i++){
		out1[i]=new int*[width+Paddingnum*2];
	}
	for(int i =0;i<height+Paddingnum*2;i++){
		for(int j=0;j<width+Paddingnum*2;j++){
			out1[i][j]=new int[BytesPerPixel];
		}
	}

	cout<<"creating GaussianFilter... with size"<<patchSize<<" and sigma"<<sigma<<endl;
	vector<double> gaussianFilter = GaussianFilter(patchSize,sigma);
	/*for(int i=0;i<gaussianFilter.size();i++){
		cout<<gaussianFilter.at(i)<<endl;
	}*/


	cout<<"begin padding..."<<endl;
	for(int r=0;r<height+Paddingnum*2;r++){
		int r2=max(0,r-Paddingnum);
		r2=min(height-1,r2);
		for(int c=0;c<width+Paddingnum*2;c++){
			int c2=max(0,c-Paddingnum);
			c2=min(width-1,c2);
			for(int k=0;k<BytesPerPixel;k++){
				out1[r][c][k]=Imagedata[r2][c2][k];
			}
		}
	}
	cout<<"finish padding"<<endl;


	for(int k=0;k<BytesPerPixel;k++){
	///////////////////////////////////////// Find all the patches at once
		int defaultR=Paddingnum;
		int defaultC=Paddingnum;
		//cout<<"height: "<<height<<endl;
		for(int r=0;r<height;r++){
			//cout<<"a: "<<r_i<<endl;
			for(int c=0;c<width;c++){
				//cout<<"a: "<<r_i<<" "<<c_i<<endl;
				int r_i=r+defaultR;
				int c_i=c+defaultC;
				//only look for surrounding 11*11 patch
				double weight_sum=0;
				double neighborSum=0;
				for(int r2=-5;r2<=5;r2++){
					for(int c2=-5;c2<=5;c2++){
						int r_j=r_i+r2; int c_j=c_i+c2;
						//cout<<r_j<<" "<<c_j<<endl;
						//cout<<r_i<<" "<<c_i<<endl;
						if(r_j<Paddingnum || r_j>height+Paddingnum-1) continue;
						if(c_j<Paddingnum || c_j>width+Paddingnum-1) continue;
						double D=CalcEuclideanD(out1, r_i, c_i, r_j, c_j,k,gaussianFilter);
						double weight=exp(-D/pow(h,2));
						neighborSum+=weight*out1[r_j][c_j][k];
						weight_sum+=weight;
					}
				}
				Imagedata[r_i-defaultR][c_i-defaultC][k]=neighborSum/weight_sum;
			}
			cout<<"For channel: "<<k<<" finish denoise row: "<<r<<endl;
		}
	}


	// Write image data (filename specified by second argument) from image data matrix

	//delete pointers
	for(int i =0;i<height;i++){
		for(int j=0;j<width;j++){
			delete out1[i][j];
		}
	}
	for(int i =0;i<height;i++){
		delete out1[i];
	}
	delete out1;


	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), width*height*BytesPerPixel, file);
	fclose(file);

	return 0;
}

vector<double> GaussianFilter(int filterSize,double sigma){

	double sum=0;
	double weight_sum=0;
	vector<double> weight;
	int center=filterSize/2; //both for row and column
	for (int r=0;r<filterSize;r++){
		for (int c=0;c<filterSize;c++){
			double temp=exp(-(pow(r-center,2)+pow(c-center,2))/(2*pow((double)sigma,2)));
			temp=temp/(2*M_PI*pow((double)sigma,2));
			weight_sum+=temp;
			weight.push_back(temp);
		}
	}
	
	for(int i=0;i<weight.size();i++){
		weight.at(i)=weight.at(i)/weight_sum;
	}
	return weight;
}

double CalcEuclideanD(int*** img, int r_i, int c_i, int r_j, int c_j,int k,vector<double> gaussianFilter){
	double D=0;
	int filterSize = sqrt(gaussianFilter.size());
	for(int i=-filterSize/2;i<=filterSize/2;i++){
		for(int j=-filterSize/2;j<=filterSize/2;j++){
			int index=(i+filterSize/2)*filterSize+(j+filterSize/2);
			//cout<<index<<endl;
			double temp=img[r_i+i][c_i+j][k]-img[r_j+i][c_j+j][k];
			temp=pow(temp,2)*gaussianFilter.at(index);
			D+=temp;
		}
	}
	return D;
}