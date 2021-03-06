/*
  ==============================================================================

    RingBuffer.h
    Created: 29 Oct 2017 11:09:47pm
    Author:  Jonathon Racz

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

template<class Type>
class RingBuffer
{
public:
    RingBuffer() {}
    RingBuffer(int logicalAllocatedCapacity, bool _fill = false, const Type& fillValue = Type()) noexcept
    {
        setLogicalCapacity(logicalAllocatedCapacity);
        if (_fill)
            fill(fillValue);
    }

    RingBuffer(const RingBuffer& other)
    {
        setCapacity(other.capacity);
        for (int i = 0; i < other.getNumElements(); ++i)
            push(other.getReference(i));
    }

    RingBuffer(RingBuffer&& other) = default;

    ~RingBuffer() noexcept {}

    void clear() noexcept
    {
        head = tail = 0;
    }

    void fill(const Type& value = Type()) noexcept
    {
        while (getNumElements() < (getLogicalCapacity()))
            push(value);
    }

    int getCapacity() const noexcept
    {
        return capacity;
    }

    void setCapacity(int num, bool doNotReallocate = false) noexcept
    {
        if (allocSize < num)
        {
            jassert(!doNotReallocate); // You would have had to reallocate here!
            setAllocatedCapacity(num);
        }
        linearize();
        capacity = num;
    }

    int getLogicalCapacity() const noexcept
    {
        return getCapacity() - 1;
    }

    void setLogicalCapacity(int num, bool doNotReallocate = false) noexcept
    {
        setCapacity(num + 1, doNotReallocate);
    }

    int getAllocatedCapacity() const noexcept
    {
        return allocSize;
    }

    int getAllocatedLogicalCapacity() const noexcept
    {
        return getAllocatedCapacity() - 1;
    }

    void deallocateToCurrentCapacity() noexcept
    {
        if (allocSize > capacity)
            setAllocatedCapacity(capacity);
    }

    int getNumElements() const noexcept
    {
        if (head < tail)
            return tail - head;
        else if (head > tail)
            return (getCapacity() - head) + tail;
        else
            return 0;
    }

    bool isFull() const noexcept
    {
        return getNumElements() == getLogicalCapacity();
    }

    bool isEmpty() const noexcept
    {
        return getNumElements() == 0;
    }

    void push(const Type& src) noexcept
    {
        data[tail] = src;
        tail = (tail + 1) % getCapacity();
        if (head == tail)
            head = (head + 1) % getCapacity();
    }

    void push(const Type* src, int num) noexcept
    {
        // Placeholder code until I can get write something more performant also
        // with non-overwrite behavior...
        for (int i = 0; i < num; ++i)
            push(src[i]);
    }

    Type pop() noexcept
    {
        jassert(head != tail);
        Type ret = data[head];
        head = (head + 1) % getCapacity();
        return ret;
    }

    void pop(Type* dst, int num) noexcept
    {
        // Ditto from the batch push() function comment.
        jassert(num <= getNumElements());
        for (int i = 0; i < num; ++i)
            dst[i] = pop();
    }

    Type& getReference(int index) const noexcept
    {
        jassert(index < getNumElements());
        return data[(head + index) % getNumElements()];
    }

    Type getFirst() const noexcept
    {
        return data[head];
    }

    Type getLast() const noexcept
    {
        return (*this)[getNumElements() - 1];
    }

    Type* getPointerToFirstHalf() const noexcept
    {
        return data.getData() + head;
    }

    int getSizeOfFirstHalf() const noexcept
    {
        if (head <= tail)
            return tail - head;
        else
            return capacity - head;
    }

    Type* getPointerToSecondHalf() const noexcept
    {
        if (head <= tail || tail == 0)
            return nullptr;
        else
            return data.getData();
    }

    int getSizeOfSecondHalf() const noexcept
    {
        if (head <= tail || tail == 0)
            return 0;
        else
            return tail;
    }

    Type operator[](int index) const noexcept
    {
        jassert(index < getNumElements());
        return data[(head + index) % getNumElements()];
    }

    void setData(const Type* src, int num) noexcept
    {
        jassert(num < getCapacity());
        if (std::is_trivially_copyable<Type>::value)
        {
            head = 0;
            tail = num;
            memcpy(data.getData(), src, num * sizeof(Type));
        }
        else
        {
            clear();
            for (int i = 0; i < num; ++i)
                push(src[i]);
        }
    }

    void linearize() noexcept
    {
        head = 0;
        tail = getNumElements();
        std::rotate(data.getData(), data.getData() + head, data.getData() + capacity);
    }

private:
    void setAllocatedCapacity(int num) noexcept
    {
        if (num)
        {
            data.realloc(num);
            for (int i = allocSize; i < num; ++i)
                new (data + i) Type();
        }
        else
        {
            data.free();
        }

        allocSize = num;
    }

    void setAllocatedLogicalCapacity(int num) noexcept
    {
        setAllocatedCapacity(num + 1);
    }

    HeapBlock<Type> data;
    int allocSize = 0;
    int capacity = 0;
    int head = 0;
    int tail = 0;
};
