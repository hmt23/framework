//
// Created by dechaiss on 2/6/20.
//

/*-------------------------
 * Neo library
 * Polyhedral mesh test
 * sdc (C)-2020
 *
 *-------------------------
 */

#include <vector>
#include <set>
#include <array>
#include <algorithm>

#include "neo/Neo.h"
#include "gtest/gtest.h"

#ifdef  HAS_XDMF
#include "XdmfDomain.hpp"
#include "XdmfInformation.hpp"
#include "XdmfUnstructuredGrid.hpp"
#include "XdmfSystemUtils.hpp"
#include "XdmfHDF5Writer.hpp"
#include "XdmfWriter.hpp"
#include "XdmfReader.hpp"
#endif

template <typename Container>
void _printContainer(Container&& container, std::string const& name){
  std::cout << name << " , size : " << container.size() << std::endl;
  for (auto element : container) {
    std::cout << element << " " ;
  }
  std::cout << std::endl;
}

namespace StaticMesh {

static const std::string cell_family_name{"CellFamily"};
static const std::string face_family_name{"FaceFamily"};
static const std::string node_family_name{"NodeFamily"};

void addItems(Neo::Mesh& mesh, Neo::Family& family, std::vector<Neo::utils::Int64> const& uids, Neo::AddedItemRange& added_item_range)
{
  auto& added_items = added_item_range.new_items;
  // Add items
  mesh.addAlgorithm(  Neo::OutProperty{family,family.lidPropName()},
                      [&family,&uids,&added_items](Neo::ItemLidsProperty & lids_property){
                        std::cout << "Algorithm: create items in family " << family.m_name << std::endl;
                        added_items = lids_property.append(uids);
                        lids_property.debugPrint();
                        std::cout << "Inserted item range : " << added_items;
                      });
  // register their uids
  auto uid_property_name = family.m_name+"_uids";
  mesh.addAlgorithm(
      Neo::InProperty{family,family.lidPropName()},
      Neo::OutProperty{family, uid_property_name},
      [&family,&uids,&added_items](Neo::ItemLidsProperty const& item_lids_property,
                                   Neo::PropertyT<Neo::utils::Int64>& item_uids_property){
        std::cout << "Algorithm: register item uids for family " << family.m_name << std::endl;
        if (item_uids_property.isInitializableFrom(added_items))
          item_uids_property.init(added_items,std::move(uids)); // init can steal the input values
        else
          item_uids_property.append(added_items, uids);
        item_uids_property.debugPrint();
      });// need to add a property check for existing uid
  }

  // todo same interface with nb_connected_item_per_items as an array
  void addConnectivity(Neo::Mesh &mesh, Neo::Family &source_family,
                       Neo::ItemRange &source_items,
                       Neo::Family& target_family,
                       std::vector<size_t>&& nb_connected_item_per_items,
                       std::vector<Neo::utils::Int64> const& connected_item_uids) {
    // add connectivity property if doesn't exist
    std::string connectivity_name =
        source_family.m_name + "to" + target_family.m_name + "_connectivity";
    source_family.addArrayProperty<Neo::utils::Int32>(connectivity_name);
    mesh.addAlgorithm(
        Neo::InProperty{source_family, source_family.lidPropName()},
        Neo::InProperty{target_family, target_family.lidPropName()},
        Neo::OutProperty{source_family, source_family.m_name + "to" +
                                            target_family.m_name +
                                            "_connectivity"},
        [&connected_item_uids, nb_connected_item_per_items, &source_items,
         &source_family, &target_family](
            Neo::ItemLidsProperty const &source_family_lids_property,
            Neo::ItemLidsProperty const &target_family_lids_property,
            Neo::ArrayProperty<Neo::utils::Int32> &source2target) {
          std::cout << "Algorithm: register connectivity between "
                    << source_family.m_name << "  and  " << target_family.m_name
                    << std::endl;
          auto connected_item_lids =
              target_family_lids_property[connected_item_uids];
          if (source2target.isInitializableFrom(source_items)) {
            source2target.resize(std::move(nb_connected_item_per_items));
            source2target.init(source_items, std::move(connected_item_lids));
          } else {
            source2target.append(source_items, connected_item_lids,
                                 nb_connected_item_per_items);
          }
          source2target.debugPrint();
        });
  }

  Neo::ArrayProperty<Neo::utils::Int32> const &
  getConnectivity(Neo::Mesh const &mesh, Neo::Family const& source_family,
                  Neo::Family const &target_family)
  {
    return source_family.getConcreteProperty <
           Neo::ArrayProperty<Neo::utils::Int32>>(source_family.m_name + "to" +
                                                 target_family.m_name+"_connectivity");
  }

  // todo : define 2 signatures to indicate eventual memory stealing...?
  void setNodeCoords(Neo::Mesh& mesh, Neo::Family& node_family, Neo::AddedItemRange& added_node_range, std::vector<Neo::utils::Real3>& node_coords){
    node_family.addProperty<Neo::utils::Real3>(std::string("node_coords"));
    auto& added_nodes = added_node_range.new_items;
    mesh.addAlgorithm(
        Neo::InProperty{node_family,node_family.lidPropName()},
        Neo::OutProperty{node_family,"node_coords"},
        [&node_coords,&added_nodes](Neo::ItemLidsProperty const& node_lids_property,
                                    Neo::PropertyT<Neo::utils::Real3> & node_coords_property){
          std::cout << "Algorithm: register node coords" << std::endl;
          if (node_coords_property.isInitializableFrom(added_nodes))  node_coords_property.init(added_nodes,std::move(node_coords)); // init can steal the input values
          else node_coords_property.append(added_nodes, node_coords);
          node_coords_property.debugPrint();
        });
  }

  // todo define also a const method
  Neo::utils::ArrayView<Neo::utils::Real3> getNodeCoords(Neo::Mesh const& mesh, Neo::Family& node_family)
  {
    auto& node_coords = node_family.getConcreteProperty<Neo::PropertyT<Neo::utils::Real3>>("node_coords");
    return node_coords.values(); //
  }

  void addConnectivity(Neo::Mesh &mesh, Neo::Family &source_family,
                       Neo::AddedItemRange &source_items,
                       Neo::Family& target_family,
                       std::vector<size_t>&& nb_connected_item_per_items,
                       std::vector<Neo::utils::Int64> const& connected_item_uids)
  {
    addConnectivity(mesh, source_family, source_items.new_items, target_family,
                    std::move(nb_connected_item_per_items), connected_item_uids);
  }

  Neo::ArrayProperty<Neo::utils::Int32> const& faces(Neo::Mesh const& mesh, Neo::Family const& source_family)
  {
    auto &face_family = mesh.getFamily(Neo::ItemKind::IK_Face, face_family_name);
    return getConnectivity(mesh, source_family, face_family);
  }

  Neo::ArrayProperty<Neo::utils::Int32> const& nodes(Neo::Mesh const& mesh, Neo::Family const& source_family)
  {
    auto& node_family = mesh.getFamily(Neo::ItemKind::IK_Node,node_family_name);
    return getConnectivity(mesh, source_family, node_family);
  }

  namespace utilities {
    using ItemNodesInCell = std::vector<std::vector<int>>; // [face_index_in_cell][node_index_in_face] = node_index_in_cell
    using NbNodeInCell = int;
    using CellTypes = std::vector<std::pair<NbNodeInCell, ItemNodesInCell>>;
    void getItemConnectivityFromCell(std::vector<Neo::utils::Int64> const& cell_nodes,
                                     std::vector<int> cell_type_indexes, CellTypes const& cell_types,
                                     int& nb_faces,
                                     std::vector<Neo::utils::Int64>& face_nodes,
                                     std::vector<Neo::utils::Int32>& cell_faces)
    {
      nb_faces = 0;
      auto cell_index = 0;
      auto face_index = 0;
      using FaceNodes = std::set<int>;
      using FaceUid = Neo::utils::Int64;
      using FaceInfo = std::pair<FaceNodes,FaceUid>;
      auto face_info_comp = [](FaceInfo const& face_info1, FaceInfo const& face_info2){
        return face_info1.first < face_info2.first;
      };
      std::set<FaceInfo, decltype(face_info_comp)> face_nodes_set(face_info_comp);
      for (int cell_nodes_index = 0; cell_nodes_index < cell_nodes.size();) {
        auto [nb_node_in_cell, face_nodes_all_faces] = cell_types[cell_type_indexes[cell_index++]];
        auto current_cell_nodes = Neo::utils::ConstArrayView<Neo::utils::Int64>{(size_t)nb_node_in_cell,&cell_nodes[cell_nodes_index]};
        for (auto current_face_node_indexes_in_cell : face_nodes_all_faces)
        {
          std::vector<Neo::utils::Int64> current_face_nodes;
          current_face_nodes.reserve(current_face_node_indexes_in_cell.size());
          std::transform(current_face_node_indexes_in_cell.begin(),
                         current_face_node_indexes_in_cell.end(),
                         std::back_inserter(current_face_nodes),
                         [&current_cell_nodes](auto& node_index)
                         {return current_cell_nodes[node_index];});
          auto [face_info, is_new_face] = face_nodes_set.emplace(FaceNodes{current_face_nodes.begin(),
                                                                           current_face_nodes.end()},face_index);
          if (!is_new_face) std::cout << "Item not inserted " << face_index << std::endl;
          if (is_new_face) {
            face_nodes.insert(face_nodes.end(),current_face_nodes.begin(), current_face_nodes.end());
          }
          cell_faces.push_back(face_info->second);
          if (is_new_face) {
            nb_faces++;
            ++face_index;
          }
        }
        cell_nodes_index += nb_node_in_cell;
      }
    }

    void reverseConnectivity(std::vector<Neo::utils::Int64> const& original_source_item_uids,
                             std::vector<Neo::utils::Int64> const& original_connectivity,
                             std::vector<size_t> const& nb_connected_items_per_item_original,
                             std::vector<Neo::utils::Int64> & new_source_item_uids,
                             std::vector<Neo::utils::Int64> & reversed_connectivity,
                             std::vector<size_t> & nb_connected_items_per_item_reversed)
    {
      assert(("Invalid argument size, utilities::reverseConnectivity",original_source_item_uids.size()==nb_connected_items_per_item_original.size()));
      assert(("Invalid argument size, utilities::reverseConnectivity",
          original_connectivity.size()==std::accumulate(nb_connected_items_per_item_original.begin(),nb_connected_items_per_item_original.end(),0)));
      auto source_item_index = 0;
      std::map<Neo::utils::Int64,std::vector<Neo::utils::Int64>> reversed_connectivity_map;
      for (int original_connectivity_index = 0; original_connectivity_index < original_connectivity.size(); ) {
        auto current_item_nb_connected_items = nb_connected_items_per_item_original[source_item_index];
        auto current_item_connected_items = Neo::utils::ConstArrayView<Neo::utils::Int64>{
            current_item_nb_connected_items,
            &original_connectivity[original_connectivity_index]};
        for (auto connected_item : current_item_connected_items) {
          reversed_connectivity_map[connected_item].push_back(original_source_item_uids[source_item_index]);
        }
        source_item_index++;
        original_connectivity_index+= current_item_nb_connected_items;
      }
      new_source_item_uids.resize(reversed_connectivity_map.size());
      nb_connected_items_per_item_reversed.resize(reversed_connectivity_map.size());
      auto new_source_item_uids_index = 0;
      reversed_connectivity.clear();
      reversed_connectivity.reserve(4*reversed_connectivity_map.size());// choose an average of 4 connected elements per item
      for (auto [new_source_item_uid,new_source_item_connected_items] : reversed_connectivity_map) {
        new_source_item_uids[new_source_item_uids_index] = new_source_item_uid;
        reversed_connectivity.insert(reversed_connectivity.end(),
                                     new_source_item_connected_items.begin(),
                                     new_source_item_connected_items.end());
        nb_connected_items_per_item_reversed[new_source_item_uids_index++] =
            new_source_item_connected_items.size();
      }
    }
  }
}

namespace PolyhedralMeshTest {

  auto &addCellFamily(Neo::Mesh &mesh, std::string family_name) {
    auto &cell_family =
        mesh.addFamily(Neo::ItemKind::IK_Cell, std::move(family_name));
    cell_family.addProperty<Neo::utils::Int64>(family_name + "_uids");
    return cell_family;
  }

  auto &addNodeFamily(Neo::Mesh &mesh, std::string family_name) {
    auto &node_family =
        mesh.addFamily(Neo::ItemKind::IK_Node, std::move(family_name));
    node_family.addProperty<Neo::utils::Int64>(family_name + "_uids");
    return node_family;
  }

  auto &addFaceFamily(Neo::Mesh &mesh, std::string family_name) {
    auto &face_family =
        mesh.addFamily(Neo::ItemKind::IK_Face, std::move(family_name));
    face_family.addProperty<Neo::utils::Int64>(family_name + "_uids");
    return face_family;
  }

void _createMesh(Neo::Mesh &mesh,
                 std::vector<Neo::utils::Int64> const &node_uids,
                 std::vector<Neo::utils::Int64> const &cell_uids,
                 std::vector<Neo::utils::Int64> const &face_uids,
                 std::vector<Neo::utils::Real3>& node_coords, // not const since they can be moved
                 std::vector<Neo::utils::Int64>& cell_nodes,
                 std::vector<Neo::utils::Int64>& cell_faces,
                 std::vector<Neo::utils::Int64>& face_nodes,
                 std::vector<size_t>&& nb_node_per_cells,
                 std::vector<size_t>&& nb_face_per_cells,
                 std::vector<size_t>&& nb_node_per_faces) {
  std::__throw_invalid_argument("Missing face_cells connectivity");
}

  void _createMesh(Neo::Mesh &mesh,
                   std::vector<Neo::utils::Int64> const &node_uids,
                   std::vector<Neo::utils::Int64> const &cell_uids,
                   std::vector<Neo::utils::Int64> const &face_uids,
                   std::vector<Neo::utils::Real3>& node_coords, // not const since they can be moved
                   std::vector<Neo::utils::Int64>& cell_nodes,
                   std::vector<Neo::utils::Int64>& cell_faces,
                   std::vector<Neo::utils::Int64>& face_nodes,
                   std::vector<Neo::utils::Int64>& face_cells,
                   std::vector<size_t>&& nb_node_per_cells,
                   std::vector<size_t>&& nb_face_per_cells,
                   std::vector<size_t>&& nb_node_per_faces,
                   std::vector<size_t>&& nb_cell_per_faces) {
    auto &cell_family = addCellFamily(mesh, StaticMesh::cell_family_name);
    auto &node_family = addNodeFamily(mesh, StaticMesh::node_family_name);
    auto &face_family = addFaceFamily(mesh, StaticMesh::face_family_name);
    mesh.beginUpdate();
    auto added_cells = Neo::AddedItemRange{};
    auto added_nodes = Neo::AddedItemRange{};
    auto added_faces = Neo::AddedItemRange{};
    StaticMesh::addItems(mesh, cell_family, cell_uids, added_cells);
    StaticMesh::addItems(mesh, node_family, node_uids, added_nodes);
    StaticMesh::addItems(mesh, face_family, face_uids, added_faces);
    StaticMesh::setNodeCoords(mesh, node_family, added_nodes, node_coords);
    StaticMesh::addConnectivity(mesh, cell_family, added_cells, node_family,
                                std::move(nb_node_per_cells), cell_nodes);
    StaticMesh::addConnectivity(mesh, face_family, added_faces, node_family,
                                std::move(nb_node_per_faces), face_nodes);
    StaticMesh::addConnectivity(mesh, cell_family, added_cells, face_family,
                                std::move(nb_face_per_cells), cell_faces);
    StaticMesh::addConnectivity(mesh, face_family, added_faces, cell_family,
                                std::move(nb_cell_per_faces), face_cells);
    auto valid_mesh_state = mesh.endUpdate();
    auto &new_cells = added_cells.get(valid_mesh_state);
    auto &new_nodes = added_nodes.get(valid_mesh_state);
    auto &new_faces = added_faces.get(valid_mesh_state);
    std::cout << "Added cells range after endUpdate: " << new_cells;
    std::cout << "Added nodes range after endUpdate: " << new_nodes;
    std::cout << "Added faces range after endUpdate: " << new_faces;
  }

  void addCells(Neo::Mesh &mesh) {
    std::vector<Neo::utils::Int64> node_uids{0, 1, 2, 3, 4, 5};
    std::vector<Neo::utils::Int64> cell_uids{0};
    std::vector<Neo::utils::Int64> face_uids{0, 1, 2, 3, 4, 5, 6, 7};

    std::vector<Neo::utils::Real3> node_coords{
        {-1, -1, 0}, {-1, 1, 0}, {1, 1, 0}, {1, -1, 0}, {0, 0, 1}, {0, 0, -1}};

    std::vector<Neo::utils::Int64> cell_nodes{0, 1, 2, 3, 4, 5};
    std::vector<Neo::utils::Int64> cell_faces{0, 1, 2, 3, 4, 5, 6, 7};

    std::vector<Neo::utils::Int64> face_nodes{0, 1, 4, 0, 1, 5, 1, 2, 4, 1, 2, 5,
                                              2, 3, 4, 2, 3, 5, 3, 0, 4, 3, 0, 5};
    std::vector<Neo::utils::Int64> face_cells(face_uids.size(),0);

    auto nb_node_per_cell = 6;
    auto nb_node_per_face = 3;
    auto nb_face_per_cell = 8;
    auto nb_cell_per_face = 1;
    _createMesh(mesh, node_uids, cell_uids, face_uids, node_coords, cell_nodes,
                cell_faces, face_nodes, face_cells,
                std::vector<size_t>(cell_uids.size(),nb_node_per_cell),
                std::vector<size_t>(cell_uids.size(),nb_face_per_cell),
                std::vector<size_t>(face_uids.size(),nb_node_per_face),
                std::vector<size_t>(face_uids.size(),nb_cell_per_face));
    // Validation
    auto cell_family = mesh.getFamily(Neo::ItemKind::IK_Cell,StaticMesh::cell_family_name);
    auto node_family = mesh.getFamily(Neo::ItemKind::IK_Node,StaticMesh::node_family_name);
    auto face_family = mesh.getFamily(Neo::ItemKind::IK_Face,StaticMesh::face_family_name);
    EXPECT_EQ(cell_uids.size(),cell_family.nbElements());
    EXPECT_EQ(node_uids.size(),node_family.nbElements());
    EXPECT_EQ(face_uids.size(),face_family.nbElements());
    // Check cell to nodes connectivity
    std::vector<Neo::utils::Int64> reconstructed_cell_nodes;
    auto cell_to_nodes = StaticMesh::getConnectivity(mesh,cell_family,node_family);
    for (auto cell : cell_family.all()) {
      auto current_cell_nodes = cell_to_nodes[cell];
      reconstructed_cell_nodes.insert(reconstructed_cell_nodes.end(),current_cell_nodes.begin(),current_cell_nodes.end());
    }
    EXPECT_TRUE(std::equal(cell_nodes.begin(),cell_nodes.end(),reconstructed_cell_nodes.begin()));
    _printContainer(reconstructed_cell_nodes, "Recons cell nodes ");
    // Check face to cells connectivity
    std::vector<Neo::utils::Int64> reconstructed_face_cells;
    auto face_to_cells = StaticMesh::getConnectivity(mesh,face_family,cell_family);
    for (auto face : face_family.all()) {
      auto current_face_cells = face_to_cells[face];
      reconstructed_face_cells.insert(reconstructed_face_cells.end(),current_face_cells.begin(),current_face_cells.end());
    }
    EXPECT_TRUE(std::equal(face_cells.begin(),face_cells.end(),reconstructed_face_cells.begin()));
    _printContainer(reconstructed_face_cells, "Recons face cells ");
  }
}

namespace XdmfTest {
  void exportMesh(Neo::Mesh const& mesh, std::string const& file_name)
  {
    auto domain = XdmfDomain::New();
    auto domain_info = XdmfInformation::New("Domain", " For polyhedral data from Neo");
    domain->insert(domain_info);
    // Needed ?
    auto xdmf_grid = XdmfUnstructuredGrid::New();
    auto xdmf_geom = XdmfGeometry::New();
    xdmf_geom->setType(XdmfGeometryType::XYZ());
    auto node_coords = StaticMesh::getNodeCoords(mesh,mesh.getFamily(Neo::ItemKind::IK_Node,StaticMesh::node_family_name));
    xdmf_geom->insert(0,(double*)node_coords.begin(),node_coords.size()*3,1,1);
    xdmf_grid->setGeometry(xdmf_geom);
    auto xdmf_topo = XdmfTopology::New();
    xdmf_topo->setType(XdmfTopologyType::Polyhedron());
    std::vector<int> cell_data;
    auto& cell_family = mesh.getFamily(Neo::ItemKind::IK_Cell,StaticMesh::cell_family_name);
    auto& face_family = mesh.getFamily(Neo::ItemKind::IK_Face,StaticMesh::face_family_name);
    cell_data.reserve(cell_family.nbElements()*4); // 4 faces by cell approx
    auto& cell_to_faces = StaticMesh::faces(mesh, cell_family);
    auto& face_to_nodes = StaticMesh::nodes(mesh, face_family);
    for (auto cell : cell_family.all()) {
      auto cell_faces = cell_to_faces[cell];
      cell_data.push_back(cell_faces.size());
      for (auto face : cell_faces) {
        auto face_nodes = face_to_nodes[face];
        cell_data.push_back(face_nodes.size());
        cell_data.insert(cell_data.end(),face_nodes.begin(),face_nodes.end());
      }
    }
    xdmf_topo->insert(0, cell_data.data(),cell_data.size(), 1, 1);
    xdmf_grid->setTopology(xdmf_topo);
    domain->insert(xdmf_grid);
//  auto heavydata_writer = XdmfHDF5Writer::New(file_name);
//  auto writer = XdmfWriter::New(file_name, heavydata_writer);
    auto writer = XdmfWriter::New(file_name);
//  domain->accept(heavydata_writer);
    writer->setLightDataLimit(1000);
    domain->accept(writer);
  }
}

TEST(PolyhedralTest,CreateMesh1)
{
  auto mesh = Neo::Mesh{"PolyhedralMesh"};
  PolyhedralMeshTest::addCells(mesh);
}

TEST(PolyhedralTest,ConnectivityUtilitiesTest){
  // get face cells by reversing connectivity
  std::vector<Neo::utils::Int64> cell_uids{0,1,2,3};
  std::vector<Neo::utils::Int64> cell_faces{0,1,2,3,4,5,0,3,5,0,1,2,3};
  std::vector<size_t> nb_face_per_cells{3,3,3,4};
  std::vector<Neo::utils::Int64> face_uids;
  std::vector<Neo::utils::Int64> face_cells;
  std::vector<size_t> nb_cell_per_faces;
  StaticMesh::utilities::reverseConnectivity(cell_uids,cell_faces,nb_face_per_cells,
                                             face_uids,face_cells,nb_cell_per_faces);
  _printContainer(face_uids, "Face uids ");
  _printContainer(face_cells, "Face cells ");
  _printContainer(nb_cell_per_faces, "Cell per faces ");
  std::vector<Neo::utils::Int64> face_uids_ref{0,1,2,3,4,5};
  std::vector<Neo::utils::Int64> face_cells_ref{0,2,3,0,3,0,3,1,2,3,1,1,2};
  std::vector<size_t> nb_cell_per_faces_ref{3,2,2,3,1,2};
  EXPECT_TRUE(std::equal(face_uids.begin(),face_uids.end(),
                         face_uids_ref.begin(),face_uids_ref.end()));
  EXPECT_TRUE(std::equal(face_cells.begin(),face_cells.end(),
                         face_cells_ref.begin(),face_cells_ref.end()));
  EXPECT_TRUE(std::equal(nb_cell_per_faces.begin(),nb_cell_per_faces.end(),
                         nb_cell_per_faces_ref.begin(),nb_cell_per_faces_ref.end()));
}

TEST(PolyhedralTest,TypedUtilitiesTest){
  std::vector<Neo::utils::Int64> cell_nodes{1, 8, 10, 15, 25, 27, 29, 30, // hexa
                                            8, 9, 11, 10, 27, 28, 31, 29, // hexa
                                            28, 9, 11, 31, 32}; // prism
  using CellTypeIndexes = std::vector<int>;
  // Get Face Connectivity info
  std::vector<Neo::utils::Int64> face_nodes;
  std::vector<Neo::utils::Int32> cell_face_indexes;
  int nb_faces = 0;
  StaticMesh::utilities::getItemConnectivityFromCell(
      cell_nodes, CellTypeIndexes{0, 0, 1},
      {{8,
        {{0, 3, 2, 1},
         {1, 2, 6, 5},
         {4, 5, 6, 7},
         {2, 3, 7, 6},
         {0, 3, 7, 4},
         {0, 1, 5, 4}}},
       {5, {{0, 3, 2, 1}, {1, 2, 4}, {2, 3, 4}, {3, 0, 4}, {0, 1, 4}}}},
      nb_faces, face_nodes, cell_face_indexes);
  std::cout << "Nb faces found from cell info " << nb_faces << std::endl;
  _printContainer(face_nodes, "Face nodes from cell info");
  _printContainer(cell_face_indexes, "Cell faces (indexes) from cell info");
  // Get Edge Connectivity info
  std::vector<Neo::utils::Int64> edge_nodes;
  std::vector<Neo::utils::Int32> cell_edge_indexes;
  int nb_edges = 0;
  StaticMesh::utilities::getItemConnectivityFromCell(
      cell_nodes, CellTypeIndexes{0, 0, 1},
      {{8, {{0, 3}, {3, 2}, {2, 1}, {1, 0}, {2, 6}, {6, 5}, {5, 1},
               {4, 5}, {6, 7}, {7, 4}, {3, 7}, {4, 0}}},
       {5, {{0, 3}, {3, 2}, {2, 1}, {1, 0}, {2, 4}, {4, 1}, {3, 4}, {0, 4}}}},
      nb_edges, edge_nodes, cell_edge_indexes);
  std::cout << "Nb edges found from cell info " << nb_edges << std::endl;
  _printContainer(edge_nodes, "Edge nodes from cell info");
  _printContainer(cell_edge_indexes, "Cell edges (indexes) from cell info");
  // Validation
  EXPECT_EQ(15,nb_faces);
  std::vector<int> cell_face_indexes_ref{0,1,2,3,4,5,6,7,8,9,1,10,7,11,12,13,14};
  EXPECT_TRUE(std::equal(
      cell_face_indexes.begin(),cell_face_indexes.end(),
      cell_face_indexes_ref.begin(),cell_face_indexes_ref.end()));
  std::vector<Neo::utils::Int64> face_nodes_ref{1,15,10,8,
                                                8,10,29,27,
                                                25,27,29,30,
                                                10,15,30,29,
                                                1,15,30,25,
                                                1,8,27,25,
                                                8,10,11,9,
                                                9,11,31,28,
                                                27,28,31,29,
                                                11,10,29,31,
                                                8,9,28,27,
                                                9,11,32,
                                                11,31,32,
                                                31,28,32,
                                                28,9,32};
  EXPECT_TRUE(std::equal(
      face_nodes.begin(),face_nodes.end(),
      face_nodes_ref.begin(),face_nodes_ref.end()));
  EXPECT_EQ(24,nb_edges);
  std::vector<int> cell_edge_indexes_ref{0,1,2,3,4,5,6,7,8,9,10,11,2,12,13,14,15,16,17,18,19,5,4,6,16,15,13,17,20,21,22,23};
  std::vector<Neo::utils::Int64> edge_nodes_ref{1,15,  15,10,  10,8,  8,1,
                                                10,29,  29,27,  27,8,  25,27,  29,30,
                                                30,25,  15,30,  25,1,  10,11,  11,9,
                                                9,8,  11,31,  31,28,  28,9, 27,28,  31,29,
                                                11,32,  32,9,  31,32,  28,32};
  EXPECT_TRUE(std::equal(
      edge_nodes.begin(),edge_nodes.end(),
      edge_nodes_ref.begin(),edge_nodes_ref.end()));
  EXPECT_TRUE(std::equal(
      cell_edge_indexes.begin(),cell_edge_indexes.end(),
      cell_edge_indexes_ref.begin(),cell_edge_indexes_ref.end()));
}

#ifdef HAS_XDMF
TEST(PolyhedralTest,CreateXdmfMesh)
{
  auto mesh = Neo::Mesh{"PolyhedralMesh"};
  PolyhedralMeshTest::addCells(mesh);
  auto exported_mesh{"test_output.xmf"};
  XdmfTest::exportMesh(mesh,exported_mesh);
  // todo reimport to check
  auto reader = XdmfReader::New();
  auto exported_primaryDomain = shared_dynamic_cast<XdmfDomain>(reader->read(exported_mesh));
  auto ref_primaryDomain = shared_dynamic_cast<XdmfDomain>(reader->read("../test/meshes/example_cell.xmf"));
  auto exported_topology_str = exported_primaryDomain->getUnstructuredGrid("Grid")->getTopology()->getValuesString();
  auto ref_topology_str = ref_primaryDomain->getUnstructuredGrid("Octahedron")->getTopology()->getValuesString();
  auto exported_geometry_str = exported_primaryDomain->getUnstructuredGrid("Grid")->getGeometry()->getValuesString();
  auto ref_geometry_str = ref_primaryDomain->getUnstructuredGrid("Octahedron")->getGeometry()->getValuesString();
  std::cout << "original topology " << ref_topology_str << std::endl;
  std::cout << "exported topology " << exported_topology_str << std::endl;
  std::cout << "original geometry " << ref_geometry_str<< std::endl;
  std::cout << "exported geometry " << exported_geometry_str << std::endl;
  EXPECT_EQ(std::string{ref_topology_str},std::string{exported_topology_str}); // comparer avec std::equal
  EXPECT_EQ(std::string{ref_geometry_str},std::string{exported_geometry_str}); // comparer avec std::equal

}

TEST(PolyhedralTest,ImportXdmfPolyhedronMesh)
{
  auto reader = XdmfReader::New();
  auto primaryDomain = shared_dynamic_cast<XdmfDomain>(reader->read("../test/meshes/example_mesh.xmf"));
  auto grid = primaryDomain->getUnstructuredGrid("Polyhedra");
  auto geometry = grid->getGeometry();
  geometry->read();
  EXPECT_EQ(geometry->getType()->getName(), XdmfGeometryType::XYZ()->getName());
  std::vector<Neo::utils::Real3> node_coords(geometry->getNumberPoints(),{-1e6,-1e6,-1e6});
  geometry->getValues(0,(double*)node_coords.data(),geometry->getNumberPoints()*3,1,1);
  auto topology = grid->getTopology();
  topology->read();
  // Read only polyhedrons
  EXPECT_EQ(XdmfTopologyType::Polyhedron()->getName(),topology->getType()->getName());
  std::vector<Neo::utils::Int32> cell_data(topology->getSize(),-1);
  topology->getValues(0,cell_data.data(),topology->getSize());
  std::vector<Neo::utils::Int64> cell_uids;
  std::vector<Neo::utils::Int64> face_uids;
  std::vector<Neo::utils::Int64> face_nodes;
  std::set<Neo::utils::Int64> node_uids_set;
  std::vector<Neo::utils::Int64> node_uids;
  std::set<Neo::utils::Int32> current_cell_nodes;
  std::vector<Neo::utils::Int64> cell_nodes;
  std::vector<Neo::utils::Int64> cell_faces;
  std::vector<size_t> nb_node_per_cells;
  std::vector<size_t> nb_face_per_cells;
  using FaceNodes = std::set<int>;
  using FaceUid = Neo::utils::Int64;
  using FaceInfo = std::pair<FaceNodes,FaceUid>;
  auto face_info_comp = [](FaceInfo const& face_info1, FaceInfo const& face_info2){
    return face_info1.first < face_info2.first;
  };
  std::set<FaceInfo, decltype(face_info_comp)> face_nodes_set(face_info_comp);
  face_nodes.reserve(topology->getSize());
  std::vector<size_t> nb_node_per_faces;
  auto cell_index = 0;
  auto face_uid = 0;
  for (auto cell_data_index = 0; cell_data_index< cell_data.size();){
    cell_uids.push_back(cell_index++);
    auto cell_nb_face  = cell_data[cell_data_index++];
    nb_face_per_cells.push_back(cell_nb_face);
    for (auto face_index = 0; face_index< cell_nb_face;++face_index){
      std::size_t face_nb_node = cell_data[cell_data_index++];
      auto current_face_nodes = Neo::utils::ArrayView<Neo::utils::Int32>{face_nb_node,&cell_data[cell_data_index]};
      auto [face_info, is_new_face] = face_nodes_set.emplace(FaceNodes{current_face_nodes.begin(),
                             current_face_nodes.end()},face_uid);
      if (!is_new_face) std::cout << "Face not inserted " << face_uid << std::endl;
      if (is_new_face) {
        face_nodes.insert(face_nodes.end(),current_face_nodes.begin(), current_face_nodes.end());
        nb_node_per_faces.push_back(face_nb_node);
      }
      cell_faces.push_back(face_info->second);
      if (is_new_face) face_uids.push_back(face_uid++);
      current_cell_nodes.insert(current_face_nodes.begin(), current_face_nodes.end());
      cell_data_index += face_nb_node;
    }
    cell_nodes.insert(cell_nodes.end(), current_cell_nodes.begin(),current_cell_nodes.end());
    nb_node_per_cells.push_back(current_cell_nodes.size());
    node_uids_set.insert(current_cell_nodes.begin(),current_cell_nodes.end());
    current_cell_nodes.clear();
  }
  node_uids.insert(node_uids.end(),std::begin(node_uids_set), std::end(node_uids_set));
  _printContainer(face_nodes, "face nodes ");
  _printContainer(face_uids, "face uids ");
  _printContainer(nb_node_per_faces, "nb node per face ");
  _printContainer(node_uids_set, "node uids ");
  _printContainer(cell_nodes, "cell nodes ");
  _printContainer(nb_node_per_cells, "nb node per cell ");
  _printContainer(cell_faces, "cell faces ");
  _printContainer(nb_face_per_cells, "nb face per cell ");
  // local checks
  std::vector<Neo::utils::Int64> cell_uids_ref = {0, 1, 2};
  EXPECT_TRUE(std::equal(cell_uids.begin(),cell_uids.end(),cell_uids_ref.begin(),cell_uids_ref.end()));
  EXPECT_EQ(27,face_uids.size());
  EXPECT_EQ(geometry->getNumberPoints(), node_uids_set.size());
  // import mesh in Neo data structure
  auto mesh = Neo::Mesh{"'ImportedMesh"};
  PolyhedralMeshTest::_createMesh(mesh, node_uids,cell_uids,face_uids,node_coords,cell_nodes,cell_faces,face_nodes,
      std::move(nb_node_per_cells),std::move(nb_face_per_cells),std::move(nb_node_per_faces));
  std::string imported_mesh{"imported_mesh.xmf"};
  XdmfTest::exportMesh(mesh,imported_mesh);
  // Compare with original mesh
  auto created_primaryDomain = shared_dynamic_cast<XdmfDomain>(reader->read(imported_mesh));
  std::cout << "original topology " << topology->getValuesString().c_str()<< std::endl;
  std::cout << "created topology " << created_primaryDomain->getUnstructuredGrid("Grid")->getTopology()->getValuesString().c_str()<< std::endl;
  std::cout << "original geometry " << geometry->getValuesString().c_str()<< std::endl;
  std::cout << "created geometry " << created_primaryDomain->getUnstructuredGrid("Grid")->getGeometry()->getValuesString().c_str()<< std::endl;
  EXPECT_EQ(std::string{geometry->getValuesString().c_str()},std::string{created_primaryDomain->getUnstructuredGrid("Grid")->getGeometry()->getValuesString().c_str()}); // comparer avec std::equal
  EXPECT_EQ(std::string{topology->getValuesString().c_str()},std::string{created_primaryDomain->getUnstructuredGrid("Grid")->getTopology()->getValuesString().c_str()}); // comparer avec std::equal
}

TEST(PolyhedralTest,ImportXdmfHexahedronMesh) {
  auto reader = XdmfReader::New();
  auto primaryDomain = shared_dynamic_cast<XdmfDomain>(
      reader->read("../test/meshes/example_hexahedron.xmf"));
  auto grid = primaryDomain->getUnstructuredGrid("Hexahedron");
  auto geometry = grid->getGeometry();
  geometry->read();
  EXPECT_EQ(geometry->getType()->getName(), XdmfGeometryType::XYZ()->getName());
  std::vector<Neo::utils::Real3> node_coords(geometry->getNumberPoints(),
                                             {-1e6, -1e6, -1e6});
  geometry->getValues(0, (double *)node_coords.data(),
                      geometry->getNumberPoints() * 3, 1, 1);
  auto topology = grid->getTopology();
  topology->read();
  // Read only polyhedrons
  EXPECT_EQ(XdmfTopologyType::Hexahedron()->getName(),
            topology->getType()->getName());
  std::vector<Neo::utils::Int32> cell_data(topology->getSize(), -1);
  topology->getValues(0, cell_data.data(), topology->getSize());
  std::vector<Neo::utils::Int64> cell_uids;
  std::set<Neo::utils::Int64> node_uids_set;
  std::vector<Neo::utils::Int64> node_uids;
  std::set<Neo::utils::Int32> current_cell_nodes;
  std::vector<Neo::utils::Int64> cell_nodes;
  std::size_t cell_nb_nodes = 8;
  auto cell_index = 0;
  for (auto cell_data_index = 0; cell_data_index < cell_data.size();) {
    cell_uids.push_back(cell_index++);
    auto current_cell_nodes = Neo::utils::ConstArrayView<int>{
        cell_nb_nodes, &cell_data[cell_data_index]};
    cell_nodes.insert(cell_nodes.end(), current_cell_nodes.begin(),
                      current_cell_nodes.end());
    node_uids_set.insert(current_cell_nodes.begin(), current_cell_nodes.end());
    cell_data_index += cell_nb_nodes;
  }
  node_uids.insert(node_uids.end(), std::begin(node_uids_set),
                   std::end(node_uids_set));
  _printContainer(node_uids, "node uids ");
  _printContainer(cell_nodes, "cell nodes ");
  auto mesh = Neo::Mesh{"'ImportedHexMesh"};
  using CellTypeIndexes = std::vector<int>;
  std::vector<Neo::utils::Int64> face_nodes;
  std::vector<Neo::utils::Int32> cell_face_indexes;
  int nb_faces = 0;
  StaticMesh::utilities::getItemConnectivityFromCell(
      cell_nodes, CellTypeIndexes{0},
      {{8,
        {{0, 3, 2, 1},
         {1, 2, 6, 5},
         {4, 5, 6, 7},
         {2, 3, 7, 6},
         {0, 3, 7, 4},
         {0, 1, 5, 4}}}},
      nb_faces, face_nodes, cell_face_indexes); //utilities
  //  // On peut symmetriser une connectivité
  //  std::vector<Neo::utils::Int64> node_faces;
  //  StaticMesh::reverseConnectivity(face_nodes,node_faces); // utilities
  std::vector<Neo::utils::Int64> face_uids(nb_faces);
  std::vector<Neo::utils::Int64> cell_faces;
  std::copy(cell_face_indexes.begin(),cell_face_indexes.end(),cell_faces.begin()); // face indexes are taken as uids
  std::iota(face_uids.begin(),face_uids.end(), 0);
  PolyhedralMeshTest::_createMesh(mesh, node_uids, cell_uids, face_uids,
                                  node_coords,cell_nodes,cell_faces,face_nodes,
                                  std::vector<size_t>(cell_uids.size(),8),
                                  std::vector<size_t>(cell_uids.size(),6),
                                  std::vector<size_t>(face_uids.size(),4));
}

#endif