/// \file VGUIContext.hpp

#ifndef VDYNAMIC_GUI_CONTEXT_HPP_INCLUDED
#define VDYNAMIC_GUI_CONTEXT_HPP_INCLUDED

#include <Vision/Runtime/EnginePlugins/VisionEnginePlugin/GUI/VGUIIncludes.hpp>

#include "VDefaultGUIContext.hpp"

/// \brief
///   Resolution aware GUI context
class VDynamicGUIContext : public VDefaultGUIContext
{
protected:
  VRectanglef rReferenceResolution;

public:

  NODE_IMPEXP VDynamicGUIContext(void);

  /// \brief
  ///   Constructor of the GUI context instance
  NODE_IMPEXP VDynamicGUIContext(VGUIManager *pManager, float fReferenceResolutionX = 2048.0f, float fReferenceResolutionY = 1536.0f) :
  VDefaultGUIContext(pManager), rReferenceResolution(0, 0, fReferenceResolutionX, fReferenceResolutionY) {};

  /// \brief
  ///   Destructor
  virtual ~VDynamicGUIContext() {};


  virtual void UpdateCursorPos(VGUIUserInfo_t &userState);
  
  virtual void OnRender();  

  // serialisation:
  V_DECLARE_SERIAL_DLLEXP( VDynamicGUIContext,  NODE_IMPEXP );
  NODE_IMPEXP virtual void Serialize( VArchive &ar );

  IMPLEMENT_OBJ_CLASS(VDynamicGUIContext);
};

typedef VSmartPtr<VDynamicGUIContext> VDynamicGUIContextPtr;

#endif

