#ifndef VUINode_HPP_INCLUDED
#define VUINode_HPP_INCLUDED

#include "UIEngineIncludes.hpp"
#include <Vision/Runtime/Engine/System/VisApiCallbacks.hpp>
//#include <Vision/Runtime/EnginePlugins/VisionEnginePlugin/GUI/VGUICommandBuffer.hpp>
//#include <Vision/Runtime/EnginePlugins/VisionEnginePlugin/GUI/VCursor.hpp>


#include "VDynamicGUIContext.hpp"
#include "VDefaultGUIContext.hpp"
#include "vCommonControls.hpp"
#include "VUINodeExportHelperXML.h"
#include "VUINodeDialog.h"

class VUINodeMananger_cl : public IVisCallbackHandler_cl
{
public:
  VUINodeMananger_cl()
  {
	  m_bRenderUI = false;
	  m_bEditorMode = false;
	  m_bInitialized = false;
	  m_bMenuResourceLoaded = false;
  }

  virtual ~VUINodeMananger_cl() { /*m_Instances.Clear();*/}

  NODE_IMPEXP void OneTimeInit();
  NODE_IMPEXP void OneTimeDeInit();

  NODE_IMPEXP void InitializeUIEngine();
  NODE_IMPEXP void DeInitializeUIEngine();

  NODE_IMPEXP void ClearDialogs();

  // access the global node manager instance
  NODE_IMPEXP static VUINodeMananger_cl* GlobalManager()
  {
	  if( g_VUINodeManager == NULL )
		  g_VUINodeManager = new VUINodeMananger_cl();

	  return g_VUINodeManager;
  }

  NODE_IMPEXP void ShowDialog(VDialog* vDialog);
  NODE_IMPEXP void CloseDialog(VDialog* vDialog);
  NODE_IMPEXP VDialog* FindDialog( VString szDialogClassName );

  bool m_bRenderUI;

#ifdef DYNAMIC_UI
  VSmartPtr<VDynamicGUIContext> m_pGUIContext;
  NODE_IMPEXP VSmartPtr<VDynamicGUIContext> GetGUIContext() const { return m_pGUIContext; }
  NODE_IMPEXP void SetGUIContext(VSmartPtr<VDynamicGUIContext> val) { m_pGUIContext = val; }
#else
  VDefaultGUIContextPtr m_pGUIContext;
  NODE_IMPEXP VDefaultGUIContextPtr GetGUIContext() const { return m_pGUIContext; }
  NODE_IMPEXP void SetGUIContext(VDefaultGUIContextPtr val) { m_pGUIContext = val; }
#endif

  NODE_IMPEXP int GetUIKey(VString szString);

  NODE_IMPEXP void SetEditorMode(bool bEditorMode)
  {
	  m_bEditorMode = bEditorMode;
  }

protected:
  VOVERRIDE void OnHandleCallback(IVisCallbackDataObject_cl *pData);

  bool m_bEditorMode;

  bool m_bInitialized;
  bool m_bMenuResourceLoaded;
 
 
  static VUINodeMananger_cl* g_VUINodeManager;
};


#endif