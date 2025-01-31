import Trainer from "./Trainer"

export interface BattleResult{
    trainer1Index: number,
    trainer2Index: number,
    seed: number
}

export interface TournamentResult{
    index: number,
    elo: number,
    wins: number,
    losses: number,
    bestWin: BattleResult,
    bestWinEloDiff: number,
    id: number
}

export interface TournamentResultSet{
    results: Array<TournamentResult>,
    trainers: Array<Trainer>,
    user: string | undefined,
    id: number
}