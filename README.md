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
git clone --branch dev --single-branch git@github.com:tfergus23/npcshowdown.git --recurse-submodules
```
And then run the setup script:
```
cd npcshowdown
./setup.sh
```
This will check that you have installed the dependencies, create build files for the backend, and run npm install for the frontend.
### Database
The app relies on a MariaDB database to function. Once created, the tables can be created using `scripts/create_tables.sql`. This will create a database called `npcs_test` (it will drop it first if it already exists) and create an admin user for you to log in with.

The executable attempts to connect to the MariaDB database on startup. The configuration for this needs to be in a file called `npcs_config.ini` in the output directory. You can copy `backend/npcs_config.ini.example` to get started, or, if this file isn't found, the following defaults will be used:
```ini
db_user=root
db_password=password
db_host=localhost
db_name=npcs_test
```
### Running
You can run the app locally with:
```
./run_dev.sh
```
This will:
- Build the backend executable
- Start serving the frontend in dev mode
- Start the database service

By default, the website can be accessed at http://localhost:4200. Killing this script with ctrl-c or by closing the terminal will run `scripts/stop_app.sh`, which will kill the web, app, and database processes.
