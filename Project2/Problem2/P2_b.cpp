// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>

using namespace std;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=1;
	int Size = 512;
	
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name DiffusionType output.raw" << endl;
		return 0;
	}

	int DiffusionM_size;
	if(atoi(argv[1])==0){
		DiffusionM_size=3;
	}
	else{
		DiffusionM_size=5;
	}

	double** DiffusionM=new double*[DiffusionM_size];

	if(atoi(argv[1])==0){
		cout<<"SF"<<endl;
		double DiffusionM_temp[DiffusionM_size][DiffusionM_size]={
			{0,0,0},
			{0,0,7/16.0},
			{3/16.0,5/16.0,1/16.0},
		};

	
		for(int i =0;i<DiffusionM_size;i++){
			DiffusionM[i]=new double[DiffusionM_size];
		}
		for(int i =0;i<DiffusionM_size;i++){
			for(int j=0;j<DiffusionM_size;j++){
				DiffusionM[i][j]=DiffusionM_temp[i][j];
				cout<<DiffusionM[i][j]<<" ";
			}
			cout<<endl;
		}
	}
	else if(atoi(argv[1])==1){
		cout<<"JJN"<<endl;
		double DiffusionM_temp[DiffusionM_size][DiffusionM_size]={
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,0,7/48.0,5/48.0},
			{3/48.0,5/48.0,7/48.0,5/48.0,3/48.0},
			{1/48.0,3/48.0,5/48.0,3/48.0,1/48.0}
		};

	
		for(int i =0;i<DiffusionM_size;i++){
			DiffusionM[i]=new double[DiffusionM_size];
		}
		for(int i =0;i<DiffusionM_size;i++){
			for(int j=0;j<DiffusionM_size;j++){
				DiffusionM[i][j]=DiffusionM_temp[i][j];
				cout<<DiffusionM[i][j]<<" ";
			}
			cout<<endl;
		}
	}
	else if(atoi(argv[1])==2){
		cout<<"Stuki"<<endl;
		double DiffusionM_temp[DiffusionM_size][DiffusionM_size]={
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,0,8/42.0,4/42.0},
			{2/42.0,4/42.0,8/42.0,4/42.0,2/42.0},
			{1/42.0,2/42.0,4/42.0,2/42.0,1/42.0}
		};

	
		for(int i =0;i<DiffusionM_size;i++){
			DiffusionM[i]=new double[DiffusionM_size];
		}
		for(int i =0;i<DiffusionM_size;i++){
			for(int j=0;j<DiffusionM_size;j++){
				DiffusionM[i][j]=DiffusionM_temp[i][j];
				cout<<DiffusionM[i][j]<<" ";
			}
			cout<<endl;
		}
	}

	// Allocate image data array
	unsigned char Imagedata[Size][Size][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen("../images/P2/man.raw","rb"))) {
		cout << "Cannot open file: " <<"man.raw" <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	double*** original=new double**[Size];
	for(int i =0;i<Size;i++){
		original[i]=new double*[Size];
	}
	for(int i =0;i<Size;i++){
		for(int j=0;j<Size;j++){
			original[i][j]=new double[BytesPerPixel];
		}
	}
	for(int i =0;i<Size;i++){
		for(int j=0;j<Size;j++){
			for(int k=0;k<BytesPerPixel;k++){
				original[i][j][k]=Imagedata[i][j][k]/(double)255;
			}
		}
	}


	for(int r=0;r<Size;r++){
		if(r%2==0){
			for(int c=0;c<Size;c++){
				double curVal=original[r][c][0];
				double error=curVal-(int)(curVal+0.5);
				original[r][c][0]=(int)(curVal+0.5);
				for(int i=-DiffusionM_size/2;i<=DiffusionM_size/2;i++){
					for(int j=-DiffusionM_size/2;j<=DiffusionM_size/2;j++){
						if(r+i<0||r+i>=Size||c+j<0||c+j>=Size) continue;
						original[r+i][c+j][0]+=error*DiffusionM[i+DiffusionM_size/2][j+DiffusionM_size/2];
					}
				}
			}
		}
		else{
			for(int c=Size-1;c>=0;c--){
				double curVal=original[r][c][0];
				double error=curVal-(int)(curVal+0.5);
				original[r][c][0]=(int)(curVal+0.5);
				for(int i=-DiffusionM_size/2;i<=DiffusionM_size/2;i++){
					for(int j=-DiffusionM_size/2;j<=DiffusionM_size/2;j++){
						if(r+i<0||r+i>=Size||c+j<0||c+j>=Size) continue;
						original[r+i][c+j][0]+=error*DiffusionM[i+DiffusionM_size/2][-j+DiffusionM_size/2];
					}
				}
			}
		}
	}

	for(int r=0;r<Size;r++){
		for(int c=0;c<Size;c++){
			Imagedata[r][c][0]=(unsigned char)original[r][c][0]*255;
		}
	}



	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);
	
	return 0;
}
