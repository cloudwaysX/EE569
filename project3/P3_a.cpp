#include <opencv2/xfeatures2d.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/utility.hpp"
#include <iostream>
  
using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

int main( int argc, char *argv[] )
{

  if(argc<4){
    cout<<"insufficient argument: program inFilename outFilename mode"<<endl;
  }

    char* inFilename = argv[1];
    char* outFilename = argv[2];
    int mode =atoi(argv[3]);

    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel=3;
    int SizeH = 300;
    int SizeW = 500;
 
    // Allocate image data array
    unsigned char Imagedata[SizeH][SizeW][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(inFilename,"rb"))) {
      cout << "Cannot open file: " << inFilename <<endl;
      exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), SizeH*SizeW*BytesPerPixel, file);
    fclose(file);
    //Create Opencv mat structure for image dimension. For 8 bit bayer, type should be CV_8UC1.
    cv::Mat image;
    image.create(SizeH, SizeW, CV_8UC3);
    for(int r=0;r<SizeH;r++){
      for(int c=0;c<SizeW;c++){
        unsigned char tmp=Imagedata[r][c][0];
        Imagedata[r][c][0]=Imagedata[r][c][2];
        Imagedata[r][c][2]=tmp;
      }
    }

    memcpy(image.data, Imagedata, SizeH*SizeW*BytesPerPixel);
    if( image.empty() )
    { 
      cout<<"unable to read the image"<<endl;
      return -1;
    }

    Mat gray;
    cvtColor( image, gray, COLOR_BGR2GRAY );
    
    vector<KeyPoint> kps;
    cv::Mat kpsMap;
    
    if(mode==0){
      cv::Ptr<SIFT> pDollar =SIFT::create(0, 3, 0.04, 10, 1.6);
      pDollar->detect(gray, kps);
    }
    else{
      cv::Ptr<SURF> pDollar =SURF::create(600,4,3,false,false);
      pDollar->detect(gray, kps);
    }

    drawKeypoints(image,kps,kpsMap);

 
    if ( outFilename == "" )
    {
      cv::namedWindow("kpsMap", 1);
      cv::imshow("kpsMap", kpsMap);
 
      cv::waitKey(0);
    }
    else
      cv::imwrite(outFilename, kpsMap);
 
      return 0;
}