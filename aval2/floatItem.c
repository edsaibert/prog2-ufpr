#include "floatItem.h"

int feq(char* a, char* b){
    return atof(a) == atof(b);
};

int fneq(char* a, char* b){
    return atof(a) != atof(b);
};

int fegt(char* a, char* b){
    return atof(a) >= atof(b);
};

int fgt(char* a, char* b){
    return atof(a) > atof(b);
};

int felt(char* a, char* b){
    return atof(a) <= atof(b);
};

int flt(char* a, char* b){
    return atof(a) < atof(b);
};