export default interface Pokemon{
    species: string,
    nickname: string,
    level: number,
    moves: [string,string,string,string],
    abilityName: string,
    gender: string,
    ivs: [number,number,number,number,number,number],
    nature: string,
    itemName: string,
    evs: [number,number,number,number,number,number],
}

export function createEmptyPokemon(): Pokemon{
    return{
        species: "Luxray",
        nickname: "",
        level: 100,
        moves: ["","","",""],
        abilityName: "",
        gender: "Random",
        ivs: [31,31,31,31,31,31],
        nature: "Serious",
        itemName: "",
        evs: [0,0,0,0,0,0]
    };
}