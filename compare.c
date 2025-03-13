#include "compare.h"

int IntCompare(const void* lhs, const void* rhs){
    const int* copy_lhs = (const int*)lhs;
    const int* copy_rhs = (const int*)rhs;
    if(*copy_lhs > *copy_rhs){
        return 1;
    }else if(*copy_lhs < *copy_rhs){
        return -1;
    }else{
        return 0;
    }
}

int DoubleCompare(const void* lhs, const void* rhs){
    const double* copy_lhs = (const double*)lhs;
    const double* copy_rhs = (const double*)rhs;
    if(*copy_lhs > *copy_rhs){
        return 1;
    }else if(*copy_lhs < *copy_rhs){
        return -1;
    }else{
        return 0;
    }
}

int SegmentCompare(const void* lhs, const void* rhs){
    const Segment* lhs_segment = (const Segment*)(lhs);
    const Segment* rhs_segment = (const Segment*)(rhs);
    if(lhs_segment->start_index < rhs_segment->start_index){
        return 1;
    }else if(lhs_segment->start_index > rhs_segment->start_index){
        return -1;
    }else{
        return 0;
    }
}
