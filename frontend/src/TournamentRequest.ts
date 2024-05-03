import Trainer from "./Trainer";

export default interface BattleRequest{
    trainers: Array<Trainer>;
    seed: string;
    rounds: number;
}