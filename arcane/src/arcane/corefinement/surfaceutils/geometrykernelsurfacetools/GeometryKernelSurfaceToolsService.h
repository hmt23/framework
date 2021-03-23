﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2021 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
#ifndef ARCGEOSIM_SURFACEUTILS_GEOMETRYKERNELSURFACETOOLS_GEOMETRYKERNELSURFACETOOLSSERVICE_H
#define ARCGEOSIM_SURFACEUTILS_GEOMETRYKERNELSURFACETOOLS_GEOMETRYKERNELSURFACETOOLSSERVICE_H
/* Author : havep at Wed Aug 27 14:57:32 2008
 * Generated by createNew
 */

#include <arcane/corefinement/surfaceutils/ISurfaceUtils.h>

using namespace Arcane::Numerics;
#include <arcane/corefinement/surfaceutils/geometrykernelsurfacetools/GeometryKernelSurfaceTools_axl.h>

ARCANE_BEGIN_NAMESPACE
NUMERICS_BEGIN_NAMESPACE

class GeometryKernelSurfaceImpl;

class GeometryKernelSurfaceToolsService :
  public ArcaneGeometryKernelSurfaceToolsObject
{
public:
  /** Constructeur de la classe */
  GeometryKernelSurfaceToolsService(const Arcane::ServiceBuildInfo & sbi) : 
    ArcaneGeometryKernelSurfaceToolsObject(sbi) 
    {
      ;
    }
  
  /** Destructeur de la classe */
  virtual ~GeometryKernelSurfaceToolsService() {}
  
public:
  //! Initialisation
  void init() ;
  
  //! Création d'une nouvelle surface
  ISurface* createSurface() ;

  //! Définit les faces d'une surface
  void setFaceToSurface(ISurface* surface, FaceGroup face_group) ;

  //! compute for each face of surface1 the nearest face of surface2 
  void computeSurfaceContact(ISurface* surface1, 
                             ISurface* surface2,
                             FaceFaceContactList & contact) ;
private:
  GeometryKernelSurfaceImpl * castSurface(ISurface *) const;
  void buildFaceGroupSurface(FaceGroup group, GeometryKernelSurfaceImpl * surface_impl) const;
};

NUMERICS_END_NAMESPACE
ARCANE_END_NAMESPACE

#endif /* ARCGEOSIM_SURFACEUTILS_GEOMETRYKERNELSURFACETOOLS_GEOMETRYKERNELSURFACETOOLSSERVICE_H */
