﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* IReduceMemoryImpl.h                                         (C) 2000-2022 */
/*                                                                           */
/* Interface de la gestion mémoire pour les réductions.                      */
/*---------------------------------------------------------------------------*/
#ifndef ARCANE_ACCELERATOR_IREDUCEMEMORYIMPL_H
#define ARCANE_ACCELERATOR_IREDUCEMEMORYIMPL_H
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "arcane/accelerator/core/AcceleratorCoreGlobal.h"

#include <stack>

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Arcane::Accelerator::impl
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*!
 * \internal
 * \brief Interface de la gestion mémoire pour les réductions.
 * \warning API en cours de définition.
 */
class ARCANE_ACCELERATOR_CORE_EXPORT IReduceMemoryImpl
{
 public:

  //! Informations mémoire pour la réduction sur les accélérateurs
  struct GridMemoryInfo
  {
   public:

    //! Mémoire allouée pour la réduction sur une grille (nb_bloc * sizeof(T))
    Byte* m_grid_memory_value_as_bytes = nullptr;
    //! Taille allouée pour \a m_grid_memory_value_as_bytes
    Int32 m_grid_memory_size = 0;
    //! Entier utilisé pour compter le nombre de blocs ayant déjà fait leur partie de la réduction
    unsigned int* m_grid_device_count = nullptr;
  };

 public:

  virtual ~IReduceMemoryImpl() = default;

 public:

  //! Alloue la mémoire pour une donnée réduite dont la taille du type est \a data_size.
  virtual void* allocateReduceDataMemory(Int64 data_size) = 0;

  //! Positionne la taille de la grille GPU (le nombre de blocs)
  virtual void setGridSizeAndAllocate(Int32 grid_size) = 0;
  virtual Int32 gridSize() const = 0;

  //! Informations sur la mémoire utilisée par la réduction
  virtual GridMemoryInfo gridMemoryInfo() = 0;

  //! Libère l'instance.
  virtual void release() = 0;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

template<typename T> T*
allocateReduceDataMemory(IReduceMemoryImpl* p)
{
  return reinterpret_cast<T*>(p->allocateReduceDataMemory(sizeof(T)));
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // End namespace Arcane::Accelerator::impl

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif  
