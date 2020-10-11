#include <FoundationPCH.h>
#include <Foundation/Types/VariantTypeRegistry.h>
#include <Foundation/Reflection/Implementation/RTTI.h>

EZ_IMPLEMENT_SINGLETON(ezVariantTypeRegistry);

// clang-format off
EZ_BEGIN_SUBSYSTEM_DECLARATION(Foundation, VariantTypeRegistry)

BEGIN_SUBSYSTEM_DEPENDENCIES
"Reflection"
END_SUBSYSTEM_DEPENDENCIES

ON_CORESYSTEMS_STARTUP
{
  EZ_DEFAULT_NEW(ezVariantTypeRegistry);
}

ON_CORESYSTEMS_SHUTDOWN
{
  ezVariantTypeRegistry* pDummy = ezVariantTypeRegistry::GetSingleton();
  EZ_DEFAULT_DELETE(pDummy);
}

EZ_END_SUBSYSTEM_DECLARATION;
// clang-format on

ezVariantTypeRegistry::ezVariantTypeRegistry()
  : m_SingletonRegistrar(this)
{
  ezPlugin::s_PluginEvents.AddEventHandler(ezMakeDelegate(&ezVariantTypeRegistry::PluginEventHandler, this));

  UpdateTypes();
}

ezVariantTypeRegistry::~ezVariantTypeRegistry()
{
  ezPlugin::s_PluginEvents.RemoveEventHandler(ezMakeDelegate(&ezVariantTypeRegistry::PluginEventHandler, this));
}

const ezVariantTypeInfo* ezVariantTypeRegistry::FindVariantTypeInfo(const ezRTTI* pType) const
{
  const ezVariantTypeInfo* pTypeInfo = nullptr;
  m_TypeInfos.TryGetValue(pType, pTypeInfo);
  return pTypeInfo;
}

void ezVariantTypeRegistry::PluginEventHandler(const ezPluginEvent& EventData)
{
  UpdateTypes();
}

void ezVariantTypeRegistry::UpdateTypes()
{
  m_TypeInfos.Clear();
  ezVariantTypeInfo* pInstance = ezVariantTypeInfo::GetFirstInstance();

  while (pInstance)
  {
    EZ_ASSERT_DEV(pInstance->GetType()->GetAllocator()->CanAllocate(), "Custom type '{0}' needs to be allocatable.", pInstance->GetType()->GetTypeName());

    m_TypeInfos.Insert(pInstance->GetType(), pInstance);
    pInstance = pInstance->GetNextInstance();
  }
}

//////////////////////////////////////////////////////////////////////////

EZ_ENUMERABLE_CLASS_IMPLEMENTATION(ezVariantTypeInfo);

ezVariantTypeInfo::ezVariantTypeInfo()
{

}
