#pragma once

#include "Processor.h"

static bool Below(long long int Val1, long long int Val2)
{
    return (Val1 < Val2);
}

static bool BelowOrEqual(long long int Val1, long long int Val2)
{
    return (Val1 <= Val2);
}

static bool Above(long long int Val1, long long int Val2)
{
    return (Val1 > Val2);
}

static bool AboveOrEqual(long long int Val1, long long int Val2)
{
    return (Val1 >= Val2);
}

static bool Equal(long long int Val1, long long int Val2)
{
    return (Val1 == Val2);
}

static bool NotEqual(long long int Val1, long long int Val2)
{
    return (Val1 != Val2);
}

