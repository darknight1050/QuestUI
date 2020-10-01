#pragma once
#include "beatsaber-hook/shared/utils/typedefs.h"

namespace ArrayUtil {

    template <class T, class Predicate>
    inline T* First(Array<T*>* array, Predicate pred)
    {
        for (int i = 0; i < array->Length(); i++) {
            T* item = array->values[i];
            if (pred(item)){
                return item;
            }
        }
        return nullptr;
    }

    template <class T, class Predicate>
    inline T* Last(Array<T*>* array, Predicate pred)
    {
        for (int i = array->Length()-1; i >= 0; i--) {
            T* item = array->values[i];
            if (pred(item)){
                return item;
            }
        }
        return nullptr;
    }
    
}