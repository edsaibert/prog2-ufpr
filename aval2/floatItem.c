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
    return atof(a) - atof(b) < 0.01;
};

int felt(char* a, char* b){
    return !fgt(a, b);
};

int flt(char* a, char* b){
    return !fegt(a, b);
};