﻿#include <PCH.h>
#include <EditorEngineProcessFramework/Plugin.h>
#include <EditorEngineProcessFramework/EngineProcess/WorldRttiConverterContext.h>

void ezWorldRttiConverterContext::Clear()
{
  ezRttiConverterContext::Clear();

  m_pWorld = nullptr;
  m_GameObjectMap.Clear();
  m_ComponentMap.Clear();

  m_OtherPickingMap.Clear();
  m_ComponentPickingMap.Clear();
}

void* ezWorldRttiConverterContext::CreateObject(const ezUuid& guid, const ezRTTI* pRtti)
{
  EZ_ASSERT_DEBUG(pRtti != nullptr, "Object type is unknown");

  if (pRtti == ezGetStaticRTTI<ezGameObject>())
  {
    ezStringBuilder tmp;

    ezGameObjectDesc d;
    d.m_sName.Assign(ezConversionUtils::ToString(guid, tmp).GetData());

    ezGameObjectHandle hObject = m_pWorld->CreateObject(d);
    ezGameObject* pObject;
    if (m_pWorld->TryGetObject(hObject, pObject))
    {
      RegisterObject(guid, pRtti, pObject);
      return pObject;
    }
    else
    {
      ezLog::Error("Failed to create ezGameObject!");
      return nullptr;
    }
  }
  else if (pRtti->IsDerivedFrom<ezComponent>())
  {
    ezComponentManagerBase* pMan = m_pWorld->GetOrCreateComponentManager(pRtti);
    if (pMan == nullptr)
    {
      ezLog::Error("Component of type '{0}' cannot be created, no component manager is registered", pRtti->GetTypeName());
      return nullptr;
    }

    // Component is added via reflection shortly so passing a nullptr as owner is fine here.
    ezComponentHandle hComponent = pMan->CreateComponent(nullptr);
    ezComponent* pComponent;
    if (pMan->TryGetComponent(hComponent, pComponent))
    {
      RegisterObject(guid, pRtti, pComponent);
      return pComponent;
    }
    else
    {
      ezLog::Error("Component of type '{0}' cannot be found after creation", pRtti->GetTypeName());
      return nullptr;
    }
  }
  else
  {
    return ezRttiConverterContext::CreateObject(guid, pRtti);
  }
}

void ezWorldRttiConverterContext::DeleteObject(const ezUuid& guid)
{
  ezRttiConverterObject object = GetObjectByGUID(guid);
  const ezRTTI* pRtti = object.m_pType;
  EZ_ASSERT_DEBUG(pRtti != nullptr, "Object does not exist!");
  if (pRtti == ezGetStaticRTTI<ezGameObject>())
  {
    auto hObject = m_GameObjectMap.GetHandle(guid);
    UnregisterObject(guid);
    m_pWorld->DeleteObjectNow(hObject);
  }
  else if (pRtti->IsDerivedFrom<ezComponent>())
  {
    ezComponentHandle hComponent = m_ComponentMap.GetHandle(guid);
    ezComponentManagerBase* pMan = m_pWorld->GetOrCreateComponentManager(pRtti);
    if (pMan == nullptr)
    {
      ezLog::Error("Component of type '{0}' cannot be created, no component manager is registered", pRtti->GetTypeName());
      return;
    }

    UnregisterObject(guid);
    pMan->DeleteComponent(hComponent);
  }
  else
  {
    ezRttiConverterContext::DeleteObject(guid);
  }
}

void ezWorldRttiConverterContext::RegisterObject(const ezUuid& guid, const ezRTTI* pRtti, void* pObject)
{
  if (pRtti == ezGetStaticRTTI<ezGameObject>())
  {
    ezGameObject* pGameObject = static_cast<ezGameObject*>(pObject);
    m_GameObjectMap.RegisterObject(guid, pGameObject->GetHandle());
  }
  else if (pRtti->IsDerivedFrom<ezComponent>())
  {
    ezComponent* pComponent = static_cast<ezComponent*>(pObject);

    EZ_ASSERT_DEV(m_pWorld != nullptr && pComponent->GetWorld() == m_pWorld, "Invalid object to register");

    m_ComponentMap.RegisterObject(guid, pComponent->GetHandle());
    pComponent->SetUniqueID(m_uiNextComponentPickingID++);
    m_ComponentPickingMap.RegisterObject(guid, pComponent->GetUniqueID());
  }

  ezRttiConverterContext::RegisterObject(guid, pRtti, pObject);
}

void ezWorldRttiConverterContext::UnregisterObject(const ezUuid& guid)
{
  ezRttiConverterObject object = GetObjectByGUID(guid);
  EZ_ASSERT_DEBUG(object.m_pObject, "Failed to retrieve object by guid!");
  const ezRTTI* pRtti = object.m_pType;
  if (pRtti == ezGetStaticRTTI<ezGameObject>())
  {
    m_GameObjectMap.UnregisterObject(guid);
  }
  else if (pRtti->IsDerivedFrom<ezComponent>())
  {
    m_ComponentMap.UnregisterObject(guid);
    m_ComponentPickingMap.UnregisterObject(guid);
  }

  ezRttiConverterContext::UnregisterObject(guid);
}

ezRttiConverterObject ezWorldRttiConverterContext::GetObjectByGUID(const ezUuid& guid) const
{
  ezRttiConverterObject object = ezRttiConverterContext::GetObjectByGUID(guid);

  // We can't look up the ptr via the base class map as it keeps changing, we we need to use the handle.
  if (object.m_pType == ezGetStaticRTTI<ezGameObject>())
  {
    auto hObject = m_GameObjectMap.GetHandle(guid);
    ezGameObject* pGameObject = nullptr;
    if (!m_pWorld->TryGetObject(hObject, pGameObject))
    {
      EZ_REPORT_FAILURE("Can't resolve game object GUID!");
    }

    // Update new ptr of game object
    if (object.m_pObject != pGameObject)
    {
      m_ObjectToGuid.Remove(object.m_pObject);
      object.m_pObject = pGameObject;
      m_ObjectToGuid.Insert(object.m_pObject, guid);
    }
  }
  else if (object.m_pType->IsDerivedFrom<ezComponent>())
  {
    auto hComponent = m_ComponentMap.GetHandle(guid);
    ezComponent* pComponent = nullptr;
    if (!m_pWorld->TryGetComponent(hComponent, pComponent))
    {
      // this can happen when one manipulates a running scene, and an object just deleted itself
      //EZ_REPORT_FAILURE("Can't resolve component GUID!");
      return object;
    }

    // Update new ptr of component
    if (object.m_pObject != pComponent)
    {
      m_ObjectToGuid.Remove(object.m_pObject);
      object.m_pObject = pComponent;
      m_ObjectToGuid.Insert(object.m_pObject, guid);
    }
  }
  return object;
}

ezUuid ezWorldRttiConverterContext::GetObjectGUID(const ezRTTI* pRtti, const void* pObject) const
{
  if (pRtti == ezGetStaticRTTI<ezGameObject>())
  {
    const ezGameObject* pGameObject = static_cast<const ezGameObject*>(pObject);
    return m_GameObjectMap.GetGuid(pGameObject->GetHandle());
  }
  else if (pRtti->IsDerivedFrom<ezComponent>())
  {
    const ezComponent* pComponent = static_cast<const ezComponent*>(pObject);
    return m_ComponentMap.GetGuid(pComponent->GetHandle());
  }
  return ezRttiConverterContext::GetObjectGUID(pRtti, pObject);
}