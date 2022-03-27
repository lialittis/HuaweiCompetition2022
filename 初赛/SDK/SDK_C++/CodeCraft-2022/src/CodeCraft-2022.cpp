#include <iostream>
#include <fstream>
#include "helper.h"
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <map>
#include <algorithm>


//int M,N;
int QosLim = 400;

// read csv data
// std::ifstream file();
// demand.csv
Table tableDemand = getNextLineAndSplitIntoTokens("../data/demand.csv");
Table tableQos = getNextLineAndSplitIntoTokens("../data/qos.csv");
Table tableSiteBW = getNextLineAndSplitIntoTokens("../data/site_bandwidth.csv");

/*
  Arrage edges to client i, until rest_demand to 0
*/
void arrange(int i,vector<queue<int>>& queues, vector<map<int,int>>& results,vector<int>& restBW, int rest_demand){
  // first loop all edge nodes to judge if it is possible to arrange all data
  int space_to_expend = 0;
  int space_left = 0;
  vector<int> qualified_edges;
  vector<pair<int,int>> poped_clients;
  vector<int> rest_BW_qualified;
  // cout<<i<<endl;

  for(int j = 0; j < int(tableQos.contents.size()); j++){
    int rest_BW = restBW[j];
    // Firstly, remove all edge nodes which don't satisify requirement of qos
    if(stoi(tableQos.contents[j][i+1]) > QosLim) continue;
    qualified_edges.push_back(j);
    space_left += rest_BW;
  }
  space_to_expend = rest_demand - space_left;
  
  //cout<<space_to_expend<<endl;
  // if no, do pop firstly, until the space needed is qualified
  while(space_to_expend > 0){
    //cout<<"problem for : "<< i<<" "<<space_to_expend<<endl;
    for(auto j:qualified_edges){
      auto client_id = queues[j].front();
      queues[j].pop();
      // cout<<client_id<<" "<< results[client_id][j]<<endl;
      auto poped_demend = results[client_id].at(j);
      space_to_expend -=poped_demend ;
      restBW[j] += poped_demend ;
      results[client_id].erase(j);
      poped_clients.push_back({client_id,poped_demend});
      if(space_to_expend <= 0) break;
    }
  }
  for(auto j: qualified_edges){
    rest_BW_qualified.push_back(restBW[j]);
  }
  // if yes, do second loop to push client
  while(rest_demand >0) {
    
    // choose the largest rest BW
    vector<int>::iterator biggest = std::max_element(std::begin(rest_BW_qualified), std::end(rest_BW_qualified));
    int index = distance(begin(rest_BW_qualified),biggest);
    int j = qualified_edges[index];

    // edge name and BW in total
    int rest_BW = restBW[j];
    // if(rest_BW == 0) continue;
    queues[j].push(i);
    if( rest_demand <= rest_BW){ // if the current edge could accept the rest demand
      // cout<<"to change"<<rest_BW_qualified[index]<<"or"<<rest_BW<<"or"<<*biggest<<" minus "<<rest_demand<<endl;
      restBW[j] -= rest_demand;
      rest_BW_qualified[index] -=rest_demand;

      results[i].insert({j,rest_demand});
      rest_demand = 0;
      // cout<<i<<rest_demand<<endl;
      break;
    }
    else{
      int temp_demand =  rest_BW;
      restBW[j] -= temp_demand;
      rest_BW_qualified[index] -= temp_demand;
      rest_demand -= temp_demand;
      results[i].insert({j,temp_demand});
    }
  }

  // re-arrange for poped clients
  for(auto poped :poped_clients){
    arrange(poped.first,queues,results,restBW,poped.second);
  }
}

int main() {
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
    return 1;
    //cerr<<"Could not open config file.\n";
  
  cFile.close();

  int M = tableDemand.names.size() - 1;
  int N = tableQos.contents.size();

  // table to store the results
  // vector<vector<int>> results(M,vector<int>(N,0));

  // output file
  // string path = "/output";
  // mkdir(path.c_str(),0777); // create directory
  fstream resFile("../output/solution.txt",fstream::out | fstream::trunc);
  /*
  if(!resFile.is_open())
    cout<<"Problem of output"<<endl;
  */
  // each time
  for(int t = 0; t < int(tableDemand.contents.size()); t++) {
    // parameters
    vector<map<int,int>> results(M);
    vector<queue<int>> queueForEdges(N);
    vector<int> restBW;
    for(int j = 0; j < int(tableSiteBW.contents.size());j++){
      restBW.push_back(stoi(tableSiteBW.contents[j][1]));
    }
    for(int i = 1; i <= M; i++){
      // int tempresult = 0; // which is actually results[i-1][j]
      // cout<<sizeof(name)<<endl;
      int rest_demand = stoi(tableDemand.contents[t][i]);
      // cout<<rest_demand<<endl;
      // arrange for client
      if (rest_demand != 0) arrange(i-1,queueForEdges, results,restBW, rest_demand);
    }
    for(int i = 1; i <= int(results.size()); i++){
      string client = tableDemand.names[i];
      int first_flag = true;
      if(results[i-1].empty()){
        resFile<<client << ":"<<"\r\n";
        continue;
      }
      for(auto sol : results[i-1]){
        auto edge = tableSiteBW.contents[sol.first][0];
        if(first_flag) {
          resFile << client <<":";
          if(sol.second != 0) resFile<<"<"<<edge<<","<< sol.second<<">";
          first_flag = false;
        }
        else resFile<< ",<"<<edge<<","<<sol.second<<">";
      }
      if(t==int(tableDemand.contents.size())-1 && i == M ){}
      else {
        resFile<<"\r\n";
      }
    }
  }
  resFile.close();
  return 0;
}
