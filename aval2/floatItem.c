#include "floatItem.h"
#include <math.h>

int feq(char* a, char* b){
    return fabs(atof(a) - atof(b)) < 0.0001;
};

int fneq(char* a, char* b){
    return (fabs(atof(a) - atof(b)) >= 0.0001);
};

int fegt(char* a, char* b){
    return atof(b) - atof(a) >= 0.0001;
};

int fgt(char* a, char* b){
    return atof(a) - atof(b) > 0.0001;
};

int felt(char* a, char* b){
    return atof(a) - atof(b) <= 0.0001;
};

int flt(char* a, char* b){
    return atof(a) - atof(b) < 0;
};