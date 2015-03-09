/*
 * The MIT License (MIT)

 * Copyright (c) 2015 Microsoft Corporation

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once


#include "replication_common.h"
#include <vector>

namespace rdsn { namespace replication {

class mutation_cache
{
public:
    mutation_cache(decree initDecree, int maxCount);
    ~mutation_cache();

    int         put(mutation_ptr& mu);
    mutation_ptr pop_min();
    mutation_ptr get_mutation_by_decree(decree decree);
    void        reset(decree initDecree, bool clearMutations);

    decree  min_decree() const { return _startDecree; } 
    decree  max_decree() const { return _endDecree; }
    long    total_size_in_bytes() const { return _totalSizeInBytes; }
    int     count()     const { return _interval; }

private:
    std::vector<mutation_ptr> _array;
    int          _maxCount;
    
    int          _interval;
    long         _totalSizeInBytes;

    int          _startIndex;
    int          _endIndex;
    decree       _startDecree;
    decree       _endDecree;
};

}} // namespace

