import Trainer from "./Trainer"

export interface TournamentResult{
    index: number,
    elo: number,
    wins: number,
    losses: number,
    bestWin: number,
    bestWinEloDiff: number,
    id: number
}

export interface TournamentResultSet{
    results: Array<TournamentResult>,
    trainers: Array<Trainer>,
    user: string | undefined,
    id: number
}