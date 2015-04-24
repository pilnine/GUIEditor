#include "UIEditorManagedPCH.h"
#include "GUIContextInstance.h"
#include "..\UIEngine\Source\VUINode.hpp"
#include "..\UIEngine\Source\VDefaultGUIContext.hpp"


namespace UIEditorManaged
{
	GUIContextInstance::GUIContextInstance()
	{

	}

	void GUIContextInstance::Initialize()
	{

	}

	void GUIContextInstance::DisposeObject()
	{

	}

	void GUIContextInstance::SetPosition(float x,float y,float z)
	{
	}

	void GUIContextInstance::SetOrientation(float yaw,float pitch,float roll)
	{
	}

	void GUIContextInstance::SetScaling(float x,float y, float z)
	{
		hkvVec2 temp;
		temp.x = x;
		temp.y = y;
		VUINodeMananger_cl::GlobalManager()->GetGUIContext()->SetScale(temp);
		//VUINodeMananger_cl::GlobalManager()->GetGUIContext()->m_fScaleX = x;
		//VUINodeMananger_cl::GlobalManager()->GetGUIContext()->m_fScaleY = y;
	}

	void GUIContextInstance::SetEditViewScale(float editScale)
	{
		hkvVec2 temp;
		temp.x = temp.y = editScale;
		VUINodeMananger_cl::GlobalManager()->GetGUIContext()->SetScale(temp);
	}

	bool GUIContextInstance::GetLocalBoundingBox(BoundingBox^ %bbox)
	{
		// create a small bounding box for picking
		float fSize = 5.f;
		(*bbox).Set(-fSize,-fSize,-fSize,fSize,fSize,fSize);
		return true;
	}

	void GUIContextInstance::TraceShape(Shape3D^ /* ownerShape*/, Vector3F /*rayStart*/,Vector3F /*rayEnd*/, ShapeTraceResult^% /*result*/)
	{
		// don't do anything because the shape code already does the picking
	}

	bool GUIContextInstance::OnExport(SceneExportInfo ^info)
	{
		// do the export: Get the export binary archive and serialize into it.
		// Requires the native node class to support serialization (derive from VisTypedEngineObject_cl and 
		// implement Serialize function)
		//   VArchive &ar = *((VArchive *)info->NativeShapeArchivePtr.ToPointer());
		//   ar.WriteObject(m_pNode);
		return true;
	}
}