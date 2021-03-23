﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
// -*- C++ -*-
#include "Numerics/Expressions/ExpressionBuilder/ExpressionBuilderR1vR1Core.h"
/***************************************************************/
/* This is an automatically generated file                     */
/*              DO NOT MODIFY                                  */
/***************************************************************/
// Generated from ExpressionBuilderRnvRmCore.cc.template
// by Template Tool Kit at Mon Jul 27 13:28:47 2009


#include "Numerics/Expressions/FunctionParser/FunctionParser.h"

using namespace Arcane;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

ExpressionBuilderR1vR1Core::
ExpressionBuilderR1vR1Core(FunctionParser * function_parser,
                                       bool delegate_destroy)
 : m_function_parser(function_parser)
 , m_delegate_destroy(delegate_destroy)
 {
   ;
 }

/*---------------------------------------------------------------------------*/

ExpressionBuilderR1vR1Core::
 ~ExpressionBuilderR1vR1Core()
{
  if (m_delegate_destroy)
    delete m_function_parser;
}

/*---------------------------------------------------------------------------*/

void
ExpressionBuilderR1vR1Core::
init()
{
  // Do nothing
}

/*---------------------------------------------------------------------------*/

void 
ExpressionBuilderR1vR1Core::
setParameter(Integer index,const Real & value)
{
  String name = m_function_parser->getParameter(index);

  m_function_parser->setParameter(name, value);
}

/*---------------------------------------------------------------------------*/

void 
ExpressionBuilderR1vR1Core::
setParameter(const String & name,const Real & value)
{
  m_function_parser->setParameter(name, value);
}

/*---------------------------------------------------------------------------*/

Integer
ExpressionBuilderR1vR1Core::
nbParameter() const
{
  return m_function_parser->getNbParameter();
}

/*---------------------------------------------------------------------------*/

String
ExpressionBuilderR1vR1Core::
parameterName(Integer index) const
{
  return m_function_parser->getParameter(index);
}

/*---------------------------------------------------------------------------*/

void 
ExpressionBuilderR1vR1Core::
setVariable(Integer index, const CArrayT<Real> & variable)
{
  String name = m_function_parser->getVariable(index);
  m_function_parser->setVariable(name, variable.unguardedBasePointer(), variable.size());
}

/*---------------------------------------------------------------------------*/

void 
ExpressionBuilderR1vR1Core::
setVariable(const String & name, const CArrayT<Real> & variable)
{
  m_function_parser->setVariable(name, variable.unguardedBasePointer(), variable.size());
}

/*---------------------------------------------------------------------------*/

Integer
ExpressionBuilderR1vR1Core::
nbVariable() const
{
  return m_function_parser->getNbVariable();
}

/*---------------------------------------------------------------------------*/

String 
ExpressionBuilderR1vR1Core::
variableName(Integer index) const
{
  return m_function_parser->getVariable(index);
}

/*---------------------------------------------------------------------------*/

void
ExpressionBuilderR1vR1Core::
setEvaluationResult(CArrayT<Real> & result)
{
  m_function_parser->setEvaluationResult(result.unguardedBasePointer(),result.size());
}

/*---------------------------------------------------------------------------*/

void
ExpressionBuilderR1vR1Core::
setDerivationResult(Integer di, CArrayT<Real> & result)
{

}

/*---------------------------------------------------------------------------*/

void
ExpressionBuilderR1vR1Core::
setDerivationResult(const String & di, CArrayT<Real> & result)
{

}

/*---------------------------------------------------------------------------*/

void
ExpressionBuilderR1vR1Core::
eval()
{

}

/*---------------------------------------------------------------------------*/

void
ExpressionBuilderR1vR1Core::
cleanup()
{

}

/*---------------------------------------------------------------------------*/

void
ExpressionBuilderR1vR1Core::
eval(const Real & var0,
     Real & res0)

{
  const std::string var0_name = m_function_parser->getVariable(0);
  m_function_parser->setVariable(var0_name, & var0, 1);
  

  m_function_parser->setEvaluationResult(& res0, 1);
  

  m_function_parser->eval();
  m_function_parser->cleanup();
}

/*---------------------------------------------------------------------------*/

void
ExpressionBuilderR1vR1Core::
eval(const CArrayT<Real> & var0,
     CArrayT<Real> & res0)
{
  const std::string var0_name = m_function_parser->getVariable(0);
  m_function_parser->setVariable(var0_name, var0.unguardedBasePointer(), var0.size());  
  

  m_function_parser->setEvaluationResult(res0.unguardedBasePointer(), res0.size());
  

  m_function_parser->eval();
  m_function_parser->cleanup();
}

/*---------------------------------------------------------------------------*/

void
ExpressionBuilderR1vR1Core::
eval(const ConstArrayView<Real> var0,
     ArrayView<Real> res0)
{
  const std::string var0_name = m_function_parser->getVariable(0);
  m_function_parser->setVariable(var0_name, var0.unguardedBasePointer(), var0.size());  
  

  m_function_parser->setEvaluationResult(res0.unguardedBasePointer(), res0.size());
  

  m_function_parser->eval();
  m_function_parser->cleanup();
}


/*---------------------------------------------------------------------------*/

Real
ExpressionBuilderR1vR1Core::
eval(const Real & var0)
{
  const std::string var0_name = m_function_parser->getVariable(0);
  m_function_parser->setVariable(var0_name, & var0, 1);
  

  Real res0 ;
  m_function_parser->setEvaluationResult(& res0, 1);

  m_function_parser->eval();
  m_function_parser->cleanup();

  return res0 ;
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
