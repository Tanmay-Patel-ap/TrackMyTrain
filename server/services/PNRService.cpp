#include "PNRService.h"
#include "utils/JsonParser.h"
#include "utils/Logger.h"
#include <stdexcept>

PNRService::PNRService(RailwayApiClient& api) : api_(api) {
}

PNR PNRService::getPNRStatus(const std::string& pnr) {
    try {
        auto json = api_.getPNRStatus(pnr);
        return json_parser::parsePNR(json);
    } catch (const std::exception& e) {
        Logger::instance().error("Failed to get PNR status for " +
                                 pnr + ": " + e.what(), __FILE__, __LINE__);
        throw std::runtime_error(std::string("Failed to get PNR status for ") +
                                 pnr + ": " + e.what());
    }
}