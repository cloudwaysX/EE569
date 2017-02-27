
// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;


bool checkHit(int FilterNum, int*** img, int** FilterType,int r, int c);
int*** BFS(int*** img, int target);
int*** morphological(int*** img,int Filter1_size, int Filter2_size,int** Filter1, int** Filter2);
vector<int> BFS_shape(int*** img, int target, int seedC,int seedR);
int BytesPerPixel=1;
int Size = 480;
int iterationNum=100;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	
	// Check for proper syntax
	if (argc < 1){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name" << endl;
		return 0;
	}
	
	// Allocate image data array
	unsigned char Imagedata[Size][Size][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen("../images/P3/board.raw","rb"))) {
		cout << "Cannot open file: " << "board.raw" <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

	//Don't change this one!!!!
	int*** originalImg=new int**[Size];
	for(int i =0;i<Size;i++){
		originalImg[i]=new int*[Size];
	}
	for(int i =0;i<Size;i++){
		for(int j=0;j<Size;j++){
			originalImg[i][j]=new int[BytesPerPixel];
		}
	}

	for(int i =0;i<Size;i++){
		for(int j=0;j<Size;j++){
			originalImg[i][j][0]=Imagedata[i][j][0];
		}
	}


	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	//////////Generate the Filter/////////////////
	int S1[58][9]={
		{0,0,1,0,1,0,0,0,0},{1,0,0,0,1,0,0,0,0},{0,0,0,0,1,0,1,0,0},{0,0,0,0,1,0,0,0,1},
		{0,0,0,0,1,1,0,0,0},{0,1,0,0,1,0,0,0,0},{0,0,0,1,1,0,0,0,0},{0,0,0,0,1,0,0,1,0},
		{0,0,1,0,1,1,0,0,0},{0,1,1,0,1,0,0,0,0},{1,1,0,0,1,0,0,0,0},{1,0,0,1,1,0,0,0,0},{0,0,0,1,1,0,1,0,0},{0,0,0,0,1,0,1,1,0},{0,0,0,0,1,0,0,1,1},{0,0,0,0,1,1,0,0,1},		
		{0,0,1,0,1,1,0,0,1},{1,1,1,0,1,0,0,0,0},{1,0,0,1,1,0,1,0,0},{0,0,0,0,1,0,1,1,1},
		{1,1,0,0,1,1,0,0,1},{1,1,1,0,1,0,0,0,0},{0,1,1,1,1,0,0,0,0},{0,0,1,0,1,1,0,1,0},
		{0,1,1,0,1,1,0,0,0},{1,1,0,1,1,0,0,0,0},{0,0,0,1,1,0,1,1,0},{0,0,0,0,1,1,0,1,1},
		{1,1,0,0,1,1,0,0,1},{0,1,1,1,1,0,1,0,0},
		{1,1,1,0,1,1,0,0,0},{0,1,1,0,1,1,0,0,1},{1,1,1,1,1,0,0,0,0},{1,1,0,1,1,0,1,0,0},{1,0,0,1,1,0,1,1,0},{0,0,0,1,1,0,1,1,1},{0,0,0,0,1,1,1,1,1},{0,0,1,0,1,1,0,1,1},
		{1,1,1,0,1,1,0,0,1},{1,1,1,1,1,0,1,0,0},{1,0,0,1,1,0,1,1,1},{0,0,1,0,1,1,1,1,1},
		{0,1,1,0,1,1,0,1,1},{1,1,1,1,1,1,0,0,0},{1,1,0,1,1,0,1,1,0},{0,0,0,1,1,1,1,1,1},
		{1,1,1,0,1,1,0,1,1},{0,1,1,0,1,1,1,1,1},{1,1,1,1,1,1,1,0,0},{1,1,1,1,1,1,0,0,1},{1,1,1,1,1,0,1,1,0},{1,1,0,1,1,0,1,1,1},{1,0,0,1,1,1,1,1,1},{0,0,1,1,1,1,1,1,1},
		{1,1,1,0,1,1,1,1,1},{1,1,1,1,1,1,1,0,1},{1,1,1,1,1,0,1,1,1},{1,0,1,1,1,1,1,1,1},

	};

	int K1[40][9]={
		{0,0,1,0,1,1,0,0,1},{1,1,1,0,1,0,0,0,0},{1,0,0,1,1,0,1,0,0},{0,0,0,0,1,0,1,1,1},
		{0,0,1,0,1,1,0,0,1},{1,1,1,0,1,0,0,0,0},{1,0,0,1,1,0,1,0,0},{0,0,0,0,1,0,1,1,1},
		{1,1,1,0,1,1,0,0,0},{0,1,1,0,1,1,0,0,1},{1,1,1,1,1,0,0,0,0},{1,1,0,1,1,0,1,0,0},{1,0,0,1,1,0,1,1,0},{0,0,0,1,1,0,1,1,1},{0,0,0,0,1,1,1,1,1},{0,0,1,0,1,1,0,1,1},
		{1,1,1,0,1,1,0,0,1},{1,1,1,1,1,0,1,0,0},{1,0,0,1,1,0,1,1,1},{0,0,1,0,1,1,1,1,1},
		{0,1,1,0,1,1,0,1,1},{1,1,1,1,1,1,0,0,0},{1,1,0,1,1,0,1,1,0},{0,0,0,1,1,1,1,1,1},
		{1,1,1,0,1,1,0,1,1},{0,1,1,0,1,1,1,1,1},{1,1,1,1,1,1,1,0,0},{1,1,1,1,1,1,0,0,1},{1,1,1,1,1,0,1,1,0},{1,1,0,1,1,0,1,1,1},{1,0,0,1,1,1,1,1,1},{0,0,1,1,1,1,1,1,1},
		{1,1,1,0,1,1,1,1,1},{1,1,1,1,1,1,1,0,1},{1,1,1,1,1,0,1,1,1},{1,0,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,0,1,1},{1,1,1,1,1,1,1,1,0},{1,1,0,1,1,1,1,1,1},{0,1,1,1,1,1,1,1,1}
	};


	int ST2[69][9]={
	//int ST2[57][9]={
		{0,0,1,0,1,0,0,0,0},{1,0,0,0,1,0,0,0,0},
		{0,0,0,0,1,0,0,1,0},{0,0,0,0,1,1,0,0,0},
		{0,0,1,0,1,1,0,0,0},{0,1,1,0,1,0,0,0,0},{1,1,0,0,1,0,0,0,0},{1,0,0,1,1,0,0,0,0},
		{0,0,0,1,1,0,1,0,0},{0,0,0,0,1,0,1,1,0},{0,0,0,0,1,0,0,1,1},{0,0,0,0,1,1,0,0,1},
		{0,1,1,1,1,0,0,0,0},{1,1,0,0,1,1,0,0,0},{0,1,0,0,1,1,0,0,1},{0,0,1,0,1,1,0,1,0},
		{0,1,1,0,1,0,1,0,0},{0,0,1,0,1,1,1,0,0},{1,0,0,1,1,0,0,0,1},{1,1,0,0,1,0,0,0,1},{0,0,1,1,1,0,1,0,0},{0,0,1,0,1,0,1,1,0},{1,0,0,0,1,0,0,1,1},{1,0,0,0,1,1,0,0,1},
		{0,1,1,0,1,1,1,0,0},{1,1,0,1,1,0,0,0,1},{0,0,1,1,1,0,1,1,0},{1,0,0,0,1,1,0,1,1},
		{1,1,-1,1,1,-1,-1,-1,-1},
		{-1,1,0,1,1,1,-1,0,0},{0,1,-1,1,1,1,0,0,-1},{0,0,-1,1,1,1,0,1,-1},{-1,0,0,1,1,1,-1,1,0},
		{-1,1,-1,1,1,0,0,1,0},{0,1,0,1,1,0,-1,1,-1},{0,1,0,0,1,1,-1,1,-1},{-1,1,-1,0,1,1,0,1,0},
		{1,-1,1,-1,1,-1,1,0,0},{1,-1,1,-1,1,-1,0,1,0},{1,-1,1,-1,1,-1,0,0,1},{1,-1,1,-1,1,0,1,-1,0},{1,-1,0,-1,1,1,1,-1,0},{1,-1,0,-1,1,0,1,-1,1},{1,0,0,-1,1,-1,1,-1,1},{0,1,0,-1,1,-1,1,-1,1},{0,0,1,-1,1,-1,1,-1,1},{1,-1,1,0,1,-1,0,-1,1},{0,-1,1,1,1,-1,0,-1,1},{0,-1,1,0,1,-1,1,-1,1},
		{1,-1,1,-1,1,-1,1,1,0},{1,-1,1,-1,1,-1,0,1,1},{1,-1,1,-1,1,-1,1,0,1},{1,-1,1,-1,1,1,1,-1,0},{1,-1,0,-1,1,1,1,-1,1},{1,-1,1,-1,1,0,1,-1,1},{1,1,0,-1,1,-1,1,-1,1},{0,1,1,-1,1,-1,1,-1,1},{1,0,1,-1,1,-1,1,-1,1},{1,-1,1,1,1,-1,0,-1,1},{0,-1,1,1,1,-1,1,-1,1},{1,-1,1,0,1,-1,1,-1,1},
		{1,-1,1,-1,1,-1,1,1,1},{1,-1,1,-1,1,1,1,-1,1},{1,1,1,-1,1,-1,1,-1,1},{1,-1,1,1,1,-1,1,-1,1},
		{-1,1,0,0,1,1,1,0,-1},{0,1,-1,1,1,0,-1,0,1},{-1,0,1,1,1,0,0,1,-1},{1,0,-1,0,1,1,-1,1,0}
	};

	int K2[50][9]={
		{0,0,0,0,1,0,0,0,1},{0,0,0,0,1,0,1,0,0},{0,0,1,0,1,0,0,0,0},{1,0,0,0,1,0,0,0,0},
		{0,0,0,1,1,0,0,0,0},{0,1,0,0,1,0,0,0,0},{0,0,0,0,1,0,0,1,0},{0,0,0,0,1,1,0,0,0},
		{0,1,0,0,1,1,0,0,0},{0,1,0,1,1,0,0,0,0},{0,0,0,0,1,1,0,1,0},{0,0,0,1,1,0,0,1,0},
		{1,1,-1,1,1,-1,-1,-1,-1},{-1,-1,-1,-1,1,1,-1,1,1},
		{-1,1,-1,1,1,1,-1,-1,-1},{-1,1,-1,1,1,-1,-1,1,-1},{-1,-1,-1,1,1,1,-1,1,-1},{-1,1,-1,-1,1,1,-1,1,-1},
		{1,-1,1,-1,1,-1,1,0,0},{1,-1,1,-1,1,-1,0,1,0},{1,-1,1,-1,1,-1,0,0,1},{1,-1,1,-1,1,0,1,-1,0},{1,-1,0,-1,1,1,1,-1,0},{1,-1,0,-1,1,0,1,-1,1},{1,0,0,-1,1,-1,1,-1,1},{0,1,0,-1,1,-1,1,-1,1},{0,0,1,-1,1,-1,1,-1,1},{1,-1,1,0,1,-1,0,-1,1},{0,-1,1,1,1,-1,0,-1,1},{0,-1,1,0,1,-1,1,-1,1},
		{1,-1,1,-1,1,-1,1,1,0},{1,-1,1,-1,1,-1,0,1,1},{1,-1,1,-1,1,-1,1,0,1},{1,-1,1,-1,1,1,1,-1,0},{1,-1,0,-1,1,1,1,-1,1},{1,-1,1,-1,1,0,1,-1,1},{1,1,0,-1,1,-1,1,-1,1},{0,1,1,-1,1,-1,1,-1,1},{1,0,1,-1,1,-1,1,-1,1},{1,-1,1,1,1,-1,0,-1,1},{0,-1,1,1,1,-1,1,-1,1},{1,-1,1,0,1,-1,1,-1,1},
		{1,-1,1,-1,1,-1,1,1,1},{1,-1,1,-1,1,1,1,-1,1},{1,1,1,-1,1,-1,1,-1,1},{1,-1,1,1,1,-1,1,-1,1},
		{-1,1,0,0,1,1,1,0,-1},{0,1,-1,1,1,0,-1,0,1},{-1,0,1,1,1,0,0,1,-1},{1,0,-1,0,1,1,-1,1,0}
	};



	int** Filter1;
	int** Filter2;
	int Filter1_size;
	int Filter2_size;

	Filter1_size=58;
	Filter2_size=69;

	Filter1=new int*[Filter1_size];
	for(int i=0;i<Filter1_size;i++){
		Filter1[i]=new int[9];
	}
	for(int i=0;i<Filter1_size;i++){
		for(int j=0;j<9;j++){
			Filter1[i][j]=S1[i][j];
		}
	}

	Filter2=new int*[Filter2_size];
	for(int i=0;i<Filter2_size;i++){
		Filter2[i]=new int[9];
	}
	for(int i=0;i<Filter2_size;i++){
		for(int j=0;j<9;j++){
			Filter2[i][j]=ST2[i][j];
		}
	}

	//declare pointer for image

	int*** img=new int**[Size];
	for(int i =0;i<Size;i++){
		img[i]=new int*[Size];
	}
	for(int i =0;i<Size;i++){
		for(int j=0;j<Size;j++){
			img[i][j]=new int[BytesPerPixel];
			img[i][j][0]=255-Imagedata[i][j][0];
		}
	}
	//cout<<"a"<<endl;
	img=morphological(img,Filter1_size,Filter2_size,Filter1,Filter2);
	// Write image data (filename specified by second argument) from image data matrix
	//only count the isolate point
	int holeNum=0;
	for(int r=0;r<Size;r++){
		for(int c=0;c<Size;c++){
			int curVal=img[r][c][0];
			Imagedata[r][c][0]=curVal;
			if(curVal==255){
				int sum=0;
				for(int i=-1;i<=1;i++){
					for(int j=-1;j<=1;j++){
						int newR=r+i; int newC=c+j;
						if(newR<0||newR>=Size||newC<0||newC>=Size) continue;
						sum+=img[r+i][c+j][0]/255;
					}
				}
				if(sum==1){
					holeNum++;
				}
			}

		}
	}

	cout<<"the hole number is: "<<holeNum<<endl;

	for(int i =0;i<Size;i++){
		for(int j=0;j<Size;j++){
			delete img[i][j];
		}
	}
	for(int i =0;i<Size;i++){
		delete img[i];
	}
	delete img;

	if (!(file=fopen("../result/P3/countHoles.raw","wb"))) {
		cout << "Cannot open file: " << "countHoles.raw" << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);
	///////////////////////////////////////////////////////////////////////////////////
	
	int*** backgroundRemovedImg=BFS(originalImg,0);
	for(int r=0;r<Size;r++){
		for(int c=0;c<Size;c++){
			int curVal=backgroundRemovedImg[r][c][0];
			if(curVal==0){
				Imagedata[r][c][0]=255;
			}
			else{
				Imagedata[r][c][0]=originalImg[r][c][0];
			}


		}
	}


	int*** img_nohole=new int**[Size];
	for(int i =0;i<Size;i++){
		img_nohole[i]=new int*[Size];
	}
	for(int i =0;i<Size;i++){
		for(int j=0;j<Size;j++){
			img_nohole[i][j]=new int[BytesPerPixel];
			img_nohole[i][j][0]=Imagedata[i][j][0];
		}
	}

	if (!(file=fopen("../result/P3/countObject_backgroundRemoved.raw","wb"))) {
		cout << "Cannot open file: " << "countHoles.raw" << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

	img=new int**[Size];
	for(int i =0;i<Size;i++){
		img[i]=new int*[Size];
	}
	for(int i =0;i<Size;i++){
		for(int j=0;j<Size;j++){
			img[i][j]=new int[BytesPerPixel];
			img[i][j][0]=Imagedata[i][j][0];
		}
	}

	img=morphological(img,Filter1_size,Filter2_size,Filter1,Filter2);

	vector< vector<int> > objectSeeds;

	for(int r=0;r<Size;r++){
		for(int c=0;c<Size;c++){
			Imagedata[r][c][0]=img[r][c][0];
			if(img[r][c][0]==255){
				vector<int> temp;
				temp.push_back(r);
				temp.push_back(c);
				objectSeeds.push_back(temp);
			} 
		}
	}
	if (!(file=fopen("../result/P3/countObject.raw","wb"))) {
		cout << "Cannot open file: " << "countObject.raw" << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

	cout<<"The object number is:"<< objectSeeds.size()<<endl;

	int squareNum=0;
	int circleNum=0;
	/////////////////////////////////////////////////////////////////////////////////////
	for(int i=0;i<objectSeeds.size();i++){
		int seedR=objectSeeds.at(i).at(0);
		int seedC=objectSeeds.at(i).at(1);
		vector<int> shape;
		shape=BFS_shape(img_nohole,255,seedC,seedR);
		cout<<seedR<<" "<<seedC<<":"<<endl;
		cout<<"Area:"<<shape.at(0)<<" Perimeter:"<<shape.at(1)<<endl;
		int ratio=shape.at(1)*shape.at(1)/shape.at(0);
		cout<<ratio<<endl;
		if(ratio==15||ratio==16){
			circleNum++;
		}
		else{
			squareNum++;
		}
	}
	cout<<"squareNum: "<<squareNum<<" circleNum: "<<circleNum<<endl;


	return 0;
}

bool checkHit(int FilterNum, int*** img, int** FilterType,int r, int c){
	bool isCandidate=false;
	for(int m=0;m<FilterNum&&!isCandidate;m++){
		for(int i=0;i<9;i++){

			int r2=i/3-1+r;
			int c2=i%3-1+c;
			if(r2<0||c2<0||r2>=Size||c2>=Size) continue;
			if(FilterType[m][i]!=-1&&FilterType[m][i]!=(img[r2][c2][0]/255)){
				break;
			}
			if(i==8){
				isCandidate=true;
			}
		}
	}
	return isCandidate;
}

int*** morphological(int*** img,int Filter1_size, int Filter2_size,int** Filter1, int** Filter2){

	int*** temp_result=new int**[Size];
	for(int i =0;i<Size;i++){
		temp_result[i]=new int*[Size];
	}
	for(int i =0;i<Size;i++){
		for(int j=0;j<Size;j++){
			temp_result[i][j]=new int[BytesPerPixel];
		}
	}


	for(int iter=0;iter<iterationNum;iter++){

		for(int r=0;r<Size;r++){
			for(int c=0;c<Size;c++){

				if(img[r][c][0]==0){ //check F(u,v)
					temp_result[r][c][0]=0; //G(u,v)=F(u,v) if F(u,v)!=1
					continue;
				} 
				//cout<<"c"<<endl;
				bool isHit=checkHit(Filter1_size, img,Filter1,r,c);
				if(!isHit){
					temp_result[r][c][0]=0; //G(u,v)=F(u,v) if M(u,v)!=1
				}
				else{
					temp_result[r][c][0]=255;
				}
			}
		}
		for(int r=0;r<Size;r++){
			for(int c=0;c<Size;c++){
				if(temp_result[r][c][0]==0){ //check F(u,v)
					continue;
				} 
				bool isHit2=checkHit(Filter2_size,temp_result,Filter2, r,c);
				if(!isHit2){
					img[r][c][0]=255-img[r][c][0]; //G(u,v)=F(u,v) if UnconditionalMask=1;
				}
			}
		}
	}

	//delete pointer
	for(int i =0;i<Size;i++){
		for(int j=0;j<Size;j++){
			delete temp_result[i][j];
		}
	}
	for(int i =0;i<Size;i++){
		delete temp_result[i];
	}
	delete temp_result;


	return img;
}

int*** BFS(int*** img, int target){
	vector<vector<int> > candidates;

	//declare pointer for image
	int*** temp_result=new int**[Size];
	for(int i =0;i<Size;i++){
		temp_result[i]=new int*[Size];
	}
	for(int i =0;i<Size;i++){
		for(int j=0;j<Size;j++){
			temp_result[i][j]=new int[BytesPerPixel];
		}
	}
	for(int i =0;i<Size;i++){
		for(int j=0;j<Size;j++){
			temp_result[i][j][0]=img[i][j][0];
		}
	}

	candidates.push_back(vector<int>(2,0));
	temp_result[0][0][0]=255-target;
	while(!candidates.empty()){
		//cout<<"new one"<<endl;
		vector<int> curPixel=candidates.front();
		//cout<<candidates.size()<<endl;
		candidates.erase(candidates.begin());
		for(int i=-1;i<=1;i++){
			for(int j=-1;j<=1;j++){
				int newR=curPixel[0]+i;
				int newC=curPixel[1]+j;
				if(newR<0||newC<0||newR>=Size||newC>=Size) continue;

				if(temp_result[newR][newC][0]==target){
					vector<int> curVal;
					curVal.push_back(newR);curVal.push_back(newC);
					candidates.push_back(curVal);
					temp_result[newR][newC][0]=255-target;
				}

			}
		}
	}

	return temp_result;

}


vector<int> BFS_shape(int*** img, int target, int seedC,int seedR){
	vector<vector<int> > candidates;

	int Area=0;
	int Perimeter=0;

	//declare pointer for image
	int*** temp_result=new int**[Size];
	for(int i =0;i<Size;i++){
		temp_result[i]=new int*[Size];
	}
	for(int i =0;i<Size;i++){
		for(int j=0;j<Size;j++){
			temp_result[i][j]=new int[BytesPerPixel];
		}
	}
	for(int i =0;i<Size;i++){
		for(int j=0;j<Size;j++){
			temp_result[i][j][0]=img[i][j][0];
		}
	}

	vector<int> seed;
	seed.push_back(seedR);seed.push_back(seedC);
	candidates.push_back(seed);
	temp_result[seedR][seedC][0]=255-target;
	while(!candidates.empty()){
		Area++;
		//cout<<"new one"<<endl;
		vector<int> curPixel=candidates.front();
		//cout<<candidates.size()<<endl;
		candidates.erase(candidates.begin());
		bool isBoundary=false;
		for(int i=-1;i<=1;i++){
			for(int j=-1;j<=1;j++){
				int newR=curPixel[0]+i;
				int newC=curPixel[1]+j;
				if(newR<0||newC<0||newR>=Size||newC>=Size) continue;

				if(temp_result[newR][newC][0]==target){
					vector<int> curVal;
					curVal.push_back(newR);curVal.push_back(newC);
					candidates.push_back(curVal);
					temp_result[newR][newC][0]=255-target;
				}
				
				if(img[newR][newC][0]!=target && !isBoundary){
					isBoundary=true;
				}
				//cout<<candidates.size()<<endl;
			}
		}
		if(isBoundary) Perimeter++;
	}

	vector<int> result;
	result.push_back(Area);result.push_back(Perimeter);
	return result;

}