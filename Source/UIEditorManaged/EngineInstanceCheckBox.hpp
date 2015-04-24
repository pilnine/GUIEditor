#pragma once

using namespace CSharpFramework;
using namespace CSharpFramework::Math;
using namespace CSharpFramework::Shapes;
using namespace CSharpFramework::Scene;


#include "EngineInstanceShape2D.hpp"
#include "../UIEngine/Source/VUINodeCheckBox.h"

namespace UIEditorManaged
{
  /// <summary>
  /// EngineInstanceNode : class that manages the native class
  /// </summary>

  public ref class UIEngineInstanceCheckBox : public UIEngineInstanceShape2D
  {
  public:     

    UIEngineInstanceCheckBox();
	virtual ~UIEngineInstanceCheckBox();

	VOVERRIDE void Initialize() override; 	

//	VOVERRIDE void SetTexture( String^ filename ); 

	VOVERRIDE void SetTexureTransform(Vector3F^ uvTransform , Vector3F^ posTransform);
	
	VOVERRIDE void SetStretchMode(int mode);
	VOVERRIDE void SetTransparency(int type);

	VOVERRIDE VUINodeCheckBox* GetCheckBox() { return static_cast<VUINodeCheckBox*>( m_pUIControlInstance ); }

	VOVERRIDE void SetUVTransform( float x, float y , float z ); 
	VOVERRIDE void SetPosTransform( float x, float y , float z ); 
	
	//VImageStates &Image()

//	VOVERRIDE VImageStates &GetImageStates(){ return ((VUINodeCheckBox*)m_pUIControlInstance)->Image(); }
	hkvMat3* UVTransform;
	hkvMat3* PosTransform;
  };
}
