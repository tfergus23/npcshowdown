SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
kill "$(head -1 $SCRIPT_DIR/../data/mariadb.pid 2> /dev/null)" 2> /dev/null
pkill "ng serve"
pkill "npcshowdown"