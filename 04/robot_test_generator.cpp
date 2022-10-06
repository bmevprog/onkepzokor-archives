#include <fstream>
#include <iostream>

using namespace std;

int main() {

  int case_num = 0;
  int max_size = 20;
  int max_wait = 100;
  int min_wait = 0;

  for (int size = 1; size <= max_size; ++size) {
    ++case_num;
    auto target = "./tests/generated/test_" + to_string(case_num) + ".in";
    cout << target << endl;
    std::ofstream out(target);
    out << size << endl;
    for (int j = 0; j < 2; ++j)
      for (int i = 0; i < size; ++i) {
        int rand_num = rand() % (max_wait - min_wait + 1) + min_wait;
        out << rand_num << ((i == (size - 1)) ? "\n" : " ");
      }
  }

  return 0;
}