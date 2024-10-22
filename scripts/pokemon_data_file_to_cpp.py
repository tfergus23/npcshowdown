#! /usr/bin/python3

in_file = open("Pokemon.data", "r")

text = in_file.read()

in_file.close()

lines = text.splitlines()[1:]
string_map = ""

for line in lines:
    split_line = line.split(",")
    id = split_line[0]
    name = split_line[1]
    type1 = split_line[2].upper()
    type2 = split_line[3].upper()
    if type2 == "":
        type2 = "NONE"
    weight = split_line[4]
    if "." not in weight:
        weight = weight + ".0"
    weight = weight + "f"

    percent_male = split_line[5]
    if "." not in percent_male:
        percent_male = percent_male + ".0"
    percent_male = percent_male + "f"

    hp = split_line[6]
    attack = split_line[7]
    defense = split_line[8]
    spattack = split_line[9]
    spdefense = split_line[10]
    speed = split_line[11]
    stripped_name = name.replace("'", "").replace(":", "").replace(" ", "_").replace("-", "_").replace(".", "").replace("♀", "_FEMALE").replace("♂", "_MALE")
    string_map += f"{{SPECIES_{stripped_name.upper()}.name, &SPECIES_{stripped_name.upper()}}},\n"
    print(f'inline const Species SPECIES_{stripped_name.upper()}({id}, "{name}", {{{type1}, {type2}}}, {weight}, {percent_male}, {{{hp}, {attack}, {defense}, {spattack}, {spdefense}, {speed}}});')
print(string_map)