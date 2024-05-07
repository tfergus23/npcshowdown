import Trainer from "./Trainer"

export interface TournamentResult{
    index: number,
    elo: number,
    wins: number,
    losses: number,
    bestWin: number,
    bestWinEloDiff: number
}

export interface TournamentResultSet{
    results: Array<TournamentResult>,
    trainers: Array<Trainer>
}