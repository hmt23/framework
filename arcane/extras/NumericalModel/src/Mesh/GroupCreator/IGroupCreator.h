﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
#ifndef GROUPCREATOR_IGROUPCREATOR_H
#define GROUPCREATOR_IGROUPCREATOR_H
/* Author : haeberlf at Fri Feb 20 10:59:31 2009
 * Generated by createNew
 */

namespace Arcane {}
using namespace Arcane;

class IGroupCreator
{
public:
  /** Constructeur de la classe */
  IGroupCreator() 
    {
      ;
    }
  
  /** Destructeur de la classe */
  virtual ~IGroupCreator() { }
  
public:

  //! Initialisation
  virtual void init() = 0;

  //! Apply service function
  virtual void apply() = 0;
};

#endif /* GROUPCREATOR_IGROUPCREATOR_H */
