//
// Created by Sam on 2018/7/26.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>
#include <numeric>
#include "classifier.h"

#define LEFT_LABEL_IDX      0
#define KEEP_LABEL_IDX      1
#define RIGHT_LABEL_IDX     2
#define NOF_LABELS          3

#define S_IDX               0
#define D_IDX               1
#define S_RATE_IDX          2
#define D_RATE_IDX          3

#define D_FEATURE_IDX           0
#define RELATIVE_D_FEATURE_IDX  1
#define S_RATE_FEATURE_IDX      2
#define D_RATE_FEATURE_IDX      3
#define NOF_FEATURES            4

#define LANE_WIDTH          4

/**
 * Initializes GNB
 */
GNB::GNB()
 : mMeans(NOF_LABELS), mMu(NOF_LABELS), mPriorProb(NOF_LABELS) {
}

GNB::~GNB() {}

void GNB::train(vector<vector<double>> data, vector<string> labels)
{

  /*
      Trains the classifier with N data points and labels.

      INPUTS
      data - array of N observations
        - Each observation is a tuple with 4 values: s, d,
          s_dot and d_dot.
        - Example : [
                [3.5, 0.1, 5.9, -0.02],
                [8.0, -0.3, 3.0, 2.2],
                ...
            ]

      labels - array of N labels
        - Each label is one of "left", "keep", or "right".
  */
  CalculateGaussianDistribution(data, labels);
}

string GNB::predict(vector<double> sample)
{
  /*
      Once trained, this method is called and expected to return
      a predicted behavior for the given observation.

      INPUTS

      observation - a 4 tuple with s, d, s_dot, d_dot.
        - Example: [3.5, 0.1, 8.5, -0.2]

      OUTPUT

      A label representing the best guess of the classifier. Can
      be one of "left", "keep" or "right".
      """
      # TODO - complete this
  */

  return this->possible_labels[1];

}

void GNB::CalculateGaussianDistribution(const vector<vector<double> > &data,
    const vector<string> &labels) {
  size_t total_labels = labels.size();
  double nof_left_labels = 0;
  double nof_keep_labels = 0;
  double nof_right_labels = 0;
  vector<vector<double>> featuresOfLeft(NOF_FEATURES);
  vector<vector<double>> featuresOfKeep(NOF_FEATURES);
  vector<vector<double>> featuresOfRight(NOF_FEATURES);

  for (int i = 0; i < total_labels; i++) {
    double relative_d = (int)data[i][D_IDX] % LANE_WIDTH;
    if (labels[i] == "left") {
      featuresOfLeft[D_FEATURE_IDX].push_back(data[i][D_IDX]);
      featuresOfLeft[RELATIVE_D_FEATURE_IDX].push_back(relative_d);
      featuresOfLeft[S_RATE_FEATURE_IDX].push_back(data[i][S_RATE_IDX]);
      featuresOfLeft[D_RATE_FEATURE_IDX].push_back(data[i][D_RATE_IDX]);

      nof_left_labels++;
    }

    if (labels[i] == "keep") {
      featuresOfKeep[D_FEATURE_IDX].push_back(data[i][D_IDX]);
      featuresOfKeep[RELATIVE_D_FEATURE_IDX].push_back(relative_d);
      featuresOfKeep[S_RATE_FEATURE_IDX].push_back(data[i][S_RATE_IDX]);
      featuresOfKeep[D_RATE_FEATURE_IDX].push_back(data[i][D_RATE_IDX]);

      nof_keep_labels++;
    }

    if (labels[i] == "right") {
      featuresOfRight[D_FEATURE_IDX].push_back(data[i][D_IDX]);
      featuresOfRight[RELATIVE_D_FEATURE_IDX].push_back(relative_d);
      featuresOfRight[S_RATE_FEATURE_IDX].push_back(data[i][S_RATE_IDX]);
      featuresOfRight[D_RATE_FEATURE_IDX].push_back(data[i][D_RATE_IDX]);

      nof_right_labels++;
    }
  }

  mPriorProb[LEFT_LABEL_IDX] = nof_left_labels / total_labels;
  mPriorProb[KEEP_LABEL_IDX] = nof_keep_labels / total_labels;
  mPriorProb[RIGHT_LABEL_IDX] = nof_right_labels / total_labels;


  CalculateDistribution(featuresOfLeft, LEFT_LABEL_IDX);
  CalculateDistribution(featuresOfKeep, KEEP_LABEL_IDX);
  CalculateDistribution(featuresOfRight, RIGHT_LABEL_IDX);
}

void GNB::CalculateDistribution(const vector<vector<double>>& features,
    int label) {
  double mean;
  double mu;

  for (int idx = 0; idx < NOF_FEATURES; idx++) {
    size_t feature_size = features[idx].size();
    mean = accumulate(features[idx].begin(), features[idx].end(), 0.0) / feature_size;
    mMeans[label].emplace_back(mean);

    vector<double> diff(feature_size);
    transform(features[idx].begin(), features[idx].end(), diff.begin(),
        [mean](double x_i){return x_i - mean;});
    mu = inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);

//    for (const double& x_i : features[idx]) {
//      mu += pow((x_i - mean), 2);
//    }

    mMu[label].emplace_back(sqrt(mu/feature_size));
  }
}