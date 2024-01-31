import Pokemon from "./Pokemon"

export default interface Trainer{
    team: [Pokemon,Pokemon,Pokemon,Pokemon,Pokemon,Pokemon],
    name: string,
    trainerClass: string,
    trainerLevel: number
}