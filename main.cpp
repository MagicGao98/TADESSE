#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <numeric>

#include "de.h"

#define NUM_RUN 30

double *OShift, *M, *y, *z, *x_bound;
int ini_flag = 0, n_flag, func_flag, *SS;

int g_function_number;
int g_problem_size;
unsigned int g_max_num_evaluations;
double g_optimum[12] = {300, 400, 600, 800, 900, 1800, 2000, 2200, 2300, 2400, 2600, 2700};

int g_pop_size;
double g_arc_rate;
int g_memory_size;
double g_p_best_rate;

double random_seeds[1000];
double fitness_memory[NUM_RUN][17];

double run_time;

using namespace std;

void read_random_seed(double *seed_list, int seed_num);
void print_results(string file_name);

void calstdev(vector<double> A, double &best, double &mean, double&stdev)
{
	double sum = std::accumulate(std::begin(A), std::end(A), 0.0);
    best = *min_element(std::begin(A), std::end(A));
	mean = sum / A.size(); //均�? 

	double accum = 0.0;
	std::for_each(std::begin(A), std::end(A), [&](const double d) {
		accum += (d - mean)*(d - mean);
	});

	stdev = sqrt(accum / (A.size())); //标准�?
}

int main(int argc, char **argv)
{
  //dimension size. please select from 10, 20
  g_problem_size = 20;
  //available number of fitness evaluations
  if (g_problem_size == 10)
  {
    g_max_num_evaluations = 200000;
  }
  else if (g_problem_size == 20)
  {
    g_max_num_evaluations = 1000000;
  }

  //L-SHADE parameters
  g_pop_size = 18 * g_problem_size;
  g_memory_size = 10;
  g_arc_rate = 1;
  g_p_best_rate = 0.11;

  // read random seeds from file
  read_random_seed(random_seeds, 1000);


  for (int i = 0; i < 12; i++)
  {
    g_function_number = i + 1;
    cout << "\n-------------------------------------------------------" << endl;
    cout << "Function = " << g_function_number << ", Dimension size = " << g_problem_size << "\n"
         << endl;

    std::vector<Fitness> bsf_fitness_array(NUM_RUN);
    Fitness mean_bsf_fitness = 0;
    Fitness std_bsf_fitness = 0;

    for (int j = 0; j < NUM_RUN; j++)
    {
      run_time = j + 1;
      //random seed is selected based on time according to competition rules
      int seed_ind = (g_problem_size / 10 * g_function_number * NUM_RUN + j) - NUM_RUN;
      seed_ind %= 1000;
      srand(random_seeds[seed_ind]);

      searchAlgorithm *alg = new TADESSE();
      
      bsf_fitness_array[j] = alg->run(&fitness_memory[j][0]);
      cout << j + 1 << "th run, "
           << "error value = " << bsf_fitness_array[j] << endl;
    }

    for (int j = 0; j < NUM_RUN; j++)
      mean_bsf_fitness += bsf_fitness_array[j];
    mean_bsf_fitness /= NUM_RUN;

    for (int j = 0; j < NUM_RUN; j++)
      std_bsf_fitness += pow((mean_bsf_fitness - bsf_fitness_array[j]), 2.0);
    std_bsf_fitness /= NUM_RUN;
    std_bsf_fitness = sqrt(std_bsf_fitness);

    double best, mean, stdv;
    calstdev(bsf_fitness_array, best, mean, stdv);
    cout << "\nmean = " << mean_bsf_fitness << ", std = " << std_bsf_fitness << endl;

  }

  return 0;
}

void read_random_seed(double *seed_list, int seed_num)
{
  string file_path = "input_data/Rand_Seeds.txt";
  ifstream fin(file_path);

  if (fin.is_open())
  {
    for (int i = 0; i < seed_num; i++)
    {
      fin >> seed_list[i];
    }

    cout << "Random seeds imported successfully!" << endl;
  }
  else
  {
    cout << "Error! cannot onpen random_seeds file." << endl;
  }

  fin.close();
}

void print_results(string file_path)
{
  ofstream fout(file_path);
  if (fout.is_open())
  {
    fout << scientific << setprecision(6);
    for (int i = 0; i < 16; i++)
    {
      for (int j = 0; j < NUM_RUN; j++)
      {
        fout << fitness_memory[j][i] << ",";
      }
      fout << endl;
    }
  }
  else
  {
    cout << "Error! cannot onpen output file " << file_path << "." << endl;
  }

  fout.close();
}
