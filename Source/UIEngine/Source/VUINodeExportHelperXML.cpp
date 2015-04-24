#include "VUINodeExportHelperXML.h"



enum Alignment_e
{
	ALIGN_UNDEFINED = -1, ///< Alignment is undefined
	ALIGN_LEFT = 0,     ///< Horizontal alignment: left
	ALIGN_TOP = 0,      ///< Vertical aligment: top
	ALIGN_CENTER = 1,   ///< Horizontal/vertical alignment: center
	ALIGN_RIGHT = 2,    ///< Horizontal alignment: right
	ALIGN_BOTTOM = 2    ///< Vertical alignment: bottom
};


/// \brief
///   Enum that defines the 4 common states of a window
enum ControlState_e
{
	DISABLED  = 0,  ///< the window is disabled (grayed out)
	NORMAL    = 1,  ///< normal state
	MOUSEOVER = 2,  ///< the mouse is over the control
	SELECTED  = 3,  ///< the control is selected or highlighted because the user clicks on it

	STATE_COUNT = 4 ///< for array lookups
};

const char* ControlStateStr[] =
{
	"DISABLED",
	"NORMAL",
	"MOUSEOVER",
	"SELECTED",
	"STATE_COUNT"
};




const char* Alignment_Horz[] =
{
	"LEFT",
	"CENTER",
	"RIGHT"
};



const char* Alignment_Vert[] =
{
	"TOP",
	"CENTER",
	"BOTTOM"
};



VUINodeExportHelperXML VUINodeExportHelperXML::g_VUINodeExportHelperXML;

VUINodeExportHelperXML::VUINodeExportHelperXML(void)
{
}


VUINodeExportHelperXML::~VUINodeExportHelperXML(void)
{
}

VUINodeExportHelperXML &VUINodeExportHelperXML::GlobalManager()
{
	return g_VUINodeExportHelperXML;
}

bool VUINodeExportHelperXML::BuildImageState(VImageStates *state , VWindowBase *pOwner, TiXmlElement *pNode, const char *szPath, bool bWrite)
{
	if (!pNode)
		return false;

	// first, get the normal state from this node



	

	VImageStateBuild( &state->m_States[VWindowBase::NORMAL] , pOwner, pNode, szPath , bWrite );


	for( int i = 0 ; i < (int)STATE_COUNT ; i++ )
	{
		TiXmlElement *pStateNode= XMLHelper::SubNode(pNode,"statemodifier",bWrite);
		const char *szState = XMLHelper::Exchange_String(pStateNode,"state",ControlStateStr[i],bWrite);
		VImageStateBuild( &state->m_States[i] , pOwner, pStateNode, szPath , bWrite );
	}

	

	//VImageStateBuild( &state->m_States[VWindowBase::SELECTED] , pOwner, pNode, szPath , bWrite );
	//VImageStateBuild( &state->m_States[VWindowBase::DISABLED] , pOwner, pNode, szPath , bWrite );


	//state->m_States[VWindowBase::NORMAL].Build(pOwner,pNode,szPath,bWrite);

	// copy over to the other states...
// 	state->m_States[VWindowBase::MOUSEOVER] = state->m_States[VWindowBase::NORMAL];
// 	state->m_States[VWindowBase::SELECTED] = state->m_States[VWindowBase::NORMAL];
// 	state->m_States[VWindowBase::DISABLED] = state->m_States[VWindowBase::NORMAL];
// 
// 	// ...and assign the differences in the "statemodifier" subnodes
// 	for (TiXmlElement *pStateNode=pNode->FirstChildElement("statemodifier"); pStateNode; pStateNode=pStateNode->NextSiblingElement("statemodifier") )
// 	{
// 		const char *szState = XMLHelper::Exchange_String(pStateNode,"state",NULL,bWrite);
// 		VWindowBase::ControlState_e eState = VWindowBase::GetState(szState);
// 		state->m_States[eState].Build(pOwner,pStateNode,szPath,bWrite);
// 	}	

	return true;
}


bool VUINodeExportHelperXML::BuildTextState(VTextStates *states , VWindowBase *pOwner, TiXmlElement *pNode, const char *szPath, bool bWrite ,const VTextStates *pDefaults)
{
	
	if (!pNode)
		return false;

	// if defaults are defined, init the states with it
	if (pDefaults)
		(*states) = (*pDefaults);

	// first, get the normal state from this node
	VTextStateBuild( &states->m_States[VWindowBase::NORMAL] , pOwner, pNode, szPath , bWrite , states->GetScaling() );

	// assign defaults from normal state
// 	if (!pDefaults)
// 	{
// 		for (int i=0;i<VWindowBase::STATE_COUNT;i++)
// 			if (i!=VWindowBase::NORMAL)
// 				states->m_States[i] = states->m_States[VWindowBase::NORMAL];
// 	}
// 
// 	// then assign the values in the "statemodifier" subnodes
// 	for (TiXmlElement *pStateNode=pNode->FirstChildElement("statemodifier"); pStateNode; 
// 		pStateNode=pStateNode->NextSiblingElement("statemodifier") )
// 	{
// 		const char *szState = XMLHelper::Exchange_String(pStateNode,"state",NULL,bWrite);
// 		VWindowBase::ControlState_e eState = VWindowBase::GetState(szState);
// 		states->m_States[eState].Build(pOwner,pStateNode,szPath,bWrite);
// 	}

	// any empty strings left? then use the normal state
//	states->FinishSetup();
	return true;

}



bool VUINodeExportHelperXML::VImageStateBuild(VImageState *state , VWindowBase *pOwner, TiXmlElement *pNode, const char *szPath, bool bWrite)
{
	if (!pNode)
		return false;

	// button texture name
	if ( state->GetCurrentTexture() && bWrite )
	{
		const char *szTexture = XMLHelper::Exchange_String(pNode,"texture",state->GetCurrentTexture()->GetFilename(),bWrite);
	}
	else
	{
		const char *szTexture = XMLHelper::Exchange_String(pNode,"texture",NULL,bWrite);
		if (szTexture) // only assign when defined to preserve normal state
			state->SetTexture(VGUIManager::LoadTexture(szTexture,szPath));
	}

	// texture coordinates and stretch mode
	float *texCoordData = (float*)(&state->GetTextureRange().m_vMin);
	XMLHelper::Exchange_Floats(pNode,"texcoords",texCoordData,4,bWrite);
	
	if ( bWrite == true )
	{
		//XMLHelper::Exchange_String(pNode,"stretchmode",	"STRETCHED",bWrite);
	}
	else
	{
		const char *szStretchMode = XMLHelper::Exchange_String(pNode,"stretchmode",	NULL,bWrite);
		if (szStretchMode)
			state->SetStretchMode(VImageState::GetStretchMode(szStretchMode) );
	}	

	VColorRef color = state->GetColor();
	// button color
	XMLHelper::Exchange_Color(pNode,"color",color,bWrite);

	VIS_TransparencyType trans = state->GetTransparency();
	XMLHelper::Exchange_Transparency(pNode,"transparency",trans,bWrite);

	
	// other states
	//bool bFiltering = false;
	//XMLHelper::Exchange_Bool(pNode,"filtering",bFiltering,bWrite);
	//if (bFiltering)
	//	m_iAdditionalStateFlags |= RENDERSTATEFLAG_FILTERING;

	// mouse over cursor
	//const char *szCursor = XMLHelper::Exchange_String(pNode,"cursor",NULL,bWrite);
	//if (szCursor) // only assign when defined to preserve normal state
	//	m_spCursor = pOwner->GetMenuManager()->LoadCursorResource(szCursor,szPath);

	return true;
}



bool VUINodeExportHelperXML::VTextStateBuild(VTextState *state, VWindowBase *pOwner, TiXmlElement *pNode, const char *szPath, bool bWrite , float fScale )
{
	if (!pNode)
		return false;

	VGUIManager *pManager = pOwner->GetMenuManager();

	// mouse over cursor
// 	const char *szCursor = XMLHelper::Exchange_String(pNode,"cursor",NULL,bWrite);
// 	if (szCursor) // only assign when defined to preserve normal state
// 		m_spCursor = pManager->LoadCursorResource(szCursor,szPath);

	// custom text value

	//state->GetText();
	const char *szUTF8 = XMLHelper::Exchange_String(pNode,"text",state->GetText(),bWrite);
	const char *szText =  pManager->TranslateString(pOwner, szUTF8);  if (szText)
		state->SetText(szText, true); // sets custom text

	// text color

	VColorRef color = state->GetColor();

	XMLHelper::Exchange_Color(pNode,"color",color,bWrite);

	// font

	VisFont_cl* pFont = state->GetFont();

	const char *fontFileName= pFont->GetFilename();
	XMLHelper::Exchange_String(pNode,"font",fontFileName,bWrite);
// 	if (szFont || !state->GetFont()->GetFontName() >m_spFont) // preserve normal state
// 		state->m_spFont = pOwner->GetMenuManager()->LoadFont(szFont,szPath);

	
	// alignment
	const char *szAlign;
	szAlign = XMLHelper::Exchange_String(pNode,"halign",Alignment_Horz[state->GetHorizontalAlignment() ],bWrite);
	//szAlign = XMLHelper::Exchange_String(pNode,"halign",Alignment_Horz[state->GetHorizontalAlignment() ],bWrite);
	if (szAlign) 
		 state->SetHorizontalAlignment(VisFont_cl::GetAlignment(szAlign));

	szAlign = XMLHelper::Exchange_String(pNode,"valign",Alignment_Vert[state->GetVerticalAlignment() ],bWrite);
	//szAlign = XMLHelper::Exchange_String(pNode,"valign",Alignment_Vert[state->GetVerticalAlignment() ],bWrite);
	if (szAlign)
		state->SetVerticalAlignment(VisFont_cl::GetAlignment(szAlign));

	// relative offset

	bool bTextWrap = state->GetTextWrap();

	//float m_fFontScaling = state->GetScaling();
	//float m_fFontScaling = 1.0f; //state->GetScaling();


	//XMLHelper::Exchange_Floats(pNode,"ofs",state->GetTextOfs().data,2,bWrite);
	XMLHelper::Exchange_Bool(pNode,"textwrap",bTextWrap,bWrite);
	//XMLHelper::Exchange_Float(pNode,"lineheight",state->m_fRelativeFontHeight,bWrite);

	// scaling
	XMLHelper::Exchange_Float(pNode,"scaling",fScale,bWrite);


	return true;
}
