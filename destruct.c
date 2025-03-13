#include "destruct.h"
#include "vector.h"
#include "stdlib.h"

void IntDestructor(void* object){
    if(object == NULL){
        return;
    }
}
void DoubleDestructor(void* object){
    if(object == NULL){
        return;
    }
}

void VectorDestructor(void* vector){
    Vector* copy_vector = (Vector*)vector;
    char* copy_data = (char*)copy_vector->data;
    for(size_t i = 0; i < copy_vector->size; ++i){
        copy_vector->type_info.destruct(copy_data + i * copy_vector->type_info.size_element);
    }
    free(copy_vector->data);
    copy_vector->data = NULL;
}

void SegmentDesrtuctor(void* Segment){}
