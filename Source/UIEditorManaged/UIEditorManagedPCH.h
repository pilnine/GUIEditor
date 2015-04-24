/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2013 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

// Include For Vision Engine
#include <Vision/Runtime/Base/VBase.hpp>
#include <Vision/Runtime/Engine/System/Vision.hpp>

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

#if (_MSC_VER >= 1400) // Visual Studio 8.0
#pragma make_public(IVRenderInterface)
#pragma make_public(IVResourcePreview)
#pragma make_public(IVObjectComponent)
#pragma make_public(VManagedResource)
#pragma make_public(VString)
#pragma make_public(VArchive)
#pragma make_public(VDynamicMesh)
#pragma make_public(VShaderEffectLib)
#pragma make_public(VShaderEffectResource)
#pragma make_public(VCompiledEffect)
#pragma make_public(VTypedObject)
#pragma make_public(VObjectComponentCollection)
//#pragma make_public(VCurvePoint2D)
//#pragma make_public(VColorCurve)
#pragma make_public(VisVariable_cl)
#pragma make_public(VisObject3D_cl)
#pragma make_public(VisTraceLineInfo_t)
#pragma make_public(VisSurfaceTextureSet_cl)
#pragma make_public(VisStaticGeometryInstanceCollection_cl)
#pragma make_public(VisTypedEngineObject_cl)
#pragma make_public(VisSurface_cl)
#pragma make_public(VisBaseEntity_cl)
//#pragma make_public(VGMaterial)
#pragma make_public(hkvAlignedBBox)
#pragma make_public(hkvPlane)
#pragma make_public(hkvMat3)
#pragma make_public(hkvMat4)
#pragma make_public(hkvVec2)
#pragma make_public(hkvVec3)
#pragma make_public(hkvVec4)
#pragma make_public(hkvAlignedBBox)
#pragma make_public(hkvBoundingSphere)




#endif