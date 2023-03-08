#pragma once

#include <ps2/ps2.hpp>

template <class T>
class List
{
public:
    List()
    {
        this->arraySize = 0;
        this->arrayMaxSize = 2;
        this->array = (T*)PS2::malloc(this->arrayMaxSize * sizeof(T));
    }

    List* add(T value)
    {
        if (this->array == nullptr)
            return this;

        // Increase array size if we are out of room
        if (this->arraySize >= this->arrayMaxSize)
        {
            uint32_t currentMemorySize = this->arrayMaxSize * sizeof(T);

            // Increase max size by double
            this->arrayMaxSize *= 2;

            // Allocate a new space in memory
            T* temp = (T*)PS2::malloc(this->arrayMaxSize * sizeof(T));

            // Copy existing data to new location
            PS2::memcpy(temp, this->array, currentMemorySize);

            // Deallocate existing location
            PS2::free(this->array);

            this->array = temp;
        }

        // Add value to list
        this->array[this->arraySize] = value;
        this->arraySize++;

        return this;
    }

    T& get(uint32_t index)
    {
        // if (this->array == nullptr || index >= this->arraySize)
        //     throw 1;
        return this->array[index];
    }

    T& operator[](int index)
    {
        return this->get(index);
    }

    uint32_t size()
    {
        return this->arraySize;
    }

    uint32_t maxSize()
    {
        return this->arrayMaxSize;
    }

    void free()
    {
        PS2::free(this->array);
        this->array = nullptr;
        this->arraySize = 0;
        this->arrayMaxSize = 0;
    }
private:
    T* array;
    uint32_t arraySize;
    uint32_t arrayMaxSize;
};