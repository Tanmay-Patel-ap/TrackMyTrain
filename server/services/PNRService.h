#pragma once

#include <string>
#include "models/PNR.h"
#include "api/RailwayApiClient.h"

class PNRService {
public:
    explicit PNRService(RailwayApiClient& api);
    PNR getPNRStatus(const std::string& pnr);

private:
    RailwayApiClient& api_;
};
