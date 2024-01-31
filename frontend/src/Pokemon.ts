export default interface Pokemon{
    empty: boolean,
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