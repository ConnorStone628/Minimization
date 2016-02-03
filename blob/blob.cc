
#include "blob.hh"

blob::blob((double) (*fitness)(std::vector<double>*), unsigned int vector_size, unsigned int initial_population, double learning_rate){

  this->sample_points.resize(initial_population);
  this->sample_fitness.resize(initial_population);
  this->fit = fitness;
  srand(time(NULL));
  for (int i = 0; i < initial_population; ++i){
    this->sample_points[i].resize(vector_size);
    for (int x = 0; x < vector_size; ++x){
      this->sample_points[i][x] = 1 - 2*(rand() % 1000)/1000.0;
    }
    this->sample_fitness[i] = (*this->fit)(&this->sample_points[i]);
  }

  
  this->alpha = learning_rate;
  this->count = 1;
  this->fit_total = this->sample_fitness[0];
  this->position_centroid.resize(vector_size);
  this->weight_centroid.resize(vector_size);
  for (int i = 0; i < vector_size; ++i){
    this->position_centroid[i] = this->sample_points[0][i];
    this->weight_centroid[i] = this->sample_points[0][i];
  }

  for (int i = 1; i < initial_population; ++i){
    this->UpdateCentroid(&this->sample_points[i], this->sample_fitness[i]);
  }
  
}

void blob::UpdateCentroid(std::vector<double>* X, double fitness){

  this->count++;
  double fit_total_update = this->fit_total + fitness;
  for (int i = 0; i < this->position_centroid.size(); ++i){
    this->position_centroid[i] = (this->count-1)*this->position_centroid[i]/this->count + (*X)[i]/this->count;
    this->weight_centroid[i] = this->fit_total*this->weight_centroid[i]/fit_total_update + fitness*(*X)[i]/fit_total_update;
  }
  this->fit_total = fit_total_update;
}

void blob::SetLearningRate(double learning_rate){
  this->alpha = learning_rate;
}

double blob::Step(){

  std::vector<double> step(this->position_centroid.size());

  for (int i = 0; i < step.size(); ++i){
    step[i] = this->alpha*this->weight_centroid[i] + (1 - this->alpha)*this->position_centroid[i];
  }

  double newfit = (*this->fit)(&step);

  this->UpdateCentroid(&step, newfit);

  return GetBestFit();
  
}

double blob::GetBestFit(){

  double best = this->sample_fitness[0];
  
  for (int i = 1; i < this->sample_fitness.size(); ++i){
    if (best > this->sample_fitness[i]){
      best = this->sample_fitness[i];
    }
  }

  return best;
  
}
