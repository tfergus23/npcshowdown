import Trainer from "./Trainer"

export interface TournamentTrainer{
    index: number,
    elo: number,
    wins: number,
    losses: number,
    bestWin: number,
    bestWinEloDiff: number
}

export interface TournamentResultSet{
    tournamentTrainers: Array<TournamentTrainer>,
    trainers: Array<Trainer>
}