#ifndef __MOVIEYEAR_H__
#define __MOVIEYEAR_H__

#include <math.h>

#include "typedefs.h"

#define MOVIEYEAR__VALUE_STORAGE_TYPE char
#define MOVIEYEAR__OFFSET 1890
#define MOVIEYEAR__MIN_YEAR MOVIEYEAR__OFFSET
#define MOVIEYEAR__MAX_YEAR MOVIEYEAR__OFFSET + pow(2, sizeof(STORAGE_TYPE) * 7) - 1

class MovieYear
{
    private:
        MOVIEYEAR__VALUE_STORAGE_TYPE _value;
    public:
        Year(const usint &v) {set(v);};
        inline usint get() const { return (usint)_value + (usint)MOVIEYEAR__OFFSET; };
        inline void set(const usint &v) { _value = v - (usint)MOVIEYEAR__OFFSET; };
        static const usint MIN() { return (usint)MIN_YEAR; };
        static const usint MAX() { return (usint)MAX_YEAR; };
};

#endif // __YEAR_H__