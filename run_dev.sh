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
cd -
cd data
mariadbd --defaults-file=./my.cnf --user=$USER &
echo "Waiting for MariaDB to start..."
until mysqladmin ping --socket=./data/mysql.sock --silent; do
    sleep 1
done
cd ..
cd backend/out/debug
./npcshowdown &> api.log &
cd -
cd frontend
ng serve &
cd -
trap on_interrupt SIGINT
trap on_hangup SIGHUP
tail -f backend/out/debug/api.log