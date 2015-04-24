#pragma once


#include "UIEngineIncludes.hpp"

#include "VUINode.hpp"


class VUINodeExportHelperXML
{
public:
	VUINodeExportHelperXML(void);
	virtual ~VUINodeExportHelperXML(void);

	const char* GetTextureStretchModeString( VImageState::ImageStretchMode_e mode)
	{		
		if ( mode == VImageState::STRETCHED)
			return "STRETCHED";
		else if ( mode == VImageState::BORDER)
			return "BORDER";
		else 
			return "TEXTURE_SIZE";
	}




	// access the global node manager instance
	NODE_IMPEXP static VUINodeExportHelperXML& GlobalManager();


	NODE_IMPEXP bool BuildImageState(VImageStates *state , VWindowBase *pOwner, TiXmlElement *pNode, const char *szPath, bool bWrite);
	NODE_IMPEXP bool BuildTextState(VTextStates *state , VWindowBase *pOwner, TiXmlElement *pNode, const char *szPath, bool bWrite ,const VTextStates *pDefaults=NULL );

	NODE_IMPEXP bool VImageStateBuild(VImageState *state , VWindowBase *pOwner, TiXmlElement *pNode, const char *szPath, bool bWrite);
	NODE_IMPEXP bool VTextStateBuild(VTextState *state, VWindowBase *pOwner, TiXmlElement *pNode, const char *szPath, bool bWrite , float fFontScale);


	static VUINodeExportHelperXML g_VUINodeExportHelperXML;
};

