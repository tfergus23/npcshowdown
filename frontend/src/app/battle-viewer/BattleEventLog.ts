interface EventLogPokemon{
  species: string,
  maxHealth: number;
  gender: string;
}

export interface EventLogTrainer{
  name: string;
  team: Array<EventLogPokemon>
}

interface EventLogEvent{
  type: string;
  message: string;
  data: any;
}

export interface BattleEventLog{
  trainer1: EventLogTrainer;
  trainer2: EventLogTrainer;
  events: Array<EventLogEvent>;
}