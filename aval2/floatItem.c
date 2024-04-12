#include "floatItem.h"

int feq(char* a, char* b){
    return atof(a) - atof(b) < 0.01;
};

int fneq(char* a, char* b){
    return !feq(a, b);
};

int fegt(char* a, char* b){
    return atof(a) - atof(b) >= 0.01;    
};

int fgt(char* a, char* b){
    return atof(a) - atof(b) > 0.01;
};

int felt(char* a, char* b){
    return atof(b) - atof(a) >= 0.01;
};

int flt(char* a, char* b){
    return atof(b) - atof(a) > 0.01;
};