//============================================================================
// Name        : Dip1.cpp
// Author      : Ronny Haensch
// Version     : 2.0
// Copyright   : -
// Description : 
//============================================================================

#include "Dip1.h"

// function that performs some kind of (simple) image processing
/*
img	input image
return	output image
*/
Mat Dip1::doSomethingThatMyTutorIsGonnaLike(Mat& img){
  
	// TO DO !!!
    int nRows=img.rows;
    int nCols=img.cols;
    
    Mat output = Mat::zeros(nRows, nCols, CV_8UC1);
    cvtColor(img,output,COLOR_RGB2GRAY);
    
    
    Mat edgeMat = Mat::zeros(nRows, nCols, CV_8UC1);
    findEdges(output, edgeMat, 74); //last parameter = threshold

    
    return edgeMat;

}

void Dip1::findEdges(Mat& inputMat, Mat& outputMat, int threshold){
    int inputMatRows=inputMat.rows;
    int inputMatCols=inputMat.cols;
    int outputMatRows=outputMat.rows;
    int outputMatCols=outputMat.cols;
    
    if((inputMatRows!=outputMatRows) || (inputMatCols!=outputMatCols)){
        printf("Dip1::findEdges(): dimensions of in and output matrix are not equal.\n");
        return;
    }
    
    
    for(int y = 0; y < inputMatRows; ++y){
        for (int x = 0; x < inputMatCols; ++x){
            if(isNeighbourGradientExceedThreshold(inputMat, x, y, threshold)){
                outputMat.at<uchar>(y, x)=255;
            }
        }
    }
}

bool Dip1::isNeighbourGradientExceedThreshold(Mat& inputMat, int x, int y, int threshold){
    int value = inputMat.at<uchar>(y, x);
    
    
    if (y>0){
        int topNeighbourValue=inputMat.at<uchar>(y-1, x);
        if(abs(value-topNeighbourValue)>threshold){
            return true;
        }
    }
    if (y<(inputMat.rows-1)){
        int bottomNeighbourValue=inputMat.at<uchar>(y+1, x);
        if(abs(value-bottomNeighbourValue)>threshold){
            return true;
        }
    }

    if (x>0){
        int leftNeighbourValue=inputMat.at<uchar>(y, x-1);
        if(abs(value-leftNeighbourValue)>threshold){
            return true;
        }
    }
    if (x<(inputMat.cols-1)){
        int rightNeighbourValue=inputMat.at<uchar>(y, x+1);
        if(abs(value-rightNeighbourValue)>threshold){
            return true;
        }
    }
    
    
    return false;
}



/* *****************************
  GIVEN FUNCTIONS
***************************** */

// function loads input image, calls processing function, and saves result
/*
fname	path to input image
*/
void Dip1::run(string fname){

	// window names
	string win1 = string ("Original image");
	string win2 = string ("Result");
  
	// some images
	Mat inputImage, outputImage;
  
	// load image
	cout << "load image" << endl;
	inputImage = imread( fname );
	cout << "done" << endl;
	
	// check if image can be loaded
	if (!inputImage.data){
	    cout << "ERROR: Cannot read file " << fname << endl;
		cout << "Continue with pressing enter..." << endl;
	    cin.get();
	    exit(-1);
	}

	// show input image
	namedWindow( win1.c_str() );
	imshow( win1.c_str(), inputImage );
	
	// do something (reasonable!)
	outputImage = doSomethingThatMyTutorIsGonnaLike( inputImage );
	
	// show result
	namedWindow( win2.c_str() );
	imshow( win2.c_str(), outputImage );
	
	// save result
	imwrite("result.jpg", outputImage);
	
	// wait a bit
	waitKey(0);

}

// function loads input image and calls processing function
// output is tested on "correctness" 
/*
fname	path to input image
*/
void Dip1::test(string fname){

	// some image variables
	Mat inputImage, outputImage;
  
	// load image
	inputImage = imread( fname );

	// check if image can be loaded
	if (!inputImage.data){
	    cout << "ERROR: Cannot read file " << fname << endl;
	    cout << "Continue with pressing enter..." << endl;
	    cin.get();
	    exit(-1);
	}

	// create output
	outputImage = doSomethingThatMyTutorIsGonnaLike( inputImage );
	// test output
	test_doSomethingThatMyTutorIsGonnaLike(inputImage, outputImage);
	
}

// function loads input image and calls processing function
// output is tested on "correctness" 
/*
inputImage	input image as used by doSomethingThatMyTutorIsGonnaLike()
outputImage	output image as created by doSomethingThatMyTutorIsGonnaLike()
*/
void Dip1::test_doSomethingThatMyTutorIsGonnaLike(Mat& inputImage, Mat& outputImage){

	// ensure that input and output have equal number of channels
	if ( (inputImage.channels() == 3) and (outputImage.channels() == 1) )
		cvtColor(inputImage, inputImage, 7); //was CV_BGR2GRAY. alternative: COLOR_RGB2GRAY in 3.0

	// split (multi-channel) image into planes
	vector<Mat> inputPlanes, outputPlanes;
	split( inputImage, inputPlanes );
	split( outputImage, outputPlanes );

	// number of planes (1=grayscale, 3=color)
	int numOfPlanes = (int)inputPlanes.size();

	// calculate and compare image histograms for each plane
	Mat inputHist, outputHist;
	// number of bins
	int histSize = 100;
	float range[] = { 0, 256 } ;
	const float* histRange = { range };
	bool uniform = true; bool accumulate = false;
	double sim = 0;
	for(int p = 0; p < numOfPlanes; p++){
		// calculate histogram
		calcHist( &inputPlanes[p], 1, 0, Mat(), inputHist, 1, &histSize, &histRange, uniform, accumulate );
		calcHist( &outputPlanes[p], 1, 0, Mat(), outputHist, 1, &histSize, &histRange, uniform, accumulate );
		// normalize
		inputHist = inputHist / sum(inputHist).val[0];
		outputHist = outputHist / sum(outputHist).val[0];
		// similarity as histogram intersection
		sim += compareHist(inputHist, outputHist, 2); //was CV_COMP_INTERSECT
	}
	sim /= numOfPlanes;

	// check whether images are to similar after transformation
	if (sim >= 0.8)
		cout << "The input and output image seem to be quite similar (similarity = " << sim << " ). Are you sure your tutor is gonna like your work?" << endl;

}
