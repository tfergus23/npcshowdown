while read -r line
do
    wget -P ./sprites https://play.pokemonshowdown.com/sprites/gen5/$line.png > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo "Error: Couldn't get $line"
    fi
done < species.txt