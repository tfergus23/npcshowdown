import os.path

species_file = open("species_declarations.txt")

lines = species_file.read().splitlines()
BOX_SPRITES_FOLDER = '../frontend/src/assets/box_sprites/'

for line in lines:
    species_start = line.find('"')+1
    species_end = line.find('"', species_start)
    species = line[species_start:species_end].lower()
    if not os.path.exists(f"{BOX_SPRITES_FOLDER}{species}.png"):
        print(f"couldn't find: {BOX_SPRITES_FOLDER}{species}.png")
    

species_file.close()