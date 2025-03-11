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
    free(copy_vector->data);
    copy_vector->data = NULL;
}
