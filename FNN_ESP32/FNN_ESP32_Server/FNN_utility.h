#ifndef FNN_UTILITY_H
#define FNN_UTILITY_H

#include <aifes.h>

#define FNN_LAYERS 4
#define DATASETS 1024
#define TEST_DATASET 126
#define FEATURES 15

  void printLoss(float loss);
  void error_handling_training(int8_t error_nr);
  void error_handling_inference(int8_t error_nr);

  void printResult_testData(float testY[TEST_DATASET] , float output_data[TEST_DATASET]);
  void printResult_trainingData(float trainY[DATASETS] , float output_data[DATASETS]);

//-----FNN configuration-----

  //FNN Structure with 15 neurons of input, two dense layer of 16 neurons each, an output layer with a single neuron
  extern uint32_t FNN_structure[FNN_LAYERS];

  //Train Data
  extern float trainX[DATASETS][FEATURES];
  extern uint16_t input_shape[];
  extern aitensor_t input_tensor;

  extern float trainY[DATASETS];
  extern uint16_t target_shape[];
  extern aitensor_t target_tensor;

  //Test Data
  extern float testX[TEST_DATASET][FEATURES];
  extern uint16_t test_input_shape[];
  extern aitensor_t test_input_tensor;

  extern float testY[TEST_DATASET];
  extern int16_t test_target_shape[];
  extern aitensor_t test_target_tensor;

  //Output
  extern float output_data[DATASETS];
  extern uint16_t output_shape[2];
  extern aitensor_t output_tensor;

#endif FNN_UTILITY_H