/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2013 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

#pragma once

using namespace System;
using namespace System::Runtime::Serialization;
using namespace CSharpFramework;
using namespace CSharpFramework::Math;
using namespace CSharpFramework::Shapes;
using namespace ManagedFramework;
using namespace CSharpFramework::Scene;
using namespace CSharpFramework::View;


#include <Vision/Runtime/EnginePlugins/VisionEnginePlugin/GUI/VGUIIncludes.hpp>
#include <Vision/Runtime/EnginePlugins/VisionEnginePlugin/GUI/Controls/VDialogStandardControls.hpp>


namespace UIEditorManaged
{   
	public ref class GUIContextInstance : public IEngineShapeInstance
	{
	public:     

		GUIContextInstance();

		VOVERRIDE void Initialize();

		VOVERRIDE void DisposeObject() override;
		VOVERRIDE IntPtr GetNativeObject() override {return System::IntPtr((void*)NULL);}

		VOVERRIDE void SetVisible(bool bStatus) override {}
		VOVERRIDE void SetPosition(float x,float y,float z) override;
		VOVERRIDE void SetOrientation(float yaw,float pitch,float roll) override;
		VOVERRIDE void SetScaling(float x,float y, float z) override;
		VOVERRIDE bool GetLocalBoundingBox(BoundingBox^ %bbox) override;
		VOVERRIDE void TraceShape(Shape3D ^ownerShape, Vector3F rayStart,Vector3F rayEnd, ShapeTraceResult^ %result) override;
		VOVERRIDE bool OnExport(SceneExportInfo ^info) override;

		VOVERRIDE void SetEditViewScale(float editScale);
		
	};

}
