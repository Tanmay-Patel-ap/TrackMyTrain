#pragma once

#include <string>

struct HttpResponse {
    int statusCode;
    std::string body;
    bool error;
    std::string errorMessage;
};

class HttpClient {
public:
    HttpClient(const std::string& baseUrl);

    HttpResponse get(const std::string& endpoint,
                     const std::string& apiKey,
                     const std::string& apiHost);

    static std::string readMockFile(const std::string& filename);

private:
    std::string baseUrl_;
};
