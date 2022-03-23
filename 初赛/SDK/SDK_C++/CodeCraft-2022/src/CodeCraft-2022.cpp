#include <iostream>
#include <fstream>
#include "helper.h"

int main() {
    std::cout << "Hello world!"<<std::endl;
    
    // read csv data
    // std::ifstream file();
    // demande.csv

    // getNextLineAndSplitIntoTokens("./CMakeLists.txt");
    ifstream file("demand.csv",ios::in);
    if(file.is_open())
      cout<<"yes"<<endl;
    else
      cout <<"no"<<endl;
    return 0;
}
