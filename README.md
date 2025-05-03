# NPC Showdown
Pit NPC Pokemon trainers against each other in the ultimate tournament!

[Website](https://npcshowdown.com)

## Introduction
NPC Showdown is both a web API and interface for simulating round robin tournaments between NPC Pokemon trainers that you create. Once the tournament is simulated, you can compare the performance of each trainer by looking at stats such as ELO, wins/losses, and biggest upsets.

## Dependencies
- [CMake ≥ 3.22](https://cmake.org/download/)
- [MariaDB server](https://mariadb.org/download/?t=mariadb&p=mariadb&r=11.7.2&os=Linux&cpu=x86_64&i=systemd&mirror=acorn)
- [Node.js + npm](https://docs.npmjs.com/downloading-and-installing-node-js-and-npm)
- Angular CLI (`npm install -g @angular/cli`)

## Development
The application currently only supports development on Linux.

Clone the repository with:
```
git clone git@github.com:tfergus23/npcshowdown.git --recurse-submodules
```
### Database
The app relies on a MariaDB database to function. Once created, the tables can be created using `scripts/create_tables.sql`. This will create a database called `npcs_test` (it will drop it first if it already exists) and create an admin user for you to log in with.
### API
The API executable can be built with cmake:
```bash
cd backend
cmake --preset debug
cd out/debug
cmake --build .
./npcshowdown
```
The executable attempts to connect to the MariaDB database on startup. The configuration for this needs to be in a file called `npcs_config.ini` in the output directory. You can copy `backend/npcs_config.ini.example` to get started, or, if this file isn't found, the following defaults will be used:
```ini
db_user=root
db_password=password
db_host=localhost
db_name=npcs_test
```
### Frontend
The frontend is an angular app.
```bash
cd frontend
npm install
```

You can run it in dev mode with:
```bash
ng serve
```
By default, the app is served on port 4200 for testing.


You can also build the angular app with:
```bash
ng build
```
and then serve the build directory statically. The backend executable can also serve the frontend if the serve_static flag and static_dir variable is set in the config.
