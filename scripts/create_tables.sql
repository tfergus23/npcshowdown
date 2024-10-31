drop database if exists npcs_test;
create database npcs_test;
use npcs_test;


create table user (
    id bigint PRIMARY KEY AUTO_INCREMENT,
    name varchar(16),
    password char(32),
    accountCreated DATE,
    lastPasswordChange DATE,
    email varchar(255)
);

create unique index user_name_index on user (name);

create table user_session(
    user bigint not null,
    FOREIGN KEY (user) 
        REFERENCES user(id),
    token char(32),
    dateGranted DATE
);

create index user_session_user_index on user_session(user);

create table tournament (
    id bigint PRIMARY KEY,
    dateRan DATE,
    user bigint,
    FOREIGN KEY (user) 
        REFERENCES user(id),
    name varchar(32)
);

CREATE TABLE trainer (
    id BIGINT PRIMARY KEY,
    user bigint,
    FOREIGN KEY (user) 
        REFERENCES user(id),
    tournament bigint,
    FOREIGN KEY (tournament) 
        REFERENCES tournament(id),
    name varchar(32),
    trainerLevel ENUM('First Move', 'Use Two Moves then Switch', 'Wild', 'Switcher', 'Trainer', 'Boss')
);

CREATE TABLE pokemon (
    trainer bigint NOT NULL,
    FOREIGN KEY (trainer) 
        REFERENCES trainer(id),
    species smallint,
    level tinyint unsigned,
    move1 smallint,
    move2 smallint,
    move3 smallint,
    move4 smallint,
    abilityID smallint,
    gender tinyint,
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
    id BIGINT PRIMARY KEY AUTO_INCREMENT,
    tournament bigint,
    FOREIGN KEY (tournament)
        REFERENCES tournament(id),
    trainer1 bigint not null,
    FOREIGN KEY (trainer1) 
        REFERENCES trainer(id),
    trainer2 bigint not null,
    FOREIGN KEY (trainer2) 
        REFERENCES trainer(id),
    seed bigint unsigned not null
);

CREATE INDEX battle_tournament_index on battle (tournament);

insert into user (name, password, email) values ("MrToadSir", "verystrongpassword", "guybro@dudemail.com");