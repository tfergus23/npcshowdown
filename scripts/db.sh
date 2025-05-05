SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
mariadb --socket "$SCRIPT_DIR/../data/data/mysql.sock" -u root --database npcs_test