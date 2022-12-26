// Developed by Dharrun Sriman CC 2022
#include <iostream>
#include <bits/stdc++.h>
#include <map>
#include <iterator>
#include <cmath>
#include <ctime>
using namespace std;

int train_test_split(double x[], double y[], vector <double> train_x, vector <double> train_y, vector <double> test_x, vector <double> test_y, int size,int actual_size)
{
  if(actual_size == size || size == 0) {cerr << "Invalid split size" << endl; exit(1);}
  for(int i = 0; i < size; i++)
  {
    train_x.push_back(x[i]);
    train_y.push_back(y[i]);
  }

  for(int j = size; j < actual_size; j++)
  {
    test_x.push_back(x[j]);
    test_y.push_back(y[j]);
  }

  return 0;
}

double init(double inp, double out)
{
  double random = (rand() % 4) + 1;
  return (((double) rand()/RAND_MAX) / sqrt(inp) / random);
}

auto create_architecture(double input_layer[], double first_layer, double output_layer,double set_size, int random_seed = 0)
{
  double layers[3] = {set_size,first_layer,output_layer};
  map <int,int> arch;
  int layer_size = sizeof(layers)/sizeof(layers[0]);
  arch.insert(make_pair(layers[0],layers[1]));
  arch.insert(make_pair(layers[1],layers[2]));
  map <int,int>::iterator itr;
  int num_1 = 0;
  for(itr = arch.begin(); itr != arch.end(); itr++)
  {
    num_1 += itr->first * itr->second;
  }
  int number_weights = num_1;
  double** weights = 0;
  int row = 0;
  int col = 0;
  for(int row_length = 2; row_length < 5; row_length++)
  {
      if(number_weights % row_length == 0)
    {
      row = row_length;
      col = number_weights/row_length;
    }
  }
  weights = new double*[row];
  for (int i = 0; i < row; i++)
  {
    weights[i] = new double[col];
    for (int j = 0; j < col; j++)
    {
      weights[i][j] = init(set_size,output_layer);
    }
  }
  return weights;
}

double sigmoid(double x)
{
  return (1/(1 + exp(-x)));
}

double sigmoid_prime(float s)
{
  return (s * (1 - s));
}

int main()
{
  srand(time(NULL));
  int set_size = 6;
  double x[set_size] = {1,2,3,4,5,6};
  vector <double> xs;
  double y[1] = {0.731}; // can be set to anything between 0-1
  vector <double> train_x,train_y,test_x,test_y;
  int actual_size = sizeof(x)/sizeof(x[0]);
  int split_size = 4;
  train_test_split(x,y,train_x,train_y,test_x,test_y,split_size,actual_size);
  int first_layer = 3;
  int output_layer = 1;
  int number_weights = (set_size*first_layer)+(first_layer*output_layer);
  int row = 0;
  int col = 0;
  for(int row_length = 2; row_length < 5; row_length++)
  {
    if(number_weights % row_length == 0)
    {
      row = row_length;
      col = number_weights/row_length;
    }
  }
  // declaring weights and seperating them
  double** weights = create_architecture(x,first_layer,output_layer,set_size,0);
  double output_weights[1][(first_layer*output_layer)];
  int num = 0;
  for(int max_row = (row - first_layer); max_row < row; max_row++)
  {
    output_weights[0][num] = weights[max_row][col-1];
    num += 1;
  }
  // reshaping the input weights
  int row_size = set_size;
  int col_size = (number_weights - (sizeof(output_weights)/sizeof(output_weights[0])))/set_size;
  double input_weights[row_size][col_size];

  for(int i = 0; i < row_size; i++)
  {
    for(int j = 0; j < col_size; j++)
    {
      input_weights[i][j] = weights[j][i];
      cout << input_weights[i][j] << ",";
      if(j == col_size-1)
      {
        cout << endl;
      }
    }
  }
  // determining the input values
  cout << endl;
  int num_col = set_size;
  double new_x[1][num_col];
  int num2 = 0;
  for(int j = 0; j < set_size; j++)
  {
    new_x[0][num2] = x[j];
    cout << new_x[0][num2] << ",";
    num2 += 1;
  }
  cout << endl;

  // dot product for first layer (3 values) // add loop here
  for(int i = 0; i < 50; i++)
  {

      vector <double> store;
      vector <double> value;

      int col_length_b = col_size;
      int col_length_a = col_size;
      int row_length_b = row_size;
      double output = 0;
      for(int current_col_b = 0; current_col_b < col_size; current_col_b++)
      {
        for(int current_row_b = 0; current_row_b < row_size; current_row_b++)
        {
          int current_col_a = current_row_b % row_size;
          output = new_x[0][current_col_a] * input_weights[current_row_b][current_col_b];
          value.push_back(output);
          if(current_row_b == row_length_b-1)
          {
            store.push_back(accumulate(value.begin(),value.end(),0.00000000));
            value.clear();
          }
        }
      }
      // sigmoid answer
      double sigma[store.size()][1];
      for(int i = 0; i < store.size(); i++)
      {
        sigma[i][0] = sigmoid(store.at(i));
      }

      // dot-product output

      vector <double> store1;
      vector <double> value1;

      int col_length_b1 = 3;
      int col_length_a1 = 1;
      int row_length_a1 = 3;
      int row_length_b1 = col_length_a1;

      double output1 = 0;
      for(int current_col_b1 = 0; current_col_b1 < col_length_b1; current_col_b1++)
      {
        int current_row_a = current_col_b1 % col_length_b1;
        output1 = sigma[current_row_a][0] * output_weights[0][current_col_b1];
        value1.push_back(output1);
        if(current_col_b1 == col_length_b-1)
        {
          store1.push_back(accumulate(value1.begin(),value1.end(),0.00000000));
          value1.clear();
        }
      }

      double pred_output;
      for(double const& i : store1)
      {
        pred_output = sigmoid(i);
      }
      cout << "predicted output: " << pred_output << endl;

      // backpropagation

      double l2_error = y[0] - pred_output;
      double l2_delta = l2_error * sigmoid_prime(pred_output);
      double l1_error[store.size()];
      double t_output_weights[(first_layer*output_layer)][1];
      for(int i = 0; i < (first_layer * output_layer); i++)
      {
        t_output_weights[i][0] = output_weights[0][i];
      }
      for(int i = 0; i < col_length_b; i++)
      {
        l1_error[i] = l2_delta * t_output_weights[i][0];
      }
      double l1_delta[store.size()];
      for(int j = 0; j < col_length_b; j++)
      {
        l1_delta[j] = l1_error[j] * sigmoid_prime(sigma[j][0]);
      }

      // update_weights

      double layer_1[store.size()][1];
      for(int i = 0; i < store.size(); i++)
      {
        layer_1[i][0] = sigma[i][0];
      }
      double input_reshape[set_size][1];
      for(int j = 0; j < set_size; j++)
      {
        input_reshape[j][0] = x[j];
      }
      double alpha = 1.00;
      double sum = 0;
      for(int i = 0; i < store.size(); i++)
      {
         sum += layer_1[i][0] * l2_delta;
      }

      for(int current_col = 0; current_col < (first_layer*output_layer); current_col++)
      {

          output_weights[0][current_col] = output_weights[0][current_col] + (alpha * sum);
      }

      double sum1 = 0;
      for(int i = 0; i < set_size; i++)
      {
        int j = i % col_length_b;
        sum1 += input_reshape[i][0] * l1_delta[j];
      }

      for(int current_row = 0; current_row < row_size; current_row++)
      {
        for(int current_col = 0; current_col < col_size; current_col++)
        {
          input_weights[current_row][current_col] = input_weights[current_row][current_col] + (alpha * sum1);

        }
      }
   }

}
