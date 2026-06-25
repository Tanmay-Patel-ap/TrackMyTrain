#include "Server.h"
#include <httplib.h>
#include <nlohmann/json.hpp>
#include "utils/Logger.h"

using json = nlohmann::json;

static json toJson(const Train& t) {
    return json{
        {"trainNumber", t.trainNumber},
        {"trainName", t.trainName},
        {"source", t.source},
        {"destination", t.destination},
        {"type", t.type}
    };
}

static json toJson(const ScheduleEntry& e) {
    return json{
        {"stationCode", e.stationCode},
        {"stationName", e.stationName},
        {"arrivalTime", e.arrivalTime},
        {"departureTime", e.departureTime},
        {"distance", e.distance},
        {"day", e.day}
    };
}

static json toJson(const LiveStatus& s) {
    return json{
        {"trainNumber", s.trainNumber},
        {"trainName", s.trainName},
        {"currentStation", s.currentStation},
        {"nextStation", s.nextStation},
        {"delay", s.delay},
        {"lastUpdated", s.lastUpdated}
    };
}

static json toJson(const Passenger& p) {
    return json{
        {"name", p.name},
        {"bookingStatus", p.bookingStatus},
        {"currentStatus", p.currentStatus},
        {"seatNumber", p.seatNumber}
    };
}

static json toJson(const PNR& pnr) {
    json passengers = json::array();
    for (auto& p : pnr.passengers) {
        passengers.push_back(toJson(p));
    }
    return json{
        {"pnrNumber", pnr.pnrNumber},
        {"trainNumber", pnr.trainNumber},
        {"trainName", pnr.trainName},
        {"boardingDate", pnr.boardingDate},
        {"from", pnr.from},
        {"to", pnr.to},
        {"bookingStatus", pnr.bookingStatus},
        {"passengers", passengers}
    };
}

static json toJson(const Station& s) {
    return json{
        {"stationCode", s.stationCode},
        {"stationName", s.stationName}
    };
}

static json toJson(const Train& t, const std::vector<ScheduleEntry>& entries) {
    json route = json::array();
    for (auto& e : entries) {
        route.push_back(toJson(e));
    }
    return json{
        {"train", toJson(t)},
        {"route", route}
    };
}

static json toJson(const StationActivity& a) {
    json arriving = json::array();
    for (auto& t : a.arrivingTrains) arriving.push_back(toJson(t));
    json departing = json::array();
    for (auto& t : a.departingTrains) departing.push_back(toJson(t));
    return json{
        {"stationCode", a.stationCode},
        {"stationName", a.stationName},
        {"arrivingTrains", arriving},
        {"departingTrains", departing}
    };
}

static json toJson(const Route& r) {
    json stations = json::array();
    for (auto& s : r.stations) stations.push_back(toJson(s));
    return json{
        {"train", toJson(r.train)},
        {"stations", stations},
        {"totalDistance", r.totalDistance},
        {"duration", r.duration}
    };
}

static json toJson(const SeatAvailability& a) {
    return json{
        {"trainNumber", a.trainNumber},
        {"trainName", a.trainName},
        {"date", a.date},
        {"from", a.from},
        {"to", a.to},
        {"className", a.className},
        {"availableSeats", a.availableSeats},
        {"fare", a.fare}
    };
}

static json toJson(const Fare& f) {
    return json{
        {"trainNumber", f.trainNumber},
        {"from", f.from},
        {"to", f.to},
        {"className", f.className},
        {"baseFare", f.baseFare},
        {"totalFare", f.totalFare},
        {"distance", f.distance}
    };
}

static json toJson(const TrainClass& c) {
    return json{
        {"classCode", c.classCode},
        {"className", c.className},
        {"fare", c.fare}
    };
}

static json success(const json& data) {
    return json{{"success", true}, {"data", data}};
}

static json error(const std::string& msg) {
    return json{{"success", false}, {"error", msg}};
}

Server::Server(TrainService& trainSvc, PNRService& pnrSvc,
               StationService& stationSvc, SeatService& seatSvc)
    : trainSvc_(trainSvc), pnrSvc_(pnrSvc),
      stationSvc_(stationSvc), seatSvc_(seatSvc) {}

void Server::start(int port) {
    httplib::Server svr;

    auto addCors = [](httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
    };

    svr.Options("(.*)", [](const auto& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "*");
        res.status = 204;
    });

    svr.Get("/api/train/schedule", [&](const auto& req, httplib::Response& res) {
        addCors(res);
        auto trainNo = req.get_param_value("trainNo");
        if (trainNo.empty()) { res.status = 400; res.set_content(error("Missing trainNo parameter").dump(), "application/json"); return; }
        try {
            auto train = trainSvc_.getTrainSchedule(trainNo);
            auto entries = trainSvc_.getScheduleEntries(trainNo);
            res.set_content(success(toJson(train, entries)).dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 500; res.set_content(error(e.what()).dump(), "application/json");
        }
    });

    svr.Get("/api/pnr/status", [&](const auto& req, httplib::Response& res) {
        addCors(res);
        auto pnr = req.get_param_value("pnr");
        if (pnr.empty()) { res.status = 400; res.set_content(error("Missing pnr parameter").dump(), "application/json"); return; }
        try {
            auto result = pnrSvc_.getPNRStatus(pnr);
            res.set_content(success(toJson(result)).dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 500; res.set_content(error(e.what()).dump(), "application/json");
        }
    });

    svr.Get("/api/train/live", [&](const auto& req, httplib::Response& res) {
        addCors(res);
        auto trainNo = req.get_param_value("trainNo");
        if (trainNo.empty()) { res.status = 400; res.set_content(error("Missing trainNo parameter").dump(), "application/json"); return; }
        try {
            auto result = trainSvc_.getLiveStatus(trainNo);
            res.set_content(success(toJson(result)).dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 500; res.set_content(error(e.what()).dump(), "application/json");
        }
    });

    svr.Get("/api/station/status", [&](const auto& req, httplib::Response& res) {
        addCors(res);
        auto code = req.get_param_value("code");
        if (code.empty()) { res.status = 400; res.set_content(error("Missing code parameter").dump(), "application/json"); return; }
        try {
            auto result = stationSvc_.getTrainsByStation(code);
            res.set_content(success(toJson(result)).dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 500; res.set_content(error(e.what()).dump(), "application/json");
        }
    });

    svr.Get("/api/train/search", [&](const auto& req, httplib::Response& res) {
        addCors(res);
        auto query = req.get_param_value("query");
        if (query.empty()) { res.status = 400; res.set_content(error("Missing query parameter").dump(), "application/json"); return; }
        try {
            auto results = trainSvc_.searchTrain(query);
            json arr = json::array();
            for (auto& t : results) arr.push_back(toJson(t));
            res.set_content(success(arr).dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 500; res.set_content(error(e.what()).dump(), "application/json");
        }
    });

    svr.Get("/api/trains/between", [&](const auto& req, httplib::Response& res) {
        addCors(res);
        auto from = req.get_param_value("from");
        auto to = req.get_param_value("to");
        auto date = req.get_param_value("date");
        if (from.empty() || to.empty() || date.empty()) {
            res.status = 400; res.set_content(error("Missing from/to/date parameters").dump(), "application/json"); return;
        }
        try {
            auto result = trainSvc_.getTrainsBetweenStations(from, to, date);
            res.set_content(success(toJson(result)).dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 500; res.set_content(error(e.what()).dump(), "application/json");
        }
    });

    svr.Get("/api/seat/availability", [&](const auto& req, httplib::Response& res) {
        addCors(res);
        auto trainNo = req.get_param_value("trainNo");
        auto from = req.get_param_value("from");
        auto to = req.get_param_value("to");
        auto date = req.get_param_value("date");
        auto cls = req.get_param_value("class");
        if (trainNo.empty() || from.empty() || to.empty() || date.empty() || cls.empty()) {
            res.status = 400; res.set_content(error("Missing trainNo/from/to/date/class parameters").dump(), "application/json"); return;
        }
        try {
            auto result = seatSvc_.checkAvailability(trainNo, from, to, date, cls);
            res.set_content(success(toJson(result)).dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 500; res.set_content(error(e.what()).dump(), "application/json");
        }
    });

    svr.Get("/api/fare", [&](const auto& req, httplib::Response& res) {
        addCors(res);
        auto trainNo = req.get_param_value("trainNo");
        auto from = req.get_param_value("from");
        auto to = req.get_param_value("to");
        auto cls = req.get_param_value("class");
        if (trainNo.empty() || from.empty() || to.empty() || cls.empty()) {
            res.status = 400; res.set_content(error("Missing trainNo/from/to/class parameters").dump(), "application/json"); return;
        }
        try {
            auto result = seatSvc_.getFare(trainNo, from, to, cls);
            res.set_content(success(toJson(result)).dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 500; res.set_content(error(e.what()).dump(), "application/json");
        }
    });

    svr.set_base_dir(CLIENT_DIR);

    Logger::instance().info("Server listening on http://localhost:" + std::to_string(port),
                            __FILE__, __LINE__);
    std::cout << "\nServer started at http://localhost:" << port << "\n";
    std::cout << "Open your browser to access the GUI.\n";
    std::cout << "Press Ctrl+C to stop.\n";

    svr.listen("0.0.0.0", port);
}
