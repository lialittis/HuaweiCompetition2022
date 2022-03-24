#include <iostream>
#include <fstream>
#include "helper.h"

int main() {

  int M,N;
  int QosLim = 0;

  // read csv data
  // std::ifstream file();
  // demand.csv
  Table tableDemand = getNextLineAndSplitIntoTokens("../data/demand.csv");
  Table tableQos = getNextLineAndSplitIntoTokens("../data/qos.csv");
  Table tableSiteBW = getNextLineAndSplitIntoTokens("../data/site_bandwidth.csv");
  ifstream cFile ("../data/config.ini",ios::in);
  if(cFile.is_open()){
    string line;
    while(getline(cFile,line)){
      if(line[0] == '[' || line.empty())
        continue;
      auto delimiterPos = line.find("=");
      auto prop = line.substr(0,delimiterPos);
      QosLim = stoi(line.substr(delimiterPos+1));
    }
  }
  else
    cerr<<"Could not open config file.\n";

  M = tableDemand.names.size() - 1;
  N = tableQos.contents.size();

  // table to store the results
  vector<vector<int>> results(M,vector<int>(N,0));

  // output file
  ofstream resFile("../output/solution.txt");

  // time
  for(int t = 0; t < int(tableDemand.contents.size()); t++) {
    for(int i = 1; i < int(tableDemand.names.size()); i++){
      // assign the demand for client i at time t
      string name = tableDemand.names[i];
      // cout<<sizeof(name)<<endl;
      int demand = stoi(tableDemand.contents[t][i]);
      // loop all edge nodes
      for(int j = 0; j < int(tableQos.contents.size()); j++){
        if(stoi(tableQos.contents[j][i]) > QosLim) continue;
        // if there are enough space for client i
        if(results[i-1][j] + demand < stoi(tableSiteBW.contents[j][1])){
          results[i-1][j] += demand;
          //auto n_name = regex_replace(name,newlines_re,"");
          resFile << name <<":<"<<tableSiteBW.contents[j][0]<<","<<demand<<">\n";
          break;
        }
        else
          continue; // we just use another new edge node
      }
    }
  }
  return 0;
}
