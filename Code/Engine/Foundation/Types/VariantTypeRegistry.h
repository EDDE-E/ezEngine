#pragma once

#include <Foundation/Basics.h>
#include <Foundation/Utilities/EnumerableClass.h>
#include <Foundation/Configuration/Singleton.h>
#include <Foundation/Configuration/Startup.h>
#include <Foundation/Reflection/Implementation/StaticRTTI.h>

class ezStreamWriter;
class ezStreamReader;
class ezVariantTypeInfo;

class EZ_FOUNDATION_DLL ezVariantTypeRegistry
{
  EZ_DECLARE_SINGLETON(ezVariantTypeRegistry);

public:
  const ezVariantTypeInfo* FindVariantTypeInfo(const ezRTTI* pType) const;
  ~ezVariantTypeRegistry();

private:
  EZ_MAKE_SUBSYSTEM_STARTUP_FRIEND(Foundation, VariantTypeRegistry);
  ezVariantTypeRegistry();

  void PluginEventHandler(const ezPluginEvent& EventData);
  void UpdateTypes();

  ezHashTable<const ezRTTI*, const ezVariantTypeInfo*> m_TypeInfos;
};

class EZ_FOUNDATION_DLL ezVariantTypeInfo : public ezEnumerable<ezVariantTypeInfo>
{
public:
  ezVariantTypeInfo();
  virtual const ezRTTI* GetType() const = 0;
  virtual ezUInt32 Hash(const void* pObject) const = 0;
  virtual bool Equal(const void* pObjectA, const void* pObjectB) const = 0;
  virtual void Serialize(ezStreamWriter& writer, const void* pObject) const = 0;
  virtual void Deserialize(ezStreamReader& reader, void* pObject) const = 0;

  EZ_DECLARE_ENUMERABLE_CLASS(ezVariantTypeInfo);
};

template<typename T>
class ezVariantTypeInfoT : public ezVariantTypeInfo
{
  const ezRTTI* GetType() const override
  {
    return ezGetStaticRTTI<T>();
  }
  ezUInt32 Hash(const void* pObject) const override
  {
    return ezHashHelper<T>::Hash(*static_cast<const T*>(pObject));
  }
  bool Equal(const void* pObjectA, const void* pObjectB) const override
  {
    return ezHashHelper<T>::Equal(*static_cast<const T*>(pObjectA), *static_cast<const T*>(pObjectB));
  }
  void Serialize(ezStreamWriter& writer, const void* pObject) const override
  {
    writer << *static_cast<const T*>(pObject);
  }
  void Deserialize(ezStreamReader& reader, void* pObject) const override
  {
    reader >> *static_cast<T*>(pObject);
  }
};



#define EZ_DEFINE_VARIANT_TYPE(TYPE)              \
  ezVariantTypeInfoT<TYPE> g_ezVariantTypeInfoT_ ## TYPE;
