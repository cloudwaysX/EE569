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
#include <set>
#include <utility>  

using namespace std;
using namespace cv;

double* computeTensor(const char input1[5],const char input2[5]);
vector<int> ClassifyD(double*** data, vector<vector<double>> centerVec, int dataSizeH, int dataSizeW);
vector<int> Kmeans(double*** data, vector<vector<double>> centerVec, int dataSizeH, int dataSizeW);
int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=3;
	int SizeW = 590;
	int SizeH =350;
	
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name output_file clusterNum" << endl;
		return 0;
	}

	char L5[]={1,4,6,4,1};
	char E5[]={-1,-2,0,2,1};
	char S5[]={-1,0,2,0,-1};
	char W5[]={-1,2,0,-2,1};
	char R5[]={1,-4,6,-4,1};

	
	// Allocate image data array
	unsigned char Imagedata[SizeH][SizeW][BytesPerPixel];


	/////////////////////////////////////////////////////////////////////////////////////////
	// Read image (filename specified by first argument) into image data matrix
	// And extracted certain features

	double*** textures = new double**[SizeH];
	for(int i =0;i<SizeH;i++){
		textures[i]=new double*[SizeW];
	}
	for(int i =0;i<SizeH;i++){
		for(int j=0;j<SizeW;j++){
			textures[i][j]=new double[25];
		}
	}

	double*** textures_window = new double**[SizeH];
	for(int i =0;i<SizeH;i++){
		textures_window[i]=new double*[SizeW];
	}
	for(int i =0;i<SizeH;i++){
		for(int j=0;j<SizeW;j++){
			textures_window[i][j]=new double[25];
			for(int n=0;n<25;n++){
				textures_window[i][j][n]=0;
			}
		}
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
		for(int i3=0;i3<25;i3++){
			//cout<<filterBanks[i][i3]<<" ";
		}
		//cout<<endl;
	}


	if (!(file=fopen("./HW3_images/P1/kitten.raw","rb"))) {
		cout << "Cannot open file: " << "./HW3_images/P1/kitten.raw" <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), SizeH*SizeW*BytesPerPixel, file);
	fclose(file);


	//substract global mean
	double mean1=0;
	for(int row=0;row<SizeH;row++){
		for(int col=0;col<SizeW;col++){
			mean1+=Imagedata[row][col][0]*0.2989+Imagedata[row][col][1]*0.5870+Imagedata[row][col][2]*0.1140;
		}
	}
	mean1=mean1/(double)(SizeH*SizeW);

	//extract the 25D features
	for(int row=0;row<SizeH;row++){
		for(int col=0;col<SizeW;col++){
			for(int n=0;n<25;n++){
				double tmp=0;
				for(int i2=-2;i2<=2;i2++){
					for(int j=-2;j<=2;j++){
						int newR=row+i2; 
						int newC=col+j;
						int filterIndex=(i2+2)*5+j+2;

						//deal with boundaeies
						newR=max(-newR,newR);
						newR=min(newR,2*(SizeH-1)-newR);
						newC=max(-newC,newC);
						newC=min(newC,2*(SizeW-1)-newC);
								
						double gray=(double)Imagedata[newR][newC][0]*0.2989+(double)Imagedata[newR][newC][1]*0.5870+(double)Imagedata[newR][newC][2]*0.1140;
						tmp+=(gray-mean1)*filterBanks[n][filterIndex];

					}
				}
				textures[row][col][n]=tmp;
			}
		}
	}

	for(int r=0;r<SizeH;r++){
		for(int c=0;c<SizeW;c++){
			for(int n=0;n<25;n++){
				for(int i=-6;i<=6;i++){
					for(int j=-6;j<=6;j++){
						int newR=r+i; 
						int newC=c+j;
						//deal with boundaeies
						newR=max(-newR,newR);
						newR=min(newR,2*(SizeH-1)-newR);
						newC=max(-newC,newC);
						newC=min(newC,2*(SizeW-1)-newC);

						textures_window[r][c][n]+=pow(textures[newR][newC][n],2)/(13.0*13.0);
					}

				}
				if(n!=0){
					textures_window[r][c][n]=textures_window[r][c][n]/textures_window[r][c][0];
				}
			}
			textures_window[r][c][0]=1;
			//cout<<r<<"  "<<c<<" "<<textures_window[r][c][1]<<endl;
		}
	}

	int featureNum=25;
	int clusterNum=atoi(argv[2]);

	//calculate means and standard deviation
	vector<vector<double>> mean(2,vector<double>(featureNum,0));
	vector<vector<double>> stdv(2,vector<double>(featureNum,0));
	cout<<"mean: "<<endl;
	for(int n=0;n<featureNum;n++){
		for(int r=0;r<SizeH;r++){
			for(int c=0;c<SizeW;c++){
				mean[0][n]+=textures_window[r][c][n]/(double)(SizeH*SizeW);
			}
		}
		cout<<mean[0][n]<<" ";
	}
	cout<<endl;

	cout<<"stdv: "<<endl;
	cout<<stdv[0][1]<<" ";
	for(int n=1;n<featureNum;n++){
		for(int r=0;r<SizeH;r++){
			for(int c=0;c<SizeW;c++){
				stdv[0][n]+=pow(textures_window[r][c][n]-mean[0][n],2)/(double)(SizeH*SizeW);
			}
		}
	}
	for(int n=0;n<featureNum;n++){
		stdv[0][n]=sqrt(stdv[0][n]);
		cout<<stdv[0][n]<<" ";
	}
	cout<<endl;

	
	//intialize the centroid 
	vector<std::vector<double>> centroids;
	vector<double> theCentroid0;
	for(int n=0;n<featureNum;n++){
		theCentroid0.push_back(mean[0][n]);
	}
	centroids.push_back(theCentroid0);
	set<pair<int,int>> visited;
	for(int k=1;k<clusterNum+1;k++){
		double maxD=0;
		int maxD_I[]={0,0};
		for(int i=0;i<SizeH;i++){
			for(int j=0;j<SizeW;j++){
				/*double minD = DBL_MAX;
				for(int m=0;m<centroids.size();m++){
					double D=0;
					for(int n=0;n<featureNum;n++){
						D+=pow(textures_window[i][j][n]-centroids[m][n],2);
					}
					if(D<minD && D!=0){
						minD=D;
						//cout<<i<<" "<<j<<" now is: "<<m<<"D is "<<D<<endl;
					}
				}*/
				double D=0;
				for(int n=0;n<featureNum;n++){
					D+=pow(textures_window[i][j][n]-centroids[k-1][n],2);
				}
				if(D>maxD && visited.find(make_pair(i,j))==visited.end()){
					maxD=D;
					maxD_I[0]=i;maxD_I[1]=j;
					visited.emplace(make_pair(i,j));
				}
				/*if(minD>maxD){
					maxD=minD;
					maxD_I[0]=i;maxD_I[1]=j;
				}*/
			}
		}
		cout<<maxD_I[0]<<" "<<maxD_I[1]<<endl;
		vector<double> theCentroid;
		for(int n=0;n<featureNum;n++){
			theCentroid.push_back(textures_window[maxD_I[0]][maxD_I[1]][n]);
		}
		centroids.push_back(theCentroid);
	}
	centroids.erase(centroids.begin());


	//cout<<"centroids are: "<<endl;
	vector<int> labels=ClassifyD(textures_window, centroids,SizeH,SizeW);
	cout<<centroids.size()<<endl;
	double classNum[]={0,0,0,0,0,0};
	for(int i=0;i<labels.size();i++){
		//int r=i/SizeW; int c=i%SizeW;
		//cout<<r<<","<<c<<" pixels is labled as class "<<labels[i]<<endl;
		classNum[labels[i]]+=1;
	}
	cout<<"class 0 has "<<classNum[0]<<endl;
	cout<<"class 1 has "<<classNum[1]<<endl;
	cout<<"class 2 has "<<classNum[2]<<endl;
	cout<<"class 3 has "<<classNum[3]<<endl;
	cout<<"class 4 has "<<classNum[4]<<endl;
	cout<<"class 5 has "<<classNum[5]<<endl;




	labels=Kmeans(textures_window,centroids, SizeH,SizeW); 
		
	// Write image data (filename specified by second argument) from image data matrix
	for(int i=0;i<labels.size();i++){
		double color=labels[i]/(double)clusterNum*255.0;
		//double color= (double)Imagedata[i/SizeW][i%SizeW][0]*0.2989+(double)Imagedata[i/SizeW][i%SizeW][1]*0.5870+(double)Imagedata[i/SizeW][i%SizeW][2]*0.1140;
		Imagedata[i/SizeW][i%SizeW][0]=(unsigned char)color;
		Imagedata[i/SizeW][i%SizeW][1]=(unsigned char)color;
		Imagedata[i/SizeW][i%SizeW][2]=(unsigned char)color;


	}
	if (!(file=fopen(argv[1],"wb"))) {
		cout << "Cannot open file: " << argv[1] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), SizeH*SizeW*BytesPerPixel, file);
	fclose(file);


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

vector<int> ClassifyD(double*** data, vector<vector<double>> centerVec, int dataSizeH, int dataSizeW){
	vector<int> labels;
	for(int r=0;r<dataSizeH;r++){
		for(int c=0;c<dataSizeW;c++){
			double minSum=FLT_MAX;
			int minClass=0;
			for(int classI=0;classI<centerVec.size();classI++){
				double sum=0;
				for(int n=0;n<centerVec[0].size();n++){
					sum+=pow(centerVec[classI][n]-data[r][c][n],2);
				}
				if(minSum>sum){
					minSum=sum;
					minClass=classI;
					//cout<<classI<<endl;
				}
			} 
			labels.push_back(minClass);
		}
	}
	return labels;
}

vector<int> Kmeans(double*** data, vector<vector<double>> centerVec, int dataSizeH, int dataSizeW){
	cout<<"0 iteration new center is: \n";
	for(int i=0;i<centerVec.size();i++){
		for(int j=0;j<centerVec[0].size();j++){
			cout<<centerVec[i][j]<<" ";
		}
		cout<<endl;
	}
	vector<int> old(centerVec[0].size(),0);
	int k=0;
	while(true){
		vector<int> labels=ClassifyD(data, centerVec, dataSizeH,dataSizeW);
		if(old == labels){
			break;
		}
		old=labels;
		double classNum[centerVec.size()];
		for(int i=0;i<centerVec.size();i++){
			classNum[i]=0;
		}
		for(int i=0;i<labels.size();i++){
			classNum[labels[i]]+=1;
		}

		vector<vector<double>> new_centerVec(centerVec.size(),vector<double>(25,0));
		for(int n=0;n<centerVec[0].size();n++){
			for(int r=0;r<dataSizeH;r++){
				for(int c=0;c<dataSizeW;c++){
					int theClass=labels[r*dataSizeW+c];
					new_centerVec[theClass][n]+=data[r][c][n]/classNum[theClass];
				}
			}	
		}
		centerVec=new_centerVec;
		k++;
		if(k%30==0){
			cout<<k<<"'s iteration"<<endl;
		}
	
	}
	return ClassifyD(data, centerVec, dataSizeH,dataSizeW);	
}


