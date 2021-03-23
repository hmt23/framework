﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2021 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* RunQueue.cc                                                 (C) 2000-2021 */
/*                                                                           */
/* Gestion d'une file d'exécution sur accélérateur.                          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "arcane/accelerator/RunQueue.h"
#include "arcane/accelerator/Runner.h"
#include "arcane/accelerator/RunQueueImpl.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Arcane::Accelerator
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

RunQueue::
RunQueue(Runner& runner)
: RunQueue(runner,runner.executionPolicy())
{
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

RunQueue::
RunQueue(Runner& runner,eExecutionPolicy exec_policy)
: m_p(RunQueueImpl::create(&runner,exec_policy))
{
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

RunQueue::
~RunQueue()
{
  m_p->release();
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

void RunQueue::
barrier()
{
  _internalRuntime()->barrier();
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

eExecutionPolicy RunQueue::
executionPolicy() const
{
  return m_p->executionPolicy();
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

IRunQueueRuntime* RunQueue::
_internalRuntime() const
{
  return m_p->_internalRuntime();
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

RunCommandImpl* RunQueue::
_getCommandImpl()
{
  return m_p->_internalCreateOrGetRunCommandImpl();
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // End namespace Arcane::Accelerator

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
