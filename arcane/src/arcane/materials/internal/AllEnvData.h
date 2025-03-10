﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2024 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* AllEnvData.h                                                (C) 2000-2024 */
/*                                                                           */
/* Informations sur les valeurs des milieux.                                 */
/*---------------------------------------------------------------------------*/
#ifndef ARCANE_MATERIALS_INTERNAL_ALLENVDATA_H
#define ARCANE_MATERIALS_INTERNAL_ALLENVDATA_H
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "arcane/utils/TraceAccessor.h"
#include "arcane/utils/Array.h"

#include "arcane/core/IIncrementalItemConnectivity.h"

#include "arcane/materials/MatItemEnumerator.h"

#include "arcane/materials/internal/MeshMaterial.h"
#include "arcane/materials/internal/MeshEnvironment.h"
#include "arcane/materials/internal/ComponentItemInternalData.h"

#include <map>

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Arcane::Materials
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*!
 * \internal
 * \brief Informations sur les valeurs des milieux.
 */
class AllEnvData
: public TraceAccessor
{
  friend class IncrementalComponentModifier;

 public:

  explicit AllEnvData(MeshMaterialMng* mmg);

 public:

  void forceRecompute(bool compute_all);
  void recomputeIncremental();

 public:

  ConstArrayView<ComponentItemInternal> allEnvItemsInternal()
  {
    return m_item_internal_data.allEnvItemsInternal();
  }

  //! Notification de la fin de création des milieux/matériaux
  void endCreate(bool is_continue);

  ConstituentConnectivityList* componentConnectivityList()
  {
    return m_component_connectivity_list;
  }
  ComponentItemInternalData* componentItemInternalData()
  {
    return &m_item_internal_data;
  }

 private:

  MeshMaterialMng* m_material_mng = nullptr;
  ConstituentConnectivityList* m_component_connectivity_list = nullptr;
  Ref<IIncrementalItemSourceConnectivity> m_component_connectivity_list_ref;

  //! Niveau de verbosité
  Int32 m_verbose_debug_level = 0;

  ComponentItemInternalData m_item_internal_data;
  Int64 m_current_mesh_timestamp = -1;

 private:

  void _computeNbEnvAndNbMatPerCell();
  void _copyBetweenPartialsAndGlobals(Int32ConstArrayView pure_local_ids,
                                      Int32ConstArrayView partial_indexes,
                                      Int32 indexer_index, bool is_add_operation);
  void _computeAndResizeEnvItemsInternal();
  bool _isFullVerbose() const;
  void _rebuildMaterialsAndEnvironmentsFromGroups();
  void _computeInfosForEnvCells();
  void _checkLocalIdsCoherency() const;
  void _printAllEnvCells(CellVectorView ids);
  void _checkConnectivityCoherency();
  void _rebuildIncrementalConnectivitiesFromGroups();
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif  
