import Pokemon from "./Pokemon"

export default interface Trainer{
    team: Array<Pokemon>,
    name: string,
    trainerLevel: string,
    id?: number
}

export function createEmptyTrainer(): Trainer{
    return {
        team: new Array<Pokemon>(),
        name: "Youngster Joey",
        trainerLevel: "Wild"
    }
}