#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include <opencv2/ximgproc.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/core/utility.hpp"
  
using namespace cv;
using namespace cv::ximgproc;

const char* keys =
{
    "{i || input image name}"
    "{m || model name}"
    "{o || output image name}"
};

int main( int argc, char *argv[] )
{
  bool printHelp = ( argc == 1 );
  printHelp = printHelp || ( argc == 2 && std::string(argv[1]) == "--help" );
  printHelp = printHelp || ( argc == 2 && std::string(argv[1]) == "-h" );

  if ( printHelp )
  {
    cv::CommandLineParser parser(argc, argv, keys);
    if ( !parser.check() )
    {
        parser.printErrors();
        return -1;
    }
    
    char* modelFilename = argv[1];
    char* inFilename = argv[2];
    char* outFilename = argv[3];

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
      cout<<"unable to read the image"<<endl;
      return -1;
    }
    
    image.convertTo(image, cv::DataType<float>::type, 1/255.0);
    cv::Mat edges(image.size(), image.type());
    
    cv::Ptr<StructuredEdgeDetection> pDollar =createStructuredEdgeDetection(modelFilename);
    pDollar->detectEdges(image, edges);
 
    if ( outFilename == "" )
    {
      cv::namedWindow("edges", 1);
      cv::imshow("edges", edges);
 
      cv::waitKey(0);
    }
    else
      cv::imwrite(outFilename, 255*edges);
 
      return 0;
}