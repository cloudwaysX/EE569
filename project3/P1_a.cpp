// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <vector>
#include <climits>

using namespace std;
using namespace cv;

double* computeTensor(const char input1[5],const char input2[5]);
vector<int> ClassifyD(float** data, vector<vector<double>> centerVec, int dataSize);
vector<int> Kmeans(float** data, vector<vector<double>> centerVec, int dataSize);

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


	/////////////////////////////////////////////////////////////////////////////////////////
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

	float** ave_featureVec = new float*[12];
	for(int i=0;i<12;i++){
		ave_featureVec[i]=new float[25];
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

	//calculate means and standard deviation
	cout<<"25D means: \n";
	vector<vector<double>> mean(2,vector<double>(25,0));
	vector<vector<double>> stdv(2,vector<double>(25,0));
	for(int i=0;i<25;i++){
		for(int j=0;j<12;j++){
			mean[0][i]+=ave_featureVec[j][i]/12.0;
		}
		cout<<mean[0][i]<<" ";
	}
	cout<<"\n"<<endl;
	//cout<<"25D stdv: \n";
	for(int i=0;i<25;i++){
		for(int j=0;j<12;j++){
			stdv[0][i]+=pow(ave_featureVec[j][i]-mean[0][i],2)/12.0;
		}
	}
	for(int i=0;i<25;i++){
		stdv[0][i]=sqrt(stdv[0][i]);
		//cout<<stdv[0][i]<<" ";
	}
	//cout<<endl;

	/*for(int i=0;i<12;i++){
		for(int j=0;j<25;j++){
			cout<<ave_featureVec[i][j]-mean[j]<<" ";
		}
		cout<<"\n"<<endl;
	}*/

	///////////////////////////////////////////////////////////////////////////////////
	//Calculate PCA

	//Mat texs = Mat(12,25,CV_32F,ave_featureVec);
	/*for(int i=0;i<12;i++){
		for(int j=0;j<25;j++){
			cout<<texs.at<float>(i,j)<<" ";
		}
		cout<<"\n"<<endl;
	}*/

	//PCA pca(texs,Mat(),PCA::DATA_AS_ROW,3);
	//Mat ave_featureVec_PCA = pca.project(texs);

	//visualize the reduced 3D 
	/*for(int i=0;i<12;i++){
		for(int j=0;j<3;j++){
			cout<<ave_featureVec_PCA.at<float>(i,j)<<" ";
		}
		cout<<"\n"<<endl;
	}*/


	////////////////////////////////////////////////////////////////////////////////////
	//K means

	//intialize the centroid for 25D
	vector<std::vector<double>> centroids;
	vector<double> centroid1,centroid2,centroid3,centroid4,centroid5,centroid6;
	for(int i=0;i<25;i++){
		centroid1.push_back(mean[0][i]-stdv[0][i]);
		centroid2.push_back(mean[0][i]+stdv[0][i]);
	}
	mean[0]=vector<double>(25,0);stdv[0]=vector<double>(25,0);
	centroids.push_back(centroid1);
	centroids.push_back(centroid2);
	double classNum[2]={0,0};
	vector<int> labels=ClassifyD(ave_featureVec, centroids, 12);
	cout<<"class 0's mean: ";
	for(int j=0;j<25;j++){
		for(int i=0;i<12;i++){
			mean[labels[i]][j]+=ave_featureVec[i][j];
			if(j==0){ classNum[labels[i]]+=1; }
		}
		cout<<mean[0][j]/classNum[0]<<" ";
	}
	cout<<endl;
	for(int j=0;j<25;j++){
		for(int i=0;i<12;i++){
			stdv[labels[i]][j]+=pow(ave_featureVec[i][j]-mean[labels[i]][j]/classNum[labels[i]],2)/classNum[labels[i]];
		}
	}
	cout<<"class 0's stdv: ";
	for(int i=0;i<25;i++){
		stdv[0][i]=sqrt(stdv[0][i]);
		stdv[1][i]=sqrt(stdv[1][i]);
		cout<<stdv[0][i]<<" ";
	}
	cout<<endl;
	cout<<"class 1's mean: ";
	for(int j=0;j<25;j++){
		cout<<mean[1][j]/classNum[1]<<" ";
	}
	cout<<endl;
	cout<<"class 1's stdv: ";
	for(int i=0;i<25;i++){
		cout<<stdv[1][i]<<" ";
	}
	cout<<endl;
	for(int i=0;i<25;i++){
		centroid3.push_back(mean[0][i]-stdv[0][i]);
		centroid4.push_back(mean[0][i]+stdv[0][i]);
		centroid5.push_back(mean[1][i]-stdv[1][i]);
		centroid6.push_back(mean[1][i]+stdv[1][i]);
	}
	centroids.clear();
	centroids.push_back(centroid3);
	centroids.push_back(centroid4);
	centroids.push_back(centroid5);
	centroids.push_back(centroid6);

	vector<int> result=Kmeans(ave_featureVec,centroids, 12);
	for(int i=0;i<result.size();i++){
		cout<<i+1<<" pic is labled as class "<<result[i]<<endl;
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

vector<int> ClassifyD(float** data, vector<vector<double>> centerVec, int dataSize){
	vector<int> labels;
	for(int i=0;i<dataSize;i++){
		double minSum=INT_MAX;
		int minClass=0;
		for(int classI=0;classI<centerVec.size();classI++){
			double sum=0;
			for(int j=0;j<centerVec[0].size();j++){
				sum+=pow(centerVec[classI][j]-data[i][j],2);
			} 
			if(minSum>sum){
				minSum=sum;
				minClass=classI;
			}
		}
		//cout<<i<<" pic is labled as class "<<maxClass<<endl;
		labels.push_back(minClass);
	}
	return labels;
}

vector<int> Kmeans(float** data, vector<vector<double>> centerVec, int dataSize){
	for(int k=0;k<10;k++){
		vector<int> labels=ClassifyD(data, centerVec, dataSize);
		vector<vector<double>> mean(4,vector<double>(25,0));
		double classNum[4]={0,0,0,0};
		for(int j=0;j<centerVec[0].size();j++){
			for(int i=0;i<dataSize;i++){
				mean[labels[i]][j]+=data[i][j];
				if(j==0){ classNum[labels[i]]+=1; }
			}
		}

		for(int i=0;i<centerVec.size();i++){
			for(int j=0;j<centerVec[0].size();j++){
				//cout<<i<<" "<<j<<endl;
				centerVec[i][j]=mean[i][j]/(classNum[i]+0.000001);
			}
		}
	}
	return ClassifyD(data, centerVec, dataSize);
		

}