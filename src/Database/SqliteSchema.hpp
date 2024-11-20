#pragma once

#include <sqlite_orm.h>

#include <FileReader/CellMapInfo.hpp>
#include <FileReader/CellTissueInfo.hpp>
#include <FileReader/CellTrackInfo.hpp>
#include <FileReader/NeedleTrackInfo.hpp>
#include <FileReader/StateTimeDifferencesInfo.hpp>

class SqliteSchema {
public:
    static auto& getDBStorage() {
        using namespace sqlite_orm;
        static auto db = make_storage(
            "DataBase.sqlite",
            make_unique_index("idx_TissueCellID", indexed_column(&CellTissueInfo::TissueCellID).desc()),
            make_index("idx_PerfusionDate", &CellTissueInfo::PerfusionDate),
            make_index("idx_PerfusionTime", &CellTissueInfo::PerfusionTime),
            make_index("idx_PerfusionUser", &CellTissueInfo::PerfusionUser),
            make_index("idx_Status", &CellTissueInfo::Status),
            make_index("idx_FluorescenceResult", &CellTissueInfo::FluorescenceResult),

            make_index("idx_TissueName", &StateTimeDifferenceInfo::TissueName),
            make_index("idx_CellId", &StateTimeDifferenceInfo::CellId),

            make_index("idx_TissueName", &CellMapInfo::TissueName),

            make_table(
                "CellTissueInfo",
                make_column("id", &CellTissueInfo::id,
                            primary_key().autoincrement()),
                make_column("TissueCellID", &CellTissueInfo::TissueCellID,
                            unique()),
                make_column("SamplePreparationDate",
                            &CellTissueInfo::SamplePreparationDate),
                make_column("SamplePreparationTime",
                            &CellTissueInfo::SamplePreparationTime),
                make_column("SamplePreparationStaff",
                            &CellTissueInfo::SamplePreparationStaff),
                make_column("SliceThickness", &CellTissueInfo::SliceThickness),
                make_column("FreshPerfusion", &CellTissueInfo::FreshPerfusion),
                make_column("Status", &CellTissueInfo::Status),
                make_column("DyeName", &CellTissueInfo::DyeName),
                make_column("DyeConcentration",
                            &CellTissueInfo::DyeConcentration),
                make_column("Immunohistochemistry",
                            &CellTissueInfo::Immunohistochemistry),
                make_column("IhcCategory", &CellTissueInfo::IhcCategory),
                make_column("PrimaryAntibodyConcentration",
                            &CellTissueInfo::PrimaryAntibodyConcentration),
                make_column("SecondAntibodyBand",
                            &CellTissueInfo::SecondAntibodyBand),
                make_column("DAPIConcentration",
                            &CellTissueInfo::DAPIConcentration),
                make_column("PerfusionDate", &CellTissueInfo::PerfusionDate),
                make_column("PerfusionTime", &CellTissueInfo::PerfusionTime),
                make_column("AddingTime", &CellTissueInfo::AddingTime),
                make_column("Depth;", &CellTissueInfo::Depth),
                make_column("CurrentIntensity",
                            &CellTissueInfo::CurrentIntensity),
                make_column("PerfusionTimeOn",
                            &CellTissueInfo::PerfusionTimeOn),
                make_column("PerfusionTimeOff",
                            &CellTissueInfo::PerfusionTimeOff),
                make_column("ExperimentTemperature",
                            &CellTissueInfo::ExperimentTemperature),
                make_column("ExperimentHumidity",
                            &CellTissueInfo::ExperimentHumidity),
                make_column("PerfusionUser", &CellTissueInfo::PerfusionUser),
                make_column("CordX", &CellTissueInfo::CordX),
                make_column("CordY", &CellTissueInfo::CordY),
                make_column("CordZ", &CellTissueInfo::CordZ),
                make_column("AddingCordX", &CellTissueInfo::AddingCordX),
                make_column("AddingCordY", &CellTissueInfo::AddingCordY),
                make_column("AddingCordZ", &CellTissueInfo::AddingCordZ),
                make_column("FluorescenceResult",
                            &CellTissueInfo::FluorescenceResult)),

            make_table(
                "CellMapInfo",
                make_column("id", &CellMapInfo::id,
                            primary_key().autoincrement()),
                make_column("TissueName", &CellMapInfo::TissueName),
                make_column("MapIndex", &CellMapInfo::MapIndex),
                make_column("MapBeginCordX", &CellMapInfo::MapBeginCordX),
                make_column("MapBeginCordY", &CellMapInfo::MapBeginCordY),
                make_column("MapEndCordX", &CellMapInfo::MapEndCordY),
                make_column("MapEndCordY", &CellMapInfo::MapEndCordY),
                make_column("PathIndex", &CellMapInfo::PathIndex),
                make_column("SurfaceZ", &CellMapInfo::SurfaceZ),
                make_column("needleIndexs", &CellMapInfo::NeedleNames)
            ),

            make_table("CellTrackInfo",
                       make_column("id", &CellTrackInfo::id,
                                   primary_key().autoincrement()),
                       make_column("TissueName", &CellTrackInfo::TissueName),
                       make_column("CellID", &CellTrackInfo::CellID),
                       make_column("CordX", &CellTrackInfo::CordX),
                       make_column("CordY", &CellTrackInfo::CordY),
                       make_column("CordZ", &CellTrackInfo::CordZ),
                       make_column("TimeStamp", &CellTrackInfo::TimeStamp),
                       make_column("CellArea", &CellTrackInfo::CellArea)),

            make_table("NeedleTrackInfo",
                       make_column("id", &NeedleTrackInfo::id,
                                   primary_key().autoincrement()),
                       make_column("TissueName", &NeedleTrackInfo::TissueName),
                       make_column("CellId", &NeedleTrackInfo::CellId),
                       make_column("NeedleName", &NeedleTrackInfo::NeedleName),
                       make_column("CordX", &NeedleTrackInfo::CordX),
                       make_column("CordY", &NeedleTrackInfo::CordY),
                       make_column("CordZ", &NeedleTrackInfo::CordZ),
                       make_column("TimeStamp", &NeedleTrackInfo::TimeStamp)),

            make_table(
                "StateTimeDifferenceInfo",
                make_column("id", &StateTimeDifferenceInfo::id,
                            primary_key().autoincrement()),
                make_column("TissueName", &StateTimeDifferenceInfo::TissueName),
                make_column("CellId", &StateTimeDifferenceInfo::CellId),
                make_column("CellDepth", &StateTimeDifferenceInfo::CellDepth),
                make_column("IsBadCell", &StateTimeDifferenceInfo::IsBadCell),
                make_column("CellArea", &StateTimeDifferenceInfo::CellArea),
                make_column("CellRadius", &StateTimeDifferenceInfo::CellRadius),
                make_column("InjectionDepth",
                            &StateTimeDifferenceInfo::InjectionDepth),
                make_column("START", &StateTimeDifferenceInfo::START),
                make_column("MOVE_TO_CELL",
                            &StateTimeDifferenceInfo::MOVE_TO_CELL),
                make_column("AUTO_CALIBRATE",
                            &StateTimeDifferenceInfo::AUTO_CALIBRATE),
                make_column("RETREAT_NEEDLE",
                            &StateTimeDifferenceInfo::RETREAT_NEEDLE),
                make_column("FIND_CELL", &StateTimeDifferenceInfo::FIND_CELL),
                make_column("LOWER_NEEDLE",
                            &StateTimeDifferenceInfo::LOWER_NEEDLE),
                make_column("AXIAL_INJECTCELL",
                            &StateTimeDifferenceInfo::AXIAL_INJECTCELL),
                make_column("START_PERFUSE",
                            &StateTimeDifferenceInfo::START_PERFUSE),
                make_column("WAIT", &StateTimeDifferenceInfo::WAIT),
                make_column("WITHDRAW_NEEDLE",
                            &StateTimeDifferenceInfo::WITHDRAW_NEEDLE),
                make_column("LIFT_NEEDLE",
                            &StateTimeDifferenceInfo::LIFT_NEEDLE),
                make_column("END", &StateTimeDifferenceInfo::END))

        );
        db.sync_schema();
        return db;
    }
};
