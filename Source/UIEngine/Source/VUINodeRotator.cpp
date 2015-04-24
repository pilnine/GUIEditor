#include "VUINodeRotator.h"


V_IMPLEMENT_SERIAL( VUINodeRotator, VPushButton, 0, &g_NodeModule );

VUINodeRotator::VUINodeRotator(void)
{
	m_fAngle = 0.0f;
	m_fAngleDelta = 0.0f;
}


VUINodeRotator::~VUINodeRotator(void)
{
}


VUINodeRotator* VUINodeRotator::CreateInstance()
{
	// CreateInstance
	VUINodeRotator* pNewImage = new VUINodeRotator();

	VisFontPtr spFont = VGUIManager::GlobalManager().GetDefaultFont();
	pNewImage->Text().SetFont(spFont );
	pNewImage->Text().SetScaling(2);
	pNewImage->Text().SetText( "CHECK_BOX" );

	VTextureObject* spTexture = Vision::TextureManager.GetPlainWhiteTexture();
	pNewImage->Image().SetTexture( spTexture );
	pNewImage->Image().SetStretchMode(VImageState::STRETCHED);

	return pNewImage;
}


void VUINodeRotator::OnVariableValueChanged(VisVariable_cl *pVar, const char * value)
{
	VASSERT (pVar);
	VASSERT (value);

	/// Texture Button
	if (pVar->name && strstr(pVar->name, "Texture") != 0)
	{
		VTextureObject* spTexture = Vision::TextureManager.Load2DTexture( value );		

		if ( pVar->name && strstr(pVar->name, "ON") != 0 )
		{
			Image().m_States[VWindowBase::SELECTED].SetTexture( spTexture );			
		}
		else if ( pVar->name && strstr(pVar->name, "OFF" ) != 0 )
		{			
			Image().m_States[VWindowBase::NORMAL].SetTexture( spTexture );
			Image().m_States[VWindowBase::MOUSEOVER].SetTexture( spTexture );
		}
		else
		{
			Image().SetTexture( spTexture );
		}
	}	
	else if (pVar->name && strcmp(pVar->name, "TextColor") == 0)
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
	else if (pVar->name && strcmp(pVar->name, "TextColor") == 0)
	{
		int iComponents[4];
		int iCount = sscanf(value,"%i/%i/%i/%i",&iComponents[0],&iComponents[1],&iComponents[2],&iComponents[3]);

		VColorRef color(iComponents[0],iComponents[1],iComponents[2],iComponents[3]);
		Text().SetColor( color );
		return;
	}
	else if (pVar->name && strcmp(pVar->name, "m_fAngle") == 0)
	{
		
		return;
	}

	

}

void VUINodeRotator::RotateImage(float fAngle)
{
	
}

void VUINodeRotator::OnPaint(VGraphicsInfo &Graphics, const VItemRenderInfo &parentState)
{
	const VRectanglef rect = GetClientRect();

	hkvVec2 vCenter = ( rect.m_vMin + rect.m_vMax ) / 2.0f;

	hkvVec2 vSize = GetSize();

	VColorRef color = VColorRef(255,128,128,255);
	Overlay2DVertex_t v[6];

	float fWidth = vSize.x / 2;
	float fHeight = vSize.y / 2;


	IVRender2DInterface::CreateQuadVertices(-fWidth,-fHeight,fWidth,fHeight,0,0,1,1,color ,v);

	for ( int i = 0 ; i < 6 ; i++ )
	{
		Overlay2DVertex_t vertex = v[i];

		float fx = v[i].screenPos.x;
		float fy = v[i].screenPos.y;

		v[i].screenPos.x = vCenter.x + (fx * hkvMath::cosDeg (m_fAngle) + fy * hkvMath::sinDeg (m_fAngle));
		v[i].screenPos.y = vCenter.y + (-fx * hkvMath::sinDeg (m_fAngle) + fy * hkvMath::cosDeg (m_fAngle));
	}

	VTextureObject *pTex = Image().m_States[VWindowBase::NORMAL].GetCurrentTexture();
	Graphics.Renderer.Draw2DBuffer(6, v, pTex, VSimpleRenderState_t(VIS_TRANSP_ALPHA,RENDERSTATEFLAG_ALWAYSVISIBLE|RENDERSTATEFLAG_DOUBLESIDED));


	VItemRenderInfo thisState(parentState,this,1.f);
	m_TextCfg.OnPaint(Graphics,thisState);
	return;
}

// toggle check status
void VUINodeRotator::OnClick(VMenuEventDataObject *pEvent)
{
	VPushButton::OnClick(pEvent);
}

void VUINodeRotator::OnPointerDown(VMenuEventDataObject *pEvent)
{
	VPushButton::OnPointerDown( pEvent );

	m_vPivotPoint = pEvent->m_vMousePos;
}

void VUINodeRotator::OnPointerUp(VMenuEventDataObject *pEvent)
{
	m_fAngleDelta = 0.0f;
	VPushButton::OnPointerUp( pEvent );
}

float VUINodeRotator::GetAngleBetweenVectors( hkvVec2 v1)
{	
	const VRectanglef rect = GetClientRect();
	hkvVec2 vCenter = ( rect.m_vMin + rect.m_vMax ) / 2.0f;
	
	v1 -= vCenter;
	hkvVec2 v2( 0, 1 );

	v1.normalize();

	if ( v1.isValid() == false)
	{			
		return 0;
	}


	float fCosAngle = v2.dot(v1);
	float fAngle = hkvMath::acosDeg( fCosAngle );

	return fAngle;
}

void VUINodeRotator::OnDragBegin(const hkvVec2 &vMousePos, int iButtonMask)
{
	VString szLog;
	szLog.Format("OnDragBegin %d %d", (int)vMousePos.x , (int)vMousePos.y );
	Text().SetText( szLog );

	m_vPivotPoint = vMousePos;
	m_vEndPoint = vMousePos;

	m_fPivotAngle = m_fAngle;
	return;
}

 
 void VUINodeRotator::OnDragging(const hkvVec2 &vMouseDelta)
 {
  	const VRectanglef rect = GetClientRect();
  	hkvVec2 vCenter = ( rect.m_vMin + rect.m_vMax ) / 2.0f;	
 
 	hkvVec2 vMousePos = VUINodeMananger_cl::GlobalManager()->GetGUIContext()->GetCurrentMousePos();

	hkvVec2 vDirection = vCenter - vMousePos;

	float fNewAngle = m_fAngle;
	
	if ( vDirection.y > 0 )
	{
		fNewAngle -= vMouseDelta.x ;		
	}
	else
	{
		fNewAngle += vMouseDelta.x;
	}

	if ( vDirection.x > 0 )
	{
		fNewAngle += vMouseDelta.y;		
	}
	else
	{
		fNewAngle -= vMouseDelta.y;
	}

	m_fAngleDelta = fNewAngle - m_fAngle;
	m_fAngle = fNewAngle;
  
 	VString szLog;
 	szLog.Format("OnDragging %.2f %.2f", vMouseDelta.x , vMouseDelta.y ); 
 	Text().SetText( szLog );
 	return;
 }



// 
// void VUINodeRotator::OnDragging(const hkvVec2 &vMouseDelta)
// {
//  	const VRectanglef rect = GetClientRect();
//  	hkvVec2 vCenter = ( rect.m_vMin + rect.m_vMax ) / 2.0f;	
// 
// 	hkvVec2 vMousePos = VUINodeMananger_cl::GlobalManager()->GetGUIContext()->GetCurrentMousePos();
// 
// 	m_vEndPoint = vMousePos;
// // 
//  	hkvVec2 v1 = m_vEndPoint;// - vCenter;
//  	hkvVec2 v2 = m_vPivotPoint - vCenter;
// 
// 
// 	float fAngle = GetAngle(v1);
// 
// // 	if ( v.x > 0 )
// // 	{
// // 
// // 	}
// // 
// // 
// // 	if ( v.y > 0 )
// // 	{
// // 
// // 	}
// // 
//  
//  
//  	m_fAngle = fAngle - m_fPivotAngle;
// 
// // 	if ( vMouseDelta.x < 0 )
// // 	{
// // 		m_fAngle += vMouseDelta.x;
// // 		// angle 에 더해주고
// // 	}
// // 	else
// // 	{
// // 		m_fAngle -= vMouseDelta.x;
// // 		// angle 에 빼주고
// // 	}
// // 
// // 	if ( vMouseDelta.y > 0 )
// // 	{
// // 		m_fAngle += vMouseDelta.y;
// // 		// angle 에 더해주고
// // 	}
// // 	else
// // 	{
// // 		m_fAngle -= vMouseDelta.y;
// // 		// angle 에 빼주고
// // 	}
// 
// 	if ( m_fAngle >= 360)
// 		m_fAngle = 0;
// 
// 
// 	VString szLog;
// 	szLog.Format("OnDragging %f", m_fAngle ); 
// 	Text().SetText( szLog );
// 	return;
// }

void VUINodeRotator::OnDragEnd(VWindowBase *pOver)
{
	VString szLog;
	szLog.Format("OnDragEnd");
	Text().SetText( szLog );
	return;
}

void VUINodeRotator::Serialize( VArchive &ar )
{
	VPushButton::Serialize(ar);
	char iLocalVersion = 0;
	if (ar.IsLoading())
	{
	
	} else
	{

	}
}



bool VUINodeRotator::Build(TiXmlElement *pNode, const char *szPath, bool bWrite) 
{
	if ( bWrite == false )
	{		
		return VPushButton::Build( pNode,szPath,bWrite);
	}

	if ( GetUniqueID() != 0 )
	{
		VString szID = VGUIManager::GetIDName( (int)GetUniqueID() );	
		XMLHelper::Exchange_String(pNode,"ID",szID,bWrite);
	}

	XMLHelper::Exchange_Floats(pNode,"pos",m_vPosition.data,2,bWrite);
	XMLHelper::Exchange_Floats(pNode,"size",m_vSize.data,2,bWrite);

	VUINodeExportHelperXML::GlobalManager().BuildImageState( &m_ButtonCfg , this , XMLHelper::SubNode(pNode,"image",bWrite) , szPath,bWrite );
	VUINodeExportHelperXML::GlobalManager().BuildTextState( &m_TextCfg , this , XMLHelper::SubNode(pNode,"text",bWrite) , szPath,bWrite );

//	m_ButtonCfg.SetStretchMode(VImageState::TEXTURE_SIZE);

	// get the size of the control
	if (m_vSize.x<=0.f)
	{
		m_vSize = m_ButtonCfg.m_States[VDlgControlBase::NORMAL].GetSize();
		float fBoxWidth = m_vSize.x;
		hkvVec2 vTextSize = m_TextCfg.m_States[VDlgControlBase::NORMAL].GetSize();
		m_vSize.x += vTextSize.x + fBoxWidth;
		m_vSize.y = hkvMath::Max(m_vSize.y,vTextSize.y);

		// offset the text by the size of the checkbox
		for (int i=0;i<VWindowBase::STATE_COUNT;i++)
		{
			hkvVec2 vNewOfs = m_TextCfg.m_States[i].GetTextOfs();
			vNewOfs.x += fBoxWidth;
			m_TextCfg.m_States[i].SetTextOfs(vNewOfs);
		}
	}

	// initial checked status (actually same as selected)
	bool bChecked = false;
	XMLHelper::Exchange_Bool(pNode,"checked",bChecked,bWrite);	
	return true;
}


// *** The variable table is used by the plugin system of the engine and editor. In that case we do not have
// *** variables which have to be initialised or which have to be available in the editor.
START_VAR_TABLE(VUINodeRotator, VisTypedEngineObject_cl,"animated entity", VFORGE_USE_NATIVE_PARENTING, NULL );
	varList->Add (VisVariable_cl::NewVariable ("Texture ON", "TextureFilename", VULPTYPE_VSTRING, -1, "White.dds", (VDisplayHintFlags_e)DISPLAY_HINT_TEXTUREFILE, 0, 0)  );
	varList->Add (VisVariable_cl::NewVariable ("Texture OFF", "TextureFilename", VULPTYPE_VSTRING, -1, "White.dds", (VDisplayHintFlags_e)DISPLAY_HINT_TEXTUREFILE, 0, 0)  );
	varList->Add (VisVariable_cl::NewVariable ("Text", "TextureFilename", VULPTYPE_VSTRING, -1, "CHECK_BOX", (VDisplayHintFlags_e)DISPLAY_HINT_NONE, 0, 0)  );
	varList->Add (VisVariable_cl::NewVariable ("TextColor", "TextColor", VULPTYPE_BYTE_COLOR4, -1, "255,255,255,255", (VDisplayHintFlags_e)DISPLAY_HINT_COLOR, 0, 0)  );
	varList->Add (VisVariable_cl::NewVariable ("Scale", "TextScale", VULPTYPE_FLOAT, -1, "2.0", (VDisplayHintFlags_e)DISPLAY_HINT_NONE, 0, 0)  );	
	varList->Add (VisVariable_cl::NewVariable ("Font", "Font", VULPTYPE_VSTRING, -1, "Fonts\\DebugFont.fnt", (VDisplayHintFlags_e)DISPLAY_HINT_CUSTOMFILE, 0, 0)  );
	DEFINE_VAR_FLOAT(VUINodeRotator, m_fAngle, "m_fAngle", "0", 0, 0);		
END_VAR_TABLE
