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

echo "You are all set. Make sure the database is running, then run ./run_dev.sh to start the app."