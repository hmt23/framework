﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
#ifndef COORESARCANE_ANALYTICSOLUTION_IDATAACCESSOR_H
#define COORESARCANE_ANALYTICSOLUTION_IDATAACCESSOR_H
/* Author : desrozis at Tue Dec 16 14:17:57 2008
 * Generated by createNew
 */

#include <arcane/IVariable.h>

using namespace Arcane;

class IDataAccessor
{
public:
  /** Constructeur de la classe */
  IDataAccessor()
    {
      ;
    }

  /** Destructeur de la classe */
  virtual ~IDataAccessor() { }

public:

  //! Initialisation
  virtual void init(const ItemGroup& group) = 0;

  //! Getting data
  virtual IVariable * variable(const ItemGroup& group) = 0;
};

#endif /* COORESARCANE_ANALYTICSOLUTION_IDATAACCESSOR_H */
