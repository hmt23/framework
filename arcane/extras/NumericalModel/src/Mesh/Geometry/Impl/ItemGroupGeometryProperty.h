﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
#ifndef ARCGEOSIM_GEOMETRY_IMPL_ITEMGROUPGEOMETRYPROPERTY_H
#define ARCGEOSIM_GEOMETRY_IMPL_ITEMGROUPGEOMETRYPROPERTY_H
/* Author : havep at Tue Apr  1 17:25:52 2008
 * Generated by createNew
 */

#include "Mesh/Geometry/IGeometryMng.h"
#include  <boost/shared_ptr.hpp>

//! Internal class implementation for TemisGeometryService and Euclidian3GeometryService
class ItemGroupGeometryProperty
{
public:
  /** Constructeur de la classe */
  ItemGroupGeometryProperty();

  /** Destructeur de la classe */  
  virtual ~ItemGroupGeometryProperty();
public:

  struct StorageInfo {
    StorageInfo() : storageType(0), externStorage(0), ownStorage(0), usageVarCount(0), usageMapCount(0) { }
    Integer storageType;
    Integer externStorage;
    Integer ownStorage;
    UInt32 usageVarCount;
    UInt32 usageMapCount;
    boost::shared_ptr<IGeometryMng::RealVariable> realVar;    
    boost::shared_ptr<IGeometryMng::Real3Variable> real3Var;
    boost::shared_ptr<IGeometryMng::RealGroupMap> realMap;
    boost::shared_ptr<IGeometryMng::Real3GroupMap> real3Map;
  };

  typedef std::map<IGeometryProperty::eProperty, StorageInfo> StorageInfos;

  Integer defined;
  Integer computed;
  Integer delegated;
  StorageInfos storages;

  bool hasProperty(const IGeometryProperty::eProperty p) const;
  Integer getStorage(const IGeometryProperty::eProperty p) const;
  void resetFlags();
};



//! Modéle générique (vide) des accès de StorageInfo par type de stockage
template<typename T>
class ContainerAccessorT { };

template<>
struct ContainerAccessorT<Real> {
  typedef IGeometryMng::RealVariable RealTVariable;
  typedef IGeometryMng::RealGroupMap RealTGroupMap;
  inline static boost::shared_ptr<RealTVariable> & getVarContainer(ItemGroupGeometryProperty::StorageInfo & storage) { return storage.realVar; }
  inline static boost::shared_ptr<RealTGroupMap> & getMapContainer(ItemGroupGeometryProperty::StorageInfo & storage) { return storage.realMap; }
};

template<>
struct ContainerAccessorT<Real3> {
  typedef IGeometryMng::Real3Variable RealTVariable;
  typedef IGeometryMng::Real3GroupMap RealTGroupMap;
  inline static boost::shared_ptr<RealTVariable> & getVarContainer(ItemGroupGeometryProperty::StorageInfo & storage) { return storage.real3Var; }
  inline static boost::shared_ptr<RealTGroupMap> & getMapContainer(ItemGroupGeometryProperty::StorageInfo & storage) { return storage.real3Map; }
};

#endif /* ARCGEOSIM_GEOMETRY_IMPL_ITEMGROUPGEOMETRYPROPERTY_H */
