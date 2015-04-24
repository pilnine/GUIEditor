#pragma once

using namespace CSharpFramework;
using namespace CSharpFramework::Math;
using namespace CSharpFramework::Shapes;
using namespace CSharpFramework::Scene;


#include "EngineInstanceShape2D.hpp"

namespace UIEditorManaged
{
  /// <summary>
  /// EngineInstanceNode : class that manages the native class
  /// </summary>

  public ref class UIEngineInstanceImage : public UIEngineInstanceShape2D
  {
  public:     

    UIEngineInstanceImage();
	virtual ~UIEngineInstanceImage();


	VOVERRIDE void Initialize() override; 	

	VOVERRIDE void SetUVTransform( float x, float y , float z ); 
	VOVERRIDE void SetPosTransform( float x, float y , float z ); 
	
	//VImageStates &Image()

//	VOVERRIDE VImageStates &GetImageStates(){ return ((VUINodeImage*)m_pUIControlInstance)->Image(); }
	hkvMat3* UVTransform;
	hkvMat3* PosTransform;
  };
}
