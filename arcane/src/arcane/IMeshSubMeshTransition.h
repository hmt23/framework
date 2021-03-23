﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2021 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* IMeshSubMeshTransition.h                                    (C) 2000-2016 */
/*---------------------------------------------------------------------------*/
#ifndef ARCANE_IMESHSUBMESHTRANSITION_H
#define ARCANE_IMESHSUBMESHTRANSITION_H
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "arcane/IMesh.h"
#include "arcane/IPrimaryMesh.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

ARCANE_BEGIN_NAMESPACE

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/*!
 * \brief Macro pour convertir un IMesh en IPrimaryMesh.
 *
 * \deprecated Ne pas utiliser. Appeler IMesh::toPrimaryMesh() à la place.
 */
#define PRIMARYMESH_CAST(m) ((m)->toPrimaryMesh())

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

ARCANE_END_NAMESPACE

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif  
