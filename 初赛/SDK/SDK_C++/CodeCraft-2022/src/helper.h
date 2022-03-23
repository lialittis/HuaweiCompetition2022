#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;


struct Table{
  vector<string> names;
  vector<vector<string>> contents;
};

Table getNextLineAndSplitIntoTokens(string fname);
