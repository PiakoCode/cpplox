#ifndef CPPLOX_TABLE_H
#define CPPLOX_TABLE_H

#include "common.h"
#include "object/String.h"
#include "value.h"

#define TABLE_MAX_LOAD 0.75
// 哈希表实现

/**
 * @brief
 * KV键值对
 */
class Entry {
public:
    obj::String* key { nullptr };
    Value value;
};

using Entries = std::vector<Entry>;
class Table {
public:
    int count;
    Entries entries;
};

extern Table table;

bool tableSet(Table& table, obj::String* key, Value value);
void adjustCapacity(Table& table, int capacity);
Entry& findEntry(std::vector<Entry>& entries, obj::String* key);
#endif