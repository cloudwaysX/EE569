
// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;


bool checkHit(int FilterNum, int*** img, int** FilterType,int r, int c);


int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=1;
	int Size = 480;
	
	// Check for proper syntax
	if (argc < 5){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name mode input_filename output_filename iter" << endl;
		return 0;
	}
	
	int mode = atoi(argv[1]);
	int iterationNum=atoi(argv[4]);
	// Allocate image data array
	unsigned char Imagedata[Size][Size][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[2],"rb"))) {
		cout << "Cannot open file: " << argv[2] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	//////////Generate the Filter/////////////////
	int S1[12][9]={
		{0,0,0,0,1,1,0,0,0},{0,1,0,0,1,0,0,0,0},{0,0,0,1,1,0,0,0,0},{0,0,0,0,1,0,0,1,0},
		{0,1,1,0,1,1,0,0,0},{1,1,0,1,1,0,0,0,0},{0,0,0,1,1,0,1,1,0},{0,0,0,0,1,1,0,1,1},
		{0,1,1,0,1,1,0,1,1},{1,1,1,1,1,1,0,0,0},{1,1,0,1,1,0,1,1,0},{0,0,0,1,1,1,1,1,1},
	};

	int T1[46][9]={
		{0,1,0,0,1,1,0,0,0},{0,1,0,1,1,0,0,0,0},{0,0,0,1,1,0,0,1,0},{0,0,0,0,1,1,0,1,0},
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

	int test[1][9]={
		{0,0,0,1,1,1,1,1,1}
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
	if(mode==0){
		Filter1_size=12;
		Filter2_size=69;
		//Filter2_size=57;

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

	}
	else if(mode==1){
		Filter1_size=46;
		Filter2_size=69;
		//Filter2_size=57;

		Filter1=new int*[Filter1_size];
		for(int i=0;i<Filter1_size;i++){
			Filter1[i]=new int[9];
		}
		for(int i=0;i<Filter1_size;i++){
			for(int j=0;j<9;j++){
				Filter1[i][j]=T1[i][j];
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

	}
	else if(mode==2){
		Filter1_size=40;
		Filter2_size=50;
		//Filter2_size=;

		Filter1=new int*[Filter1_size];
		for(int i=0;i<Filter1_size;i++){
			Filter1[i]=new int[9];
		}
		for(int i=0;i<Filter1_size;i++){
			for(int j=0;j<9;j++){
				Filter1[i][j]=K1[i][j];
			}
		}

		Filter2=new int*[Filter2_size];
		for(int i=0;i<Filter2_size;i++){
			Filter2[i]=new int[9];
		}
		for(int i=0;i<Filter2_size;i++){
			for(int j=0;j<9;j++){
				Filter2[i][j]=K2[i][j];
			}
		}
	}
	else{
		Filter1_size=1;
		Filter2_size=50;
		//Filter2_size=;

		Filter1=new int*[Filter1_size];
		for(int i=0;i<Filter1_size;i++){
			Filter1[i]=new int[9];
		}
		for(int i=0;i<Filter1_size;i++){
			for(int j=0;j<9;j++){
				Filter1[i][j]=test[i][j];
			}
		}

		Filter2=new int*[Filter2_size];
		for(int i=0;i<Filter2_size;i++){
			Filter2[i]=new int[9];
		}
		for(int i=0;i<Filter2_size;i++){
			for(int j=0;j<9;j++){
				Filter2[i][j]=K2[i][j];
			}
		}		
	}


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

	int*** img=new int**[Size];
	for(int i =0;i<Size;i++){
		img[i]=new int*[Size];
	}
	for(int i =0;i<Size;i++){
		for(int j=0;j<Size;j++){
			img[i][j]=new int[BytesPerPixel];
			img[i][j][0]=Imagedata[i][j][0];
		}
	}

	//Do the removal
	unsigned char Target=255; //shrink the white square, 255->1
	for(int iter=0;iter<iterationNum;iter++){
		//cout<<iter<<endl;
		for(int r=0;r<Size;r++){
			for(int c=0;c<Size;c++){
				//cout<<r<<" "<<c<<endl;
				if(img[r][c][0]==255-Target){ //check F(u,v)
					temp_result[r][c][0]=0; //G(u,v)=F(u,v) if F(u,v)!=1
					continue;
				} 
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
				if(temp_result[r][c][0]==255-Target){ //check F(u,v)
					continue;
				} 
				bool isHit2=checkHit(Filter2_size,temp_result,Filter2, r,c);
				if(!isHit2){
					img[r][c][0]=255-img[r][c][0]; //G(u,v)=F(u,v) if UnconditionalMask=1;
				}
			}
		}

		/*for(int r=0;r<Size;r++){
			for(int c=0;c<Size;c++){
				img[r][c][0]=temp_result[r][c][0];
			}
		}*/

	}

	// Write image data (filename specified by second argument) from image data matrix
	for(int r=0;r<Size;r++){
		for(int c=0;c<Size;c++){
			Imagedata[r][c][0]=img[r][c][0];
		}
	}

	if (!(file=fopen(argv[3],"wb"))) {
		cout << "Cannot open file: " << argv[3] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
	fclose(file);


	//delete pointer

	return 0;
}

bool checkHit(int FilterNum, int*** img, int** FilterType,int r, int c){
	bool isCandidate=false;
	for(int m=0;m<FilterNum&&!isCandidate;m++){
		for(int i=0;i<9;i++){
			int r2=i/3-1;
			int c2=i%3-1;
			if(FilterType[m][i]!=-1&&FilterType[m][i]!=(img[r2+r][c2+c][0]/255)){
				break;
			}
			if(i==8){
				isCandidate=true;
			}
		}
	}
	return isCandidate;
}