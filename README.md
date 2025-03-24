# NPC Showdown
Pit NPC Pokemon trainers against each other in the ultimate tournament!

Remaking [tfergus23/pokemonbattlesim](https://github.com/tfergus23/pokemonbattlesim) in C++ with a web based frontend.

## Development
Clone the repository with 
```
git clone git@github.com:tfergus23/npcshowdown.git --recurse-submodules
```
### Backend
The backend executable can be built with cmake.
```bash
cd backend
mkdir out
cd out
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
./npcs_sim
```
The backend attempts to connect to a MariaDB database on startup. The configuration for this needs to be in a file called `npcs_config.ini`.

Example config file:
```ini
tournament_threads=8
db_user=root
db_password=password
db_host=localhost
db_name=npcs_test
max_user_sessions=10
max_trainers_per_user=10
```
### Frontend
The frontend is an angular app. You can run it in dev mode with
```bash
cd frontend
ng serve
```
Or build it with
```bash
cd frontend
ng build
```
