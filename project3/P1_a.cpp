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
vector<int> ClassifyD(double** data, vector<vector<double>> centerVec, int dataSize);
vector<int> Kmeans(double** data, vector<vector<double>> &centerVec, int dataSize);

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=1;
	int Size = 128;
	
	// Check for proper syntax
	if (argc < 2){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name mode[full dimension/PCA]" << endl;
		return 0;
	}

	int mode = atoi(argv[1]);

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


	double** ave_featureVec25D = new double*[12];
	for(int i=0;i<12;i++){
		ave_featureVec25D[i]=new double[25];
	}

	double** ave_featureVec25D_test = new double*[12];
	for(int i=0;i<12;i++){
		ave_featureVec25D_test[i]=new double[25];
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
	//cout<<"filterBanks"<<endl;
	for(int i =0;i<25;i++){
		filterBanks[i]=computeTensor(Kernels1D[i/5],Kernels1D[i%5]);
		for(int i3=0;i3<25;i3++){
			//cout<<filterBanks[i][i3]<<" ";
		}
		//cout<<endl;
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
				mean+=Imagedata[row][col][0];
			}
		}
		mean=mean/(double)(Size*Size);

		//extract the 25D features
		for(int row=0;row<Size;row++){
			for(int col=0;col<Size;col++){
				for(int n=0;n<25;n++){
					float tmp=0;
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
								
							//textures[row*Size+col][n][i]+=Imagedata[newR][newC][0]*filterBanks[n][m]
							tmp+=((double)Imagedata[newR][newC][0]-mean)*filterBanks[n][filterIndex];

						}
					}
					//ave_featureVec25D[i][n]+=pow(tmp,2)/(float)(Size*Size);
					ave_featureVec25D[i][n]+=abs(tmp)/(float)(Size*Size);

				}

			}
		}

	}

	string s[6] ={"A","B","C","D","E","F"};
	for (int i=0;i<6;i++){
		string temp ="./HW3_images/P1/Texture"+ s[i] +".raw";
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
				mean+=Imagedata[row][col][0];
			}
		}
		mean=mean/(double)(Size*Size);

		//extract the 25D features
		for(int row=0;row<Size;row++){
			for(int col=0;col<Size;col++){
				for(int n=0;n<25;n++){
					float tmp=0;
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
								
							tmp+=((double)Imagedata[newR][newC][0]-mean)*filterBanks[n][filterIndex];

						}
					}
					//ave_featureVec25D[i][n]+=pow(tmp,2)/(float)(Size*Size);
					ave_featureVec25D_test[i][n]+=abs(tmp)/(float)(Size*Size);

				}

			}
		}

	}
	int featureNum=25;
	/*
	//visualize 25D features
	cout.precision(2);
	cout<<featureNum<<"D features: \n";
	for(int i=0;i<12;i++){
		for(int j=0;j<featureNum;j++){
			cout<<ave_featureVec25D[i][j]<<" ";
		}
		cout<<endl;
	}*/

	
	///////////////////////////////////////////////////////////////////////////////////
	//Calculate PCA
	double** ave_featureVec3D;
	double** ave_featureVec3D_test;
	if(mode==1){
		float ave_featureVec25D2[12][25];
		for(int i=0;i<12;i++){
			for(int j=0;j<25;j++){
				ave_featureVec25D2[i][j]=(float)ave_featureVec25D[i][j];
			}
		}
		Mat texs = Mat(12,25,CV_32FC1,ave_featureVec25D2);
		PCA pca(texs,Mat(),PCA::DATA_AS_ROW,3);
		Mat ave_featureVec_PCA;
		ave_featureVec_PCA.create(texs.rows,3,texs.type());
		pca.project(texs,ave_featureVec_PCA);

		ave_featureVec3D = new double*[12];
		for(int i=0;i<12;i++){
			ave_featureVec3D[i]=new double[3];
			for(int j=0;j<3;j++){
				ave_featureVec3D[i][j]=ave_featureVec_PCA.at<float>(i,j);
			}
		}

		//also project testing
		float ave_featureVec25D2_test[6][25];
		for(int i=0;i<6;i++){
			for(int j=0;j<25;j++){
				ave_featureVec25D2_test[i][j]=(float)ave_featureVec25D_test[i][j];
			}
		}
		Mat texs2 = Mat(6,25,CV_32FC1,ave_featureVec25D2_test);
		Mat ave_featureVec_PCA_test;
		ave_featureVec_PCA_test.create(texs2.rows,3,texs.type());
		pca.project(texs2,ave_featureVec_PCA_test);

		ave_featureVec3D_test = new double*[6];
		for(int i=0;i<6;i++){
			ave_featureVec3D_test[i]=new double[3];
			for(int j=0;j<3;j++){
				ave_featureVec3D_test[i][j]=ave_featureVec_PCA_test.at<float>(i,j);
			}
		}

		/*//visualize the reduced 3D 
		cout<<"3D features"<<endl;
		for(int i=0;i<12;i++){
			for(int j=0;j<3;j++){
				cout<<ave_featureVec3D[i][j]<<" ";
			}
			cout<<"\n"<<endl;
		}*/
	}


	////////////////////////////////////////////////////////////////////////////
	////////K means////////////////////////
	double** ave_featureVec;
	double p1=1;
	double p2=1;
	if(mode==1){
		featureNum=3;
		ave_featureVec = ave_featureVec3D;
		p1=1.2;
		p2=0.8;
	}
	else{
		featureNum=25;
		ave_featureVec = ave_featureVec25D;
		p1=1.2;
		p2=0.8;
	}

	//calculate means and standard deviation
	//cout<<featureNum<<"D means: \n";
	vector<vector<double>> mean(2,vector<double>(featureNum,0));
	vector<vector<double>> stdv(2,vector<double>(featureNum,0));
	for(int i=0;i<featureNum;i++){
		for(int j=0;j<12;j++){
			mean[0][i]+=ave_featureVec[j][i]/12.0;
		}
		//cout<<mean[0][i]<<" ";
	}
	//cout<<"\n"<<endl;
	//cout<<featureNum<<"D stdv: \n";
	for(int i=0;i<featureNum;i++){
		for(int j=0;j<12;j++){
			stdv[0][i]+=pow(ave_featureVec[j][i]-mean[0][i],2)/12.0;
		}
	}
	for(int i=0;i<featureNum;i++){
		stdv[0][i]=sqrt(stdv[0][i]);
		//cout<<stdv[0][i]<<" ";
	}
	cout<<endl;

	//intialize the centroid 
	vector<std::vector<double>> centroids;
	vector<double> centroid1,centroid2,centroid3,centroid4,centroid5,centroid6;
	for(int i=0;i<featureNum;i++){
		centroid1.push_back(mean[0][i]-p1*stdv[0][i]);
		centroid2.push_back(mean[0][i]+p2*stdv[0][i]);
	}
	mean[0]=vector<double>(featureNum,0);stdv[0]=vector<double>(featureNum,0);
	centroids.push_back(centroid1);
	centroids.push_back(centroid2);

	double classNum[2]={0,0};
	vector<int> labels=ClassifyD(ave_featureVec, centroids, 12);
	for(int i=0;i<labels.size();i++){
		classNum[labels[i]]+=1;
	}
	cout<<"classNum: "<<classNum[0]<<" "<<classNum[1]<<endl;

	//first division:visualize
	for(int j=0;j<featureNum;j++){
		for(int i=0;i<12;i++){
			mean[labels[i]][j]+=ave_featureVec[i][j]/classNum[labels[i]];
		}
		//cout<<mean[0][j]<<" ";
	}

	for(int i=0;i<2;i++){
		//cout<<"class "<<i<<"'s mean: ";
		for(int j=0;j<featureNum;j++){
			//cout<<mean[i][j]<<" ";
		}
	}
	cout<<endl;

	for(int j=0;j<featureNum;j++){
		for(int i=0;i<12;i++){
			stdv[labels[i]][j]+=pow(ave_featureVec[i][j]-mean[labels[i]][j],2)/classNum[labels[i]];
		}
	}

	for(int i=0;i<2;i++){
		//cout<<"class "<<i<<"'s mean: ";
		for(int j=0;j<featureNum;j++){
			stdv[i][j]=sqrt(stdv[i][j]);
			//cout<<stdv[i][j]<<" ";
		}
	}

	//second division: visualize
	for(int i=0;i<featureNum;i++){
		centroid3.push_back(mean[0][i]-p1*stdv[0][i]);
		centroid4.push_back(mean[0][i]+p2*stdv[0][i]);
		centroid5.push_back(mean[1][i]-p1*stdv[1][i]);
		centroid6.push_back(mean[1][i]+p2*stdv[1][i]);

	}
	centroids.clear();
	centroids.push_back(centroid3);
	centroids.push_back(centroid4);
	centroids.push_back(centroid5);
	centroids.push_back(centroid6);

	vector<int> result=Kmeans(ave_featureVec,centroids, 12);
	//vector<int> result=ClassifyD(ave_featureVec, centroids, 12);
	cout<<" final center is : "<<endl;
	for(int i=0;i<centroids.size();i++){
		for(int j=0;j<centroids[0].size();j++){
			cout<<centroids[i][j]<<" ";
		}
		cout<<endl;
	}
	for(int i=0;i<result.size();i++){
		cout<<i+1<<" pic is labled as class "<<result[i]<<endl;
	}
	if(mode==0){
		vector<int> test_result=ClassifyD(ave_featureVec25D_test, centroids, 6);
		for(int i=0;i<test_result.size();i++){
			char name = i+65;
			cout<<name<<" pic is labled as class "<<test_result[i]<<endl;
		}
	}
	else{
		vector<int> test_result=ClassifyD(ave_featureVec3D_test, centroids, 6);
		for(int i=0;i<test_result.size();i++){
			char name = i+65;
			cout<<name<<" pic is labled as class "<<test_result[i]<<endl;
		}

	}


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

vector<int> ClassifyD(double** data, vector<vector<double>> centerVec, int dataSize){
	vector<int> labels;
	for(int i=0;i<dataSize;i++){
		float minSum=FLT_MAX;
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
		//cout<<i<<" pic is labled as class "<<minClass<<endl;
		labels.push_back(minClass);
	}
	return labels;
}

vector<int> Kmeans(double** data, vector<vector<double>> &centerVec, int dataSize){
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
		//classify according to new centroid and test if changed
		vector<int> labels=ClassifyD(data, centerVec, dataSize);
		if(old == labels){
			break;
		}
		old=labels;


		vector<double> classNum(centerVec.size(),0);
		for(int i=0;i<labels.size();i++){
			classNum[labels[i]]+=1;
		}

		vector<vector<double>> new_centerVec(centerVec.size(),vector<double>(centerVec[0].size(),0));
		for(int j=0;j<centerVec[0].size();j++){
			for(int i=0;i<dataSize;i++){
				int theClass = labels[i];
				new_centerVec[theClass][j]+=data[i][j]/classNum[theClass];
			}
		}
		centerVec = new_centerVec;

		/*cout<<k<<" iteration's : "<<endl;
		for(int i=0;i<centerVec.size();i++){
			cout<<"class "<<i<<" has "<<classNum[i]<<endl;
		}*/

		k++;
	}
	return ClassifyD(data, centerVec, dataSize);		
}