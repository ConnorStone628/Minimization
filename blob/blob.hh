
#include <vector>
#include <cstdlib>
#include <time.h>

class blob{
private:

  double (*fit)(std::vector<double>*);

  std::vector<double> position_centroid;

  std::vector<double> weight_centroid;

  std::vector< std::vector<double> > sample_points;
  std::vector<double> sample_fitness;

  double alpha;

  static unsigned int count;
  static double fit_total;

  void UpdateCentroid(std::vector<double>* X, double fitness);
  
public:

  blob((double) (*fitness)(std::vector<double>*), unsigned int vector_size, unsigned int initial_population = 100, double learning_rate = 2);

  ~blob();

  void SetLearningRate(double learning_rate);

  double GetBestFit();
  
  double Step();
  
};
