#include "sim/data/Species.hpp"
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

Species::Species(int id, const char* name, const std::array<Type,2>& type, float weightKG, float percentMale, const std::array<int, 6>& baseStats) : id{id}, type{type}, weightKG{weightKG}, percentMale{percentMale}, baseStats{baseStats}{
    strcat(this->name, name);
}

const Species SPECIES_BULBASAUR(1, "Bulbasaur", {Type::GRASS, Type::POISON}, 6.9f, 87.5f, {45, 49, 49, 65, 65, 45});
const Species SPECIES_IVYSAUR(2, "Ivysaur", {Type::GRASS, Type::POISON}, 13.0f, 87.5f, {60, 62, 63, 80, 80, 60});
const Species SPECIES_VENUSAUR(3, "Venusaur", {Type::GRASS, Type::POISON}, 100.0f, 87.5f, {80, 82, 83, 100, 100, 80});
const Species SPECIES_CHARMANDER(4, "Charmander", {Type::FIRE, Type::NONE}, 8.5f, 87.5f, {39, 52, 43, 60, 50, 65});
const Species SPECIES_CHARMELEON(5, "Charmeleon", {Type::FIRE, Type::NONE}, 19.0f, 87.5f, {58, 64, 58, 80, 65, 80});
const Species SPECIES_CHARIZARD(6, "Charizard", {Type::FIRE, Type::FLYING}, 90.5f, 87.5f, {78, 84, 78, 109, 85, 100});
const Species SPECIES_SQUIRTLE(7, "Squirtle", {Type::WATER, Type::NONE}, 9.0f, 87.5f, {44, 48, 65, 50, 64, 43});
const Species SPECIES_WARTORTLE(8, "Wartortle", {Type::WATER, Type::NONE}, 22.5f, 87.5f, {59, 63, 80, 65, 80, 58});
const Species SPECIES_BLASTOISE(9, "Blastoise", {Type::WATER, Type::NONE}, 85.5f, 87.5f, {79, 83, 100, 85, 105, 78});
const Species SPECIES_CATERPIE(10, "Caterpie", {Type::BUG, Type::NONE}, 2.9f, 50.0f, {45, 30, 35, 20, 20, 45});
const Species SPECIES_METAPOD(11, "Metapod", {Type::BUG, Type::NONE}, 9.9f, 50.0f, {50, 20, 55, 25, 25, 30});
const Species SPECIES_BUTTERFREE(12, "Butterfree", {Type::BUG, Type::FLYING}, 32.0f, 50.0f, {60, 45, 50, 80, 80, 70});
const Species SPECIES_WEEDLE(13, "Weedle", {Type::BUG, Type::POISON}, 3.2f, 50.0f, {40, 35, 30, 20, 20, 50});
const Species SPECIES_KAKUNA(14, "Kakuna", {Type::BUG, Type::POISON}, 10.0f, 50.0f, {45, 25, 50, 25, 25, 35});
const Species SPECIES_BEEDRILL(15, "Beedrill", {Type::BUG, Type::POISON}, 29.5f, 50.0f, {65, 80, 40, 45, 80, 75});
const Species SPECIES_PIDGEY(16, "Pidgey", {Type::NORMAL, Type::FLYING}, 1.8f, 50.0f, {40, 45, 40, 35, 35, 56});
const Species SPECIES_PIDGEOTTO(17, "Pidgeotto", {Type::NORMAL, Type::FLYING}, 30.0f, 50.0f, {63, 60, 55, 50, 50, 71});
const Species SPECIES_PIDGEOT(18, "Pidgeot", {Type::NORMAL, Type::FLYING}, 39.5f, 50.0f, {83, 80, 75, 70, 70, 91});
const Species SPECIES_RATTATA(19, "Rattata", {Type::NORMAL, Type::NONE}, 3.5f, 50.0f, {30, 56, 35, 25, 35, 72});
const Species SPECIES_RATICATE(20, "Raticate", {Type::NORMAL, Type::NONE}, 18.5f, 50.0f, {55, 81, 60, 50, 70, 97});
const Species SPECIES_SPEAROW(21, "Spearow", {Type::NORMAL, Type::FLYING}, 2.0f, 50.0f, {40, 60, 30, 31, 31, 70});
const Species SPECIES_FEAROW(22, "Fearow", {Type::NORMAL, Type::FLYING}, 38.0f, 50.0f, {65, 90, 65, 61, 61, 100});
const Species SPECIES_EKANS(23, "Ekans", {Type::POISON, Type::NONE}, 6.9f, 50.0f, {35, 60, 44, 40, 54, 55});
const Species SPECIES_ARBOK(24, "Arbok", {Type::POISON, Type::NONE}, 65.0f, 50.0f, {60, 85, 69, 65, 79, 80});
const Species SPECIES_PIKACHU(25, "Pikachu", {Type::ELECTRIC, Type::NONE}, 6.0f, 50.0f, {35, 55, 30, 50, 40, 90});
const Species SPECIES_RAICHU(26, "Raichu", {Type::ELECTRIC, Type::NONE}, 30.0f, 50.0f, {60, 90, 55, 90, 80, 100});
const Species SPECIES_SANDSHREW(27, "Sandshrew", {Type::GROUND, Type::NONE}, 12.0f, 50.0f, {50, 75, 85, 20, 30, 40});
const Species SPECIES_SANDSLASH(28, "Sandslash", {Type::GROUND, Type::NONE}, 29.5f, 50.0f, {75, 100, 110, 45, 55, 65});
const Species SPECIES_NIDORAN_FEMALE(29, "Nidoran♀", {Type::POISON, Type::NONE}, 7.0f, 0.0f, {55, 47, 52, 40, 40, 41});
const Species SPECIES_NIDORINA(30, "Nidorina", {Type::POISON, Type::NONE}, 20.0f, 0.0f, {70, 62, 67, 55, 55, 56});
const Species SPECIES_NIDOQUEEN(31, "Nidoqueen", {Type::POISON, Type::GROUND}, 60.0f, 0.0f, {90, 82, 87, 75, 85, 76});
const Species SPECIES_NIDORAN_MALE(32, "Nidoran♂", {Type::POISON, Type::NONE}, 9.0f, 100.0f, {46, 57, 40, 40, 40, 50});
const Species SPECIES_NIDORINO(33, "Nidorino", {Type::POISON, Type::NONE}, 19.5f, 100.0f, {61, 72, 57, 55, 55, 65});
const Species SPECIES_NIDOKING(34, "Nidoking", {Type::POISON, Type::GROUND}, 62.0f, 100.0f, {81, 92, 77, 85, 75, 85});
const Species SPECIES_CLEFAIRY(35, "Clefairy", {Type::FAIRY, Type::NONE}, 7.5f, 25.0f, {70, 45, 48, 60, 65, 35});
const Species SPECIES_CLEFABLE(36, "Clefable", {Type::FAIRY, Type::NONE}, 40.0f, 25.0f, {95, 70, 73, 85, 90, 60});
const Species SPECIES_VULPIX(37, "Vulpix", {Type::FIRE, Type::NONE}, 9.9f, 25.0f, {38, 41, 40, 50, 65, 65});
const Species SPECIES_NINETALES(38, "Ninetales", {Type::FIRE, Type::NONE}, 19.9f, 25.0f, {73, 76, 75, 81, 100, 100});
const Species SPECIES_JIGGLYPUFF(39, "Jigglypuff", {Type::NORMAL, Type::FAIRY}, 5.5f, 25.0f, {115, 45, 20, 45, 25, 20});
const Species SPECIES_WIGGLYTUFF(40, "Wigglytuff", {Type::NORMAL, Type::FAIRY}, 12.0f, 25.0f, {140, 70, 45, 75, 50, 45});
const Species SPECIES_ZUBAT(41, "Zubat", {Type::POISON, Type::FLYING}, 7.5f, 50.0f, {40, 45, 35, 30, 40, 55});
const Species SPECIES_GOLBAT(42, "Golbat", {Type::POISON, Type::FLYING}, 55.0f, 50.0f, {75, 80, 70, 65, 75, 90});
const Species SPECIES_ODDISH(43, "Oddish", {Type::GRASS, Type::POISON}, 5.4f, 50.0f, {45, 50, 55, 75, 65, 30});
const Species SPECIES_GLOOM(44, "Gloom", {Type::GRASS, Type::POISON}, 8.6f, 50.0f, {60, 65, 70, 85, 75, 40});
const Species SPECIES_VILEPLUME(45, "Vileplume", {Type::GRASS, Type::POISON}, 18.6f, 50.0f, {75, 80, 85, 100, 90, 50});
const Species SPECIES_PARAS(46, "Paras", {Type::BUG, Type::GRASS}, 5.4f, 50.0f, {35, 70, 55, 45, 55, 25});
const Species SPECIES_PARASECT(47, "Parasect", {Type::BUG, Type::GRASS}, 29.5f, 50.0f, {60, 95, 80, 60, 80, 30});
const Species SPECIES_VENONAT(48, "Venonat", {Type::BUG, Type::POISON}, 30.0f, 50.0f, {60, 55, 50, 40, 55, 45});
const Species SPECIES_VENOMOTH(49, "Venomoth", {Type::BUG, Type::POISON}, 12.5f, 50.0f, {70, 65, 60, 90, 75, 90});
const Species SPECIES_DIGLETT(50, "Diglett", {Type::GROUND, Type::NONE}, 0.8f, 50.0f, {10, 55, 25, 35, 45, 95});
const Species SPECIES_DUGTRIO(51, "Dugtrio", {Type::GROUND, Type::NONE}, 33.3f, 50.0f, {35, 80, 50, 50, 70, 120});
const Species SPECIES_MEOWTH(52, "Meowth", {Type::NORMAL, Type::NONE}, 4.2f, 50.0f, {40, 45, 35, 40, 40, 90});
const Species SPECIES_PERSIAN(53, "Persian", {Type::NORMAL, Type::NONE}, 32.0f, 50.0f, {65, 70, 60, 65, 65, 115});
const Species SPECIES_PSYDUCK(54, "Psyduck", {Type::WATER, Type::NONE}, 19.6f, 50.0f, {50, 52, 48, 65, 50, 55});
const Species SPECIES_GOLDUCK(55, "Golduck", {Type::WATER, Type::NONE}, 76.6f, 50.0f, {80, 82, 78, 95, 80, 85});
const Species SPECIES_MANKEY(56, "Mankey", {Type::FIGHTING, Type::NONE}, 28.0f, 50.0f, {40, 80, 35, 35, 45, 70});
const Species SPECIES_PRIMEAPE(57, "Primeape", {Type::FIGHTING, Type::NONE}, 32.0f, 50.0f, {65, 105, 60, 60, 70, 95});
const Species SPECIES_GROWLITHE(58, "Growlithe", {Type::FIRE, Type::NONE}, 19.0f, 75.0f, {55, 70, 45, 70, 50, 60});
const Species SPECIES_ARCANINE(59, "Arcanine", {Type::FIRE, Type::NONE}, 155.0f, 75.0f, {90, 110, 80, 100, 80, 95});
const Species SPECIES_POLIWAG(60, "Poliwag", {Type::WATER, Type::NONE}, 12.4f, 50.0f, {40, 50, 40, 40, 40, 90});
const Species SPECIES_POLIWHIRL(61, "Poliwhirl", {Type::WATER, Type::NONE}, 20.0f, 50.0f, {65, 65, 65, 50, 50, 90});
const Species SPECIES_POLIWRATH(62, "Poliwrath", {Type::WATER, Type::FIGHTING}, 54.0f, 50.0f, {90, 85, 95, 70, 90, 70});
const Species SPECIES_ABRA(63, "Abra", {Type::PSYCHIC, Type::NONE}, 19.5f, 75.0f, {25, 20, 15, 105, 55, 90});
const Species SPECIES_KADABRA(64, "Kadabra", {Type::PSYCHIC, Type::NONE}, 56.5f, 75.0f, {40, 35, 30, 120, 70, 105});
const Species SPECIES_ALAKAZAM(65, "Alakazam", {Type::PSYCHIC, Type::NONE}, 48.0f, 75.0f, {55, 50, 45, 135, 85, 120});
const Species SPECIES_MACHOP(66, "Machop", {Type::FIGHTING, Type::NONE}, 19.5f, 75.0f, {70, 80, 50, 35, 35, 35});
const Species SPECIES_MACHOKE(67, "Machoke", {Type::FIGHTING, Type::NONE}, 70.5f, 75.0f, {80, 100, 70, 50, 60, 45});
const Species SPECIES_MACHAMP(68, "Machamp", {Type::FIGHTING, Type::NONE}, 130.0f, 75.0f, {90, 130, 80, 65, 85, 55});
const Species SPECIES_BELLSPROUT(69, "Bellsprout", {Type::GRASS, Type::POISON}, 4.0f, 50.0f, {50, 75, 35, 70, 30, 40});
const Species SPECIES_WEEPINBELL(70, "Weepinbell", {Type::GRASS, Type::POISON}, 6.4f, 50.0f, {65, 90, 50, 85, 45, 55});
const Species SPECIES_VICTREEBEL(71, "Victreebel", {Type::GRASS, Type::POISON}, 15.5f, 50.0f, {80, 105, 65, 100, 60, 70});
const Species SPECIES_TENTACOOL(72, "Tentacool", {Type::WATER, Type::POISON}, 45.5f, 50.0f, {40, 40, 35, 50, 100, 70});
const Species SPECIES_TENTACRUEL(73, "Tentacruel", {Type::WATER, Type::POISON}, 55.0f, 50.0f, {80, 70, 65, 80, 120, 100});
const Species SPECIES_GEODUDE(74, "Geodude", {Type::ROCK, Type::GROUND}, 20.0f, 50.0f, {40, 80, 100, 30, 30, 20});
const Species SPECIES_GRAVELER(75, "Graveler", {Type::ROCK, Type::GROUND}, 105.0f, 50.0f, {55, 95, 115, 45, 45, 35});
const Species SPECIES_GOLEM(76, "Golem", {Type::ROCK, Type::GROUND}, 300.0f, 50.0f, {80, 110, 130, 55, 65, 45});
const Species SPECIES_PONYTA(77, "Ponyta", {Type::FIRE, Type::NONE}, 30.0f, 50.0f, {50, 85, 55, 65, 65, 90});
const Species SPECIES_RAPIDASH(78, "Rapidash", {Type::FIRE, Type::NONE}, 95.0f, 50.0f, {65, 100, 70, 80, 80, 105});
const Species SPECIES_SLOWPOKE(79, "Slowpoke", {Type::WATER, Type::PSYCHIC}, 36.0f, 50.0f, {90, 65, 65, 40, 40, 15});
const Species SPECIES_SLOWBRO(80, "Slowbro", {Type::WATER, Type::PSYCHIC}, 78.5f, 50.0f, {95, 75, 110, 100, 80, 30});
const Species SPECIES_MAGNEMITE(81, "Magnemite", {Type::ELECTRIC, Type::STEEL}, 6.0f, -1.0f, {25, 35, 70, 95, 55, 45});
const Species SPECIES_MAGNETON(82, "Magneton", {Type::ELECTRIC, Type::STEEL}, 60.0f, -1.0f, {50, 60, 95, 120, 70, 70});
const Species SPECIES_FARFETCHD(83, "Farfetch'd", {Type::NORMAL, Type::FLYING}, 15.0f, 50.0f, {52, 65, 55, 58, 62, 60});
const Species SPECIES_DODUO(84, "Doduo", {Type::NORMAL, Type::FLYING}, 39.2f, 50.0f, {35, 85, 45, 35, 35, 75});
const Species SPECIES_DODRIO(85, "Dodrio", {Type::NORMAL, Type::FLYING}, 85.2f, 50.0f, {60, 110, 70, 60, 60, 100});
const Species SPECIES_SEEL(86, "Seel", {Type::WATER, Type::NONE}, 90.0f, 50.0f, {65, 45, 55, 45, 70, 45});
const Species SPECIES_DEWGONG(87, "Dewgong", {Type::WATER, Type::ICE}, 120.0f, 50.0f, {90, 70, 80, 70, 95, 70});
const Species SPECIES_GRIMER(88, "Grimer", {Type::POISON, Type::NONE}, 30.0f, 50.0f, {80, 80, 50, 40, 50, 25});
const Species SPECIES_MUK(89, "Muk", {Type::POISON, Type::NONE}, 30.0f, 50.0f, {105, 105, 75, 65, 100, 50});
const Species SPECIES_SHELLDER(90, "Shellder", {Type::WATER, Type::NONE}, 4.0f, 50.0f, {30, 65, 100, 45, 25, 40});
const Species SPECIES_CLOYSTER(91, "Cloyster", {Type::WATER, Type::ICE}, 132.5f, 50.0f, {50, 95, 180, 85, 45, 70});
const Species SPECIES_GASTLY(92, "Gastly", {Type::GHOST, Type::POISON}, 0.1f, 50.0f, {30, 35, 30, 100, 35, 80});
const Species SPECIES_HAUNTER(93, "Haunter", {Type::GHOST, Type::POISON}, 0.1f, 50.0f, {45, 50, 45, 115, 55, 95});
const Species SPECIES_GENGAR(94, "Gengar", {Type::GHOST, Type::POISON}, 40.5f, 50.0f, {60, 65, 60, 130, 75, 110});
const Species SPECIES_ONIX(95, "Onix", {Type::ROCK, Type::GROUND}, 210.0f, 50.0f, {35, 45, 160, 30, 45, 70});
const Species SPECIES_DROWZEE(96, "Drowzee", {Type::PSYCHIC, Type::NONE}, 32.4f, 50.0f, {60, 48, 45, 43, 90, 42});
const Species SPECIES_HYPNO(97, "Hypno", {Type::PSYCHIC, Type::NONE}, 75.6f, 50.0f, {85, 73, 70, 73, 115, 67});
const Species SPECIES_KRABBY(98, "Krabby", {Type::WATER, Type::NONE}, 6.5f, 50.0f, {30, 105, 90, 25, 25, 50});
const Species SPECIES_KINGLER(99, "Kingler", {Type::WATER, Type::NONE}, 60.0f, 50.0f, {55, 130, 115, 50, 50, 75});
const Species SPECIES_VOLTORB(100, "Voltorb", {Type::ELECTRIC, Type::NONE}, 10.4f, -1.0f, {40, 30, 50, 55, 55, 100});
const Species SPECIES_ELECTRODE(101, "Electrode", {Type::ELECTRIC, Type::NONE}, 66.6f, -1.0f, {60, 50, 70, 80, 80, 140});
const Species SPECIES_EXEGGCUTE(102, "Exeggcute", {Type::GRASS, Type::PSYCHIC}, 2.5f, 50.0f, {60, 40, 80, 60, 45, 40});
const Species SPECIES_EXEGGUTOR(103, "Exeggutor", {Type::GRASS, Type::PSYCHIC}, 120.0f, 50.0f, {95, 95, 85, 125, 65, 55});
const Species SPECIES_CUBONE(104, "Cubone", {Type::GROUND, Type::NONE}, 6.5f, 50.0f, {50, 50, 95, 40, 50, 35});
const Species SPECIES_MAROWAK(105, "Marowak", {Type::GROUND, Type::NONE}, 45.0f, 50.0f, {60, 80, 110, 50, 80, 45});
const Species SPECIES_HITMONLEE(106, "Hitmonlee", {Type::FIGHTING, Type::NONE}, 49.8f, 100.0f, {50, 120, 53, 35, 110, 87});
const Species SPECIES_HITMONCHAN(107, "Hitmonchan", {Type::FIGHTING, Type::NONE}, 50.2f, 100.0f, {50, 105, 79, 35, 110, 76});
const Species SPECIES_LICKITUNG(108, "Lickitung", {Type::NORMAL, Type::NONE}, 65.5f, 50.0f, {90, 55, 75, 60, 75, 30});
const Species SPECIES_KOFFING(109, "Koffing", {Type::POISON, Type::NONE}, 1.0f, 50.0f, {40, 65, 95, 60, 45, 35});
const Species SPECIES_WEEZING(110, "Weezing", {Type::POISON, Type::NONE}, 9.5f, 50.0f, {65, 90, 120, 85, 70, 60});
const Species SPECIES_RHYHORN(111, "Rhyhorn", {Type::GROUND, Type::ROCK}, 115.0f, 50.0f, {80, 85, 95, 30, 30, 25});
const Species SPECIES_RHYDON(112, "Rhydon", {Type::GROUND, Type::ROCK}, 120.0f, 50.0f, {105, 130, 120, 45, 45, 40});
const Species SPECIES_CHANSEY(113, "Chansey", {Type::NORMAL, Type::NONE}, 34.6f, 0.0f, {250, 5, 5, 35, 105, 50});
const Species SPECIES_TANGELA(114, "Tangela", {Type::GRASS, Type::NONE}, 35.0f, 50.0f, {65, 55, 115, 100, 40, 60});
const Species SPECIES_KANGASKHAN(115, "Kangaskhan", {Type::NORMAL, Type::NONE}, 80.0f, 0.0f, {105, 95, 80, 40, 80, 90});
const Species SPECIES_HORSEA(116, "Horsea", {Type::WATER, Type::NONE}, 8.0f, 50.0f, {30, 40, 70, 70, 25, 60});
const Species SPECIES_SEADRA(117, "Seadra", {Type::WATER, Type::NONE}, 25.0f, 50.0f, {55, 65, 95, 95, 45, 85});
const Species SPECIES_GOLDEEN(118, "Goldeen", {Type::WATER, Type::NONE}, 15.0f, 50.0f, {45, 67, 60, 35, 50, 63});
const Species SPECIES_SEAKING(119, "Seaking", {Type::WATER, Type::NONE}, 39.0f, 50.0f, {80, 92, 65, 65, 80, 68});
const Species SPECIES_STARYU(120, "Staryu", {Type::WATER, Type::NONE}, 34.5f, -1.0f, {30, 45, 55, 70, 55, 85});
const Species SPECIES_STARMIE(121, "Starmie", {Type::WATER, Type::PSYCHIC}, 80.0f, -1.0f, {60, 75, 85, 100, 85, 115});
const Species SPECIES_MR_MIME(122, "Mr. Mime", {Type::PSYCHIC, Type::FAIRY}, 54.5f, 50.0f, {40, 45, 65, 100, 120, 90});
const Species SPECIES_SCYTHER(123, "Scyther", {Type::BUG, Type::FLYING}, 56.0f, 50.0f, {70, 110, 80, 55, 80, 105});
const Species SPECIES_JYNX(124, "Jynx", {Type::ICE, Type::PSYCHIC}, 40.6f, 0.0f, {65, 50, 35, 115, 95, 95});
const Species SPECIES_ELECTABUZZ(125, "Electabuzz", {Type::ELECTRIC, Type::NONE}, 30.0f, 75.0f, {65, 83, 57, 95, 85, 105});
const Species SPECIES_MAGMAR(126, "Magmar", {Type::FIRE, Type::NONE}, 44.5f, 75.0f, {65, 95, 57, 100, 85, 93});
const Species SPECIES_PINSIR(127, "Pinsir", {Type::BUG, Type::NONE}, 55.0f, 50.0f, {65, 125, 100, 55, 70, 85});
const Species SPECIES_TAUROS(128, "Tauros", {Type::NORMAL, Type::NONE}, 88.4f, 100.0f, {75, 100, 95, 40, 70, 110});
const Species SPECIES_MAGIKARP(129, "Magikarp", {Type::WATER, Type::NONE}, 10.0f, 50.0f, {20, 10, 55, 15, 20, 80});
const Species SPECIES_GYARADOS(130, "Gyarados", {Type::WATER, Type::FLYING}, 235.0f, 50.0f, {95, 125, 79, 60, 100, 81});
const Species SPECIES_LAPRAS(131, "Lapras", {Type::WATER, Type::ICE}, 220.0f, 50.0f, {130, 85, 80, 85, 95, 60});
const Species SPECIES_DITTO(132, "Ditto", {Type::NORMAL, Type::NONE}, 4.0f, -1.0f, {48, 48, 48, 48, 48, 48});
const Species SPECIES_EEVEE(133, "Eevee", {Type::NORMAL, Type::NONE}, 6.5f, 87.5f, {55, 55, 50, 45, 65, 55});
const Species SPECIES_VAPOREON(134, "Vaporeon", {Type::WATER, Type::NONE}, 29.0f, 87.5f, {130, 65, 60, 110, 95, 65});
const Species SPECIES_JOLTEON(135, "Jolteon", {Type::ELECTRIC, Type::NONE}, 24.5f, 87.5f, {65, 65, 60, 110, 95, 130});
const Species SPECIES_FLAREON(136, "Flareon", {Type::FIRE, Type::NONE}, 25.0f, 87.5f, {65, 130, 60, 95, 110, 65});
const Species SPECIES_PORYGON(137, "Porygon", {Type::NORMAL, Type::NONE}, 36.5f, -1.0f, {65, 60, 70, 85, 75, 40});
const Species SPECIES_OMANYTE(138, "Omanyte", {Type::ROCK, Type::WATER}, 7.5f, 87.5f, {35, 40, 100, 90, 55, 35});
const Species SPECIES_OMASTAR(139, "Omastar", {Type::ROCK, Type::WATER}, 35.0f, 87.5f, {70, 60, 125, 115, 70, 55});
const Species SPECIES_KABUTO(140, "Kabuto", {Type::ROCK, Type::WATER}, 11.5f, 87.5f, {30, 80, 90, 55, 45, 55});
const Species SPECIES_KABUTOPS(141, "Kabutops", {Type::ROCK, Type::WATER}, 40.5f, 87.5f, {60, 115, 105, 65, 70, 80});
const Species SPECIES_AERODACTYL(142, "Aerodactyl", {Type::ROCK, Type::FLYING}, 59.0f, 87.5f, {80, 105, 65, 60, 75, 130});
const Species SPECIES_SNORLAX(143, "Snorlax", {Type::NORMAL, Type::NONE}, 460.0f, 87.5f, {160, 110, 65, 65, 110, 30});
const Species SPECIES_ARTICUNO(144, "Articuno", {Type::ICE, Type::FLYING}, 55.4f, -1.0f, {90, 85, 100, 95, 125, 85});
const Species SPECIES_ZAPDOS(145, "Zapdos", {Type::ELECTRIC, Type::FLYING}, 52.6f, -1.0f, {90, 90, 85, 125, 90, 100});
const Species SPECIES_MOLTRES(146, "Moltres", {Type::FIRE, Type::FLYING}, 60.0f, -1.0f, {90, 100, 90, 125, 85, 90});
const Species SPECIES_DRATINI(147, "Dratini", {Type::DRAGON, Type::NONE}, 3.3f, 50.0f, {41, 64, 45, 50, 50, 50});
const Species SPECIES_DRAGONAIR(148, "Dragonair", {Type::DRAGON, Type::NONE}, 16.5f, 50.0f, {61, 84, 65, 70, 70, 70});
const Species SPECIES_DRAGONITE(149, "Dragonite", {Type::DRAGON, Type::FLYING}, 210.0f, 50.0f, {91, 134, 95, 100, 100, 80});
const Species SPECIES_MEWTWO(150, "Mewtwo", {Type::PSYCHIC, Type::NONE}, 122.0f, -1.0f, {106, 110, 90, 154, 90, 130});
const Species SPECIES_MEW(151, "Mew", {Type::PSYCHIC, Type::NONE}, 4.0f, -1.0f, {100, 100, 100, 100, 100, 100});
const Species SPECIES_CHIKORITA(152, "Chikorita", {Type::GRASS, Type::NONE}, 6.4f, 87.5f, {45, 49, 65, 49, 65, 45});
const Species SPECIES_BAYLEEF(153, "Bayleef", {Type::GRASS, Type::NONE}, 15.8f, 87.5f, {60, 62, 80, 63, 80, 60});
const Species SPECIES_MEGANIUM(154, "Meganium", {Type::GRASS, Type::NONE}, 100.5f, 87.5f, {80, 82, 100, 83, 100, 80});
const Species SPECIES_CYNDAQUIL(155, "Cyndaquil", {Type::FIRE, Type::NONE}, 7.9f, 87.5f, {39, 52, 43, 60, 50, 65});
const Species SPECIES_QUILAVA(156, "Quilava", {Type::FIRE, Type::NONE}, 19.0f, 87.5f, {58, 64, 58, 80, 65, 80});
const Species SPECIES_TYPHLOSION(157, "Typhlosion", {Type::FIRE, Type::NONE}, 79.5f, 87.5f, {78, 84, 78, 109, 85, 100});
const Species SPECIES_TOTODILE(158, "Totodile", {Type::WATER, Type::NONE}, 9.5f, 87.5f, {50, 65, 64, 44, 48, 43});
const Species SPECIES_CROCONAW(159, "Croconaw", {Type::WATER, Type::NONE}, 25.0f, 87.5f, {65, 80, 80, 59, 63, 58});
const Species SPECIES_FERALIGATR(160, "Feraligatr", {Type::WATER, Type::NONE}, 88.8f, 87.5f, {85, 105, 100, 79, 83, 78});
const Species SPECIES_SENTRET(161, "Sentret", {Type::NORMAL, Type::NONE}, 6.0f, 50.0f, {35, 46, 34, 35, 45, 20});
const Species SPECIES_FURRET(162, "Furret", {Type::NORMAL, Type::NONE}, 32.5f, 50.0f, {85, 76, 64, 45, 55, 90});
const Species SPECIES_HOOTHOOT(163, "Hoothoot", {Type::NORMAL, Type::FLYING}, 21.2f, 50.0f, {60, 30, 30, 36, 56, 50});
const Species SPECIES_NOCTOWL(164, "Noctowl", {Type::NORMAL, Type::FLYING}, 40.8f, 50.0f, {100, 50, 50, 76, 96, 70});
const Species SPECIES_LEDYBA(165, "Ledyba", {Type::BUG, Type::FLYING}, 10.8f, 50.0f, {40, 20, 30, 40, 80, 55});
const Species SPECIES_LEDIAN(166, "Ledian", {Type::BUG, Type::FLYING}, 35.6f, 50.0f, {55, 35, 50, 55, 110, 85});
const Species SPECIES_SPINARAK(167, "Spinarak", {Type::BUG, Type::POISON}, 8.5f, 50.0f, {40, 60, 40, 40, 40, 30});
const Species SPECIES_ARIADOS(168, "Ariados", {Type::BUG, Type::POISON}, 33.5f, 50.0f, {70, 90, 70, 60, 60, 40});
const Species SPECIES_CROBAT(169, "Crobat", {Type::POISON, Type::FLYING}, 75.0f, 50.0f, {85, 90, 80, 70, 80, 130});
const Species SPECIES_CHINCHOU(170, "Chinchou", {Type::WATER, Type::ELECTRIC}, 12.0f, 50.0f, {75, 38, 38, 56, 56, 67});
const Species SPECIES_LANTURN(171, "Lanturn", {Type::WATER, Type::ELECTRIC}, 22.5f, 50.0f, {125, 58, 58, 76, 76, 67});
const Species SPECIES_PICHU(172, "Pichu", {Type::ELECTRIC, Type::NONE}, 2.0f, 50.0f, {20, 40, 15, 35, 35, 60});
const Species SPECIES_CLEFFA(173, "Cleffa", {Type::FAIRY, Type::NONE}, 3.0f, 25.0f, {50, 25, 28, 45, 55, 15});
const Species SPECIES_IGGLYBUFF(174, "Igglybuff", {Type::NORMAL, Type::FAIRY}, 1.0f, 25.0f, {90, 30, 15, 40, 20, 15});
const Species SPECIES_TOGEPI(175, "Togepi", {Type::FAIRY, Type::NONE}, 1.5f, 87.5f, {35, 20, 65, 40, 65, 20});
const Species SPECIES_TOGETIC(176, "Togetic", {Type::FAIRY, Type::FLYING}, 3.2f, 87.5f, {55, 40, 85, 80, 105, 40});
const Species SPECIES_NATU(177, "Natu", {Type::PSYCHIC, Type::FLYING}, 2.0f, 50.0f, {40, 50, 45, 70, 45, 70});
const Species SPECIES_XATU(178, "Xatu", {Type::PSYCHIC, Type::FLYING}, 15.0f, 50.0f, {65, 75, 70, 95, 70, 95});
const Species SPECIES_MAREEP(179, "Mareep", {Type::ELECTRIC, Type::NONE}, 7.8f, 50.0f, {55, 40, 40, 65, 45, 35});
const Species SPECIES_FLAAFFY(180, "Flaaffy", {Type::ELECTRIC, Type::NONE}, 13.3f, 50.0f, {70, 55, 55, 80, 60, 45});
const Species SPECIES_AMPHAROS(181, "Ampharos", {Type::ELECTRIC, Type::NONE}, 61.5f, 50.0f, {90, 75, 75, 115, 90, 55});
const Species SPECIES_BELLOSSOM(182, "Bellossom", {Type::GRASS, Type::NONE}, 5.8f, 50.0f, {75, 80, 85, 90, 100, 50});
const Species SPECIES_MARILL(183, "Marill", {Type::WATER, Type::FAIRY}, 8.5f, 50.0f, {70, 20, 50, 20, 50, 40});
const Species SPECIES_AZUMARILL(184, "Azumarill", {Type::WATER, Type::FAIRY}, 28.5f, 50.0f, {100, 50, 80, 50, 80, 50});
const Species SPECIES_SUDOWOODO(185, "Sudowoodo", {Type::ROCK, Type::NONE}, 38.0f, 50.0f, {70, 100, 115, 30, 65, 30});
const Species SPECIES_POLITOED(186, "Politoed", {Type::WATER, Type::NONE}, 33.9f, 50.0f, {90, 75, 75, 90, 100, 70});
const Species SPECIES_HOPPIP(187, "Hoppip", {Type::GRASS, Type::FLYING}, 0.5f, 50.0f, {35, 35, 40, 35, 55, 50});
const Species SPECIES_SKIPLOOM(188, "Skiploom", {Type::GRASS, Type::FLYING}, 1.0f, 50.0f, {55, 45, 50, 45, 65, 80});
const Species SPECIES_JUMPLUFF(189, "Jumpluff", {Type::GRASS, Type::FLYING}, 3.0f, 50.0f, {75, 55, 70, 55, 85, 110});
const Species SPECIES_AIPOM(190, "Aipom", {Type::NORMAL, Type::NONE}, 11.5f, 50.0f, {55, 70, 55, 40, 55, 85});
const Species SPECIES_SUNKERN(191, "Sunkern", {Type::GRASS, Type::NONE}, 1.8f, 50.0f, {30, 30, 30, 30, 30, 30});
const Species SPECIES_SUNFLORA(192, "Sunflora", {Type::GRASS, Type::NONE}, 8.5f, 50.0f, {75, 75, 55, 105, 85, 30});
const Species SPECIES_YANMA(193, "Yanma", {Type::BUG, Type::FLYING}, 38.0f, 50.0f, {65, 65, 45, 75, 45, 95});
const Species SPECIES_WOOPER(194, "Wooper", {Type::WATER, Type::GROUND}, 8.5f, 50.0f, {55, 45, 45, 25, 25, 15});
const Species SPECIES_QUAGSIRE(195, "Quagsire", {Type::WATER, Type::GROUND}, 75.0f, 50.0f, {95, 85, 85, 65, 65, 35});
const Species SPECIES_ESPEON(196, "Espeon", {Type::PSYCHIC, Type::NONE}, 26.5f, 87.5f, {65, 65, 60, 130, 95, 110});
const Species SPECIES_UMBREON(197, "Umbreon", {Type::DARK, Type::NONE}, 27.0f, 87.5f, {95, 65, 110, 60, 130, 65});
const Species SPECIES_MURKROW(198, "Murkrow", {Type::DARK, Type::FLYING}, 2.1f, 50.0f, {60, 85, 42, 85, 42, 91});
const Species SPECIES_SLOWKING(199, "Slowking", {Type::WATER, Type::PSYCHIC}, 79.5f, 50.0f, {95, 75, 80, 100, 110, 30});
const Species SPECIES_MISDREAVUS(200, "Misdreavus", {Type::GHOST, Type::NONE}, 1.0f, 50.0f, {60, 60, 60, 85, 85, 85});
const Species SPECIES_UNOWN(201, "Unown", {Type::PSYCHIC, Type::NONE}, 5.0f, -1.0f, {48, 72, 48, 72, 48, 48});
const Species SPECIES_WOBBUFFET(202, "Wobbuffet", {Type::PSYCHIC, Type::NONE}, 28.5f, 50.0f, {190, 33, 58, 33, 58, 33});
const Species SPECIES_GIRAFARIG(203, "Girafarig", {Type::NORMAL, Type::PSYCHIC}, 41.5f, 50.0f, {70, 80, 65, 90, 65, 85});
const Species SPECIES_PINECO(204, "Pineco", {Type::BUG, Type::NONE}, 7.2f, 50.0f, {50, 65, 90, 35, 35, 15});
const Species SPECIES_FORRETRESS(205, "Forretress", {Type::BUG, Type::STEEL}, 125.8f, 50.0f, {75, 90, 140, 60, 60, 40});
const Species SPECIES_DUNSPARCE(206, "Dunsparce", {Type::NORMAL, Type::NONE}, 14.0f, 50.0f, {100, 70, 70, 65, 65, 45});
const Species SPECIES_GLIGAR(207, "Gligar", {Type::GROUND, Type::FLYING}, 64.8f, 50.0f, {65, 75, 105, 35, 65, 85});
const Species SPECIES_STEELIX(208, "Steelix", {Type::STEEL, Type::GROUND}, 400.0f, 50.0f, {75, 85, 200, 55, 65, 30});
const Species SPECIES_SNUBBULL(209, "Snubbull", {Type::FAIRY, Type::NONE}, 7.8f, 25.0f, {60, 80, 50, 40, 40, 30});
const Species SPECIES_GRANBULL(210, "Granbull", {Type::FAIRY, Type::NONE}, 48.7f, 25.0f, {90, 120, 75, 60, 60, 45});
const Species SPECIES_QWILFISH(211, "Qwilfish", {Type::WATER, Type::POISON}, 3.9f, 50.0f, {65, 95, 75, 55, 55, 85});
const Species SPECIES_SCIZOR(212, "Scizor", {Type::BUG, Type::STEEL}, 118.0f, 50.0f, {70, 130, 100, 55, 80, 65});
const Species SPECIES_SHUCKLE(213, "Shuckle", {Type::BUG, Type::ROCK}, 20.5f, 50.0f, {20, 10, 230, 10, 230, 5});
const Species SPECIES_HERACROSS(214, "Heracross", {Type::BUG, Type::FIGHTING}, 54.0f, 50.0f, {80, 125, 75, 40, 95, 85});
const Species SPECIES_SNEASEL(215, "Sneasel", {Type::DARK, Type::ICE}, 28.0f, 50.0f, {55, 95, 55, 35, 75, 115});
const Species SPECIES_TEDDIURSA(216, "Teddiursa", {Type::NORMAL, Type::NONE}, 8.8f, 50.0f, {60, 80, 50, 50, 50, 40});
const Species SPECIES_URSARING(217, "Ursaring", {Type::NORMAL, Type::NONE}, 125.8f, 50.0f, {90, 130, 75, 75, 75, 55});
const Species SPECIES_SLUGMA(218, "Slugma", {Type::FIRE, Type::NONE}, 35.0f, 50.0f, {40, 40, 40, 70, 40, 20});
const Species SPECIES_MAGCARGO(219, "Magcargo", {Type::FIRE, Type::ROCK}, 55.0f, 50.0f, {50, 50, 120, 80, 80, 30});
const Species SPECIES_SWINUB(220, "Swinub", {Type::ICE, Type::GROUND}, 6.5f, 50.0f, {50, 50, 40, 30, 30, 50});
const Species SPECIES_PILOSWINE(221, "Piloswine", {Type::ICE, Type::GROUND}, 55.8f, 50.0f, {100, 100, 80, 60, 60, 50});
const Species SPECIES_CORSOLA(222, "Corsola", {Type::WATER, Type::ROCK}, 5.0f, 25.0f, {55, 55, 85, 65, 85, 35});
const Species SPECIES_REMORAID(223, "Remoraid", {Type::WATER, Type::NONE}, 12.0f, 50.0f, {35, 65, 35, 65, 35, 65});
const Species SPECIES_OCTILLERY(224, "Octillery", {Type::WATER, Type::NONE}, 28.5f, 50.0f, {75, 105, 75, 105, 75, 45});
const Species SPECIES_DELIBIRD(225, "Delibird", {Type::ICE, Type::FLYING}, 16.0f, 50.0f, {45, 55, 45, 65, 45, 75});
const Species SPECIES_MANTINE(226, "Mantine", {Type::WATER, Type::FLYING}, 220.0f, 50.0f, {65, 40, 70, 80, 140, 70});
const Species SPECIES_SKARMORY(227, "Skarmory", {Type::STEEL, Type::FLYING}, 50.5f, 50.0f, {65, 80, 140, 40, 70, 70});
const Species SPECIES_HOUNDOUR(228, "Houndour", {Type::DARK, Type::FIRE}, 10.8f, 50.0f, {45, 60, 30, 80, 50, 65});
const Species SPECIES_HOUNDOOM(229, "Houndoom", {Type::DARK, Type::FIRE}, 35.0f, 50.0f, {75, 90, 50, 110, 80, 95});
const Species SPECIES_KINGDRA(230, "Kingdra", {Type::WATER, Type::DRAGON}, 152.0f, 50.0f, {75, 95, 95, 95, 95, 85});
const Species SPECIES_PHANPY(231, "Phanpy", {Type::GROUND, Type::NONE}, 33.5f, 50.0f, {90, 60, 60, 40, 40, 40});
const Species SPECIES_DONPHAN(232, "Donphan", {Type::GROUND, Type::NONE}, 120.0f, 50.0f, {90, 120, 120, 60, 60, 50});
const Species SPECIES_PORYGON2(233, "Porygon2", {Type::NORMAL, Type::NONE}, 32.5f, -1.0f, {85, 80, 90, 105, 95, 60});
const Species SPECIES_STANTLER(234, "Stantler", {Type::NORMAL, Type::NONE}, 71.2f, 50.0f, {73, 95, 62, 85, 65, 85});
const Species SPECIES_SMEARGLE(235, "Smeargle", {Type::NORMAL, Type::NONE}, 58.0f, 50.0f, {55, 20, 35, 20, 45, 75});
const Species SPECIES_TYROGUE(236, "Tyrogue", {Type::FIGHTING, Type::NONE}, 21.0f, 100.0f, {35, 35, 35, 35, 35, 35});
const Species SPECIES_HITMONTOP(237, "Hitmontop", {Type::FIGHTING, Type::NONE}, 48.0f, 100.0f, {50, 95, 95, 35, 110, 70});
const Species SPECIES_SMOOCHUM(238, "Smoochum", {Type::ICE, Type::PSYCHIC}, 6.0f, 0.0f, {45, 30, 15, 85, 65, 65});
const Species SPECIES_ELEKID(239, "Elekid", {Type::ELECTRIC, Type::NONE}, 23.5f, 75.0f, {45, 63, 37, 65, 55, 95});
const Species SPECIES_MAGBY(240, "Magby", {Type::FIRE, Type::NONE}, 21.4f, 75.0f, {45, 75, 37, 70, 55, 83});
const Species SPECIES_MILTANK(241, "Miltank", {Type::NORMAL, Type::NONE}, 75.5f, 0.0f, {95, 80, 105, 40, 70, 100});
const Species SPECIES_BLISSEY(242, "Blissey", {Type::NORMAL, Type::NONE}, 46.8f, 0.0f, {255, 10, 10, 75, 135, 55});
const Species SPECIES_RAIKOU(243, "Raikou", {Type::ELECTRIC, Type::NONE}, 178.0f, -1.0f, {90, 85, 75, 115, 100, 115});
const Species SPECIES_ENTEI(244, "Entei", {Type::FIRE, Type::NONE}, 198.0f, -1.0f, {115, 115, 85, 90, 75, 100});
const Species SPECIES_SUICUNE(245, "Suicune", {Type::WATER, Type::NONE}, 187.0f, -1.0f, {100, 75, 115, 90, 115, 85});
const Species SPECIES_LARVITAR(246, "Larvitar", {Type::ROCK, Type::GROUND}, 72.0f, 50.0f, {50, 64, 50, 45, 50, 41});
const Species SPECIES_PUPITAR(247, "Pupitar", {Type::ROCK, Type::GROUND}, 152.0f, 50.0f, {70, 84, 70, 65, 70, 51});
const Species SPECIES_TYRANITAR(248, "Tyranitar", {Type::ROCK, Type::DARK}, 202.0f, 50.0f, {100, 134, 110, 95, 100, 61});
const Species SPECIES_LUGIA(249, "Lugia", {Type::PSYCHIC, Type::FLYING}, 216.0f, -1.0f, {106, 90, 130, 90, 154, 110});
const Species SPECIES_HO_OH(250, "Ho-Oh", {Type::FIRE, Type::FLYING}, 199.0f, -1.0f, {106, 130, 90, 110, 154, 90});
const Species SPECIES_CELEBI(251, "Celebi", {Type::PSYCHIC, Type::GRASS}, 5.0f, -1.0f, {100, 100, 100, 100, 100, 100});
const Species SPECIES_TREECKO(252, "Treecko", {Type::GRASS, Type::NONE}, 5.0f, 87.5f, {40, 45, 35, 65, 55, 70});
const Species SPECIES_GROVYLE(253, "Grovyle", {Type::GRASS, Type::NONE}, 21.6f, 87.5f, {50, 65, 45, 85, 65, 95});
const Species SPECIES_SCEPTILE(254, "Sceptile", {Type::GRASS, Type::NONE}, 52.2f, 87.5f, {70, 85, 65, 105, 85, 120});
const Species SPECIES_TORCHIC(255, "Torchic", {Type::FIRE, Type::NONE}, 2.5f, 87.5f, {45, 60, 40, 70, 50, 45});
const Species SPECIES_COMBUSKEN(256, "Combusken", {Type::FIRE, Type::FIGHTING}, 19.5f, 87.5f, {60, 85, 60, 85, 60, 55});
const Species SPECIES_BLAZIKEN(257, "Blaziken", {Type::FIRE, Type::FIGHTING}, 52.0f, 87.5f, {80, 120, 70, 110, 70, 80});
const Species SPECIES_MUDKIP(258, "Mudkip", {Type::WATER, Type::NONE}, 7.6f, 87.5f, {50, 70, 50, 50, 50, 40});
const Species SPECIES_MARSHTOMP(259, "Marshtomp", {Type::WATER, Type::GROUND}, 28.0f, 87.5f, {70, 85, 70, 60, 70, 50});
const Species SPECIES_SWAMPERT(260, "Swampert", {Type::WATER, Type::GROUND}, 81.9f, 87.5f, {100, 110, 90, 85, 90, 60});
const Species SPECIES_POOCHYENA(261, "Poochyena", {Type::DARK, Type::NONE}, 13.6f, 50.0f, {35, 55, 35, 30, 30, 35});
const Species SPECIES_MIGHTYENA(262, "Mightyena", {Type::DARK, Type::NONE}, 37.0f, 50.0f, {70, 90, 70, 60, 60, 70});
const Species SPECIES_ZIGZAGOON(263, "Zigzagoon", {Type::NORMAL, Type::NONE}, 17.5f, 50.0f, {38, 30, 41, 30, 41, 60});
const Species SPECIES_LINOONE(264, "Linoone", {Type::NORMAL, Type::NONE}, 32.5f, 50.0f, {78, 70, 61, 50, 61, 100});
const Species SPECIES_WURMPLE(265, "Wurmple", {Type::BUG, Type::NONE}, 3.6f, 50.0f, {45, 45, 35, 20, 30, 20});
const Species SPECIES_SILCOON(266, "Silcoon", {Type::BUG, Type::NONE}, 10.0f, 50.0f, {50, 35, 55, 25, 25, 15});
const Species SPECIES_BEAUTIFLY(267, "Beautifly", {Type::BUG, Type::FLYING}, 28.4f, 50.0f, {60, 70, 50, 90, 50, 65});
const Species SPECIES_CASCOON(268, "Cascoon", {Type::BUG, Type::NONE}, 11.5f, 50.0f, {50, 35, 55, 25, 25, 15});
const Species SPECIES_DUSTOX(269, "Dustox", {Type::BUG, Type::POISON}, 31.6f, 50.0f, {60, 50, 70, 50, 90, 65});
const Species SPECIES_LOTAD(270, "Lotad", {Type::WATER, Type::GRASS}, 2.6f, 50.0f, {40, 30, 30, 40, 50, 30});
const Species SPECIES_LOMBRE(271, "Lombre", {Type::WATER, Type::GRASS}, 32.5f, 50.0f, {60, 50, 50, 60, 70, 50});
const Species SPECIES_LUDICOLO(272, "Ludicolo", {Type::WATER, Type::GRASS}, 55.0f, 50.0f, {80, 70, 70, 90, 100, 70});
const Species SPECIES_SEEDOT(273, "Seedot", {Type::GRASS, Type::NONE}, 4.0f, 50.0f, {40, 40, 50, 30, 30, 30});
const Species SPECIES_NUZLEAF(274, "Nuzleaf", {Type::GRASS, Type::DARK}, 28.0f, 50.0f, {70, 70, 40, 60, 40, 60});
const Species SPECIES_SHIFTRY(275, "Shiftry", {Type::GRASS, Type::DARK}, 59.6f, 50.0f, {90, 100, 60, 90, 60, 80});
const Species SPECIES_TAILLOW(276, "Taillow", {Type::NORMAL, Type::FLYING}, 2.3f, 50.0f, {40, 55, 30, 30, 30, 85});
const Species SPECIES_SWELLOW(277, "Swellow", {Type::NORMAL, Type::FLYING}, 19.8f, 50.0f, {60, 85, 60, 50, 50, 125});
const Species SPECIES_WINGULL(278, "Wingull", {Type::WATER, Type::FLYING}, 9.5f, 50.0f, {40, 30, 30, 55, 30, 85});
const Species SPECIES_PELIPPER(279, "Pelipper", {Type::WATER, Type::FLYING}, 28.0f, 50.0f, {60, 50, 100, 85, 70, 65});
const Species SPECIES_RALTS(280, "Ralts", {Type::PSYCHIC, Type::FAIRY}, 6.6f, 50.0f, {28, 25, 25, 45, 35, 40});
const Species SPECIES_KIRLIA(281, "Kirlia", {Type::PSYCHIC, Type::FAIRY}, 20.2f, 50.0f, {38, 35, 35, 65, 55, 50});
const Species SPECIES_GARDEVOIR(282, "Gardevoir", {Type::PSYCHIC, Type::FAIRY}, 48.4f, 50.0f, {68, 65, 65, 125, 115, 80});
const Species SPECIES_SURSKIT(283, "Surskit", {Type::BUG, Type::WATER}, 1.7f, 50.0f, {40, 30, 32, 50, 52, 65});
const Species SPECIES_MASQUERAIN(284, "Masquerain", {Type::BUG, Type::FLYING}, 3.6f, 50.0f, {70, 60, 62, 80, 82, 60});
const Species SPECIES_SHROOMISH(285, "Shroomish", {Type::GRASS, Type::NONE}, 4.5f, 50.0f, {60, 40, 60, 40, 60, 35});
const Species SPECIES_BRELOOM(286, "Breloom", {Type::GRASS, Type::FIGHTING}, 39.2f, 50.0f, {60, 130, 80, 60, 60, 70});
const Species SPECIES_SLAKOTH(287, "Slakoth", {Type::NORMAL, Type::NONE}, 24.0f, 50.0f, {60, 60, 60, 35, 35, 30});
const Species SPECIES_VIGOROTH(288, "Vigoroth", {Type::NORMAL, Type::NONE}, 46.5f, 50.0f, {80, 80, 80, 55, 55, 90});
const Species SPECIES_SLAKING(289, "Slaking", {Type::NORMAL, Type::NONE}, 130.5f, 50.0f, {150, 160, 100, 95, 65, 100});
const Species SPECIES_NINCADA(290, "Nincada", {Type::BUG, Type::GROUND}, 5.5f, 50.0f, {31, 45, 90, 30, 30, 40});
const Species SPECIES_NINJASK(291, "Ninjask", {Type::BUG, Type::FLYING}, 12.0f, 50.0f, {61, 90, 45, 50, 50, 160});
const Species SPECIES_SHEDINJA(292, "Shedinja", {Type::BUG, Type::GHOST}, 1.2f, -1.0f, {1, 90, 45, 30, 30, 40});
const Species SPECIES_WHISMUR(293, "Whismur", {Type::NORMAL, Type::NONE}, 16.3f, 50.0f, {64, 51, 23, 51, 23, 28});
const Species SPECIES_LOUDRED(294, "Loudred", {Type::NORMAL, Type::NONE}, 40.5f, 50.0f, {84, 71, 43, 71, 43, 48});
const Species SPECIES_EXPLOUD(295, "Exploud", {Type::NORMAL, Type::NONE}, 84.0f, 50.0f, {104, 91, 63, 91, 63, 68});
const Species SPECIES_MAKUHITA(296, "Makuhita", {Type::FIGHTING, Type::NONE}, 86.4f, 75.0f, {72, 60, 30, 20, 30, 25});
const Species SPECIES_HARIYAMA(297, "Hariyama", {Type::FIGHTING, Type::NONE}, 253.8f, 75.0f, {144, 120, 60, 40, 60, 50});
const Species SPECIES_AZURILL(298, "Azurill", {Type::NORMAL, Type::FAIRY}, 2.0f, 25.0f, {50, 20, 40, 20, 40, 20});
const Species SPECIES_NOSEPASS(299, "Nosepass", {Type::ROCK, Type::NONE}, 97.0f, 50.0f, {30, 45, 135, 45, 90, 30});
const Species SPECIES_SKITTY(300, "Skitty", {Type::NORMAL, Type::NONE}, 11.0f, 25.0f, {50, 45, 45, 35, 35, 50});
const Species SPECIES_DELCATTY(301, "Delcatty", {Type::NORMAL, Type::NONE}, 32.6f, 25.0f, {70, 65, 65, 55, 55, 70});
const Species SPECIES_SABLEYE(302, "Sableye", {Type::DARK, Type::GHOST}, 11.0f, 50.0f, {50, 75, 75, 65, 65, 50});
const Species SPECIES_MAWILE(303, "Mawile", {Type::STEEL, Type::FAIRY}, 11.5f, 50.0f, {50, 85, 85, 55, 55, 50});
const Species SPECIES_ARON(304, "Aron", {Type::STEEL, Type::ROCK}, 60.0f, 50.0f, {50, 70, 100, 40, 40, 30});
const Species SPECIES_LAIRON(305, "Lairon", {Type::STEEL, Type::ROCK}, 120.0f, 50.0f, {60, 90, 140, 50, 50, 40});
const Species SPECIES_AGGRON(306, "Aggron", {Type::STEEL, Type::ROCK}, 360.0f, 50.0f, {70, 110, 180, 60, 60, 50});
const Species SPECIES_MEDITITE(307, "Meditite", {Type::FIGHTING, Type::PSYCHIC}, 11.2f, 50.0f, {30, 40, 55, 40, 55, 60});
const Species SPECIES_MEDICHAM(308, "Medicham", {Type::FIGHTING, Type::PSYCHIC}, 31.5f, 50.0f, {60, 60, 75, 60, 75, 80});
const Species SPECIES_ELECTRIKE(309, "Electrike", {Type::ELECTRIC, Type::NONE}, 15.2f, 50.0f, {40, 45, 40, 65, 40, 65});
const Species SPECIES_MANECTRIC(310, "Manectric", {Type::ELECTRIC, Type::NONE}, 40.2f, 50.0f, {70, 75, 60, 105, 60, 105});
const Species SPECIES_PLUSLE(311, "Plusle", {Type::ELECTRIC, Type::NONE}, 4.2f, 50.0f, {60, 50, 40, 85, 75, 95});
const Species SPECIES_MINUN(312, "Minun", {Type::ELECTRIC, Type::NONE}, 4.2f, 50.0f, {60, 40, 50, 75, 85, 95});
const Species SPECIES_VOLBEAT(313, "Volbeat", {Type::BUG, Type::NONE}, 17.7f, 100.0f, {65, 73, 55, 47, 75, 85});
const Species SPECIES_ILLUMISE(314, "Illumise", {Type::BUG, Type::NONE}, 17.7f, 0.0f, {65, 47, 55, 73, 75, 85});
const Species SPECIES_ROSELIA(315, "Roselia", {Type::GRASS, Type::POISON}, 2.0f, 50.0f, {50, 60, 45, 100, 80, 65});
const Species SPECIES_GULPIN(316, "Gulpin", {Type::POISON, Type::NONE}, 10.3f, 50.0f, {70, 43, 53, 43, 53, 40});
const Species SPECIES_SWALOT(317, "Swalot", {Type::POISON, Type::NONE}, 80.0f, 50.0f, {100, 73, 83, 73, 83, 55});
const Species SPECIES_CARVANHA(318, "Carvanha", {Type::WATER, Type::DARK}, 20.8f, 50.0f, {45, 90, 20, 65, 20, 65});
const Species SPECIES_SHARPEDO(319, "Sharpedo", {Type::WATER, Type::DARK}, 88.8f, 50.0f, {70, 120, 40, 95, 40, 95});
const Species SPECIES_WAILMER(320, "Wailmer", {Type::WATER, Type::NONE}, 130.0f, 50.0f, {130, 70, 35, 70, 35, 60});
const Species SPECIES_WAILORD(321, "Wailord", {Type::WATER, Type::NONE}, 398.0f, 50.0f, {170, 90, 45, 90, 45, 60});
const Species SPECIES_NUMEL(322, "Numel", {Type::FIRE, Type::GROUND}, 24.0f, 50.0f, {60, 60, 40, 65, 45, 35});
const Species SPECIES_CAMERUPT(323, "Camerupt", {Type::FIRE, Type::GROUND}, 220.0f, 50.0f, {70, 100, 70, 105, 75, 40});
const Species SPECIES_TORKOAL(324, "Torkoal", {Type::FIRE, Type::NONE}, 80.4f, 50.0f, {70, 85, 140, 85, 70, 20});
const Species SPECIES_SPOINK(325, "Spoink", {Type::PSYCHIC, Type::NONE}, 30.6f, 50.0f, {60, 25, 35, 70, 80, 60});
const Species SPECIES_GRUMPIG(326, "Grumpig", {Type::PSYCHIC, Type::NONE}, 71.5f, 50.0f, {80, 45, 65, 90, 110, 80});
const Species SPECIES_SPINDA(327, "Spinda", {Type::NORMAL, Type::NONE}, 5.0f, 50.0f, {60, 60, 60, 60, 60, 60});
const Species SPECIES_TRAPINCH(328, "Trapinch", {Type::GROUND, Type::NONE}, 15.0f, 50.0f, {45, 100, 45, 45, 45, 10});
const Species SPECIES_VIBRAVA(329, "Vibrava", {Type::GROUND, Type::DRAGON}, 15.3f, 50.0f, {50, 70, 50, 50, 50, 70});
const Species SPECIES_FLYGON(330, "Flygon", {Type::GROUND, Type::DRAGON}, 82.0f, 50.0f, {80, 100, 80, 80, 80, 100});
const Species SPECIES_CACNEA(331, "Cacnea", {Type::GRASS, Type::NONE}, 51.3f, 50.0f, {50, 85, 40, 85, 40, 35});
const Species SPECIES_CACTURNE(332, "Cacturne", {Type::GRASS, Type::DARK}, 77.4f, 50.0f, {70, 115, 60, 115, 60, 55});
const Species SPECIES_SWABLU(333, "Swablu", {Type::NORMAL, Type::FLYING}, 1.2f, 50.0f, {45, 40, 60, 40, 75, 50});
const Species SPECIES_ALTARIA(334, "Altaria", {Type::DRAGON, Type::FLYING}, 20.6f, 50.0f, {75, 70, 90, 70, 105, 80});
const Species SPECIES_ZANGOOSE(335, "Zangoose", {Type::NORMAL, Type::NONE}, 40.3f, 50.0f, {73, 115, 60, 60, 60, 90});
const Species SPECIES_SEVIPER(336, "Seviper", {Type::POISON, Type::NONE}, 52.5f, 50.0f, {73, 100, 60, 100, 60, 65});
const Species SPECIES_LUNATONE(337, "Lunatone", {Type::ROCK, Type::PSYCHIC}, 168.0f, -1.0f, {70, 55, 65, 95, 85, 70});
const Species SPECIES_SOLROCK(338, "Solrock", {Type::ROCK, Type::PSYCHIC}, 154.0f, -1.0f, {70, 95, 85, 55, 65, 70});
const Species SPECIES_BARBOACH(339, "Barboach", {Type::WATER, Type::GROUND}, 1.9f, 50.0f, {50, 48, 43, 46, 41, 60});
const Species SPECIES_WHISCASH(340, "Whiscash", {Type::WATER, Type::GROUND}, 23.6f, 50.0f, {110, 78, 73, 76, 71, 60});
const Species SPECIES_CORPHISH(341, "Corphish", {Type::WATER, Type::NONE}, 11.5f, 50.0f, {43, 80, 65, 50, 35, 35});
const Species SPECIES_CRAWDAUNT(342, "Crawdaunt", {Type::WATER, Type::DARK}, 32.8f, 50.0f, {63, 120, 85, 90, 55, 55});
const Species SPECIES_BALTOY(343, "Baltoy", {Type::GROUND, Type::PSYCHIC}, 21.5f, -1.0f, {40, 40, 55, 40, 70, 55});
const Species SPECIES_CLAYDOL(344, "Claydol", {Type::GROUND, Type::PSYCHIC}, 108.0f, -1.0f, {60, 70, 105, 70, 120, 75});
const Species SPECIES_LILEEP(345, "Lileep", {Type::ROCK, Type::GRASS}, 23.8f, 87.5f, {66, 41, 77, 61, 87, 23});
const Species SPECIES_CRADILY(346, "Cradily", {Type::ROCK, Type::GRASS}, 60.4f, 87.5f, {86, 81, 97, 81, 107, 43});
const Species SPECIES_ANORITH(347, "Anorith", {Type::ROCK, Type::BUG}, 12.5f, 87.5f, {45, 95, 50, 40, 50, 75});
const Species SPECIES_ARMALDO(348, "Armaldo", {Type::ROCK, Type::BUG}, 68.2f, 87.5f, {75, 125, 100, 70, 80, 45});
const Species SPECIES_FEEBAS(349, "Feebas", {Type::WATER, Type::NONE}, 7.4f, 50.0f, {20, 15, 20, 10, 55, 80});
const Species SPECIES_MILOTIC(350, "Milotic", {Type::WATER, Type::NONE}, 162.0f, 50.0f, {95, 60, 79, 100, 125, 81});
const Species SPECIES_CASTFORM(351, "Castform", {Type::NORMAL, Type::NONE}, 0.8f, 50.0f, {70, 70, 70, 70, 70, 70});
const Species SPECIES_KECLEON(352, "Kecleon", {Type::NORMAL, Type::NONE}, 22.0f, 50.0f, {60, 90, 70, 60, 120, 40});
const Species SPECIES_SHUPPET(353, "Shuppet", {Type::GHOST, Type::NONE}, 2.3f, 50.0f, {44, 75, 35, 63, 33, 45});
const Species SPECIES_BANETTE(354, "Banette", {Type::GHOST, Type::NONE}, 12.5f, 50.0f, {64, 115, 65, 83, 63, 65});
const Species SPECIES_DUSKULL(355, "Duskull", {Type::GHOST, Type::NONE}, 15.0f, 50.0f, {20, 40, 90, 30, 90, 25});
const Species SPECIES_DUSCLOPS(356, "Dusclops", {Type::GHOST, Type::NONE}, 30.6f, 50.0f, {40, 70, 130, 60, 130, 25});
const Species SPECIES_TROPIUS(357, "Tropius", {Type::GRASS, Type::FLYING}, 100.0f, 50.0f, {99, 68, 83, 72, 87, 51});
const Species SPECIES_CHIMECHO(358, "Chimecho", {Type::PSYCHIC, Type::NONE}, 1.0f, 50.0f, {65, 50, 70, 95, 80, 65});
const Species SPECIES_ABSOL(359, "Absol", {Type::DARK, Type::NONE}, 47.0f, 50.0f, {65, 130, 60, 75, 60, 75});
const Species SPECIES_WYNAUT(360, "Wynaut", {Type::PSYCHIC, Type::NONE}, 14.0f, 50.0f, {95, 23, 48, 23, 48, 23});
const Species SPECIES_SNORUNT(361, "Snorunt", {Type::ICE, Type::NONE}, 16.8f, 50.0f, {50, 50, 50, 50, 50, 50});
const Species SPECIES_GLALIE(362, "Glalie", {Type::ICE, Type::NONE}, 256.5f, 50.0f, {80, 80, 80, 80, 80, 80});
const Species SPECIES_SPHEAL(363, "Spheal", {Type::ICE, Type::WATER}, 39.5f, 50.0f, {70, 40, 50, 55, 50, 25});
const Species SPECIES_SEALEO(364, "Sealeo", {Type::ICE, Type::WATER}, 87.6f, 50.0f, {90, 60, 70, 75, 70, 45});
const Species SPECIES_WALREIN(365, "Walrein", {Type::ICE, Type::WATER}, 150.6f, 50.0f, {110, 80, 90, 95, 90, 65});
const Species SPECIES_CLAMPERL(366, "Clamperl", {Type::WATER, Type::NONE}, 52.5f, 50.0f, {35, 64, 85, 74, 55, 32});
const Species SPECIES_HUNTAIL(367, "Huntail", {Type::WATER, Type::NONE}, 27.0f, 50.0f, {55, 104, 105, 94, 75, 52});
const Species SPECIES_GOREBYSS(368, "Gorebyss", {Type::WATER, Type::NONE}, 22.6f, 50.0f, {55, 84, 105, 114, 75, 52});
const Species SPECIES_RELICANTH(369, "Relicanth", {Type::WATER, Type::ROCK}, 23.4f, 87.5f, {100, 90, 130, 45, 65, 55});
const Species SPECIES_LUVDISC(370, "Luvdisc", {Type::WATER, Type::NONE}, 8.7f, 25.0f, {43, 30, 55, 40, 65, 97});
const Species SPECIES_BAGON(371, "Bagon", {Type::DRAGON, Type::NONE}, 42.1f, 50.0f, {45, 75, 60, 40, 30, 50});
const Species SPECIES_SHELGON(372, "Shelgon", {Type::DRAGON, Type::NONE}, 110.5f, 50.0f, {65, 95, 100, 60, 50, 50});
const Species SPECIES_SALAMENCE(373, "Salamence", {Type::DRAGON, Type::FLYING}, 102.6f, 50.0f, {95, 135, 80, 110, 80, 100});
const Species SPECIES_BELDUM(374, "Beldum", {Type::STEEL, Type::PSYCHIC}, 95.2f, -1.0f, {40, 55, 80, 35, 60, 30});
const Species SPECIES_METANG(375, "Metang", {Type::STEEL, Type::PSYCHIC}, 202.5f, -1.0f, {60, 75, 100, 55, 80, 50});
const Species SPECIES_METAGROSS(376, "Metagross", {Type::STEEL, Type::PSYCHIC}, 550.0f, -1.0f, {80, 135, 130, 95, 90, 70});
const Species SPECIES_REGIROCK(377, "Regirock", {Type::ROCK, Type::NONE}, 230.0f, -1.0f, {80, 100, 200, 50, 100, 50});
const Species SPECIES_REGICE(378, "Regice", {Type::ICE, Type::NONE}, 175.0f, -1.0f, {80, 50, 100, 100, 200, 50});
const Species SPECIES_REGISTEEL(379, "Registeel", {Type::STEEL, Type::NONE}, 205.0f, -1.0f, {80, 75, 150, 75, 150, 50});
const Species SPECIES_LATIAS(380, "Latias", {Type::DRAGON, Type::PSYCHIC}, 40.0f, 0.0f, {80, 80, 90, 110, 130, 110});
const Species SPECIES_LATIOS(381, "Latios", {Type::DRAGON, Type::PSYCHIC}, 60.0f, 100.0f, {80, 90, 80, 130, 110, 110});
const Species SPECIES_KYOGRE(382, "Kyogre", {Type::WATER, Type::NONE}, 352.0f, -1.0f, {100, 100, 90, 150, 140, 90});
const Species SPECIES_GROUDON(383, "Groudon", {Type::GROUND, Type::NONE}, 950.0f, -1.0f, {100, 150, 140, 100, 90, 90});
const Species SPECIES_RAYQUAZA(384, "Rayquaza", {Type::DRAGON, Type::FLYING}, 206.5f, -1.0f, {105, 150, 90, 150, 90, 95});
const Species SPECIES_JIRACHI(385, "Jirachi", {Type::STEEL, Type::PSYCHIC}, 1.1f, -1.0f, {100, 100, 100, 100, 100, 100});
const Species SPECIES_DEOXYS(386, "Deoxys", {Type::PSYCHIC, Type::NONE}, 60.8f, -1.0f, {50, 150, 50, 150, 50, 150});
const Species SPECIES_TURTWIG(387, "Turtwig", {Type::GRASS, Type::NONE}, 10.2f, 87.5f, {55, 68, 64, 45, 55, 31});
const Species SPECIES_GROTLE(388, "Grotle", {Type::GRASS, Type::NONE}, 97.0f, 87.5f, {75, 89, 85, 55, 65, 36});
const Species SPECIES_TORTERRA(389, "Torterra", {Type::GRASS, Type::GROUND}, 310.0f, 87.5f, {95, 109, 105, 75, 85, 56});
const Species SPECIES_CHIMCHAR(390, "Chimchar", {Type::FIRE, Type::NONE}, 6.2f, 87.5f, {44, 58, 44, 58, 44, 61});
const Species SPECIES_MONFERNO(391, "Monferno", {Type::FIRE, Type::FIGHTING}, 22.0f, 87.5f, {64, 78, 52, 78, 52, 81});
const Species SPECIES_INFERNAPE(392, "Infernape", {Type::FIRE, Type::FIGHTING}, 55.0f, 87.5f, {76, 104, 71, 104, 71, 108});
const Species SPECIES_PIPLUP(393, "Piplup", {Type::WATER, Type::NONE}, 5.2f, 87.5f, {53, 51, 53, 61, 56, 40});
const Species SPECIES_PRINPLUP(394, "Prinplup", {Type::WATER, Type::NONE}, 23.0f, 87.5f, {64, 66, 68, 81, 76, 50});
const Species SPECIES_EMPOLEON(395, "Empoleon", {Type::WATER, Type::STEEL}, 84.5f, 87.5f, {84, 86, 88, 111, 101, 60});
const Species SPECIES_STARLY(396, "Starly", {Type::NORMAL, Type::FLYING}, 2.0f, 50.0f, {40, 55, 30, 30, 30, 60});
const Species SPECIES_STARAVIA(397, "Staravia", {Type::NORMAL, Type::FLYING}, 15.5f, 50.0f, {55, 75, 50, 40, 40, 80});
const Species SPECIES_STARAPTOR(398, "Staraptor", {Type::NORMAL, Type::FLYING}, 24.9f, 50.0f, {85, 120, 70, 50, 50, 100});
const Species SPECIES_BIDOOF(399, "Bidoof", {Type::NORMAL, Type::NONE}, 20.0f, 50.0f, {59, 45, 40, 35, 40, 31});
const Species SPECIES_BIBAREL(400, "Bibarel", {Type::NORMAL, Type::WATER}, 31.5f, 50.0f, {79, 85, 60, 55, 60, 71});
const Species SPECIES_KRICKETOT(401, "Kricketot", {Type::BUG, Type::NONE}, 2.2f, 50.0f, {37, 25, 41, 25, 41, 25});
const Species SPECIES_KRICKETUNE(402, "Kricketune", {Type::BUG, Type::NONE}, 25.5f, 50.0f, {77, 85, 51, 55, 51, 65});
const Species SPECIES_SHINX(403, "Shinx", {Type::ELECTRIC, Type::NONE}, 9.5f, 50.0f, {45, 65, 34, 40, 34, 45});
const Species SPECIES_LUXIO(404, "Luxio", {Type::ELECTRIC, Type::NONE}, 30.5f, 50.0f, {60, 85, 49, 60, 49, 60});
const Species SPECIES_LUXRAY(405, "Luxray", {Type::ELECTRIC, Type::NONE}, 42.0f, 50.0f, {80, 120, 79, 95, 79, 70});
const Species SPECIES_BUDEW(406, "Budew", {Type::GRASS, Type::POISON}, 1.2f, 50.0f, {40, 30, 35, 50, 70, 55});
const Species SPECIES_ROSERADE(407, "Roserade", {Type::GRASS, Type::POISON}, 14.5f, 50.0f, {60, 70, 55, 125, 105, 90});
const Species SPECIES_CRANIDOS(408, "Cranidos", {Type::ROCK, Type::NONE}, 31.5f, 87.5f, {67, 125, 40, 30, 30, 58});
const Species SPECIES_RAMPARDOS(409, "Rampardos", {Type::ROCK, Type::NONE}, 102.5f, 87.5f, {97, 165, 60, 65, 50, 58});
const Species SPECIES_SHIELDON(410, "Shieldon", {Type::ROCK, Type::STEEL}, 57.0f, 87.5f, {30, 42, 118, 42, 88, 30});
const Species SPECIES_BASTIODON(411, "Bastiodon", {Type::ROCK, Type::STEEL}, 149.5f, 87.5f, {60, 52, 168, 47, 138, 30});
const Species SPECIES_BURMY(412, "Burmy", {Type::BUG, Type::NONE}, 3.4f, 50.0f, {40, 29, 45, 29, 45, 36});
const Species SPECIES_WORMADAM(413, "Wormadam", {Type::BUG, Type::GRASS}, 6.5f, 0.0f, {60, 59, 85, 79, 105, 36});
const Species SPECIES_MOTHIM(414, "Mothim", {Type::BUG, Type::FLYING}, 23.3f, 100.0f, {70, 94, 50, 94, 50, 66});
const Species SPECIES_COMBEE(415, "Combee", {Type::BUG, Type::FLYING}, 5.5f, 87.5f, {30, 30, 42, 30, 42, 70});
const Species SPECIES_VESPIQUEN(416, "Vespiquen", {Type::BUG, Type::FLYING}, 38.5f, 0.0f, {70, 80, 102, 80, 102, 40});
const Species SPECIES_PACHIRISU(417, "Pachirisu", {Type::ELECTRIC, Type::NONE}, 3.9f, 50.0f, {60, 45, 70, 45, 90, 95});
const Species SPECIES_BUIZEL(418, "Buizel", {Type::WATER, Type::NONE}, 29.5f, 50.0f, {55, 65, 35, 60, 30, 85});
const Species SPECIES_FLOATZEL(419, "Floatzel", {Type::WATER, Type::NONE}, 33.5f, 50.0f, {85, 105, 55, 85, 50, 115});
const Species SPECIES_CHERUBI(420, "Cherubi", {Type::GRASS, Type::NONE}, 3.3f, 50.0f, {45, 35, 45, 62, 53, 35});
const Species SPECIES_CHERRIM(421, "Cherrim", {Type::GRASS, Type::NONE}, 9.3f, 50.0f, {70, 60, 70, 87, 78, 85});
const Species SPECIES_SHELLOS(422, "Shellos", {Type::WATER, Type::NONE}, 6.3f, 50.0f, {76, 48, 48, 57, 62, 34});
const Species SPECIES_GASTRODON(423, "Gastrodon", {Type::WATER, Type::GROUND}, 29.9f, 50.0f, {111, 83, 68, 92, 82, 39});
const Species SPECIES_AMBIPOM(424, "Ambipom", {Type::NORMAL, Type::NONE}, 20.3f, 50.0f, {75, 100, 66, 60, 66, 115});
const Species SPECIES_DRIFLOON(425, "Drifloon", {Type::GHOST, Type::FLYING}, 1.2f, 50.0f, {90, 50, 34, 60, 44, 70});
const Species SPECIES_DRIFBLIM(426, "Drifblim", {Type::GHOST, Type::FLYING}, 15.0f, 50.0f, {150, 80, 44, 90, 54, 80});
const Species SPECIES_BUNEARY(427, "Buneary", {Type::NORMAL, Type::NONE}, 5.5f, 50.0f, {55, 66, 44, 44, 56, 85});
const Species SPECIES_LOPUNNY(428, "Lopunny", {Type::NORMAL, Type::NONE}, 33.3f, 50.0f, {65, 76, 84, 54, 96, 105});
const Species SPECIES_MISMAGIUS(429, "Mismagius", {Type::GHOST, Type::NONE}, 4.4f, 50.0f, {60, 60, 60, 105, 105, 105});
const Species SPECIES_HONCHKROW(430, "Honchkrow", {Type::DARK, Type::FLYING}, 27.3f, 50.0f, {100, 125, 52, 105, 52, 71});
const Species SPECIES_GLAMEOW(431, "Glameow", {Type::NORMAL, Type::NONE}, 3.9f, 25.0f, {49, 55, 42, 42, 37, 85});
const Species SPECIES_PURUGLY(432, "Purugly", {Type::NORMAL, Type::NONE}, 43.8f, 25.0f, {71, 82, 64, 64, 59, 112});
const Species SPECIES_CHINGLING(433, "Chingling", {Type::PSYCHIC, Type::NONE}, 0.6f, 50.0f, {45, 30, 50, 65, 50, 45});
const Species SPECIES_STUNKY(434, "Stunky", {Type::POISON, Type::DARK}, 19.2f, 50.0f, {63, 63, 47, 41, 41, 74});
const Species SPECIES_SKUNTANK(435, "Skuntank", {Type::POISON, Type::DARK}, 38.0f, 50.0f, {103, 93, 67, 71, 61, 84});
const Species SPECIES_BRONZOR(436, "Bronzor", {Type::STEEL, Type::PSYCHIC}, 60.5f, -1.0f, {57, 24, 86, 24, 86, 23});
const Species SPECIES_BRONZONG(437, "Bronzong", {Type::STEEL, Type::PSYCHIC}, 187.0f, -1.0f, {67, 89, 116, 79, 116, 33});
const Species SPECIES_BONSLY(438, "Bonsly", {Type::ROCK, Type::NONE}, 15.0f, 50.0f, {50, 80, 95, 10, 45, 10});
const Species SPECIES_MIME_JR(439, "Mime Jr.", {Type::PSYCHIC, Type::FAIRY}, 13.0f, 50.0f, {20, 25, 45, 70, 90, 60});
const Species SPECIES_HAPPINY(440, "Happiny", {Type::NORMAL, Type::NONE}, 24.4f, 0.0f, {100, 5, 5, 15, 65, 30});
const Species SPECIES_CHATOT(441, "Chatot", {Type::NORMAL, Type::FLYING}, 1.9f, 50.0f, {76, 65, 45, 92, 42, 91});
const Species SPECIES_SPIRITOMB(442, "Spiritomb", {Type::GHOST, Type::DARK}, 108.0f, 50.0f, {50, 92, 108, 92, 108, 35});
const Species SPECIES_GIBLE(443, "Gible", {Type::DRAGON, Type::GROUND}, 20.5f, 50.0f, {58, 70, 45, 40, 45, 42});
const Species SPECIES_GABITE(444, "Gabite", {Type::DRAGON, Type::GROUND}, 56.0f, 50.0f, {68, 90, 65, 50, 55, 82});
const Species SPECIES_GARCHOMP(445, "Garchomp", {Type::DRAGON, Type::GROUND}, 95.0f, 50.0f, {108, 130, 95, 80, 85, 102});
const Species SPECIES_MUNCHLAX(446, "Munchlax", {Type::NORMAL, Type::NONE}, 105.0f, 87.5f, {135, 85, 40, 40, 85, 5});
const Species SPECIES_RIOLU(447, "Riolu", {Type::FIGHTING, Type::NONE}, 20.2f, 87.5f, {40, 70, 40, 35, 40, 60});
const Species SPECIES_LUCARIO(448, "Lucario", {Type::FIGHTING, Type::STEEL}, 54.0f, 87.5f, {70, 110, 70, 115, 70, 90});
const Species SPECIES_HIPPOPOTAS(449, "Hippopotas", {Type::GROUND, Type::NONE}, 49.5f, 50.0f, {68, 72, 78, 38, 42, 32});
const Species SPECIES_HIPPOWDON(450, "Hippowdon", {Type::GROUND, Type::NONE}, 300.0f, 50.0f, {108, 112, 118, 68, 72, 47});
const Species SPECIES_SKORUPI(451, "Skorupi", {Type::POISON, Type::BUG}, 12.0f, 50.0f, {40, 50, 90, 30, 55, 65});
const Species SPECIES_DRAPION(452, "Drapion", {Type::POISON, Type::DARK}, 61.5f, 50.0f, {70, 90, 110, 60, 75, 95});
const Species SPECIES_CROAGUNK(453, "Croagunk", {Type::POISON, Type::FIGHTING}, 23.0f, 50.0f, {48, 61, 40, 61, 40, 50});
const Species SPECIES_TOXICROAK(454, "Toxicroak", {Type::POISON, Type::FIGHTING}, 44.4f, 50.0f, {83, 106, 65, 86, 65, 85});
const Species SPECIES_CARNIVINE(455, "Carnivine", {Type::GRASS, Type::NONE}, 27.0f, 50.0f, {74, 100, 72, 90, 72, 46});
const Species SPECIES_FINNEON(456, "Finneon", {Type::WATER, Type::NONE}, 7.0f, 50.0f, {49, 49, 56, 49, 61, 66});
const Species SPECIES_LUMINEON(457, "Lumineon", {Type::WATER, Type::NONE}, 24.0f, 50.0f, {69, 69, 76, 69, 86, 91});
const Species SPECIES_MANTYKE(458, "Mantyke", {Type::WATER, Type::FLYING}, 65.0f, 50.0f, {45, 20, 50, 60, 120, 50});
const Species SPECIES_SNOVER(459, "Snover", {Type::GRASS, Type::ICE}, 50.5f, 50.0f, {60, 62, 50, 62, 60, 40});
const Species SPECIES_ABOMASNOW(460, "Abomasnow", {Type::GRASS, Type::ICE}, 135.5f, 50.0f, {90, 92, 75, 92, 85, 60});
const Species SPECIES_WEAVILE(461, "Weavile", {Type::DARK, Type::ICE}, 34.0f, 50.0f, {70, 120, 65, 45, 85, 125});
const Species SPECIES_MAGNEZONE(462, "Magnezone", {Type::ELECTRIC, Type::STEEL}, 180.0f, -1.0f, {70, 70, 115, 130, 90, 60});
const Species SPECIES_LICKILICKY(463, "Lickilicky", {Type::NORMAL, Type::NONE}, 140.0f, 50.0f, {110, 85, 95, 80, 95, 50});
const Species SPECIES_RHYPERIOR(464, "Rhyperior", {Type::GROUND, Type::ROCK}, 282.8f, 50.0f, {115, 140, 130, 55, 55, 40});
const Species SPECIES_TANGROWTH(465, "Tangrowth", {Type::GRASS, Type::NONE}, 128.6f, 50.0f, {100, 100, 125, 110, 50, 50});
const Species SPECIES_ELECTIVIRE(466, "Electivire", {Type::ELECTRIC, Type::NONE}, 138.6f, 75.0f, {75, 123, 67, 95, 85, 95});
const Species SPECIES_MAGMORTAR(467, "Magmortar", {Type::FIRE, Type::NONE}, 68.0f, 75.0f, {75, 95, 67, 125, 95, 83});
const Species SPECIES_TOGEKISS(468, "Togekiss", {Type::FAIRY, Type::FLYING}, 38.0f, 87.5f, {85, 50, 95, 120, 115, 80});
const Species SPECIES_YANMEGA(469, "Yanmega", {Type::BUG, Type::FLYING}, 51.5f, 50.0f, {86, 76, 86, 116, 56, 95});
const Species SPECIES_LEAFEON(470, "Leafeon", {Type::GRASS, Type::NONE}, 25.5f, 87.5f, {65, 110, 130, 60, 65, 95});
const Species SPECIES_GLACEON(471, "Glaceon", {Type::ICE, Type::NONE}, 25.9f, 87.5f, {65, 60, 110, 130, 95, 65});
const Species SPECIES_GLISCOR(472, "Gliscor", {Type::GROUND, Type::FLYING}, 42.5f, 50.0f, {75, 95, 125, 45, 75, 95});
const Species SPECIES_MAMOSWINE(473, "Mamoswine", {Type::ICE, Type::GROUND}, 291.0f, 50.0f, {110, 130, 80, 70, 60, 80});
const Species SPECIES_PORYGON_Z(474, "Porygon-Z", {Type::NORMAL, Type::NONE}, 34.0f, -1.0f, {85, 80, 70, 135, 75, 90});
const Species SPECIES_GALLADE(475, "Gallade", {Type::PSYCHIC, Type::FIGHTING}, 52.0f, 100.0f, {68, 125, 65, 65, 115, 80});
const Species SPECIES_PROBOPASS(476, "Probopass", {Type::ROCK, Type::STEEL}, 340.0f, 50.0f, {60, 55, 145, 75, 150, 40});
const Species SPECIES_DUSKNOIR(477, "Dusknoir", {Type::GHOST, Type::NONE}, 106.6f, 50.0f, {45, 100, 135, 65, 135, 45});
const Species SPECIES_FROSLASS(478, "Froslass", {Type::ICE, Type::GHOST}, 26.6f, 0.0f, {70, 80, 70, 80, 70, 110});
const Species SPECIES_ROTOM(479, "Rotom", {Type::ELECTRIC, Type::GHOST}, 0.3f, -1.0f, {50, 50, 77, 95, 77, 91});
const Species SPECIES_UXIE(480, "Uxie", {Type::PSYCHIC, Type::NONE}, 0.3f, -1.0f, {75, 75, 130, 75, 130, 95});
const Species SPECIES_MESPRIT(481, "Mesprit", {Type::PSYCHIC, Type::NONE}, 0.3f, -1.0f, {80, 105, 105, 105, 105, 80});
const Species SPECIES_AZELF(482, "Azelf", {Type::PSYCHIC, Type::NONE}, 0.3f, -1.0f, {75, 125, 70, 125, 70, 115});
const Species SPECIES_DIALGA(483, "Dialga", {Type::STEEL, Type::DRAGON}, 683.0f, -1.0f, {100, 120, 120, 150, 100, 90});
const Species SPECIES_PALKIA(484, "Palkia", {Type::WATER, Type::DRAGON}, 336.0f, -1.0f, {90, 120, 100, 150, 120, 100});
const Species SPECIES_HEATRAN(485, "Heatran", {Type::FIRE, Type::STEEL}, 430.0f, 50.0f, {91, 90, 106, 130, 106, 77});
const Species SPECIES_REGIGIGAS(486, "Regigigas", {Type::NORMAL, Type::NONE}, 420.0f, -1.0f, {110, 160, 110, 80, 110, 100});
const Species SPECIES_GIRATINA(487, "Giratina", {Type::GHOST, Type::DRAGON}, 750.0f, -1.0f, {150, 100, 120, 100, 120, 90});
const Species SPECIES_CRESSELIA(488, "Cresselia", {Type::PSYCHIC, Type::NONE}, 85.6f, 0.0f, {120, 70, 120, 75, 130, 85});
const Species SPECIES_PHIONE(489, "Phione", {Type::WATER, Type::NONE}, 3.1f, -1.0f, {80, 80, 80, 80, 80, 80});
const Species SPECIES_MANAPHY(490, "Manaphy", {Type::WATER, Type::NONE}, 1.4f, -1.0f, {100, 100, 100, 100, 100, 100});
const Species SPECIES_DARKRAI(491, "Darkrai", {Type::DARK, Type::NONE}, 50.5f, -1.0f, {70, 90, 90, 135, 90, 125});
const Species SPECIES_SHAYMIN(492, "Shaymin", {Type::GRASS, Type::NONE}, 2.1f, -1.0f, {100, 100, 100, 100, 100, 100});
const Species SPECIES_ARCEUS(493, "Arceus", {Type::NORMAL, Type::NONE}, 320.0f, -1.0f, {120, 120, 120, 120, 120, 120});
const Species SPECIES_VICTINI(494, "Victini", {Type::PSYCHIC, Type::FIRE}, 4.0f, -1.0f, {100, 100, 100, 100, 100, 100});
const Species SPECIES_SNIVY(495, "Snivy", {Type::GRASS, Type::NONE}, 8.1f, 87.5f, {45, 45, 55, 45, 55, 63});
const Species SPECIES_SERVINE(496, "Servine", {Type::GRASS, Type::NONE}, 16.0f, 87.5f, {60, 60, 75, 60, 75, 83});
const Species SPECIES_SERPERIOR(497, "Serperior", {Type::GRASS, Type::NONE}, 63.0f, 87.5f, {75, 75, 95, 75, 95, 113});
const Species SPECIES_TEPIG(498, "Tepig", {Type::FIRE, Type::NONE}, 9.9f, 87.5f, {65, 63, 45, 45, 45, 45});
const Species SPECIES_PIGNITE(499, "Pignite", {Type::FIRE, Type::FIGHTING}, 55.5f, 87.5f, {90, 93, 55, 70, 55, 55});
const Species SPECIES_EMBOAR(500, "Emboar", {Type::FIRE, Type::FIGHTING}, 150.0f, 87.5f, {110, 123, 65, 100, 65, 65});
const Species SPECIES_OSHAWOTT(501, "Oshawott", {Type::WATER, Type::NONE}, 5.9f, 87.5f, {55, 55, 45, 63, 45, 45});
const Species SPECIES_DEWOTT(502, "Dewott", {Type::WATER, Type::NONE}, 24.5f, 87.5f, {75, 75, 60, 83, 60, 60});
const Species SPECIES_SAMUROTT(503, "Samurott", {Type::WATER, Type::NONE}, 94.6f, 87.5f, {95, 100, 85, 108, 70, 70});
const Species SPECIES_PATRAT(504, "Patrat", {Type::NORMAL, Type::NONE}, 11.6f, 50.0f, {45, 55, 39, 35, 39, 42});
const Species SPECIES_WATCHOG(505, "Watchog", {Type::NORMAL, Type::NONE}, 27.0f, 50.0f, {60, 85, 69, 60, 69, 77});
const Species SPECIES_LILLIPUP(506, "Lillipup", {Type::NORMAL, Type::NONE}, 4.1f, 50.0f, {45, 60, 45, 25, 45, 55});
const Species SPECIES_HERDIER(507, "Herdier", {Type::NORMAL, Type::NONE}, 14.7f, 50.0f, {65, 80, 65, 35, 65, 60});
const Species SPECIES_STOUTLAND(508, "Stoutland", {Type::NORMAL, Type::NONE}, 61.0f, 50.0f, {85, 100, 90, 45, 90, 80});
const Species SPECIES_PURRLOIN(509, "Purrloin", {Type::DARK, Type::NONE}, 10.1f, 50.0f, {41, 50, 37, 50, 37, 66});
const Species SPECIES_LIEPARD(510, "Liepard", {Type::DARK, Type::NONE}, 37.5f, 50.0f, {64, 88, 50, 88, 50, 106});
const Species SPECIES_PANSAGE(511, "Pansage", {Type::GRASS, Type::NONE}, 10.5f, 87.5f, {50, 53, 48, 53, 48, 64});
const Species SPECIES_SIMISAGE(512, "Simisage", {Type::GRASS, Type::NONE}, 30.5f, 87.5f, {75, 98, 63, 98, 63, 101});
const Species SPECIES_PANSEAR(513, "Pansear", {Type::FIRE, Type::NONE}, 11.0f, 87.5f, {50, 53, 48, 53, 48, 64});
const Species SPECIES_SIMISEAR(514, "Simisear", {Type::FIRE, Type::NONE}, 28.0f, 87.5f, {75, 98, 63, 98, 63, 101});
const Species SPECIES_PANPOUR(515, "Panpour", {Type::WATER, Type::NONE}, 13.5f, 87.5f, {50, 53, 48, 53, 48, 64});
const Species SPECIES_SIMIPOUR(516, "Simipour", {Type::WATER, Type::NONE}, 29.0f, 87.5f, {75, 98, 63, 98, 63, 101});
const Species SPECIES_MUNNA(517, "Munna", {Type::PSYCHIC, Type::NONE}, 23.3f, 50.0f, {76, 25, 45, 67, 55, 24});
const Species SPECIES_MUSHARNA(518, "Musharna", {Type::PSYCHIC, Type::NONE}, 60.5f, 50.0f, {116, 55, 85, 107, 95, 29});
const Species SPECIES_PIDOVE(519, "Pidove", {Type::NORMAL, Type::FLYING}, 2.1f, 50.0f, {50, 55, 50, 36, 30, 43});
const Species SPECIES_TRANQUILL(520, "Tranquill", {Type::NORMAL, Type::FLYING}, 15.0f, 50.0f, {62, 77, 62, 50, 42, 65});
const Species SPECIES_UNFEZANT(521, "Unfezant", {Type::NORMAL, Type::FLYING}, 29.0f, 50.0f, {80, 105, 80, 65, 55, 93});
const Species SPECIES_BLITZLE(522, "Blitzle", {Type::ELECTRIC, Type::NONE}, 29.8f, 50.0f, {45, 60, 32, 50, 32, 76});
const Species SPECIES_ZEBSTRIKA(523, "Zebstrika", {Type::ELECTRIC, Type::NONE}, 79.5f, 50.0f, {75, 100, 63, 80, 63, 116});
const Species SPECIES_ROGGENROLA(524, "Roggenrola", {Type::ROCK, Type::NONE}, 18.0f, 50.0f, {55, 75, 85, 25, 25, 15});
const Species SPECIES_BOLDORE(525, "Boldore", {Type::ROCK, Type::NONE}, 102.0f, 50.0f, {70, 105, 105, 50, 40, 20});
const Species SPECIES_GIGALITH(526, "Gigalith", {Type::ROCK, Type::NONE}, 260.0f, 50.0f, {85, 135, 130, 60, 70, 25});
const Species SPECIES_WOOBAT(527, "Woobat", {Type::PSYCHIC, Type::FLYING}, 2.1f, 50.0f, {55, 45, 43, 55, 43, 72});
const Species SPECIES_SWOOBAT(528, "Swoobat", {Type::PSYCHIC, Type::FLYING}, 10.5f, 50.0f, {67, 57, 55, 77, 55, 114});
const Species SPECIES_DRILBUR(529, "Drilbur", {Type::GROUND, Type::NONE}, 8.5f, 50.0f, {60, 85, 40, 30, 45, 68});
const Species SPECIES_EXCADRILL(530, "Excadrill", {Type::GROUND, Type::STEEL}, 40.4f, 50.0f, {110, 135, 60, 50, 65, 88});
const Species SPECIES_AUDINO(531, "Audino", {Type::NORMAL, Type::NONE}, 31.0f, 50.0f, {103, 60, 86, 60, 86, 50});
const Species SPECIES_TIMBURR(532, "Timburr", {Type::FIGHTING, Type::NONE}, 12.5f, 75.0f, {75, 80, 55, 25, 35, 35});
const Species SPECIES_GURDURR(533, "Gurdurr", {Type::FIGHTING, Type::NONE}, 40.0f, 75.0f, {85, 105, 85, 40, 50, 40});
const Species SPECIES_CONKELDURR(534, "Conkeldurr", {Type::FIGHTING, Type::NONE}, 87.0f, 75.0f, {105, 140, 95, 55, 65, 45});
const Species SPECIES_TYMPOLE(535, "Tympole", {Type::WATER, Type::NONE}, 4.5f, 50.0f, {50, 50, 40, 50, 40, 64});
const Species SPECIES_PALPITOAD(536, "Palpitoad", {Type::WATER, Type::GROUND}, 17.0f, 50.0f, {75, 65, 55, 65, 55, 69});
const Species SPECIES_SEISMITOAD(537, "Seismitoad", {Type::WATER, Type::GROUND}, 62.0f, 50.0f, {105, 85, 75, 85, 75, 74});
const Species SPECIES_THROH(538, "Throh", {Type::FIGHTING, Type::NONE}, 55.5f, 100.0f, {120, 100, 85, 30, 85, 45});
const Species SPECIES_SAWK(539, "Sawk", {Type::FIGHTING, Type::NONE}, 51.0f, 100.0f, {75, 125, 75, 30, 75, 85});
const Species SPECIES_SEWADDLE(540, "Sewaddle", {Type::BUG, Type::GRASS}, 2.5f, 50.0f, {45, 53, 70, 40, 60, 42});
const Species SPECIES_SWADLOON(541, "Swadloon", {Type::BUG, Type::GRASS}, 7.3f, 50.0f, {55, 63, 90, 50, 80, 42});
const Species SPECIES_LEAVANNY(542, "Leavanny", {Type::BUG, Type::GRASS}, 20.5f, 50.0f, {75, 103, 80, 70, 70, 92});
const Species SPECIES_VENIPEDE(543, "Venipede", {Type::BUG, Type::POISON}, 5.3f, 50.0f, {30, 45, 59, 30, 39, 57});
const Species SPECIES_WHIRLIPEDE(544, "Whirlipede", {Type::BUG, Type::POISON}, 58.5f, 50.0f, {40, 55, 99, 40, 79, 47});
const Species SPECIES_SCOLIPEDE(545, "Scolipede", {Type::BUG, Type::POISON}, 200.5f, 50.0f, {60, 90, 89, 55, 69, 112});
const Species SPECIES_COTTONEE(546, "Cottonee", {Type::GRASS, Type::FAIRY}, 0.6f, 50.0f, {40, 27, 60, 37, 50, 66});
const Species SPECIES_WHIMSICOTT(547, "Whimsicott", {Type::GRASS, Type::FAIRY}, 6.6f, 50.0f, {60, 67, 85, 77, 75, 116});
const Species SPECIES_PETILIL(548, "Petilil", {Type::GRASS, Type::NONE}, 6.6f, 0.0f, {45, 35, 50, 70, 50, 30});
const Species SPECIES_LILLIGANT(549, "Lilligant", {Type::GRASS, Type::NONE}, 16.3f, 0.0f, {70, 60, 75, 110, 75, 90});
const Species SPECIES_BASCULIN(550, "Basculin", {Type::WATER, Type::NONE}, 18.0f, 50.0f, {70, 92, 65, 80, 55, 98});
const Species SPECIES_SANDILE(551, "Sandile", {Type::GROUND, Type::DARK}, 15.2f, 50.0f, {50, 72, 35, 35, 35, 65});
const Species SPECIES_KROKOROK(552, "Krokorok", {Type::GROUND, Type::DARK}, 33.4f, 50.0f, {60, 82, 45, 45, 45, 74});
const Species SPECIES_KROOKODILE(553, "Krookodile", {Type::GROUND, Type::DARK}, 96.3f, 50.0f, {95, 117, 70, 65, 70, 92});
const Species SPECIES_DARUMAKA(554, "Darumaka", {Type::FIRE, Type::NONE}, 37.5f, 50.0f, {70, 90, 45, 15, 45, 50});
const Species SPECIES_DARMANITAN(555, "Darmanitan", {Type::FIRE, Type::NONE}, 92.9f, 50.0f, {105, 140, 55, 30, 55, 95});
const Species SPECIES_MARACTUS(556, "Maractus", {Type::GRASS, Type::NONE}, 28.0f, 50.0f, {75, 86, 67, 106, 67, 60});
const Species SPECIES_DWEBBLE(557, "Dwebble", {Type::BUG, Type::ROCK}, 14.5f, 50.0f, {50, 65, 85, 35, 35, 55});
const Species SPECIES_CRUSTLE(558, "Crustle", {Type::BUG, Type::ROCK}, 200.0f, 50.0f, {70, 95, 125, 65, 75, 45});
const Species SPECIES_SCRAGGY(559, "Scraggy", {Type::DARK, Type::FIGHTING}, 11.8f, 50.0f, {50, 75, 70, 35, 70, 48});
const Species SPECIES_SCRAFTY(560, "Scrafty", {Type::DARK, Type::FIGHTING}, 30.0f, 50.0f, {65, 90, 115, 45, 115, 58});
const Species SPECIES_SIGILYPH(561, "Sigilyph", {Type::PSYCHIC, Type::FLYING}, 14.0f, 50.0f, {72, 58, 80, 103, 80, 97});
const Species SPECIES_YAMASK(562, "Yamask", {Type::GHOST, Type::NONE}, 1.5f, 50.0f, {38, 30, 85, 55, 65, 30});
const Species SPECIES_COFAGRIGUS(563, "Cofagrigus", {Type::GHOST, Type::NONE}, 76.5f, 50.0f, {58, 50, 145, 95, 105, 30});
const Species SPECIES_TIRTOUGA(564, "Tirtouga", {Type::WATER, Type::ROCK}, 16.5f, 87.5f, {54, 78, 103, 53, 45, 22});
const Species SPECIES_CARRACOSTA(565, "Carracosta", {Type::WATER, Type::ROCK}, 81.0f, 87.5f, {74, 108, 133, 83, 65, 32});
const Species SPECIES_ARCHEN(566, "Archen", {Type::ROCK, Type::FLYING}, 9.5f, 87.5f, {55, 112, 45, 74, 45, 70});
const Species SPECIES_ARCHEOPS(567, "Archeops", {Type::ROCK, Type::FLYING}, 32.0f, 87.5f, {75, 140, 65, 112, 65, 110});
const Species SPECIES_TRUBBISH(568, "Trubbish", {Type::POISON, Type::NONE}, 31.0f, 50.0f, {50, 50, 62, 40, 62, 65});
const Species SPECIES_GARBODOR(569, "Garbodor", {Type::POISON, Type::NONE}, 107.3f, 50.0f, {80, 95, 82, 60, 82, 75});
const Species SPECIES_ZORUA(570, "Zorua", {Type::DARK, Type::NONE}, 12.5f, 87.5f, {40, 65, 40, 80, 40, 65});
const Species SPECIES_ZOROARK(571, "Zoroark", {Type::DARK, Type::NONE}, 81.1f, 87.5f, {60, 105, 60, 120, 60, 105});
const Species SPECIES_MINCCINO(572, "Minccino", {Type::NORMAL, Type::NONE}, 5.8f, 25.0f, {55, 50, 40, 40, 40, 75});
const Species SPECIES_CINCCINO(573, "Cinccino", {Type::NORMAL, Type::NONE}, 7.5f, 25.0f, {75, 95, 60, 65, 60, 115});
const Species SPECIES_GOTHITA(574, "Gothita", {Type::PSYCHIC, Type::NONE}, 5.8f, 25.0f, {45, 30, 50, 55, 65, 45});
const Species SPECIES_GOTHORITA(575, "Gothorita", {Type::PSYCHIC, Type::NONE}, 18.0f, 25.0f, {60, 45, 70, 75, 85, 55});
const Species SPECIES_GOTHITELLE(576, "Gothitelle", {Type::PSYCHIC, Type::NONE}, 44.0f, 25.0f, {70, 55, 95, 95, 110, 65});
const Species SPECIES_SOLOSIS(577, "Solosis", {Type::PSYCHIC, Type::NONE}, 1.0f, 50.0f, {45, 30, 40, 105, 50, 20});
const Species SPECIES_DUOSION(578, "Duosion", {Type::PSYCHIC, Type::NONE}, 8.0f, 50.0f, {65, 40, 50, 125, 60, 30});
const Species SPECIES_REUNICLUS(579, "Reuniclus", {Type::PSYCHIC, Type::NONE}, 20.1f, 50.0f, {110, 65, 75, 125, 85, 30});
const Species SPECIES_DUCKLETT(580, "Ducklett", {Type::WATER, Type::FLYING}, 5.5f, 50.0f, {62, 44, 50, 44, 50, 55});
const Species SPECIES_SWANNA(581, "Swanna", {Type::WATER, Type::FLYING}, 24.2f, 50.0f, {75, 87, 63, 87, 63, 98});
const Species SPECIES_VANILLITE(582, "Vanillite", {Type::ICE, Type::NONE}, 5.7f, 50.0f, {36, 50, 50, 65, 60, 44});
const Species SPECIES_VANILLISH(583, "Vanillish", {Type::ICE, Type::NONE}, 41.0f, 50.0f, {51, 65, 65, 80, 75, 59});
const Species SPECIES_VANILLUXE(584, "Vanilluxe", {Type::ICE, Type::NONE}, 57.5f, 50.0f, {71, 95, 85, 110, 95, 79});
const Species SPECIES_DEERLING(585, "Deerling", {Type::NORMAL, Type::GRASS}, 19.5f, 50.0f, {60, 60, 50, 40, 50, 75});
const Species SPECIES_SAWSBUCK(586, "Sawsbuck", {Type::NORMAL, Type::GRASS}, 92.5f, 50.0f, {80, 100, 70, 60, 70, 95});
const Species SPECIES_EMOLGA(587, "Emolga", {Type::ELECTRIC, Type::FLYING}, 5.0f, 50.0f, {55, 75, 60, 75, 60, 103});
const Species SPECIES_KARRABLAST(588, "Karrablast", {Type::BUG, Type::NONE}, 5.9f, 50.0f, {50, 75, 45, 40, 45, 60});
const Species SPECIES_ESCAVALIER(589, "Escavalier", {Type::BUG, Type::STEEL}, 33.0f, 50.0f, {70, 135, 105, 60, 105, 20});
const Species SPECIES_FOONGUS(590, "Foongus", {Type::GRASS, Type::POISON}, 1.0f, 50.0f, {69, 55, 45, 55, 55, 15});
const Species SPECIES_AMOONGUSS(591, "Amoonguss", {Type::GRASS, Type::POISON}, 10.5f, 50.0f, {114, 85, 70, 85, 80, 30});
const Species SPECIES_FRILLISH(592, "Frillish", {Type::WATER, Type::GHOST}, 33.0f, 50.0f, {55, 40, 50, 65, 85, 40});
const Species SPECIES_JELLICENT(593, "Jellicent", {Type::WATER, Type::GHOST}, 135.0f, 50.0f, {100, 60, 70, 85, 105, 60});
const Species SPECIES_ALOMOMOLA(594, "Alomomola", {Type::WATER, Type::NONE}, 31.6f, 50.0f, {165, 75, 80, 40, 45, 65});
const Species SPECIES_JOLTIK(595, "Joltik", {Type::BUG, Type::ELECTRIC}, 0.6f, 50.0f, {50, 47, 50, 57, 50, 65});
const Species SPECIES_GALVANTULA(596, "Galvantula", {Type::BUG, Type::ELECTRIC}, 14.3f, 50.0f, {70, 77, 60, 97, 60, 108});
const Species SPECIES_FERROSEED(597, "Ferroseed", {Type::GRASS, Type::STEEL}, 18.8f, 50.0f, {44, 50, 91, 24, 86, 10});
const Species SPECIES_FERROTHORN(598, "Ferrothorn", {Type::GRASS, Type::STEEL}, 110.0f, 50.0f, {74, 94, 131, 54, 116, 20});
const Species SPECIES_KLINK(599, "Klink", {Type::STEEL, Type::NONE}, 21.0f, -1.0f, {40, 55, 70, 45, 60, 30});
const Species SPECIES_KLANG(600, "Klang", {Type::STEEL, Type::NONE}, 51.0f, -1.0f, {60, 80, 95, 70, 85, 50});
const Species SPECIES_KLINKLANG(601, "Klinklang", {Type::STEEL, Type::NONE}, 81.0f, -1.0f, {60, 100, 115, 70, 85, 90});
const Species SPECIES_TYNAMO(602, "Tynamo", {Type::ELECTRIC, Type::NONE}, 0.3f, 50.0f, {35, 55, 40, 45, 40, 60});
const Species SPECIES_EELEKTRIK(603, "Eelektrik", {Type::ELECTRIC, Type::NONE}, 22.0f, 50.0f, {65, 85, 70, 75, 70, 40});
const Species SPECIES_EELEKTROSS(604, "Eelektross", {Type::ELECTRIC, Type::NONE}, 80.5f, 50.0f, {85, 115, 80, 105, 80, 50});
const Species SPECIES_ELGYEM(605, "Elgyem", {Type::PSYCHIC, Type::NONE}, 9.0f, 50.0f, {55, 55, 55, 85, 55, 30});
const Species SPECIES_BEHEEYEM(606, "Beheeyem", {Type::PSYCHIC, Type::NONE}, 34.5f, 50.0f, {75, 75, 75, 125, 95, 40});
const Species SPECIES_LITWICK(607, "Litwick", {Type::GHOST, Type::FIRE}, 3.1f, 50.0f, {50, 30, 55, 65, 55, 20});
const Species SPECIES_LAMPENT(608, "Lampent", {Type::GHOST, Type::FIRE}, 13.0f, 50.0f, {60, 40, 60, 95, 60, 55});
const Species SPECIES_CHANDELURE(609, "Chandelure", {Type::GHOST, Type::FIRE}, 34.3f, 50.0f, {60, 55, 90, 145, 90, 80});
const Species SPECIES_AXEW(610, "Axew", {Type::DRAGON, Type::NONE}, 18.0f, 50.0f, {46, 87, 60, 30, 40, 57});
const Species SPECIES_FRAXURE(611, "Fraxure", {Type::DRAGON, Type::NONE}, 36.0f, 50.0f, {66, 117, 70, 40, 50, 67});
const Species SPECIES_HAXORUS(612, "Haxorus", {Type::DRAGON, Type::NONE}, 105.5f, 50.0f, {76, 147, 90, 60, 70, 97});
const Species SPECIES_CUBCHOO(613, "Cubchoo", {Type::ICE, Type::NONE}, 8.5f, 50.0f, {55, 70, 40, 60, 40, 40});
const Species SPECIES_BEARTIC(614, "Beartic", {Type::ICE, Type::NONE}, 260.0f, 50.0f, {95, 110, 80, 70, 80, 50});
const Species SPECIES_CRYOGONAL(615, "Cryogonal", {Type::ICE, Type::NONE}, 148.0f, -1.0f, {70, 50, 30, 95, 135, 105});
const Species SPECIES_SHELMET(616, "Shelmet", {Type::BUG, Type::NONE}, 7.7f, 50.0f, {50, 40, 85, 40, 65, 25});
const Species SPECIES_ACCELGOR(617, "Accelgor", {Type::BUG, Type::NONE}, 25.3f, 50.0f, {80, 70, 40, 100, 60, 145});
const Species SPECIES_STUNFISK(618, "Stunfisk", {Type::GROUND, Type::ELECTRIC}, 11.0f, 50.0f, {109, 66, 84, 81, 99, 32});
const Species SPECIES_MIENFOO(619, "Mienfoo", {Type::FIGHTING, Type::NONE}, 20.0f, 50.0f, {45, 85, 50, 55, 50, 65});
const Species SPECIES_MIENSHAO(620, "Mienshao", {Type::FIGHTING, Type::NONE}, 35.5f, 50.0f, {65, 125, 60, 95, 60, 105});
const Species SPECIES_DRUDDIGON(621, "Druddigon", {Type::DRAGON, Type::NONE}, 139.0f, 50.0f, {77, 120, 90, 60, 90, 48});
const Species SPECIES_GOLETT(622, "Golett", {Type::GROUND, Type::GHOST}, 92.0f, -1.0f, {59, 74, 50, 35, 50, 35});
const Species SPECIES_GOLURK(623, "Golurk", {Type::GROUND, Type::GHOST}, 330.0f, -1.0f, {89, 124, 80, 55, 80, 55});
const Species SPECIES_PAWNIARD(624, "Pawniard", {Type::DARK, Type::STEEL}, 10.2f, 50.0f, {45, 85, 70, 40, 40, 60});
const Species SPECIES_BISHARP(625, "Bisharp", {Type::DARK, Type::STEEL}, 70.0f, 50.0f, {65, 125, 100, 60, 70, 70});
const Species SPECIES_BOUFFALANT(626, "Bouffalant", {Type::NORMAL, Type::NONE}, 94.6f, 50.0f, {95, 110, 95, 40, 95, 55});
const Species SPECIES_RUFFLET(627, "Rufflet", {Type::NORMAL, Type::FLYING}, 10.5f, 100.0f, {70, 83, 50, 37, 50, 60});
const Species SPECIES_BRAVIARY(628, "Braviary", {Type::NORMAL, Type::FLYING}, 41.0f, 100.0f, {100, 123, 75, 57, 75, 80});
const Species SPECIES_VULLABY(629, "Vullaby", {Type::DARK, Type::FLYING}, 9.0f, 0.0f, {70, 55, 75, 45, 65, 60});
const Species SPECIES_MANDIBUZZ(630, "Mandibuzz", {Type::DARK, Type::FLYING}, 39.5f, 0.0f, {110, 65, 105, 55, 95, 80});
const Species SPECIES_HEATMOR(631, "Heatmor", {Type::FIRE, Type::NONE}, 58.0f, 50.0f, {85, 97, 66, 105, 66, 65});
const Species SPECIES_DURANT(632, "Durant", {Type::BUG, Type::STEEL}, 33.0f, 50.0f, {58, 109, 112, 48, 48, 109});
const Species SPECIES_DEINO(633, "Deino", {Type::DARK, Type::DRAGON}, 17.3f, 50.0f, {52, 65, 50, 45, 50, 38});
const Species SPECIES_ZWEILOUS(634, "Zweilous", {Type::DARK, Type::DRAGON}, 50.0f, 50.0f, {72, 85, 70, 65, 70, 58});
const Species SPECIES_HYDREIGON(635, "Hydreigon", {Type::DARK, Type::DRAGON}, 160.0f, 50.0f, {92, 105, 90, 125, 90, 98});
const Species SPECIES_LARVESTA(636, "Larvesta", {Type::BUG, Type::FIRE}, 28.8f, 50.0f, {55, 85, 55, 50, 55, 60});
const Species SPECIES_VOLCARONA(637, "Volcarona", {Type::BUG, Type::FIRE}, 46.0f, 50.0f, {85, 60, 65, 135, 105, 100});
const Species SPECIES_COBALION(638, "Cobalion", {Type::STEEL, Type::FIGHTING}, 250.0f, -1.0f, {91, 90, 129, 90, 72, 108});
const Species SPECIES_TERRAKION(639, "Terrakion", {Type::ROCK, Type::FIGHTING}, 260.0f, -1.0f, {91, 129, 90, 72, 90, 108});
const Species SPECIES_VIRIZION(640, "Virizion", {Type::GRASS, Type::FIGHTING}, 200.0f, -1.0f, {91, 90, 72, 90, 129, 108});
const Species SPECIES_TORNADUS(641, "Tornadus", {Type::FLYING, Type::NONE}, 63.0f, 100.0f, {79, 115, 70, 125, 80, 111});
const Species SPECIES_THUNDURUS(642, "Thundurus", {Type::ELECTRIC, Type::FLYING}, 61.0f, 100.0f, {79, 115, 70, 125, 80, 111});
const Species SPECIES_RESHIRAM(643, "Reshiram", {Type::DRAGON, Type::FIRE}, 330.0f, -1.0f, {100, 120, 100, 150, 120, 90});
const Species SPECIES_ZEKROM(644, "Zekrom", {Type::DRAGON, Type::ELECTRIC}, 345.0f, -1.0f, {100, 150, 120, 120, 100, 90});
const Species SPECIES_LANDORUS(645, "Landorus", {Type::GROUND, Type::FLYING}, 68.0f, 100.0f, {89, 125, 90, 115, 80, 101});
const Species SPECIES_KYUREM(646, "Kyurem", {Type::DRAGON, Type::ICE}, 325.0f, -1.0f, {125, 130, 90, 130, 90, 95});
const Species SPECIES_KELDEO(647, "Keldeo", {Type::WATER, Type::FIGHTING}, 48.5f, -1.0f, {91, 72, 90, 129, 90, 108});
const Species SPECIES_MELOETTA(648, "Meloetta", {Type::NORMAL, Type::PSYCHIC}, 6.5f, -1.0f, {100, 77, 77, 128, 128, 90});
const Species SPECIES_GENESECT(649, "Genesect", {Type::BUG, Type::STEEL}, 82.5f, -1.0f, {71, 120, 95, 120, 95, 99});
const Species SPECIES_CHESPIN(650, "Chespin", {Type::GRASS, Type::NONE}, 9.0f, 87.5f, {56, 61, 65, 48, 45, 38});
const Species SPECIES_QUILLADIN(651, "Quilladin", {Type::GRASS, Type::NONE}, 29.0f, 87.5f, {61, 78, 95, 56, 58, 57});
const Species SPECIES_CHESNAUGHT(652, "Chesnaught", {Type::GRASS, Type::FIGHTING}, 90.0f, 87.5f, {88, 107, 122, 74, 75, 64});
const Species SPECIES_FENNEKIN(653, "Fennekin", {Type::FIRE, Type::NONE}, 9.4f, 87.5f, {40, 45, 40, 62, 60, 60});
const Species SPECIES_BRAIXEN(654, "Braixen", {Type::FIRE, Type::NONE}, 14.5f, 87.5f, {59, 59, 58, 90, 70, 73});
const Species SPECIES_DELPHOX(655, "Delphox", {Type::FIRE, Type::PSYCHIC}, 39.0f, 87.5f, {75, 69, 72, 114, 100, 104});
const Species SPECIES_FROAKIE(656, "Froakie", {Type::WATER, Type::NONE}, 7.0f, 87.5f, {41, 56, 40, 62, 44, 71});
const Species SPECIES_FROGADIER(657, "Frogadier", {Type::WATER, Type::NONE}, 10.9f, 87.5f, {54, 63, 52, 83, 56, 97});
const Species SPECIES_GRENINJA(658, "Greninja", {Type::WATER, Type::DARK}, 40.0f, 87.5f, {72, 95, 67, 103, 71, 122});
const Species SPECIES_BUNNELBY(659, "Bunnelby", {Type::NORMAL, Type::NONE}, 5.0f, 50.0f, {38, 36, 38, 32, 36, 57});
const Species SPECIES_DIGGERSBY(660, "Diggersby", {Type::NORMAL, Type::GROUND}, 42.4f, 50.0f, {85, 56, 77, 50, 77, 78});
const Species SPECIES_FLETCHLING(661, "Fletchling", {Type::NORMAL, Type::FLYING}, 1.7f, 50.0f, {45, 50, 43, 40, 38, 62});
const Species SPECIES_FLETCHINDER(662, "Fletchinder", {Type::FIRE, Type::FLYING}, 16.0f, 50.0f, {62, 73, 55, 56, 52, 84});
const Species SPECIES_TALONFLAME(663, "Talonflame", {Type::FIRE, Type::FLYING}, 24.5f, 50.0f, {78, 81, 71, 74, 69, 126});
const Species SPECIES_SCATTERBUG(664, "Scatterbug", {Type::BUG, Type::NONE}, 2.5f, 50.0f, {38, 35, 40, 27, 25, 35});
const Species SPECIES_SPEWPA(665, "Spewpa", {Type::BUG, Type::NONE}, 8.4f, 50.0f, {45, 22, 60, 27, 30, 29});
const Species SPECIES_VIVILLON(666, "Vivillon", {Type::BUG, Type::FLYING}, 17.0f, 50.0f, {80, 52, 50, 90, 50, 89});
const Species SPECIES_LITLEO(667, "Litleo", {Type::FIRE, Type::NORMAL}, 13.5f, 12.5f, {62, 50, 58, 73, 54, 72});
const Species SPECIES_PYROAR(668, "Pyroar", {Type::FIRE, Type::NORMAL}, 81.5f, 12.5f, {86, 68, 72, 109, 66, 106});
const Species SPECIES_FLABEBE(669, "Flabébé", {Type::FAIRY, Type::NONE}, 0.1f, 0.0f, {44, 38, 39, 61, 79, 42});
const Species SPECIES_FLOETTE(670, "Floette", {Type::FAIRY, Type::NONE}, 0.9f, 0.0f, {54, 45, 47, 75, 98, 52});
const Species SPECIES_FLORGES(671, "Florges", {Type::FAIRY, Type::NONE}, 10.0f, 0.0f, {78, 65, 68, 112, 154, 75});
const Species SPECIES_SKIDDO(672, "Skiddo", {Type::GRASS, Type::NONE}, 31.0f, 50.0f, {66, 65, 48, 62, 57, 52});
const Species SPECIES_GOGOAT(673, "Gogoat", {Type::GRASS, Type::NONE}, 91.0f, 50.0f, {123, 100, 62, 97, 81, 68});
const Species SPECIES_PANCHAM(674, "Pancham", {Type::FIGHTING, Type::NONE}, 8.0f, 50.0f, {67, 82, 62, 46, 48, 43});
const Species SPECIES_PANGORO(675, "Pangoro", {Type::FIGHTING, Type::DARK}, 136.0f, 50.0f, {95, 124, 78, 69, 71, 58});
const Species SPECIES_FURFROU(676, "Furfrou", {Type::NORMAL, Type::NONE}, 28.0f, 50.0f, {75, 80, 60, 65, 90, 102});
const Species SPECIES_ESPURR(677, "Espurr", {Type::PSYCHIC, Type::NONE}, 3.5f, 50.0f, {62, 48, 54, 63, 60, 68});
const Species SPECIES_MEOWSTIC(678, "Meowstic", {Type::PSYCHIC, Type::NONE}, 8.5f, 50.0f, {74, 48, 76, 83, 81, 104});
const Species SPECIES_HONEDGE(679, "Honedge", {Type::STEEL, Type::GHOST}, 2.0f, 50.0f, {45, 80, 100, 35, 37, 28});
const Species SPECIES_DOUBLADE(680, "Doublade", {Type::STEEL, Type::GHOST}, 4.5f, 50.0f, {59, 110, 150, 45, 49, 35});
const Species SPECIES_AEGISLASH(681, "Aegislash", {Type::STEEL, Type::GHOST}, 53.0f, 50.0f, {60, 50, 150, 50, 150, 60});
const Species SPECIES_SPRITZEE(682, "Spritzee", {Type::FAIRY, Type::NONE}, 0.5f, 50.0f, {78, 52, 60, 63, 65, 23});
const Species SPECIES_AROMATISSE(683, "Aromatisse", {Type::FAIRY, Type::NONE}, 15.5f, 50.0f, {101, 72, 72, 99, 89, 29});
const Species SPECIES_SWIRLIX(684, "Swirlix", {Type::FAIRY, Type::NONE}, 3.5f, 50.0f, {62, 48, 66, 59, 57, 49});
const Species SPECIES_SLURPUFF(685, "Slurpuff", {Type::FAIRY, Type::NONE}, 5.0f, 50.0f, {82, 80, 86, 85, 75, 72});
const Species SPECIES_INKAY(686, "Inkay", {Type::DARK, Type::PSYCHIC}, 3.5f, 50.0f, {53, 54, 53, 37, 46, 45});
const Species SPECIES_MALAMAR(687, "Malamar", {Type::DARK, Type::PSYCHIC}, 47.0f, 50.0f, {86, 92, 88, 68, 75, 73});
const Species SPECIES_BINACLE(688, "Binacle", {Type::ROCK, Type::WATER}, 31.0f, 50.0f, {42, 52, 67, 39, 56, 50});
const Species SPECIES_BARBARACLE(689, "Barbaracle", {Type::ROCK, Type::WATER}, 96.0f, 50.0f, {72, 105, 115, 54, 86, 68});
const Species SPECIES_SKRELP(690, "Skrelp", {Type::POISON, Type::WATER}, 7.3f, 50.0f, {50, 60, 60, 60, 60, 30});
const Species SPECIES_DRAGALGE(691, "Dragalge", {Type::POISON, Type::DRAGON}, 81.5f, 50.0f, {65, 75, 90, 97, 123, 44});
const Species SPECIES_CLAUNCHER(692, "Clauncher", {Type::WATER, Type::NONE}, 8.3f, 50.0f, {50, 53, 62, 58, 63, 44});
const Species SPECIES_CLAWITZER(693, "Clawitzer", {Type::WATER, Type::NONE}, 35.3f, 50.0f, {71, 73, 88, 120, 89, 59});
const Species SPECIES_HELIOPTILE(694, "Helioptile", {Type::ELECTRIC, Type::NORMAL}, 6.0f, 50.0f, {44, 38, 33, 61, 43, 70});
const Species SPECIES_HELIOLISK(695, "Heliolisk", {Type::ELECTRIC, Type::NORMAL}, 21.0f, 50.0f, {62, 55, 52, 109, 94, 109});
const Species SPECIES_TYRUNT(696, "Tyrunt", {Type::ROCK, Type::DRAGON}, 26.0f, 87.5f, {58, 89, 77, 45, 45, 48});
const Species SPECIES_TYRANTRUM(697, "Tyrantrum", {Type::ROCK, Type::DRAGON}, 270.0f, 87.5f, {82, 121, 119, 69, 59, 71});
const Species SPECIES_AMAURA(698, "Amaura", {Type::ROCK, Type::ICE}, 25.2f, 87.5f, {77, 59, 50, 67, 63, 46});
const Species SPECIES_AURORUS(699, "Aurorus", {Type::ROCK, Type::ICE}, 225.0f, 87.5f, {123, 77, 72, 99, 92, 58});
const Species SPECIES_SYLVEON(700, "Sylveon", {Type::FAIRY, Type::NONE}, 23.5f, 87.5f, {95, 65, 65, 110, 130, 60});
const Species SPECIES_HAWLUCHA(701, "Hawlucha", {Type::FIGHTING, Type::FLYING}, 21.5f, 50.0f, {78, 92, 75, 74, 63, 118});
const Species SPECIES_DEDENNE(702, "Dedenne", {Type::ELECTRIC, Type::FAIRY}, 2.2f, 50.0f, {67, 58, 57, 81, 67, 101});
const Species SPECIES_CARBINK(703, "Carbink", {Type::ROCK, Type::FAIRY}, 5.7f, -1.0f, {50, 50, 150, 50, 150, 50});
const Species SPECIES_GOOMY(704, "Goomy", {Type::DRAGON, Type::NONE}, 2.8f, 50.0f, {45, 50, 35, 55, 75, 40});
const Species SPECIES_SLIGGOO(705, "Sliggoo", {Type::DRAGON, Type::NONE}, 17.5f, 50.0f, {68, 75, 53, 83, 113, 60});
const Species SPECIES_GOODRA(706, "Goodra", {Type::DRAGON, Type::NONE}, 150.5f, 50.0f, {90, 100, 70, 110, 150, 80});
const Species SPECIES_KLEFKI(707, "Klefki", {Type::STEEL, Type::FAIRY}, 3.0f, 50.0f, {57, 80, 91, 80, 87, 75});
const Species SPECIES_PHANTUMP(708, "Phantump", {Type::GHOST, Type::GRASS}, 7.0f, 50.0f, {43, 70, 48, 50, 60, 38});
const Species SPECIES_TREVENANT(709, "Trevenant", {Type::GHOST, Type::GRASS}, 71.0f, 50.0f, {85, 110, 76, 65, 82, 56});
const Species SPECIES_PUMPKABOO(710, "Pumpkaboo", {Type::GHOST, Type::GRASS}, 3.5f, 50.0f, {44, 66, 70, 44, 55, 56});
const Species SPECIES_GOURGEIST(711, "Gourgeist", {Type::GHOST, Type::GRASS}, 9.5f, 50.0f, {55, 85, 122, 58, 75, 99});
const Species SPECIES_BERGMITE(712, "Bergmite", {Type::ICE, Type::NONE}, 99.5f, 50.0f, {55, 69, 85, 32, 35, 28});
const Species SPECIES_AVALUGG(713, "Avalugg", {Type::ICE, Type::NONE}, 505.0f, 50.0f, {95, 117, 184, 44, 46, 28});
const Species SPECIES_NOIBAT(714, "Noibat", {Type::FLYING, Type::DRAGON}, 8.0f, 50.0f, {40, 30, 35, 45, 40, 55});
const Species SPECIES_NOIVERN(715, "Noivern", {Type::FLYING, Type::DRAGON}, 85.0f, 50.0f, {85, 70, 80, 97, 80, 123});
const Species SPECIES_XERNEAS(716, "Xerneas", {Type::FAIRY, Type::NONE}, 215.0f, -1.0f, {126, 131, 95, 131, 98, 99});
const Species SPECIES_YVELTAL(717, "Yveltal", {Type::DARK, Type::FLYING}, 203.0f, -1.0f, {126, 131, 95, 131, 98, 99});
const Species SPECIES_ZYGARDE(718, "Zygarde", {Type::DRAGON, Type::GROUND}, 305.0f, -1.0f, {108, 100, 121, 81, 95, 95});
const Species SPECIES_DIANCIE(719, "Diancie", {Type::ROCK, Type::FAIRY}, 8.8f, -1.0f, {50, 100, 150, 100, 150, 50});
const Species SPECIES_HOOPA(720, "Hoopa", {Type::PSYCHIC, Type::GHOST}, 9.0f, -1.0f, {80, 110, 60, 150, 130, 70});
const Species SPECIES_VOLCANION(721, "Volcanion", {Type::FIRE, Type::WATER}, 195.0f, -1.0f, {80, 110, 120, 130, 90, 70});
const Species SPECIES_ROWLET(722, "Rowlet", {Type::GRASS, Type::FLYING}, 1.5f, 87.5f, {68, 55, 55, 50, 50, 42});
const Species SPECIES_DARTRIX(723, "Dartrix", {Type::GRASS, Type::FLYING}, 16.0f, 87.5f, {78, 75, 75, 70, 70, 52});
const Species SPECIES_DECIDUEYE(724, "Decidueye", {Type::GRASS, Type::GHOST}, 36.6f, 87.5f, {78, 107, 75, 100, 100, 70});
const Species SPECIES_LITTEN(725, "Litten", {Type::FIRE, Type::NONE}, 4.3f, 87.5f, {45, 65, 40, 60, 40, 70});
const Species SPECIES_TORRACAT(726, "Torracat", {Type::FIRE, Type::NONE}, 25.0f, 87.5f, {65, 85, 50, 80, 50, 90});
const Species SPECIES_INCINEROAR(727, "Incineroar", {Type::FIRE, Type::DARK}, 83.0f, 87.5f, {95, 115, 90, 80, 90, 60});
const Species SPECIES_POPPLIO(728, "Popplio", {Type::WATER, Type::NONE}, 7.5f, 87.5f, {50, 54, 54, 66, 56, 40});
const Species SPECIES_BRIONNE(729, "Brionne", {Type::WATER, Type::NONE}, 17.5f, 87.5f, {60, 69, 69, 91, 81, 50});
const Species SPECIES_PRIMARINA(730, "Primarina", {Type::WATER, Type::FAIRY}, 44.0f, 87.5f, {80, 74, 74, 126, 116, 60});
const Species SPECIES_PIKIPEK(731, "Pikipek", {Type::NORMAL, Type::FLYING}, 1.2f, 50.0f, {35, 75, 30, 30, 30, 65});
const Species SPECIES_TRUMBEAK(732, "Trumbeak", {Type::NORMAL, Type::FLYING}, 14.8f, 50.0f, {55, 85, 50, 40, 50, 75});
const Species SPECIES_TOUCANNON(733, "Toucannon", {Type::NORMAL, Type::FLYING}, 26.0f, 50.0f, {80, 120, 75, 75, 75, 60});
const Species SPECIES_YUNGOOS(734, "Yungoos", {Type::NORMAL, Type::NONE}, 6.0f, 50.0f, {48, 70, 30, 30, 30, 45});
const Species SPECIES_GUMSHOOS(735, "Gumshoos", {Type::NORMAL, Type::NONE}, 14.2f, 50.0f, {88, 110, 60, 55, 60, 45});
const Species SPECIES_GRUBBIN(736, "Grubbin", {Type::BUG, Type::NONE}, 4.4f, 50.0f, {47, 62, 45, 55, 45, 46});
const Species SPECIES_CHARJABUG(737, "Charjabug", {Type::BUG, Type::ELECTRIC}, 10.5f, 50.0f, {57, 82, 95, 55, 75, 36});
const Species SPECIES_VIKAVOLT(738, "Vikavolt", {Type::BUG, Type::ELECTRIC}, 45.0f, 50.0f, {77, 70, 90, 145, 75, 43});
const Species SPECIES_CRABRAWLER(739, "Crabrawler", {Type::FIGHTING, Type::NONE}, 7.0f, 50.0f, {47, 82, 57, 42, 47, 63});
const Species SPECIES_CRABOMINABLE(740, "Crabominable", {Type::FIGHTING, Type::ICE}, 180.0f, 50.0f, {97, 132, 77, 62, 67, 43});
const Species SPECIES_ORICORIO(741, "Oricorio", {Type::FIRE, Type::FLYING}, 3.4f, 25.0f, {75, 70, 70, 98, 70, 93});
const Species SPECIES_CUTIEFLY(742, "Cutiefly", {Type::BUG, Type::FAIRY}, 0.2f, 50.0f, {40, 45, 40, 55, 40, 84});
const Species SPECIES_RIBOMBEE(743, "Ribombee", {Type::BUG, Type::FAIRY}, 0.5f, 50.0f, {60, 55, 60, 95, 70, 124});
const Species SPECIES_ROCKRUFF(744, "Rockruff", {Type::ROCK, Type::NONE}, 9.2f, 50.0f, {45, 65, 40, 30, 40, 60});
const Species SPECIES_LYCANROC(745, "Lycanroc", {Type::ROCK, Type::NONE}, 25.0f, 50.0f, {75, 115, 65, 55, 65, 112});
const Species SPECIES_WISHIWASHI(746, "Wishiwashi", {Type::WATER, Type::NONE}, 0.3f, 50.0f, {45, 20, 20, 25, 25, 40});
const Species SPECIES_MAREANIE(747, "Mareanie", {Type::POISON, Type::WATER}, 8.0f, 50.0f, {50, 53, 62, 43, 52, 45});
const Species SPECIES_TOXAPEX(748, "Toxapex", {Type::POISON, Type::WATER}, 14.5f, 50.0f, {50, 63, 152, 53, 142, 35});
const Species SPECIES_MUDBRAY(749, "Mudbray", {Type::GROUND, Type::NONE}, 110.0f, 50.0f, {70, 100, 70, 45, 55, 45});
const Species SPECIES_MUDSDALE(750, "Mudsdale", {Type::GROUND, Type::NONE}, 920.0f, 50.0f, {100, 125, 100, 55, 85, 35});
const Species SPECIES_DEWPIDER(751, "Dewpider", {Type::WATER, Type::BUG}, 4.0f, 50.0f, {38, 40, 52, 40, 72, 27});
const Species SPECIES_ARAQUANID(752, "Araquanid", {Type::WATER, Type::BUG}, 82.0f, 50.0f, {68, 70, 92, 50, 132, 42});
const Species SPECIES_FOMANTIS(753, "Fomantis", {Type::GRASS, Type::NONE}, 1.5f, 50.0f, {40, 55, 35, 50, 35, 35});
const Species SPECIES_LURANTIS(754, "Lurantis", {Type::GRASS, Type::NONE}, 18.5f, 50.0f, {70, 105, 90, 80, 90, 45});
const Species SPECIES_MORELULL(755, "Morelull", {Type::GRASS, Type::FAIRY}, 1.5f, 50.0f, {40, 35, 55, 65, 75, 15});
const Species SPECIES_SHIINOTIC(756, "Shiinotic", {Type::GRASS, Type::FAIRY}, 11.5f, 50.0f, {60, 45, 80, 90, 100, 30});
const Species SPECIES_SALANDIT(757, "Salandit", {Type::POISON, Type::FIRE}, 4.8f, 87.5f, {48, 44, 40, 71, 40, 77});
const Species SPECIES_SALAZZLE(758, "Salazzle", {Type::POISON, Type::FIRE}, 22.2f, 0.0f, {68, 64, 60, 111, 60, 117});
const Species SPECIES_STUFFUL(759, "Stufful", {Type::NORMAL, Type::FIGHTING}, 6.8f, 50.0f, {70, 75, 50, 45, 50, 50});
const Species SPECIES_BEWEAR(760, "Bewear", {Type::NORMAL, Type::FIGHTING}, 135.0f, 50.0f, {120, 125, 80, 55, 60, 60});
const Species SPECIES_BOUNSWEET(761, "Bounsweet", {Type::GRASS, Type::NONE}, 3.2f, 0.0f, {42, 30, 38, 30, 38, 32});
const Species SPECIES_STEENEE(762, "Steenee", {Type::GRASS, Type::NONE}, 8.2f, 0.0f, {52, 40, 48, 40, 48, 62});
const Species SPECIES_TSAREENA(763, "Tsareena", {Type::GRASS, Type::NONE}, 21.4f, 0.0f, {72, 120, 98, 50, 98, 72});
const Species SPECIES_COMFEY(764, "Comfey", {Type::FAIRY, Type::NONE}, 0.3f, 25.0f, {51, 52, 90, 82, 110, 100});
const Species SPECIES_ORANGURU(765, "Oranguru", {Type::NORMAL, Type::PSYCHIC}, 76.0f, 50.0f, {90, 60, 80, 90, 110, 60});
const Species SPECIES_PASSIMIAN(766, "Passimian", {Type::FIGHTING, Type::NONE}, 82.8f, 50.0f, {100, 120, 90, 40, 60, 80});
const Species SPECIES_WIMPOD(767, "Wimpod", {Type::BUG, Type::WATER}, 12.0f, 50.0f, {25, 35, 40, 20, 30, 80});
const Species SPECIES_GOLISOPOD(768, "Golisopod", {Type::BUG, Type::WATER}, 108.0f, 50.0f, {75, 125, 140, 60, 90, 40});
const Species SPECIES_SANDYGAST(769, "Sandygast", {Type::GHOST, Type::GROUND}, 70.0f, 50.0f, {55, 55, 80, 70, 45, 15});
const Species SPECIES_PALOSSAND(770, "Palossand", {Type::GHOST, Type::GROUND}, 250.0f, 50.0f, {85, 75, 110, 100, 75, 35});
const Species SPECIES_PYUKUMUKU(771, "Pyukumuku", {Type::WATER, Type::NONE}, 1.2f, 50.0f, {55, 60, 130, 30, 130, 5});
const Species SPECIES_TYPE_NULL(772, "Type: Null", {Type::NORMAL, Type::NONE}, 120.5f, -1.0f, {95, 95, 95, 95, 95, 59});
const Species SPECIES_SILVALLY(773, "Silvally", {Type::NORMAL, Type::NONE}, 100.5f, -1.0f, {95, 95, 95, 95, 95, 95});
const Species SPECIES_MINIOR(774, "Minior", {Type::ROCK, Type::FLYING}, 40.0f, -1.0f, {60, 60, 100, 60, 100, 60});
const Species SPECIES_KOMALA(775, "Komala", {Type::NORMAL, Type::NONE}, 19.9f, 50.0f, {65, 115, 65, 75, 95, 65});
const Species SPECIES_TURTONATOR(776, "Turtonator", {Type::FIRE, Type::DRAGON}, 212.0f, 50.0f, {60, 78, 135, 91, 85, 36});
const Species SPECIES_TOGEDEMARU(777, "Togedemaru", {Type::ELECTRIC, Type::STEEL}, 3.3f, 50.0f, {65, 98, 63, 40, 73, 96});
const Species SPECIES_MIMIKYU(778, "Mimikyu", {Type::GHOST, Type::FAIRY}, 0.7f, 50.0f, {55, 90, 80, 50, 105, 96});
const Species SPECIES_BRUXISH(779, "Bruxish", {Type::WATER, Type::PSYCHIC}, 19.0f, 50.0f, {68, 105, 70, 70, 70, 92});
const Species SPECIES_DRAMPA(780, "Drampa", {Type::NORMAL, Type::DRAGON}, 185.0f, 50.0f, {78, 60, 85, 135, 91, 36});
const Species SPECIES_DHELMISE(781, "Dhelmise", {Type::GHOST, Type::GRASS}, 210.0f, -1.0f, {70, 131, 100, 86, 90, 40});
const Species SPECIES_JANGMO_O(782, "Jangmo-o", {Type::DRAGON, Type::NONE}, 29.7f, 50.0f, {45, 55, 65, 45, 45, 45});
const Species SPECIES_HAKAMO_O(783, "Hakamo-o", {Type::DRAGON, Type::FIGHTING}, 47.0f, 50.0f, {55, 75, 90, 65, 70, 65});
const Species SPECIES_KOMMO_O(784, "Kommo-o", {Type::DRAGON, Type::FIGHTING}, 78.2f, 50.0f, {75, 110, 125, 100, 105, 85});
const Species SPECIES_TAPU_KOKO(785, "Tapu Koko", {Type::ELECTRIC, Type::FAIRY}, 20.5f, -1.0f, {70, 115, 85, 95, 75, 130});
const Species SPECIES_TAPU_LELE(786, "Tapu Lele", {Type::PSYCHIC, Type::FAIRY}, 18.6f, -1.0f, {70, 85, 75, 130, 115, 95});
const Species SPECIES_TAPU_BULU(787, "Tapu Bulu", {Type::GRASS, Type::FAIRY}, 45.5f, -1.0f, {70, 130, 115, 85, 95, 75});
const Species SPECIES_TAPU_FINI(788, "Tapu Fini", {Type::WATER, Type::FAIRY}, 21.2f, -1.0f, {70, 75, 115, 95, 130, 85});
const Species SPECIES_COSMOG(789, "Cosmog", {Type::PSYCHIC, Type::NONE}, 0.1f, -1.0f, {43, 29, 31, 29, 31, 37});
const Species SPECIES_COSMOEM(790, "Cosmoem", {Type::PSYCHIC, Type::NONE}, 999.9f, -1.0f, {43, 29, 131, 29, 131, 37});
const Species SPECIES_SOLGALEO(791, "Solgaleo", {Type::PSYCHIC, Type::STEEL}, 230.0f, -1.0f, {137, 137, 107, 113, 89, 97});
const Species SPECIES_LUNALA(792, "Lunala", {Type::PSYCHIC, Type::GHOST}, 120.0f, -1.0f, {137, 113, 89, 137, 107, 97});
const Species SPECIES_NIHILEGO(793, "Nihilego", {Type::ROCK, Type::POISON}, 55.5f, -1.0f, {109, 53, 47, 127, 131, 103});
const Species SPECIES_BUZZWOLE(794, "Buzzwole", {Type::BUG, Type::FIGHTING}, 333.6f, -1.0f, {107, 139, 139, 53, 53, 79});
const Species SPECIES_PHEROMOSA(795, "Pheromosa", {Type::BUG, Type::FIGHTING}, 25.0f, -1.0f, {71, 137, 37, 137, 37, 151});
const Species SPECIES_XURKITREE(796, "Xurkitree", {Type::ELECTRIC, Type::NONE}, 100.0f, -1.0f, {83, 89, 71, 173, 71, 83});
const Species SPECIES_CELESTEELA(797, "Celesteela", {Type::STEEL, Type::FLYING}, 999.9f, -1.0f, {97, 101, 103, 107, 101, 61});
const Species SPECIES_KARTANA(798, "Kartana", {Type::GRASS, Type::STEEL}, 0.1f, -1.0f, {59, 181, 131, 59, 31, 109});
const Species SPECIES_GUZZLORD(799, "Guzzlord", {Type::DARK, Type::DRAGON}, 888.0f, -1.0f, {223, 101, 53, 97, 53, 43});
const Species SPECIES_NECROZMA(800, "Necrozma", {Type::PSYCHIC, Type::NONE}, 230.0f, -1.0f, {97, 107, 101, 127, 89, 79});
const Species SPECIES_MAGEARNA(801, "Magearna", {Type::STEEL, Type::FAIRY}, 80.5f, -1.0f, {80, 95, 115, 130, 115, 65});
const Species SPECIES_MARSHADOW(802, "Marshadow", {Type::FIGHTING, Type::GHOST}, 22.2f, -1.0f, {90, 125, 80, 90, 90, 125});
const Species SPECIES_POIPOLE(803, "Poipole", {Type::POISON, Type::NONE}, 1.8f, -1.0f, {67, 73, 67, 73, 67, 73});
const Species SPECIES_NAGANADEL(804, "Naganadel", {Type::POISON, Type::DRAGON}, 150.0f, -1.0f, {73, 73, 73, 127, 73, 121});
const Species SPECIES_STAKATAKA(805, "Stakataka", {Type::ROCK, Type::STEEL}, 820.0f, -1.0f, {61, 131, 211, 53, 101, 13});
const Species SPECIES_BLACEPHALON(806, "Blacephalon", {Type::FIRE, Type::GHOST}, 13.0f, -1.0f, {53, 127, 53, 151, 79, 107});
const Species SPECIES_ZERAORA(807, "Zeraora", {Type::ELECTRIC, Type::NONE}, 44.5f, -1.0f, {88, 112, 75, 102, 80, 143});
const Species SPECIES_MELTAN(808, "Meltan", {Type::STEEL, Type::NONE}, 8.0f, -1.0f, {46, 65, 65, 55, 35, 34});
const Species SPECIES_MELMETAL(809, "Melmetal", {Type::STEEL, Type::NONE}, 800.0f, -1.0f, {135, 143, 143, 80, 65, 34});

const std::unordered_map<std::string, const Species*> speciesMap = {
    {SPECIES_BULBASAUR.name, &SPECIES_BULBASAUR},
    {SPECIES_IVYSAUR.name, &SPECIES_IVYSAUR},
    {SPECIES_VENUSAUR.name, &SPECIES_VENUSAUR},
    {SPECIES_CHARMANDER.name, &SPECIES_CHARMANDER},
    {SPECIES_CHARMELEON.name, &SPECIES_CHARMELEON},
    {SPECIES_CHARIZARD.name, &SPECIES_CHARIZARD},
    {SPECIES_SQUIRTLE.name, &SPECIES_SQUIRTLE},
    {SPECIES_WARTORTLE.name, &SPECIES_WARTORTLE},
    {SPECIES_BLASTOISE.name, &SPECIES_BLASTOISE},
    {SPECIES_CATERPIE.name, &SPECIES_CATERPIE},
    {SPECIES_METAPOD.name, &SPECIES_METAPOD},
    {SPECIES_BUTTERFREE.name, &SPECIES_BUTTERFREE},
    {SPECIES_WEEDLE.name, &SPECIES_WEEDLE},
    {SPECIES_KAKUNA.name, &SPECIES_KAKUNA},
    {SPECIES_BEEDRILL.name, &SPECIES_BEEDRILL},
    {SPECIES_PIDGEY.name, &SPECIES_PIDGEY},
    {SPECIES_PIDGEOTTO.name, &SPECIES_PIDGEOTTO},
    {SPECIES_PIDGEOT.name, &SPECIES_PIDGEOT},
    {SPECIES_RATTATA.name, &SPECIES_RATTATA},
    {SPECIES_RATICATE.name, &SPECIES_RATICATE},
    {SPECIES_SPEAROW.name, &SPECIES_SPEAROW},
    {SPECIES_FEAROW.name, &SPECIES_FEAROW},
    {SPECIES_EKANS.name, &SPECIES_EKANS},
    {SPECIES_ARBOK.name, &SPECIES_ARBOK},
    {SPECIES_PIKACHU.name, &SPECIES_PIKACHU},
    {SPECIES_RAICHU.name, &SPECIES_RAICHU},
    {SPECIES_SANDSHREW.name, &SPECIES_SANDSHREW},
    {SPECIES_SANDSLASH.name, &SPECIES_SANDSLASH},
    {SPECIES_NIDORAN_FEMALE.name, &SPECIES_NIDORAN_FEMALE},
    {SPECIES_NIDORINA.name, &SPECIES_NIDORINA},
    {SPECIES_NIDOQUEEN.name, &SPECIES_NIDOQUEEN},
    {SPECIES_NIDORAN_MALE.name, &SPECIES_NIDORAN_MALE},
    {SPECIES_NIDORINO.name, &SPECIES_NIDORINO},
    {SPECIES_NIDOKING.name, &SPECIES_NIDOKING},
    {SPECIES_CLEFAIRY.name, &SPECIES_CLEFAIRY},
    {SPECIES_CLEFABLE.name, &SPECIES_CLEFABLE},
    {SPECIES_VULPIX.name, &SPECIES_VULPIX},
    {SPECIES_NINETALES.name, &SPECIES_NINETALES},
    {SPECIES_JIGGLYPUFF.name, &SPECIES_JIGGLYPUFF},
    {SPECIES_WIGGLYTUFF.name, &SPECIES_WIGGLYTUFF},
    {SPECIES_ZUBAT.name, &SPECIES_ZUBAT},
    {SPECIES_GOLBAT.name, &SPECIES_GOLBAT},
    {SPECIES_ODDISH.name, &SPECIES_ODDISH},
    {SPECIES_GLOOM.name, &SPECIES_GLOOM},
    {SPECIES_VILEPLUME.name, &SPECIES_VILEPLUME},
    {SPECIES_PARAS.name, &SPECIES_PARAS},
    {SPECIES_PARASECT.name, &SPECIES_PARASECT},
    {SPECIES_VENONAT.name, &SPECIES_VENONAT},
    {SPECIES_VENOMOTH.name, &SPECIES_VENOMOTH},
    {SPECIES_DIGLETT.name, &SPECIES_DIGLETT},
    {SPECIES_DUGTRIO.name, &SPECIES_DUGTRIO},
    {SPECIES_MEOWTH.name, &SPECIES_MEOWTH},
    {SPECIES_PERSIAN.name, &SPECIES_PERSIAN},
    {SPECIES_PSYDUCK.name, &SPECIES_PSYDUCK},
    {SPECIES_GOLDUCK.name, &SPECIES_GOLDUCK},
    {SPECIES_MANKEY.name, &SPECIES_MANKEY},
    {SPECIES_PRIMEAPE.name, &SPECIES_PRIMEAPE},
    {SPECIES_GROWLITHE.name, &SPECIES_GROWLITHE},
    {SPECIES_ARCANINE.name, &SPECIES_ARCANINE},
    {SPECIES_POLIWAG.name, &SPECIES_POLIWAG},
    {SPECIES_POLIWHIRL.name, &SPECIES_POLIWHIRL},
    {SPECIES_POLIWRATH.name, &SPECIES_POLIWRATH},
    {SPECIES_ABRA.name, &SPECIES_ABRA},
    {SPECIES_KADABRA.name, &SPECIES_KADABRA},
    {SPECIES_ALAKAZAM.name, &SPECIES_ALAKAZAM},
    {SPECIES_MACHOP.name, &SPECIES_MACHOP},
    {SPECIES_MACHOKE.name, &SPECIES_MACHOKE},
    {SPECIES_MACHAMP.name, &SPECIES_MACHAMP},
    {SPECIES_BELLSPROUT.name, &SPECIES_BELLSPROUT},
    {SPECIES_WEEPINBELL.name, &SPECIES_WEEPINBELL},
    {SPECIES_VICTREEBEL.name, &SPECIES_VICTREEBEL},
    {SPECIES_TENTACOOL.name, &SPECIES_TENTACOOL},
    {SPECIES_TENTACRUEL.name, &SPECIES_TENTACRUEL},
    {SPECIES_GEODUDE.name, &SPECIES_GEODUDE},
    {SPECIES_GRAVELER.name, &SPECIES_GRAVELER},
    {SPECIES_GOLEM.name, &SPECIES_GOLEM},
    {SPECIES_PONYTA.name, &SPECIES_PONYTA},
    {SPECIES_RAPIDASH.name, &SPECIES_RAPIDASH},
    {SPECIES_SLOWPOKE.name, &SPECIES_SLOWPOKE},
    {SPECIES_SLOWBRO.name, &SPECIES_SLOWBRO},
    {SPECIES_MAGNEMITE.name, &SPECIES_MAGNEMITE},
    {SPECIES_MAGNETON.name, &SPECIES_MAGNETON},
    {SPECIES_FARFETCHD.name, &SPECIES_FARFETCHD},
    {SPECIES_DODUO.name, &SPECIES_DODUO},
    {SPECIES_DODRIO.name, &SPECIES_DODRIO},
    {SPECIES_SEEL.name, &SPECIES_SEEL},
    {SPECIES_DEWGONG.name, &SPECIES_DEWGONG},
    {SPECIES_GRIMER.name, &SPECIES_GRIMER},
    {SPECIES_MUK.name, &SPECIES_MUK},
    {SPECIES_SHELLDER.name, &SPECIES_SHELLDER},
    {SPECIES_CLOYSTER.name, &SPECIES_CLOYSTER},
    {SPECIES_GASTLY.name, &SPECIES_GASTLY},
    {SPECIES_HAUNTER.name, &SPECIES_HAUNTER},
    {SPECIES_GENGAR.name, &SPECIES_GENGAR},
    {SPECIES_ONIX.name, &SPECIES_ONIX},
    {SPECIES_DROWZEE.name, &SPECIES_DROWZEE},
    {SPECIES_HYPNO.name, &SPECIES_HYPNO},
    {SPECIES_KRABBY.name, &SPECIES_KRABBY},
    {SPECIES_KINGLER.name, &SPECIES_KINGLER},
    {SPECIES_VOLTORB.name, &SPECIES_VOLTORB},
    {SPECIES_ELECTRODE.name, &SPECIES_ELECTRODE},
    {SPECIES_EXEGGCUTE.name, &SPECIES_EXEGGCUTE},
    {SPECIES_EXEGGUTOR.name, &SPECIES_EXEGGUTOR},
    {SPECIES_CUBONE.name, &SPECIES_CUBONE},
    {SPECIES_MAROWAK.name, &SPECIES_MAROWAK},
    {SPECIES_HITMONLEE.name, &SPECIES_HITMONLEE},
    {SPECIES_HITMONCHAN.name, &SPECIES_HITMONCHAN},
    {SPECIES_LICKITUNG.name, &SPECIES_LICKITUNG},
    {SPECIES_KOFFING.name, &SPECIES_KOFFING},
    {SPECIES_WEEZING.name, &SPECIES_WEEZING},
    {SPECIES_RHYHORN.name, &SPECIES_RHYHORN},
    {SPECIES_RHYDON.name, &SPECIES_RHYDON},
    {SPECIES_CHANSEY.name, &SPECIES_CHANSEY},
    {SPECIES_TANGELA.name, &SPECIES_TANGELA},
    {SPECIES_KANGASKHAN.name, &SPECIES_KANGASKHAN},
    {SPECIES_HORSEA.name, &SPECIES_HORSEA},
    {SPECIES_SEADRA.name, &SPECIES_SEADRA},
    {SPECIES_GOLDEEN.name, &SPECIES_GOLDEEN},
    {SPECIES_SEAKING.name, &SPECIES_SEAKING},
    {SPECIES_STARYU.name, &SPECIES_STARYU},
    {SPECIES_STARMIE.name, &SPECIES_STARMIE},
    {SPECIES_MR_MIME.name, &SPECIES_MR_MIME},
    {SPECIES_SCYTHER.name, &SPECIES_SCYTHER},
    {SPECIES_JYNX.name, &SPECIES_JYNX},
    {SPECIES_ELECTABUZZ.name, &SPECIES_ELECTABUZZ},
    {SPECIES_MAGMAR.name, &SPECIES_MAGMAR},
    {SPECIES_PINSIR.name, &SPECIES_PINSIR},
    {SPECIES_TAUROS.name, &SPECIES_TAUROS},
    {SPECIES_MAGIKARP.name, &SPECIES_MAGIKARP},
    {SPECIES_GYARADOS.name, &SPECIES_GYARADOS},
    {SPECIES_LAPRAS.name, &SPECIES_LAPRAS},
    {SPECIES_DITTO.name, &SPECIES_DITTO},
    {SPECIES_EEVEE.name, &SPECIES_EEVEE},
    {SPECIES_VAPOREON.name, &SPECIES_VAPOREON},
    {SPECIES_JOLTEON.name, &SPECIES_JOLTEON},
    {SPECIES_FLAREON.name, &SPECIES_FLAREON},
    {SPECIES_PORYGON.name, &SPECIES_PORYGON},
    {SPECIES_OMANYTE.name, &SPECIES_OMANYTE},
    {SPECIES_OMASTAR.name, &SPECIES_OMASTAR},
    {SPECIES_KABUTO.name, &SPECIES_KABUTO},
    {SPECIES_KABUTOPS.name, &SPECIES_KABUTOPS},
    {SPECIES_AERODACTYL.name, &SPECIES_AERODACTYL},
    {SPECIES_SNORLAX.name, &SPECIES_SNORLAX},
    {SPECIES_ARTICUNO.name, &SPECIES_ARTICUNO},
    {SPECIES_ZAPDOS.name, &SPECIES_ZAPDOS},
    {SPECIES_MOLTRES.name, &SPECIES_MOLTRES},
    {SPECIES_DRATINI.name, &SPECIES_DRATINI},
    {SPECIES_DRAGONAIR.name, &SPECIES_DRAGONAIR},
    {SPECIES_DRAGONITE.name, &SPECIES_DRAGONITE},
    {SPECIES_MEWTWO.name, &SPECIES_MEWTWO},
    {SPECIES_MEW.name, &SPECIES_MEW},
    {SPECIES_CHIKORITA.name, &SPECIES_CHIKORITA},
    {SPECIES_BAYLEEF.name, &SPECIES_BAYLEEF},
    {SPECIES_MEGANIUM.name, &SPECIES_MEGANIUM},
    {SPECIES_CYNDAQUIL.name, &SPECIES_CYNDAQUIL},
    {SPECIES_QUILAVA.name, &SPECIES_QUILAVA},
    {SPECIES_TYPHLOSION.name, &SPECIES_TYPHLOSION},
    {SPECIES_TOTODILE.name, &SPECIES_TOTODILE},
    {SPECIES_CROCONAW.name, &SPECIES_CROCONAW},
    {SPECIES_FERALIGATR.name, &SPECIES_FERALIGATR},
    {SPECIES_SENTRET.name, &SPECIES_SENTRET},
    {SPECIES_FURRET.name, &SPECIES_FURRET},
    {SPECIES_HOOTHOOT.name, &SPECIES_HOOTHOOT},
    {SPECIES_NOCTOWL.name, &SPECIES_NOCTOWL},
    {SPECIES_LEDYBA.name, &SPECIES_LEDYBA},
    {SPECIES_LEDIAN.name, &SPECIES_LEDIAN},
    {SPECIES_SPINARAK.name, &SPECIES_SPINARAK},
    {SPECIES_ARIADOS.name, &SPECIES_ARIADOS},
    {SPECIES_CROBAT.name, &SPECIES_CROBAT},
    {SPECIES_CHINCHOU.name, &SPECIES_CHINCHOU},
    {SPECIES_LANTURN.name, &SPECIES_LANTURN},
    {SPECIES_PICHU.name, &SPECIES_PICHU},
    {SPECIES_CLEFFA.name, &SPECIES_CLEFFA},
    {SPECIES_IGGLYBUFF.name, &SPECIES_IGGLYBUFF},
    {SPECIES_TOGEPI.name, &SPECIES_TOGEPI},
    {SPECIES_TOGETIC.name, &SPECIES_TOGETIC},
    {SPECIES_NATU.name, &SPECIES_NATU},
    {SPECIES_XATU.name, &SPECIES_XATU},
    {SPECIES_MAREEP.name, &SPECIES_MAREEP},
    {SPECIES_FLAAFFY.name, &SPECIES_FLAAFFY},
    {SPECIES_AMPHAROS.name, &SPECIES_AMPHAROS},
    {SPECIES_BELLOSSOM.name, &SPECIES_BELLOSSOM},
    {SPECIES_MARILL.name, &SPECIES_MARILL},
    {SPECIES_AZUMARILL.name, &SPECIES_AZUMARILL},
    {SPECIES_SUDOWOODO.name, &SPECIES_SUDOWOODO},
    {SPECIES_POLITOED.name, &SPECIES_POLITOED},
    {SPECIES_HOPPIP.name, &SPECIES_HOPPIP},
    {SPECIES_SKIPLOOM.name, &SPECIES_SKIPLOOM},
    {SPECIES_JUMPLUFF.name, &SPECIES_JUMPLUFF},
    {SPECIES_AIPOM.name, &SPECIES_AIPOM},
    {SPECIES_SUNKERN.name, &SPECIES_SUNKERN},
    {SPECIES_SUNFLORA.name, &SPECIES_SUNFLORA},
    {SPECIES_YANMA.name, &SPECIES_YANMA},
    {SPECIES_WOOPER.name, &SPECIES_WOOPER},
    {SPECIES_QUAGSIRE.name, &SPECIES_QUAGSIRE},
    {SPECIES_ESPEON.name, &SPECIES_ESPEON},
    {SPECIES_UMBREON.name, &SPECIES_UMBREON},
    {SPECIES_MURKROW.name, &SPECIES_MURKROW},
    {SPECIES_SLOWKING.name, &SPECIES_SLOWKING},
    {SPECIES_MISDREAVUS.name, &SPECIES_MISDREAVUS},
    {SPECIES_UNOWN.name, &SPECIES_UNOWN},
    {SPECIES_WOBBUFFET.name, &SPECIES_WOBBUFFET},
    {SPECIES_GIRAFARIG.name, &SPECIES_GIRAFARIG},
    {SPECIES_PINECO.name, &SPECIES_PINECO},
    {SPECIES_FORRETRESS.name, &SPECIES_FORRETRESS},
    {SPECIES_DUNSPARCE.name, &SPECIES_DUNSPARCE},
    {SPECIES_GLIGAR.name, &SPECIES_GLIGAR},
    {SPECIES_STEELIX.name, &SPECIES_STEELIX},
    {SPECIES_SNUBBULL.name, &SPECIES_SNUBBULL},
    {SPECIES_GRANBULL.name, &SPECIES_GRANBULL},
    {SPECIES_QWILFISH.name, &SPECIES_QWILFISH},
    {SPECIES_SCIZOR.name, &SPECIES_SCIZOR},
    {SPECIES_SHUCKLE.name, &SPECIES_SHUCKLE},
    {SPECIES_HERACROSS.name, &SPECIES_HERACROSS},
    {SPECIES_SNEASEL.name, &SPECIES_SNEASEL},
    {SPECIES_TEDDIURSA.name, &SPECIES_TEDDIURSA},
    {SPECIES_URSARING.name, &SPECIES_URSARING},
    {SPECIES_SLUGMA.name, &SPECIES_SLUGMA},
    {SPECIES_MAGCARGO.name, &SPECIES_MAGCARGO},
    {SPECIES_SWINUB.name, &SPECIES_SWINUB},
    {SPECIES_PILOSWINE.name, &SPECIES_PILOSWINE},
    {SPECIES_CORSOLA.name, &SPECIES_CORSOLA},
    {SPECIES_REMORAID.name, &SPECIES_REMORAID},
    {SPECIES_OCTILLERY.name, &SPECIES_OCTILLERY},
    {SPECIES_DELIBIRD.name, &SPECIES_DELIBIRD},
    {SPECIES_MANTINE.name, &SPECIES_MANTINE},
    {SPECIES_SKARMORY.name, &SPECIES_SKARMORY},
    {SPECIES_HOUNDOUR.name, &SPECIES_HOUNDOUR},
    {SPECIES_HOUNDOOM.name, &SPECIES_HOUNDOOM},
    {SPECIES_KINGDRA.name, &SPECIES_KINGDRA},
    {SPECIES_PHANPY.name, &SPECIES_PHANPY},
    {SPECIES_DONPHAN.name, &SPECIES_DONPHAN},
    {SPECIES_PORYGON2.name, &SPECIES_PORYGON2},
    {SPECIES_STANTLER.name, &SPECIES_STANTLER},
    {SPECIES_SMEARGLE.name, &SPECIES_SMEARGLE},
    {SPECIES_TYROGUE.name, &SPECIES_TYROGUE},
    {SPECIES_HITMONTOP.name, &SPECIES_HITMONTOP},
    {SPECIES_SMOOCHUM.name, &SPECIES_SMOOCHUM},
    {SPECIES_ELEKID.name, &SPECIES_ELEKID},
    {SPECIES_MAGBY.name, &SPECIES_MAGBY},
    {SPECIES_MILTANK.name, &SPECIES_MILTANK},
    {SPECIES_BLISSEY.name, &SPECIES_BLISSEY},
    {SPECIES_RAIKOU.name, &SPECIES_RAIKOU},
    {SPECIES_ENTEI.name, &SPECIES_ENTEI},
    {SPECIES_SUICUNE.name, &SPECIES_SUICUNE},
    {SPECIES_LARVITAR.name, &SPECIES_LARVITAR},
    {SPECIES_PUPITAR.name, &SPECIES_PUPITAR},
    {SPECIES_TYRANITAR.name, &SPECIES_TYRANITAR},
    {SPECIES_LUGIA.name, &SPECIES_LUGIA},
    {SPECIES_HO_OH.name, &SPECIES_HO_OH},
    {SPECIES_CELEBI.name, &SPECIES_CELEBI},
    {SPECIES_TREECKO.name, &SPECIES_TREECKO},
    {SPECIES_GROVYLE.name, &SPECIES_GROVYLE},
    {SPECIES_SCEPTILE.name, &SPECIES_SCEPTILE},
    {SPECIES_TORCHIC.name, &SPECIES_TORCHIC},
    {SPECIES_COMBUSKEN.name, &SPECIES_COMBUSKEN},
    {SPECIES_BLAZIKEN.name, &SPECIES_BLAZIKEN},
    {SPECIES_MUDKIP.name, &SPECIES_MUDKIP},
    {SPECIES_MARSHTOMP.name, &SPECIES_MARSHTOMP},
    {SPECIES_SWAMPERT.name, &SPECIES_SWAMPERT},
    {SPECIES_POOCHYENA.name, &SPECIES_POOCHYENA},
    {SPECIES_MIGHTYENA.name, &SPECIES_MIGHTYENA},
    {SPECIES_ZIGZAGOON.name, &SPECIES_ZIGZAGOON},
    {SPECIES_LINOONE.name, &SPECIES_LINOONE},
    {SPECIES_WURMPLE.name, &SPECIES_WURMPLE},
    {SPECIES_SILCOON.name, &SPECIES_SILCOON},
    {SPECIES_BEAUTIFLY.name, &SPECIES_BEAUTIFLY},
    {SPECIES_CASCOON.name, &SPECIES_CASCOON},
    {SPECIES_DUSTOX.name, &SPECIES_DUSTOX},
    {SPECIES_LOTAD.name, &SPECIES_LOTAD},
    {SPECIES_LOMBRE.name, &SPECIES_LOMBRE},
    {SPECIES_LUDICOLO.name, &SPECIES_LUDICOLO},
    {SPECIES_SEEDOT.name, &SPECIES_SEEDOT},
    {SPECIES_NUZLEAF.name, &SPECIES_NUZLEAF},
    {SPECIES_SHIFTRY.name, &SPECIES_SHIFTRY},
    {SPECIES_TAILLOW.name, &SPECIES_TAILLOW},
    {SPECIES_SWELLOW.name, &SPECIES_SWELLOW},
    {SPECIES_WINGULL.name, &SPECIES_WINGULL},
    {SPECIES_PELIPPER.name, &SPECIES_PELIPPER},
    {SPECIES_RALTS.name, &SPECIES_RALTS},
    {SPECIES_KIRLIA.name, &SPECIES_KIRLIA},
    {SPECIES_GARDEVOIR.name, &SPECIES_GARDEVOIR},
    {SPECIES_SURSKIT.name, &SPECIES_SURSKIT},
    {SPECIES_MASQUERAIN.name, &SPECIES_MASQUERAIN},
    {SPECIES_SHROOMISH.name, &SPECIES_SHROOMISH},
    {SPECIES_BRELOOM.name, &SPECIES_BRELOOM},
    {SPECIES_SLAKOTH.name, &SPECIES_SLAKOTH},
    {SPECIES_VIGOROTH.name, &SPECIES_VIGOROTH},
    {SPECIES_SLAKING.name, &SPECIES_SLAKING},
    {SPECIES_NINCADA.name, &SPECIES_NINCADA},
    {SPECIES_NINJASK.name, &SPECIES_NINJASK},
    {SPECIES_SHEDINJA.name, &SPECIES_SHEDINJA},
    {SPECIES_WHISMUR.name, &SPECIES_WHISMUR},
    {SPECIES_LOUDRED.name, &SPECIES_LOUDRED},
    {SPECIES_EXPLOUD.name, &SPECIES_EXPLOUD},
    {SPECIES_MAKUHITA.name, &SPECIES_MAKUHITA},
    {SPECIES_HARIYAMA.name, &SPECIES_HARIYAMA},
    {SPECIES_AZURILL.name, &SPECIES_AZURILL},
    {SPECIES_NOSEPASS.name, &SPECIES_NOSEPASS},
    {SPECIES_SKITTY.name, &SPECIES_SKITTY},
    {SPECIES_DELCATTY.name, &SPECIES_DELCATTY},
    {SPECIES_SABLEYE.name, &SPECIES_SABLEYE},
    {SPECIES_MAWILE.name, &SPECIES_MAWILE},
    {SPECIES_ARON.name, &SPECIES_ARON},
    {SPECIES_LAIRON.name, &SPECIES_LAIRON},
    {SPECIES_AGGRON.name, &SPECIES_AGGRON},
    {SPECIES_MEDITITE.name, &SPECIES_MEDITITE},
    {SPECIES_MEDICHAM.name, &SPECIES_MEDICHAM},
    {SPECIES_ELECTRIKE.name, &SPECIES_ELECTRIKE},
    {SPECIES_MANECTRIC.name, &SPECIES_MANECTRIC},
    {SPECIES_PLUSLE.name, &SPECIES_PLUSLE},
    {SPECIES_MINUN.name, &SPECIES_MINUN},
    {SPECIES_VOLBEAT.name, &SPECIES_VOLBEAT},
    {SPECIES_ILLUMISE.name, &SPECIES_ILLUMISE},
    {SPECIES_ROSELIA.name, &SPECIES_ROSELIA},
    {SPECIES_GULPIN.name, &SPECIES_GULPIN},
    {SPECIES_SWALOT.name, &SPECIES_SWALOT},
    {SPECIES_CARVANHA.name, &SPECIES_CARVANHA},
    {SPECIES_SHARPEDO.name, &SPECIES_SHARPEDO},
    {SPECIES_WAILMER.name, &SPECIES_WAILMER},
    {SPECIES_WAILORD.name, &SPECIES_WAILORD},
    {SPECIES_NUMEL.name, &SPECIES_NUMEL},
    {SPECIES_CAMERUPT.name, &SPECIES_CAMERUPT},
    {SPECIES_TORKOAL.name, &SPECIES_TORKOAL},
    {SPECIES_SPOINK.name, &SPECIES_SPOINK},
    {SPECIES_GRUMPIG.name, &SPECIES_GRUMPIG},
    {SPECIES_SPINDA.name, &SPECIES_SPINDA},
    {SPECIES_TRAPINCH.name, &SPECIES_TRAPINCH},
    {SPECIES_VIBRAVA.name, &SPECIES_VIBRAVA},
    {SPECIES_FLYGON.name, &SPECIES_FLYGON},
    {SPECIES_CACNEA.name, &SPECIES_CACNEA},
    {SPECIES_CACTURNE.name, &SPECIES_CACTURNE},
    {SPECIES_SWABLU.name, &SPECIES_SWABLU},
    {SPECIES_ALTARIA.name, &SPECIES_ALTARIA},
    {SPECIES_ZANGOOSE.name, &SPECIES_ZANGOOSE},
    {SPECIES_SEVIPER.name, &SPECIES_SEVIPER},
    {SPECIES_LUNATONE.name, &SPECIES_LUNATONE},
    {SPECIES_SOLROCK.name, &SPECIES_SOLROCK},
    {SPECIES_BARBOACH.name, &SPECIES_BARBOACH},
    {SPECIES_WHISCASH.name, &SPECIES_WHISCASH},
    {SPECIES_CORPHISH.name, &SPECIES_CORPHISH},
    {SPECIES_CRAWDAUNT.name, &SPECIES_CRAWDAUNT},
    {SPECIES_BALTOY.name, &SPECIES_BALTOY},
    {SPECIES_CLAYDOL.name, &SPECIES_CLAYDOL},
    {SPECIES_LILEEP.name, &SPECIES_LILEEP},
    {SPECIES_CRADILY.name, &SPECIES_CRADILY},
    {SPECIES_ANORITH.name, &SPECIES_ANORITH},
    {SPECIES_ARMALDO.name, &SPECIES_ARMALDO},
    {SPECIES_FEEBAS.name, &SPECIES_FEEBAS},
    {SPECIES_MILOTIC.name, &SPECIES_MILOTIC},
    {SPECIES_CASTFORM.name, &SPECIES_CASTFORM},
    {SPECIES_KECLEON.name, &SPECIES_KECLEON},
    {SPECIES_SHUPPET.name, &SPECIES_SHUPPET},
    {SPECIES_BANETTE.name, &SPECIES_BANETTE},
    {SPECIES_DUSKULL.name, &SPECIES_DUSKULL},
    {SPECIES_DUSCLOPS.name, &SPECIES_DUSCLOPS},
    {SPECIES_TROPIUS.name, &SPECIES_TROPIUS},
    {SPECIES_CHIMECHO.name, &SPECIES_CHIMECHO},
    {SPECIES_ABSOL.name, &SPECIES_ABSOL},
    {SPECIES_WYNAUT.name, &SPECIES_WYNAUT},
    {SPECIES_SNORUNT.name, &SPECIES_SNORUNT},
    {SPECIES_GLALIE.name, &SPECIES_GLALIE},
    {SPECIES_SPHEAL.name, &SPECIES_SPHEAL},
    {SPECIES_SEALEO.name, &SPECIES_SEALEO},
    {SPECIES_WALREIN.name, &SPECIES_WALREIN},
    {SPECIES_CLAMPERL.name, &SPECIES_CLAMPERL},
    {SPECIES_HUNTAIL.name, &SPECIES_HUNTAIL},
    {SPECIES_GOREBYSS.name, &SPECIES_GOREBYSS},
    {SPECIES_RELICANTH.name, &SPECIES_RELICANTH},
    {SPECIES_LUVDISC.name, &SPECIES_LUVDISC},
    {SPECIES_BAGON.name, &SPECIES_BAGON},
    {SPECIES_SHELGON.name, &SPECIES_SHELGON},
    {SPECIES_SALAMENCE.name, &SPECIES_SALAMENCE},
    {SPECIES_BELDUM.name, &SPECIES_BELDUM},
    {SPECIES_METANG.name, &SPECIES_METANG},
    {SPECIES_METAGROSS.name, &SPECIES_METAGROSS},
    {SPECIES_REGIROCK.name, &SPECIES_REGIROCK},
    {SPECIES_REGICE.name, &SPECIES_REGICE},
    {SPECIES_REGISTEEL.name, &SPECIES_REGISTEEL},
    {SPECIES_LATIAS.name, &SPECIES_LATIAS},
    {SPECIES_LATIOS.name, &SPECIES_LATIOS},
    {SPECIES_KYOGRE.name, &SPECIES_KYOGRE},
    {SPECIES_GROUDON.name, &SPECIES_GROUDON},
    {SPECIES_RAYQUAZA.name, &SPECIES_RAYQUAZA},
    {SPECIES_JIRACHI.name, &SPECIES_JIRACHI},
    {SPECIES_DEOXYS.name, &SPECIES_DEOXYS},
    {SPECIES_TURTWIG.name, &SPECIES_TURTWIG},
    {SPECIES_GROTLE.name, &SPECIES_GROTLE},
    {SPECIES_TORTERRA.name, &SPECIES_TORTERRA},
    {SPECIES_CHIMCHAR.name, &SPECIES_CHIMCHAR},
    {SPECIES_MONFERNO.name, &SPECIES_MONFERNO},
    {SPECIES_INFERNAPE.name, &SPECIES_INFERNAPE},
    {SPECIES_PIPLUP.name, &SPECIES_PIPLUP},
    {SPECIES_PRINPLUP.name, &SPECIES_PRINPLUP},
    {SPECIES_EMPOLEON.name, &SPECIES_EMPOLEON},
    {SPECIES_STARLY.name, &SPECIES_STARLY},
    {SPECIES_STARAVIA.name, &SPECIES_STARAVIA},
    {SPECIES_STARAPTOR.name, &SPECIES_STARAPTOR},
    {SPECIES_BIDOOF.name, &SPECIES_BIDOOF},
    {SPECIES_BIBAREL.name, &SPECIES_BIBAREL},
    {SPECIES_KRICKETOT.name, &SPECIES_KRICKETOT},
    {SPECIES_KRICKETUNE.name, &SPECIES_KRICKETUNE},
    {SPECIES_SHINX.name, &SPECIES_SHINX},
    {SPECIES_LUXIO.name, &SPECIES_LUXIO},
    {SPECIES_LUXRAY.name, &SPECIES_LUXRAY},
    {SPECIES_BUDEW.name, &SPECIES_BUDEW},
    {SPECIES_ROSERADE.name, &SPECIES_ROSERADE},
    {SPECIES_CRANIDOS.name, &SPECIES_CRANIDOS},
    {SPECIES_RAMPARDOS.name, &SPECIES_RAMPARDOS},
    {SPECIES_SHIELDON.name, &SPECIES_SHIELDON},
    {SPECIES_BASTIODON.name, &SPECIES_BASTIODON},
    {SPECIES_BURMY.name, &SPECIES_BURMY},
    {SPECIES_WORMADAM.name, &SPECIES_WORMADAM},
    {SPECIES_MOTHIM.name, &SPECIES_MOTHIM},
    {SPECIES_COMBEE.name, &SPECIES_COMBEE},
    {SPECIES_VESPIQUEN.name, &SPECIES_VESPIQUEN},
    {SPECIES_PACHIRISU.name, &SPECIES_PACHIRISU},
    {SPECIES_BUIZEL.name, &SPECIES_BUIZEL},
    {SPECIES_FLOATZEL.name, &SPECIES_FLOATZEL},
    {SPECIES_CHERUBI.name, &SPECIES_CHERUBI},
    {SPECIES_CHERRIM.name, &SPECIES_CHERRIM},
    {SPECIES_SHELLOS.name, &SPECIES_SHELLOS},
    {SPECIES_GASTRODON.name, &SPECIES_GASTRODON},
    {SPECIES_AMBIPOM.name, &SPECIES_AMBIPOM},
    {SPECIES_DRIFLOON.name, &SPECIES_DRIFLOON},
    {SPECIES_DRIFBLIM.name, &SPECIES_DRIFBLIM},
    {SPECIES_BUNEARY.name, &SPECIES_BUNEARY},
    {SPECIES_LOPUNNY.name, &SPECIES_LOPUNNY},
    {SPECIES_MISMAGIUS.name, &SPECIES_MISMAGIUS},
    {SPECIES_HONCHKROW.name, &SPECIES_HONCHKROW},
    {SPECIES_GLAMEOW.name, &SPECIES_GLAMEOW},
    {SPECIES_PURUGLY.name, &SPECIES_PURUGLY},
    {SPECIES_CHINGLING.name, &SPECIES_CHINGLING},
    {SPECIES_STUNKY.name, &SPECIES_STUNKY},
    {SPECIES_SKUNTANK.name, &SPECIES_SKUNTANK},
    {SPECIES_BRONZOR.name, &SPECIES_BRONZOR},
    {SPECIES_BRONZONG.name, &SPECIES_BRONZONG},
    {SPECIES_BONSLY.name, &SPECIES_BONSLY},
    {SPECIES_MIME_JR.name, &SPECIES_MIME_JR},
    {SPECIES_HAPPINY.name, &SPECIES_HAPPINY},
    {SPECIES_CHATOT.name, &SPECIES_CHATOT},
    {SPECIES_SPIRITOMB.name, &SPECIES_SPIRITOMB},
    {SPECIES_GIBLE.name, &SPECIES_GIBLE},
    {SPECIES_GABITE.name, &SPECIES_GABITE},
    {SPECIES_GARCHOMP.name, &SPECIES_GARCHOMP},
    {SPECIES_MUNCHLAX.name, &SPECIES_MUNCHLAX},
    {SPECIES_RIOLU.name, &SPECIES_RIOLU},
    {SPECIES_LUCARIO.name, &SPECIES_LUCARIO},
    {SPECIES_HIPPOPOTAS.name, &SPECIES_HIPPOPOTAS},
    {SPECIES_HIPPOWDON.name, &SPECIES_HIPPOWDON},
    {SPECIES_SKORUPI.name, &SPECIES_SKORUPI},
    {SPECIES_DRAPION.name, &SPECIES_DRAPION},
    {SPECIES_CROAGUNK.name, &SPECIES_CROAGUNK},
    {SPECIES_TOXICROAK.name, &SPECIES_TOXICROAK},
    {SPECIES_CARNIVINE.name, &SPECIES_CARNIVINE},
    {SPECIES_FINNEON.name, &SPECIES_FINNEON},
    {SPECIES_LUMINEON.name, &SPECIES_LUMINEON},
    {SPECIES_MANTYKE.name, &SPECIES_MANTYKE},
    {SPECIES_SNOVER.name, &SPECIES_SNOVER},
    {SPECIES_ABOMASNOW.name, &SPECIES_ABOMASNOW},
    {SPECIES_WEAVILE.name, &SPECIES_WEAVILE},
    {SPECIES_MAGNEZONE.name, &SPECIES_MAGNEZONE},
    {SPECIES_LICKILICKY.name, &SPECIES_LICKILICKY},
    {SPECIES_RHYPERIOR.name, &SPECIES_RHYPERIOR},
    {SPECIES_TANGROWTH.name, &SPECIES_TANGROWTH},
    {SPECIES_ELECTIVIRE.name, &SPECIES_ELECTIVIRE},
    {SPECIES_MAGMORTAR.name, &SPECIES_MAGMORTAR},
    {SPECIES_TOGEKISS.name, &SPECIES_TOGEKISS},
    {SPECIES_YANMEGA.name, &SPECIES_YANMEGA},
    {SPECIES_LEAFEON.name, &SPECIES_LEAFEON},
    {SPECIES_GLACEON.name, &SPECIES_GLACEON},
    {SPECIES_GLISCOR.name, &SPECIES_GLISCOR},
    {SPECIES_MAMOSWINE.name, &SPECIES_MAMOSWINE},
    {SPECIES_PORYGON_Z.name, &SPECIES_PORYGON_Z},
    {SPECIES_GALLADE.name, &SPECIES_GALLADE},
    {SPECIES_PROBOPASS.name, &SPECIES_PROBOPASS},
    {SPECIES_DUSKNOIR.name, &SPECIES_DUSKNOIR},
    {SPECIES_FROSLASS.name, &SPECIES_FROSLASS},
    {SPECIES_ROTOM.name, &SPECIES_ROTOM},
    {SPECIES_UXIE.name, &SPECIES_UXIE},
    {SPECIES_MESPRIT.name, &SPECIES_MESPRIT},
    {SPECIES_AZELF.name, &SPECIES_AZELF},
    {SPECIES_DIALGA.name, &SPECIES_DIALGA},
    {SPECIES_PALKIA.name, &SPECIES_PALKIA},
    {SPECIES_HEATRAN.name, &SPECIES_HEATRAN},
    {SPECIES_REGIGIGAS.name, &SPECIES_REGIGIGAS},
    {SPECIES_GIRATINA.name, &SPECIES_GIRATINA},
    {SPECIES_CRESSELIA.name, &SPECIES_CRESSELIA},
    {SPECIES_PHIONE.name, &SPECIES_PHIONE},
    {SPECIES_MANAPHY.name, &SPECIES_MANAPHY},
    {SPECIES_DARKRAI.name, &SPECIES_DARKRAI},
    {SPECIES_SHAYMIN.name, &SPECIES_SHAYMIN},
    {SPECIES_ARCEUS.name, &SPECIES_ARCEUS},
    {SPECIES_VICTINI.name, &SPECIES_VICTINI},
    {SPECIES_SNIVY.name, &SPECIES_SNIVY},
    {SPECIES_SERVINE.name, &SPECIES_SERVINE},
    {SPECIES_SERPERIOR.name, &SPECIES_SERPERIOR},
    {SPECIES_TEPIG.name, &SPECIES_TEPIG},
    {SPECIES_PIGNITE.name, &SPECIES_PIGNITE},
    {SPECIES_EMBOAR.name, &SPECIES_EMBOAR},
    {SPECIES_OSHAWOTT.name, &SPECIES_OSHAWOTT},
    {SPECIES_DEWOTT.name, &SPECIES_DEWOTT},
    {SPECIES_SAMUROTT.name, &SPECIES_SAMUROTT},
    {SPECIES_PATRAT.name, &SPECIES_PATRAT},
    {SPECIES_WATCHOG.name, &SPECIES_WATCHOG},
    {SPECIES_LILLIPUP.name, &SPECIES_LILLIPUP},
    {SPECIES_HERDIER.name, &SPECIES_HERDIER},
    {SPECIES_STOUTLAND.name, &SPECIES_STOUTLAND},
    {SPECIES_PURRLOIN.name, &SPECIES_PURRLOIN},
    {SPECIES_LIEPARD.name, &SPECIES_LIEPARD},
    {SPECIES_PANSAGE.name, &SPECIES_PANSAGE},
    {SPECIES_SIMISAGE.name, &SPECIES_SIMISAGE},
    {SPECIES_PANSEAR.name, &SPECIES_PANSEAR},
    {SPECIES_SIMISEAR.name, &SPECIES_SIMISEAR},
    {SPECIES_PANPOUR.name, &SPECIES_PANPOUR},
    {SPECIES_SIMIPOUR.name, &SPECIES_SIMIPOUR},
    {SPECIES_MUNNA.name, &SPECIES_MUNNA},
    {SPECIES_MUSHARNA.name, &SPECIES_MUSHARNA},
    {SPECIES_PIDOVE.name, &SPECIES_PIDOVE},
    {SPECIES_TRANQUILL.name, &SPECIES_TRANQUILL},
    {SPECIES_UNFEZANT.name, &SPECIES_UNFEZANT},
    {SPECIES_BLITZLE.name, &SPECIES_BLITZLE},
    {SPECIES_ZEBSTRIKA.name, &SPECIES_ZEBSTRIKA},
    {SPECIES_ROGGENROLA.name, &SPECIES_ROGGENROLA},
    {SPECIES_BOLDORE.name, &SPECIES_BOLDORE},
    {SPECIES_GIGALITH.name, &SPECIES_GIGALITH},
    {SPECIES_WOOBAT.name, &SPECIES_WOOBAT},
    {SPECIES_SWOOBAT.name, &SPECIES_SWOOBAT},
    {SPECIES_DRILBUR.name, &SPECIES_DRILBUR},
    {SPECIES_EXCADRILL.name, &SPECIES_EXCADRILL},
    {SPECIES_AUDINO.name, &SPECIES_AUDINO},
    {SPECIES_TIMBURR.name, &SPECIES_TIMBURR},
    {SPECIES_GURDURR.name, &SPECIES_GURDURR},
    {SPECIES_CONKELDURR.name, &SPECIES_CONKELDURR},
    {SPECIES_TYMPOLE.name, &SPECIES_TYMPOLE},
    {SPECIES_PALPITOAD.name, &SPECIES_PALPITOAD},
    {SPECIES_SEISMITOAD.name, &SPECIES_SEISMITOAD},
    {SPECIES_THROH.name, &SPECIES_THROH},
    {SPECIES_SAWK.name, &SPECIES_SAWK},
    {SPECIES_SEWADDLE.name, &SPECIES_SEWADDLE},
    {SPECIES_SWADLOON.name, &SPECIES_SWADLOON},
    {SPECIES_LEAVANNY.name, &SPECIES_LEAVANNY},
    {SPECIES_VENIPEDE.name, &SPECIES_VENIPEDE},
    {SPECIES_WHIRLIPEDE.name, &SPECIES_WHIRLIPEDE},
    {SPECIES_SCOLIPEDE.name, &SPECIES_SCOLIPEDE},
    {SPECIES_COTTONEE.name, &SPECIES_COTTONEE},
    {SPECIES_WHIMSICOTT.name, &SPECIES_WHIMSICOTT},
    {SPECIES_PETILIL.name, &SPECIES_PETILIL},
    {SPECIES_LILLIGANT.name, &SPECIES_LILLIGANT},
    {SPECIES_BASCULIN.name, &SPECIES_BASCULIN},
    {SPECIES_SANDILE.name, &SPECIES_SANDILE},
    {SPECIES_KROKOROK.name, &SPECIES_KROKOROK},
    {SPECIES_KROOKODILE.name, &SPECIES_KROOKODILE},
    {SPECIES_DARUMAKA.name, &SPECIES_DARUMAKA},
    {SPECIES_DARMANITAN.name, &SPECIES_DARMANITAN},
    {SPECIES_MARACTUS.name, &SPECIES_MARACTUS},
    {SPECIES_DWEBBLE.name, &SPECIES_DWEBBLE},
    {SPECIES_CRUSTLE.name, &SPECIES_CRUSTLE},
    {SPECIES_SCRAGGY.name, &SPECIES_SCRAGGY},
    {SPECIES_SCRAFTY.name, &SPECIES_SCRAFTY},
    {SPECIES_SIGILYPH.name, &SPECIES_SIGILYPH},
    {SPECIES_YAMASK.name, &SPECIES_YAMASK},
    {SPECIES_COFAGRIGUS.name, &SPECIES_COFAGRIGUS},
    {SPECIES_TIRTOUGA.name, &SPECIES_TIRTOUGA},
    {SPECIES_CARRACOSTA.name, &SPECIES_CARRACOSTA},
    {SPECIES_ARCHEN.name, &SPECIES_ARCHEN},
    {SPECIES_ARCHEOPS.name, &SPECIES_ARCHEOPS},
    {SPECIES_TRUBBISH.name, &SPECIES_TRUBBISH},
    {SPECIES_GARBODOR.name, &SPECIES_GARBODOR},
    {SPECIES_ZORUA.name, &SPECIES_ZORUA},
    {SPECIES_ZOROARK.name, &SPECIES_ZOROARK},
    {SPECIES_MINCCINO.name, &SPECIES_MINCCINO},
    {SPECIES_CINCCINO.name, &SPECIES_CINCCINO},
    {SPECIES_GOTHITA.name, &SPECIES_GOTHITA},
    {SPECIES_GOTHORITA.name, &SPECIES_GOTHORITA},
    {SPECIES_GOTHITELLE.name, &SPECIES_GOTHITELLE},
    {SPECIES_SOLOSIS.name, &SPECIES_SOLOSIS},
    {SPECIES_DUOSION.name, &SPECIES_DUOSION},
    {SPECIES_REUNICLUS.name, &SPECIES_REUNICLUS},
    {SPECIES_DUCKLETT.name, &SPECIES_DUCKLETT},
    {SPECIES_SWANNA.name, &SPECIES_SWANNA},
    {SPECIES_VANILLITE.name, &SPECIES_VANILLITE},
    {SPECIES_VANILLISH.name, &SPECIES_VANILLISH},
    {SPECIES_VANILLUXE.name, &SPECIES_VANILLUXE},
    {SPECIES_DEERLING.name, &SPECIES_DEERLING},
    {SPECIES_SAWSBUCK.name, &SPECIES_SAWSBUCK},
    {SPECIES_EMOLGA.name, &SPECIES_EMOLGA},
    {SPECIES_KARRABLAST.name, &SPECIES_KARRABLAST},
    {SPECIES_ESCAVALIER.name, &SPECIES_ESCAVALIER},
    {SPECIES_FOONGUS.name, &SPECIES_FOONGUS},
    {SPECIES_AMOONGUSS.name, &SPECIES_AMOONGUSS},
    {SPECIES_FRILLISH.name, &SPECIES_FRILLISH},
    {SPECIES_JELLICENT.name, &SPECIES_JELLICENT},
    {SPECIES_ALOMOMOLA.name, &SPECIES_ALOMOMOLA},
    {SPECIES_JOLTIK.name, &SPECIES_JOLTIK},
    {SPECIES_GALVANTULA.name, &SPECIES_GALVANTULA},
    {SPECIES_FERROSEED.name, &SPECIES_FERROSEED},
    {SPECIES_FERROTHORN.name, &SPECIES_FERROTHORN},
    {SPECIES_KLINK.name, &SPECIES_KLINK},
    {SPECIES_KLANG.name, &SPECIES_KLANG},
    {SPECIES_KLINKLANG.name, &SPECIES_KLINKLANG},
    {SPECIES_TYNAMO.name, &SPECIES_TYNAMO},
    {SPECIES_EELEKTRIK.name, &SPECIES_EELEKTRIK},
    {SPECIES_EELEKTROSS.name, &SPECIES_EELEKTROSS},
    {SPECIES_ELGYEM.name, &SPECIES_ELGYEM},
    {SPECIES_BEHEEYEM.name, &SPECIES_BEHEEYEM},
    {SPECIES_LITWICK.name, &SPECIES_LITWICK},
    {SPECIES_LAMPENT.name, &SPECIES_LAMPENT},
    {SPECIES_CHANDELURE.name, &SPECIES_CHANDELURE},
    {SPECIES_AXEW.name, &SPECIES_AXEW},
    {SPECIES_FRAXURE.name, &SPECIES_FRAXURE},
    {SPECIES_HAXORUS.name, &SPECIES_HAXORUS},
    {SPECIES_CUBCHOO.name, &SPECIES_CUBCHOO},
    {SPECIES_BEARTIC.name, &SPECIES_BEARTIC},
    {SPECIES_CRYOGONAL.name, &SPECIES_CRYOGONAL},
    {SPECIES_SHELMET.name, &SPECIES_SHELMET},
    {SPECIES_ACCELGOR.name, &SPECIES_ACCELGOR},
    {SPECIES_STUNFISK.name, &SPECIES_STUNFISK},
    {SPECIES_MIENFOO.name, &SPECIES_MIENFOO},
    {SPECIES_MIENSHAO.name, &SPECIES_MIENSHAO},
    {SPECIES_DRUDDIGON.name, &SPECIES_DRUDDIGON},
    {SPECIES_GOLETT.name, &SPECIES_GOLETT},
    {SPECIES_GOLURK.name, &SPECIES_GOLURK},
    {SPECIES_PAWNIARD.name, &SPECIES_PAWNIARD},
    {SPECIES_BISHARP.name, &SPECIES_BISHARP},
    {SPECIES_BOUFFALANT.name, &SPECIES_BOUFFALANT},
    {SPECIES_RUFFLET.name, &SPECIES_RUFFLET},
    {SPECIES_BRAVIARY.name, &SPECIES_BRAVIARY},
    {SPECIES_VULLABY.name, &SPECIES_VULLABY},
    {SPECIES_MANDIBUZZ.name, &SPECIES_MANDIBUZZ},
    {SPECIES_HEATMOR.name, &SPECIES_HEATMOR},
    {SPECIES_DURANT.name, &SPECIES_DURANT},
    {SPECIES_DEINO.name, &SPECIES_DEINO},
    {SPECIES_ZWEILOUS.name, &SPECIES_ZWEILOUS},
    {SPECIES_HYDREIGON.name, &SPECIES_HYDREIGON},
    {SPECIES_LARVESTA.name, &SPECIES_LARVESTA},
    {SPECIES_VOLCARONA.name, &SPECIES_VOLCARONA},
    {SPECIES_COBALION.name, &SPECIES_COBALION},
    {SPECIES_TERRAKION.name, &SPECIES_TERRAKION},
    {SPECIES_VIRIZION.name, &SPECIES_VIRIZION},
    {SPECIES_TORNADUS.name, &SPECIES_TORNADUS},
    {SPECIES_THUNDURUS.name, &SPECIES_THUNDURUS},
    {SPECIES_RESHIRAM.name, &SPECIES_RESHIRAM},
    {SPECIES_ZEKROM.name, &SPECIES_ZEKROM},
    {SPECIES_LANDORUS.name, &SPECIES_LANDORUS},
    {SPECIES_KYUREM.name, &SPECIES_KYUREM},
    {SPECIES_KELDEO.name, &SPECIES_KELDEO},
    {SPECIES_MELOETTA.name, &SPECIES_MELOETTA},
    {SPECIES_GENESECT.name, &SPECIES_GENESECT},
    {SPECIES_CHESPIN.name, &SPECIES_CHESPIN},
    {SPECIES_QUILLADIN.name, &SPECIES_QUILLADIN},
    {SPECIES_CHESNAUGHT.name, &SPECIES_CHESNAUGHT},
    {SPECIES_FENNEKIN.name, &SPECIES_FENNEKIN},
    {SPECIES_BRAIXEN.name, &SPECIES_BRAIXEN},
    {SPECIES_DELPHOX.name, &SPECIES_DELPHOX},
    {SPECIES_FROAKIE.name, &SPECIES_FROAKIE},
    {SPECIES_FROGADIER.name, &SPECIES_FROGADIER},
    {SPECIES_GRENINJA.name, &SPECIES_GRENINJA},
    {SPECIES_BUNNELBY.name, &SPECIES_BUNNELBY},
    {SPECIES_DIGGERSBY.name, &SPECIES_DIGGERSBY},
    {SPECIES_FLETCHLING.name, &SPECIES_FLETCHLING},
    {SPECIES_FLETCHINDER.name, &SPECIES_FLETCHINDER},
    {SPECIES_TALONFLAME.name, &SPECIES_TALONFLAME},
    {SPECIES_SCATTERBUG.name, &SPECIES_SCATTERBUG},
    {SPECIES_SPEWPA.name, &SPECIES_SPEWPA},
    {SPECIES_VIVILLON.name, &SPECIES_VIVILLON},
    {SPECIES_LITLEO.name, &SPECIES_LITLEO},
    {SPECIES_PYROAR.name, &SPECIES_PYROAR},
    {SPECIES_FLABEBE.name, &SPECIES_FLABEBE},
    {SPECIES_FLOETTE.name, &SPECIES_FLOETTE},
    {SPECIES_FLORGES.name, &SPECIES_FLORGES},
    {SPECIES_SKIDDO.name, &SPECIES_SKIDDO},
    {SPECIES_GOGOAT.name, &SPECIES_GOGOAT},
    {SPECIES_PANCHAM.name, &SPECIES_PANCHAM},
    {SPECIES_PANGORO.name, &SPECIES_PANGORO},
    {SPECIES_FURFROU.name, &SPECIES_FURFROU},
    {SPECIES_ESPURR.name, &SPECIES_ESPURR},
    {SPECIES_MEOWSTIC.name, &SPECIES_MEOWSTIC},
    {SPECIES_HONEDGE.name, &SPECIES_HONEDGE},
    {SPECIES_DOUBLADE.name, &SPECIES_DOUBLADE},
    {SPECIES_AEGISLASH.name, &SPECIES_AEGISLASH},
    {SPECIES_SPRITZEE.name, &SPECIES_SPRITZEE},
    {SPECIES_AROMATISSE.name, &SPECIES_AROMATISSE},
    {SPECIES_SWIRLIX.name, &SPECIES_SWIRLIX},
    {SPECIES_SLURPUFF.name, &SPECIES_SLURPUFF},
    {SPECIES_INKAY.name, &SPECIES_INKAY},
    {SPECIES_MALAMAR.name, &SPECIES_MALAMAR},
    {SPECIES_BINACLE.name, &SPECIES_BINACLE},
    {SPECIES_BARBARACLE.name, &SPECIES_BARBARACLE},
    {SPECIES_SKRELP.name, &SPECIES_SKRELP},
    {SPECIES_DRAGALGE.name, &SPECIES_DRAGALGE},
    {SPECIES_CLAUNCHER.name, &SPECIES_CLAUNCHER},
    {SPECIES_CLAWITZER.name, &SPECIES_CLAWITZER},
    {SPECIES_HELIOPTILE.name, &SPECIES_HELIOPTILE},
    {SPECIES_HELIOLISK.name, &SPECIES_HELIOLISK},
    {SPECIES_TYRUNT.name, &SPECIES_TYRUNT},
    {SPECIES_TYRANTRUM.name, &SPECIES_TYRANTRUM},
    {SPECIES_AMAURA.name, &SPECIES_AMAURA},
    {SPECIES_AURORUS.name, &SPECIES_AURORUS},
    {SPECIES_SYLVEON.name, &SPECIES_SYLVEON},
    {SPECIES_HAWLUCHA.name, &SPECIES_HAWLUCHA},
    {SPECIES_DEDENNE.name, &SPECIES_DEDENNE},
    {SPECIES_CARBINK.name, &SPECIES_CARBINK},
    {SPECIES_GOOMY.name, &SPECIES_GOOMY},
    {SPECIES_SLIGGOO.name, &SPECIES_SLIGGOO},
    {SPECIES_GOODRA.name, &SPECIES_GOODRA},
    {SPECIES_KLEFKI.name, &SPECIES_KLEFKI},
    {SPECIES_PHANTUMP.name, &SPECIES_PHANTUMP},
    {SPECIES_TREVENANT.name, &SPECIES_TREVENANT},
    {SPECIES_PUMPKABOO.name, &SPECIES_PUMPKABOO},
    {SPECIES_GOURGEIST.name, &SPECIES_GOURGEIST},
    {SPECIES_BERGMITE.name, &SPECIES_BERGMITE},
    {SPECIES_AVALUGG.name, &SPECIES_AVALUGG},
    {SPECIES_NOIBAT.name, &SPECIES_NOIBAT},
    {SPECIES_NOIVERN.name, &SPECIES_NOIVERN},
    {SPECIES_XERNEAS.name, &SPECIES_XERNEAS},
    {SPECIES_YVELTAL.name, &SPECIES_YVELTAL},
    {SPECIES_ZYGARDE.name, &SPECIES_ZYGARDE},
    {SPECIES_DIANCIE.name, &SPECIES_DIANCIE},
    {SPECIES_HOOPA.name, &SPECIES_HOOPA},
    {SPECIES_VOLCANION.name, &SPECIES_VOLCANION},
    {SPECIES_ROWLET.name, &SPECIES_ROWLET},
    {SPECIES_DARTRIX.name, &SPECIES_DARTRIX},
    {SPECIES_DECIDUEYE.name, &SPECIES_DECIDUEYE},
    {SPECIES_LITTEN.name, &SPECIES_LITTEN},
    {SPECIES_TORRACAT.name, &SPECIES_TORRACAT},
    {SPECIES_INCINEROAR.name, &SPECIES_INCINEROAR},
    {SPECIES_POPPLIO.name, &SPECIES_POPPLIO},
    {SPECIES_BRIONNE.name, &SPECIES_BRIONNE},
    {SPECIES_PRIMARINA.name, &SPECIES_PRIMARINA},
    {SPECIES_PIKIPEK.name, &SPECIES_PIKIPEK},
    {SPECIES_TRUMBEAK.name, &SPECIES_TRUMBEAK},
    {SPECIES_TOUCANNON.name, &SPECIES_TOUCANNON},
    {SPECIES_YUNGOOS.name, &SPECIES_YUNGOOS},
    {SPECIES_GUMSHOOS.name, &SPECIES_GUMSHOOS},
    {SPECIES_GRUBBIN.name, &SPECIES_GRUBBIN},
    {SPECIES_CHARJABUG.name, &SPECIES_CHARJABUG},
    {SPECIES_VIKAVOLT.name, &SPECIES_VIKAVOLT},
    {SPECIES_CRABRAWLER.name, &SPECIES_CRABRAWLER},
    {SPECIES_CRABOMINABLE.name, &SPECIES_CRABOMINABLE},
    {SPECIES_ORICORIO.name, &SPECIES_ORICORIO},
    {SPECIES_CUTIEFLY.name, &SPECIES_CUTIEFLY},
    {SPECIES_RIBOMBEE.name, &SPECIES_RIBOMBEE},
    {SPECIES_ROCKRUFF.name, &SPECIES_ROCKRUFF},
    {SPECIES_LYCANROC.name, &SPECIES_LYCANROC},
    {SPECIES_WISHIWASHI.name, &SPECIES_WISHIWASHI},
    {SPECIES_MAREANIE.name, &SPECIES_MAREANIE},
    {SPECIES_TOXAPEX.name, &SPECIES_TOXAPEX},
    {SPECIES_MUDBRAY.name, &SPECIES_MUDBRAY},
    {SPECIES_MUDSDALE.name, &SPECIES_MUDSDALE},
    {SPECIES_DEWPIDER.name, &SPECIES_DEWPIDER},
    {SPECIES_ARAQUANID.name, &SPECIES_ARAQUANID},
    {SPECIES_FOMANTIS.name, &SPECIES_FOMANTIS},
    {SPECIES_LURANTIS.name, &SPECIES_LURANTIS},
    {SPECIES_MORELULL.name, &SPECIES_MORELULL},
    {SPECIES_SHIINOTIC.name, &SPECIES_SHIINOTIC},
    {SPECIES_SALANDIT.name, &SPECIES_SALANDIT},
    {SPECIES_SALAZZLE.name, &SPECIES_SALAZZLE},
    {SPECIES_STUFFUL.name, &SPECIES_STUFFUL},
    {SPECIES_BEWEAR.name, &SPECIES_BEWEAR},
    {SPECIES_BOUNSWEET.name, &SPECIES_BOUNSWEET},
    {SPECIES_STEENEE.name, &SPECIES_STEENEE},
    {SPECIES_TSAREENA.name, &SPECIES_TSAREENA},
    {SPECIES_COMFEY.name, &SPECIES_COMFEY},
    {SPECIES_ORANGURU.name, &SPECIES_ORANGURU},
    {SPECIES_PASSIMIAN.name, &SPECIES_PASSIMIAN},
    {SPECIES_WIMPOD.name, &SPECIES_WIMPOD},
    {SPECIES_GOLISOPOD.name, &SPECIES_GOLISOPOD},
    {SPECIES_SANDYGAST.name, &SPECIES_SANDYGAST},
    {SPECIES_PALOSSAND.name, &SPECIES_PALOSSAND},
    {SPECIES_PYUKUMUKU.name, &SPECIES_PYUKUMUKU},
    {SPECIES_TYPE_NULL.name, &SPECIES_TYPE_NULL},
    {SPECIES_SILVALLY.name, &SPECIES_SILVALLY},
    {SPECIES_MINIOR.name, &SPECIES_MINIOR},
    {SPECIES_KOMALA.name, &SPECIES_KOMALA},
    {SPECIES_TURTONATOR.name, &SPECIES_TURTONATOR},
    {SPECIES_TOGEDEMARU.name, &SPECIES_TOGEDEMARU},
    {SPECIES_MIMIKYU.name, &SPECIES_MIMIKYU},
    {SPECIES_BRUXISH.name, &SPECIES_BRUXISH},
    {SPECIES_DRAMPA.name, &SPECIES_DRAMPA},
    {SPECIES_DHELMISE.name, &SPECIES_DHELMISE},
    {SPECIES_JANGMO_O.name, &SPECIES_JANGMO_O},
    {SPECIES_HAKAMO_O.name, &SPECIES_HAKAMO_O},
    {SPECIES_KOMMO_O.name, &SPECIES_KOMMO_O},
    {SPECIES_TAPU_KOKO.name, &SPECIES_TAPU_KOKO},
    {SPECIES_TAPU_LELE.name, &SPECIES_TAPU_LELE},
    {SPECIES_TAPU_BULU.name, &SPECIES_TAPU_BULU},
    {SPECIES_TAPU_FINI.name, &SPECIES_TAPU_FINI},
    {SPECIES_COSMOG.name, &SPECIES_COSMOG},
    {SPECIES_COSMOEM.name, &SPECIES_COSMOEM},
    {SPECIES_SOLGALEO.name, &SPECIES_SOLGALEO},
    {SPECIES_LUNALA.name, &SPECIES_LUNALA},
    {SPECIES_NIHILEGO.name, &SPECIES_NIHILEGO},
    {SPECIES_BUZZWOLE.name, &SPECIES_BUZZWOLE},
    {SPECIES_PHEROMOSA.name, &SPECIES_PHEROMOSA},
    {SPECIES_XURKITREE.name, &SPECIES_XURKITREE},
    {SPECIES_CELESTEELA.name, &SPECIES_CELESTEELA},
    {SPECIES_KARTANA.name, &SPECIES_KARTANA},
    {SPECIES_GUZZLORD.name, &SPECIES_GUZZLORD},
    {SPECIES_NECROZMA.name, &SPECIES_NECROZMA},
    {SPECIES_MAGEARNA.name, &SPECIES_MAGEARNA},
    {SPECIES_MARSHADOW.name, &SPECIES_MARSHADOW},
    {SPECIES_POIPOLE.name, &SPECIES_POIPOLE},
    {SPECIES_NAGANADEL.name, &SPECIES_NAGANADEL},
    {SPECIES_STAKATAKA.name, &SPECIES_STAKATAKA},
    {SPECIES_BLACEPHALON.name, &SPECIES_BLACEPHALON},
    {SPECIES_ZERAORA.name, &SPECIES_ZERAORA},
    {SPECIES_MELTAN.name, &SPECIES_MELTAN},
    {SPECIES_MELMETAL.name, &SPECIES_MELMETAL}
};

const Species* speciesFromString(const std::string& speciesName){
    return speciesMap.at(speciesName);
}

bool compareSpeciesNames(const std::string& spec1, const std::string& spec2){
    return speciesMap.at(spec1)->id < speciesMap.at(spec2)->id;
}

std::string createSpeciesDataResponse(){
    std::vector<std::string> speciesNames;
    for (auto& [name,data] : speciesMap){
        if (data == nullptr) continue;
        speciesNames.push_back(std::string(name));
    }
    std::sort(speciesNames.begin(), speciesNames.end(), compareSpeciesNames);
    json response;
    response["success"] = true;
    response["message"] = "Ok";
    response["data"] = speciesNames;
    return response.dump();
}