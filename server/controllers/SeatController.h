#pragma once

#include <string>
#include "services/SeatService.h"

class SeatController {
public:
    explicit SeatController(SeatService& service);

    void checkSeatAvailability();
    void showTrainClasses();
    void showFare();

private:
    SeatService& service_;
};
