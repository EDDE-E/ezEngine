#pragma once

#include <Foundation/Types/UniquePtr.h>
#include <RmlUiPlugin/Resources/RmlUiResource.h>
#include <RmlUiPlugin/RmlUiPluginDLL.h>

#include <RmlUi/Include/RmlUi/Controls.h>
#include <RmlUi/Include/RmlUi/Core.h>

class ezRenderData;

namespace ezRmlUiInternal
{
  class Extractor;
  class EventListener;
} // namespace ezRmlUiInternal

class EZ_RMLUIPLUGIN_DLL ezRmlUiContext final : public Rml::Core::Context
{
public:
  ezRmlUiContext(const Rml::Core::String& name);
  ~ezRmlUiContext();

public:
  ezResult LoadDocumentFromResource(const ezRmlUiResourceHandle& hResource);
  ezResult LoadDocumentFromString(const ezStringView& sContent);

  void UnloadDocument();
  ezResult ReloadDocumentFromResource(const ezRmlUiResourceHandle& hResource);

  void ShowDocument();
  void HideDocument();

  void UpdateInput(const ezVec2& mousePos);
  bool WantsInput() const { return m_bWantsInput; }

  void SetOffset(const ezVec2I32& offset);
  void SetSize(const ezVec2U32& size);
  void SetDpiScale(float fScale);

  using EventHandler = ezDelegate<void(Rml::Core::Event&)>;

  void RegisterEventHandler(const char* szIdentifier, EventHandler handler);
  void DeregisterEventHandler(const char* szIdentifier);

private:
  bool HasDocument() { return GetNumDocuments() > 0; }

  friend class ezRmlUi;
  void ExtractRenderData(ezRmlUiInternal::Extractor& extractor);

  friend class ezRmlUiInternal::EventListener;
  void ProcessEvent(const ezHashedString& sIdentifier, Rml::Core::Event& event);

  ezVec2I32 m_Offset = ezVec2I32::ZeroVector();

  ezHashTable<ezHashedString, EventHandler> m_EventHandler;

  ezUInt64 m_uiExtractedFrame = 0;
  ezRenderData* m_pRenderData = nullptr;

  bool m_bWantsInput = false;
};

namespace ezRmlUiInternal
{
  class ContextInstancer : public Rml::Core::ContextInstancer
  {
  public:
    virtual Rml::Core::ContextPtr InstanceContext(const Rml::Core::String& name) override;
    virtual void ReleaseContext(Rml::Core::Context* context) override;

  private:
    virtual void Release() override;
  };
} // namespace ezRmlUiInternal
