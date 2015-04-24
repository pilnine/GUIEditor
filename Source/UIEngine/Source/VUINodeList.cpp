#include "VUINodeList.h"


V_IMPLEMENT_SERIAL( VUINodeList, VListControl, 0, &g_NodeModule );

VUINodeList::VUINodeList(void)
{

}


VUINodeList::~VUINodeList(void)
{
}

VUINodeList* VUINodeList::CreateInstance()
{
	// CreateInstance
	VUINodeList* pNewList = new VUINodeList();


	float m_fScale = 2.0f;


	VSliderControl* pScrollbar = new VSliderControl();
	pScrollbar->SetSize(16.0f * m_fScale, 1.0f);
	pScrollbar->SetSliderRange(0, 10, 0);
	pScrollbar->Frame().m_States[VWindowBase::NORMAL].SetTexture(Vision::TextureManager.Load2DTexture("\\Textures\\vapp_scrollbar.dds"));
	pScrollbar->GetSlider()->Images().m_States[VWindowBase::NORMAL].SetTexture(Vision::TextureManager.Load2DTexture("\\Textures\\vapp_slider.dds"));
	pScrollbar->GetSlider()->Images().m_States[VWindowBase::NORMAL].SetTextureRange(VRectanglef(-m_fScale*0.5f, -m_fScale*0.5f, m_fScale*0.5f, m_fScale*0.5f));
	pScrollbar->GetSlider()->OnBuildFinished();

	VListControl *pListControl = new VListControl();
	pListControl->SetPosition( 400,100);
	pListControl->SetSize( 500,600 );
	pListControl->SetBackgroundColor( VColorRef(255,125,64,255));
	pListControl->m_bAllowSelection = true;
	pListControl->SetScrollBar(pScrollbar);
	//m_pDialogLogo->AddControl( pListControl );
	//VDialog *pDlg = VUINodeMananger_cl::GlobalManager()->GetGUIContext()->GetOpenDialogs().GetAt(0);
	//pDlg->AddControl(pNewList );


	float s_iItemHeight  =50.0f;
	for ( int i = 0 ; i < 50 ;i++ )
	{
		VUINodeListItem *pItem = new VUINodeListItem();
		pItem->SetParent(pListControl);
		VString szText;
		szText.Format( "Test Item %d", i);		
		pItem->SetText(szText);
		pListControl->AddItem( pItem);

		pItem->m_Text.SetColor( VColorRef(0,255,255,255) );

		pItem->m_iCustomHeight = (int)(s_iItemHeight*m_fScale);
		//pItem->m_Text.SetFont( Vis);
		pItem->m_Text.SetTextOfs(hkvVec2(20.0f*m_fScale, 10.0f));
		pItem->m_Text.m_States[VWindowBase::NORMAL].SetColor( VColorRef(200, 200, 200, 255) );
		pItem->m_Text.m_States[VWindowBase::MOUSEOVER].SetColor( VColorRef(255, 255, 255, 255) );
		pItem->m_Text.m_States[VWindowBase::SELECTED].SetColor( VColorRef(253, 178,   0, 255) );
		pItem->m_Text.SetScaling(m_fScale * 2.0f);
		pItem->m_Text.SetVerticalAlignment(VisFont_cl::ALIGN_CENTER);
		//   pItem->SetGroupIcon(m_spItemGroup);
		//	   pItem->SetActiveIcon(m_spItemActive);
	}

	

	return pNewList;
}




void VUINodeList::Serialize( VArchive &ar )
{
	VListControl::Serialize(ar);
	char iLocalVersion = 0;
	if (ar.IsLoading())
	{
	
	}
	else
	{	

	}
}



bool VUINodeList::Build(TiXmlElement *pNode, const char *szPath, bool bWrite) 
{
	if ( bWrite == false )
	{		
		return VListControl::Build( pNode,szPath,bWrite);
	}

	if ( GetUniqueID() != 0 )
	{
		VString szID = VGUIManager::GetIDName( (int)GetUniqueID() );	
		XMLHelper::Exchange_String(pNode,"ID",szID,bWrite);
	}		

	XMLHelper::Exchange_Floats(pNode,"pos",m_vPosition.data,2,bWrite);
	XMLHelper::Exchange_Floats(pNode,"size",m_vSize.data,2,bWrite);

	// background color
	XMLHelper::Exchange_Color(XMLHelper::SubNode(pNode,"background",bWrite),"color",m_iBackgroundCol,bWrite);

	// selection
	XMLHelper::Exchange_Bool(pNode,"allowselection",m_bAllowSelection,bWrite);

	// icons
	XMLHelper::Exchange_Float(pNode,"iconsize", m_fIconSize, bWrite);
	XMLHelper::Exchange_Floats(pNode,"iconofs", m_vIconOfs.data, 2, bWrite);

	// load items
	m_Items.Build(XMLHelper::SubNode(pNode,"listitems",bWrite), szPath, bWrite);

	// scroll bars
	TiXmlElement *pScrollV = XMLHelper::SubNode(pNode,"vscrollbar",bWrite);
	if (pScrollV)
	{
		m_spVScrollbar = new VSliderControl();
		m_spVScrollbar->SetParent(this);
		m_spVScrollbar->SetVertical(true);
		m_spVScrollbar->Build(pScrollV,szPath,bWrite);
		m_spVScrollbar->OnBuildFinished();
	}
	m_bCollectionChanged = true;
	return true;
}


void VUINodeList::OnPaint(VGraphicsInfo &Graphics, const VItemRenderInfo &parentState)
{
	VDlgControlBase::OnPaint(Graphics,parentState);

	// background:
	if (m_iBackgroundCol.a>0)
	{
		VRectanglef bbox = GetBoundingBox();
		VSimpleRenderState_t state = VGUIManager::DefaultGUIRenderState();
		Graphics.Renderer.DrawSolidQuad(bbox.m_vMin,bbox.m_vMax,m_iBackgroundCol,state);
	}

	// items
	if (m_Items.Count()>0)
	{
		VItemRenderInfo thisInfo(parentState,this,1.f);
		hkvVec2 vScrollPos(0.f, -m_fCurrentScrollPos);

		if (m_spVScrollbar)
		{
			if (m_bCollectionChanged)
			{
				m_Items.MeasureItems(VRectanglef(0,0,m_vSize.x,m_vSize.y),vScrollPos); // otherwise size isn't always correct
				m_bCollectionChanged = false;
				hkvVec2 vItemsSize = m_Items.GetSize();
				float fRange = vItemsSize.y - GetSize().y;
				m_spVScrollbar->SetVisible(fRange>0.f);
				if (fRange>0.f)
				{
					m_spVScrollbar->SetSliderRange(0.f,fRange, 0);
					m_spVScrollbar->SetSliderRelSize(GetSize().y/vItemsSize.y);
				}
			}
			if (m_spVScrollbar->IsVisible())
			{
				if (m_bFromScrollbar)
					m_fCurrentScrollPos = m_spVScrollbar->GetValue();
				else
					m_spVScrollbar->SetValue(m_fCurrentScrollPos);
			}

			m_bFromScrollbar = true;
			vScrollPos.y = -m_fCurrentScrollPos;
		}

		// pass the relative clipping rectangle to measureitems
		m_Items.MeasureItems(VRectanglef(0,0,m_vSize.x,m_vSize.y),vScrollPos);

		// Apply GUI Context Scale
		VRectanglef rect = GetBoundingBox();
		rect *= VUINodeMananger_cl::GlobalManager()->GetGUIContext()->GetScale();

		Graphics.ClippingStack.Push(rect,true);
		m_Items.PaintAll(Graphics,thisInfo);
		Graphics.ClippingStack.Pop();
	}
	else if (m_spVScrollbar)
		m_spVScrollbar->SetVisible(false);

	// paint scroll bar
	if (m_spVScrollbar && m_spVScrollbar->IsVisible())
		m_spVScrollbar->OnPaint(Graphics,parentState);
	 return;
}

void VUINodeList::SetScrollBar2( VSliderControl* pScrollbar )
{
	pScrollbar->SetParent(this);
	pScrollbar->SetVertical( true );

	
	hkvVec2 vSize = GetSize();
	hkvVec2 vScrollBarSize = pScrollbar->GetSize();

	//pScrollbar->SetSize( 10.0f, vSize.y - 10.0f);
	pScrollbar->SetSliderRange(0,100,0);

	pScrollbar->SetSliderRelSize(0);

	pScrollbar->OnBuildFinished();


	pScrollbar->GetSlider()->SetSize( 10.0f, 10.0f);
	pScrollbar->GetSlider()->OnBuildFinished();
	

	// return GetSize() - GetSlider()->GetSize() - m_BorderSize.m_vMin - m_BorderSize.m_vMax;

	m_spVScrollbar = pScrollbar;



	m_spVScrollbar->SetVisible( bShowScrollBar );
}

void VUINodeList::OnVariableValueChanged(VisVariable_cl *pVar, const char * value)
{
	VASSERT (pVar);
	VASSERT (value);

	if (pVar->name && strcmp(pVar->name, "Texture") == 0)
	{
	
	}	
	if (pVar->name && strcmp(pVar->name, "bShowScrollBar") == 0)
	{
		if (m_spVScrollbar == NULL)
		{
			float m_fScale = 2.0f;

			VSliderControl* pScrollbar = new VSliderControl();
			pScrollbar->SetSize(16.0f*m_fScale, 1.0f);
			pScrollbar->SetSliderRange(0, 100, 0);
			pScrollbar->Frame().m_States[VWindowBase::NORMAL].SetTexture(Vision::TextureManager.Load2DTexture("\\Textures\\vapp_scrollbar.dds"));
			pScrollbar->GetSlider()->Images().m_States[VWindowBase::NORMAL].SetTexture(Vision::TextureManager.Load2DTexture("\\Textures\\vapp_slider.dds"));
			pScrollbar->GetSlider()->Images().m_States[VWindowBase::NORMAL].SetTextureRange(VRectanglef(-m_fScale*0.5f, -m_fScale*0.5f, m_fScale*0.5f, m_fScale*0.5f));
			pScrollbar->GetSlider()->OnBuildFinished();


			

			pScrollbar->SetVisible(true);

	
			//	m_pListControl = new VListControl();
			//	m_pListControl->SetPosition(s_fBorderWidth, s_fBorderWidth + m_pLabel->GetSize().y);
			//	m_pListControl->SetBackgroundColor(VAppMenuColors::GetColor(VAppMenuColors::COLOR_LIST_CONTROL_BG));
			this->SetScrollBar(pScrollbar);
			AdjustScrollBars();

			hkvVec2 vScrollbarSize = pScrollbar->GetSize();
			hkvVec2 vSliderSize = pScrollbar->GetSlider()->GetSize();

			hkvVec2 vMoveRange = vScrollbarSize - vSliderSize;
			VASSERT(vMoveRange.x>=0.f && vMoveRange.y>=0.f);


			//	AddControl(m_pListControl);
		}

		m_spVScrollbar->SetVisible( bShowScrollBar );		
	}	
	if (pVar->name && strstr(pVar->name, "Color") == 0)
	{
		int iComponents[4];
		int iCount = sscanf(value,"%i/%i/%i/%i",&iComponents[0],&iComponents[1],&iComponents[2],&iComponents[3]);

		VColorRef color(iComponents[0],iComponents[1],iComponents[2],iComponents[3]);

		if ( strcmp(pVar->name, "ItemColorSelected") == 0 )
		{
			m_Items.m_DefaultProperties.m_States[VWindowBase::SELECTED].SetColor( color );
		}		
		else if ( strcmp(pVar->name, "ItemColor") == 0 )
		{
			m_Items.m_DefaultProperties.SetColor( color );
		}
		else if ( strcmp(pVar->name, "SliderColor") == 0 )
		{
			m_Items.m_DefaultProperties.SetColor( color );
		}
		else if ( strcmp(pVar->name, "SliderFrameColor") == 0 )
		{
			m_Items.m_DefaultProperties.SetColor( color );
		}

	}

	if (pVar->name && strcmp(pVar->name, "Text") == 0)
	{
		m_Items.m_DefaultProperties.SetText( pVar->name );
	}

	if (pVar->name && strcmp(pVar->name, "Scale") == 0)
	{
		float fScale = 1.0f;
		int iCount = sscanf(value,"%f",&fScale);		
		m_Items.m_DefaultProperties.SetScaling(fScale );
	}

	if (pVar->name && strcmp(pVar->name, "Font") == 0)
	{
		VisFontPtr spFont = VGUIManager::GlobalManager().LoadFont( value );		
		m_Items.m_DefaultProperties.SetFont( spFont );
	}
// 
// 	if ( pVar->name && strcmp(pVar->name, "m_bVerticalScroll") == 0  )
// 	{
// 		if ( m_bVerticalScroll == true )
// 		{
// 			m_spVScrollbar = new VSliderControl();
// 			m_spVScrollbar->SetParent(this);
// 			m_spVScrollbar->SetVertical(true);			
// 			m_spVScrollbar->OnBuildFinished();
// 		}
// 		else
// 		{
// 			if ( m_spVScrollbar != NULL )
// 			{
// 				delete m_spVScrollbar;
// 				m_spVScrollbar = NULL;
// 			}
// 		}		
// 	}

	

	if ( pVar->name && strcmp(pVar->name, "m_bHorizontalScroll") == 0 )
	{

	}
}

VUINodeListItem* VUINodeList::AddListItem(const char* szText, VTextureObject* pThumbnail)
{
	VUINodeListItem* pItem = new VUINodeListItem(szText , pThumbnail );	
	pItem->SetSize(300,300);
	AddItem(pItem, -1, false);

	return pItem;
}

// *** The variable table is used by the plugin system of the engine and editor. In that case we do not have
// *** variables which have to be initialised or which have to be available in the editor.
START_VAR_TABLE(VUINodeList, VisTypedEngineObject_cl,"animated entity", VVARIABLELIST_FLAGS_NONE, NULL );
 varList->Add (VisVariable_cl::NewVariable ("Texture", "Filename", VULPTYPE_VSTRING, -1, "", (VDisplayHintFlags_e)DISPLAY_HINT_TEXTUREFILE, 0, 0)  );
 DEFINE_VAR_COLORREF(VUINodeList, m_iBackgroundCol, "Background Color", "255/255/255/255", 0, 0);	

 varList->Add (VisVariable_cl::NewVariable ("ItemColor", "ItemColor", VULPTYPE_BYTE_COLOR4, -1, "60,80,255,255", (VDisplayHintFlags_e)DISPLAY_HINT_COLOR, 0, 0)  );
 varList->Add (VisVariable_cl::NewVariable ("ItemColorSelected", "ItemColorSelected", VULPTYPE_BYTE_COLOR4, -1, "140,180,255,255", (VDisplayHintFlags_e)DISPLAY_HINT_COLOR, 0, 0)  );

 varList->Add (VisVariable_cl::NewVariable ("Text", "TextureFilename", VULPTYPE_VSTRING, -1, "LIST_ITEM", DISPLAY_HINT_NONE, 0, 0)  );
 varList->Add (VisVariable_cl::NewVariable ("Scale", "TextScale", VULPTYPE_FLOAT, -1, "2.0", DISPLAY_HINT_NONE, 0, 0)  );	
 varList->Add (VisVariable_cl::NewVariable ("Font", "Font", VULPTYPE_VSTRING, -1, "Fonts\\NanumGothic.fnt", DISPLAY_HINT_CUSTOMFILE, 0, 0)  );

 DEFINE_VAR_FLOAT(VUINodeList, m_fIconSize, "", "50.0", 0, 0);	
 

 DEFINE_CATEGORY (VUINodeList, "Slider"); 
 DEFINE_VAR_BOOL(VUINodeList, bShowScrollBar, "", "TRUE", 0, 0);
  //varList->Add (VisVariable_cl::NewVariable ("SliderTexture", "SliderTexture", VULPTYPE_VSTRING, -1, "White.dds", (VDisplayHintFlags_e)DISPLAY_HINT_TEXTUREFILE, 0, 0)  );
  varList->Add (VisVariable_cl::NewVariable ("SliderColor", "SliderColor", VULPTYPE_BYTE_COLOR4, -1, "255,128,128,255", (VDisplayHintFlags_e)DISPLAY_HINT_COLOR, 0, 0)  );

  //varList->Add (VisVariable_cl::NewVariable ("SliderFrameTexture", "SliderFrameTexture", VULPTYPE_VSTRING, -1, "White.dds", (VDisplayHintFlags_e)DISPLAY_HINT_TEXTUREFILE, 0, 0)  );
  varList->Add (VisVariable_cl::NewVariable ("SliderFrameColor", "SliderFrameColor", VULPTYPE_BYTE_COLOR4, -1, "128,128,128,255", (VDisplayHintFlags_e)DISPLAY_HINT_COLOR, 0, 0)  );

  varList->Add (VisVariable_cl::NewVariable ("SliderBorderMode", "SliderBorderMode", VULPTYPE_BOOL, -1, "True", DISPLAY_HINT_NONE, 0, 0)  );
END_VAR_TABLE
