# NPC Showdown
⚔️ Pit NPC Pokemon trainers against each other in the ultimate tournament!

[Website](https://npcshowdown.com)

## Introduction
NPC Showdown is a web app and API for simulating round-robin tournaments between custom NPC Pokémon trainers. After a tournament runs, you can compare trainer performance using stats like ELO, win/loss records, and biggest upsets.

## Dependencies
- [CMake ≥ 3.22](https://cmake.org/download/)
- [MariaDB server](https://mariadb.org/download/?t=mariadb&p=mariadb&r=11.4.5&os=Linux&i=systemd&mirror=acorn) (An existing database instance is not needed, just make sure the bin and scripts directories are in your path)
- [Node.js + npm](https://docs.npmjs.com/downloading-and-installing-node-js-and-npm)
- Angular CLI (`npm install -g @angular/cli`)

## Development
**The application currently only supports development on Linux.**

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
