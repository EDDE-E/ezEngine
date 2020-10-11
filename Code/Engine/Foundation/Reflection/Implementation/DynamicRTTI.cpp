#include "DynamicRTTI.h"

/// \brief Returns whether the type of this instance is of the given type or derived from it.
#include <FoundationPCH.h>

#include <Foundation/Reflection/Implementation/DynamicRTTI.h>
#include <Foundation/Reflection/Implementation/RTTI.h>

bool ezReflectedClass::IsInstanceOf(const ezRTTI * pType) const
{
  return GetDynamicRTTI()->IsDerivedFrom(pType);
}
