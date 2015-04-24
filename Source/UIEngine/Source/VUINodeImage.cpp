#include "VUINodeImage.h"



V_IMPLEMENT_SERIAL( VUINodeImage, VImageControl, 0, &g_NodeModule );

VUINodeImage::VUINodeImage(void)
{
}


VUINodeImage::~VUINodeImage(void)
{
}


VUINodeImage* VUINodeImage::CreateInstance()
{
	// CreateInstance
	VUINodeImage* pNewImage = new VUINodeImage();

	VTextureObject* spTexture = Vision::TextureManager.GetPlainWhiteTexture();
	
	pNewImage->m_Image.SetTexture( spTexture );
	pNewImage->SetPosition(0,0);
	pNewImage->SetSize(100,100);

	return pNewImage;
}

void VUINodeImage::Serialize( VArchive &ar )
{
	VImageControl::Serialize(ar);
	char iLocalVersion = 0;
	if (ar.IsLoading())
	{
	
	}
	else
	{	

	}
}


bool VUINodeImage::Build(TiXmlElement *pNode, const char *szPath, bool bWrite) 
{
	if ( bWrite == false )
	{		
		return VImageControl::Build( pNode,szPath,bWrite);
	}

	if ( GetUniqueID() != 0 )
	{
		VString szID = VGUIManager::GetIDName( (int)GetUniqueID() );	
		XMLHelper::Exchange_String(pNode,"ID",szID,bWrite);
	}		

	XMLHelper::Exchange_Floats(pNode,"pos",m_vPosition.data,2,bWrite);
	XMLHelper::Exchange_Floats(pNode,"size",m_vSize.data,2,bWrite);

// 	if (!VDlgControlBase::Build(pNode,szPath,bWrite))
// 		return false;

	VUINodeExportHelperXML::GlobalManager().BuildImageState( &m_Image , this , XMLHelper::SubNode(pNode,"image",bWrite) , szPath,bWrite );

	return true;
}


void VUINodeImage::OnVariableValueChanged(VisVariable_cl *pVar, const char * value)
{
	VASSERT (pVar);
	VASSERT (value);

	if (pVar->name && strcmp(pVar->name, "Texture") == 0)
	{
		VTextureObject* spTexture = Vision::TextureManager.Load2DTexture( value );
		Image().SetTexture( spTexture );
	}	
	else if (pVar->name && strcmp(pVar->name, "m_eTransp") == 0)
	{		
		sscanf(value,"%d",&m_eTransp);
		Image().SetTransparency(m_eTransp);
	}
	else if (pVar->name && strcmp(pVar->name, "m_eStretcheMode") == 0)
	{
		sscanf(value,"%d",&m_eStretcheMode);	
		Image().SetStretchMode( m_eStretcheMode );
	}
	else if (pVar->name && strcmp(pVar->name, "ShaderEffect") == 0)
	{
	

	//	Image().SetTechnique( spTexture );
	}
	else if (pVar->name && strcmp(pVar->name, "TextureRange") == 0)
	{
		float fComponents[4];
		int iCount = sscanf(value,"%f/%f/%f/%f",&fComponents[0],&fComponents[1],&fComponents[2],&fComponents[3]);
	
		VRectanglef coord;
		coord.Add( hkvVec2(fComponents[0],fComponents[1]));
		coord.Add( hkvVec2(fComponents[2],fComponents[3]));

		Image().SetTextureRange( coord );
	}
	else if (pVar->name && strcmp(pVar->name, "Color") == 0)
	{
		int iComponents[4];
		int iCount = sscanf(value,"%i/%i/%i/%i",&iComponents[0],&iComponents[1],&iComponents[2],&iComponents[3]);

		VColorRef color(iComponents[0],iComponents[1],iComponents[2],iComponents[3]);	
		SetColor( color );
	}	
}


void VUINodeImage::OnPaint(VGraphicsInfo &Graphics, const VItemRenderInfo &parentState)
{
	VItemRenderInfo thisState(parentState,this,1.f);
	//Graphics.ClippingStack.Push(GetBoundingBox(),true);
	m_Image.OnPaint(Graphics,thisState);
	//Graphics.ClippingStack.Pop();
}

void VUINodeImage::SetColor( VColorRef color )
{
	for (int i=0;i<VWindowBase::STATE_COUNT;i++)
		Image().m_States[i].SetColor( color );
}

VColorRef VUINodeImage::GetColor()
{
	return Image().m_States[NORMAL].GetColor();
}


// *** The variable table is used by the plugin system of the engine and editor. In that case we do not have
// *** variables which have to be initialised or which have to be available in the editor.
START_VAR_TABLE(VUINodeImage, VisTypedEngineObject_cl,"animated entity", VVARIABLELIST_FLAGS_NONE, NULL );
varList->Add (VisVariable_cl::NewVariable ("Texture", "Filename", VULPTYPE_VSTRING, -1, "White.dds", (VDisplayHintFlags_e)DISPLAY_HINT_TEXTUREFILE, 0, 0));
	DEFINE_VAR_INT_AND_NAME(VUINodeImage, m_eTransp, "Transparency" ,  "Transparency", "0", 0, "CSharpType(TransparencyType)");
	DEFINE_VAR_INT_AND_NAME(VUINodeImage, m_eStretcheMode, "StretcheMode" ,  "StretcheMode", "0", 0, "CSharpType(ImageStretchMode)");
	//DEFINE_VAR_SHADEREFFECT(VUINodeImage, m_ShaderEffect, "ShaderEffect" ,  "Shaders\\BaseShaders.ShaderLib|BS_TintColor|", 0, 0);
	varList->Add (VisVariable_cl::NewVariable ("TextureRange", "TextureRange", VULPTYPE_FLOAT_VECTOR4, -1, "0/0/1/1", (VDisplayHintFlags_e)0, 0, 0));
	varList->Add (VisVariable_cl::NewVariable ("Color", "Color", VULPTYPE_BYTE_COLOR4, -1, "255/255/255/255", (VDisplayHintFlags_e)DISPLAY_HINT_COLOR, 0, 0));	
END_VAR_TABLE
