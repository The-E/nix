// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @file DataArray.cpp
 * @brief Implementations of the class DataArray.
 */

#include <pandora/Group.hpp>
#include <pandora/File.hpp>
#include <pandora/Block.hpp>
#include <pandora/DataSet.hpp>
#include <pandora/DataArray.hpp>

using namespace std;

namespace pandora {


const PSize DataArray::MIN_CHUNK_SIZE = {1};
const PSize DataArray::MAX_SIZE_1D = {H5S_UNLIMITED};


DataArray::DataArray(const DataArray &data_array)
  : EntityWithSources(data_array.file, data_array.block, data_array.group, data_array.entity_id)
{
  dimension_group = data_array.dimension_group;
}


DataArray::DataArray(File file, const Block block, Group group, string id)
  : EntityWithSources(file, block, group, id)
{
  dimension_group = group.openGroup("representations");

  if (!group.hasAttr("polynom_coefficients")) {
    vector<double> pc = {1};
    polynomCoefficients(pc);
  }

  if (!group.hasAttr("expansion_origin")) {
    expansionOrigin(0);
  }
}


DataArray::DataArray(File file, const Block block, Group group, string id, time_t time)
  : EntityWithSources(file, block, group, id, time)
{
  dimension_group = group.openGroup("representations");

  if (!group.hasAttr("polynom_coefficients")) {
    vector<double> pc = {1};
    polynomCoefficients(pc);
  }

  if (!group.hasAttr("expansion_origin")) {
    expansionOrigin(0);
  }
}


string DataArray::label() const {
	string value;
	group.getAttr("label", value);
	return value;
}


void DataArray::label(const string &value) {
	group.setAttr("label", value);
	forceUpdatedAt();
}

string DataArray::unit() const {
	string value;
	group.getAttr("unit", value);
	return value;
}

void DataArray::unit(const string &value) {
	group.setAttr("unit", value);
	forceUpdatedAt();
}


double DataArray::expansionOrigin() const {
	double expansion_origin;
	group.getAttr("expansion_origin", expansion_origin);
	return expansion_origin;
}


void DataArray::expansionOrigin(double expansion_origin) {
  group.setAttr("expansion_origin", expansion_origin);
  forceUpdatedAt();
}


vector<double> DataArray::polynomCoefficients()const{
  vector<double> polynom_coefficients;

  if (group.hasData("polynom_coefficients")) {
    DataSet ds = group.openData("polynom_coefficients");
    ds.read(polynom_coefficients, true);
  }

  return polynom_coefficients;
}


void DataArray::polynomCoefficients(vector<double> &coefficients){
  if (group.hasData("polynom_coefficients")) {
    DataSet ds = group.openData("polynom_coefficients");
    ds.extend({coefficients.size()});
    ds.write(coefficients);
  } else {
    DataSet ds = DataSet::create(group.h5Group(), "polynom_coefficients", coefficients, &MAX_SIZE_1D, &MIN_CHUNK_SIZE);
    ds.write(coefficients);
  }
  forceUpdatedAt();
}

// TODO put missing methods here.


DataArray::~DataArray(){
	//dtor
}

} //namespace pandora
