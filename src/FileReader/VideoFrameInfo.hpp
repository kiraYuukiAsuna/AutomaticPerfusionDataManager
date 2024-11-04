#pragma once

#include <rapidcsv.h>

#include "json.hpp"
#include <string>
#include <vector>

struct NeedleInfo {
    std::string NeedleName;
    float NeedleGlobalPositionX = 0;
    float NeedleGlobalPositionY = 0;
    float NeedleGlobalPositionZ = 0;
    float NeedlePositionX = 0;
    float NeedlePositionY = 0;
    float NeedlePositionZ = 0;
    double Speed = 0;
    double AngleXY = 0;
    double AngleA = 0;

    friend void to_json(nlohmann::json&jsonObject, const NeedleInfo&dataObject) {
        jsonObject["NeedleName"] = dataObject.NeedleName;
        jsonObject["NeedleGlobalPositionX"] = dataObject.NeedleGlobalPositionX;
        jsonObject["NeedleGlobalPositionY"] = dataObject.NeedleGlobalPositionY;
        jsonObject["NeedleGlobalPositionZ"] = dataObject.NeedleGlobalPositionZ;
        jsonObject["NeedlePositionX"] = dataObject.NeedlePositionX;
        jsonObject["NeedlePositionY"] = dataObject.NeedlePositionY;
        jsonObject["NeedlePositionZ"] = dataObject.NeedlePositionZ;
        jsonObject["Speed"] = dataObject.Speed;
        jsonObject["AngleXY"] = dataObject.AngleXY;
        jsonObject["AngleA"] = dataObject.AngleA;
    }

    friend void from_json(const nlohmann::json&jsonObject, NeedleInfo&dataObject) {
        NeedleInfo defaultDataObject;
        dataObject.NeedleName = jsonObject.value("NeedleName", defaultDataObject.NeedleName);
        dataObject.NeedleGlobalPositionX = jsonObject.value("NeedleGlobalPositionX",
                                                            defaultDataObject.NeedleGlobalPositionX);
        dataObject.NeedleGlobalPositionY = jsonObject.value("NeedleGlobalPositionY",
                                                            defaultDataObject.NeedleGlobalPositionY);
        dataObject.NeedleGlobalPositionZ = jsonObject.value("NeedleGlobalPositionZ",
                                                            defaultDataObject.NeedleGlobalPositionZ);
        dataObject.NeedlePositionX = jsonObject.value("NeedlePositionX", defaultDataObject.NeedlePositionX);
        dataObject.NeedlePositionY = jsonObject.value("NeedlePositionY", defaultDataObject.NeedlePositionY);
        dataObject.NeedlePositionZ = jsonObject.value("NeedlePositionZ", defaultDataObject.NeedlePositionZ);
        dataObject.Speed = jsonObject.value("Speed", defaultDataObject.Speed);
        dataObject.AngleXY = jsonObject.value("AngleXY", defaultDataObject.AngleXY);
        dataObject.AngleA = jsonObject.value("AngleA", defaultDataObject.AngleA);
    }
};

struct VideoFrameInfo {
    int FrameCount = 0;
    time_t TimeStamp = 0;
    std::string CurrentNeedleName;
    std::string CurrentNeedleState;
    float CameraGlobalPositionX;
    float CameraGlobalRotationY;
    float CameraGlobalRotationZ;
    float CameraDevicePositionX;
    float CameraDevicePositionY;
    float CameraDevicePositionZ;
    std::vector<NeedleInfo> NeedleInfos;
};

using VideoFrameInfoList = std::vector<VideoFrameInfo>;

VideoFrameInfoList ReadVideoFrameInfoListFromFile(const std::string&filePath) {
    rapidcsv::Document doc(filePath, rapidcsv::LabelParams(0, -1));

    VideoFrameInfoList videoFrameInfoList;
    for (int row = 0; row < doc.GetRowCount(); row++) {
        VideoFrameInfo videoFrameInfo;

        videoFrameInfo.FrameCount = doc.GetCell<int>(0, row);
        videoFrameInfo.TimeStamp = doc.GetCell<time_t>(1, row);
        videoFrameInfo.CurrentNeedleName = doc.GetCell<std::string>(2, row);
        videoFrameInfo.CurrentNeedleState = doc.GetCell<std::string>(3, row);
        videoFrameInfo.CameraGlobalPositionX = doc.GetCell<float>(4, row);
        videoFrameInfo.CameraGlobalRotationY = doc.GetCell<float>(5, row);
        videoFrameInfo.CameraGlobalRotationZ = doc.GetCell<float>(6, row);
        videoFrameInfo.CameraDevicePositionX = doc.GetCell<float>(7, row);
        videoFrameInfo.CameraDevicePositionY = doc.GetCell<float>(8, row);
        videoFrameInfo.CameraDevicePositionZ = doc.GetCell<float>(9, row);

        NeedleInfo needleInfo;
        for (int col = 10; col < doc.GetColumnCount(); col += 9) {
            needleInfo.NeedleName = "";
            needleInfo.NeedleGlobalPositionX = doc.GetCell<float>(col, row);
            needleInfo.NeedleGlobalPositionY = doc.GetCell<float>(col + 1, row);
            needleInfo.NeedleGlobalPositionZ = doc.GetCell<float>(col + 2, row);
            needleInfo.NeedlePositionX = doc.GetCell<float>(col + 3, row);
            needleInfo.NeedlePositionY = doc.GetCell<float>(col + 4, row);
            needleInfo.NeedlePositionZ = doc.GetCell<float>(col + 5, row);
            needleInfo.Speed = doc.GetCell<double>(col + 6, row);
            needleInfo.AngleXY = doc.GetCell<double>(col + 7, row);
            needleInfo.AngleA = doc.GetCell<double>(col + 8, row);
            videoFrameInfo.NeedleInfos.emplace_back(needleInfo);
        }

        videoFrameInfoList.emplace_back(videoFrameInfo);
    }
    return videoFrameInfoList;
}
