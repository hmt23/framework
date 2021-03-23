﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2021 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* IMeshPartitioner.h                                          (C) 2000-2014 */
/*                                                                           */
/* Interface d'un partitionneur de maillage.                                 */
/*---------------------------------------------------------------------------*/
#ifndef ARCANE_IMESHPARTITIONER_H
#define ARCANE_IMESHPARTITIONER_H
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "arcane/ArcaneTypes.h"

#include "arcane/IService.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

ARCANE_BEGIN_NAMESPACE

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

class IMesh;
class ILoadBalanceMng;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*!
 * \brief Interface d'un partitionneur de maillage.
 *
 * Le partitionneur réaffecte les propriétaires des entités.
 * Il n'effectue pas directement l'échange d'entité.
 * Le partitionneur peut utiliser certaines informations comme
 * le timeRatio() ou imbalance() pour calculer un partionnement efficace.
 */
class IMeshPartitioner
{
 public:

  virtual ~IMeshPartitioner() {} //!< Libère les ressources.

 public:

  virtual void build() =0;

 public:

  /*!
   * Repartitionne le maillage \a mesh
   *
   * Cette méthode change les propriétaires des entités et
   * remplit la variable IItemFamily::itemsNewOwner() de chaque famille d'entité
   * du maillage \a mesh avec le numéro du nouveau sous-domaine propriétaire.
   *
   * L'appelant doit ensuite appeler IMesh::exchangeItems() pour échanger
   * les entités entre les sous-domaines.
   *
   * Si \a initial_partition est vrai, il s'agit alors du partitionnement
   * initial. Sinon, les valeurs données par setCellsWeight() sont utilisées
   * pour associer un cout de calcul à chaque maille.
   *
   * Une fois l'échange terminé, l'appelant doit appeler
   * la méthode notifyEndPartition().
   *
   * Cette méthode est collective sur le maillage mesh().
   *
   * \note Cette méthode est réservée aux développeurs Arcane.
   * Si un module souhaite effectuer un repartitionnement,
   * il est préférable d'appeler
   * IMeshUtilities::partitionAndExchangeMeshWithReplication()
   * qui gère à la fois le partitionnement et l'échange des
   * informations et supporte la réplication de domaine.
   */
  virtual void partitionMesh(bool initial_partition) =0;

  virtual void partitionMesh(bool initial_partition,Int32 nb_part) =0;

  //! Notification lors de la fin d'un repartionnement (après échange des entités)
  virtual void notifyEndPartition() =0;
  
  //! Maillage associé au partitionneur
  virtual IMesh* mesh() const =0;

 public:
  
  /*!{ \name compact
   *
   * Proportion du temps de calcul de ce sous-domaine par rapport à celui
   * du sous-domaine qui à le temps de calcul de plus élevé.
   */
  //! Positionne la proportion du temps de calcul
  //virtual void setTimeRatio(Real v) =0;
  //! Proportion du temps de calcul
  //virtual Real timeRatio() const =0;
  //@}

  //! Temps de calcul du sous-domaine le plus chargé
  virtual ARCANE_DEPRECATED_116 void setMaximumComputationTime(Real v) =0;
  virtual ARCANE_DEPRECATED_116 Real maximumComputationTime() const =0;

  /*! \brief Temps de calcul de se sous-domaine.
   * Le premier élément indique le temps de calcul du sous-domaine
   * correspondante aux calcul dont le cout est proportionnel aux mailles.
   * Les suivants doivent être associées à une variable (à faire).
   */
  virtual ARCANE_DEPRECATED_116 void setComputationTimes(RealConstArrayView v) =0;
  virtual ARCANE_DEPRECATED_116 RealConstArrayView computationTimes() const =0;

  /*!@{ \name imbalance
   *
   * Déséquilibre de temps calcul. Il est calculé comme suit
   * imbalance = (max_computation_time - min_computation_time) / min_computation_time;
   */
  //! Positionne le déséquilibre de temps de calcul
  virtual void setImbalance(Real v) =0;
  //! Déséquilibre de temps de calcul
  virtual Real imbalance() const =0;
  //@}

  //! Positionne le déséquilibre maximal autorisé
  virtual void setMaxImbalance(Real v) =0;
  //! Déséquilibre maximal autorisé
  virtual Real maxImbalance() const =0;

  //! Permet de définir les poids des objets à partitionner : on doit utiliser le ILoadBalanceMng maintenant.
  virtual ARCANE_DEPRECATED_116 void setCellsWeight(ArrayView<float> weights,Integer nb_weight) =0;
  virtual ARCANE_DEPRECATED_116 ArrayView<float> cellsWeight() const =0;

  //! Change le ILoadBalanceMng à utiliser.
  virtual void setILoadBalanceMng(ILoadBalanceMng* mng) =0;
  virtual ILoadBalanceMng* loadBalanceMng() const =0;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

ARCANE_END_NAMESPACE

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif  
