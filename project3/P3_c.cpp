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

    char** inFilenames = new char*[4];
    inFilenames[0]="./HW3_images/P2/truck.raw";
    inFilenames[1]="./HW3_images/P2/suv.raw";
    inFilenames[2]="./HW3_images/P2/convertible_1.raw"
    inFilenames[3]="./HW3_images/P2/convertible_2.raw"


    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel=3;
    int SizeH = 300;
    int SizeW = 500;
 
    // Allocate image data array
    unsigned char Imagedata[SizeH][SizeW][BytesPerPixel];
    Mat images[4];
    vector<vector<KeyPoint>> kps;
    Mat kpsMaps[4];
    Mat deses[4];
    

    for(int k=0;k<4;k++){
      // Read image (filename specified by first argument) into image data matrix
      if (!(file=fopen(inFilename[k]],"rb"))) {
        cout << "Cannot open file: " << inFilename1 <<endl;
        exit(1);
      }
      fread(Imagedata, sizeof(unsigned char), SizeH*SizeW*BytesPerPixel, file);
      fclose(file);
      //Create Opencv mat structure for image dimension. For 8 bit bayer, type should be CV_8UC1.
      image[k].create(SizeH, SizeW, CV_8UC3);
      for(int r=0;r<SizeH;r++){
        for(int c=0;c<SizeW;c++){
          unsigned char tmp=Imagedata[r][c][0];
          Imagedata[r][c][0]=Imagedata[r][c][2];
          Imagedata[r][c][2]=tmp;
        }
      }

      memcpy(image[k].data, Imagedata, SizeH*SizeW*BytesPerPixel);
      if( images[k]].empty() )
      { 
        cout<<"unable to read the image1"<<endl;
        return -1;
      }

      if(mode==0){
        cv::Ptr<SIFT> pDollar =SIFT::create(0, 3, 0.04, 10, 1.6);
        pDollar->detectAndCompute(images[k], noArray(),kps[k]],deses[k],false);
      }
      else{
        cv::Ptr<SURF> pDollar =SURF::create(300,4,3,false,false);
        pDollar->detectAndCompute(images[k], noArray(),kps[k]],deses[k],false);
      }
      drawKeypoints(images[k],kps[k])


    }


    ///////////////////////Feature Match////////////
    cv::Mat matchedImg;

    cv::Ptr<DescriptorMatcher> bf =BFMatcher::create(NORM_L2,true);
    vector<DMatch> matches;
    bf->match(des1,des2,matches,noArray());
    
    //we only want the best 10 pairs

    sort(matches.begin(),matches.end());
    matches.resize(10);
    drawMatches(image1,kps1,image2,kps2,matches,matchedImg);
    cv::imwrite(outFilename3,matchedImg);

 
      return 0;
}