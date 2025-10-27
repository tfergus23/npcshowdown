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
    id: string
}

export interface TournamentResultSet{
    results: Array<TournamentResult>,
    trainers: Array<Trainer>,
    user: string | undefined,
    id: string
    name: string,
    dateRan: string
}


export function isResultSet(data: any) : boolean{
    return data.results && Array.isArray(data.results) &&
           data.trainers && Array.isArray(data.trainers) &&
           (!data.user || (data.user && typeof(data.user) == 'string')) &&
           data.id && typeof(data.id) == 'string' &&
           data.name && typeof(data.name) == 'string' &&
           data.dateRan && typeof(data.dateRan) == 'string';
}