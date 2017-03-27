#include <opencv2/ximgproc.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/core/utility.hpp"
#include <iostream>
  
using namespace cv;
using namespace cv::ximgproc;
using namespace std;

Mat image;
Mat edges, dst;
int edgeTreshold;

const char* window_name = "Edge Map";
static void SEThreshold(int, void*)
{
    dst=1-edges>edgeTreshold/100.0;
    imshow( window_name, dst );
}

int main( int argc, char *argv[] )
{

  if(argc<4){
    cout<<"insufficient argument: program inFilename outFilename"<<endl;
  }

    char* inFilename = argv[1];
    char* outFilename = argv[2];
    double mode =atof(argv[3]);

    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel=3;
    int SizeH = 321;
    int SizeW = 481;
 
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
      cout<<"unable to read the image mode[0/threhold]"<<endl;
      return -1;
    }
    
    image.convertTo(image, cv::DataType<float>::type, 1/255.0);
    

    Ptr<ximgproc::StructuredEdgeDetection> pDollar =ximgproc::createStructuredEdgeDetection("./model.yml.gz");
    pDollar->detectEdges(image, edges);
    dst.create( image.size(), image.type() );


    if(mode != 0){
      namedWindow( window_name, WINDOW_AUTOSIZE );
      createTrackbar( "Min Threshold:", window_name, &edgeTreshold, 100, SEThreshold );
      SEThreshold(0,0);
      while(true){
        if(waitKey(0)=='x'){
          string outFilename=string(argv[2]);
          outFilename=string(outFilename)+to_string(edgeTreshold/100.0)+".jpg";
          cout<<"wring to Image "<<outFilename<<endl;
          imwrite(outFilename, 255*dst);
        }
        else if(waitKey(0)=='q'){
          exit(1);
        }
    }
    }
    else{
      edges=1-edges;
      //edges are black which is 0 
      cv::imwrite(outFilename, 255*edges);
    }
 
    return 0;
}

