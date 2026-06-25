#include "HttpClient.h"
#include "utils/Logger.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <httplib.h>

HttpClient::HttpClient(const std::string& baseUrl)
    : baseUrl_(baseUrl) {
    if (baseUrl_.empty()) {
        Logger::instance().error("Base URL cannot be empty",
                                 __FILE__, __LINE__);
        throw std::invalid_argument("Base URL cannot be empty");
    }
}

HttpResponse HttpClient::get(const std::string& endpoint,
                              const std::string& apiKey,
                              const std::string& apiHost) {
    HttpResponse resp{0, "", false, ""};

    if (endpoint.empty()) {
        resp.error = true;
        resp.errorMessage = "Endpoint cannot be empty";
        Logger::instance().error("HTTP request failed: endpoint is empty",
                                 __FILE__, __LINE__);
        return resp;
    }
    if (apiKey.empty()) {
        resp.error = true;
        resp.errorMessage = "API key cannot be empty";
        Logger::instance().error("HTTP request failed: API key is empty",
                                 __FILE__, __LINE__);
        return resp;
    }
    if (apiHost.empty()) {
        resp.error = true;
        resp.errorMessage = "API host cannot be empty";
        Logger::instance().error("HTTP request failed: API host is empty",
                                 __FILE__, __LINE__);
        return resp;
    }

    try {
        httplib::Client cli(baseUrl_);
        cli.set_follow_location(true);
        cli.set_connection_timeout(5);
        cli.set_read_timeout(10);

        httplib::Headers headers = {
            {"x-rapidapi-key", apiKey},
            {"x-rapidapi-host", apiHost}
        };

        auto res = cli.Get(endpoint.c_str(), headers);

        if (res) {
            resp.statusCode = res->status;
            resp.body = res->body;
            if (res->status < 200 || res->status >= 300) {
                resp.error = true;
                resp.errorMessage = "HTTP " + std::to_string(res->status) + ": " + res->body;
                Logger::instance().error("HTTP " + std::to_string(res->status) +
                                         " for " + endpoint, __FILE__, __LINE__);
            }
        } else {
            auto err = httplib::to_string(res.error());
            resp.error = true;
            resp.errorMessage = "Connection failed: " + err;
            Logger::instance().error("Connection failed for " + endpoint +
                                     ": " + err, __FILE__, __LINE__);
        }
    } catch (const std::exception& e) {
        resp.error = true;
        resp.errorMessage = std::string("Request error: ") + e.what();
        Logger::instance().error("Request exception for " + endpoint +
                                 ": " + e.what(), __FILE__, __LINE__);
    }

    return resp;
}

std::string HttpClient::readMockFile(const std::string& filename) {
    if (filename.empty()) {
        Logger::instance().error("Mock filename is empty",
                                 __FILE__, __LINE__);
        throw std::invalid_argument("Mock filename cannot be empty");
    }

    std::ifstream file(filename);
    if (!file.is_open()) {
        Logger::instance().error("Mock file not found: " + filename,
                                 __FILE__, __LINE__);
        throw std::runtime_error("Mock file not found: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    if (buffer.str().empty()) {
        Logger::instance().error("Mock file is empty: " + filename,
                                __FILE__, __LINE__);
        throw std::runtime_error("Mock file is empty: " + filename);
    }

    return buffer.str();
}