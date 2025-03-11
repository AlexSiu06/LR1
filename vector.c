#include "vector.h"
#include "stdlib.h"
#include "string.h"
#include "compare.h"
#include "destruct.h"

void Create(Vector* vector, size_t size, size_t size_element, int (*compare)(const void*, const void*)) {
  vector->size_element = size_element;
  vector->data = calloc(size, size_element);
  vector->size = size;
  vector->capacity = size;
  vector->compare = compare;
}

void Clear(Vector* vector) {
  free(vector->data);
  vector->data = NULL;
  vector->size = 0;
  vector->capacity = 0;
  vector->compare = NULL;
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
      vector->data = MyRealoc(vector->data, 0, 1, vector->size_element);
      vector->capacity = 1;
    } else {
      vector->data = MyRealoc(vector->data, vector->capacity, vector->capacity * 2, vector->size_element);
      vector->capacity *= 2;
    }
  }
  char* copy_data = vector->data;
  const char* copy_value = value;
  for (size_t i = 0; i < vector->size_element; ++i) {

    copy_data[i + vector->size_element * vector->size] = copy_value[i];
  }
  ++vector->size;
}

Vector Copy(const Vector* other){
  Vector vector;
    Create(&vector, other->size, other->size_element, other->compare);
  char* copy_data = vector.data;
  char* copy_other_data = other->data;
  for (size_t i = 0; i < other->size; ++i) {
    char* data = copy_data + i * vector.size_element;
    const char* other_data = copy_other_data + i * other->size_element;
    for (size_t j = 0; j < other->size_element; ++j) {
      data[j] = other_data[j];
    }
  }
  return vector;
}

void PopBack(Vector* vector, void (*Destructor)(void*)){
  --vector->size;
    char* copy_object = (char*)vector->data;
    Destructor(copy_object + vector->size * vector->size_element);
}

void* GetElement(Vector vector, size_t index) {
    if(index < 0 || index >= vector.size){
        return NULL;
    }
  return vector.data + index;
}

int Erase(Vector* vector, size_t index) {
    if(index < 0 || index >= vector->size){
        return 1;
    }
  char* copy_data = vector->data;
  for (size_t i = 0; i < vector->size; ++i) {
    char* data = copy_data + i * vector->size_element;
    char* next_data = copy_data + (i + 1) * vector->size_element;
    for (size_t j = 0; j < vector->size_element; ++j) {
      data[j] = next_data[j];
    }
  }
  --vector->size;
    return 0;
}

int Insert(Vector* vector, const void* value, size_t index) {
    if(index < 0 || index >=vector->size){
        return 1;
    }
    if (vector->size == vector->capacity) {
    vector->data = MyRealoc(vector->data, vector->capacity, vector->capacity * 2, vector->size_element);
  }
  char* copy_data = vector->data;
  for (size_t i = 0; i < vector->size; ++i) {
    char* data = copy_data + i * vector->size_element;
    char* previos_data = copy_data + (i - 1) * vector->size_element;
    for (size_t j = vector->size_element; j > index; --j) {
      data[j] = previos_data[j];
    }
  }
  char* data = vector->data;
  const char* copy_value = value;
  for (size_t i = 0; i < vector->size_element; ++i) {
    data[i] = copy_value[i + i * vector->size_element];
  }
    return 0;
}

Vector Where(const Vector* vector, int Predicate(void*)){
    Vector result;
    Create(&result, vector->size, result.size_element, vector->compare);
    char* copy_vector_data = vector->data;
    for(size_t i = 0; i < vector->size; ++i){
        char* data = copy_vector_data + i * vector->size_element;
        if(Predicate(data) == 1){
            memcpy(&result, data, 1);
        }
    }
    return result;
}

Vector Map(size_t count_segment, size_t size) {
    Vector result;
    Create(&result, count_segment, sizeof(Segment), SegmentCompare);
    size_t segment_size = size / count_segment;
    size_t remainder = size % count_segment;
    Segment* segments = (Segment*)result.data;
    size_t start_index = 0;
    for (size_t i = 0; i < count_segment; ++i) {
        size_t end_index = start_index + segment_size + (i < remainder ? 1 : 0);
        segments[i].start_index = start_index;
        segments[i].end_index = end_index;
        start_index = end_index;
    }

    return result;
}

Vector Concatenate(const Vector* vector1, const Vector* vector2) {
    Vector result;
    size_t total_size = vector1->size + vector2->size;
    Create(&result, total_size, vector1->size_element, vector1->compare);
    char* result_data = (char*)result.data;
    char* data1 = (char*)vector1->data;
    for (size_t i = 0; i < vector1->size; ++i) {
        memcpy(result_data + i * vector1->size_element, data1 + i * vector1->size_element, vector1->size_element);
    }
    char* data2 = (char*)vector2->data;
    for (size_t i = 0; i < vector2->size; ++i) {
        memcpy(result_data + (vector1->size + i) * vector2->size_element, data2 + i * vector2->size_element,vector2->size_element);
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
    if (vector->compare == NULL) {
            return;
        }
    char* byfer = calloc(vector->size, vector->size_element);
    MergeSortRec(vector->data, vector->size, vector->size_element, vector->compare, byfer);
    free(byfer);
}

