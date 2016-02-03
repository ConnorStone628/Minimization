#include "simplex.hh"
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>

int n = 0;

double f(std::vector<double>* X){
  double total = 0;
  n += 1;
  
  for (int i = 0; i < X->size(); ++i){
    total += (*X)[i]*(*X)[i];
  }

  return total;
}


int main(){

  srand(time(NULL));
  
  simplex S(f, 10, -100, 100);

  std::vector<double> V = S.GetBestVector();

  S.SetItterationParameters(1,2,0.5,0.5);
  
  // std::cout << "best vector:" << std::endl;
  
  // for (int i = 0; i < V.size(); ++i){
  //   std::cout << V[i] << ", ";
  // }
  // std::cout << std::endl;
  
  for (int i = 0; i < 100; ++i){
    std::cout << S.Step() << std::endl;
  }

  V = S.GetBestVector();

  // std::cout << "best vector:" << std::endl;
  
  // for (int i = 0; i < V.size(); ++i){
  //   std::cout << V[i] << ", ";
  // }
  // std::cout << std::endl;

  std::cout << "number of evaluations: " << n << std::endl;


  
  return 0;
}
