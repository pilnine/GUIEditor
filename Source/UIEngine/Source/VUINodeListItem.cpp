
#include "VUINodeListItem.h"


V_IMPLEMENT_SERIAL( VUINodeListItem, VListControlItem, 0, &g_NodeModule );

VUINodeListItem* VUINodeListItem::CreateInstance()
{
	// CreateInstance
	VUINodeListItem* pNewList = new VUINodeListItem("TEST", NULL );
	pNewList->SetSize( 300, 300 );
	return pNewList;
}


void VUINodeListItem::Serialize( VArchive &ar )
{
	VListControlItem::Serialize(ar);
	char iLocalVersion = 0;
	if (ar.IsLoading())
	{
	
	}
	else
	{	

	}
}
// 
// VColorRef( 68,  68,  68, 255), // COLOR_DIALOG_BG
// 	VColorRef(200, 200, 200, 255), // COLOR_TEXT
// 	VColorRef( 32,  32,  32, 255), // COLOR_LIST_CONTROL_BG
// 	VColorRef( 52,  52,  52, 255), // COLOR_LIST_ITEM_BG
// 	VColorRef( 42,  42,  42, 255), // COLOR_LIST_GROUP_BG
// 	VColorRef(200, 200, 200, 255), // COLOR_LIST_ITEM_TEXT_NORMAL
// 	VColorRef(255, 255, 255, 255), // COLOR_LIST_ITEM_TEXT_OVER
// 	VColorRef(253, 178,   0, 255)  // COLOR_LIST_ITEM_TEXT_SELECTED
VUINodeListItem::VUINodeListItem()
{

}

VUINodeListItem::VUINodeListItem(const char* szText, VTextureObject* pThumbnail )
{
	m_iCustomHeight = (int)(VUISharedData::GetIconSize() + 2 * VUISharedData::GetPadding());

	SetText(szText);
	m_Text.SetFont(  &Vision::Fonts.DebugFont() );
	m_Text.m_States[VWindowBase::NORMAL].SetColor(VColorRef(200, 200, 200, 255));
	m_Text.m_States[VWindowBase::MOUSEOVER].SetColor(VColorRef(255, 255, 255, 255));
	m_Text.m_States[VWindowBase::SELECTED].SetColor(VColorRef(253, 178,   0, 255));
	m_Text.SetVerticalAlignment(VisFont_cl::ALIGN_CENTER);
	m_Text.SetTextOfs(hkvVec2(VUISharedData::GetPadding(), 0.0f));
	m_Text.SetScaling(VUISharedData::GetFontScale());
	m_Text.FinishSetup();

	if ( pThumbnail == NULL)
	{
		pThumbnail = Vision::TextureManager.GetPlainWhiteTexture();
	}

	m_Icon.m_States[VWindowBase::NORMAL].SetTexture(pThumbnail);
}

VUINodeListItem::~VUINodeListItem()
{

}

void VUINodeListItem::OnPaint(VGraphicsInfo& Graphics, const VItemRenderInfo& parentState)
{
	if (IsClipped())
		return;

	VItemRenderInfo thisState(parentState, this, 1.0f);

	const int iItemCount = static_cast<VListControl*>(GetParent())->Items().Count();

	VRectanglef rect = thisState.m_pWindow->GetBoundingBox();
	const float fBorderWidth = VUISharedData::GetBorderWidth();
	rect.m_vMin += hkvVec2(fBorderWidth, (GetIndex() == 0) ? fBorderWidth : fBorderWidth * 0.5f);
	rect.m_vMax -= hkvVec2(fBorderWidth, (GetIndex() == iItemCount - 1) ? fBorderWidth : fBorderWidth * 0.5f);

	const bool bHighlight = (IsMouseOver() || IsSelected());
	VColorRef color;
	if ( bHighlight )
		color = VColorRef( 52,  52,  52, 255);
	else
		color = VColorRef( 42,  42,  42, 255);
	
	const VSimpleRenderState_t iProperties = VGUIManager::DefaultGUIRenderState();

	Graphics.Renderer.DrawSolidQuad(rect.m_vMin, rect.m_vMax, color, iProperties);

	// Item itself (text, etc.)
	VListControlItem::OnPaint(Graphics, parentState);
}

void VUINodeListItem::OnMeasureItem(const VRectanglef& area, hkvVec2& vCurrentPos)
{
	VListControlItem::OnMeasureItem(area, vCurrentPos);		
}


bool VUINodeListItem::Build(TiXmlElement *pNode, const char *szPath, bool bWrite)
{
	if (!VWindowBase::Build(pNode,szPath,bWrite))
		return false;

	XMLHelper::Exchange_Int(pNode,"data", m_iData,bWrite);
	m_Text.Build(m_pOwner,pNode,szPath,bWrite,m_pDefaultStates);

	TiXmlElement *pIconNode = XMLHelper::SubNode(pNode,"icon",bWrite);
	m_Icon.Build(m_pOwner,pIconNode,szPath,bWrite);

	return true;
}

// *** The variable table is used by the plugin system of the engine and editor. In that case we do not have
// *** variables which have to be initialised or which have to be available in the editor.
START_VAR_TABLE(VUINodeListItem, VisTypedEngineObject_cl,"animated entity", VVARIABLELIST_FLAGS_NONE, NULL );
 varList->Add (VisVariable_cl::NewVariable ("Texture", "Filename", VULPTYPE_VSTRING, -1, "", (VDisplayHintFlags_e)DISPLAY_HINT_TEXTUREFILE, 0, 0)  );
 varList->Add (VisVariable_cl::NewVariable ("ItemColor", "ItemColor", VULPTYPE_BYTE_COLOR4, -1, "60,80,255,255", (VDisplayHintFlags_e)DISPLAY_HINT_COLOR, 0, 0)  );
 varList->Add (VisVariable_cl::NewVariable ("ItemColorSelected", "ItemColorSelected", VULPTYPE_BYTE_COLOR4, -1, "140,180,255,255", (VDisplayHintFlags_e)DISPLAY_HINT_COLOR, 0, 0)  );
  DEFINE_VAR_VSTRING    (VUINodeListItem, m_szString,    "TEXT", "idle", 0, 0, 0);	 
END_VAR_TABLE
