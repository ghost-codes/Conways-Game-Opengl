#pragma once

typedef unsigned int EntityIndex;
typedef unsigned int EntityVersion;
typedef unsigned long long EntityID;

inline EntityID CreateEntityId(EntityIndex index, EntityVersion version) {
  // shift the index up 32 and puth the version at the bottom;
  return ((EntityID)index << 32 | (EntityVersion)version);
}

inline EntityIndex GetEntityIndex(EntityID id) { return id >> 32; }

inline EntityVersion GetEntityVersion(EntityID id) { return (EntityVersion)id; }

inline bool IsEntityInvalid(EntityID id) {
  return (id >> 32) != EntityIndex(-1);
}

#define INVALID_ENTITY CreateEntityId(EntityIndex(-1), 0)
