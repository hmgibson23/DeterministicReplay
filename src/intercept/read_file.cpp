#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <climits>
#include <cassert>
#include <cstdlib>


extern "C"
int* get_vector(char* filename)
{


  std::vector<int> sched;
  std::string line;
    std::cout << filename << std::endl;
  std::ifstream input(filename);

  if(input) {
    getline(input, line);
  } else {
    std::cout << "No schedule found!" << std::endl;
      exit(1);
  }
  
  std::replace(line.begin(), line.end(), ',', ' ');

  std::istringstream iss(line);

  int conversion_result;
  
  while (iss >> conversion_result)
    {
      sched.push_back(conversion_result);
    }

  sched.push_back(INT_MAX);

  int *ret;
  ret = static_cast<int*> (std::malloc(sched.size() * sizeof(int)));

  for(unsigned int i = 0; i < sched.size(); ++i) {
    ret[i] = sched[i];
  }

  return ret;
 
}
