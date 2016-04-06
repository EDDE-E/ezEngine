#include <Foundation/PCH.h>
#include <Foundation/Reflection/Reflection.h>
#include <Foundation/Reflection/Implementation/PropertyAttributes.h>

EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezPropertyAttribute, 1, ezRTTINoAllocator);
EZ_END_DYNAMIC_REFLECTED_TYPE();

EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezReadOnlyAttribute, 1, ezRTTIDefaultAllocator<ezReadOnlyAttribute>);
EZ_END_DYNAMIC_REFLECTED_TYPE();

EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezHiddenAttribute, 1, ezRTTIDefaultAllocator<ezHiddenAttribute>);
EZ_END_DYNAMIC_REFLECTED_TYPE();

EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezCategoryAttribute, 1, ezRTTIDefaultAllocator<ezCategoryAttribute>);
EZ_BEGIN_PROPERTIES
EZ_MEMBER_PROPERTY("Category", m_sCategory)
EZ_END_PROPERTIES
EZ_END_DYNAMIC_REFLECTED_TYPE();

EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezDefaultValueAttribute, 1, ezRTTIDefaultAllocator<ezDefaultValueAttribute>);
EZ_BEGIN_PROPERTIES
EZ_MEMBER_PROPERTY("Value", m_Value),
EZ_END_PROPERTIES
EZ_END_DYNAMIC_REFLECTED_TYPE();

EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezClampValueAttribute, 1, ezRTTIDefaultAllocator<ezClampValueAttribute>);
EZ_BEGIN_PROPERTIES
EZ_MEMBER_PROPERTY("Min", m_MinValue),
EZ_MEMBER_PROPERTY("Max", m_MaxValue),
EZ_END_PROPERTIES
EZ_END_DYNAMIC_REFLECTED_TYPE();

EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezTypeWidgetAttribute, 1, ezRTTINoAllocator);
EZ_END_DYNAMIC_REFLECTED_TYPE();

EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezContainerWidgetAttribute, 1, ezRTTINoAllocator);
EZ_END_DYNAMIC_REFLECTED_TYPE();

EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezTagSetWidgetAttribute, 1, ezRTTIDefaultAllocator<ezTagSetWidgetAttribute>);
EZ_BEGIN_PROPERTIES
EZ_MEMBER_PROPERTY("Filter", m_sTagFilter)
EZ_END_PROPERTIES
EZ_END_DYNAMIC_REFLECTED_TYPE();

EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezContainerAttribute, 1, ezRTTIDefaultAllocator<ezContainerAttribute>);
EZ_BEGIN_PROPERTIES
EZ_MEMBER_PROPERTY("CanAdd", m_bCanAdd),
EZ_MEMBER_PROPERTY("CanDelete", m_bCanDelete),
EZ_MEMBER_PROPERTY("CanMove", m_bCanMove),
EZ_END_PROPERTIES
EZ_END_DYNAMIC_REFLECTED_TYPE();

EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezFileBrowserAttribute, 1, ezRTTIDefaultAllocator<ezFileBrowserAttribute>);
EZ_BEGIN_PROPERTIES
EZ_MEMBER_PROPERTY("Title", m_sDialogTitle),
EZ_MEMBER_PROPERTY("Filter", m_sTypeFilter)
EZ_END_PROPERTIES
EZ_END_DYNAMIC_REFLECTED_TYPE();

EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezAssetBrowserAttribute, 1, ezRTTIDefaultAllocator<ezAssetBrowserAttribute>);
EZ_BEGIN_PROPERTIES
EZ_MEMBER_PROPERTY("Filter", m_sTypeFilter)
EZ_END_PROPERTIES
EZ_END_DYNAMIC_REFLECTED_TYPE();

EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezDynamicEnumAttribute, 1, ezRTTIDefaultAllocator<ezDynamicEnumAttribute>);
EZ_BEGIN_PROPERTIES
EZ_MEMBER_PROPERTY("DynamicEnum", m_sDynamicEnumName)
EZ_END_PROPERTIES
EZ_END_DYNAMIC_REFLECTED_TYPE();

EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezDynamicStringEnumAttribute, 1, ezRTTIDefaultAllocator<ezDynamicStringEnumAttribute>);
EZ_BEGIN_PROPERTIES
EZ_MEMBER_PROPERTY("DynamicEnum", m_sDynamicEnumName)
EZ_END_PROPERTIES
EZ_END_DYNAMIC_REFLECTED_TYPE();

//////////////////////////////////////////////////////////////////////////

EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezManipulatorAttribute, 1, ezRTTINoAllocator);
  EZ_BEGIN_PROPERTIES
    EZ_MEMBER_PROPERTY("Property1", m_sProperty1),
    EZ_MEMBER_PROPERTY("Property2", m_sProperty2),
    EZ_MEMBER_PROPERTY("Property3", m_sProperty3),
    EZ_MEMBER_PROPERTY("Property4", m_sProperty4),
  EZ_END_PROPERTIES
EZ_END_DYNAMIC_REFLECTED_TYPE();


ezManipulatorAttribute::ezManipulatorAttribute(const char* szProperty1, const char* szProperty2 /*= nullptr*/, const char* szProperty3 /*= nullptr*/, const char* szProperty4 /*= nullptr*/)
{
  m_sProperty1 = szProperty1;
  m_sProperty2 = szProperty2;
  m_sProperty3 = szProperty3;
  m_sProperty4 = szProperty4;
}

//////////////////////////////////////////////////////////////////////////

EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezSphereManipulatorAttribute, 1, ezRTTIDefaultAllocator<ezSphereManipulatorAttribute>);
EZ_END_DYNAMIC_REFLECTED_TYPE();

ezSphereManipulatorAttribute::ezSphereManipulatorAttribute()
  : ezManipulatorAttribute(nullptr)
{

}

ezSphereManipulatorAttribute::ezSphereManipulatorAttribute(const char* szOuterRadius, const char* szInnerRadius)
  : ezManipulatorAttribute(szOuterRadius, szInnerRadius)
{

}

//////////////////////////////////////////////////////////////////////////

EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezCapsuleManipulatorAttribute, 1, ezRTTIDefaultAllocator<ezCapsuleManipulatorAttribute>);
EZ_END_DYNAMIC_REFLECTED_TYPE();

ezCapsuleManipulatorAttribute::ezCapsuleManipulatorAttribute()
  : ezManipulatorAttribute(nullptr)
{

}

ezCapsuleManipulatorAttribute::ezCapsuleManipulatorAttribute(const char* szLength, const char* szRadius, const ezVec3& up)
  : ezManipulatorAttribute(szLength, szRadius)
{
  m_vUp = up;
}


//////////////////////////////////////////////////////////////////////////

EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezBoxManipulatorAttribute, 1, ezRTTIDefaultAllocator<ezBoxManipulatorAttribute>);
EZ_END_DYNAMIC_REFLECTED_TYPE();

ezBoxManipulatorAttribute::ezBoxManipulatorAttribute()
  : ezManipulatorAttribute(nullptr)
{
}

ezBoxManipulatorAttribute::ezBoxManipulatorAttribute(const char* szSize)
  : ezManipulatorAttribute(szSize)
{

}

EZ_STATICLINK_FILE(Foundation, Foundation_Reflection_Implementation_PropertyAttributes);
