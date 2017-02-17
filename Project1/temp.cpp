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

double CalcEuclideanD(vector<unsigned char> Ni, vector<unsigned char> Nj, double h,double sigma);
double GaussianFilter(vector<unsigned char> Patch,double sigma);

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
	vector<double> gaussianFilter = GaussianFilter(patchSize) 

	cout<<"begin padding..."<<endl;
	for(int r=0;r<height+Paddingnum*2;r++){
		int r2=max(0,r-Paddingnum);
		r2=min(height-1,r2);
		for(int c=0;c<width+Paddingnum*2;c++){
			int c2=max(0,c-Paddingnum);
			c2=min(width-1,c2);
			//cout<<r2<< " "<<c2<<endl;
			//cout<<r<< " "<<c<<endl;
			for(int k=0;k<BytesPerPixel;k++){
				//out<<(int)Imagedata[r2][c2][k]<<endl;
				out1[r][c][k]=Imagedata[r2][c2][k];
			}
		}
	}

	cout<<"finish padding"<<endl;


	for(int k=0;k<BytesPerPixel;k++){
	///////////////////////////////////////// Find all the patches at once
		vector< vector<unsigned char> > patchBank;
		int defaultR=Paddingnum;
		int defaultC=Paddingnum;
		for(int r=0;r<height;r++){
			for(int c=0;c<width;c++){
				vector<unsigned char> curPatch;
				//cout<<"r: "<<r<<"c: "<<c<<endl;
				for(int i=-Paddingnum;i<=Paddingnum;i++){
					for(int j=-Paddingnum;j<=Paddingnum;j++){
						curPatch.push_back(out1[r+defaultR+i][c+defaultC+j][k]);
						//cout<<(int)out1[r+defaultR+i][c+defaultC+j][k]<<endl;
					}
				}
				//cout<<"c"<<endl;
				patchBank.push_back(curPatch);
				//cout<<"c"<<endl;
			}
		}

		cout<<"For channel: "<<k<<" finish finding all patches" <<endl;
	//Do the NL-means algorithm
		for(int r=0;r<height;r++){
			for(int c=0;c<width;c++){
				int index_i=r*width+c;
				double normalizeTerm=0;
				double result=0;
				//vector<unsigned char> test=patchBank.at(index_i);
				//cout<<"center: "<<r<<" "<<c<<endl;
				/*for(int m=0;m<test.size();m++){
					cout<<(int)test.at(m)<<endl;
				}*/

				//only look for surrounding 15*15 patch
				for(int i=-5;i<=5;i++){
					for(int j=-5;j<=5;j++){
						if(i==0 && j==0) continue;
						int cur_r=r+i;
						int cur_c=c+j;
						if(cur_r<0 || cur_r>height-1) continue;
						if(cur_c<0 || cur_c>width-1) continue;
						int index_j=cur_r*width+cur_c;
						//cout<<"center: "<<r<<" "<<c<<endl;
						//cout<<"r: "<<cur_r<<"c: "<<cur_c<<endl;
						//double weight=CalcEuclideanD(patchBank.at(index_i),patchBank.at(index_j),h,sigma);
						//result+=weight*Imagedata[cur_r][cur_c][k];
						//cout<<weight<<endl;
						//cout<<"weight"<<weight<<"img"<<(int)Imagedata[cur_r][cur_c][k]<<endl;
						//normalizeTerm+=weight;
					}
				}
				//cout<<(int)Imagedata[r][c][k]<<endl;
				Imagedata[r][c][k]=result/normalizeTerm;
				//cout<<(int)Imagedata[r][c][k]<<endl;
				//cout<<"For channel: "<<k<<" finish denoise row: "<<r<<"col: "<<c<<endl;
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

double CalcEuclideanD(vector<unsigned char> Ni, vector<unsigned char> Nj, double h, double sigma){
	vector<unsigned char> diff;
	for(int i=0;i<Ni.size();i++){
		diff.push_back(Ni.at(i)-Nj.at(i));
		cout<<"N: "<<Ni.at(i)-Nj.at(i)<<endl;
	}
	double D=GaussianFilter(diff,sigma);
	cout<<D<<endl;
	return exp(-D/pow(h,2));
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