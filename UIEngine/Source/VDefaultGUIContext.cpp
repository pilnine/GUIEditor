#include "UIEnginePluginPCH.h"
#include "UIEngineIncludes.hpp"
#include "VDefaultGUIContext.hpp"

#include "VUINode.hpp"
#include "VUINodeDialog.h"



#define THUMBSTICK_DEADZONE 5000
#define DOUBLECLICK_DELAY 0.2f
#define INVALIDATE_DOUBLECLICK {userState.m_fLastClickTime=-1.f;userState.m_iLastClickedMask=0;}


VDefaultGUIContext::VDefaultGUIContext() : VisTypedEngineObject_cl(),
	 VGUIMainContext(NULL)
{
// 	VGUIMainContext *m_pGuiMain = new VGUIMainContext(NULL);
// 	m_pGuiMain->SetDragThreshold(100.0f);

	m_ClientRect.Set(0.f,0.f, (float)Vision::Video.GetXRes(), (float)Vision::Video.GetYRes());

#ifdef WIN32
	m_fScaleX  = 1.0f;
	m_fScaleY  = 1.0f;
#else
	m_fScaleX  = 1.0f;
	m_fScaleY  = 1.0f;
#endif

this->SetDragThreshold(Vision::Video.GetDeviceDpi() / 96.0f * 10.0f);
}




 V_IMPLEMENT_SERIAL( VDefaultGUIContext, VisTypedEngineObject_cl, 0, &g_NodeModule );
 void VDefaultGUIContext::Serialize( VArchive &ar )
 {
   VisTypedEngineObject_cl::Serialize(ar); // call base class 
 }



 void VDefaultGUIContext::OnVariableValueChanged(VisVariable_cl *pVar, const char * value)
 {
	 VASSERT (pVar);
	 VASSERT (value);

	 if (pVar->name && strcmp(pVar->name, "TextColor") == 0)
	 {
		 int iComponents[4];
		 int iCount = sscanf(value,"%i/%i/%i/%i",&iComponents[0],&iComponents[1],&iComponents[2],&iComponents[3]);

		 VColorRef color(iComponents[0],iComponents[1],iComponents[2],iComponents[3]);
	//	 Text().SetColor( color );
		 return;
	 }	 
 }




 void VDefaultGUIContext::UpdateCursorPos(VGUIUserInfo_t &userState)
 {
	 VGUIMainContext::UpdateCursorPos(userState);
	
	 //convert to int
	 userState.m_vMousePos.x = floorf(userState.m_vMousePosAccum.x * (1 / m_fScaleX));
	 userState.m_vMousePos.y = floorf(userState.m_vMousePosAccum.y * (1 / m_fScaleY));

	 rReferenceResolution.Clamp(userState.m_vMousePos);
 }  

 void VDefaultGUIContext::OnRender()
 {
	 //VISION_PROFILE_FUNCTION(VGUIManager::PROFILING_RENDERING);
	 IVRender2DInterface *pRenderer = Vision::RenderLoopHelper.BeginOverlayRendering();
	 hkvVec4 scaling;
	 scaling.setZero();
	 scaling.x = m_fScaleX;
	 scaling.y = m_fScaleY;
	 pRenderer->SetTransformation(&scaling);
	 VItemRenderInfo defaultState(this, NULL);

	m_ClientRect.Set(0.f,0.f, (float)Vision::Video.GetXRes(), (float)Vision::Video.GetYRes());
	 VGraphicsInfo Graphics(*pRenderer, *this);



	 OnPaint(Graphics,defaultState);
	 Vision::RenderLoopHelper.EndOverlayRendering();
 }

 void VDefaultGUIContext::ShowDIalogByString(VString szDialog)
 { 
	 int iHashKey = VUINodeMananger_cl::GlobalManager()->GetUIKey(szDialog );

	 for ( int i = 0 ; i < GetOpenDialogs().Count() ; i++)
	 {
		 VUINodeDialog *dialog = (VUINodeDialog *)GetOpenDialogs().GetAt( i );
		 _int64 key = dialog->GetUniqueID();
		 if (key == iHashKey )
		 {			 
			 dialog->SetVisible( !dialog->IsVisible()  );
			 dialog->OnSetFocus(true);
			 dialog->SetOrder(10);
		 }
	 }
 }

 void VDefaultGUIContext::CloseDialog(VDialog* pDialog)
 {
	 if (pDialog == NULL || m_openDialogs.Contains(pDialog) == false )
		 return;

	 if (m_spFocusDlg == pDialog)
	 {
		 SetFocus(NULL); // otherwise it refuses to set the next one as focus dialog
		 if(pDialog->m_pOwner != NULL)
			SetFocus(static_cast<VDialog*>(pDialog->m_pOwner));
	 }

	 m_openDialogs.Remove(pDialog);
 }

hkvVec2 VDefaultGUIContext::GetGUIPositionFromScreenPos( hkvVec2 vPos )
{
	hkvVec2 vec(0,0);	

	vec.x = vPos.x * (1 / m_fScaleX);
	vec.y = vPos.y * (1 / m_fScaleY);

	return vec;
}

hkvVec2 VDefaultGUIContext::GetScreenPositionFromGUIPos( hkvVec2 vPos )
{
	hkvVec2 vec(0,0);	

	vec.x = vPos.x * m_fScaleX;
	vec.y = vPos.y * m_fScaleY;


	return vec;
}


 void VDefaultGUIContext::OnTickFunction(float fTimeDelta)
 {
	//IVGUIContext::OnTickFunction(fTimeDelta);	

//	 VISION_PROFILE_FUNCTION(VGUIManager::PROFILING_TICKFUNCTIONS);

	 for (int i=0;i<VGUIUserInfo_t::GUIMaxUser;i++)
	 {
		 if ((m_iUserIDMask&(1<<i))==0)
			 continue; // user active?

		 if(m_bCursorActive)
		 {
			 // Store old state
			 VGUIUserInfo_t &userState(m_UserStates[i]);
			 int iOldButton = userState.m_iButtonMask;
			 hkvVec2 vOldMousePos = userState.m_vMousePos;
			 VWindowBase *pOldMouseOver = userState.m_spMouseOverItem;

			 // Fetch new state
			 UpdateCursorPos(userState);
			 userState.m_iButtonMask = GetButtonMask(userState);

			 // Compute state deltas
			 int iClicked = (userState.m_iButtonMask & (~iOldButton)) & BUTTONMASK_CLICKEVENTS;
			 int iReleased = (iOldButton & (~userState.m_iButtonMask)) & BUTTONMASK_CLICKEVENTS;
			 hkvVec2 vMouseDelta = userState.m_vMousePos - vOldMousePos; 
			 hkvVec2 vLastClickDistance = userState.m_vMousePos - userState.m_vLastClickPos;     

			 if (vLastClickDistance.getLength() > m_fDragThreshold)
				 INVALIDATE_DOUBLECLICK;

			 // Reset dragging history on click
			 if(iClicked)
			 {
				 userState.m_vLastClickPos = userState.m_vMousePos;
				 userState.m_iDragHistorySize = 0;
				 vLastClickDistance.setZero();
			 }

			 if(userState.m_iButtonMask)
			 {
				 // Remove first entry if drag history is full
				 if(userState.m_iDragHistorySize >= V_ARRAY_SIZE(userState.m_vDragHistory))
				 {
					 userState.m_iDragHistorySize--;
					 memmove(&userState.m_vDragHistory[0], &userState.m_vDragHistory[1], userState.m_iDragHistorySize * sizeof(userState.m_vDragHistory[0]));
				 }

				 // Append current mouse position
				 userState.m_vDragHistory[userState.m_iDragHistorySize] = userState.m_vMousePos;
				 userState.m_iDragHistorySize++;
			 }

			 // Estimated current dragging speed (in px/frame)
			 //hkvVec2 vDragSpeed; 
			 if(userState.m_iDragHistorySize > 0)
			 {
				 m_vDragSpeed = (userState.m_vDragHistory[userState.m_iDragHistorySize - 1] - userState.m_vDragHistory[0]) / float(userState.m_iDragHistorySize);
			 }
			 else
			 {
				 m_vDragSpeed.setZero();
			 }

			 // tick function for all dialogs and tooltip
			 m_openDialogs.OnTickFunction(fTimeDelta);
			 if (m_spTooltip)
				 m_spTooltip->OnTick(fTimeDelta);

			 // if no focus dialog set, use the first in hierarchy
			 if (m_openDialogs.Count()>0)
			 {
				 if (!m_spFocusDlg)
					 m_spFocusDlg = m_openDialogs.GetAt(0);
				 userState.m_pCurrentCursor = m_spFocusDlg->GetCurrentCursor(userState);
			 }
			 else
			 {
				 m_spFocusDlg = NULL;
				 userState.m_pCurrentCursor = NULL;
			 }

			 if (!userState.m_pCurrentCursor && m_bShowCursor)
				 userState.m_pCurrentCursor = (m_spOverrideCursor!=NULL) ? m_spOverrideCursor.GetPtr() : GetManager()->GetDefaultCursor();

			 // mouse over
			 if (m_spFocusDlg != NULL)
			 {
				 VMenuEventDataObject data(NULL, this, NULL, NULL, userState, userState.m_iButtonMask);
				 m_spFocusDlg->FillEvent(&data); 

				 SetMouseOverItem(userState, data.m_pItem);
			 }

			 // Handle pointer down / up events
			 if(iClicked)
			 {
				 VMenuEventDataObject data(&OnPointerDown, this, NULL, NULL, userState, iClicked);
				 FillEvent(data);

				 if (m_spFocusDlg != NULL) 
					 m_spFocusDlg->OnPointerDown(&data);

				 if(data.m_pItem != NULL)
					 data.m_pItem->OnPointerDown(&data);

				 OnPointerDown.TriggerCallbacks(&data);
			 }

			 if (iReleased)
			 {
				 VMenuEventDataObject pointerData(&OnPointerUp, this, NULL, NULL, userState, iReleased);
				 FillEvent(pointerData);

				 VMenuEventDataObject itemData(&OnItemClick, this, NULL, NULL, userState, iReleased);
				 FillEvent(itemData);

				 if ( pointerData.m_pItem )
				 if (vLastClickDistance.getLength() <= m_fDragThreshold)
				 {
					 if (m_spFocusDlg != NULL)
						 m_spFocusDlg->OnPointerUp(&pointerData);

					 if(pointerData.m_pItem != NULL)
						 pointerData.m_pItem->OnPointerUp(&pointerData);

					 OnPointerUp.TriggerCallbacks(&pointerData);

					 if (m_spFocusDlg != NULL) 
						 m_spFocusDlg->OnClick(&itemData);

					 if (itemData.m_pItem != NULL && itemData.m_pItem == userState.m_spMouseDownItem)
					 {
						 VASSERT(itemData.m_pDialog);
						 itemData.m_pDialog->OnItemClicked(&itemData); // this one forwards it to the item
						 OnItemClick.TriggerCallbacks(&itemData);
					 }
				 }
				 else
				 {
					 // On touch devices, the delta is usually zero
					 // in the frame in which the touch was released, so 
					 // use the average drag delta
					 // to allow scrolling to continue after release.
					 if (userState.m_spDragItem)
					 {
						 userState.m_spDragItem->OnDragging(m_vDragSpeed);					
					 }
				 }

				 if (m_bMouseOverOnInputOnly && (userState.m_spMouseOverItem != NULL))
					 userState.m_spMouseOverItem->OnMouseLeave(userState);

				 userState.m_fLastClickTime = 0.f;
				 userState.m_iLastClickedMask = iReleased;
			 }

			 // dragging finished?
			 bool bDragging = IsDragging();
			 if (userState.m_spDragItem && userState.m_iButtonMask==0)
			 {
				 userState.m_spDragItem->OnDragEnd(userState.m_spMouseOverItem);
				 userState.m_spDragItem = NULL;

				 if (m_spFocusDlg != NULL)
				 {
					 VMenuEventDataObject pointerData(&OnPointerUp, this, NULL, NULL, userState, iReleased);
					 FillEvent(pointerData);
					 m_spFocusDlg->OnPointerUp(&pointerData);
				 }
			 }

			 // check for click and doubleclick
			 if (userState.m_fLastClickTime>=0.f)
				 userState.m_fLastClickTime+=fTimeDelta;

			 if (iClicked && !bDragging)
			 {
				 userState.m_spMouseDownItem = userState.m_spMouseOverItem;

				 if (iClicked == userState.m_iLastClickedMask)
				 {
					 if (userState.m_fLastClickTime>=0.f && userState.m_fLastClickTime<DOUBLECLICK_DELAY)
					 {
						 VMenuEventDataObject data(&OnItemDoubleClick, this, NULL,NULL,userState,iReleased);
						 if (m_spFocusDlg) // first, notify the dialog
							 m_spFocusDlg->OnDoubleClick(&data);
						 FillEvent(data);
						 if (data.m_pItem)
						 {
							 VASSERT(data.m_pDialog);
							 data.m_pDialog->OnItemDoubleClicked(&data); // forwards it to the item
							 OnItemDoubleClick.TriggerCallbacks(&data);
						 }
					 }
				 }
				 else
					 INVALIDATE_DOUBLECLICK;
			 }

			 // dragging?
			 if (userState.m_iButtonMask && vLastClickDistance.getLength() > m_fDragThreshold)
			 {
				 if (!userState.m_spDragItem && userState.m_spMouseOverItem)
				 {
					 userState.m_spDragItem = userState.m_spMouseOverItem;
					 userState.m_spDragItem->OnDragBegin(vOldMousePos, userState.m_iButtonMask); // pass the old mouse pos (where it started dragging)
					 m_vDragSpeed.setZero();
				 }
				 if (userState.m_spDragItem)
				 {
					 userState.m_spDragItem->OnDragging(vMouseDelta);  
					 m_vDragSpeed = vMouseDelta;
				 }
			 }

			 // show tooltip
			 if (vMouseDelta.getLength() > m_fDragThreshold)
			 {
				 SetTooltip(NULL); // remove tooltip if mouse is moving
			 } 
			 else if (userState.m_spMouseOverItem && m_spTooltip==NULL)
			 {
				 VTooltip *pTT = userState.m_spMouseOverItem->GetTooltip(userState);
				 SetTooltip(pTT);
			 }
		 }

		 if (m_bHandleKeyboard)
		 {
			 int iModifier = 0;

#ifdef SUPPORTS_KEYBOARD
#ifdef DO_NOT_USE_VGL
			 if (VInputManagerPC::GetKeyboard().GetRawControlValue(CT_KB_LSHIFT)) iModifier|=KEYMODIFIER_LSHIFT;
			 if (VInputManagerPC::GetKeyboard().GetRawControlValue(CT_KB_RSHIFT)) iModifier|=KEYMODIFIER_RSHIFT;
			 if (VInputManagerPC::GetKeyboard().GetRawControlValue(CT_KB_LCTRL)) iModifier|=KEYMODIFIER_LCTRL;
			 if (VInputManagerPC::GetKeyboard().GetRawControlValue(CT_KB_RCTRL)) iModifier|=KEYMODIFIER_RCTRL;
			 if (VInputManagerPC::GetKeyboard().GetRawControlValue(CT_KB_LALT)) iModifier|=KEYMODIFIER_LALT;
			 if (VInputManagerPC::GetKeyboard().GetRawControlValue(CT_KB_RALT)) iModifier|=KEYMODIFIER_RALT;
#else
			 if (VGLIsKeyPressed(VGLK_LSHIFT)) iModifier|=KEYMODIFIER_LSHIFT;
			 if (VGLIsKeyPressed(VGLK_RSHIFT)) iModifier|=KEYMODIFIER_RSHIFT;
			 if (VGLIsKeyPressed(VGLK_LCTRL)) iModifier|=KEYMODIFIER_LCTRL;
			 if (VGLIsKeyPressed(VGLK_RCTRL)) iModifier|=KEYMODIFIER_RCTRL;
			 if (VGLIsKeyPressed(VGLK_LALT)) iModifier|=KEYMODIFIER_LALT;
			 if (VGLIsKeyPressed(VGLK_RALT)) iModifier|=KEYMODIFIER_RALT;
#endif
#endif



#if !defined(VISION_GUI_USE_WINDOWS_INPUT)
			 // handle keyboard
			 if (m_spFocusDlg)
			 {
				 for (int iKey=1;iKey<255;iKey++)
					 if (HandleSingleKey(iKey))
						 m_spFocusDlg->OnKeyPressed(iKey, iModifier);
			 }
#endif
		 }
	 }
 }



 // *** The variable table is used by the plugin system of the engine and editor. In that case we do not have
 // *** variables which have to be initialised or which have to be available in the editor.
 START_VAR_TABLE(VDefaultGUIContext, VisTypedEngineObject_cl,"animated entity", VVARIABLELIST_FLAGS_NONE, NULL );
 varList->Add (VisVariable_cl::NewVariable ("Texture", "TextureFilename", VULPTYPE_VSTRING, -1, "White.dds", (VDisplayHintFlags_e)DISPLAY_HINT_TEXTUREFILE, 0, 0)  ); 
 END_VAR_TABLE
