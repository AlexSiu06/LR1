#include "test.h"
#include "vector.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "compare.h"
#include "destruct.h"

TypeInfo type_info_int = {
    .compare = IntCompare,
    .destruct = IntDestructor,
    .size_element = sizeof(int)
};

TypeInfo type_info_double = {
    .compare = DoubleCompare,
    .destruct = DoubleDestructor,
    .size_element = sizeof(double)
};

TypeInfo type_info_segment = {
    .compare = SegmentCompare,
    .destruct = SegmentDesrtuctor,
    .size_element = sizeof(Segment)
};

void TestCreate(void) {
    Vector vector;
    size_t capacity = 5;
    Create(&vector, capacity, type_info_int);
    assert(vector.data != NULL);
    assert(vector.size == 0);
    assert(vector.capacity == 5);
    free(vector.data);

    Create(&vector, 0, type_info_int);
    assert(vector.data == NULL);
    assert(vector.size == 0);
    assert(vector.capacity == 0);
}

void TestCopy(void) {
    Vector other;
    Create(&other, 3, type_info_int);
    assert(other.data != NULL);

    int i = 1, j = 2, k = 3;
    PushBack(&other, &i);
    PushBack(&other, &j);
    PushBack(&other, &k);

    Vector vector = Copy(&other);
    assert(vector.data != NULL);

    int* copy_data = (int*)vector.data;
    assert(copy_data[0] == 1);
    assert(copy_data[1] == 2);
    assert(copy_data[2] == 3);
    assert(vector.size == 3);
    assert(vector.capacity >= 3);

    free(other.data);
    free(vector.data);

    Create(&other, 0, type_info_int);
    vector = Copy(&other);
    assert(vector.data == NULL);
    free(other.data);
    free(vector.data);
}

void TestPushBack(void) {
    Vector vector;
    Create(&vector, 3, type_info_double);
    assert(vector.data != NULL);

    double values[] = {2.0, 0.5, 5.0, 1.5};
    for (size_t i = 0; i < 4; ++i) {
        PushBack(&vector, &values[i]);
    }

    assert(vector.size == 4);
    assert(vector.capacity >= 4);
    free(vector.data);

    Create(&vector, 0, type_info_int);
    int intValue = 10;
    PushBack(&vector, &intValue);
    assert(vector.data != NULL);
    assert(vector.size == 1);
    free(vector.data);
}

void TestPopBack(void) {
    Vector vector;
    Create(&vector, 1, type_info_int);
    assert(vector.data != NULL);

    int* data = (int*)vector.data;
    data[0] = 5;
    PopBack(&vector);
    assert(vector.size == 0);
    free(vector.data);

    Create(&vector, 0, type_info_int);
    PopBack(&vector);
    assert(vector.size == 0);
    free(vector.data);
}

void TestGetElement(void) {
    Vector vector;
    Create(&vector, 4, type_info_int);
    assert(vector.data != NULL);

    int* data = (int*)vector.data;
    data[0] = 3; data[1] = 8; data[2] = 2006; data[3] = 4;

    int* element = (int*)GetElement(vector, 2);
    assert(*element == 2006);

    element = (int*)GetElement(vector, 5);
    assert(element == NULL);
    free(vector.data);
}

void TestInsert(void) {
    Vector vector;
    Create(&vector, 3, type_info_double);
    assert(vector.data != NULL);

    double data[] = {4.0, 0.001, 33.0};
    for (size_t i = 0; i < 3; ++i) {
        ((double*)vector.data)[i] = data[i];
    }

    double value = 0.07;
    Insert(&vector, &value, 2);
    assert(((double*)vector.data)[2] == 0.07);
    assert(vector.size == 4);
    free(vector.data);

    Create(&vector, 3, type_info_int);
    int intData[] = {2, 3, 4};
    for (size_t i = 0; i < 3; ++i) {
        ((int*)vector.data)[i] = intData[i];
    }
    int intValue = 1;
    Insert(&vector, &intValue, 0);
    assert(((int*)vector.data)[0] == 1);
    free(vector.data);
}

void TestErase(void) {
    Vector vector;
    Create(&vector, 3, type_info_double);
    assert(vector.data != NULL);

    double data[] = {4.0, 0.001, 33.0};
    for (size_t i = 0; i < 3; ++i) {
        ((double*)vector.data)[i] = data[i];
    }

    Erase(&vector, 1);
    assert(((double*)vector.data)[1] == 33.0);
    assert(vector.size == 2);
    free(vector.data);
}

void TestConcatenate(void) {
    Vector vector1, vector2;
    Create(&vector1, 2, type_info_int);
    Create(&vector2, 3, type_info_int);
    assert(vector1.data != NULL && vector2.data != NULL);

    int data1[] = {1, 2}, data2[] = {3, 4, 5};
    for (size_t i = 0; i < 2; ++i) ((int*)vector1.data)[i] = data1[i];
    for (size_t i = 0; i < 3; ++i) ((int*)vector2.data)[i] = data2[i];

    Vector result = Concatenate(&vector1, &vector2);
    assert(result.data != NULL);
    assert(result.size == 5);
    free(vector1.data);
    free(vector2.data);
    free(result.data);
}

void TestSort(void) {
    Vector vec;
    Create(&vec, 6, type_info_int);
    assert(vec.data != NULL);

    int data[] = {6, 3, 7, 0, 1, 9};
    for (size_t i = 0; i < 6; ++i) ((int*)vec.data)[i] = data[i];

    Sort(&vec);
    assert(((int*)vec.data)[0] == 0);
    assert(((int*)vec.data)[5] == 9);
    free(vec.data);
}

int IsEven(void* data) {
    return (*(int*)data) % 2 == 0;
}

void TestWhere(void) {
    Vector vector;
    Create(&vector, 4, type_info_int);
    assert(vector.data != NULL);

    int data[] = {1, 2, 3, 4};
    for (size_t i = 0; i < 4; ++i) ((int*)vector.data)[i] = data[i];

    Vector result = Where(&vector, IsEven);
    assert(result.data != NULL);
    assert(result.size == 2);
    free(vector.data);
    free(result.data);
}

void IncrementInt(void* data) {
    (*(int*)data)++;
}

void TestMap(void) {
    Vector vector;
    Create(&vector, 3, type_info_int);
    assert(vector.data != NULL);

    int data[] = {1, 2, 3};
    for (size_t i = 0; i < 3; ++i) ((int*)vector.data)[i] = data[i];
    vector.size = 3;

    Vector result = Map(&vector, IncrementInt);
    assert(result.data != NULL);
    assert(((int*)result.data)[0] == 2);
    free(vector.data);
    free(result.data);
}
