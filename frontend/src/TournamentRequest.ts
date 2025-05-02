import Trainer from "./Trainer";

export default interface TournamentRequest{
    trainers: Array<Trainer>;
    seed: string;
    rounds: number;
    name: string;
    user: string | undefined;
}