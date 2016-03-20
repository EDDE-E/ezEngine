#include <Core/PCH.h>
#include <Core/WorldSerializer/WorldWriter.h>
#include <Core/ResourceManager/HandleSerialization.h>


void ezWorldWriter::Write(ezStreamWriter& stream, ezWorld& world, const ezTagSet* pExclude)
{
  m_pStream = &stream;
  m_pWorld = &world;
  m_pExclude = pExclude;

  EZ_LOCK(m_pWorld->GetReadMarker());

  const ezUInt8 uiVersion = 2;
  stream << uiVersion;

  m_AllRootObjects.Clear();
  m_AllChildObjects.Clear();
  m_AllComponents.Clear();
  m_uiNumComponents = 0;

  // invalid handles
  {
    m_WrittenGameObjectHandles.Clear();
    m_WrittenComponentHandles.Clear();

    m_WrittenGameObjectHandles[ezGameObjectHandle()] = 0;
    m_WrittenComponentHandles[ezComponentHandle()] = 0;
  }

  m_pWorld->Traverse(ezMakeDelegate(&ezWorldWriter::ObjectTraverser, this), ezWorld::TraversalMethod::DepthFirst);

  stream << m_AllRootObjects.GetCount();
  stream << m_AllChildObjects.GetCount();
  stream << m_AllComponents.GetCount();
  stream << m_uiNumComponents;

  AssignGameObjectIndices();
  AssignComponentHandleIndices();


  for (const auto* pObject : m_AllRootObjects)
  {
    WriteGameObject(pObject);
  }

  for (const auto* pObject : m_AllChildObjects)
  {
    WriteGameObject(pObject);
  }

  for (auto it = m_AllComponents.GetIterator(); it.IsValid(); ++it)
  {
    WriteComponentInfo(it.Key());
  }

  {
    ezResourceHandleWriteContext ResHandleWriter;

    for (auto it = m_AllComponents.GetIterator(); it.IsValid(); ++it)
    {
      WriteComponentsOfType(it.Key(), it.Value(), ResHandleWriter);
    }

    ResHandleWriter.Finalize(m_pStream);
  }
}


void ezWorldWriter::AssignGameObjectIndices()
{
  ezUInt32 uiGameObjectIndex = 1;
  for (const auto* pObject : m_AllRootObjects)
  {
    m_WrittenGameObjectHandles[pObject->GetHandle()] = uiGameObjectIndex;
    ++uiGameObjectIndex;
  }

  for (const auto* pObject : m_AllChildObjects)
  {
    m_WrittenGameObjectHandles[pObject->GetHandle()] = uiGameObjectIndex;
    ++uiGameObjectIndex;
  }
}

void ezWorldWriter::AssignComponentHandleIndices()
{
  // assign the component handle indices in the order in which the components are written
  ezUInt32 uiComponentIndex = 1;
  for (auto it = m_AllComponents.GetIterator(); it.IsValid(); ++it)
  {
    for (const auto* pComp : it.Value())
    {
      m_WrittenComponentHandles[pComp->GetHandle()] = uiComponentIndex;
      ++uiComponentIndex;
    }
  }
}

void ezWorldWriter::WriteGameObjectHandle(const ezGameObjectHandle& hObject)
{
  auto it = m_WrittenGameObjectHandles.Find(hObject);

  ezUInt32 uiIndex = 0;

  EZ_ASSERT_DEBUG(it.IsValid(), "Handle should always be in the written map at this point");

  if (it.IsValid())
    uiIndex = it.Value();

  *m_pStream << uiIndex;
}

void ezWorldWriter::WriteComponentHandle(const ezComponentHandle& hComponent)
{
  auto it = m_WrittenComponentHandles.Find(hComponent);

  EZ_ASSERT_DEBUG(it.IsValid(), "Handle should always be in the written map at this point");

  if (it.IsValid())
    *m_pStream << it.Value();
  else
  {
    const ezUInt32 uiInvalid = 0;
    *m_pStream << uiInvalid;
  }
}

bool ezWorldWriter::ObjectTraverser(ezGameObject* pObject)
{
  if (m_pExclude && pObject->GetTags().IsAnySet(*m_pExclude))
    return true;

  if (pObject->GetParent())
    m_AllChildObjects.PushBack(pObject);
  else
    m_AllRootObjects.PushBack(pObject);

  auto components = pObject->GetComponents();

  for (const auto* pComp : components)
  {
    m_AllComponents[pComp->GetDynamicRTTI()].PushBack(pComp);
  }

  m_uiNumComponents += components.GetCount();

  return true;
}

void ezWorldWriter::WriteGameObject(const ezGameObject* pObject)
{
  if (pObject->GetParent())
    WriteGameObjectHandle(pObject->GetParent()->GetHandle());
  else
    WriteGameObjectHandle(ezGameObjectHandle());

  ezStreamWriter& s = *m_pStream;

  s << pObject->GetName();
  s << pObject->GetLocalPosition();
  s << pObject->GetLocalRotation();
  s << pObject->GetLocalScaling();
  s << pObject->GetLocalUniformScaling();
  s << pObject->IsActive();
  s << pObject->IsDynamic();

  /// \todo tags, write strings only once
}


void ezWorldWriter::WriteComponentInfo(const ezRTTI* pRtti)
{
  ezStreamWriter& s = *m_pStream;

  s << pRtti->GetTypeName();
  s << pRtti->GetTypeVersion();
}

void ezWorldWriter::WriteComponentsOfType(const ezRTTI* pRtti, const ezDeque<const ezComponent*>& components, ezResourceHandleWriteContext& ResHandleWriter)
{
  ezMemoryStreamStorage storage;
  ezMemoryStreamWriter memWriter(&storage);

  ezStreamWriter* pPrevStream = m_pStream;
  m_pStream = &memWriter;

  ResHandleWriter.SetStream(m_pStream);

  // write to memory stream
  {
    ezStreamWriter& s = *m_pStream;
    s << components.GetCount();

    for (const auto* pComp : components)
    {
      WriteGameObjectHandle(pComp->GetOwner()->GetHandle());
      WriteComponentHandle(pComp->GetHandle());

      s << pComp->IsActive();
      s << pComp->IsDynamic();

      pComp->SerializeComponent(*this);
    }
  }

  m_pStream = pPrevStream;

  ResHandleWriter.SetStream(nullptr);

  // write result to actual stream
  {
    ezStreamWriter& s = *m_pStream;
    s << storage.GetStorageSize();

    s.WriteBytes(storage.GetData(), storage.GetStorageSize());
  }
}



EZ_STATICLINK_FILE(Core, Core_WorldSerializer_Implementation_WorldWriter);

