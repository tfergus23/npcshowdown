on_interrupt() {
    ./scripts/stop_app.sh
}

on_hangup() {
    ./scripts/stop_app.sh
}

./scripts/stop_app.sh
cd backend/out/debug
cmake --build .
if [ $? -ne 0 ]; then
    exit $?
fi
sudo /etc/init.d/mariadb start
./npcshowdown &> api.log &
cd -
cd frontend
ng serve &
cd -
trap on_interrupt SIGINT
trap on_hangup SIGHUP
tail -f backend/out/debug/api.log