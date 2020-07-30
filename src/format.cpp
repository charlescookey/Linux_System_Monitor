#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    if(seconds<=0)return string();
    long a  = seconds / 3600;
    long b  = seconds / 60;
    long c  = b %60;
    long d  = seconds%60;

    string answer;
    answer += a>9?"":"0";
    answer+=to_string(a);
    answer+=":";
    answer += c>9?"":"0";
    answer+=to_string(c);
    answer+=":";
    answer += d>9?"":"0";
    answer+=to_string(d);

    return answer;
}