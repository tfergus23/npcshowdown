import Trainer from "./Trainer";

export default interface BattleRequest{
    trainer1: Trainer,
    trainer2: Trainer,
    seed: string
}