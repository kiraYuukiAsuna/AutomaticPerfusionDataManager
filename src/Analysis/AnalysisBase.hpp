#pragma once
#include "Database/SqliteSchema.hpp"
#include "Util/Util.hpp"

class AnalysisBase {
private:
    AnalysisBase() {
    }

public:
    ~AnalysisBase() = default;

    static AnalysisBase& getInstance() {
        static AnalysisBase instance;
        return instance;
    }

    auto GetStatusGlobal(std::time_t start = 0, std::time_t end = 0) {
        using namespace sqlite_orm;
        auto db = SqliteSchema::getDBStorage();

        auto results = db.select(
            columns(&CellTissueInfo::Status, count(&CellTissueInfo::Status)),
            group_by(&CellTissueInfo::Status));

        if (false && start == 0 && end == 0) {
            return results;
        }
        else {
            // auto startTimeStr = FormatDateTime(start);
            // auto endTimeStr = FormatDateTime(end);

            std::string startDateStr = "2024-10-23";
            std::string endDateStr = "2024-10-23";

            std::string startTimeStr = "20:07:00";
            std::string endTimeStr = "23:07:00";

            try {
                auto results2 = db.select(
                    columns(&CellTissueInfo::Status, count(&CellTissueInfo::Status)),
                    where(
                        ((c(&CellTissueInfo::PerfusionDate) > startDateStr) ||
                         (c(&CellTissueInfo::PerfusionDate) == startDateStr && c(&CellTissueInfo::PerfusionTime) >= startTimeStr)) &&
                        ((c(&CellTissueInfo::PerfusionDate) < endDateStr) ||
                         (c(&CellTissueInfo::PerfusionDate) == endDateStr && c(&CellTissueInfo::PerfusionTime) <= endTimeStr))
                    ),
                    group_by(&CellTissueInfo::Status));
                return results2;
            }
            catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        }
    }

    auto GetFluorescenceResultGlobal() {
        using namespace sqlite_orm;
        auto db = SqliteSchema::getDBStorage();

        auto results = db.select(columns(&CellTissueInfo::FluorescenceResult,
                                         count(&CellTissueInfo::FluorescenceResult)),
                                 group_by(&CellTissueInfo::FluorescenceResult));

        return results;
    }

    auto GetSuccessRateGlobal() {
        using namespace sqlite_orm;
        auto db = SqliteSchema::getDBStorage();

        // Get the count of each FluorescenceResult
        auto fluorescenceResults =
                db.select(columns(&CellTissueInfo::FluorescenceResult,
                                  count(&CellTissueInfo::FluorescenceResult)),
                          group_by(&CellTissueInfo::FluorescenceResult));

        // Get the count of each Status
        auto statusResults = db.select(
            columns(&CellTissueInfo::Status, count(&CellTissueInfo::Status)),
            group_by(&CellTissueInfo::Status));


        return std::make_tuple(fluorescenceResults, statusResults);
    }

    auto GetTimeRangeGlobal() {
        using namespace sqlite_orm;
        auto db = SqliteSchema::getDBStorage();

        // Retrieve all PerfusionDate and PerfusionTime from the database
        auto results = db.select(
            columns(&CellTissueInfo::PerfusionDate, &CellTissueInfo::PerfusionTime));

        return results;
    }

    auto GetPerfusionResultsGlobal() {
        using namespace sqlite_orm;
        auto db = SqliteSchema::getDBStorage();

        // Retrieve all PerfusionDate and FluorescenceResult from the database
        auto results = db.select(columns(&CellTissueInfo::PerfusionDate,
                                         &CellTissueInfo::FluorescenceResult));

        return results;
    }
};
