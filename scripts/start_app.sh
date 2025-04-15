./scripts/stop_app.sh
cd backend/out/debug
cmake --build .
if [ $? -ne 0 ]; then
    exit $?
fi
~/bin/startmysql
nohup ./npcshowdown &>api.log &
cd -
cd frontend
nohup ng serve &
cd -
tail -f backend/out/debug/api.log