
#include "simplex.hh"
#include <stdlib.h>
#include <iostream>

//private
////////////////////////////////////////////////////////////////////////////////

void simplex::Merge(unsigned int start, unsigned int end){
  if (end - start < 1){
    return;
  }else if (end - start == 1){
    if (this->simplex_fitness[end] < this->simplex_fitness[start]){
      double temp_fit;
      std::vector<double>* temp_vector;
      temp_fit = this->simplex_fitness[start];
      this->simplex_fitness[start] = this->simplex_fitness[end];
      this->simplex_fitness[end] = temp_fit;

      temp_vector = this->simplex_vectors[start];
      this->simplex_vectors[start] = this->simplex_vectors[end];
      this->simplex_vectors[end] = temp_vector;
    }
  }else{
    this->Merge(start, (end+start)/2);
    this->Merge((end+start)/2 + 1, end);
    std::vector<double> fit_storage((end-start)/2+1);
    std::vector< std::vector<double>* > vector_storage((end-start)/2+1);
    unsigned int lead_1 = 0, lead_2 = (end+start)/2+1, placement = start;
    for (int i = start; i < lead_2; ++i){
      fit_storage[i - start] = this->simplex_fitness[i];
      vector_storage[i - start] = this->simplex_vectors[i];
    }

    while (lead_1 < fit_storage.size()){
      if (lead_2 > end){
	for (int i = placement; i <= end; ++i){
	  this->simplex_fitness[i] = fit_storage[lead_1];
	  this->simplex_vectors[i] = vector_storage[lead_1];
	  vector_storage[lead_1] = NULL;
	  lead_1++;
	}
	break;
      }

      if (fit_storage[lead_1] < this->simplex_fitness[lead_2]){
	this->simplex_fitness[placement] = fit_storage[lead_1];
	this->simplex_vectors[placement] = vector_storage[lead_1];
	vector_storage[lead_1] = NULL;
	lead_1++;
      }else{
	this->simplex_fitness[placement] = this->simplex_fitness[lead_2];
	this->simplex_vectors[placement] = this->simplex_vectors[lead_2];
	lead_2++;
      }
      placement++;
    }
  }
}

void simplex::SortSimplex(){

  // Merge sort implimented
  this->Merge(0,this->size);
  
}

void simplex::Centroid(){

  for (int i = 0; i < this->centroid.size(); ++i){
    this->centroid[i] = 0;
    for (int v = 0; v < this->simplex_vectors.size()-1; ++v){
      this->centroid[i] += (*this->simplex_vectors[v])[i]/(this->size);
    }
  }
  
}

double simplex::Reflection(){

  for (int i = 0; i < reflection.size(); ++i){
    reflection[i] = (1+this->alpha)*this->centroid[i] - this->alpha*(*this->simplex_vectors[this->simplex_vectors.size()-1])[i];
  }

  return this->fit(&reflection);
  
}

double simplex::Expansion(){

  for (int i = 0; i < expansion.size(); ++i){
    expansion[i] = (1+this->beta)*this->centroid[i] - this->beta*(*this->simplex_vectors[this->simplex_vectors.size()-1])[i];
  }

  return this->fit(&expansion);
  
}

double simplex::Contraction(){

  for (int i = 0; i < contraction.size(); ++i){
    contraction[i] = (1-this->delta)*this->centroid[i] + this->delta*(*this->simplex_vectors[this->simplex_vectors.size()-1])[i];
  }

  return this->fit(&contraction);
  
}

double simplex::Random(unsigned int vec){
  
  for (int v = 0; v < simplex_vectors[vec]->size(); ++v){
    (*this->simplex_vectors[vec])[v] = this->min + (this->max-this->min)*(rand() % 1000 + 0.5)/1000.0;
  }
  this->simplex_fitness[vec] = this->fit(this->simplex_vectors[vec]);

  return this->simplex_fitness[vec];
  
}

void simplex::Shrink(){

  for (int i = 1; i < this->simplex_vectors.size(); ++i){
    for (int v = 0; v < this->simplex_vectors[i]->size(); ++v){
      (*this->simplex_vectors[i])[v] = this->gamma*(*this->simplex_vectors[i])[v] + (1-this->gamma)*(*this->simplex_vectors[0])[v];
    }
    this->simplex_fitness[i] = this->fit(this->simplex_vectors[i]);
  }

}

void Print(std::vector<double>* X){
  for (int i = 0; i < X->size(); ++i){
    std::cout << (*X)[i] << ", ";
  }
  std::cout << std::endl;
}
//public
////////////////////////////////////////////////////////////////////////////////
simplex::simplex(double (*f)(std::vector<double>*), unsigned int _size, double _min, double _max){
  
  this->alpha = 1;
  this->beta = 2;
  this->delta = 0.5;
  this->gamma = 0.5;

  this->fit = f;

  this->size = _size;
  this->min = _min;
  this->max = _max;
  this->simplex_vectors.resize(this->size+1);
  this->simplex_fitness.resize(this->size+1);
  this->centroid.resize(this->size);
  this->reflection.resize(this->size);
  this->expansion.resize(this->size);
  this->contraction.resize(this->size);

  if (max < min){throw 1;}
  
  for (int i = 0; i < simplex_vectors.size(); ++i){
    this->simplex_vectors[i] = new std::vector<double>;
    this->simplex_vectors[i]->resize(this->size);
    this->Random(i);
  }

  std::cout << "simplex fitness" << std::endl;
  Print(&this->simplex_fitness);
  
  this->SortSimplex();

  Print(&this->simplex_fitness);
}

void simplex::SetItterationParameters(double _alpha, double _beta, double _delta, double _gamma){
  this->alpha = _alpha;
  this->beta = _beta;
  this->delta = _delta;
  this->gamma = _gamma;
}

double simplex::Step(){

  double reflection_fit, expansion_fit, contraction_fit;

  this->SortSimplex();
  Print(&this->simplex_fitness);  

  this->Centroid();

  reflection_fit = this->Reflection();
  if (reflection_fit < simplex_fitness[0]){
    expansion_fit = this->Expansion();

    if (expansion_fit < reflection_fit){
      for (int i = 0; i < simplex_vectors[this->size]->size(); ++i){
	(*simplex_vectors[this->size])[i] = expansion[i];
      }
      simplex_fitness[this->size] = expansion_fit;
      this->SortSimplex();
      return simplex_fitness[0];
    }
  }

  if (reflection_fit < simplex_fitness[this->size]){
    for (int i = 0; i < simplex_vectors[this->size]->size(); ++i){
      (*simplex_vectors[this->size])[i] = reflection[i];
    }
    simplex_fitness[this->size] = reflection_fit;
    this->SortSimplex();
    return simplex_fitness[0];    
  }

  contraction_fit = this->Contraction();

  if (contraction_fit < simplex_fitness[0]){
    this->Shrink();
    this->SortSimplex();
    return simplex_fitness[0];
  }
  
  if (contraction_fit < simplex_fitness[this->size]){
    for (int i = 0; i < simplex_vectors[this->size]->size(); ++i){
      (*simplex_vectors[this->size])[i] = contraction[i];
    }
    simplex_fitness[this->size] = contraction_fit;
    this->SortSimplex();
    return simplex_fitness[0];    
  }

  this->Random(this->size);
  this->SortSimplex();
  return simplex_fitness[0];
  
}

std::vector<double> simplex::GetBestVector(){
  std::vector<double> ret_vector(this->size);
  for (int i = 0; i < ret_vector.size(); ++i){
    ret_vector[i] = (*this->simplex_vectors[0])[i];
  }
  return ret_vector;
}

double simplex::GetBestFitness(){
  return simplex_fitness[0];
}

