/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2013 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

#pragma once

using namespace System;
using namespace CSharpFramework;
using namespace CSharpFramework::Math;
using namespace CSharpFramework::Shapes;
using namespace CSharpFramework::Scene;
using namespace System::Collections::Specialized;

#include "../UIEngine/Source/VUINode.hpp"

namespace UIEditorManaged
{
  /// <summary>
  /// EngineInstanceNode : class that manages the native class
  /// </summary>
	[Flags]
	public enum class DialogFlags // matches VisCoronaScaleMode_e
	{
		None = DIALOGFLAGS_NONE,
		FULLSCREEN = DIALOGFLAGS_FULLSCREEN,
		MODAL = DIALOGFLAGS_MODAL,		
	};

	public enum class DialogFlags_e // matches VisCoronaScaleMode_e
	{
		None = DIALOGFLAGS_NONE,
		FULLSCREEN = DIALOGFLAGS_FULLSCREEN,
		MODAL = DIALOGFLAGS_MODAL,		
	};


	[Flags]
	public enum class StartPositionType // matches StartPositionType_e
	{
		AbsolutePos,
		ScreenCenter,
		ParentPos,	
		ParentCenter,
	};

	/// \brief
	[Flags]
	public enum class TransparencyType 
	{
		VIS_TRANSP_NONE = 0,              ///< no transparency
		VIS_TRANSP_MULTIPLICATIVE = 1,    ///< multiplicative transparency
		VIS_TRANSP_ALPHA = 2,             ///< regular alpha blending
		VIS_TRANSP_ADDITIVE = 3,          ///< additive transparency		
		VIS_TRANSP_ALPHATEST = 4,         ///< replaces 'VIS_TRANSP_COLORKEY' : no transparency, only alpha test
		VIS_TRANSP_ADD_MODULATE = 5,      ///< add the modulated result (dest=dest*(1+src))
		VIS_TRANSP_ADDITIVE_NOALPHA = 6,  ///< additive transparency, ignoring source and destination alpha
		VIS_TRANSP_NOCOLORWRITE = 7,      ///< no blend, no color write, no alpha test
		VIS_TRANSP_MODULATE2X = 8,        ///< modulate and multiply by two
		VIS_TRANSP_SUBTRACTIVE = 9,       ///< subtractive blending (dest-src*src.a)
		VIS_TRANSP_PREMULTIPLIEDALPHA = 10, ///< subtractive blending (src+(dest*(1-src.a))
		VIS_TRANSP_ALPHA_NOALPHATEST_PARTICLES = 11, ///< same as alpha without alphatest but selects special states for particles (internal use)
		VIS_TRANSP_ALPHA_PARTICLES = 12,  ///< same as alpha but selects special states for particles (internal use)
		VIS_TRANSP_ALPHA_NOALPHATEST = 15, ///< same as alpha, but without alpha test (internal use)
	};
	
	[Flags]
	public enum class ImageStretchMode
	{
		TEXTURE_SIZE = 0,  ///< Use actual pixel size of the texture
		STRETCHED = 1,     ///< Stretch the image to fit into the screen rectangle
		BORDER = 2         ///< Render texture in border mode
	};



	[Flags]
	public enum class AlignmentVertical
	{		
		ALIGN_TOP = 0,      ///< Vertical aligment: top
		ALIGN_CENTER = 1,   ///< Horizontal/vertical alignment: center
		ALIGN_BOTTOM = 2    ///< Vertical alignment: bottom
	};

	[Flags]
	public enum class AlignmentHorizontal
	{
		ALIGN_LEFT = 0,     ///< Horizontal alignment: left
		ALIGN_CENTER = 1,   ///< Horizontal/vertical alignment: center
		ALIGN_RIGHT = 2,    ///< Horizontal alignment: right	
	};

	

  public ref class UIEngineInstanceShape2D : public IEngineShapeInstance
  {
  public:     

    UIEngineInstanceShape2D();

	UIEngineInstanceShape2D(bool Initialize);

	UIEngineInstanceShape2D( String ^sEntityClass );

	UIEngineInstanceShape2D( String ^sEntityClass, UIEngineInstanceShape2D^ parentDialog , Rectangle2D ^bound, StringDictionary ^propertyDict, bool bCallInitFunction );

	VOVERRIDE void Initialize(); 

	VOVERRIDE void UpdateProperties(StringDictionary ^propertyDict);
	VOVERRIDE void SetUIKey( String ^sVarName );
	VOVERRIDE void SetOrder( int iOrder );


    VOVERRIDE void DisposeObject() override;
    VOVERRIDE IntPtr GetNativeObject() override {return System::IntPtr((void *)m_pUIControlInstance);}

    // overridden IEngineShapeInstance functions
    VOVERRIDE void SetVisible(bool bStatus) override {m_bIsVisible=bStatus;}
    VOVERRIDE void SetPosition(float x,float y,float z) override;
	VOVERRIDE void SetSize(float x,float y);

	VOVERRIDE bool GetPosition(Vector3F %enginePosition) override;
	VOVERRIDE void GetSize(Vector2F %engineSize);
	VOVERRIDE Rectangle2D^ GetBound();

    VOVERRIDE void SetOrientation(float yaw,float pitch,float roll) override;
    VOVERRIDE void SetScaling(float /*x*/,float /*y*/, float /*z*/) override {}
    VOVERRIDE bool GetLocalBoundingBox(BoundingBox^ %bbox) override;
    VOVERRIDE void TraceShape(Shape3D ^ownerShape, Vector3F rayStart,Vector3F rayEnd, ShapeTraceResult^ %result) override;
    VOVERRIDE bool OnExport(SceneExportInfo ^info) override;


	VOVERRIDE void OnAttachToParent(IEngineShapeInstance ^pNewParent) override;
	VOVERRIDE void OnDetachFromParent() override;

    void LinkNode(UIEngineInstanceShape2D ^pOther);
    void UnLinkNode(UIEngineInstanceShape2D ^pOther);

	bool SetVariable(String ^sVarName, String ^sVarData);
	void SendValueChangingMessage(String ^propertyName, String ^newValue);

    bool m_bIsVisible; 
	VWindowBase *m_pUIControlInstance;	
  };
}

/*
 * Havok SDK - Base file, BUILD(#20130624)
 * 
 * Confidential Information of Havok.  (C) Copyright 1999-2013
 * Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
 * Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
 * rights, and intellectual property rights in the Havok software remain in
 * Havok and/or its suppliers.
 * 
 * Use of this software for evaluation purposes is subject to and indicates
 * acceptance of the End User licence Agreement for this product. A copy of
 * the license is included with this software and is also available from salesteam@havok.com.
 * 
 */
