#pragma once

#include <GuiFoundation/Basics.h>
#include <Foundation/Strings/String.h>
#include <Foundation/Containers/Map.h>
#include <Foundation/Containers/DynamicArray.h>
#include <GuiFoundation/DocumentWindow/DocumentWindow.moc.h>
#include <ToolsFoundation/Project/ToolsProject.h>
#include <QMainWindow>

class ezDocumentManagerBase;
class ezDocumentBase;
struct ezDocumentTypeDescriptor;

class EZ_GUIFOUNDATION_DLL ezContainerWindow : public QMainWindow
{
  Q_OBJECT

public:
  ezContainerWindow();
  ~ezContainerWindow();

  static const ezDynamicArray<ezContainerWindow*>& GetAllContainerWindows() { return s_AllContainerWindows; }

  void MoveDocumentWindowToContainer(ezDocumentWindow* pDocWindow);

  static void CreateOrOpenDocument(bool bCreate);
  static void CreateOrOpenProject(bool bCreate);
  static void CreateOrOpenDocument(bool bCreate, const char* szFile);
  static void CreateOrOpenProject(bool bCreate, const char* szFile);

private:
  friend class ezDocumentWindow;

  ezResult EnsureVisible(ezDocumentWindow* pDocWindow);
  ezResult EnsureVisible(ezDocumentBase* pDocument);

  static ezResult EnsureVisibleAnyContainer(ezDocumentBase* pDocument);

private slots:
  void SlotDocumentTabCloseRequested(int index);
  void SlotRestoreLayout();
  void SlotTabsContextMenuRequested(const QPoint& pos);
  void SlotDocumentTabCurrentChanged(int index);

private:
  QTabWidget* GetTabWidget() const;
  static ezString BuildDocumentTypeFileFilter(bool bForCreation);

  void SaveWindowLayout();
  void RestoreWindowLayout();

  void UpdateWindowTitle();

  void RemoveDocumentWindowFromContainer(ezDocumentWindow* pDocWindow);
  void UpdateWindowDecoration(ezDocumentWindow* pDocWindow);

  void SetupDocumentTabArea();

  const char* GetUniqueName() const { return "ezEditor"; /* todo */ }

  void DocumentWindowEventHandler(const ezDocumentWindow::Event& e);
  void ProjectEventHandler(const ezToolsProject::Event& e);

  void closeEvent(QCloseEvent* e);

private:
  ezDynamicArray<ezDocumentWindow*> m_DocumentWindows;

  static ezDynamicArray<ezContainerWindow*> s_AllContainerWindows;
};




