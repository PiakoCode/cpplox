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

#include "table.h"
#include "object/String.h"
#include <cstdint>
#include <cstring>

Table table;

Entry& findEntry(std::vector<Entry>& entries,
    obj::String* key)
{
    auto capacity = entries.capacity();
    uint32_t index = key->hash % capacity;
    Entry* tombstone = nullptr;
    while (true) {
        Entry& entry = entries[index];
        
        if (entry.key == nullptr) {
            if (Value::isNil(entry.value)) {
                // Empty entry.
                return tombstone != nullptr ? *tombstone : entry;
            }
            // We found a tombstone.
            if (tombstone == nullptr) {
                tombstone = &entry;
            }
        } else if (entry.key == key) {
            // We found the key.
            return entry;
        }
        index = (index + 1) % capacity;
    }
}

void adjustCapacity(Table& table, int capacity)
{
    table.entries.reserve(capacity);
    auto& entries = table.entries;
    for (int i = 0; i < capacity; i++) {
        entries[i].key = nullptr;
        entries[i].value = Value(nullptr);
    }
    table.count = 0;
    for (int i = 0; i < capacity / 2; i++) {
        Entry& entry = table.entries[i];
        if (entry.key == nullptr) {
            continue;
        }

        Entry& dest = findEntry(entries, entry.key);
        dest.key = entry.key;
        dest.value = entry.value;
        table.count++;
    }
}

bool tableSet(Table& table, obj::String* key, Value value)
{

    if ((double)table.entries.size() + 1.0 > (double)table.entries.capacity() * TABLE_MAX_LOAD) {
        if (table.entries.capacity() == 0) {
            adjustCapacity(table, 1);
        } else {
            adjustCapacity(table, table.entries.capacity() * 2);
        }
        
    }

    Entry& entry = findEntry(table.entries, key);
    bool isNewKey = entry.key == nullptr;

    if (isNewKey && Value::isNil(entry.value)) {
        table.count++;
    }

    entry.key = key;
    entry.value = value;

    return isNewKey;
}

bool tableGet(Table& table, obj::String* key, Value& value)
{
    if (table.count == 0) {
        return false;
    }
    Entry& entry = findEntry(table.entries, key);
    if (entry.key == nullptr) {
        return false;
    }
    value = entry.value;
    return true;
}

bool tableDelete(Table& table, obj::String* key)
{
    if (table.count == 0) {
        return false;
    }
    Entry& entry = findEntry(table.entries, key);
    if (entry.key == nullptr) {
        return false;
    }
    entry.key = nullptr;
    entry.value = Value(false);
    return true;
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

obj::String* tableFindString(Table& table, const std::string& str, uint32_t hash)
{
    if (table.count == 0) {
        return nullptr;
    }

    uint32_t index = hash % table.entries.capacity();
    while (true) {
        Entry& entry = table.entries[index];
        if (entry.key == nullptr) {
            // Stop if we find an empty non-tombstone entry;
            if (Value::isNil(entry.value)) {
                return nullptr;
            }
        } else if (entry.key->length() == str.length() && entry.key->hash == hash && memcmp(entry.key->c_str(), str.c_str(), str.length()) == 0) {
            // We found it.
            return entry.key;
        }

        index = (index + 1) % table.entries.capacity();
    }
}