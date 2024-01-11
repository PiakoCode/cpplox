/**
 * @file table.cpp
 * @author Piako (1163312641@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-01-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "../include/table.h"
#include <functional>
#include <memory>
#include <vector>

Table table;

static Entry& findEntry(std::vector<Entry>& entries,
    obj::String* key)
{
    auto capacity = entries.capacity();
    uint32_t index = key->hash % capacity;
    while (true) {
        Entry& entry = entries[index];
        if (entry.key == key || entry.key == nullptr) {
            return entry;
        }

        index = (index + 1) % capacity;
    }
}

static void adjustCapacity(Table& table, int capacity)
{
    table.entries.reserve(capacity);
    auto& entries = table.entries;
    for (int i = 0; i < capacity; i++) {
        entries[i].key = nullptr;
        entries[i].value = NIL_VAL;
    }
    for (int i = 0; i < capacity / 2; i++) {
        Entry& entry = table.entries[i];
        if (entry.key == nullptr) {
            continue;
        }

        Entry& dest = findEntry(entries, entry.key);
        dest.key = entry.key;
        dest.value = entry.value;
    }
}

bool tableSet(Table& table, obj::String* key, Value value)
{

    if ((double)table.entries.size() + 1.0 > (double)table.entries.capacity() * TABLE_MAX_LOAD) {
        adjustCapacity(table, table.entries.capacity() * 2);
    }

    Entry& entry = findEntry(table.entries, key);
    bool isNewKey = entry.key == nullptr;

    if (isNewKey) {
        table.count++;
    }

    entry.key = key;
    entry.value = value;

    return isNewKey;
}

void tableAddAll(Table& from, Table& to)
{
    for (int i = 0; i < from.entries.capacity(); i++) {
        Entry& entry = from.entries[i];
        if (entry.key != nullptr) {
            tableSet(to, entry.key, entry.value);
        }
    }
}
