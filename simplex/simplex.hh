
#include <vector>
#include <math.h>

class simplex{
private:

  double (*fit)(std::vector<double>*);

  std::vector< std::vector<double>* > simplex_vectors;

  std::vector<double> simplex_fitness, centroid, reflection, expansion, contraction;

  double max, min, alpha, beta, delta, gamma;

  unsigned int size;

  void SortSimplex();
  void Merge(unsigned int start, unsigned int end);
  void Heapify(unsigned int root = 0);
  
  void Centroid();

  double Reflection();

  double Expansion();

  double Contraction();

  void Shrink();

  double Random(unsigned int vec);
  
public:

  simplex(double (*f)(std::vector<double>*), unsigned int size, double _min = -1, double _max = 1);

  void SetItterationParameters(double _alpha = 1, double _beta = 2, double _delta = 0.5, double _gamma = 0.5);

  double Step();

  std::vector<double> GetBestVector();

  double GetBestFitness();
  
};
