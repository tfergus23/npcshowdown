# NPC Showdown
Pit NPC Pokemon trainers against each other in the ultimate tournament!

Remaking [tfergus23/pokemonbattlesim](https://github.com/tfergus23/pokemonbattlesim) in C++ with a web based frontend.

## Development
Clone the repository with:
```
git clone git@github.com:tfergus23/npcshowdown.git --recurse-submodules
```
### Backend
The backend executable can be built with cmake:
```bash
cd backend
mkdir out
cd out
cmake -DCMAKE_BUILD_TYPE=Release -DNPCS_BUILD_TESTS=True ..
cmake --build .
./npcshowdown
```
The backend attempts to connect to a MariaDB database on startup. The configuration for this needs to be in a file called `npcs_config.ini`. If this file isn't found, the following defaults will be used:
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
The frontend is an angular app.
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
