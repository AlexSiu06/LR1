#ifndef VECTOR_H
#define VECTOR_H
#include <stddef.h>

typedef struct TypeInfo{
    size_t size_element;
    int (*compare)(const void*, const void*);
    void (*destruct)(void*);
}TypeInfo;

typedef struct Vector {
    size_t size;
    void* data;
    size_t capacity;
    TypeInfo type_info;
} Vector;

void Create(Vector* vector, size_t capacity, TypeInfo type_info);

void VectorDestructor(void* vector);

void Clear(Vector* vector);

void PushBack(Vector* vector, const void* value);

Vector Copy(const Vector* other);

void PopBack(Vector* vector);

void* GetElement(Vector vector, size_t index);

int Erase(Vector* vector, size_t index);

int Insert(Vector* vector, const void* value, size_t index);

Vector Where(const Vector* vector, int Predicate(void*));

Vector Map(Vector* vector, void (*unary)(void*));

Vector Concatenate(const Vector* vector1, const Vector* vector2);

void Sort(Vector* vector);

#endif  // VECTOR_H

