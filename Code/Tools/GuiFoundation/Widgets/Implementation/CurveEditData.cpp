#include <PCH.h>
#include <GuiFoundation/Widgets/CurveEditData.h>
#include <Foundation/Math/Math.h>
#include <Foundation/Math/Curve1D.h>

EZ_BEGIN_STATIC_REFLECTED_ENUM(ezCurveTangentMode, 1)
EZ_ENUM_CONSTANTS(ezCurveTangentMode::Bezier, ezCurveTangentMode::FixedLength, ezCurveTangentMode::Linear, ezCurveTangentMode::Auto)
EZ_END_STATIC_REFLECTED_ENUM()

EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezCurve1DControlPoint, 3, ezRTTIDefaultAllocator<ezCurve1DControlPoint>)
{
  EZ_BEGIN_PROPERTIES
  {
    EZ_MEMBER_PROPERTY("Time", m_fTime),
    EZ_MEMBER_PROPERTY("Value", m_fValue),
    EZ_MEMBER_PROPERTY("LeftTangent", m_LeftTangent)->AddAttributes(new ezDefaultValueAttribute(ezVec2(-0.1f, 0))),
    EZ_MEMBER_PROPERTY("RightTangent", m_RightTangent)->AddAttributes(new ezDefaultValueAttribute(ezVec2(+0.1f, 0))),
    EZ_MEMBER_PROPERTY("Linked", m_bTangentsLinked)->AddAttributes(new ezDefaultValueAttribute(true)),
    EZ_ENUM_MEMBER_PROPERTY("LeftTangentMode", ezCurveTangentMode, m_LeftTangentMode),
    EZ_ENUM_MEMBER_PROPERTY("RightTangentMode", ezCurveTangentMode, m_RightTangentMode),
  }
  EZ_END_PROPERTIES
}
EZ_END_DYNAMIC_REFLECTED_TYPE

EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezCurve1DData, 2, ezRTTIDefaultAllocator<ezCurve1DData>)
{
  EZ_BEGIN_PROPERTIES
  {
    EZ_MEMBER_PROPERTY("Color", m_CurveColor)->AddAttributes(new ezDefaultValueAttribute(ezColorGammaUB(ezColor::GreenYellow))),
    EZ_ARRAY_MEMBER_PROPERTY("ControlPoints", m_ControlPoints),
  }
  EZ_END_PROPERTIES
}
EZ_END_DYNAMIC_REFLECTED_TYPE

EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezCurve1DAssetData, 1, ezRTTIDefaultAllocator<ezCurve1DAssetData>)
{
  EZ_BEGIN_PROPERTIES
  {
    EZ_ARRAY_MEMBER_PROPERTY("Curves", m_Curves),
  }
  EZ_END_PROPERTIES
}
EZ_END_DYNAMIC_REFLECTED_TYPE


void ezCurve1DData::ConvertToRuntimeData(ezCurve1D& out_Result) const
{
  out_Result.Clear();

  for (const auto& cp : m_ControlPoints)
  {
    auto& ccp = out_Result.AddControlPoint(cp.m_fTime);
    ccp.m_Position.y = cp.m_fValue;
    ccp.m_LeftTangent = cp.m_LeftTangent;
    ccp.m_RightTangent = cp.m_RightTangent;
    ccp.m_TangentModeLeft = cp.m_LeftTangentMode;
    ccp.m_TangentModeRight = cp.m_RightTangentMode;
  }
}

void ezCurve1DAssetData::ConvertToRuntimeData(ezUInt32 uiCurveIdx, ezCurve1D& out_Result) const
{
  m_Curves[uiCurveIdx].ConvertToRuntimeData(out_Result);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#include <Foundation/Serialization/GraphPatch.h>
#include <Foundation/Serialization/AbstractObjectGraph.h>

class ezCurve1DControlPoint_2_3 : public ezGraphPatch
{
public:
  ezCurve1DControlPoint_2_3()
    : ezGraphPatch("ezCurve1DControlPoint", 3) {}

  virtual void Patch(ezGraphPatchContext& context, ezAbstractObjectGraph* pGraph, ezAbstractObjectNode* pNode) const override
  {
    auto* pPoint = pNode->FindProperty("Point");
    if (pPoint && pPoint->m_Value.IsA<ezVec2>())
    {
      ezVec2 pt = pPoint->m_Value.Get<ezVec2>();
      pNode->AddProperty("Time", (double)ezMath::Max(0.0f, pt.x));
      pNode->AddProperty("Value", (double)pt.y);
      pNode->AddProperty("LeftTangentMode", (ezUInt32)ezCurveTangentMode::Bezier);
      pNode->AddProperty("RightTangentMode", (ezUInt32)ezCurveTangentMode::Bezier);
    }
  }
};

ezCurve1DControlPoint_2_3 g_ezCurve1DControlPoint_2_3;