// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2023 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* Property                                        (C) 2000-2023             */
/*                                                                           */
/* Classes and tools for Property                                            */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#ifndef NEO_PROPERTY_H
#define NEO_PROPERTY_H

#include <iterator>
#include <numeric>
#include <unordered_map>

#include "neo/Utils.h"
#include "neo/Items.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Neo
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

enum class PropertyStatus
{
  ExistingProperty,
  ComputedProperty
};

/*---------------------------------------------------------------------------*/

template <typename ValueType>
struct PropertyViewIterator
{
 public:
  using iterator_category = std::input_iterator_tag;
  using value_type = ValueType;
  using difference_type = int;
  using pointer = ValueType*;
  using reference = ValueType&;
  using PropertyViewIteratorType = PropertyViewIterator<ValueType>;
  std::vector<int> const& m_indexes;
  typename std::vector<int>::const_iterator m_indexes_interator;
  ValueType* m_data_iterator;

  ValueType& operator*() const noexcept { return *m_data_iterator; }
  ValueType* operator->() const noexcept { return m_data_iterator; }

  bool operator==(PropertyViewIteratorType const& prop_view_iterator) const noexcept {
    return (m_indexes_interator == prop_view_iterator.m_indexes_interator);
  }
  bool operator!=(PropertyViewIteratorType const& prop_view_iterator) const noexcept {
    return !(*this == prop_view_iterator);
  }

  PropertyViewIteratorType& operator++() noexcept {
    _increment_iterator();
    return *this;
  }
  PropertyViewIteratorType operator++(int) noexcept {
    _increment_iterator();
    return *this;
  }
  PropertyViewIteratorType& operator--() noexcept {
    _decrement_iterator();
    return *this;
  }
  PropertyViewIteratorType operator--(int) noexcept {
    _decrement_iterator();
    return *this;
  }
  PropertyViewIteratorType& operator+=(difference_type n) noexcept {
    _increment_iterator(n);
    return *this;
  }
  PropertyViewIteratorType operator+(difference_type n) noexcept {
    _increment_iterator(n);
    return *this;
  }
  PropertyViewIteratorType& operator-=(difference_type n) noexcept {
    _decrement_iterator(n);
    return *this;
  }
  PropertyViewIteratorType operator-(difference_type n) noexcept {
    _decrement_iterator(n);
    return *this;
  }

 protected:
  void _increment_iterator() {
    auto last_index = *m_indexes_interator;
    ++m_indexes_interator;
    if (m_indexes_interator != m_indexes.end()) {
      m_data_iterator += *m_indexes_interator - last_index;
    }
  }
  void _increment_iterator(difference_type n) {
    for (auto i = 0; i < n; ++i) {
      _increment_iterator();
    }
  }
  void _decrement_iterator() {
    if (m_indexes_interator == m_indexes.begin())
      return;
    auto last_index = 0;
    if (m_indexes_interator != m_indexes.end()) {
      last_index = *m_indexes_interator;
    }
    else {
      last_index = *(m_indexes_interator - 1);
    }
    --m_indexes_interator;
    m_data_iterator -= last_index - *m_indexes_interator;
  }
  void _decrement_iterator(difference_type n) {
    for (auto i = 0; i < n; ++i) {
      _decrement_iterator();
    }
  }
};

/*---------------------------------------------------------------------------*/

template <typename ValueType>
class MeshScalarPropertyView
{
 public:
  std::vector<int> const m_indexes;
  Neo::utils::Span<ValueType> m_data_view;

  ValueType& operator[](int index) {
    assert(("Error, exceeds property view size", index < m_indexes.size()));
    return m_data_view[m_indexes[index]];
  }

  int size() const noexcept { return m_indexes.size(); }

  PropertyViewIterator<ValueType> begin() { return { m_indexes, m_indexes.begin()++, m_data_view.begin() + m_indexes[0] }; }
  PropertyViewIterator<ValueType> end() { return { m_indexes, m_indexes.end(), m_data_view.end() }; }
};

//----------------------------------------------------------------------------/

template <typename ValueType>
class MeshScalarPropertyConstView
{
 public:
  std::vector<int> const m_indexes;
  Neo::utils::ConstSpan<ValueType> m_data_view;

  int size() const noexcept { return m_indexes.size(); }

  ValueType const& operator[](int index) const {
    assert(("Error, exceeds property view size", index < m_indexes.size()));
    return m_data_view[m_indexes[index]];
  }

  PropertyViewIterator<ValueType const> begin() { return { m_indexes, m_indexes.begin()++, m_data_view.begin() + m_indexes[0] }; }
  PropertyViewIterator<ValueType const> end() { return { m_indexes, m_indexes.end(), m_data_view.end() }; }
};

/*---------------------------------------------------------------------------*/

class PropertyBase
{
 public:
  std::string m_name;

  PropertyBase() = default;

  explicit PropertyBase(std::string name)
  : m_name(std::move(name)) {}

  std::string name() const noexcept {
    return m_name;
  }
};

/*---------------------------------------------------------------------------*/

template <typename DataType>
class ScalarPropertyT : public PropertyBase
{
 private:
  DataType m_data;

 public:
  ScalarPropertyT() = default;

  explicit ScalarPropertyT(std::string name)
  : PropertyBase(std::move(name)) {}

  void set(DataType const& value) noexcept {
    m_data = value;
  }

  DataType& get() noexcept {
    return m_data;
  }

  DataType& operator()() noexcept {
    return m_data;
  }

  DataType const& get() const noexcept {
    return m_data;
  }
};

/*---------------------------------------------------------------------------*/

template <typename DataType>
class ArrayPropertyT : public PropertyBase
{
 private:
  std::vector<DataType> m_data;

 public:
  ArrayPropertyT() = default;

  explicit ArrayPropertyT(std::string name)
  : PropertyBase(std::move(name)) {}

  std::size_t size() const noexcept { return m_data.size(); };

  void resize(int new_size) { m_data.resize(new_size); }

  void reserve(int new_size) { m_data.reserve(new_size); }

  void push_back(DataType const& value) { m_data.push_back(value); }

  DataType& back() noexcept { m_data.back(); }

  DataType& operator[](std::size_t index) { return m_data[index]; }
  DataType const& operator[](std::size_t index) const { return m_data[index]; }

  auto begin() noexcept { return m_data.begin(); }
  auto end() noexcept { return m_data.end(); }

  void clear() noexcept { m_data.clear(); }

  void debugPrint() const {
    if constexpr (ndebug)
      return;
    std::cout << "= Print array property " << m_name << " =" << std::endl;
    std::cout << m_data;
    std::cout << std::endl;
  }
};

/*---------------------------------------------------------------------------*/

template <typename DataType>
class MeshScalarPropertyT : public PropertyBase
{
 private:
  std::vector<DataType> m_data;

 public:
  MeshScalarPropertyT() = default;

  explicit MeshScalarPropertyT(std::string name)
  : PropertyBase(std::move(name)) {}

  /*!
   * @brief Fill a property (empty or not) with a scalar value, over an item_range.
   * @param item_range: range containing the items that will be set to the \a value
   * @param value
   */
  void init(const ItemRange& item_range, const DataType& value) {
    if (isInitializableFrom(item_range))
      init(item_range, std::vector<DataType>(item_range.size(), value));
    else
      append(item_range, std::vector<DataType>(item_range.size(), value));
  }

  bool isInitializableFrom(const ItemRange& item_range) const { return item_range.isContiguous() && (*item_range.begin() == 0) && m_data.empty(); }

  /*!
   * @brief Fill an \b empty property with an array of values indexed by a range. May copy or move the values.
   * @param item_range: contiguous 0-starting range
   * @param values: give a rvalue (temporary or moved array) to be efficient (they won't be copied).
   * This method tries to avoid copy via move construct. Work only if a rvalue is passed for \a values argument. Property must be empty.
   */
  void init(const ItemRange& item_range, std::vector<DataType> values) {
    // data must be empty
    assert(("Property must be empty and item range contiguous to call init", isInitializableFrom(item_range)));
    m_data = std::move(values);
  }

  /*!
   * @brief Fill a property (empty or not) with an array of values. Always copy values.
   * @param item_range
   * @param values
   * @param default_value: used if \a item_range is a subrange of the property support
   */
  void append(const ItemRange& item_range, const std::vector<DataType>& values, DataType default_value = DataType{}) {
    if (item_range.size() == 0)
      return;
    assert(("item_range and values sizes differ", item_range.size() == values.size()));
    auto max_item = utils::maxItem(item_range);
    if (max_item > m_data.size())
      m_data.resize(max_item + 1, default_value);
    std::size_t counter{ 0 };
    for (auto item : item_range) {
      m_data[item] = values[counter++];
    }
  }

  DataType& operator[](utils::Int32 item) {
    assert(("Input item lid > max local id, In MeshScalarPropertyT[]", item < m_data.size()));
    return m_data[item];
  }
  DataType const& operator[](utils::Int32 item) const {
    assert(("Input item lid > max local id, In MeshScalarPropertyT[]", item < m_data.size()));
    return m_data[item];
  }
  std::vector<DataType> operator[](std::vector<utils::Int32> const& items) const {
    return _arrayAccessor(items);
  }
  std::vector<DataType> operator[](utils::ConstSpan<utils::Int32> const& items) const {
    return _arrayAccessor(items);
  }

  template <typename ArrayType>
  std::vector<DataType> _arrayAccessor(ArrayType items) const {
    if (items.size() == 0)
      return std::vector<DataType>{};
    // check bounds
    assert(("Max input item lid > max local id, In MeshScalarPropertyT[]",
            *(std::max_element(items.begin(), items.end())) < (int)m_data.size()));

    std::vector<DataType> values;
    values.reserve(items.size());
    std::transform(items.begin(), items.end(), std::back_inserter(values),
                   [this](auto item) { return m_data[item]; });
    return values;
  }

  void debugPrint() const {
    if constexpr (ndebug)
      return;
    std::cout << "= Print mesh scalar property " << m_name << " =" << std::endl;
    for (auto& val : m_data) {
      std::cout << "\"" << val << "\" ";
    }
    std::cout << std::endl;
  }

  utils::Span<DataType> values() { return Neo::utils::Span<DataType>{ m_data.data(), m_data.size() }; }

  std::size_t size() const { return m_data.size(); }

  void clear() {
    m_data.clear();
  }

  utils::Span<DataType> view() noexcept {
    return utils::Span<DataType>{ m_data.data(), m_data.size() };
  }

  utils::ConstSpan<DataType> constView() const noexcept {
    return utils::ConstSpan<DataType>{ m_data.data(), m_data.size() };
  }

  /*!
   * @brief returns a view of the values for a given item range
   * @param items
   * @return a MeshScalarPropertyView object pointing to the values of the item range given
   */
  MeshScalarPropertyView<DataType> view(ItemRange const& items) {
    return MeshScalarPropertyView<DataType>{ std::move(items.localIds()), Neo::utils::Span<DataType>{ m_data.data(), m_data.size() } };
  }

  /*!
   * @brief returns a const view of the values for a given item range
   * @param items
   * @return a MeshScalarPropertyConstView object pointing to the values of the item range given
   */
  MeshScalarPropertyConstView<DataType> constView(ItemRange const& items) const {
    return MeshScalarPropertyConstView<DataType>{ std::move(items.localIds()), Neo::utils::Span<DataType>{ const_cast<DataType*>(m_data.data()), m_data.size() } };
  }

  auto begin() noexcept { return m_data.begin(); }
  auto begin() const noexcept { return m_data.begin(); }
  auto end() noexcept { return m_data.end(); }
  auto end() const noexcept { return m_data.end(); }
};

/*---------------------------------------------------------------------------*/

template <typename DataType>
class MeshArrayPropertyT : public PropertyBase
{

 private:
  std::vector<DataType> m_data;
  std::vector<int> m_offsets;
  std::vector<int> m_indexes;
  int m_size;

 public:
  MeshArrayPropertyT() = default;

  explicit MeshArrayPropertyT(std::string name)
  : PropertyBase(std::move(name)) {}

  /*!
  * @brief Resize an array property before a call to \a init. Resize must not be done before a call to \a append method.
  * @param sizes: an array the number of items of the property support and storing the number of values for each item.
  */
  void resize(std::vector<int> sizes) { // only 2 moves if a rvalue is passed. One copy + one move if lvalue
    m_offsets = std::move(sizes);
    _updateIndexes();
  }
  bool isInitializableFrom(const ItemRange& item_range) { return item_range.isContiguous() && (*item_range.begin() == 0) && m_data.empty(); }

  /*!
   * @brief Initialize an \b empty array property. Must call resize first.
   * @param item_range must be a contiguous, 0-starting item range
   * @param values: to be efficient a rvalue should be passed (temporary or moved array).
   * This method tries to avoid copy via move construct. Work only if a rvalue is passed for \a values argument. Property must be empty.
   */
  void init(const ItemRange& item_range, std::vector<DataType> values) {
    assert(("Property must be empty and item range contiguous to call init", isInitializableFrom(item_range)));
    assert(("call resize before init", !item_range.isEmpty() && m_size != 0));
    m_data = std::move(values);
  }

  /*!
   * @brief Fill an array property (empty or not) with an array of values. Always copy values.
   * @param item_range
   * @param values
   * @param nb_values_per_item
   */
  void append(ItemRange const& item_range, std::vector<DataType> const& values, std::vector<int> const& nb_values_per_item) {
    if (item_range.size() == 0)
      return;
    // todo: see how to handle new element add or remove impact on property (size/values)
    assert(item_range.size() == nb_values_per_item.size());
    assert(("connected items array size and nb_values_per_item size are not compatible",
            values.size() == std::accumulate(nb_values_per_item.begin(), nb_values_per_item.end(), 0)));
    if (utils::minItem(item_range) >= m_offsets.size())
      _appendByBackInsertion(item_range, values, nb_values_per_item); // only new items
    else
      _appendByReconstruction(item_range, values, nb_values_per_item); // includes existing items
  }

  utils::Span<DataType> operator[](const utils::Int32 item) {
    assert(("item local id must be >0 in MeshArrayPropertyT::[item_lid]]", item >= 0));
    return utils::Span<DataType>{ m_offsets[item], &m_data[m_indexes[item]] };
  }

  utils::ConstSpan<DataType> operator[](const utils::Int32 item) const {
    assert(("item local id must be >0 in MeshArrayPropertyT::[item_lid]]", item >= 0));
    return utils::ConstSpan<DataType>{ m_offsets[item], &m_data[m_indexes[item]] };
  }

  void debugPrint() const {
    if constexpr (ndebug)
      return;
    std::cout << "= Print mesh array property " << m_name << " =" << std::endl;
    for (auto& val : m_data) {
      std::cout << "\"" << val << "\" ";
    }
    std::cout << std::endl;
    Neo::utils::printContainer(m_offsets, "Offsets ");
    Neo::utils::printContainer(m_indexes, "Indexes");
  }

  /*!
   * @return number of items of property support
   */
  int size() const {
    return m_size;
  }

  void clear() {
    m_data.clear();
    m_offsets.clear();
    m_indexes.clear();
    m_size = 0;
  }

  /*!
   * @brief returns a 1D contiguous view of the property
   * @return a 1D view of the property, the values of the array for each item are contiguous
   */
  utils::Span<DataType> view() noexcept {
    return utils::Span<DataType>{ m_data.data(), m_data.size() };
  }
  /*!
   * @brief returns a const 1D contiguous view of the property
   * @return a const 1D view of the property, the values of the array for each item are contiguous
   */
  utils::ConstSpan<DataType> constView() const noexcept {
    return utils::ConstSpan<DataType>{ m_data.data(), m_data.size() };
  }

  auto begin() noexcept { return m_data.begin(); }
  auto begin() const noexcept { return m_data.begin(); }
  auto end() noexcept { return m_data.end(); }
  auto end() const noexcept { return m_data.end(); }

 private:
  void _appendByReconstruction(ItemRange const& item_range, std::vector<DataType> const& values, std::vector<int> const& nb_connected_item_per_item) {
    Neo::print() << "Append in MeshArrayPropertyT by reconstruction" << std::endl;
    // Compute new offsets
    std::vector<int> new_offsets(m_offsets);
    if (utils::maxItem(item_range) >= new_offsets.size())
      new_offsets.resize(utils::maxItem(item_range) + 1); // todo ajouter MeshArrayPropertyT::resize(maxlid)
    auto index = 0;
    for (auto item : item_range) {
      new_offsets[item] = nb_connected_item_per_item[index++];
    }
    // Compute new indexes
    std::vector<int> new_indexes;
    _computeIndexesFromOffsets(new_indexes, new_offsets);
    // Compute new values
    auto new_data_size = _computeSizeFromOffsets(new_offsets);
    std::vector<DataType> new_data(new_data_size);
    // copy new_values
    auto global_index = 0;
    std::vector<bool> marked_items(new_offsets.size(), false);
    for (auto item : item_range) {
      marked_items[item] = true;
      auto item_index = new_indexes[item];
      for (auto connected_item_index = item_index; connected_item_index < item_index + new_offsets[item]; ++connected_item_index) {
        new_data[connected_item_index] = values[global_index++];
      }
    }
    // copy old values
    ItemRange old_values_range{ ItemLocalIds{ {}, 0, (int)m_offsets.size() } };
    for (auto item : old_values_range) {
      if (!marked_items[item]) {
        auto connected_items = (*this)[item];
        std::copy(connected_items.begin(), connected_items.end(), &new_data[new_indexes[item]]);
      }
    }
    m_offsets = std::move(new_offsets);
    m_indexes = std::move(new_indexes);
    m_data = std::move(new_data);
    m_size = new_data_size;
  }

  void _appendByBackInsertion(ItemRange const& item_range, std::vector<DataType> const& values, std::vector<int> const& nb_connected_item_per_item) {
    if (item_range.isContiguous()) {
      Neo::print() << "Append in MeshArrayPropertyT by back insertion, contiguous range" << std::endl;
      auto max_existing_lid = m_offsets.size() - 1;
      auto min_new_lid = utils::minItem(item_range);
      if (min_new_lid > max_existing_lid + 1) {
        m_offsets.resize(min_new_lid, 0);
      }
      std::copy(nb_connected_item_per_item.begin(),
                nb_connected_item_per_item.end(),
                std::back_inserter(m_offsets));
      std::copy(values.begin(), values.end(), std::back_inserter(m_data));
      _updateIndexes();
    }
    else {
      Neo::print() << "Append in MeshArrayPropertyT by back insertion, non contiguous range" << std::endl;
      m_offsets.resize(utils::maxItem(item_range) + 1);
      auto index = 0;
      for (auto item : item_range)
        m_offsets[item] = nb_connected_item_per_item[index++];
      m_data.resize(m_data.size() + values.size(), DataType());
      _updateIndexes();
      index = 0;
      for (auto item : item_range) {
        auto connected_items = (*this)[item];
        for (auto& connected_item : connected_items) {
          connected_item = values[index++];
        }
      }
    }
  }

  void _updateIndexes() {
    _computeIndexesFromOffsets(m_indexes, m_offsets);
    m_size = _computeSizeFromOffsets(m_offsets);
  }

  void _computeIndexesFromOffsets(std::vector<int>& new_indexes, std::vector<int> const& new_offsets) {
    new_indexes.resize(new_offsets.size());
    auto i = 0, offset_sum = 0;
    for (auto& index : new_indexes) {
      index = offset_sum;
      offset_sum += new_offsets[i++];
    }
    // todo use algo version instead with more recent compilers (gcc >=9, clang >=5)
    //std::exclusive_scan(new_offsets.begin(),new_offsets.end(),new_indexes.begin(),0);
  }

  int _computeSizeFromOffsets(std::vector<int> const& new_offsets) {
    return std::accumulate(new_offsets.begin(), new_offsets.end(), 0);
  }
};

/*---------------------------------------------------------------------------*/

// special case of local ids property
class ItemLidsProperty : public PropertyBase
{
 public:
  explicit ItemLidsProperty(std::string const& name)
  : PropertyBase{ name } {};

  ItemRange append(std::vector<Neo::utils::Int64> const& uids);

  /*! Remove item with uids \a uids.
   *
   * @param uids :unique ids of item to remove. If a non existing uid is given,
   * the code won't throw. The return range will contain a NULL_ITEM_LID for this
   * unexisting uid.
   * @return Returns a range  containing the local ids of removed items. They can
   * be used to update properties. If a local id is NULL_ITEM_LID, it means the
   * corresponding uids was not existing.
   *
   */
  ItemRange remove(std::vector<utils::Int64> const& uids) noexcept;

  std::size_t size() const;

  /*! Access to the item_lids stored in the property through an ItemRange object.
   *
   * @return  the ItemRange containing the lids of the property.
   */
  ItemRange values() const;

  void debugPrint() const;

  utils::Int32 _getLidFromUid(utils::Int64 const uid) const;

  void _getLidsFromUids(std::vector<utils::Int32>& lids, std::vector<utils::Int64> const& uids) const;

  std::vector<utils::Int32> operator[](std::vector<utils::Int64> const& uids) const;

 private:
  std::vector<Neo::utils::Int32> m_empty_lids;
  std::unordered_map<Neo::utils::Int64, Neo::utils::Int32> m_uid2lid;
  int m_last_id = -1;
};

/*---------------------------------------------------------------------------*/

// seems to lead to very high build time with gcc 7.3. To confirm
//template <typename... DataTypes>
//using PropertyTemplate = std::variant<
//MeshScalarPropertyT<DataTypes>...,
//ItemLidsProperty,
//MeshArrayPropertyT<DataTypes>...,
//ScalarPropertyT<DataTypes>...>;
//using Property = PropertyTemplate<utils::Int32, utils::Real3, utils::Int64, bool>;
using Property =
std::variant<
MeshScalarPropertyT<utils::Int32>,
MeshScalarPropertyT<utils::Real3>,
MeshScalarPropertyT<utils::Int64>,
ItemLidsProperty,
MeshArrayPropertyT<utils::Int32>,
ScalarPropertyT<utils::Int32>,
ScalarPropertyT<utils::Real3>,
ArrayPropertyT<utils::Int32>,
ArrayPropertyT<utils::Real3>>;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // namespace Neo

#endif //NEO_PROPERTY_H
