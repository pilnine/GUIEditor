// TKBMS v1.0 -----------------------------------------------------
//
// PLATFORM		: ALL
// PRODUCT		: VISION
// VISIBILITY		: PUBLIC
//
// ------------------------------------------------------TKBMS v1.0

#pragma once

using namespace System;
using namespace CSharpFramework;
using namespace CSharpFramework::Math;
using namespace CSharpFramework::Shapes;
using namespace CSharpFramework::Scene;
using namespace CSharpFramework::DynamicProperties;
using namespace ManagedFramework;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Runtime::Serialization;
using namespace CSharpFramework::Serialization;


#include "..\UIEngine\Source\UIEngineIncludes.hpp"

namespace UIEditorManaged
{
  public ref class UIEditorMessageService 
  {
  public:

	  UIEditorMessageService();
	  virtual ~UIEditorMessageService();

	  DynamicPropertyManager^ _uiPropertyManager;

	  void ConvertUIRTTIClasses(DynamicPropertyManager ^manager, VType *pBaseType, bool bIgnoreBaseClass);
	  void GetUIRTTIClasses( DynamicPropertyManager ^manager);
	  void GetGUIContextRTTIClasses( DynamicPropertyManager ^manager);
	  
	  void GetClassNames( System::String^ className , ArrayList^ Classes);
	  void GetContextClassNames(ArrayList^ Classes);

	  VOVERRIDE property DynamicPropertyManager^ UIPropertyManager
	  {
		  DynamicPropertyManager^ get() override;
	  }

	  void OnPropertyChanged(System::String^ propertyName , int iValue );
	  void OnPropertyChanged(System::String^ propertyName , float fValue );
	  void OnPropertyChanged(System::String^ propertyName , System::String^ sValue );
	  void OnPropertyChanged(System::String^ propertyName , bool bValue );
	  	 
	  static void Initialize()
	  {		  
		  s_UiEditorMessageService = gcnew UIEditorMessageService();
	  }

	  static void Deinitialize()
	  {		
		  delete s_UiEditorMessageService;
	  }

	  static UIEditorMessageService^ GetInstance()
	  {		  
		  return s_UiEditorMessageService;
	  }

	  float GetResolutionScaleX();
	  float GetResolutionScaleY();

	  void RenderLine2D(float x1,float y1, float x2,float y2, UINT iColorRef, float fStrength);

  private:

	  

	  static UIEditorMessageService^ s_UiEditorMessageService;
  };
}
