# NPC Showdown
Pit NPC Pokemon trainers against each other in the ultimate tournament!

[Website](https://npcshowdown.com)

## Introduction
NPC Showdown is both a web API and interface for simulating round robin tournaments between NPC Pokemon trainers that you create. Once the tournament is simulated, you can compare the performance of each trainer by looking at stats such as ELO, wins/losses, and biggest upsets.

## Prerequisites
- CMake ≥ 3.22
- MariaDB server
- Node.js + npm
- Angular CLI (`npm install -g @angular/cli`)

## Development
Clone the repository with:
```
git clone git@github.com:tfergus23/npcshowdown.git --recurse-submodules
```
### Database
The app relies on a MariaDB database to function. Once created, the tables can be created using `scripts/create_tables.sql`.
### API
The API executable can be built with cmake:
```bash
cd backend
mkdir out
cd out
cmake -DCMAKE_BUILD_TYPE=Release -DNPCS_BUILD_TESTS=True ..
cmake --build .
./npcshowdown
```
The executable attempts to connect to the MariaDB database on startup. The configuration for this needs to be in a file called `npcs_config.ini`. You can copy `npcs_config.ini.example` to get started, or, if this file isn't found, the following defaults will be used:
```ini
tournament_threads=8
db_user=root
db_password=password
db_host=localhost
db_name=npcs_test
max_user_sessions=10
max_trainers_per_user=10
website_url=http://localhost:4200
port=3000
serve_static=0
static_dir=./static/
```
### Frontend
The frontend is an angular app. By default, the app is served on port 4200 for testing.
```bash
cd frontend
npm install
```

You can run it in dev mode with:
```bash
ng serve
```
Or build it with:
```bash
ng build
```
and then serve the build directory statically. The backend executable can also serve the frontend if the serve_static flag and static_dir variables are set in the config.
