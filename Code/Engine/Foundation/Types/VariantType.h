#pragma once

#include <Foundation/Types/Types.h>
#include <Foundation/Strings/String.h>

class ezReflectedClass;
class ezVariant;
struct ezTime;
class ezUuid;
struct ezStringView;
struct ezTypedObject;
struct ezTypedPointer;

using ezDataBuffer = ezDynamicArray<ezUInt8>;
using ezVariantArray = ezDynamicArray<ezVariant>;
using ezVariantDictionary = ezHashTable<ezString, ezVariant>;

/// \brief This enum describes the type of data that is currently stored inside the variant.
struct ezVariantType
{
  using StorageType = ezUInt8;
  /// \brief This enum describes the type of data that is currently stored inside the variant.
  /// Note that changes to this enum require an increase of the reflection version and either
  /// patches to the serializer or a re-export of binary data that contains ezVariants.
  enum Enum : ezUInt8
  {
    Invalid = 0, ///< The variant stores no (valid) data at the moment.

    /// *** Types that are flagged as 'StandardTypes' (see DetermineTypeFlags) ***
    FirstStandardType = 1,
    Bool,       ///< The variant stores a bool.
    Int8,       ///< The variant stores an ezInt8.
    UInt8,      ///< The variant stores an ezUInt8.
    Int16,      ///< The variant stores an ezInt16.
    UInt16,     ///< The variant stores an ezUInt16.
    Int32,      ///< The variant stores an ezInt32.
    UInt32,     ///< The variant stores an ezUInt32.
    Int64,      ///< The variant stores an ezInt64.
    UInt64,     ///< The variant stores an ezUInt64.
    Float,      ///< The variant stores a float.
    Double,     ///< The variant stores a double.
    Color,      ///< The variant stores an ezColor.
    Vector2,    ///< The variant stores an ezVec2.
    Vector3,    ///< The variant stores an ezVec3.
    Vector4,    ///< The variant stores an ezVec4.
    Vector2I,   ///< The variant stores an ezVec2I32.
    Vector3I,   ///< The variant stores an ezVec3I32.
    Vector4I,   ///< The variant stores an ezVec4I32.
    Vector2U,   ///< The variant stores an ezVec2U32.
    Vector3U,   ///< The variant stores an ezVec3U32.
    Vector4U,   ///< The variant stores an ezVec4U32.
    Quaternion, ///< The variant stores an ezQuat.
    Matrix3,    ///< The variant stores an ezMat3. A heap allocation is required to store this data type.
    Matrix4,    ///< The variant stores an ezMat4. A heap allocation is required to store this data type.
    Transform,  ///< The variant stores an ezTransform. A heap allocation is required to store this data type.
    String,     ///< The variant stores a string. A heap allocation is required to store this data type.
    StringView, ///< The variant stores an ezStringView.
    DataBuffer, ///< The variant stores an ezDataBuffer, a typedef to DynamicArray<ezUInt8>. A heap allocation is required to store this data type.
    Time,       ///< The variant stores an ezTime value.
    Uuid,       ///< The variant stores an ezUuid value.
    Angle,      ///< The variant stores an ezAngle value.
    ColorGamma, ///< The variant stores an ezColorGammaUB value.
    LastStandardType,
    /// *** Types that are flagged as 'StandardTypes' (see DetermineTypeFlags) ***

    FirstExtendedType = 64,
    VariantArray,      ///< The variant stores an array of ezVariant's. A heap allocation is required to store this data type.
    VariantDictionary, ///< The variant stores a dictionary (hashmap) of ezVariant's. A heap allocation is required to store this type.
    TypedPointer,
    TypedObject,
    LastExtendedType,  ///< Number of values for ezVariant::Type.

    MAX_ENUM_VALUE = LastExtendedType,
    Default = Invalid ///< Default value used by ezEnum.
  };
};

EZ_DEFINE_AS_POD_TYPE(ezVariantType::Enum);

enum ezVariantClass
{
  Invalid,
  DirectCast, ///< A standard type
  PointerCast, ///< Any cast to T*
  TypedObject, ///< ezTypedObject cast
  ObjectCast, ///< Custom object types
};

/// \brief A helper struct to convert the C++ type, which is passed as the template argument, into one of the ezVariant::Type enum values.
template <typename T>
struct ezVariantTypeDeduction
{
  enum
  {
    value = ezVariantType::Invalid,
    forceSharing = false,
    hasReflectedMembers = false,
    classification = ezVariantClass::Invalid
  };

  using StorageType = T;
};

#define EZ_DECLARE_VARIANT_TYPE(TYPE)             \
  template <>                                     \
  struct ezVariantTypeDeduction<TYPE>             \
  {                                               \
    enum                                          \
    {                                             \
      value = ezVariantType::TypedObject,         \
      forceSharing = false,                       \
      hasReflectedMembers = true,                 \
      classification = ezVariantClass::ObjectCast \
    };                                            \
    using StorageType = TYPE;                     \
  };

#include <Foundation/Types/Implementation/VariantTypeDeduction_inl.h>
