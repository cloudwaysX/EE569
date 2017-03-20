// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

double* computeTensor(const char input1[5],const char input2[5]);

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=1;
	int Size = 128;
	
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name output_file mode[full dimension/PCA]" << endl;
		return 0;
	}

	char L5[]={1,4,6,4,1};
	char E5[]={-1,-2,0,2,1};
	char S5[]={-1,0,2,0,-1};
	char W5[]={-1,2,0,-2,1};
	char R5[]={1,-4,6,-4,1};

	
	// Allocate image data array
	unsigned char Imagedata[Size][Size][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	// And extracted certain features

	/*double*** textures = new double**[Size*Size];
	for(int i =0;i<Size*Size;i++){
		textures[i]=new double*[25];
	}
	for(int i =0;i<25;i++){
		for(int j=0;j<25;j++){
			textures[i][j]=new double[12];
		}
	}*/

	double** ave_featureVec = new double*[12];
	for(int i=0;i<12;i++){
		ave_featureVec[i]=new double[25];
	}

	char** Kernels1D = new char*[5];
	for(int i =0;i<5;i++){
		Kernels1D[i]=new char[5];
	}
	Kernels1D[0]=L5;
	Kernels1D[1]=E5;
	Kernels1D[2]=S5;
	Kernels1D[3]=W5;
	Kernels1D[4]=R5;


	double** filterBanks = new double*[25];
	for(int i =0;i<25;i++){
		filterBanks[i]=computeTensor(Kernels1D[i/5],Kernels1D[i%5]);
		//cout<<i<<endl;
		for(int i3=0;i3<25;i3++){
			//cout<<filterBanks[i][i3]<<" ";
		}
		//cout<<endl;
		//cout<<filterBanks[i][1]<<endl;
	}


	for (int i=0;i<12;i++){
		string temp ="./HW3_images/P1/Texture"+ to_string(i+1) +".raw";
		const char* filename=temp.c_str();
		if (!(file=fopen(filename,"rb"))) {
			cout << "Cannot open file: " << filename <<endl;
			exit(1);
		}
		fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
		fclose(file);


		//substract global mean
		double mean=0;
		for(int row=0;row<Size;row++){
			for(int col=0;col<Size;col++){
				mean=Imagedata[row][col][0];
			}
		}
		mean=mean/(double)(Size*Size);

		//extract the features
		for(int row=0;row<Size;row++){
			for(int col=0;col<Size;col++){
				//cout<<row<<" "<<col<<endl;
				for(int i2=-2;i2<=2;i2++){
					for(int j=-2;j<=2;j++){
						//cout<<i2<<" "<<j<<endl;
						int newR=row+i2; 
						int newC=col+j;
						int filterIndex=(i2+2)*5+j+2;

						//deal with boundaeies
						newR=max(-newR,newR);
						newR=min(newR,2*(Size-1)-newR);
						newC=max(-newC,newC);
						newC=min(newC,2*(Size-1)-newC);

						for(int n=0;n<25;n++){
								//textures[row*Size+col][n][i]+=Imagedata[newR][newC][0]*filterBanks[n][m]
								ave_featureVec[i][n]+=(double)Imagedata[newR][newC][0]*filterBanks[n][filterIndex]/(double)(Size*Size);
								//cout<<(double)Imagedata[newR][newC][0]*filterBanks[n][m]<<endl;
						}

					}
				}
			}
		}

	}

	//visualize 25D features
	double mean[25];
	for(int i=0;i<25;i++){
		double tmp=0;
		for(int j=0;j<12;j++){
			tmp+=ave_featureVec[j][i];
		}
		mean[i]=tmp/12.0;
	}

	for(int i=0;i<12;i++){
		for(int j=0;j<25;j++){
			cout<<ave_featureVec[i][j]-mean[j]<<" ";
		}
		cout<<"\n"<<endl;
	}



	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	// Write image data (filename specified by second argument) from image data matrix

	/*if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);*/

	return 0;
}

double* computeTensor(const char input1[5],const char input2[5]){
	double* result= new double[25];
	for(int i=0;i<25;i++){
		int r=i/5;
		int c=i%5;
		result[i]=input1[r]*input2[c];
		//cout<<result[i]<<" ";
	}
	//cout<<endl;
	return result;
}