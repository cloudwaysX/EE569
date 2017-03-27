#include <opencv2/xfeatures2d.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>
#include <iostream>
#include <algorithm>
  
using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;
using namespace cv::ml;

int main( int argc, char *argv[] )
{
    char** inFilenames = new char*[4];
    inFilenames[0]=(char*)"./HW3_images/P2/truck.raw";
    inFilenames[1]=(char*)"./HW3_images/P2/suv.raw";
    inFilenames[2]=(char*)"./HW3_images/P2/convertible_1.raw";
    inFilenames[3]=(char*)"./HW3_images/P2/convertible_2.raw";

    string inFilenames2[4];
    inFilenames2[0]="./HW3_images/P2/truck.raw";
    inFilenames2[1]="./HW3_images/P2/suv.raw";
    inFilenames2[2]="./HW3_images/P2/convertible_1.raw";
    inFilenames2[3]="./HW3_images/P2/convertible_2.raw";


    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel=3;
    int SizeH = 300;
    int SizeW = 500;
 
    // Allocate image data array
    unsigned char Imagedata[SizeH][SizeW][BytesPerPixel];
    vector<Mat> images;
    vector<Mat> grayes;
    vector<KeyPoint> kps;
    //Mat kpsMaps[4];
    Mat des;
    Mat featuresUnclustered;

    
    cv::Ptr<SIFT> pDollar_sift =SIFT::create();

    for(int k=0;k<4;k++){
      // Read image (filename specified by first argument) into image data matrix
      if (!(file=fopen(inFilenames[k],"rb"))) {
        cout << "Cannot open file: " << inFilenames[k] <<endl;
        exit(1);
      }
      fread(Imagedata, sizeof(unsigned char), SizeH*SizeW*BytesPerPixel, file);
      fclose(file);
      //Create Opencv mat structure for image dimension. For 8 bit bayer, type should be CV_8UC1
      Mat image;
      image.create(SizeH,SizeW,CV_8UC3);
      for(int r=0;r<SizeH;r++){
        for(int c=0;c<SizeW;c++){
          unsigned char tmp=Imagedata[r][c][0];
          Imagedata[r][c][0]=Imagedata[r][c][2];
          Imagedata[r][c][2]=tmp;
        }
      }

      memcpy(image.data,Imagedata, SizeH*SizeW*BytesPerPixel);


      if( image.empty() )
      { 
        cout<<"unable to read the image1"<<endl;
        return -1;
      }
      Mat gray;
      cvtColor(image,gray,COLOR_BGR2GRAY);
      grayes.push_back(gray);

      //cv::imwrite("gray"+to_string(k)+".jpg", grayes[k]);
      if(k<3){
        pDollar_sift->detect(gray,kps);
        pDollar_sift->compute(gray,kps,des);
        featuresUnclustered.push_back(des);
      }
    }
    ///////////////////////////
    int clusterCount=8;
    TermCriteria tc(CV_TERMCRIT_ITER,100,0.001);
    int attemps=1;
    int flags=KMEANS_PP_CENTERS;
    BOWKMeansTrainer bowTrainer(clusterCount,tc,attemps,flags);
    Mat dictionary=bowTrainer.cluster(featuresUnclustered); 
    //store the vocabulary
    FileStorage fs("dictionary_kmeans.yml", FileStorage::WRITE);
    fs << "vocabulary" << dictionary;
    fs.release();


 ///////////////////////Feature Match////////////
    //create a nearest neighbor matcher
    Ptr<DescriptorMatcher> matcher = FlannBasedMatcher::create();
    //create Sift feature point extracter
    Ptr<FeatureDetector> detector = SiftFeatureDetector::create();
    //create Sift descriptor extractor
    Ptr<DescriptorExtractor> extractor = SiftDescriptorExtractor::create();
    //create BoF (or BoW) descriptor extractor
    BOWImgDescriptorExtractor bowDE(extractor,matcher);
    //Set the dictionary with the vocabulary we created in the first step
    bowDE.setVocabulary(dictionary);

    vector<Mat> deses_BoW;
    for(int i=0;i<4;i++){
      vector<KeyPoint> kps3;
      Mat descriptor3;
      detector->detect(grayes[i],kps3);
      bowDE.compute(grayes[i],kps3,descriptor3);
      deses_BoW.push_back(descriptor3);
      for(int i=0;i<8;i++){
        cout<<descriptor3.at<float>(0,i)<<" ";
      }
      cout<<endl;
    }

    float minD=100;
    int minI=-1;
    for(int i=0;i<3;i++){
      float D=norm(deses_BoW[i],deses_BoW[3],NORM_L2,noArray());
      cout<<D<<endl;
      if(minD>D){
        minD=D;
        minI=i;
      }
    }

    cout<<"convertible_2 should be classified to "<<inFilenames2[minI]<<endl;    


 
    return 0;
}