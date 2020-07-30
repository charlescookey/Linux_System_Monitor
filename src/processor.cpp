#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    std::vector<std::string> entry = LinuxParser::CpuUtilization();
    
 
    float user , nice , system , idle , iowait , irq, softirq , steal;

    if(entry[0] != "")
    user = std::stof(entry[0]);
    if(entry[1] != "")
    nice = std::stof(entry[1]);
    if(entry[2] != "")
    system = std::stof(entry[2]);
    if(entry[3] != "")
    idle = std::stof(entry[3]);
    if(entry[4] != "")
    iowait = std::stof(entry[4]);
    if(entry[5] != "")
    irq = std::stof(entry[5]);
    if(entry[6] != "")
    softirq = std::stof(entry[6]);
    if(entry[7] != "")
    steal = std::stof(entry[7]);

    PrevIdle = previdle + previowait;
    float Idle = idle + iowait;


    PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
    float NonIdle = user + nice + system + irq + softirq + steal;
    PrevTotal = PrevIdle + PrevNonIdle;
    float Total = Idle + NonIdle;
    float totald = Total - PrevTotal;
    float idled = Idle - PrevIdle;
    float CPU_Percentage = (totald - idled)/totald;

    previdle = idle;
    previowait = iowait;
    prevnice = nice;
    prevsoftirq = softirq;
    prevsteal = steal;
    prevsystem = system;
    prevuser = user;
    previrq = irq;

    return CPU_Percentage;
}