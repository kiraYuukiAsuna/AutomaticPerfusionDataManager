#pragma once
#include "Database/SqliteSchema.hpp"
#include "Util/Util.hpp"

struct QueryCreateInfo {
    std::time_t Start = 0;
    std::time_t End = 0;

    std::vector<std::string> FilteredUsers;
};

class AnalysisBase {
private:
    AnalysisBase() = default;

public:
    ~AnalysisBase() = default;

    static auto GetAllUsers() {
        using namespace sqlite_orm;
        auto db = SqliteSchema::getDBStorage();

        auto results = db.select(
            columns(distinct(&CellTissueInfo::PerfusionUser)));

        return results;
    }

    static auto GetTimeRange() {
        using namespace sqlite_orm;
        auto db = SqliteSchema::getDBStorage();

        auto results = db.select(columns(&CellTissueInfo::PerfusionDate,
                                         &CellTissueInfo::PerfusionTime));

        return results;
    }

    static auto GetStatus(const QueryCreateInfo&queryInfo = {}) {
        using namespace sqlite_orm;
        auto db = SqliteSchema::getDBStorage();


        if (queryInfo.Start == 0 && queryInfo.End == 0) {
            auto results = db.select(
                columns(&CellTissueInfo::Status, count(&CellTissueInfo::Status)),
                where(not_in(&CellTissueInfo::PerfusionUser, queryInfo.FilteredUsers)),
                group_by(&CellTissueInfo::Status));
            return results;
        }

        auto [startDateStr, startTimeStr] = FormatDateTime(queryInfo.Start);
        auto [endDateStr, endTimeStr] = FormatDateTime(queryInfo.End);

        // std::string startDateStr = "2024-10-23";
        // std::string endDateStr = "2024-10-23";
        //
        // std::string startTimeStr = "20:07:00";
        // std::string endTimeStr = "23:07:00";


        auto results = db.select(
            columns(&CellTissueInfo::Status, count(&CellTissueInfo::Status)),
            where(((c(&CellTissueInfo::PerfusionDate) > startDateStr) ||
                   (c(&CellTissueInfo::PerfusionDate) == startDateStr &&
                    c(&CellTissueInfo::PerfusionTime) >= startTimeStr)) &&
                  ((c(&CellTissueInfo::PerfusionDate) < endDateStr) ||
                   (c(&CellTissueInfo::PerfusionDate) == endDateStr &&
                    c(&CellTissueInfo::PerfusionTime) <= endTimeStr))
                  && not_in(&CellTissueInfo::PerfusionUser, queryInfo.FilteredUsers)),
            group_by(&CellTissueInfo::Status));
        return results;
    }

    static auto GetFluorescenceResult(const QueryCreateInfo&queryInfo = {}) {
        using namespace sqlite_orm;
        auto db = SqliteSchema::getDBStorage();

        if (queryInfo.Start == 0 && queryInfo.End == 0) {
            auto results =
                    db.select(columns(&CellTissueInfo::FluorescenceResult,
                                      count(&CellTissueInfo::FluorescenceResult)),
                              where(not_in(&CellTissueInfo::PerfusionUser, queryInfo.FilteredUsers)),
                              group_by(&CellTissueInfo::FluorescenceResult));
            return results;
        }

        auto [startDateStr, startTimeStr] = FormatDateTime(queryInfo.Start);
        auto [endDateStr, endTimeStr] = FormatDateTime(queryInfo.End);

        auto results =
                db.select(columns(&CellTissueInfo::FluorescenceResult,
                                  count(&CellTissueInfo::FluorescenceResult)),
                          where(((c(&CellTissueInfo::PerfusionDate) > startDateStr) ||
                                 (c(&CellTissueInfo::PerfusionDate) == startDateStr &&
                                  c(&CellTissueInfo::PerfusionTime) >= startTimeStr)) &&
                                ((c(&CellTissueInfo::PerfusionDate) < endDateStr) ||
                                 (c(&CellTissueInfo::PerfusionDate) == endDateStr &&
                                  c(&CellTissueInfo::PerfusionTime) <= endTimeStr))
                                && not_in(&CellTissueInfo::PerfusionUser, queryInfo.FilteredUsers)),
                          group_by(&CellTissueInfo::FluorescenceResult));
        return results;
    }

    static auto GetPerfusionResults(const QueryCreateInfo&queryInfo = {}) {
        using namespace sqlite_orm;
        auto db = SqliteSchema::getDBStorage();

        if (queryInfo.Start == 0 && queryInfo.End == 0) {
            auto results = db.select(columns(&CellTissueInfo::PerfusionDate,
                                             &CellTissueInfo::FluorescenceResult),
                                     where(not_in(&CellTissueInfo::PerfusionUser, queryInfo.FilteredUsers)));
            return results;
        }

        auto [startDateStr, startTimeStr] = FormatDateTime(queryInfo.Start);
        auto [endDateStr, endTimeStr] = FormatDateTime(queryInfo.End);

        auto results = db.select(columns(&CellTissueInfo::PerfusionDate,
                                         &CellTissueInfo::FluorescenceResult),
                                 where(((c(&CellTissueInfo::PerfusionDate) > startDateStr) ||
                                        (c(&CellTissueInfo::PerfusionDate) == startDateStr &&
                                         c(&CellTissueInfo::PerfusionTime) >= startTimeStr)) &&
                                       ((c(&CellTissueInfo::PerfusionDate) < endDateStr) ||
                                        (c(&CellTissueInfo::PerfusionDate) == endDateStr &&
                                         c(&CellTissueInfo::PerfusionTime) <= endTimeStr))
                                       && not_in(&CellTissueInfo::PerfusionUser, queryInfo.FilteredUsers)));
        return results;
    }

    static auto GetSuccessRate(const QueryCreateInfo&queryInfo = {}) {
        auto fluorescenceResults = GetFluorescenceResult(queryInfo);

        auto statusResults = GetStatus(queryInfo);

        return std::make_tuple(fluorescenceResults, statusResults);
    }
};
