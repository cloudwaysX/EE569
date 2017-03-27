#include <opencv2/xfeatures2d.hpp>
#include <opencv2/features2d.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/utility.hpp"
#include <iostream>
#include <algorithm>
  
using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

int main( int argc, char *argv[] )
{

  if(argc<5){
    cout<<"insufficient argument: program inFilename1 inFilename2 outFilename3 mode"<<endl;
  }

    char* inFilename1 = argv[1];
    char* inFilename2 = argv[2];
    char* outFilename3 = argv[3];
    int mode =atoi(argv[4]);

    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel=3;
    int SizeH = 300;
    int SizeW = 500;
 
    // Allocate image data array
    unsigned char Imagedata[SizeH][SizeW][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(inFilename1,"rb"))) {
      cout << "Cannot open file: " << inFilename1 <<endl;
      exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), SizeH*SizeW*BytesPerPixel, file);
    fclose(file);
    //Create Opencv mat structure for image dimension. For 8 bit bayer, type should be CV_8UC1.
    cv::Mat image1;
    image1.create(SizeH, SizeW, CV_8UC3);
    for(int r=0;r<SizeH;r++){
      for(int c=0;c<SizeW;c++){
        unsigned char tmp=Imagedata[r][c][0];
        Imagedata[r][c][0]=Imagedata[r][c][2];
        Imagedata[r][c][2]=tmp;
      }
    }

    memcpy(image1.data, Imagedata, SizeH*SizeW*BytesPerPixel);
    if( image1.empty() )
    { 
      cout<<"unable to read the image1"<<endl;
      return -1;
    }
    Mat gray1;
    cvtColor( image1, gray1, COLOR_BGR2GRAY );

    if (!(file=fopen(inFilename2,"rb"))) {
      cout << "Cannot open file: " << inFilename2 <<endl;
      exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), SizeH*SizeW*BytesPerPixel, file);
    fclose(file);
    //Create Opencv mat structure for image dimension. For 8 bit bayer, type should be CV_8UC1.
    cv::Mat image2;
    image2.create(SizeH, SizeW, CV_8UC3);
    for(int r=0;r<SizeH;r++){
      for(int c=0;c<SizeW;c++){
        unsigned char tmp=Imagedata[r][c][0];
        Imagedata[r][c][0]=Imagedata[r][c][2];
        Imagedata[r][c][2]=tmp;
      }
    }

    memcpy(image2.data, Imagedata, SizeH*SizeW*BytesPerPixel);
    if( image2.empty() )
    { 
      cout<<"unable to read the image2"<<endl;
      return -1;
    }

    Mat gray2;
    cvtColor( image2, gray2, COLOR_BGR2GRAY );

    ///////////////////////////Extract the features for two images////////////////
    
    vector<KeyPoint> kps1,kps2;
    cv::Mat kpsMap1(gray1.size(), gray1.type());
    cv::Mat kpsMap2(gray2.size(), gray2.type());
    cv::Mat des1(gray1.size(), gray1.type());
    cv::Mat des2(gray2.size(), gray2.type());
    
    if(mode==0){
      cv::Ptr<SIFT> pDollar =SIFT::create(0, 3, 0.04, 10, 1.6);
      pDollar->detectAndCompute(gray1, noArray(),kps1,des1,false);
      pDollar->detectAndCompute(gray2, noArray(),kps2,des2,false);
    }
    else{
      cv::Ptr<SURF> pDollar =SURF::create(600,4,3,false,false);
      pDollar->detectAndCompute(gray1, noArray(),kps1,des1,false);
      pDollar->detectAndCompute(gray2, noArray(),kps2,des2,false);
    }

    ///////////////////////Feature Match////////////
    cv::Mat matchedImg;

    //cv::Ptr<DescriptorMatcher> bf =BFMatcher::create(NORM_L2,true);
    cv::Ptr<DescriptorMatcher> bf =BFMatcher::create();
    vector<vector<DMatch>> matches;
    vector<DMatch> good;
    good.reserve(matches.size());
    bf->knnMatch(des1,des2,matches,2);
    for(int i=0;i<matches.size();i++){
      if(matches[i].size()<2) continue;
      if(matches[i][0].distance<0.75*matches[i][1].distance){
        good.push_back(matches[i][0]);
      }

    }
    

    //sort(matches.begin(),matches.end());
    //matches.resize(10);
    drawMatches(image1,kps1,image2,kps2,good,matchedImg);
    cv::imwrite(outFilename3,matchedImg);

 
      return 0;
}