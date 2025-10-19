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

function assert(bool: boolean){
    if (!bool) throw new Error();
}

export function validateTrainer(trainer: Trainer){
    assert(typeof trainer.team == 'object');
    assert(typeof trainer.name == 'string');
    assert(typeof trainer.trainerLevel == 'string');
    if (trainer.id) assert(typeof(trainer.id) == 'number');
}