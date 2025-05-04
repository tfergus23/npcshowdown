SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
mysql --socket "$SCRIPT_DIR/../data/data/mysql.sock" -u root --database npcs_test