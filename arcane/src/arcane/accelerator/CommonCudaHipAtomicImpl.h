﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2023 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* CommonCudaHipAtomicImpl.h                                   (C) 2000-2023 */
/*                                                                           */
/* Implémentation CUDA et HIP des opérations atomiques.                      */
/*---------------------------------------------------------------------------*/
#ifndef ARCANE_ACCELERATOR_COMMONCUDHIPATOMICIMPL_H
#define ARCANE_ACCELERATOR_COMMONCUDHIPATOMICIMPL_H
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

// Ce fichier doit être inclus uniquement par 'arcane/accelerator/Reduce.h'
// et n'est valide que compilé par le compilateur CUDA et HIP

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

// Attention: avec ROCm et un GPU sur bus PCI express la plupart des
// méthodes atomiques ne fonctionnent pas si le pointeur est allouée
// en mémoire unifiée. A priori le problème se pose avec atomicMin, atomicMax,
// atomicInc. Par contre atomicAdd a l'air de fonctionner si les accès
// concurrents ne sont pas trop nombreux

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Arcane::Accelerator::impl
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

template <typename DataType>
class CommonCudaHipAtomicAdd;
template <typename DataType>
class CommonCudaHipAtomicMax;
template <typename DataType>
class CommonCudaHipAtomicMin;

template <>
class CommonCudaHipAtomicAdd<int>
{
 public:

  static ARCCORE_DEVICE void apply(int* ptr, int v)
  {
    ::atomicAdd(ptr, v);
  }
};

template <>
class CommonCudaHipAtomicMax<int>
{
 public:

  static ARCCORE_DEVICE void apply(int* ptr, int v)
  {
    ::atomicMax(ptr, v);
  }
};

template <>
class CommonCudaHipAtomicMin<int>
{
 public:

  static ARCCORE_DEVICE void apply(int* ptr, int v)
  {
    ::atomicMin(ptr, v);
  }
};

template <>
class CommonCudaHipAtomicAdd<Int64>
{
 public:

  static ARCCORE_DEVICE void apply(Int64* ptr, Int64 v)
  {
    static_assert(sizeof(Int64) == sizeof(long long int), "Bad pointer size");
    ::atomicAdd((unsigned long long int*)ptr, v);
  }
};

template <>
class CommonCudaHipAtomicMax<Int64>
{
 public:

#if defined(__HIP__)
  static ARCCORE_DEVICE void apply(Int64*, Int64)
  {
    // N'existe pas sur ROCm (5.5)
    assert(0); // "AtomicMax<Int64> is not supported on ROCm");
  }
#else
  static ARCCORE_DEVICE void apply(Int64* ptr, Int64 v)
  {
    ::atomicMax((long long int*)ptr, v);
  }
#endif
};

template <>
class CommonCudaHipAtomicMin<Int64>
{
 public:

#if defined(__HIP__)
  static ARCCORE_DEVICE void apply(Int64*, Int64)
  {
    // N'existe pas sur ROCm (5.3)
    assert(0); // "AtomicMin<Int64> is only not supported on ROCm");
  }
#else
  static ARCCORE_DEVICE void apply(Int64* ptr, Int64 v)
  {
    ::atomicMin((long long int*)ptr, v);
  }
#endif
};

// Les devices d'architecture inférieure à 6.0 ne supportent pas
// les atomicAdd sur les 'double'.
// Ce code est issu de la documentation NVIDIA (programming guide)
__device__ inline double
preArch60atomicAdd(double* address, double val)
{
  unsigned long long int* address_as_ull = (unsigned long long int*)address;
  unsigned long long int old = *address_as_ull, assumed;

  do {
    assumed = old;
    old = atomicCAS(address_as_ull, assumed,
                    __double_as_longlong(val +
                                         __longlong_as_double(assumed)));
    // Note: uses integer comparison to avoid hang in case of NaN (since NaN != NaN)
  } while (assumed != old);

  return __longlong_as_double(old);
}
__device__ inline double
atomicMaxDouble(double* address, double val)
{
  unsigned long long int* address_as_ull = (unsigned long long int*)address;
  unsigned long long int old = *address_as_ull, assumed;

  do {
    assumed = old;
    double assumed_as_double = __longlong_as_double(assumed);
    old = atomicCAS(address_as_ull, assumed,
                    __double_as_longlong(val > assumed_as_double ? val : assumed_as_double));
    // Note: uses integer comparison to avoid hang in case of NaN (since NaN != NaN)
  } while (assumed != old);

  return __longlong_as_double(old);
}

__device__ inline double
atomicMinDouble(double* address, double val)
{
  unsigned long long int* address_as_ull = (unsigned long long int*)address;
  unsigned long long int old = *address_as_ull, assumed;

  do {
    assumed = old;
    double assumed_as_double = __longlong_as_double(assumed);
    old = atomicCAS(address_as_ull, assumed,
                    __double_as_longlong(val < assumed_as_double ? val : assumed_as_double));
    // Note: uses integer comparison to avoid hang in case of NaN (since NaN != NaN)
  } while (assumed != old);

  return __longlong_as_double(old);
}

template <>
class CommonCudaHipAtomicAdd<double>
{
 public:

  static ARCCORE_DEVICE void apply(double* ptr, double v)
  {
#if __CUDA_ARCH__ >= 600
    ::atomicAdd(ptr, v);
#else
    preArch60atomicAdd(ptr, v);
#endif
  }
};

template <>
class CommonCudaHipAtomicMax<double>
{
 public:

  static ARCCORE_DEVICE void apply(double* ptr, double v)
  {
    atomicMaxDouble(ptr, v);
  }
};

template <>
class CommonCudaHipAtomicMin<double>
{
 public:

  static ARCCORE_DEVICE void apply(double* ptr, double v)
  {
    atomicMinDouble(ptr, v);
  }
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // namespace Arcane::Accelerator::impl

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif
