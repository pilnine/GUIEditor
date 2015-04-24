#include "VUINodeTextInput.h"

#define CURSOR_FREQ   1.3f
#define CURSOR_WIDTH  2.f

#define VERSION_MULTI_LINE


V_IMPLEMENT_SERIAL( VUINodeTextInput, VTextControl, 0, &g_NodeModule );

VUINodeTextInput::VUINodeTextInput(void)
{
	m_bMultiLine = false;
	m_vTextOfs = hkvVec2(0,0);
}


VUINodeTextInput::~VUINodeTextInput(void)
{
}


VUINodeTextInput* VUINodeTextInput::CreateInstance()
{
	// CreateInstance
	VUINodeTextInput* pNewImage = new VUINodeTextInput();
	pNewImage->m_bMultiLine = false;
	return pNewImage;
}


void VUINodeTextInput::Serialize( VArchive &ar )
{
	VTextControl::Serialize(ar);
	char iLocalVersion = 0;
	if (ar.IsLoading())
	{	
		ar >> m_bMultiLine;	
	}
	else
	{	
		ar << m_bMultiLine;
	}
}



bool VUINodeTextInput::Build(TiXmlElement *pNode, const char *szPath, bool bWrite) 
{
	if ( bWrite == false )
	{
		bool bResult = VTextControl::Build( pNode,szPath, bWrite);
		
		if ( m_Frame.GetTexture() == NULL )
		{
			m_Frame.SetTexture(Vision::TextureManager.GetPlainBlackTexture());
		}

		return bResult;
	}

	//if (!VDlgControlBase::Build(pNode,szPath,bWrite))
	//	return false;

	if ( GetUniqueID() != 0 )
	{
		VString szID = VGUIManager::GetIDName( (int)GetUniqueID() );	
		XMLHelper::Exchange_String(pNode,"ID",szID,bWrite);
	}	

	XMLHelper::Exchange_Floats(pNode,"pos",m_vPosition.data,2,bWrite);
	XMLHelper::Exchange_Floats(pNode,"size",m_vSize.data,2,bWrite);

	// frame

	TiXmlElement *pFrame = XMLHelper::SubNode(pNode,"frame",bWrite);
	if (pFrame)
	{
		if ( m_Frame.GetTexture() != NULL )
		{
			XMLHelper::Exchange_String(pFrame,"texture",m_Frame.GetTexture()->GetFilename(),bWrite);			
		}
		VColorRef color = m_Frame.GetColor();
		XMLHelper::Exchange_Color(pFrame, "color", color, bWrite);
	}	

	XMLHelper::Exchange_Int(pNode,"maxchars",m_iMaxChars,bWrite);

	VUINodeExportHelperXML::GlobalManager().BuildTextState( &m_Text , this , XMLHelper::SubNode(pNode,"text",bWrite) , szPath,bWrite );

	// text
// 	TiXmlElement *pTextNode = XMLHelper::SubNode(pNode,"text",bWrite);
// 	if (pTextNode)
// 	{
// 		m_Text.Build(this,pTextNode,szPath,bWrite);
// 		SetText(m_Text.GetText()); // sets the cursor pos
// 		SetPasswordChar(m_iPasswordChar);
// 
// 		m_vTextOfs += m_Text.GetTextOfs();
// 	}

	return true;
}


void VUINodeTextInput::OnVariableValueChanged(VisVariable_cl *pVar, const char * value)
{
	VASSERT (pVar);
	VASSERT (value);

	if (pVar->name && strcmp(pVar->name, "TextColor") == 0)
	{
		int iComponents[4];
		int iCount = sscanf(value,"%i/%i/%i/%i",&iComponents[0],&iComponents[1],&iComponents[2],&iComponents[3]);

		VColorRef color(iComponents[0],iComponents[1],iComponents[2],iComponents[3]);
		Text().SetColor( color );
		return;
	}
	else if (pVar->name && strcmp(pVar->name, "Text") == 0)
	{		
		Text().SetText( value );			
		return;
	}
	else if (pVar->name && strcmp(pVar->name, "Scale") == 0)
	{
		float fScale = 1.0f;
		int iCount = sscanf(value,"%f",&fScale);		
		Text().SetScaling( fScale );
		return;
	}
	else if (pVar->name && strcmp(pVar->name, "Font") == 0)
	{
		VisFontPtr spFont = VGUIManager::GlobalManager().LoadFont( value );		
		Text().SetFont( spFont );
		return;
	}
	else if (pVar->name && strcmp(pVar->name, "TextWrap") == 0)
	{
		if (value != NULL && strcmp(value, "True") == 0)
		{
			m_bMultiLine = true;
		}
		else
		{
			m_bMultiLine = false;
		}

		Text().SetTextWrap(m_bMultiLine);			
		return;
	}
	else if (pVar->name && strcmp(pVar->name, "Texture") == 0)
	{
		VTextureObject* spTexture = Vision::TextureManager.Load2DTexture( value );
		m_Frame.SetTexture( spTexture );
		return;
	}	
	if (pVar->name && strcmp(pVar->name, "FrameColor") == 0)
	{
		int iComponents[4];
		int iCount = sscanf(value,"%i/%i/%i/%i",&iComponents[0],&iComponents[1],&iComponents[2],&iComponents[3]);

		VColorRef color(iComponents[0],iComponents[1],iComponents[2],iComponents[3]);
		m_Frame.SetColor( color );
		return;
	}	
	else if (pVar->name && strcmp(pVar->name, "AlignmentVertical") == 0)
	{
		int iValue = 1;
		sscanf(value,"%d",&iValue);		
		Text().SetVerticalAlignment( (VisFont_cl::Alignment_e)iValue  );
	}
	else if (pVar->name && strcmp(pVar->name, "AlignmentHorizontal") == 0)
	{
		int iValue = 1;
		sscanf(value,"%d",&iValue);		
		Text().SetHorizontalAlignment( (VisFont_cl::Alignment_e)iValue  );
	}
}



void VUINodeTextInput::OnPaint(VGraphicsInfo &Graphics, const VItemRenderInfo &parentState)
{
	VDlgControlBase::OnPaint(Graphics,parentState);

	VItemRenderInfo thisState(parentState,this,1.f);

	// render frame
	m_Frame.OnPaint(Graphics,thisState);

	// render text
	m_Text.SetTextOfs(m_vTextOfs+hkvVec2(-m_fTextOfs,0.f));
	// set clipping rectangle
//	Graphics.ClippingStack.Push(GetClientRect(),true);
	m_Text.OnPaint(Graphics,thisState);

	// render blinking cursor
	if (m_fCursorPhase<(CURSOR_FREQ*0.5f) && HasFocus())
	{
		const VString &sText(m_iPasswordChar<0 ? m_sCurrentText : m_sPasswordString);

		// find out the cursor position
		VRectanglef rect;
		if (!m_Text.GetFont()->GetTextDimension(sText, rect, m_iCursorPos))
			rect.m_vMax.set(0.f,0.f);

		hkvVec2 p1 = GetAbsPosition() + m_Text.GetTextOfs();
		p1.x += rect.m_vMax.x;
		hkvVec2 p2 = p1 + hkvVec2(CURSOR_WIDTH,m_Text.GetFont()->GetFontHeight());
		VSimpleRenderState_t cursorState = VGUIManager::DefaultGUIRenderState();
		Graphics.Renderer.DrawSolidQuad(p1,p2,V_RGBA_WHITE,cursorState);
	}
//	Graphics.ClippingStack.Pop();
}


void VUINodeTextInput::SetText(const char *szText)
{
	if (m_sCurrentText!=szText)
	{
		m_sCurrentText=szText;
		SetCursorPosMultiLine(m_sCurrentText.GetLen(), false); // set cursor to the end
		SendTextChangedEvent();
	}
}


#ifdef SUPPORTS_SOFTKEYBOARD

void VUINodeTextInput::EnterText(const char* pUTF8Text)
{
	if(pUTF8Text == NULL)
		return;

	// insert a character
	if (!m_sValidChars.IsEmpty() && m_sValidChars.Find(pUTF8Text, true) < 0)
		return;
	if (m_iMaxChars > 0 && m_sCurrentText.GetLen() >= m_iMaxChars)
		return;

	m_sCurrentText.InsertAt(m_iCursorPos, pUTF8Text);
	SendTextChangedEvent();
	SetCursorPosMultiLine(m_iCursorPos+1);  
}


void VUINodeTextInput::OnSpecialKey(unsigned int uiKey)
{
	int iCurrentLength = m_sCurrentText.GetLen();

	switch(uiKey)
	{
		// back space
	case VGLK_BACKSP: 
		if (m_iCursorPos > 0)
		{
			m_sCurrentText.RemoveAt(m_iCursorPos-1);
			SendTextChangedEvent();
			SetCursorPosMultiLine(m_iCursorPos-1);
		}
		return;
		// delete
	case VGLK_DEL:
		if (m_iCursorPos < iCurrentLength)
		{
			m_sCurrentText.RemoveAt(m_iCursorPos);
			SendTextChangedEvent();
			SetCursorPosMultiLine(m_iCursorPos);
		}
		return;
		// cursor keys
	case VGLK_LEFT:
		SetCursorPosMultiLine(m_iCursorPos-1);
		return;
	case VGLK_RIGHT:
		SetCursorPosMultiLine(m_iCursorPos+1);
		return;
		// home
	case VGLK_HOME:
		SetCursorPosMultiLine(0);
		return;
		// end
	case VGLK_END:
		SetCursorPosMultiLine(iCurrentLength);
		return;      
	}
}

#endif



void VUINodeTextInput::OnClick(VMenuEventDataObject *pEvent)
{
	if (pEvent->m_iButtons != BUTTON_LMOUSE)
	{
		// no left button, so nothing to be done here
		return;
	}

	VDlgControlBase::OnClick(pEvent);

	const VString &sText(m_iPasswordChar < 0 ? m_sCurrentText : m_sPasswordString);

	// relative position inside text
	hkvVec2 vRelPos = pEvent->m_vMousePos - GetAbsPosition() - m_Text.GetTextOfs();

	// find character at mouse click position
	SetCursorPosMultiLine(m_Text.GetFont()->GetCharacterIndexAtPos(sText, vRelPos.x), true);
}


void VUINodeTextInput::SetCursorPosMultiLine(int iCharacterPos, bool bEnsureVisible)
{
	// int iOldPos = m_iCursorPos;
	// clamp to valid range
	const VString &sText(m_iPasswordChar < 0 ? m_sCurrentText : m_sPasswordString);

	if (iCharacterPos < 0) iCharacterPos = 0; 
	else if (iCharacterPos > sText.GetLen())
		iCharacterPos = sText.GetLen();

	m_iCursorPos = iCharacterPos;
	//if (iOldPos!=m_iCursorPos) 
	m_fCursorPhase = 0.f; // reset the cursor phase

	if (bEnsureVisible)
	{
		hkvVec2 vSize = GetClientRect().GetSize();
		float fCursorPos = -m_fTextOfs;
		VRectanglef textRect;
		if (m_Text.GetFont()->GetTextDimension(sText,textRect,m_iCursorPos))
			fCursorPos += textRect.m_vMax.x;

		if (fCursorPos <= 0.f || fCursorPos >= vSize.x) // recenter the cursor if cursor pos is out of range
		{
			//      float fDiff = fCursorPos - vSize.x*0.5f;
			//       m_fTextOfs += fDiff;
			//       if (m_fTextOfs < 0.f)
			//         m_fTextOfs=0.f;
		}
	}
}


// *** The variable table is used by the plugin system of the engine and editor. In that case we do not have
// *** variables which have to be initialised or which have to be available in the editor.
START_VAR_TABLE(VUINodeTextInput, VisTypedEngineObject_cl,"animated entity", VVARIABLELIST_FLAGS_NONE, NULL );
 DEFINE_VAR_INT(VUINodeTextInput, m_iMaxChars, "Max Character", "20", 0, 0);
 varList->Add (VisVariable_cl::NewVariable ("Texture", "TextureFilename", VULPTYPE_VSTRING, -1, "White.dds", (VDisplayHintFlags_e)DISPLAY_HINT_TEXTUREFILE, 0, 0)  );
 varList->Add (VisVariable_cl::NewVariable ("Text", "TextureFilename", VULPTYPE_VSTRING, -1, "TEXT_LABEL", (VDisplayHintFlags_e)DISPLAY_HINT_NONE, 0, 0)  );
 varList->Add (VisVariable_cl::NewVariable ("TextColor", "TextColor", VULPTYPE_BYTE_COLOR4, -1, "255,255,255,255", (VDisplayHintFlags_e)DISPLAY_HINT_COLOR, 0, 0)  );
 varList->Add (VisVariable_cl::NewVariable ("Scale", "TextScale", VULPTYPE_FLOAT, -1, "2.0", (VDisplayHintFlags_e)DISPLAY_HINT_NONE, 0, 0)  );	
 varList->Add (VisVariable_cl::NewVariable ("Font", "Font", VULPTYPE_VSTRING, -1, "Fonts\\DebugFont.fnt", (VDisplayHintFlags_e)DISPLAY_HINT_CUSTOMFILE, 0, 0)  );
 varList->Add (VisVariable_cl::NewVariable ("TextWrap", "TextWrap", VULPTYPE_BOOL, -1, "True", DISPLAY_HINT_NONE, 0, 0)  );
 varList->Add (VisVariable_cl::NewVariable ("FrameColor", "FrameColor", VULPTYPE_BYTE_COLOR4, -1, "255,255,0,255", (VDisplayHintFlags_e)DISPLAY_HINT_COLOR, 0, 0)  );
 varList->Add (VisVariable_cl::NewVariable ("AlignmentVertical", "AlignmentVertical", VULPTYPE_INT, -1, "ALIGN_CENTER", DISPLAY_HINT_NONE, "CSharpType(AlignmentVertical)", 0)  );
 varList->Add (VisVariable_cl::NewVariable ("AlignmentHorizontal", "AlignmentHorizontal", VULPTYPE_INT, -1, "ALIGN_CENTER", DISPLAY_HINT_NONE, "CSharpType(AlignmentHorizontal)", 0)  );
END_VAR_TABLE
