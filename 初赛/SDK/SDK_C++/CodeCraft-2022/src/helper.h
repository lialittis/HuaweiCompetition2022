#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;


struct Table{
  vector<string> names;
  vector<vector<string>> contents;
};

typedef struct{
  int *QosLim;
} Settings;

Table getNextLineAndSplitIntoTokens(string fname);

void readConfigFile(char *cfgfile, int&QosLim);
