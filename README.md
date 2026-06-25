# TrackMyTrain

A C++ railway information application powered by the **RailCore Engine**. Comes with both a terminal UI and a web GUI. Fetches live train data from the [Indian Railways API](https://rapidapi.com/rakeshmk/api/indian-railways-live-train-status/) (RapidAPI) with a built-in mock mode for offline development.

## Features

| # | Feature | CLI | Web GUI |
|---|---------|-----|---------|
| 1 | Train Schedule | yes | yes |
| 2 | PNR Status | yes | yes |
| 3 | Live Train Status | yes | yes |
| 4 | Station Status | yes | yes |
| 5 | Search Train | yes | yes |
| 6 | Trains Between Stations | yes | yes |
| 7 | Seat Availability | yes | yes |
| 8 | Fare Enquiry | yes | yes |

## Architecture

```
Terminal UI          Web Browser
    ↓                     ↓
Controller Layer     REST API (httplib::Server)
    ↓                     ↓
Service Layer       (business logic, error wrapping)
    ↓
API Layer           (HTTP client, mock file reader)
    ↓
Railway API / Test Data
```

## Tech Stack

- **C++17** with CMake 3.16+
- **MinGW-w64** g++ 15.2.0
- **cpp-httplib** — header-only HTTP client/server
- **nlohmann/json** — header-only JSON parser
- **RapidAPI** — Indian Railways Live Train Status API
- **Vanilla HTML/CSS/JS** — web frontend (no framework)

## Project Structure

```
trackmytrain/
├── server/
│   ├── main.cpp                 Entry point (CLI + --server flag)
│   ├── Server.h/.cpp            REST API server (8 endpoints)
│   ├── CMakeLists.txt           Build configuration
│   ├── config/AppConfig         Singleton config (API key, mode)
│   ├── api/HttpClient           HTTP GET + mock file reader
│   ├── api/RailwayApiClient     API endpoint methods
│   ├── models/                  11 plain-data structs
│   ├── utils/
│   │   ├── JsonParser           JSON → model parsing
│   │   └── Logger               Singleton file logger
│   ├── services/                4 service classes
│   ├── controllers/             4 controller classes
│   └── test_data/               13 mock JSON files
├── client/
│   ├── index.html               Single-page app
│   ├── style.css                Dark theme, responsive
│   └── script.js                Fetch + DOM rendering
└── README.md
```

## Build

```bash
cd server/build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

## Run

### CLI mode

```bash
cd server/build
./TrackMyTrain.exe
```

### Web server mode

```bash
cd server/build
./TrackMyTrain.exe --server
# Open http://localhost:8080
```

Press **Enter** at the API key prompt to use MOCK mode with local test data. Enter a RapidAPI key for live data.

### Custom port

```bash
./TrackMyTrain.exe --server 9090
```

## Build Notes

- `TEST_DATA_DIR` and `CLIENT_DIR` are set automatically at compile time
- Dependencies (cpp-httplib, nlohmann/json) are fetched via CMake `FetchContent`
- On Windows, links `ws2_32` and `crypt32` for HTTPS
- On Linux, links `pthread`
- Logs written to `trackmytrain.log` in the working directory

## License

MIT
