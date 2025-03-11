#include "test.h"
#include "vector.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "compare.h"
#include "destruct.h"

void TestCreate(void) {
    Vector vector;
    size_t size = 5;
    Create(&vector, size, sizeof(int), IntCompare);
    assert(vector.size == 5);
    assert(vector.capacity == 5);
    free(vector.data);
    Create(&vector, 0, sizeof(int),IntCompare);
    assert(vector.size == 0);
    assert(vector.capacity == 0);
    assert(vector.data == NULL);
    free(vector.data);
}

void TestCopy(void) {
    Vector other;
    Create(&other, 3, sizeof(int), IntCompare);
    int* data = (int*)other.data;
    data[0] = 1;
    data[1] = 2;
    data[2] = 3;
    Vector vector = Copy(&other);
    int* copy_data = (int*)vector.data;
    assert(copy_data[0] == 1);
    assert(copy_data[1] == 2);
    assert(copy_data[2] == 3);
    assert(vector.size == 3);
    assert(vector.capacity >= 3);
    free(other.data);
    free(vector.data);
    Create(&other, 0, sizeof(int), IntCompare);
    vector = Copy(&other);
    assert(vector.size == 0);
    assert(vector.capacity == 0);
    assert(vector.data == NULL);
    free(other.data);
    free(vector.data);
}

void TestPushBack(void) {
    Vector vector;
    Create(&vector, 3, sizeof(double), DoubleCompare);
    double* data = (double*)vector.data;
    data[0] = 2.0;
    data[1] = 0.5;
    data[2] = 5.0;
    double value = 1.5;
    PushBack(&vector, &value);
    assert(data[3] == 1.5);
    assert(vector.size == 4);
    assert(vector.capacity >= 4);
    free(vector.data);
    Create(&vector, 0, sizeof(int), DoubleCompare);
    int intValue = 10;
    PushBack(&vector, &intValue);
    int* intData = (int*)vector.data;
    assert(intData[0] == 10);
    assert(vector.size == 1);
    assert(vector.capacity >= 1);
    free(vector.data);
}

void TestPopBack(void) {
    Vector vector;
    Create(&vector, 1, sizeof(int), IntCompare);
    int* data = (int*)vector.data;
    data[0] = 5;
    PopBack(&vector, IntDestructor);
    assert(vector.size == 0);
    assert(vector.capacity >= 0);
    free(vector.data);
    Create(&vector, 0, sizeof(int), IntCompare);
    PopBack(&vector, IntDestructor);
    assert(vector.size == 0);
    assert(vector.capacity == 0);
    free(vector.data);
}

void TestGetElement(void) {
    Vector vector;
    Create(&vector, 4, sizeof(int), IntCompare);
    int* data = (int*)vector.data;
    data[0] = 3;
    data[1] = 8;
    data[2] = 2006;
    data[3] = 4;
    int* element = (int*)GetElement(vector, 2);
    assert(*element == 2006);
    free(vector.data);
    element = (int*)GetElement(vector, 5);
    assert(element == NULL);
}

void TestInsert(void) {
    Vector vector;
    Create(&vector, 3, sizeof(double), DoubleCompare);
    double* data = (double*)vector.data;
    data[0] = 4.0;
    data[1] = 0.001;
    data[2] = 33.0;
    double value = 0.07;
    Insert(&vector, &value, 2);
    assert(data[2] == 0.07);
    assert(vector.size == 4);
    assert(vector.capacity >= 4);
    free(vector.data);
    Create(&vector, 3, sizeof(double), DoubleCompare);
    Insert(&vector, &value, 5);
    assert(vector.size == 3);
    free(vector.data);
    Create(&vector, 3, sizeof(int), IntCompare);
    int* intData = (int*)vector.data;
    intData[0] = 2;
    intData[1] = 3;
    intData[2] = 4;
    int intValue = 1;
    Insert(&vector, &intValue, 0);
    assert(intData[0] == 1);
    assert(intData[1] == 2);
    assert(intData[2] == 3);
    assert(intData[3] == 4);
    assert(vector.size == 4);
    assert(vector.capacity >= 4);
    free(vector.data);
}

void TestErase(void) {
    Vector vector;
    Create(&vector, 3, sizeof(double), DoubleCompare);
    double* data = (double*)vector.data;
    data[0] = 4.0;
    data[1] = 0.001;
    data[2] = 33.0;
    Erase(&vector, 1);
    assert(data[1] == 33.0);
    assert(vector.size == 2);
    assert(vector.capacity >= 2);
    free(vector.data);
    Create(&vector, 3, sizeof(double), DoubleCompare);
    Erase(&vector, 5);
    assert(vector.size == 3);
    free(vector.data);
    Create(&vector, 3, sizeof(int), IntCompare);
    int* intData = (int*)vector.data;
    intData[0] = 1;
    intData[1] = 2;
    intData[2] = 3;
    Erase(&vector, 0);
    assert(intData[0] == 2);
    assert(intData[1] == 3);
    assert(vector.size == 2);
    assert(vector.capacity >= 2);
    free(vector.data);
}

void TestConcatenate(void) {
    Vector vector1, vector2;
    Create(&vector1, 2, sizeof(int), IntCompare);
    Create(&vector2, 3, sizeof(int), IntCompare);
    int* data1 = (int*)vector1.data;
    data1[0] = 1;
    data1[1] = 2;
    int* data2 = (int*)vector2.data;
    data2[0] = 3;
    data2[1] = 4;
    data2[2] = 5;
    Vector result = Concatenate(&vector1, &vector2);
    int* resultData = (int*)result.data;
    assert(result.size == 5);
    assert(resultData[0] == 1);
    assert(resultData[1] == 2);
    assert(resultData[2] == 3);
    assert(resultData[3] == 4);
    assert(resultData[4] == 5);
    free(vector1.data);
    free(vector2.data);
    free(result.data);
    Create(&vector1, 0, sizeof(int), IntCompare);
    Create(&vector2, 0, sizeof(int), IntCompare);
    result = Concatenate(&vector1, &vector2);
    assert(result.size == 0);
    assert(result.capacity == 0);
    assert(result.data == NULL);
    free(vector1.data);
    free(vector2.data);
    free(result.data);
}

void TestSort(void) {
    Vector vec;
    Create(&vec, 6, sizeof(int), IntCompare);
    int* data1 = (int*)vec.data;
    data1[0] = 6;
    data1[1] = 3;
    data1[2] = 7;
    data1[3] = 0;
    data1[4] = 1;
    data1[5] = 9;
    Sort(&vec);
    assert(data1[0] == 0);
    assert(data1[1] == 1);
    assert(data1[2] == 3);
    assert(data1[3] == 6);
    assert(data1[4] == 7);
    assert(data1[5] == 9);
    free(vec.data);
    Create(&vec, 0, sizeof(int), IntCompare);
    Sort(&vec);
    assert(vec.size == 0);
    assert(vec.capacity == 0);
    assert(vec.data == NULL);
    free(vec.data);
}

int IsEven(void* data) {
    int* value = (int*)data;
    return (*value % 2) == 0;
}

void TestWhere(void) {
    Vector vector;
    Create(&vector, 4, sizeof(int), IntCompare);
    int* data = (int*)vector.data;
    data[0] = 1;
    data[1] = 2;
    data[2] = 3;
    data[3] = 4;
    Vector result = Where(&vector, IsEven);
    int* res_data = (int*)result.data;
    assert(result.size == 2);
    assert(res_data[0] == 2);
    assert(res_data[1] == 4);
    free(vector.data);
    free(result.data);
    Create(&vector, 0, sizeof(int), IntCompare);
    result = Where(&vector, IsEven);
    assert(result.size == 0);
    assert(result.capacity == 0);
    assert(result.data == NULL);
    free(vector.data);
    free(result.data);
    Create(&vector, 3, sizeof(int), IntCompare);
    data = (int*)vector.data;
    data[0] = 1;
    data[1] = 3;
    data[2] = 5;
    result = Where(&vector, IsEven);
    assert(result.size == 0);
    assert(result.capacity == 0);
    assert(result.data == NULL);
    free(vector.data);
    free(result.data);
}

void TestMap(void) {
    Vector vector;
    Create(&vector, 10, sizeof(int), IntCompare);
    int* data = (int*)vector.data;
    data[0] = 5;
    data[1] = 1;
    data[2] = 2;
    data[3] = 1;
    data[4] = 3;
    data[5] = 4;
    data[6] = 0;
    data[7] = 7;
    data[8] = 9;
    data[9] = 8;
    Vector result = Map(3, 10);
    Segment* segments = (Segment*)result.data;
    assert(segments[0].start_index == 0);
    assert(segments[0].end_index == 4);
    assert(segments[1].start_index == 4);
    assert(segments[1].end_index == 7);
    assert(segments[2].start_index == 7);
    assert(segments[2].end_index == 10);
    free(vector.data);
    free(result.data);
    Create(&vector, 0, sizeof(int), IntCompare);
    result = Map(3, 0);
    assert(result.size == 0);
    assert(result.capacity == 0);
    assert(result.data == NULL);
    free(vector.data);
    free(result.data);
    Create(&vector, 5, sizeof(int), IntCompare);
    result = Map(10, 5);
    assert(result.size == 0);
    assert(result.capacity == 0);
    assert(result.data == NULL);
    free(vector.data);
    free(result.data);
}


