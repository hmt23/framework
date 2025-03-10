﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2024 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* VariableUtils.h                                             (C) 2000-2024 */
/*                                                                           */
/* Fonctions utilitaires diverses sur les variables.                         */
/*---------------------------------------------------------------------------*/
#ifndef ARCANE_CORE_VARIABLEUTILS_H
#define ARCANE_CORE_VARIABLEUTILS_H
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "arcane/core/ArcaneTypes.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Arcane::VariableUtils
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*!
 * \brief Pré-copie la mémoire associée à la variable \a var.
 *
 * Pré-copie la mémoire associé à la variable \a sur l'accélérateur
 * spécifié par \a queue_or_null si elle ne s'y trouve pas encore.
 *
 * \a var doit être une variable d'un type numérique.
 * Si \a queue_or_null est nul, aucune opération n'est effectuée.
 * L'opération est asynchrone.
 */
extern "C++" ARCANE_CORE_EXPORT
void prefetchVariableAsync(IVariable* var, RunQueue* queue_or_null);

/*!
 * \brief Pré-copie la mémoire associée à la variable \a var.
 * \sa void prefetchVariableAsync(IVariable* var, RunQueue* queue_or_null);
 */
extern "C++" ARCANE_CORE_EXPORT
void prefetchVariableAsync(VariableRef& var, RunQueue* queue_or_null);

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // namespace Arcane::VariableUtils

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif
