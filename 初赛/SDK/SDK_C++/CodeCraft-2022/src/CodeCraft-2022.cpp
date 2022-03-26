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
  int N = tableQos.contents.size();

  // table to store the results
  // vector<vector<int>> results(M,vector<int>(N,0));

  // output file
  string path = "/output";
  mkdir(path.c_str(),0777); // create directory
  fstream resFile("/output/solution.txt",fstream::out | fstream::trunc);
  if(!resFile.is_open())
    cout<<"Problem of output"<<endl;
  // time
  for(int t = 0; t < int(tableDemand.contents.size()); t++) {
    // temp
    vector<int> temp_usedBW(N,0);
    for(int i = 1; i < int(tableDemand.names.size()); i++){
      // int tempresult = 0; // which is actually results[i-1][j]
      // assign the demand for client i at time t
      string name = tableDemand.names[i];
      // cout<<sizeof(name)<<endl;
      int rest_demand = stoi(tableDemand.contents[t][i]);
      int flag_firt = 1;
      // loop all edge nodes
      for(int j = 0; j < int(tableQos.contents.size()); j++){
        // edge name and BW in total
        auto edge = tableSiteBW.contents[j][0];
        int total_BW = stoi(tableSiteBW.contents[j][1]);
        int rest_BW = total_BW - temp_usedBW[j];
        // Firstly, remove all edge nodes which don't satisify requirement of qos
        if(stoi(tableQos.contents[j][i]) > QosLim || rest_BW == 0) continue;
        // if there are enough space for client i
        if( rest_demand <= rest_BW){
          temp_usedBW[j] += rest_demand;
          //auto n_name = regex_replace(name,newlines_re,"");
          if(flag_firt) {
            resFile << name <<":<"<<edge<<","<<rest_demand<<">";
            flag_firt = 0;
          }
          else {
            resFile<< ",<"<<edge<<","<<rest_demand<<">";
          }
          rest_demand = 0;
          if(t==int(tableDemand.contents.size())-1 && i == int(tableDemand.names.size()) - 1){}
          else {
            resFile<<"\n";
          }
          break;
        }
        else{
          int temp_demand =  rest_BW;
          temp_usedBW[j] += temp_demand;
          rest_demand -= temp_demand;
          if(flag_firt) {
            resFile << name <<":<"<<edge<<","<<temp_demand<<">";
            flag_firt=0;
          }
          else {
            resFile<< ",<"<<edge<<","<<temp_demand<<">";
          }
          // continue; // we just use another new edge node
        }
      }
    }
  }
  resFile.close();
  return 0;
}
