#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace cv;
using namespace std;
Mat src, src_gray;
Mat dst, detected_edges;
int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;

const char* window_name = "Edge Map";
static void CannyThreshold(int, void*)
{
    blur( src_gray, detected_edges, Size(3,3) );
    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
    dst = Scalar::all(0);
    src.copyTo( dst, detected_edges);
    imshow( window_name, dst );
}
int main( int argc, char** argv )
{

  if(argc<2){
    cout<<"insufficient argument: program inFilename outFilename"<<endl;
  }


  // Define file pointer and variables
  FILE *file;
  int BytesPerPixel=3;
  int SizeH = 321;
  int SizeW = 481;
 
  // Allocate image data array
  unsigned char Imagedata[SizeH][SizeW][BytesPerPixel];

  // Read image (filename specified by first argument) into image data matrix
  if (!(file=fopen(argv[1],"rb"))) {
    cout << "Cannot open file: " << argv[1] <<endl;
    exit(1);
  }
  fread(Imagedata, sizeof(unsigned char), SizeH*SizeW*BytesPerPixel, file);
  fclose(file);
  //Create Opencv mat structure for image dimension. For 8 bit bayer, type should be CV_8UC1.
  src.create(SizeH, SizeW, CV_8UC3);
  for(int r=0;r<SizeH;r++){
    for(int c=0;c<SizeW;c++){
      unsigned char tmp=Imagedata[r][c][0];
      Imagedata[r][c][0]=Imagedata[r][c][2];
      Imagedata[r][c][2]=tmp;
    }
  }

  memcpy(src.data, Imagedata, SizeH*SizeW*BytesPerPixel);
  if( src.empty() )
    { 
      cout<<"unable to read the image"<<endl;
      return -1;
    }
  dst.create( src.size(), src.type() );
  cvtColor( src, src_gray, COLOR_RGB2GRAY );
  namedWindow( window_name, WINDOW_AUTOSIZE );
  createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
  CannyThreshold(0, 0);

  while(true){
    if(waitKey(0)=='x'){
      string outFilename=string(argv[2]);
      outFilename=string(outFilename)+to_string(lowThreshold)+".jpg";
      cout<<"wring to Image "<<outFilename<<endl;
      Mat dst_gray;
      cvtColor(dst,dst_gray,COLOR_RGB2GRAY);
      imwrite(outFilename, 255-dst_gray);
    }
    else if(waitKey(0)=='q'){
      exit(1);
    }
  }


  return 0;
}