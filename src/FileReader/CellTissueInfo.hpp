#pragma once

#include <rapidcsv.h>

#include <string>
#include <vector>

#include "json.hpp"
#include "Log/Log.h"

struct CellTissueInfo {
	int id = -1;

	std::string TissueCellID;
	std::string SamplePreparationDate;
	std::string SamplePreparationTime;
	std::string SamplePreparationStaff;
	float SliceThickness = 0;
	int FreshPerfusion;
	std::string Status;
	std::string DyeName;
	float DyeConcentration;
	int Immunohistochemistry;
	std::string IhcCategory;
	std::string PrimaryAntibodyConcentration;
	std::string SecondAntibodyBand;
	std::string DAPIConcentration;
	std::string PerfusionDate;
	std::string PerfusionTime;
	std::string AddingTime;
	std::string Depth;
	std::string CurrentIntensity;
	std::string PerfusionTimeOn;
	std::string PerfusionTimeOff;
	std::string ExperimentTemperature;
	std::string ExperimentHumidity;
	std::string PerfusionUser;
	float CordX;
	float CordY;
	float CordZ;
	float AddingCordX;
	float AddingCordY;
	float AddingCordZ;
	std::string FluorescenceResult;

	friend void to_json(nlohmann::json& jsonObject,
						const CellTissueInfo& dataObject) {
		jsonObject["TissueCellID"] = dataObject.TissueCellID;
		jsonObject["SamplePreparationDate"] = dataObject.SamplePreparationDate;
		jsonObject["SamplePreparationTime"] = dataObject.SamplePreparationTime;
		jsonObject["SamplePreparationStaff"] =
			dataObject.SamplePreparationStaff;
		jsonObject["SliceThickness"] = dataObject.SliceThickness;
		jsonObject["FreshPerfusion"] = dataObject.FreshPerfusion;
		jsonObject["Status"] = dataObject.Status;
		jsonObject["DyeName"] = dataObject.DyeName;
		jsonObject["DyeConcentration"] = dataObject.DyeConcentration;
		jsonObject["Immunohistochemistry"] = dataObject.Immunohistochemistry;
		jsonObject["IhcCategory"] = dataObject.IhcCategory;
		jsonObject["PrimaryAntibodyConcentration"] =
			dataObject.PrimaryAntibodyConcentration;
		jsonObject["SecondAntibodyBand"] = dataObject.SecondAntibodyBand;
		jsonObject["DAPIConcentration"] = dataObject.DAPIConcentration;
		jsonObject["PerfusionDate"] = dataObject.PerfusionDate;
		jsonObject["PerfusionTime"] = dataObject.PerfusionTime;
		jsonObject["AddingTime"] = dataObject.AddingTime;
		jsonObject["Depth"] = dataObject.Depth;
		jsonObject["CurrentIntensity"] = dataObject.CurrentIntensity;
		jsonObject["PerfusionTimeOn"] = dataObject.PerfusionTimeOn;
		jsonObject["PerfusionTimeOff"] = dataObject.PerfusionTimeOff;
		jsonObject["ExperimentTemperature"] = dataObject.ExperimentTemperature;
		jsonObject["ExperimentHumidity"] = dataObject.ExperimentHumidity;
		jsonObject["PerfusionUser"] = dataObject.PerfusionUser;
		jsonObject["CordX"] = dataObject.CordX;
		jsonObject["CordY"] = dataObject.CordY;
		jsonObject["CordZ"] = dataObject.CordZ;
		jsonObject["AddingCordX"] = dataObject.AddingCordX;
		jsonObject["AddingCordY"] = dataObject.AddingCordY;
		jsonObject["AddingCordZ"] = dataObject.AddingCordZ;
		jsonObject["FluorescenceResult"] = dataObject.FluorescenceResult;
	}

	friend void from_json(const nlohmann::json& jsonObject,
						  CellTissueInfo& dataObject) {
		CellTissueInfo defaultDataObject;
		dataObject.TissueCellID =
			jsonObject.value("TissueCellID", defaultDataObject.TissueCellID);
		dataObject.SamplePreparationDate = jsonObject.value(
			"SamplePreparationDate", defaultDataObject.SamplePreparationDate);
		dataObject.SamplePreparationTime = jsonObject.value(
			"SamplePreparationTime", defaultDataObject.SamplePreparationTime);
		dataObject.SamplePreparationStaff = jsonObject.value(
			"SamplePreparationStaff", defaultDataObject.SamplePreparationStaff);
		dataObject.SliceThickness = jsonObject.value(
			"SliceThickness", defaultDataObject.SliceThickness);
		dataObject.FreshPerfusion = jsonObject.value(
			"FreshPerfusion", defaultDataObject.FreshPerfusion);
		dataObject.Status =
			jsonObject.value("Status", defaultDataObject.Status);
		dataObject.DyeName =
			jsonObject.value("DyeName", defaultDataObject.DyeName);
		dataObject.DyeConcentration = jsonObject.value(
			"DyeConcentration", defaultDataObject.DyeConcentration);
		dataObject.Immunohistochemistry = jsonObject.value(
			"Immunohistochemistry", defaultDataObject.Immunohistochemistry);
		dataObject.IhcCategory =
			jsonObject.value("IhcCategory", defaultDataObject.IhcCategory);
		dataObject.PrimaryAntibodyConcentration =
			jsonObject.value("PrimaryAntibodyConcentration",
							 defaultDataObject.PrimaryAntibodyConcentration);
		dataObject.SecondAntibodyBand = jsonObject.value(
			"SecondAntibodyBand", defaultDataObject.SecondAntibodyBand);
		dataObject.DAPIConcentration = jsonObject.value(
			"DAPIConcentration", defaultDataObject.DAPIConcentration);
		dataObject.PerfusionDate =
			jsonObject.value("PerfusionDate", defaultDataObject.PerfusionDate);
		dataObject.PerfusionTime =
			jsonObject.value("PerfusionTime", defaultDataObject.PerfusionTime);
		dataObject.AddingTime =
			jsonObject.value("AddingTime", defaultDataObject.AddingTime);
		dataObject.Depth = jsonObject.value("Depth", defaultDataObject.Depth);
		dataObject.CurrentIntensity = jsonObject.value(
			"CurrentIntensity", defaultDataObject.CurrentIntensity);
		dataObject.PerfusionTimeOn = jsonObject.value(
			"PerfusionTimeOn", defaultDataObject.PerfusionTimeOn);
		dataObject.PerfusionTimeOff = jsonObject.value(
			"PerfusionTimeOff", defaultDataObject.PerfusionTimeOff);
		dataObject.ExperimentTemperature = jsonObject.value(
			"ExperimentTemperature", defaultDataObject.ExperimentTemperature);
		dataObject.ExperimentHumidity = jsonObject.value(
			"ExperimentHumidity", defaultDataObject.ExperimentHumidity);
		dataObject.PerfusionUser =
			jsonObject.value("PerfusionUser", defaultDataObject.PerfusionUser);
		dataObject.CordX = jsonObject.value("CordX", defaultDataObject.CordX);
		dataObject.CordY = jsonObject.value("CordY", defaultDataObject.CordY);
		dataObject.CordZ = jsonObject.value("CordZ", defaultDataObject.CordZ);
		dataObject.AddingCordX =
			jsonObject.value("AddingCordX", defaultDataObject.AddingCordX);
		dataObject.AddingCordY =
			jsonObject.value("AddingCordY", defaultDataObject.AddingCordY);
		dataObject.AddingCordZ =
			jsonObject.value("AddingCordZ", defaultDataObject.AddingCordZ);
		dataObject.FluorescenceResult = jsonObject.value(
			"FluorescenceResult", defaultDataObject.FluorescenceResult);
	}
};

using CellTissueInfoList = std::vector<CellTissueInfo>;

inline CellTissueInfoList ReadCellTissueListFromFile(
	const std::filesystem::path& filePath) {
	rapidcsv::Document doc(filePath.string(), rapidcsv::LabelParams(0, -1));

	CellTissueInfoList cellTissueInfoList;
	for (int row = 0; row < doc.GetRowCount(); row++) {
		if(doc.GetColumnCount()<31) {
			SeeleError("CellTissue File {} Has Invalid Column Count!", filePath.string());
			return {};
			throw std::runtime_error(std::format("CellTissue File {} Has Invalid Column Count!", filePath.string()));
		}
		CellTissueInfo cellTissueInfo;
		cellTissueInfo.TissueCellID = doc.GetCell<std::string>(0, row);
		cellTissueInfo.SamplePreparationDate = doc.GetCell<std::string>(1, row);
		cellTissueInfo.SamplePreparationTime = doc.GetCell<std::string>(2, row);
		cellTissueInfo.SamplePreparationStaff =
			doc.GetCell<std::string>(3, row);
		cellTissueInfo.SliceThickness = doc.GetCell<float>(4, row);
		cellTissueInfo.FreshPerfusion = doc.GetCell<int>(5, row);
		cellTissueInfo.Status = doc.GetCell<std::string>(6, row);
		cellTissueInfo.DyeName = doc.GetCell<std::string>(7, row);
		cellTissueInfo.DyeConcentration = doc.GetCell<float>(8, row);
		cellTissueInfo.Immunohistochemistry = doc.GetCell<int>(9, row);
		cellTissueInfo.IhcCategory = doc.GetCell<std::string>(10, row);
		cellTissueInfo.PrimaryAntibodyConcentration =
			doc.GetCell<std::string>(11, row);
		cellTissueInfo.SecondAntibodyBand = doc.GetCell<std::string>(12, row);
		cellTissueInfo.DAPIConcentration = doc.GetCell<std::string>(13, row);
		cellTissueInfo.PerfusionDate = doc.GetCell<std::string>(14, row);
		cellTissueInfo.PerfusionTime = doc.GetCell<std::string>(15, row);
		cellTissueInfo.AddingTime = doc.GetCell<std::string>(16, row);
		cellTissueInfo.Depth = doc.GetCell<std::string>(17, row);
		cellTissueInfo.CurrentIntensity = doc.GetCell<std::string>(18, row);
		cellTissueInfo.PerfusionTimeOn = doc.GetCell<std::string>(19, row);
		cellTissueInfo.PerfusionTimeOff = doc.GetCell<std::string>(20, row);
		cellTissueInfo.ExperimentTemperature =
			doc.GetCell<std::string>(21, row);
		cellTissueInfo.ExperimentHumidity = doc.GetCell<std::string>(22, row);
		cellTissueInfo.PerfusionUser = doc.GetCell<std::string>(23, row);
		cellTissueInfo.CordX = doc.GetCell<float>(24, row);
		cellTissueInfo.CordY = doc.GetCell<float>(25, row);
		cellTissueInfo.CordZ = doc.GetCell<float>(26, row);
		cellTissueInfo.AddingCordX = doc.GetCell<float>(27, row);
		cellTissueInfo.AddingCordY = doc.GetCell<float>(28, row);
		cellTissueInfo.AddingCordZ = doc.GetCell<float>(29, row);
		cellTissueInfo.FluorescenceResult = doc.GetCell<std::string>(30, row);
		cellTissueInfoList.emplace_back(cellTissueInfo);
	}
	return cellTissueInfoList;
}
