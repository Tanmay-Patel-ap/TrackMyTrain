#pragma once

#include <string>
#include "services/PNRService.h"

class PNRController {
public:
    explicit PNRController(PNRService& service);
    void showPNRStatus();

private:
    PNRService& service_;
};
