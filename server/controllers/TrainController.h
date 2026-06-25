#pragma once

#include <string>
#include "services/TrainService.h"

class TrainController {
public:
    explicit TrainController(TrainService& service);

    void showTrainSchedule();
    void showLiveStatus();
    void searchTrain();
    void showTrainsBetweenStations();

private:
    TrainService& service_;
};
