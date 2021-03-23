﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2021 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* ModuleBuildInfo.cc                                          (C) 2000-2019 */
/*                                                                           */
/* Paramètres pour construire un module.                                     */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "arcane/utils/ArcanePrecomp.h"

#include "arcane/ModuleBuildInfo.h"
#include "arcane/ISubDomain.h"
#include "arcane/IMesh.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Arcane
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

ModuleBuildInfo::
ModuleBuildInfo(ISubDomain* sd,IMesh* mesh,const String& name)
: m_sub_domain(sd)
, m_mesh_handle(mesh->handle())
, m_graph(nullptr)
, m_name(name)
{
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

ModuleBuildInfo::
ModuleBuildInfo(ISubDomain* sd,const MeshHandle& mesh_handle,const String& name)
: m_sub_domain(sd)
, m_mesh_handle(mesh_handle)
, m_graph(nullptr)
, m_name(name)
{
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

ModuleBuildInfo::
ModuleBuildInfo(ISubDomain* sd,const String& name)
: m_sub_domain(sd)
, m_mesh_handle(sd->defaultMeshHandle())
, m_graph(nullptr)
, m_name(name)
{
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

IGraph* ModuleBuildInfo::
graph() const 
{
  return nullptr;
}
  
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // End namespace

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

