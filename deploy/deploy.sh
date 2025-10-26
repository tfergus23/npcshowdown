rm -rf ./static/
rm ./npcshowdown
tar -xzvf ./npcs-new.tar.gz
cp /npcs/static/assets/AppConfig.json .
rm -rf /npcs/static/
rm /npcs/npcshowdown
cp -R ./static /npcs/
cp npcshowdown /npcs/
chown -R www-data:www-data /npcs/static
chmod -R 755 /npcs/static
chmod 500 /npcs/npcshowdown
chown svcnpcshowdown:svcnpcshowdown /npcs/npcshowdown
cp ./AppConfig.json /npcs/static/assets/
rm ./AppConfig.json
systemctl restart npcshowdown.service
systemctl restart nginx