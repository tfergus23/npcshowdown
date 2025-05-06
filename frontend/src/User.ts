import { TournamentResultSet } from "./TournamentResultSet";
import Trainer from "./Trainer";

export default interface User{
    name: string;
    id: number;
    token: string;
    accountCreated: string,
    lastPasswordChange: string,
    isAdmin: boolean
}