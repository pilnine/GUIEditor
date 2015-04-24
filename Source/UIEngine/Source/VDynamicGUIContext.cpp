#include "UIEnginePluginPCH.h"
#include "UIEngineIncludes.hpp"
#include "VDynamicGUIContext.hpp"



V_IMPLEMENT_SERIAL( VDynamicGUIContext, VDefaultGUIContext, 0, &g_NodeModule );

VDynamicGUIContext::VDynamicGUIContext()
	: VDefaultGUIContext(NULL)
{
//	Init();
}

 void VDynamicGUIContext::UpdateCursorPos(VGUIUserInfo_t &userState)
 {
	 VGUIMainContext::UpdateCursorPos(userState);

	 float fScaleX = rReferenceResolution.GetSizeX() / m_ClientRect.GetSizeX();
	 float fScaleY = rReferenceResolution.GetSizeY() / m_ClientRect.GetSizeY();

	 //convert to int
	 userState.m_vMousePos.x = floorf(userState.m_vMousePosAccum.x * fScaleX);
	 userState.m_vMousePos.y = floorf(userState.m_vMousePosAccum.y * fScaleY);

	 rReferenceResolution.Clamp(userState.m_vMousePos);
 }  

 void VDynamicGUIContext::OnRender()
 {
	 //VISION_PROFILE_FUNCTION(VGUIManager::PROFILING_RENDERING);
	 IVRender2DInterface *pRenderer = Vision::RenderLoopHelper.BeginOverlayRendering();
	 hkvVec4 scaling;
	 scaling.x = m_ClientRect.GetSizeX() / rReferenceResolution.GetSizeX();
	 scaling.y = m_ClientRect.GetSizeY() / rReferenceResolution.GetSizeY();
	 pRenderer->SetTransformation(&scaling);
	 VItemRenderInfo defaultState(this, NULL);
	 VGraphicsInfo Graphics(*pRenderer, *this);
	 OnPaint(Graphics,defaultState);
	 Vision::RenderLoopHelper.EndOverlayRendering();

	 m_fScaleX = scaling.x;
	 m_fScaleY = scaling.y;
 }

 void VDynamicGUIContext::Serialize( VArchive &ar )
 {
	 VisTypedEngineObject_cl::Serialize(ar); // call base class 
 }

 // *** The variable table is used by the plugin system of the engine and editor. In that case we do not have
 // *** variables which have to be initialised or which have to be available in the editor.
 START_VAR_TABLE(VDynamicGUIContext, VisTypedEngineObject_cl,"animated entity", VVARIABLELIST_FLAGS_NONE, NULL );
 varList->Add (VisVariable_cl::NewVariable ("Texture", "TextureFilename", VULPTYPE_VSTRING, -1, "White.dds", (VDisplayHintFlags_e)DISPLAY_HINT_TEXTUREFILE, 0, 0)  ); 
 END_VAR_TABLE
