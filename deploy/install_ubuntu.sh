if [ "$EUID" -ne 0 ]; then
  echo "ERROR: Please run the install script as root."
  exit 1
fi
while true; do
  read -sp "Enter database account password: " dbpassword
  echo
  read -sp "Enter database account password (Repeat): " dbpassword2
  echo

  if [[ "$dbpassword" == "$dbpassword2" ]]; then
    unset dbpassword2
    break
  fi
  echo "Passwords do not match."
  unset dbpassword dbpassword2
done

mkdir -p /npcs
mv npcs-ubuntu-x64-*.tar.gz /npcs
cd /npcs
tar -xzvf npcs-ubuntu-x64-*.tar.gz
rm npcs-ubuntu-x64-*.tar.gz
touch npcs_config.ini

echo "tournament_threads=8" >> npcs_config.ini
echo "db_user=svcnpcshowdown" >> npcs_config.ini
echo "db_password=password" >> npcs_config.ini
echo "db_host=localhost" >> npcs_config.ini
echo "db_name=npcs_prod" >> npcs_config.ini
echo "db_port=3306" >> npcs_config.ini
echo "max_user_sessions=10" >> npcs_config.ini
echo "max_trainers_per_user=150" >> npcs_config.ini
echo "website_url=http://192.168.56.101" >> npcs_config.ini
echo "api_port=8080" >> npcs_config.ini
echo "serve_static=0" >> npcs_config.ini
echo "static_dir=./static/" >> npcs_config.ini
echo "keep_tournament_days=7" >> npcs_config.ini
echo "max_tournaments_per_day=10000" >> npcs_config.ini
echo "max_signups_per_day=3" >> npcs_config.ini

sudo useradd --system --no-create-home --shell /usr/sbin/nologin svcnpcshowdown

sudo chown -R svcnpcshowdown:svcnpcshowdown /npcs
sudo chmod -R 755 /npcs
sudo chmod 500 npcshowdown
sudo chmod 600 npcs_config.ini

cd -

sudo cp npcshowdown.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable npcshowdown.service

sudo apt update
sudo apt upgrade -y
sudo unattended-upgrades

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

echo >> create-tables-prod.sql
echo "CREATE USER 'svcnpcshowdown'@'localhost' IDENTIFIED BY '$dbpassword';" >> create-tables-prod.sql
echo "GRANT ALL PRIVILEGES ON npcs_prod.* TO 'svcnpcshowdown'@'localhost';" >> create-tables-prod.sql
echo "FLUSH PRIVILEGES;" >> create-tables-prod.sql

sudo mariadb -u root < create-tables-prod.sql

sudo apt install nginx -y
sudo cp npcshowdown.conf /etc/nginx/sites-available/
sudo ln -s /etc/nginx/sites-available/npcshowdown.conf /etc/nginx/sites-enabled/
sudo rm /etc/nginx/sites-enabled/default
sudo nginx -t
if [ $? -ne 0 ]; then
  echo "Problem with nginx configuration. Stopping."
  exit 1
fi
sudo chown -R www-data:www-data /npcs/static