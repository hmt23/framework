﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2023 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* ItemLocalIdListView.cc                                      (C) 2000-2023 */
/*                                                                           */
/* Vue sur une liste de ItemLocalId.                                         */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "arcane/core/ItemLocalIdListView.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Arcane
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

bool operator==(const ItemLocalIdListView& lhs, const ItemLocalIdListView& rhs)
{
  Int32 size1 = lhs.size();
  Int32 size2 = rhs.size();
  if (size1 != size2)
    return false;
  for (Int32 i = 0; i < size1; ++i)
    if (lhs[i] != rhs[i])
      return false;
  return true;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

std::ostream&
operator<<(std::ostream& o, const ItemLocalIdListView& lhs)
{
  const Int32 n = lhs.size();
  if (n == 0) {
    o << "{}";
  }
  else {
    o << "{ " << lhs[0];
    for (Int32 i = 1; i < n; ++i)
      o << ", " << lhs[i];
    o << " }";
  }
  return o;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

// Pour tester l'instantiation de ces classes
template class ItemLocalIdViewT<Node>;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // namespace Arcane

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
