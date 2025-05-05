# NPC Showdown
⚔️ Pit NPC Pokemon trainers against each other in the ultimate tournament!

[Website](https://npcshowdown.com)

## Introduction
NPC Showdown is a web app and API for simulating round-robin tournaments between custom NPC Pokémon trainers. After a tournament runs, you can compare trainer performance using stats like ELO, win/loss records, and biggest upsets.

## Development
**Currently, development is only supported on Linux**

### Dependencies
- [CMake ≥ 3.22](https://cmake.org/download/)
  - Ubuntu/Debian: `sudo apt install cmake`
  - Fedora: `sudo dnf install cmake`
  - Arch: `sudo pacman -S cmake`
- [MariaDB Server (Stable) >= 10.4](https://mariadb.org/download/) (An existing instance is not needed; a local one will be created for testing. You just need the server tools in your PATH.)
  - Ubuntu/Debian: `sudo apt install mariadb-server`
  - Fedora: `sudo dnf install mariadb`
  - Arch: `sudo pacman -S mariadb`
- [Node.js + npm](https://docs.npmjs.com/downloading-and-installing-node-js-and-npm)
  - Ubuntu/Debian: `sudo apt install nodejs npm`
  - Fedora: `sudo dnf install nodejs npm`
  - Arch: `sudo pacman -S nodejs npm`
- Angular CLI
  - `npm install -g @angular/cli`
### Setup
Clone the repository with:
```
git clone --branch dev --single-branch git@github.com:tfergus23/npcshowdown.git --recurse-submodules
```
And then run the setup script:
```
cd npcshowdown
./setup.sh
```
This will: 
- Check that you have installed the dependencies 
- Create build files for the backend 
- Run npm install for the frontend 
- Install a MariaDB instance into `data/`
- Create the necessary application tables in a database called `npcs_test`.
### Running
You can run the app locally with:
```
./run_dev.sh
```
This will:
- Build the backend executable
- Start serving the frontend in dev mode
- Start the database daemon

By default, the website can be accessed at http://localhost:4200. Killing this script with ctrl-c or by closing the terminal will run `scripts/stop_app.sh`, which will kill the web, app, and database processes.

If you make changes to the backend, you will need to kill and restart the `run_dev.sh` script in order to recompile.
