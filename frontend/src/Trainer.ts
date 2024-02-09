import Pokemon from "./Pokemon"

export default interface Trainer{
    team: Array<Pokemon>,
    name: string,
    trainerClass: string,
    trainerLevel: string
}