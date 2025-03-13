#include "vector.h"
#include "stdlib.h"
#include "string.h"
#include "compare.h"
#include "destruct.h"

void SetElement(Vector* vector, const void* data){
    const char* copy_data = (const void*)data;
    char* vector_data = (char*)data;
     for (size_t i = 0; i < vector->type_info.size_element; ++i) {
         vector_data[i + vector->type_info.size_element * vector->size] = copy_data[i];
     }
}

void Create(Vector* vector, size_t capacity, TypeInfo type_info) {
  vector->data = calloc(capacity, type_info.size_element);
  vector->size = 0;
  vector->capacity = capacity;
  vector->type_info = type_info;
}

void Clear(Vector* vector) {
  free(vector->data);
  vector->data = NULL;
  vector->size = 0;
  vector->capacity = 0;
}

void* MyRealoc(void* data, size_t old_capacity, size_t new_capacity, size_t size_element);

void* MyRealoc(void* data, size_t old_capacity, size_t new_capacity, size_t size_element) {
  char* new_data = calloc(new_capacity, size_element);
  for (size_t i = 0; i < old_capacity; ++i) {
    char* data_copy = data;
    new_data[i] = data_copy[i];
  }
  free(data);
  return new_data;
}

void PushBack(Vector* vector, const void* value) {
  if (vector->size == vector->capacity) {
    if (vector->capacity == 0) {
      vector->data = MyRealoc(vector->data, 0, 1, vector->type_info.size_element);
      vector->capacity = 1;
    } else {
      vector->data = MyRealoc(vector->data, vector->capacity, vector->capacity * 2, vector->type_info.size_element);
      vector->capacity *= 2;
    }
  }
    SetElement(vector, value);
  ++vector->size;
}

Vector Copy(const Vector* other){
  Vector vector;
    Create(&vector, other->size, other->type_info);
  char* copy_data = vector.data;
  char* copy_other_data = other->data;
  for (size_t i = 0; i < other->size; ++i) {
    char* data = copy_data + i * vector.type_info.size_element;
    const char* other_data = copy_other_data + i * other->type_info.size_element;
    for (size_t j = 0; j < other->type_info.size_element; ++j) {
      data[j] = other_data[j];
    }
  }
  return vector;
}

void PopBack(Vector* vector){
  --vector->size;
    char* copy_object = (char*)vector->data;
    vector->type_info.destruct(copy_object + vector->size * vector->type_info.size_element);
}

void* GetElement(Vector vector, size_t index) {
    if(index >= vector.size){
        return NULL;
    }
  return vector.data + index;
}

int Erase(Vector* vector, size_t index) {
    if(index >= vector->size){
        return 1;
    }
  char* copy_data = vector->data;
  for (size_t i = 0; i < vector->size; ++i) {
    char* data = copy_data + i * vector->type_info.size_element;
    char* next_data = copy_data + (i + 1) * vector->type_info.size_element;
    for (size_t j = 0; j < vector->type_info.size_element; ++j) {
      data[j] = next_data[j];
    }
  }
  --vector->size;
    return 0;
}

int Insert(Vector* vector, const void* value, size_t index) {
    if(index >=vector->size){
        return 1;
    }
    if (vector->size == vector->capacity) {
    vector->data = MyRealoc(vector->data, vector->capacity, vector->capacity * 2, vector->type_info.size_element);
  }
  char* copy_data = vector->data;
  for (size_t i = 0; i < vector->size; ++i) {
    char* data = copy_data + i * vector->type_info.size_element;
    char* previos_data = copy_data + (i - 1) * vector->type_info.size_element;
    for (size_t j = vector->type_info.size_element; j > index; --j) {
      data[j] = previos_data[j];
    }
  }
  char* data = vector->data;
  const char* copy_value = value;
  for (size_t i = 0; i < vector->type_info.size_element; ++i) {
    data[i] = copy_value[i + i * vector->type_info.size_element];
  }
    return 0;
}

Vector Where(const Vector* vector, int Predicate(void*)){
    Vector result;
    Create(&result, vector->size, vector->type_info);
    char* copy_vector_data = vector->data;
    for(size_t i = 0; i < vector->size; ++i){
        char* data = copy_vector_data + i * vector->type_info.size_element;
        if(Predicate(data) == 1){
            PushBack(&result, data);
        }
    }
    return result;
}

Vector Map(Vector* vector, void (*unary)(void*)) {
    Vector result = Copy(vector);
    char* copy_vector_data = (char*)result.data;
    for(size_t i = 0; i < vector->size; ++i){
        char* data = copy_vector_data + i * vector->type_info.size_element;
        unary(data);
    }
    return result;
}

Vector Concatenate(const Vector* vector1, const Vector* vector2) {
    Vector result;
    size_t total_size = vector1->size + vector2->size;
    Create(&result, total_size, vector1->type_info);
    char* result_data = (char*)result.data;
    char* data1 = (char*)vector1->data;
    for (size_t i = 0; i < vector1->size; ++i) {
        memcpy(result_data + i * vector1->type_info.size_element, data1 + i * vector1->type_info.size_element, vector1->type_info.size_element);
    }
    char* data2 = (char*)vector2->data;
    for (size_t i = 0; i < vector2->size; ++i) {
        memcpy(result_data + (vector1->size + i) * vector2->type_info.size_element, data2 + i * vector2->type_info.size_element,vector2->type_info.size_element);
    }
    return result;
}

void Merge(void* data, size_t first_size, size_t second_size, void* byfer, size_t size_element, int (*compare)(const void*, const void*)){
    char* copy_data = data;
    char* copy_byfer = byfer;
    size_t i = 0;
    size_t j = 0;
    while(i < first_size && j < second_size){
        if(compare(copy_data + i * size_element, copy_data + (first_size + j) * size_element) > 0){
            memcpy(copy_byfer + (i + j) * size_element, copy_data + (first_size + j) * size_element, size_element);
            ++j;
        }else{
            memcpy(copy_byfer + (i + j) * size_element, copy_data + i * size_element, size_element);
            ++i;
        }
    }
    while (i < first_size) {
        memcpy(copy_byfer + (i + j) * size_element, copy_data + i * size_element, size_element);
        ++i;
    }
    while (j < second_size) {
        memcpy(copy_byfer + (i + j) * size_element, copy_data + (first_size + j) * size_element, size_element);
        ++j;
    }
    memcpy(data, copy_byfer, (i + j) * size_element);
}

void MergeSortRec(void* data, size_t size, size_t size_element, int (*compare)(const void*, const void*), void* byfer){
    if(size < 2){
        return;
    }
    char* copy_data = data;
    MergeSortRec(data, size/2, size_element, compare, byfer);
    MergeSortRec(copy_data + (size/2) * size_element, size - size /2 , size_element, compare, byfer);
    Merge(data, size/2, size - size/2, byfer, size_element, compare);
}

void Sort(Vector* vector){
    if (vector->type_info.compare == NULL) {
        return;
    }
    char* buffer = calloc(vector->size, vector->type_info.size_element);
    MergeSortRec(vector->data, vector->size, vector->type_info.size_element, vector->type_info.compare, buffer);
    free(buffer);
}

