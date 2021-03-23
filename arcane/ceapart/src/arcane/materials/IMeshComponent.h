﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2021 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* IMeshComponent.h                                            (C) 2000-2017 */
/*                                                                           */
/* Interface d'un composant (matériau ou milieu) d'un maillage.              */
/*---------------------------------------------------------------------------*/
#ifndef ARCANE_MATERIALS_IMESHCOMPONENT_H
#define ARCANE_MATERIALS_IMESHCOMPONENT_H
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "arcane/ItemTypes.h"
#include "arcane/materials/MaterialsGlobal.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

ARCANE_BEGIN_NAMESPACE

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

MATERIALS_BEGIN_NAMESPACE

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

class IMeshMaterialMng;
class MeshMaterialVariableIndexer;
class ComponentItemInternal;
class ComponentCell;
class ComponentItemVectorView;
class AllEnvCell;
class ComponentPartItemVectorView;
class ComponentPurePartItemVectorView;
class ComponentImpurePartItemVectorView;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*!
 * \ingroup ArcaneMaterials
 * \brief Interface d'un composant (matériau ou milieu) d'un maillage.
 */
class ARCANE_MATERIALS_EXPORT IMeshComponent
{
 public:

  virtual ~IMeshComponent(){}

 public:

  //! Gestionnaire associé.
  virtual IMeshMaterialMng* materialMng() =0;

  //! Gestionnaire de trace associé.
  virtual ITraceMng* traceMng() =0;

  //! Nom du composant
  virtual const String& name() const =0;

  /*!
   * \brief Groupe des mailles de ce matériau.
   *
   * \warning Ce groupe ne doit pas être modifié. Pour changer
   * le nombre d'éléments d'un matériau, il faut passer
   * par le materialMng().
   */
  virtual CellGroup cells() const =0;

  /*!
   * \internal
   * Indexeur pour accéder aux variables partielles.
   */
  virtual MeshMaterialVariableIndexer* variableIndexer() =0;

  /*!
   * \internal
   * Vue sur les mailles du composant.
   */
  virtual ConstArrayView<ComponentItemInternal*> itemsInternalView() const =0;

  /*!
   * \brief Identifiant du composant.
   *
   * Il s'agit aussi de l'indice (en commençant par 0) de ce composant
   * dans la liste des composants de ce type.
   * Il existe une liste spécifique pour les matériaux et les milieux
   * et donc un composant qui représente un matériau peut avoir le
   * même id qu'un composant représentant un milieu.
   */
  virtual Int32 id() const =0;

  /*!
   * \brief Maille de ce composant pour la maille \a c.
   *
   * Si le composant n'est pas présent dans la présent dans la maille,
   * la maille nulle est retournée.
   *
   * Le coût de cette fonction est proportionnel au nombre de composants
   * présents dans la maille.
   */   
  virtual ComponentCell findComponentCell(AllEnvCell c) const =0;

  //! Vue associée à ce composant
  virtual ComponentItemVectorView view() =0;

  //! Vérifie que le composant est valide.
  virtual void checkValid() =0;

  //! Vrai si le composant est un matériau
  virtual bool isMaterial() const =0;

  //! Vrai si le composant est un milieu
  virtual bool isEnvironment() const =0;

  //! Indique si le composant est défini pour l'espace \a space
  virtual bool hasSpace(MatVarSpace space) const =0;

  //! Vue sur la liste des entités pures (associées à la maille globale) du composant
  virtual ComponentPurePartItemVectorView pureItems() =0;

  //! Vue sur la liste des entités impures (partielles) partielles du composant
  virtual ComponentImpurePartItemVectorView impureItems() =0;

  //! Vue sur la partie pure ou impure des entités du composant
  virtual ComponentPartItemVectorView partItems(eMatPart part) =0;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

MATERIALS_END_NAMESPACE
ARCANE_END_NAMESPACE

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif  
