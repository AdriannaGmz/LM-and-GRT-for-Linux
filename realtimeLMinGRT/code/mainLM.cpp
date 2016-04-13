/******************************************************************************\
* 2015-2016     CINVESTAV   -  D. Adriana Gómez Rosal.    All rights reserved. *
* Adriana Gómez proprietary and confidential. Not for distribution.. yet.      *
\******************************************************************************/
/******************************************************************************\
* This program saves LeapMotion data into a file.txt:
* Class_Label, followed by the 4 distances among the 5 fingers
* The purpose of saving this data is to fit it into the GRT incoming files
*
* Guarda el archivo dataFromLM.csv que genera featureVector para dos Labels, y en grtANBC se emplea éste archivo para clasificar
*   	/home/adriana/Documents/Dropbox/AsusLMDE/fromLMtoGRT/2processingDataInGRT
\******************************************************************************/

/******************************************************************************\
Getting Started.
In this version, we will use a simple ANBC to classify the feature vector provided by LM
\******************************************************************************/

#include <iostream>
#include <string>
#include "/home/adriana/Documents/grt/GRT/GRT.h"
#include "Leap.h"
#include "LMListener.h"
#include <stdio.h>		//from here, the following are for the kbhit implementation
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

using namespace Leap;
using namespace GRT;

int kbhit(void){	//this is only to allow the inf loop
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	if(ch != EOF)
	{
		ungetc(ch, stdin);
		return 1;
	}
	return 0;
}


int main(){
	// Create a controller
	Controller controller;

	//Creating the GRT Pipeline 
	GestureRecognitionPipeline pipeline;

	//Add a moving average filter (with a buffer size of 5 and for a 1 dimensional signal) as a pre-processing module
	//pipeline.addPreProcessingModule( MovingAverageFilter(5,1) );

	//Set the classifier at the core of the pipeline, Adaptive Naive Bayes Classifier
	pipeline.setClassifier( ANBC() );

	//Add a class label timeout filter to the end of the pipeline (with a timeout value of 1 second),
	//this will filter the predicted class output from the ANBC algorithm
	//pipeline.addPostProcessingModule( ClassLabelTimeoutFilter(1000) );

	//**************************RECORDING TRAINING DATA************************
//Comment from here
	//Create a new instance of the ClassificationData
	//ClassificationData trainingData;

	//Set the dimensionality of the data
	//trainingData.setNumDimensions( 3 );

	//Here you would grab some data from your sensor and label it with the corresponding gesture it belongs to
	//UINT gestureLabel = 1;
	//vector< double > sample(3);
	//sample[0] = //....Data from sensor
	//sample[1] = //....Data from sensor
	//sample[2] = //....Data from sensor

	//Add the sample to the training data
	//trainingData.addSample( gestureLabel, sample );
//Comment until here

    //Load some training data from a file
    ClassificationData trainingData;    
    cout << "Loading dataset..." << endl;
    if( !trainingData.load( "../data/dataFromLM.csv" ) )
    {        
        cout << "ERROR: Failed to load training data from file\n";
        return EXIT_FAILURE;
    }
    cout << "Data Loaded!" << endl;
    
    //Print out some stats about the training data
    trainingData.printStats();
    
    //Partition the training data into a training dataset and a test dataset.
    //80 means that 80% of the data will be used for the training data and 20% will be returned as the test dataset
    cout << "Splitting data into training/test split..." << endl;
    ClassificationData testData = trainingData.partition(80);

	//After recording your training data you can then save it to a file
	bool saveResult = trainingData.save( "../data/TrainingData.grt" );

	//This can then be loaded later
	bool loadResult = trainingData.load( "../data/TrainingData.grt" );

	//**************************TRAINING/LEARNING************************

	cout << "Training model..." << endl;
	//bool trainSuccess = pipeline.train( trainingData );  // trains data and checks if worked.. or the if:
    if( !pipeline.train( trainingData ) ){
        cout << "ERROR: Failed to train the pipeline!\n";
        return EXIT_FAILURE;
    }
    cout << "Model trained!" << endl;

    //Save the pipeline to a file   --optional
    if( !pipeline.save( "HelloWorldPipeline" ) ){
        cout << "ERROR: Failed to save the pipeline!\n";
        return EXIT_FAILURE;
    }

	//**************************TESTING THE RECOGNITION ACCURACY************************

    //Load the pipeline from a file 	
    if( !pipeline.load( "HelloWorldPipeline" ) ){
        cout << "ERROR: Failed to load the pipeline!\n";
        return EXIT_FAILURE;
    }
    //Test the pipeline using the test data
    cout << "Testing model..." << endl;
    if( !pipeline.test( testData ) ){
        cout << "ERROR: Failed to test the pipeline!\n";
        return EXIT_FAILURE;
    }










    //Test and Print some stats about the testing

	//Test the classification accuracy of the trained pipeline
	bool testSuccess = pipeline.test( testData );

	//You can then get then get the accuracy of how well the pipeline performed with the test data
	double accuracy = pipeline.getTestAccuracy();
	cout << "Test Accuracy: " << pipeline.getTestAccuracy() << endl;

	//Along with some other results such as the F-Measure, Precision and Recall
	//double fMeasure = pipeline.getTestFMeasure();		//Revisar esto.. dice q no se puede coinvertir
	//double precision = pipeline.getTestPrecision();
	//double recall = pipeline.getTestRecall();
    
    cout << "Precision: ";
    for(UINT k=0; k<pipeline.getNumClassesInModel(); k++){
        UINT classLabel = pipeline.getClassLabels()[k];
        cout << "\t" << pipeline.getTestPrecision(classLabel);
    }cout << endl;
    
    cout << "Recall: ";
    for(UINT k=0; k<pipeline.getNumClassesInModel(); k++){
        UINT classLabel = pipeline.getClassLabels()[k];
        cout << "\t" << pipeline.getTestRecall(classLabel);
    }cout << endl;
    
    cout << "FMeasure: ";
    for(UINT k=0; k<pipeline.getNumClassesInModel(); k++){
        UINT classLabel = pipeline.getClassLabels()[k];
        cout << "\t" << pipeline.getTestFMeasure(classLabel);
    }cout << endl;
    
    MatrixDouble confusionMatrix = pipeline.getTestConfusionMatrix();
    cout << "ConfusionMatrix: \n";
    for(UINT i=0; i<confusionMatrix.getNumRows(); i++){
        for(UINT j=0; j<confusionMatrix.getNumCols(); j++){
            cout << confusionMatrix[i][j] << "\t";
        }cout << endl;
    }

	////*****or WITH K FOLD*****  10 IS THE NUMBER OF FOLDS FOR THE CROSS-VALIDATION
	//Perform the prediction
	//bool trainSuccess = pipeline.train( trainingData, 10 );

	//You can then get then get the accuracy of how well the pipeline performed during the k-fold cross validation testing
	//double accuracy = pipeline.getCrossValidationAccuracy();



	//**************************CLASSIFICATING IN REAL TIME*************************


	//Keeps on loop until a key is pressed
	while( !kbhit() ) 		
	{
		if(controller.isServiceConnected()) 
		{
			// Get the latest frame
		    Frame frameMain = controller.frame(); 			
			Hand firstHand	= frameMain.hands()[0];
			if (firstHand.isValid())
				{	
					//Retrieves the finger pointables of the valid Frame
					PointableList pointerFingers = firstHand.pointables();

					//Generate the empty vector, ready to save the features
					vector< double > inputFeatureVector(4);

					// Save the features: 4 distances between the 5 fingers
					for (int i = 0; i < 4; i++)  
					{
						// Generates each feature
						float pointFingerDist = pointerFingers[i].tipPosition().distanceTo(pointerFingers[i+1].tipPosition());
						
						// Save the 4 features (distances among fingers)
						inputFeatureVector[i] = pointFingerDist;	
					}

					//GRT's job:
					//Perform the prediction
					bool predictionSuccess = pipeline.predict( inputFeatureVector );

					//You can then get the predicted class label from the pipeline
					UINT predictedClassLabel = pipeline.getPredictedClassLabel();

					//Along with some other results such as the likelihood of the most likely class or the likelihood of all the classes in the model
					double bestLoglikelihood = pipeline.getMaximumLikelihood();
					vector<double> classLikelihoods = pipeline.getClassLikelihoods();

					//You can then use the predicted class label to trigger the action associated with that gesture
					if( predictedClassLabel == 1 ){
					    //Trigger the action associated with gesture 1
					    cout << "Label 1, Extended Hand"<< endl;
					}
					else if( predictedClassLabel == 2 ){
					    //Trigger the action associated with gesture 2
					   	cout << "Label 2, Non-Extended Hand"<< endl;
					}
					else{
					   	cout << "Label 0, Gesture Not Recognized"<< endl;
					}

				} //Finishes the processing of a frame (feature extraction and prediction with pipeline)

			usleep(10000); // grabs a frame every 10 ms

		}	//Finishes when !controller.isServiceConnected()

	}	// kbhit. When a Key is pressed, comes to this point
	
	

	std::cout << "Pressed a Key to Exit." << std::endl;		

  return 0;
	//return EXIT_SUCCESS;	// GRT
}
