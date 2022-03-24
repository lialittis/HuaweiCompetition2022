#include <iostream>
#include <fstream>
#include "helper.h"
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() {

  //int M,N;
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

  cFile.close();

  //M = tableDemand.names.size() - 1;
  //N = tableQos.contents.size();

  // table to store the results
  // vector<vector<int>> results(M,vector<int>(N,0));

  // output file
  string path = "../output";
  mkdir(path.c_str(),0777); // create directory
  fstream resFile("../output/solution.txt",fstream::out | fstream::trunc);
  if(!resFile.is_open())
    cout<<"Problem of output"<<endl;
  // time
  for(int t = 0; t < int(tableDemand.contents.size()); t++) {
    // temp
    for(int i = 1; i < int(tableDemand.names.size()); i++){
      int tempresult = 0; // which is actually results[i-1][j]
      // assign the demand for client i at time t
      string name = tableDemand.names[i];
      // cout<<sizeof(name)<<endl;
      int demand = stoi(tableDemand.contents[t][i]);
      // loop all edge nodes
      for(int j = 0; j < int(tableQos.contents.size()); j++){
        if(stoi(tableQos.contents[j][i]) > QosLim) continue;
        // if there are enough space for client i
        if(tempresult + demand < stoi(tableSiteBW.contents[j][1])){
          tempresult += demand;
          //auto n_name = regex_replace(name,newlines_re,"");
          resFile << name <<":<"<<tableSiteBW.contents[j][0]<<","<<demand<<">\r\n";
          break;
        }
        else
          continue; // we just use another new edge node
      }
    }
  }
  resFile.close();
  return 0;
}
