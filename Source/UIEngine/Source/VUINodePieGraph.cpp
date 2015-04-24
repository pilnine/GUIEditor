// TKBMS v1.0 -----------------------------------------------------
//
// PLATFORM       : ALL
// PRODUCT        : VISION 
// VISIBILITY     : PUBLIC
//
// ------------------------------------------------------TKBMS v1.0


#include "VUINodePieGraph.h"


V_IMPLEMENT_SERIAL( VUINodePieGraph, VDlgControlBase, 0, &g_NodeModule );

VUINodePieGraph::VUINodePieGraph(void)
{
	m_fMaxValue = 100.0f;
	//m_Image;  ///< image
	m_iValueCount = 6;

	for ( int i = 0 ; i < 6 ; i++ )
		m_Values.Add(0.6f);
	
	m_pTex = Vision::TextureManager.GetPlainWhiteTexture();

}


VUINodePieGraph::~VUINodePieGraph(void)
{
}


VUINodePieGraph* VUINodePieGraph::CreateInstance()
{
	// CreateInstance
	VUINodePieGraph* pGraph = new VUINodePieGraph();

	//VTextureObject* spTexture = Vision::TextureManager.GetPlainWhiteTexture();

	//pNewImage->m_Image.SetTexture( spTexture );
	//pNewImage->SetPosition(0,0);
	//pNewImage->SetSize(100,100);

	return pGraph;
}


void VUINodePieGraph::Serialize( VArchive &ar )
{
	char iLocalVersion = 0;
	VDlgControlBase::Serialize(ar);
	if (ar.IsLoading())
	{
		ar >> iLocalVersion; VASSERT_MSG(iLocalVersion==0,"Invalid version");
		ar >> m_Image;
	} else
	{
		ar << iLocalVersion;
		ar << m_Image;
	}
}

hkvVec2 Rotate2D(hkvVec2 v , hkvVec2 center , float angle )
{
	float x = (v.x * hkvMath::cosDeg( angle )) - (v.y * hkvMath::sinDeg(angle));
	float y = (v.y * hkvMath::cosDeg( angle )) + (v.x * hkvMath::sinDeg(angle));

	return hkvVec2(x+ 200,y+ 200);
}

void VUINodePieGraph::SetCount(int iValueCount)
{
	m_iValueCount = iValueCount;
	m_Values.Reserve( iValueCount );
	m_Values.SetSize( iValueCount );

	for ( int i  = 0 ; i < iValueCount ; i++ )
	{
		m_Values[i] =  1.0f ;
	}	

	m_pTex = Vision::TextureManager.Load2DTexture("ring.dds");
}

void VUINodePieGraph::SetValue(int iIndex , float fValue)
{
	m_Values[iIndex] = fValue;
}

void VUINodePieGraph::SetValueColor(int iIndex , VColorRef color)
{

}

void VUINodePieGraph::OnPaint(VGraphicsInfo &Graphics, const VItemRenderInfo &parentState)
{
	const VRectanglef rect = GetBoundingBox();


	Overlay2DVertex_t v[6];
	IVRender2DInterface::CreateQuadVertices(rect.m_vMin.x,rect.m_vMin.y,rect.m_vMax.x,rect.m_vMax.y,0,0,1,1,m_ColorBackground ,v);
	Graphics.Renderer.Draw2DBuffer(6, v, m_pTex, VSimpleRenderState_t(VIS_TRANSP_ALPHA,RENDERSTATEFLAG_ALWAYSVISIBLE|RENDERSTATEFLAG_DOUBLESIDED));


	VColorRef color = VColorRef(255,128,128,255);

	const VSimpleRenderState_t iRenderState(VIS_TRANSP_NONE, RENDERSTATEFLAG_ALWAYSVISIBLE|RENDERSTATEFLAG_DOUBLESIDED);

	float sourceLeft = 0.5f;
	float sourceTop = 0.5f;
	float sourceRight = 1;
	float sourceBottom = 1;

	hkvVec2 center = GetPosition();
	float fSize = GetSize().x;

	center.x += GetSize().x/2.0f;
	center.y += GetSize().y/2.0f;

	hkvVec2 vTempPos[10];	

	float fDegree = 360 / m_iValueCount;

	for ( int i = 0 ; i < m_iValueCount ; i++ )
	{
		vTempPos[i] = hkvVec2(hkvMath::cosDeg (fDegree * (i - 1) ), hkvMath::sinDeg (fDegree * (i - 1) )) * ( fSize * m_Values[i] / 2.0f );	
	}


	Overlay2DVertex_t vOverlay[100];
	int i = 0;
	for (  int i = 0, iIndex = 0 ; i <m_iValueCount  ;  i++ , iIndex += 3 )
	{
		vOverlay[iIndex].Set(        0.0f + center.x,         0.0f + center.y,  sourceLeft,    sourceBottom);	
		vOverlay[iIndex].color = m_ColorInside;
		vOverlay[iIndex+1].Set(        vTempPos[i].x + center.x, vTempPos[i].y + center.y,  sourceRight, sourceTop);

		if ( i == m_iValueCount - 1)
		{
			vOverlay[iIndex+2].Set( vTempPos[0].x + center.x,         vTempPos[0].y + center.y, sourceRight,    sourceBottom);
		}
		else
		{
			vOverlay[iIndex+2].Set( vTempPos[i+1].x + center.x,         vTempPos[i+1].y + center.y, sourceRight,    sourceBottom);
		}
		

		vOverlay[iIndex+1].color = vOverlay[iIndex+2].color = m_ColorOutside;		
	}

	Graphics.Renderer.Draw2DBuffer(m_iValueCount * 3, vOverlay, m_pTex, iRenderState);
}

bool VUINodePieGraph::Build(TiXmlElement *pNode, const char *szPath, bool bWrite)
{
	if (!VDlgControlBase::Build(pNode,szPath,bWrite))
		return false;

	m_Image.Build(this,XMLHelper::SubNode(pNode,"image",bWrite),szPath,bWrite);
	return true;
}

void VUINodePieGraph::OnBuildFinished()
{
	if (m_vSize.x<0.f)
		m_vSize = m_Image.m_States[VWindowBase::NORMAL].GetSize();
}



// *** The variable table is used by the plugin system of the engine and editor. In that case we do not have
// *** variables which have to be initialised or which have to be available in the editor.
START_VAR_TABLE(VUINodePieGraph, VisTypedEngineObject_cl,"animated entity", VVARIABLELIST_FLAGS_NONE, NULL );
DEFINE_VAR_INT(VUINodePieGraph, m_iValueCount, "PieCount", "6", 0, 0);
DEFINE_VAR_FLOAT(VUINodePieGraph, m_fMaxValue, "Max Value", "100", 0, 0);
DEFINE_VAR_COLORREF(VUINodePieGraph, m_ColorBackground, "Background Color", "255/255/255/128", 0, 0);	
DEFINE_VAR_COLORREF(VUINodePieGraph, m_ColorInside, "Background Color", "64/255/64/255", 0, 0);	
DEFINE_VAR_COLORREF(VUINodePieGraph, m_ColorOutside, "Background Color", "128/128/255/255", 0, 0);	



varList->Add (VisVariable_cl::NewVariable ("AverageValue", "AverageValue", VULPTYPE_INT, -1, "50", DISPLAY_HINT_NONE, "CSharpType(AlignmentVertical)", 0)  );
varList->Add (VisVariable_cl::NewVariable ("Value", "Value", VULPTYPE_VSTRING, -1, "50/60/70/80/90/40", (VDisplayHintFlags_e)DISPLAY_HINT_NONE, 0, 0)  );
END_VAR_TABLE