#include "../include/stringItem.h"

int cmp(const void* a, const void* b){
    float fa = *(const float*)a;
    float fb = *(const float*)b;
    return ((fa > fb) - (fa < fb));
};

int qs_cmp(const void* a, const void* b){
    return strcmp( (const char*) a, (const char*) b);
};

int qs_cmp_i(const void* a, const void* b){
    return strcmp( (const char*) b, (const char*) a);
};

int eq(const void* a, const void* b){
    return cmp(a, b) == 0;
};
int seq(const void* a, const void* b){
    return qs_cmp(a, b) == 0;
};

int neq(const void* a, const void* b){
    return cmp(a, b) != 0;
};
int sneq(const void* a, const void* b){
    return qs_cmp(a, b) != 0;
};

int egt(const void* a, const void* b){
    return cmp(a, b) >= 0;
};
int segt(const void* a, const void* b){
    return qs_cmp(a, b) >= 0;
};

int gt(const void* a, const void* b){
    return cmp(a, b) > 0;
};
int sgt(const void* a, const void* b){
    return qs_cmp(a, b) > 0;
};

int elt(const void* a, const void* b){
    return cmp(a, b) <= 0;
};
int selt(const void* a, const void* b){
    return qs_cmp(a, b) <= 0;
};

int lt(const void* a, const void* b){
    return cmp(a, b) < 0;
};
int slt(const void* a, const void* b){
    return qs_cmp(a, b) < 0;
};