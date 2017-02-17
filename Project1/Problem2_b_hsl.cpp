// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector> 
#include <cmath>

using namespace std;

vector<unsigned char> HSLtoRGB(unsigned char H, unsigned char L, unsigned char S);
double CalcH(double M, double C, int mode, double diff);

double CalcL(double M, double m);

double CalcS(double L, double C);


int main(int argc, char *argv[])

{
	// Check for proper syntax
	if (argc < 5){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw height width " << endl;
		return 0;
	}


	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel=3;
	int height = atoi(argv[3]);
	int width = atoi(argv[4]);
	double range=255;

	
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
	//Mirror the Image acrross vertical axis
	unsigned char normalizeParameter = 255;

	for(int r=0;r<height;r++){
		for(int c=0;c<width;c++){
			double R=Imagedata[r][c][0]/255.0;
			double G=Imagedata[r][c][1]/255.0;
			double B=Imagedata[r][c][2]/255.0;

			cout<<"new: "<<R*255<<" "<<G*255<<" "<<B*255<<endl;
			double M=max(max(R,G),B);
			double m=min(min(R,G),B);
			double C=M-m;

			double L=CalcL(M,m);
			double S=CalcS(L,C);

			int mode=0;
			double diff=0;

			if(M==R){
				mode=1; diff=G-B;
			}
			else if(M==G){
				mode=2; diff=B-R;
			}
			else{
				mode=3; diff=R-G;
			}
			double H=CalcH(M,C,mode,diff);

			unsigned char temp[3]={H/360.0*255,S*255,L*255};
			
			Imagedata[r][c][0]=temp[0];
			Imagedata[r][c][1]=temp[1];
			Imagedata[r][c][2]=temp[2];

		}
	}

	int channel=2;
	vector<unsigned int> hist(256,0);
	vector<double> culhis(256,0);
		for(int r=0;r<height;r++){
			for(int c=0;c<width;c++){
				hist.at(Imagedata[r][c][channel])+=1;
			}
		}
		culhis.at(0)=hist.at(0)/(height*width);
		for(int i=1;i<256;i++){
			culhis.at(i)=culhis.at(i-1)+hist.at(i)/(double)(height*width);
		}

		for(int r=0;r<height;r++){
			for(int c=0;c<width;c++){
			
				Imagedata[r][c][channel]=(unsigned char)(culhis.at((int)Imagedata[r][c][channel])*range);
				//cout<<(int)Imagedata[r][c][0]<<endl;
			}
		}

	for(int r=0;r<height;r++){
		for(int c=0;c<width;c++){
			vector<unsigned char> RGB=HSLtoRGB(Imagedata[r][c][0],Imagedata[r][c][1],Imagedata[r][c][2]);
			Imagedata[r][c][0]=RGB[0];
			Imagedata[r][c][1]=RGB[1];
			Imagedata[r][c][2]=RGB[2];

		}
	}


	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	return 0;
}

//0 to 360
double CalcH(double M, double C, int mode, double diff){
	if(C==0) return 0;

	switch(mode){
		case 1 : return 60*((diff/C)-(int)(diff/C/6)*6);
		case 2 : return 60*(diff/C+2);
		case 3 : return 60*(diff/C+4);
	}		
}

//0 to 1
double CalcL(double M, double m){
	return (M+m)/2.0;
}

//0 to 1
double CalcS(double L, double C){
	if(L==0) return 0;

	if(L<0.5) return C/L/2.0;

	return C/(1.0-L)/2.0;	
}



vector<unsigned char> HSLtoRGB(unsigned char H, unsigned char S,unsigned char L){
	vector<unsigned char> RGB;
	double R,G,B;
	double l=L/255.0; double s=S/255.0; double h=H/255.0*360;
	double c=(1-abs(2*l-1))*s;
	double x=c*(1-abs(fmod(h/60.0,2)-1));
	if(h<=60){
		R=c;G=x;B=0;
	}
	else if(h<=120){
		R=x;G=c;B=0;
	}
	else if(h<=180){
		R=0;G=c;B=x;
	}
	else if(h<=240){
		R=0;G=x;B=c;
	}
	else if(h<=300){
		R=x;G=0;B=c;
	}
	else{
		R=c;G=0;B=x;
	}

	double m=l-c/2.0;

	RGB.push_back(255*(R+m));
	RGB.push_back(255*(G+m));
	RGB.push_back(255*(B+m));
	return RGB;

}
