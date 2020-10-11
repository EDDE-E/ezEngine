#pragma once

#include <Foundation/Basics.h>
#include <Foundation/Reflection/Implementation/StaticRTTI.h>
#include <Foundation/Types/TypeTraits.h>

struct EZ_FOUNDATION_DLL ezVarianceTypeBase
{
  EZ_DECLARE_POD_TYPE();

  float m_fVariance = 0;
};

EZ_DECLARE_REFLECTABLE_TYPE(EZ_FOUNDATION_DLL, ezVarianceTypeBase);

struct EZ_FOUNDATION_DLL ezVarianceTypeFloat : public ezVarianceTypeBase
{
  EZ_DECLARE_POD_TYPE();

  float m_Value = 0;
};

EZ_DECLARE_REFLECTABLE_TYPE(EZ_FOUNDATION_DLL, ezVarianceTypeFloat);

struct EZ_FOUNDATION_DLL ezVarianceTypeTime : public ezVarianceTypeBase
{
  EZ_DECLARE_POD_TYPE();

  ezTime m_Value;
};

EZ_DECLARE_REFLECTABLE_TYPE(EZ_FOUNDATION_DLL, ezVarianceTypeTime);

struct EZ_FOUNDATION_DLL ezVarianceTypeAngle : public ezVarianceTypeBase
{
  EZ_DECLARE_POD_TYPE();

  bool operator==(const ezVarianceTypeAngle& rhs) const
  {
    return m_fVariance == rhs.m_fVariance && m_Value == rhs.m_Value;
  }
  bool operator!=(const ezVarianceTypeAngle& rhs) const
  {
    return !(*this == rhs);
  }

  ezAngle m_Value;
};

template <>
struct ezHashHelper<ezVarianceTypeAngle>
{
  EZ_ALWAYS_INLINE static ezUInt32 Hash(const ezVarianceTypeAngle& value)
  {
    return ezHashingUtils::xxHash32(&value, sizeof(ezVarianceTypeAngle));
  }

  EZ_ALWAYS_INLINE static bool Equal(const ezVarianceTypeAngle& a, const ezVarianceTypeAngle& b)
  {
    return a == b;
  }
};

EZ_DECLARE_REFLECTABLE_TYPE(EZ_FOUNDATION_DLL, ezVarianceTypeAngle);

EZ_DECLARE_VARIANT_TYPE(ezVarianceTypeFloat);
EZ_DECLARE_VARIANT_TYPE(ezVarianceTypeTime);
EZ_DECLARE_VARIANT_TYPE(ezVarianceTypeAngle);
