#include <iostream>
#include <fstream>
#include "helper.h"
#include <filesystem>
#include <unistd.h>

int main() {

  // read csv data
  // std::ifstream file();
  // demand.csv
  Table tableDemand = getNextLineAndSplitIntoTokens("../CodeCraft-2022/src/data/demand.csv");
  Table tableQos = getNextLineAndSplitIntoTokens("../CodeCraft-2022/src/data/qos.csv");
  Table tableSiteBW = getNextLineAndSplitIntoTokens("../CodeCraft-2022/src/data/site_bandwidth.csv");
  
  return 0;
}
