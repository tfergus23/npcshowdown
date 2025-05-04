#!/bin/bash

echo "Checking dependencies..."

# Check if `mysql` command exists
if ! command -v mysql &> /dev/null; then
    echo "MariaDB is not installed (mysql command not found)."
    exit 1
fi

# Check if it's MariaDB and not MySQL
if ! mysql --version | grep -q "MariaDB"; then
    echo "mysql is installed but it is not MariaDB."
    exit 1
fi

if ! command -v mariadb-install-db &> /dev/null; then
    echo "MariaDB install script not found (mariadb-install-db). Make sure you have MariaDB Server."
    exit 1
fi

echo "MariaDB is installed."

# Check if `cmake` command exists
if ! command -v cmake &> /dev/null; then
    echo "CMake is not installed (cmake command not found)."
    exit 1
fi

# Check if CMake version is >= 3.22
required_version="3.22"
installed_version=$(cmake --version | head -n 1 | awk '{print $3}')

# Compare versions
if [ "$(printf '%s\n' "$required_version" "$installed_version" | sort -V | head -n 1)" != "$required_version" ]; then
    echo "CMake version is too old. Required: >= $required_version, Installed: $installed_version."
    exit 1
fi

echo "CMake $installed_version is installed and meets the version requirement."

#!/bin/bash

# Check if `npm` command exists
if ! command -v npm &> /dev/null; then
    echo "npm is not installed (npm command not found)."
    exit 1
fi

# Check if Angular CLI (`ng`) is installed
if ! command -v ng &> /dev/null; then
    echo "Angular CLI is not installed (ng command not found)."
    exit 1
fi

# Get the installed Angular CLI version
cd frontend
angular_version=$(ng v | grep "Angular CLI" | awk '{print $3}')

# Check if Angular CLI version is >= 16
required_version="16.0.0"

# Compare the installed version with the required version
if [ "$(printf '%s\n' "$required_version" "$angular_version" | sort -V | head -n 1)" != "$required_version" ]; then
    echo "Angular CLI version is too old. Required: >= 16.0.0, Installed: $angular_version."
    exit 1
fi

echo "npm and Angular CLI version $angular_version are installed and meet the version requirement."

npm install

cd ..

cd backend
cmake --preset debug
cd ..
mkdir -p data/data
cd data
touch my.cnf
echo "[mysqld]" >> my.cnf
echo "datadir=./data" >> my.cnf
echo "socket=./mysql.sock" >> my.cnf
echo "port=3306" >> my.cnf
echo "log-error=./mariadb.err" >> my.cnf
echo "pid-file=./mariadb.pid" >> my.cnf

mariadb-install-db --auth-root-authentication-method=normal --defaults-file=./my.cnf

mysqld --defaults-file=./my.cnf --user=$USER &
echo "Waiting for MariaDB to start..."
until mysqladmin ping --socket=./data/mysql.sock --silent; do
    sleep 1
done

mysql --socket ./data/mysql.sock -u root < ../scripts/create_tables.sql

kill "$(head -1 ./mariadb.pid)"

cd ..

echo "You are all set. Run ./run_dev.sh to start the app."