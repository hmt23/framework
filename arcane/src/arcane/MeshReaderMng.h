﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2021 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* MeshReaderMng.h                                             (C) 2000-2019 */
/*                                                                           */
/* Gestionnaire de lecteurs de maillage.                                     */
/*---------------------------------------------------------------------------*/
#ifndef ARCANE_MESHREADERMNG_H
#define ARCANE_MESHREADERMNG_H
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "arcane/utils/String.h"
#include "arcane/ArcaneTypes.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Arcane
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*!
 * \brief Gestionnaire de lecteurs de maillage.
 */
class ARCANE_CORE_EXPORT MeshReaderMng
{
  class Impl;

 public:
 
  MeshReaderMng(ISubDomain* sd);
  MeshReaderMng(const MeshReaderMng&) = delete;
  ~MeshReaderMng();
  const MeshReaderMng& operator=(const MeshReaderMng&) = delete;

 public:

  /*!
   * \brief Lit le maillage dont le nom de fichier est \a file_name.
   *
   * \a file_name doit avoir une extension et le lecteur utilisé est basé
   * sur cette extension.
   * Le maillage créé est associé à un \a IParallelMng séquentiel et aura
   * pour nom \a mesh_name.
   *
   * Cette méthode lève une exception si le maillage ne peut pas être lu.
   */
  IMesh* readMesh(const String& mesh_name,const String& file_name);

 private:

  Impl* m_p;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // End namespace Arcane

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif  

