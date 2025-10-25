if [ "$EUID" -ne 0 ]; then
  echo "ERROR: Please run the install script as root."
  exit 1
fi

mkdir -p /npcs
mv npcs-ubuntu-x64-*.tar.gz /npcs
cd /npcs
tar -xzvf npcs-ubuntu-x64-*.tar.gz
rm npcs-ubuntu-x64-*.tar.gz
touch npcs_config.ini

echo "tournament_threads=8" >> npcs_config.ini
echo "db_user=root" >> npcs_config.ini
echo "db_password=" >> npcs_config.ini
echo "db_host=localhost" >> npcs_config.ini
echo "db_name=npcs_test" >> npcs_config.ini
echo "db_port=3307" >> npcs_config.ini
echo "max_user_sessions=10" >> npcs_config.ini
echo "max_trainers_per_user=150" >> npcs_config.ini
echo "website_url=http://localhost:4200" >> npcs_config.ini
echo "api_port=3000" >> npcs_config.ini
echo "serve_static=0" >> npcs_config.ini
echo "static_dir=./static/" >> npcs_config.ini
echo "keep_tournament_days=7" >> npcs_config.ini
echo "max_tournaments_per_day=10000" >> npcs_config.ini
echo "max_signups_per_day=3" >> npcs_config.ini

sudo useradd --system --no-create-home --shell /usr/sbin/nologin svcnpcshowdown

sudo chown -R svcnpcshowdown:svcnpcshowdown /npcs
sudo chmod -R 500 /npcs
sudo chmod 500 npcshowdown
sudo chmod 600 npcs_config.ini

cd -

sudo cp npcshowdown.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable npcshowdown.service
sudo setcap 'cap_net_bind_service=+ep' /npcs/npcshowdown

exit 0

sudo apt update
sudo apt upgrade -y

sudo apt install vim -y

sudo apt install ufw -y
sudo ufw reset
sudo ufw default deny incoming
sudo ufw default allow outgoing
sudo ufw allow 22/tcp
sudo ufw allow 80/tcp
sudo ufw allow 443/tcp
sudo ufw enable

sudo apt install mariadb-server -y
sudo systemctl enable mariadb
sudo mysql_secure_installation

sudo mariadb -u root < create-tables-prod.sql