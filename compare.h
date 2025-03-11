#ifndef COMPARE_H
#define COMPARE_H
#include <stdlib.h>

int IntCompare(const void* lhs, const void* rhs);

int DoubleCompare(const void* lhs, const void* rhs);

typedef struct Segment{
    size_t start_index;
    size_t end_index;
} Segment;

int SegmentCompare(const void* lhs, const void* rhs);

#endif  // COMPARE_H
