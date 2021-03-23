﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2021 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* IMesh.h                                                     (C) 2000-2020 */
/*                                                                           */
/* Informations de connectivité d'un maillage non structuré.                 */
/*---------------------------------------------------------------------------*/
#ifndef ARCANE_UNSTRUCTUREDMESHCONNECTIVITY_H
#define ARCANE_UNSTRUCTUREDMESHCONNECTIVITY_H
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "arcane/Item.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Arcane
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/*!
 * \brief Vue sur une liste d'entités d'une connectivité.
 */
template <typename ItemType>
class ItemLocalIdView
{
 public:
  using LocalIdType = typename ItemType::LocalIdType;
  using SpanType = SmallSpan<const LocalIdType>;
  using iterator = typename SpanType::iterator;
  using const_iterator = typename SpanType::const_iterator;
 public:
  ARCCORE_HOST_DEVICE ItemLocalIdView(SpanType ids) : m_ids(ids){}
  ARCCORE_HOST_DEVICE ItemLocalIdView(const LocalIdType* ids,Int32 s) : m_ids(ids,s){}
  ItemLocalIdView() = default;
  ARCCORE_HOST_DEVICE operator SpanType() const { return m_ids; }
 public:
  ARCCORE_HOST_DEVICE SpanType ids() const { return m_ids; }
  ARCCORE_HOST_DEVICE LocalIdType operator[](Int32 i) const { return m_ids[i]; }
  ARCCORE_HOST_DEVICE Int32 size() const { return m_ids.size(); }
  ARCCORE_HOST_DEVICE iterator begin() { return m_ids.begin(); }
  ARCCORE_HOST_DEVICE iterator end() { return m_ids.end(); }
  ARCCORE_HOST_DEVICE const_iterator begin() const { return m_ids.begin(); }
  ARCCORE_HOST_DEVICE const_iterator end() const { return m_ids.end(); }
 public:
  ARCCORE_HOST_DEVICE const LocalIdType* data() const { return m_ids.data(); }
 private:
  SpanType m_ids;
};

template <typename ItemType>
class UnstructuredMeshConnectivityViewTraits;

template <>
class UnstructuredMeshConnectivityViewTraits<Node>
{
 public:
  using LocalIdType = NodeLocalId;
 public:
  static ARCANE_CORE_EXPORT IItemFamily* family(IMesh* m);
  static constexpr Int32 IDX() { return ItemInternalConnectivityList::NODE_IDX; }
};

template <>
class UnstructuredMeshConnectivityViewTraits<Edge>
{
 public:
  using LocalIdType = EdgeLocalId;
 public:
  static ARCANE_CORE_EXPORT IItemFamily* family(IMesh* m);
  static constexpr Int32 IDX() { return ItemInternalConnectivityList::EDGE_IDX; }
};

template <>
class UnstructuredMeshConnectivityViewTraits<Face>
{
 public:
  using LocalIdType = FaceLocalId;
 public:
  static ARCANE_CORE_EXPORT IItemFamily* family(IMesh* m);
  static constexpr Int32 IDX() { return ItemInternalConnectivityList::FACE_IDX; }
};

template <>
class UnstructuredMeshConnectivityViewTraits<Cell>
{
 public:
  using LocalIdType = CellLocalId;
 public:
  static ARCANE_CORE_EXPORT IItemFamily* family(IMesh* m);
  static constexpr Int32 IDX() { return ItemInternalConnectivityList::CELL_IDX; }
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*!
 * \brief Classe de base d'une vue sur une connectivité non structurée.
 */
class ARCANE_CORE_EXPORT UnstructuredItemConnectivityBaseView
{
 public:
  ARCCORE_HOST_DEVICE Int32 nbItem(ItemLocalId lid) const { return m_nb_item[lid]; }
 protected:
  ItemInternalConnectivityList* _getConnectivityList(IItemFamily* f);
 protected:
  SmallSpan<const Int32> m_nb_item;
  SmallSpan<const Int32> m_indexes;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*!
 * \brief Vue sur une connectivité non structurée entre deux entités.
 */
template<typename ItemType1,typename ItemType2>
class UnstructuredItemConnectivityView
: public UnstructuredItemConnectivityBaseView
{
 public:
  using Traits1 = UnstructuredMeshConnectivityViewTraits<ItemType1>;
  using Traits2 = UnstructuredMeshConnectivityViewTraits<ItemType2>;
  using ItemLocalId1 = typename Traits1::LocalIdType;
  using ItemLocalId2 = typename Traits2::LocalIdType;
  using ItemLocalIdViewType = ItemLocalIdView<ItemType2>;
 public:
  UnstructuredItemConnectivityView() = default;
 public:
  void init(IMesh* mesh)
  {
    IItemFamily* family = Traits1::family(mesh);
    ItemInternalConnectivityList* clist = _getConnectivityList(family);
    auto item_index_type = Traits2::IDX();
    m_indexes = clist->connectivityIndex(item_index_type);
    auto list_type = clist->connectivityList(item_index_type);
    m_list = { reinterpret_cast<const ItemLocalId2*>(list_type.data()), list_type.size() };
    m_nb_item = clist->connectivityNbItem(item_index_type);
  }
 public:
  ARCCORE_HOST_DEVICE Int32 nbItem(ItemLocalId1 lid) const { return m_nb_item[lid]; }
  ARCCORE_HOST_DEVICE ItemLocalIdViewType items(ItemLocalId1 lid) const
  {
    return { m_list.data() + m_indexes[lid], m_nb_item[lid] };
  }
 protected:
  ItemLocalIdViewType m_list;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*!
 * \brief Vue sur une connectivité Item->Node.
 */
template<typename ItemType>
class UnstructuredItemNodeConnectivityView
: public UnstructuredItemConnectivityView<ItemType,Node>
{
 public:
  using BaseClass = UnstructuredItemConnectivityView<ItemType,Node>;
  using ItemLocalIdType = typename ItemType::LocalIdType;
  using ItemLocalIdViewType = typename BaseClass::ItemLocalIdViewType;
 public:
  UnstructuredItemNodeConnectivityView() = default;
 public:
  ARCCORE_HOST_DEVICE Int32 nbNode(ItemLocalIdType lid) const { return BaseClass::nbItem(lid); }
  ARCCORE_HOST_DEVICE ItemLocalIdViewType nodes(ItemLocalIdType lid) const { return BaseClass::items(lid); }
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*!
 * \brief Vue sur une connectivité Item->Edge.
 */
template<typename ItemType>
class UnstructuredItemEdgeConnectivityView
: public UnstructuredItemConnectivityView<ItemType,Edge>
{
 public:
  using BaseClass = UnstructuredItemConnectivityView<ItemType,Edge>;
  using ItemLocalIdType = typename ItemType::LocalIdType;
  using ItemLocalIdViewType = typename BaseClass::ItemLocalIdViewType;
 public:
  UnstructuredItemEdgeConnectivityView() = default;
 public:
  ARCCORE_HOST_DEVICE Int32 nbEdge(ItemLocalIdType lid) const { return BaseClass::nbItem(lid); }
  ARCCORE_HOST_DEVICE ItemLocalIdViewType edges(ItemLocalIdType lid) const { return BaseClass::items(lid); }
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*!
 * \brief Vue sur une connectivité Item->Face.
 */
template<typename ItemType>
class UnstructuredItemFaceConnectivityView
: public UnstructuredItemConnectivityView<ItemType,Face>
{
 public:
  using BaseClass = UnstructuredItemConnectivityView<ItemType,Face>;
  using ItemLocalIdType = typename ItemType::LocalIdType;
  using ItemLocalIdViewType = typename BaseClass::ItemLocalIdViewType;
 public:
  UnstructuredItemFaceConnectivityView() = default;
 public:
  ARCCORE_HOST_DEVICE Int32 nbFace(ItemLocalIdType lid) const { return BaseClass::nbItem(lid); }
  ARCCORE_HOST_DEVICE ItemLocalIdViewType faces(ItemLocalIdType lid) const { return BaseClass::items(lid); }
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*!
 * \brief Vue sur une connectivité Item->Cell.
 */
template<typename ItemType>
class UnstructuredItemCellConnectivityView
: public UnstructuredItemConnectivityView<ItemType,Cell>
{
 public:
  using BaseClass = UnstructuredItemConnectivityView<ItemType,Cell>;
  using ItemLocalIdType = typename ItemType::LocalIdType;
  using ItemLocalIdViewType = typename BaseClass::ItemLocalIdViewType;
 public:
  UnstructuredItemCellConnectivityView() = default;
 public:
  ARCCORE_HOST_DEVICE Int32 nbCell(ItemLocalIdType lid) const { return BaseClass::nbItem(lid); }
  ARCCORE_HOST_DEVICE ItemLocalIdViewType cells(ItemLocalIdType lid) const { return BaseClass::items(lid); }
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

class UnstructuredCellNodeConnectivityView
: public UnstructuredItemNodeConnectivityView<Cell>
{
};
class UnstructuredCellEdgeConnectivityView
: public UnstructuredItemEdgeConnectivityView<Cell>
{
};
class UnstructuredCellFaceConnectivityView
: public UnstructuredItemFaceConnectivityView<Cell>
{
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

class UnstructuredFaceNodeConnectivityView
: public UnstructuredItemNodeConnectivityView<Face>
{
};
class UnstructuredFaceEdgeConnectivityView
: public UnstructuredItemEdgeConnectivityView<Face>
{
};
class UnstructuredFaceCellConnectivityView
: public UnstructuredItemCellConnectivityView<Face>
{
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

class UnstructuredNodeEdgeConnectivityView
: public UnstructuredItemEdgeConnectivityView<Node>
{
};
class UnstructuredNodeFaceConnectivityView
: public UnstructuredItemFaceConnectivityView<Node>
{
};
class UnstructuredNodeCellConnectivityView
: public UnstructuredItemCellConnectivityView<Node>
{
};


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*!
 * \brief Vue sur les connectivités standards d'un maillage non structuré.
 *
 * Il faut appeler setMesh() avant d'utiliser les méthodes de cette classe.
 * La méthode setMesh() doit être appelée si la cardinalité du maillage évolue.
 */
class ARCANE_CORE_EXPORT UnstructuredMeshConnectivityView
{
 public:

  void setMesh(IMesh* m);

 public:

  UnstructuredCellNodeConnectivityView cellNode() const
  { _checkValid(); return m_cell_node_connectivity_view; }
  UnstructuredCellEdgeConnectivityView cellEdge() const
  { _checkValid(); return m_cell_edge_connectivity_view; }
  UnstructuredCellFaceConnectivityView cellFace() const
  { _checkValid(); return m_cell_face_connectivity_view; }

  UnstructuredFaceNodeConnectivityView faceNode() const
  { _checkValid(); return m_face_node_connectivity_view; }
  UnstructuredFaceEdgeConnectivityView faceEdge() const
  { _checkValid(); return m_face_edge_connectivity_view; }
  UnstructuredFaceCellConnectivityView faceCell() const
  { _checkValid(); return m_face_cell_connectivity_view; }

  UnstructuredNodeEdgeConnectivityView nodeEdge() const
  { _checkValid(); return m_node_edge_connectivity_view; }
  UnstructuredNodeFaceConnectivityView nodeFace() const
  { _checkValid(); return m_node_face_connectivity_view; }
  UnstructuredNodeCellConnectivityView nodeCell() const
  { _checkValid(); return m_node_cell_connectivity_view; }

 private:

  UnstructuredCellNodeConnectivityView m_cell_node_connectivity_view;
  UnstructuredCellEdgeConnectivityView m_cell_edge_connectivity_view;
  UnstructuredCellFaceConnectivityView m_cell_face_connectivity_view;

  UnstructuredFaceNodeConnectivityView m_face_node_connectivity_view;
  UnstructuredFaceEdgeConnectivityView m_face_edge_connectivity_view;
  UnstructuredFaceCellConnectivityView m_face_cell_connectivity_view;

  UnstructuredNodeEdgeConnectivityView m_node_edge_connectivity_view;
  UnstructuredNodeFaceConnectivityView m_node_face_connectivity_view;
  UnstructuredNodeCellConnectivityView m_node_cell_connectivity_view;

  IMesh* m_mesh = nullptr;

 private:

  void _checkValid() const;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // End namespace Arcane

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif  
