drop database if exists npcs_test;
create database npcs_test;
use npcs_test;


create table user (
    id bigint unsigned PRIMARY KEY AUTO_INCREMENT,
    name varchar(16),
    password char(64),
    accountCreated DATE,
    lastPasswordChange DATE
);

create unique index user_name_index on user (name);

create table user_session(
    user bigint unsigned not null,
    FOREIGN KEY (user) 
        REFERENCES user(id) on delete cascade,
    token char(32),
    dateGranted DATETIME,
    lastUsed DATETIME
);

create index user_session_user_index on user_session(user);
create index user_session_user_token_index on user_session(user,token);

create table tournament (
    id bigint unsigned PRIMARY KEY AUTO_INCREMENT,
    dateRan DATE,
    lastUnsave DATE,
    user bigint unsigned,
    FOREIGN KEY (user) 
        REFERENCES user(id) on delete cascade,
    name varchar(32),
    done boolean not null,
    ip varchar(15)
);

CREATE TABLE trainer (
    id BIGINT unsigned PRIMARY KEY AUTO_INCREMENT,
    user bigint unsigned,
    FOREIGN KEY (user) 
        REFERENCES user(id) on delete cascade,
    tournament bigint unsigned,
    FOREIGN KEY (tournament) 
        REFERENCES tournament(id) on delete cascade,
    name varchar(16),
    trainerLevel tinyint
);

CREATE TABLE pokemon (
    trainer bigint unsigned NOT NULL,
    FOREIGN KEY (trainer) 
        REFERENCES trainer(id) on delete cascade,
    position tinyint,
    species smallint,
    level tinyint unsigned,
    move1 smallint,
    move2 smallint,
    move3 smallint,
    move4 smallint,
    abilityID smallint,
    gender ENUM('Random', 'Male', 'Female'),
    nature tinyint,
    itemID smallint,
    nickname varchar(16),
    hpIV  tinyint unsigned,
    atkIV tinyint unsigned,
    defIV tinyint unsigned,
    spaIV tinyint unsigned,
    spdIV tinyint unsigned,
    speIV tinyint unsigned,
    hpEV  tinyint unsigned,
    atkEV tinyint unsigned,
    defEV tinyint unsigned,
    spaEV tinyint unsigned,
    spdEV tinyint unsigned,
    speEV tinyint unsigned
);

CREATE INDEX pokemon_trainer_index on pokemon (trainer);

CREATE TABLE battle (
    id BIGINT unsigned PRIMARY KEY AUTO_INCREMENT,
    tournament bigint unsigned not null,
    FOREIGN KEY (tournament)
        REFERENCES tournament(id) on delete cascade,
    trainer1 int not null,
    trainer2 int not null,
    seed bigint unsigned not null
);

CREATE INDEX battle_tournament_index on battle (tournament);

CREATE TABLE trainer_stats (
    tournament bigint unsigned not null,
    FOREIGN KEY (tournament) 
        REFERENCES tournament(id) on delete cascade,
    trainerIndex int not null,
    trainer bigint unsigned not null,
    FOREIGN KEY (trainer)
        REFERENCES trainer(id) on delete cascade,
    elo int not null,
    wins int not null,
    losses int not null,
    bestWin bigint unsigned,
    FOREIGN KEY (bestWin)
        REFERENCES battle(id) on delete cascade,
    bestWinEloDiff int not null
);

CREATE INDEX stats_tournament_index on trainer_stats(tournament);

create table saved_tournaments (
    user BIGINT unsigned not null,
    FOREIGN KEY (user)
        REFERENCES user(id) on delete cascade,
    tournament bigint  unsigned not null,
    FOREIGN KEY (tournament)
        REFERENCES tournament(id) on delete cascade
);

CREATE INDEX saved_tournaments_user_index on saved_tournaments(user);

create table error_battle (
    hash bigint unsigned PRIMARY KEY,
    request TEXT,
    dateRan DATE,
    acknowledged boolean
);

create index ack_error_battle_index on error_battle(acknowledged);
create index date_error_battle_index on error_battle(dateRan);
create index ack_date_error_battle_index on error_battle(acknowledged, dateRan);

insert into user (name, password, accountCreated, lastPasswordChange) values ("admin", "8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918", NOW(), NOW());