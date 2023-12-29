#ifndef CPPLOX_TABLE_H
#define CPPLOX_TABLE_H

#include "common.h"
#include "value.h"

class Entry {
public:
    obj::String* key;
    Value value;
};

class Table {
public:
    int count{0};
    int capacity{0};
    Entry* entries{nullptr};
};

#endif