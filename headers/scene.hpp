#pragma once

#include "components.hpp"
#include "entity.hpp"
#include <algorithm>
#include <bitset>
#include <iostream>
#include <ostream>
#include <vector>

const int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> ComponentMask;

struct Scene {
  struct EntityDesc {
    EntityID id;
    ComponentMask mask;
  };

  std::vector<EntityDesc> entities;
  std::vector<ComponentPool *> componentPools;
  std::vector<EntityIndex> free_entities;

  EntityID NewEntity() {
    if (!free_entities.empty()) {
      EntityIndex newIndex = free_entities.back();
      free_entities.pop_back();
      EntityID newID =
          CreateEntityId(newIndex, GetEntityVersion(entities[newIndex].id));
      entities[newIndex].id = newID;
      return entities[newIndex].id;
    }
    entities.push_back(
        {CreateEntityId(EntityIndex(entities.size()), 0), ComponentMask()});
    return entities.back().id;
  }

  template <typename T> T *Assign(EntityID id) {
    int componentId = GetId<T>();

    if (componentPools.size() <= componentId) {
      componentPools.resize(componentId + 1, nullptr);
    }

    if (componentPools[componentId] == nullptr) {
      (componentPools)[componentId] = new ComponentPool(sizeof(T));
    }

    ((*componentPools[componentId]).addData(GetEntityIndex(id), new T()));

    entities[GetEntityIndex(id)].mask.set(componentId);
    return static_cast<T *>(
        (*componentPools[componentId]).get(GetEntityIndex(id)));
  }

  template <typename T> T *Get(EntityID id) {
    int componentId = GetId<T>();
    if (!entities[GetEntityIndex(id)].mask.test(componentId))
      return nullptr;

    T *pComponent = static_cast<T *>(
        (componentPools[componentId])->get(GetEntityIndex(id)));

    return pComponent;
  }

  template <typename T> void Remove(EntityID id) {
    // ensure you are not access entity that has been deleted
    if (entities[GetEntityIndex(id)].id != id) {
      return;
    }

    int componentId = GetId<T>();
    entities[GetEntityIndex(id)].mask.reset(componentId);
  }
  void DestroyEntity(EntityID id) {
    EntityID newID = CreateEntityId(EntityIndex(-1), GetEntityVersion(id) + 1);
    entities[GetEntityIndex(id)].id = newID;
    entities[GetEntityIndex(id)].mask.reset();
    free_entities.push_back(GetEntityIndex(id));
  }
};
