/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2013 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

////////////////////////////////////////////////////////////////////////////
//
// This project is a sample engine plugin DLL that contains the classes used
// by the engine and vForge for integrating it into the Vision Engine
// 
////////////////////////////////////////////////////////////////////////////
#include "UIEnginePluginPCH.h"
#include "VUINode.hpp"
#include "VUINodeDialog.h"
#include "VUINodeImage.h"
#include "VUINodeTextLabel.h"
#include "VUINodeCheckBox.h"
#include "VUINodeList.h"
#include "VUINodeSliderControl.h"
#include "VUINodeTextInput.h"
#include "VUINodePushButton.h"
#include "VUINodeColorButton.h"
#include "VUINodeStyleButton.h"
#include "VUINodeModelPreviewControl.h"
#include "VUINodePieGraph.h"
#include "VUINodeRotator.h"
#include "VUINodeNumericUpDown.h"
#include "VUINodeDraggingButton.h"

#include "UIEventHandlerComponent.h"


// This class implements the IVisPlugin_cl interface. The engine queries an instance of this class via 
// the exported GetEnginePlugin class.
class UIEnginePlugin_cl : public IVisPlugin_cl
{
public:

  // this function is only called once since an external init counter takes care
  VOVERRIDE void OnInitEnginePlugin()
  {
    Vision::RegisterModule(&g_NodeModule);



	//In some cases the compiler optimizes away the full class from the plugin since it seems to be dead code. 
	//One workaround to prevent this is to add the following helper macro into the plugin initialization code:
	FORCE_LINKDYNCLASS( VDefaultGUIContext );
	FORCE_LINKDYNCLASS( VUINodeDialog );
	FORCE_LINKDYNCLASS( VUINodeImage );
	FORCE_LINKDYNCLASS( VUINodeTextLabel );
	FORCE_LINKDYNCLASS( VUINodeCheckBox );
	FORCE_LINKDYNCLASS( VUINodeDialog );
	FORCE_LINKDYNCLASS( VUINodeList );
	FORCE_LINKDYNCLASS( VUINodePushButton );
	FORCE_LINKDYNCLASS( VUINodeSliderControl );
	FORCE_LINKDYNCLASS( VUINodeTextInput );
	FORCE_LINKDYNCLASS( VUINodeStyleButton);
	FORCE_LINKDYNCLASS( VUINodeColorButton);
	FORCE_LINKDYNCLASS( VUINodeModelPreviewControl);
	FORCE_LINKDYNCLASS( VUINodePieGraph);
	FORCE_LINKDYNCLASS( VUINodeRotator);
	FORCE_LINKDYNCLASS( VUINodeNumericUpDown );
	FORCE_LINKDYNCLASS( VUINodeDraggingButton );
	
	FORCE_LINKDYNCLASS( UIEventHandlerComponent );

	VUINodeMananger_cl::GlobalManager()->OneTimeInit();
  }

  // only called once
  VOVERRIDE void OnDeInitEnginePlugin()
  {
    Vision::UnregisterModule(&g_NodeModule);
    VUINodeMananger_cl::GlobalManager()->OneTimeDeInit();
  }
  
  VOVERRIDE const char *GetPluginName()
  {
    return "UIEnginePlugin";  //must match DLL name
  }
};


// our global instance of the plugin descriptor:
UIEnginePlugin_cl g_UIEnginePlugin;

// this plugin also declares a module which is needed for RTTI/serialization
DECLARE_THIS_MODULE( g_NodeModule, MAKE_VERSION(1, 0),
                     "Nodes", "Havok", "Vision Engine Nodes", &g_UIEnginePlugin );

////////////////////////////////////////////////////////////////////////////
// These functions are called directly by modules that statically link
// against this library. We give these function unique names so we don't 
// get any naming conflicts with other modules.
////////////////////////////////////////////////////////////////////////////

//  Use this to get and initialize the plugin when you link statically
VEXPORT IVisPlugin_cl* GetEnginePlugin_UIEnginePlugin()
{
  return &g_UIEnginePlugin;
}

#if (defined _DLL) || (defined _WINDLL)

//  The engine uses this to get and initialize the plugin dynamically
VEXPORT IVisPlugin_cl* GetEnginePlugin()
{
  return GetEnginePlugin_UIEnginePlugin();
}

#endif // _DLL or _WINDLL

/*
 * Havok SDK - Base file, BUILD(#20130624)
 * 
 * Confidential Information of Havok.  (C) Copyright 1999-2013
 * Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
 * Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
 * rights, and intellectual property rights in the Havok software remain in
 * Havok and/or its suppliers.
 * 
 * Use of this software for evaluation purposes is subject to and indicates
 * acceptance of the End User licence Agreement for this product. A copy of
 * the license is included with this software and is also available from salesteam@havok.com.
 * 
 */
