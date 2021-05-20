﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* PolyhedralMesh.h                                (C) 2000-2021             */
/*                                                                           */
/* Polyhedral mesh impl using Neo data structure                             */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#ifndef ARCANE_POLYHEDRALMESH_H
#define ARCANE_POLYHEDRALMESH_H
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include <memory>
#include "arcane/utils/ArcaneGlobal.h"
#include "arcane/utils/String.h"
#include "arcane/utils/Collection.h"
#include "arcane/MeshHandle.h"
#include "arcane/ItemGroup.h"
#include "arcane/mesh/EmptyMesh.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Arcane {
class ISubDomain;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/


namespace Arcane::mesh {

class PolyhedralMeshImpl;
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

class ARCANE_MESH_EXPORT PolyhedralMesh : public EmptyMesh {
 public :
  ISubDomain* m_subdomain;
  inline static const String m_mesh_handle_name = "polyhedral_mesh_handle";
  MeshHandle m_mesh_handle;

  std::unique_ptr<PolyhedralMeshImpl> m_mesh; // using pimpl to limit dependency to neo lib to cc file

 public:
  PolyhedralMesh(ISubDomain* subDomain);
  ~PolyhedralMesh(); // for pimpl idiom

 public:
  void read(String const& filename);

  // IMeshBase interface
 public:

  static String handleName() { return m_mesh_handle_name; }

  //! Handle sur ce maillage
  const MeshHandle& handle() const override;

 public:

  String name() const override;

  Integer nbNode() override;

  Integer nbEdge() override;

  Integer nbFace() override;

  Integer nbCell() override;

  Integer nbItem(eItemKind ik) override;

  ITraceMng* traceMng() override;

  Integer dimension() override;

  NodeGroup allNodes() override { return ItemGroup{}; }

  EdgeGroup allEdges() override { return ItemGroup{}; }

  FaceGroup allFaces() override { return ItemGroup{}; }

  CellGroup allCells() override;

  NodeGroup ownNodes() override { return ItemGroup{}; }

  EdgeGroup ownEdges() override { return ItemGroup{}; }

  FaceGroup ownFaces() override { return ItemGroup{}; }

  CellGroup ownCells() override { return ItemGroup{}; }

  FaceGroup outerFaces() override { return ItemGroup{}; }

  IItemFamily* createItemFamily(eItemKind ik,const String& name) override {return nullptr;}

  IItemFamily* findItemFamily(eItemKind ik,const String& name,bool create_if_needed=false) override { return nullptr; }
  IItemFamily* findItemFamily(const String& name,bool throw_exception=false) override { return nullptr; }

  IItemFamilyModifier* findItemFamilyModifier(eItemKind ik,const String& name) override { return nullptr; }
  IItemFamily* itemFamily(eItemKind ik) override { return nullptr; }

  IItemFamily* nodeFamily() override { return nullptr; }
  IItemFamily* edgeFamily() override { return nullptr; }
  IItemFamily* faceFamily() override { return nullptr; }
  IItemFamily* cellFamily() override { return nullptr; }

  IItemFamilyCollection itemFamilies() override { return IItemFamilyCollection{}; }

 private:
  [[noreturn]] void _errorEmptyMesh() const;

};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // End namespace Arcane::mesh

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif //ARCANE_POLYHEDRALMESH_H
