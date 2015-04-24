/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2013 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

/// \file SpeedTree6EnginePlugin/VSpeedTreeIncludes.hpp

#ifndef UIENGINE_INCLUDES_HPP
#define UIENGINE_INCLUDES_HPP

#include <Vision/Runtime/Base/VBase.hpp>
#include <Vision/Runtime/Engine/System/Vision.hpp>


// import/export
#ifdef NODEENGINEPLUGIN_EXPORTS
#define NODE_IMPEXP			__declspec(dllexport)
#define NODE_IMPEXP_DATA	__declspec(dllexport)
#elif defined (NODEENGINEPLUGIN_IMPORTS)
#define NODE_IMPEXP			__declspec(dllimport)
#define NODE_IMPEXP_DATA	__declspec(dllimport)
#else
#define NODE_IMPEXP 
#define NODE_IMPEXP_DATA 
#endif

extern VModule g_NodeModule; // declared below with DECLARE_THIS_MODULE macro



class VUINodeMananger_cl;
//class VUINode_cl;


#endif // VSPEEDTREE_INCLUDES_HPP

/*
 * Havok SDK - Base file, BUILD(#20130523)
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
