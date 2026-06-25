# TrackMyTrain

A C++ railway information console application powered by the **RailCore Engine**. Fetches live train data from the [Indian Railways API](https://rapidapi.com/rakeshmk/api/indian-railways-live-train-status/) (RapidAPI) with a built-in mock mode for offline development.

## Features

| # | Feature | Description |
|---|---------|-------------|
| 1 | Train Schedule | Get route, timings, and distance for any train |
| 2 | PNR Status | Check booking/current status of PNR (10-digit) |
| 3 | Live Train Status | Track live location, delay, and next station |
| 4 | Station Status | View arriving and departing trains at a station |
| 5 | Search Train | Find trains by name or number |
| 6 | Trains Between Stations | List trains between two stations on a given date |
| 7 | Seat Availability | Check available seats and fare for a class |
| 8 | Fare Enquiry | Get base and total fare for a train route |

## Architecture

```
Terminal UI
    ↓
Controller Layer    (input validation, display)
    ↓
Service Layer       (business logic, error wrapping)
    ↓
API Layer           (HTTP client, mock file reader)
    ↓
Railway API / Test Data
```

## Tech Stack

- **C++17** with CMake 3.16+
- **MinGW-w64** g++ 15.2.0
- **cpp-httplib** — header-only HTTP client
- **nlohmann/json** — header-only JSON parser
- **RapidAPI** — Indian Railways Live Train Status API

## Project Structure

```
trackmytrain/
├── server/
│   ├── main.cpp                 Entry point and menu loop
│   ├── CMakeLists.txt           Build configuration
│   ├── config/
│   │   └── AppConfig.h/.cpp     Singleton config (API key, mode)
│   ├── api/
│   │   ├── HttpClient.h/.cpp    HTTP GET + mock file reader
│   │   └── RailwayApiClient.h/.cpp  API endpoint methods
│   ├── models/                  11 plain-data structs (Train, PNR, Fare, etc.)
│   ├── utils/
│   │   ├── JsonParser.h/.cpp    JSON → model parsing
│   │   └── Logger.h/.cpp        Singleton file logger with timestamps
│   ├── services/                4 service classes with error handling
│   ├── controllers/             4 controller classes with validation
│   └── test_data/               13 mock JSON files for all features
├── client/                      Future V2 web frontend
├── documentation.md             Development roadmap
└── README.md
```

## Build & Run

### Prerequisites

- CMake 3.16+
- C++17 compiler (g++ 15.2.0 recommended)
- Internet connection (first build fetches dependencies)

### Build

```bash
cd server/build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

### Run

```bash
cd server/build
./TrackMyTrain.exe
```

Press **Enter** at the API key prompt to use MOCK mode with local test data. Enter a RapidAPI key for live data.

### Build Notes

- `TEST_DATA_DIR` is set automatically at compile time — mock files are resolved to `server/test_data/`
- Dependencies (cpp-httplib, nlohmann/json) are fetched via CMake `FetchContent`
- On Windows, links `ws2_32` and `crypt32` for HTTPS support
- Logs written to `trackmytrain.log` in the working directory

## Usage

1. Launch the app
2. Press Enter to use MOCK mode (or paste a RapidAPI key for live data)
3. Select an option from the menu (1-8 or 0 to exit)
4. Follow the prompts for each feature

### Example

```
Enter RapidAPI Key (or press Enter to use MOCK mode):
Mode set to MOCK (using local test data)

        TRACKMYTRAIN - RailCore
========================================
--- Main Menu ---
 1. Train Schedule
 2. PNR Status
 ...
 0. Exit
Choice: 1
```

## Roadmap

- **V1** (current) — Core engine + terminal UI (complete)
- **V2** (planned) — Web frontend in `client/` with REST API backend

See `documentation.md` for the full development roadmap.

## License

MIT
