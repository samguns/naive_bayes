//
// Created by Sam on 2018/7/26.
//

#ifndef NAIVE_BAYES_CLASSIFIER_H
#define NAIVE_BAYES_CLASSIFIER_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;

class GNB {
 public:

  vector<string> possible_labels = {"left","keep","right"};


  /**
    * Constructor
    */
  GNB();

  /**
   * Destructor
   */
  virtual ~GNB();

  void train(vector<vector<double> > data, vector<string>  labels);

  string predict(vector<double>);

 private:
  vector<double> mMeans;
  vector<double> mMu;
};

#endif //NAIVE_BAYES_CLASSIFIER_H
