#include "helper.h"

using namespace std;

void printTable(Table table);

Table getNextLineAndSplitIntoTokens(string fname)
{
  Table table;
  vector<string> row;
  string line;
  string cell;
/*
  // get file name
  string line;
  getline(str,line);
*/
  fstream file (fname,fstream::in);

  if(file.is_open()){
    int count = 0;
    while(getline(file,line)){
      row.clear();
      stringstream str(line);
       while(getline(str,cell,','))
         row.push_back(cell);
       if(count==0)
         table.names = row;
       else
         table.contents.push_back(row);
       count ++;
    }
  }
  else
    cout<<"Could not open the file :"<< fname<<endl;
  printTable(table);
  return table;
}

void printTable(Table table)
{
  for(int i = 0; i < int(table.names.size()); i++) {
    cout << table.names[i] <<' ';
  }
  cout <<endl;
  for(int i = 0; i < int(table.names.size()); i++) {
    for(int j = 0; j < int(table.contents[i].size()); j++){
      cout <<table.contents[i][j]<<' ';
    }
    cout <<endl;
  }

}
