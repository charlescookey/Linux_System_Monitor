#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
 float PrevIdle ;
 float previdle;
 float previowait;
 float PrevNonIdle;
 float prevuser;
 float prevnice;
 float prevsystem;
 float previrq;
 float prevsoftirq;
 float prevsteal;
 float PrevTotal;
};

#endif