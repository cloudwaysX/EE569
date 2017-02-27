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
	if (argc < 4){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name BayerMatrix_Size output.raw grayScale_level" << endl;
		return 0;
	}

	int BayerMatrix_Size=pow(2,atoi(argv[1]));
	if(atoi(argv[1])==0){//use the given matrix
		BayerMatrix_Size=4;
	}

	int grayScale_level=atoi(argv[3]);

	double** BayerM=new double*[BayerMatrix_Size];
	for(int i =0;i<BayerMatrix_Size;i++){
		BayerM[i]=new double[BayerMatrix_Size];
	}

	if(atoi(argv[1])!=0){
		BayerM[0][0]=0;
		for(int level=1;level<=atoi(argv[1]);level++){
			for(int r=0;r<pow(2,level);r++){
				for(int c=0;c<pow(2,level);c++){
					int source_r=r% (int)pow(2,level-1);
					int source_c=c% (int)pow(2,level-1);
					int add_on=0;
					int multiply_parameter=1;

					if(r/(int)pow(2,level-1)==0&&c/(int)pow(2,level-1)==0){
						multiply_parameter=4;
						add_on=0;
					}
					else if(r/(int)pow(2,level-1)==0){
						add_on=2;
					}
					else if(c/(int)pow(2,level-1)==0){
						add_on=3;
					}
					else{
						add_on=1;
					}
				
					BayerM[r][c]=(multiply_parameter*BayerM[source_r][source_c]+add_on);
				}
			}
		}
	}
	else{
		int A4_Matrix[4][4]={
			{14,10,11,15},
			{9,3,0,4},
			{8,2,1,5},
			{13,7,6,12}
		};
		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++){
				BayerM[i][j]=A4_Matrix[i][j];
			}
		}
		BayerMatrix_Size=4;
		cout<<BayerMatrix_Size<<endl;

	}

	for(int r=0;r<BayerMatrix_Size;r++){
		for(int c=0;c<BayerMatrix_Size;c++){
			cout<<BayerM[r][c]<<" ";
			if(grayScale_level==2){
				BayerM[r][c]=(BayerM[r][c]+0.5)/pow(BayerMatrix_Size,2)*255;
			}
			else{
				BayerM[r][c]=(BayerM[r][c]+0.5)/pow(BayerMatrix_Size,2)*85;
			}
		}
		cout<<endl;
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
	if(grayScale_level==2){
		for(int r=0;r<Size;r++){
			for(int c=0;c<Size;c++){
				double T=BayerM[r%BayerMatrix_Size][c%BayerMatrix_Size];
				if(255-(double)Imagedata[r][c][0]>T){
					Imagedata[r][c][0]=(unsigned char)0;
				}
				else{
					Imagedata[r][c][0]=(unsigned char)255;
				}
			}
		}
	}
	else{
		for(int r=0;r<Size;r++){
			for(int c=0;c<Size;c++){
				double T_255=BayerM[r%BayerMatrix_Size][c%BayerMatrix_Size]+170;
				double T_170=BayerM[r%BayerMatrix_Size][c%BayerMatrix_Size]+85;
				double T_85=BayerM[r%BayerMatrix_Size][c%BayerMatrix_Size];
				if(255-(double)Imagedata[r][c][0]>T_255){
					Imagedata[r][c][0]=(unsigned char)0;
				}
				else if(255-(double)Imagedata[r][c][0]>T_170){
					Imagedata[r][c][0]=(unsigned char)85;
				}
				else if(255-(double)Imagedata[r][c][0]>T_85){
					Imagedata[r][c][0]=(unsigned char)170;
				}
				else{
					Imagedata[r][c][0]=(unsigned char)255;
				}
			}
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
