﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2023 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* MaterialModifierOperation.h                                 (C) 2000-2023 */
/*                                                                           */
/* Opération d'ajout/suppression de mailles d'un matériau.                   */
/*---------------------------------------------------------------------------*/
#ifndef ARCANE_MATERIALS_INTERNAL_MATERIALMODIFIEROPERATION_H
#define ARCANE_MATERIALS_INTERNAL_MATERIALMODIFIEROPERATION_H
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "arcane/utils/Array.h"

#include "arcane/materials/MaterialsGlobal.h"
#include "arcane/materials/internal/IMeshMaterialModifierImpl.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Arcane::Materials
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*!
 * \brief Opération d'ajout ou suppression de mailles d'un matériau.
 */
class MaterialModifierOperation
{
 public:

  MaterialModifierOperation() = default;

 private:

  MaterialModifierOperation(IMeshMaterial* mat, Int32ConstArrayView ids, bool is_add)
  : m_mat(mat)
  , m_is_add(is_add)
  , m_ids(ids)
  {
  }

 public:

  static MaterialModifierOperation* createAdd(IMeshMaterial* mat, Int32ConstArrayView ids)
  {
    return new MaterialModifierOperation(mat, ids, true);
  }
  static MaterialModifierOperation* createRemove(IMeshMaterial* mat, Int32ConstArrayView ids)
  {
    return new MaterialModifierOperation(mat, ids, false);
  }

 public:

  //! Indique si l'opération consiste à ajouter ou supprimer des mailles du matériau
  bool isAdd() const { return m_is_add; }

  //! Matériau dont on souhaite ajouter/supprimer des mailles
  IMeshMaterial* material() const { return m_mat; }

  //! Liste des localId() des mailles à ajouter/supprimer
  ConstArrayView<Int32> ids() const { return m_ids; }

 public:

  /*!
   * \brief Filtre les ids des mailles.
   *
   * Si isAdd() est vrai, filtre les ids pour supprimer ceux qui sont déjà dans
   * la matériau. Si isAdd() est faux, filtre les ids pour supprimer ceux qui
   * ne sont pas dans le matériau.
   *
   * Ces opérations sont couteuses car il faut parcourir toutes les entités présentes.
   * Il ne faut donc en général utiliser cette méthode que en mode vérification.
   */
  void filterIds();

 private:

  static void _filterValidIds(MaterialModifierOperation* operation, Int32Array& valid_ids);
  static Int32 _checkMaterialPresence(MaterialModifierOperation* operation);

 private:

  IMeshMaterial* m_mat = nullptr;
  bool m_is_add = false;
  UniqueArray<Int32> m_ids;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // namespace Arcane::Materials

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif

