﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* QHyodaPapi.h                                                (C) 2000-2022 */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#ifndef Q_HYODA_PAPI_H
#define Q_HYODA_PAPI_H

#include <QtWidgets>

#include "ui_hyodaPapi.h"

class QHyodaTool;

class QHyodaPapi:public QWidget, public Ui::profilerWidget {
  Q_OBJECT
public:
  QHyodaPapi(QHyodaTool*);
  ~QHyodaPapi();
  void update(QByteArray *byteArray);
public:
  const int max_nb_func_to_profile=4;
  void ini();
private:
  QHyodaTool *tool;
};

#endif //  Q_HYODA_PAPI_H

