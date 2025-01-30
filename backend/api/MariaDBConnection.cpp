#include "MariaDBConnection.hpp"
#include <iostream>
#include <mutex>
#include "api_utils.hpp"
#include "sim/battle/TrainerLevel.hpp"
#include "sim/battle/Battle.hpp"
#include "sim/data/Species.hpp"
#include "sim/data/Moves.hpp"
#include "sim/data/Abilities.hpp"
#include "sim/battle/Nature.hpp"


static std::mutex mostRecentKeyMutex;

json TournamentResults::toJSON(MariaDBConnection& db) const{
    json data;
    std::vector<json> trainerJSONs;
    std::vector<json> statJSONs;
    for(auto& stat : trainerStats){
        statJSONs.push_back(stat.toJSON());
    }
    for(auto trainer : trainers){
        trainerJSONs.push_back(db.getTrainer(trainer).value().toJSON());
    }
    data["trainers"] = trainerJSONs;
    data["results"] = statJSONs;
    if (user != ""){
        data["user"] = user;
    }
    return data;
}

MariaDBConnection::MariaDBConnection(const std::string& username, const std::string& password, const std::string& host, const std::string& database, int maxUserSessions) : maxUserSessions{maxUserSessions}{
    sql::Driver* driver= sql::mariadb::get_driver_instance();
    sql::SQLString url("jdbc:mariadb://" + host + ":3306/" + database);
    sql::Properties properties({{"user", username}, {"password", password}});
    conn = std::unique_ptr<sql::Connection>(driver->connect(url, properties));
    if (maxUserSessions < 1){
        throw std::runtime_error("Invalid value for max_user_sessions: " + std::to_string(maxUserSessions));
    }
}

std::optional<Trainer> MariaDBConnection::getTrainer(size_t id){
    std::unique_ptr<sql::PreparedStatement>  trainerStmnt(conn->prepareStatement("select trainerLevel, name from trainer where id = ?"));
    trainerStmnt->setUInt64(1, id);
    std::unique_ptr<sql::ResultSet> trainerResult(trainerStmnt->executeQuery());
    if (!trainerResult->next()){
        return std::optional<Trainer>();
    }

    TrainerLevel trainerLevel =  (TrainerLevel)trainerResult->getInt(1);
    std::string trainerName(trainerResult->getString(2));

    std::unique_ptr<sql::PreparedStatement> teamStmnt(conn->prepareStatement("select * from pokemon where trainer = ? order by position asc"));
    teamStmnt->setUInt64(1, id);

    std::unique_ptr<sql::ResultSet> teamResults(teamStmnt->executeQuery());

    std::vector<PokemonBlueprint> team;
    while (teamResults->next())
    {
        team.emplace_back(
            speciesFromID(teamResults->getShort(3))->name,
            (uint8_t)teamResults->getUInt(4),
            std::array<std::string,4>{
                moveFromID(teamResults->getShort(5))->name,
                moveFromID(teamResults->getShort(6))->name,
                moveFromID(teamResults->getShort(7))->name,
                moveFromID(teamResults->getShort(8))->name
            },
            abilityFromId(teamResults->getShort(9))->name,
            std::string(teamResults->getString(10)), //Gender, this might not work
            std::array<uint8_t,6>{
                (uint8_t)teamResults->getUInt(14),
                (uint8_t)teamResults->getUInt(15),
                (uint8_t)teamResults->getUInt(16),
                (uint8_t)teamResults->getUInt(17),
                (uint8_t)teamResults->getUInt(18),
                (uint8_t)teamResults->getUInt(19)
            },
            stringFromNature((Nature) teamResults->getInt(11)),
            itemFromID(teamResults->getShort(12))->name,
            std::array<uint8_t,6>{
                (uint8_t)teamResults->getUInt(20),
                (uint8_t)teamResults->getUInt(21),
                (uint8_t)teamResults->getUInt(22),
                (uint8_t)teamResults->getUInt(23),
                (uint8_t)teamResults->getUInt(24),
                (uint8_t)teamResults->getUInt(25),
            },
            std::string(teamResults->getString(13))
        );
    }
    return Trainer(trainerName, team, trainerLevel);
}
std::optional<BattleResult> MariaDBConnection::getBattle(size_t id){
    std::unique_ptr<sql::PreparedStatement> selectStmnt(conn->prepareStatement("select trainer1, trainer2, seed from battle where id = ?"));
    selectStmnt->setUInt64(1, id);
    std::unique_ptr<sql::ResultSet> results (selectStmnt->executeQuery());
    if (results->rowsCount() != 1){
        return std::optional<BattleResult>();
    }
    results->next();
    return BattleResult{
        results->getUInt64(1),
        results->getUInt64(2),
        results->getUInt64(3)
    };
}
std::optional<TournamentResults> MariaDBConnection::getTournament(size_t id){
    //TODO: This needs to include the username of the user that ran the tournament
    std::unique_ptr<sql::PreparedStatement> doneStatement(conn->prepareStatement("select done,user from tournament where id = ?"));
    doneStatement->setUInt64(1, id);
    std::unique_ptr<sql::ResultSet> doneResults(doneStatement->executeQuery());
    if (doneResults->rowsCount() < 1){
        return std::optional<TournamentResults>();
    }
    if (doneResults->rowsCount() > 1){
        throw std::runtime_error("Somehow got multiple tournamnets for id: " + std::to_string(id));
    }

    TournamentResults result;

    doneResults->next();
    if (!doneResults->isNull(2)){
        std::unique_ptr<sql::PreparedStatement> usernameStatement(conn->prepareStatement("select name from user where id = ?"));
        usernameStatement->setUInt64(1, doneResults->getUInt64(2));
        std::unique_ptr<sql::ResultSet> usernameResults(usernameStatement->executeQuery());
        usernameResults->next();
        result.user = usernameResults->getString(1);
    }

    result.ready = doneResults->getBoolean(1);
    if (!result.ready){
        return result;
    }

    std::unique_ptr<sql::PreparedStatement> selectStmnt(conn->prepareStatement("select trainerIndex,trainer,elo,wins,losses,bestWin,bestWinEloDiff from trainer_stats where tournament = ? order by trainerIndex asc"));
    selectStmnt->setUInt64(1, id);
    std::unique_ptr<sql::ResultSet> results(selectStmnt->executeQuery());

    while (results->next()){
        result.trainerStats.emplace_back();
        TrainerStats& stats = result.trainerStats[result.trainerStats.size()-1];
        stats.trainerIndex = results->getInt(1);
        stats.elo = results->getInt(3);
        stats.wins = results->getInt(4);
        stats.losses = results->getInt(5);
        stats.bestWin = results->getInt64(6);
        stats.bestWinEloDiff = results->getInt(7);
        result.trainers.push_back(results->getUInt64(2));
    }

    return result;
}

size_t MariaDBConnection::createEmptyTournament(size_t user){
    std::unique_ptr<sql::PreparedStatement> insertStmnt(conn->prepareStatement("insert into tournament (user, done, lastUnsave) values (?,?,NOW())"));
    if (user){
        insertStmnt->setUInt64(1, user);
    }
    else{
        insertStmnt->setNull(1, sql::Types::BIGINT);
    }
    insertStmnt->setBoolean(2, false);

    return executeInsertAndGetID(insertStmnt.get());
}

size_t MariaDBConnection::saveTrainer(const Trainer& trainer, size_t user, size_t tournament){
    assert(!(user == 0 && tournament == 0));
    assert(!(user && tournament));
    std::string sql = "insert into trainer (user, tournament, name, trainerLevel) values (?, ?, ?, ?)";
    std::unique_ptr<sql::PreparedStatement> insertTrainerStmnt(conn->prepareStatement(sql));
    if (user){
        insertTrainerStmnt->setUInt64(1, user);
    }
    else{
        insertTrainerStmnt->setNull(1, sql::Types::BIGINT);
    }
    
    if (tournament){
        insertTrainerStmnt->setInt64(2, tournament);
    }
    else{
        insertTrainerStmnt->setNull(2, sql::Types::BIGINT);
    }
    insertTrainerStmnt->setString(3, trainer.trainerInfo.name);
    insertTrainerStmnt->setInt(4, (int8_t)trainer.trainerInfo.trainerLevel);

    size_t id = executeInsertAndGetID(insertTrainerStmnt.get());

    saveTeam(trainer.teamBlueprint, id);
    return id;
}

size_t MariaDBConnection::saveBattle(const BattleResult& result, size_t tournament){
    std::unique_ptr<sql::PreparedStatement> insertStmnt(conn->prepareStatement("insert into battle (tournament, trainer1, trainer2, seed) values (?,?,?,?)"));
    insertStmnt->setUInt64(1, tournament);
    insertStmnt->setUInt64(2, result.trainer1);
    insertStmnt->setUInt64(3, result.trainer2);
    insertStmnt->setUInt64(4, result.seed);

    return executeInsertAndGetID(insertStmnt.get());
}

void MariaDBConnection::saveTournament(const Tournament& tournament, size_t id){
    std::vector<size_t> trainers;
    for (auto& trainer : tournament.trainers){
        trainers.push_back(saveTrainer(trainer, 0, id));
    }

    std::vector<TrainerStats> stats;
    std::unordered_map<size_t,size_t> addedBattles;
    for (auto stat : tournament.trainerStats){
        if (!addedBattles.contains(stat.bestWin) && stat.bestWin >= 0){
            BattleResult bestWin = tournament.results[stat.bestWin];
            bestWin.trainer1 = trainers[bestWin.trainer1];
            bestWin.trainer2 = trainers[bestWin.trainer2];
            bestWin.winner = trainers[bestWin.winner];
            addedBattles[stat.bestWin] = saveBattle(bestWin, id);
        }
        stat.bestWin = addedBattles[stat.bestWin];
        stats.push_back(stat);
    }

    std::string sql = "insert into trainer_stats (tournament, trainerIndex, trainer, elo, wins, losses, bestWin, bestWinEloDiff) values";

    for (auto& stat : stats){
        sql += "(?,?,?,?,?,?,?,?),";
    }
    sql.pop_back();

    std::unique_ptr<sql::PreparedStatement> insertStmnt(conn->prepareStatement(sql));

    int32_t columnIndex = 1;
    for(auto& stat: stats){
        insertStmnt->setUInt64(columnIndex++, id);
        insertStmnt->setInt(columnIndex++, stat.trainerIndex);
        insertStmnt->setUInt64(columnIndex++, trainers[stat.trainerIndex]);
        insertStmnt->setInt(columnIndex++, stat.elo);
        insertStmnt->setInt(columnIndex++, stat.wins);
        insertStmnt->setInt(columnIndex++, stat.losses);
        insertStmnt->setUInt64(columnIndex++, stat.bestWin);
        insertStmnt->setInt(columnIndex++, stat.bestWinEloDiff);
    }

    executeInsert(insertStmnt.get());

    std::unique_ptr<sql::PreparedStatement> updateStmnt(conn->prepareStatement("update tournament set done=?, dateRan=NOW() where id = ?"));
    updateStmnt->setBoolean(1, true);
    updateStmnt->setUInt64(2, id);

    delete updateStmnt->executeQuery();
}

bool MariaDBConnection::isTokenValid(const std::string& username, const std::string& token){
    size_t userID = userIdFromName(username);
  
    std::unique_ptr<sql::PreparedStatement> tokenStatement(conn->prepareStatement("select token from user_session where user = ? and token = ?"));
    tokenStatement->setUInt64(1, userID);
    tokenStatement->setString(2, token);
    std::unique_ptr<sql::ResultSet> tokenResults(tokenStatement->executeQuery());

    return tokenResults->rowsCount();
}

std::string MariaDBConnection::createUserSession(const std::string& username, const std::string& password, std::string& outToken){
    std::unique_ptr<sql::PreparedStatement> passwordStatement(conn->prepareStatement("select id,password from user where name = ?"));
    passwordStatement->setString(1, username);
    std::unique_ptr<sql::ResultSet> results(passwordStatement->executeQuery());

    if (results->rowsCount() < 1){
        return "That user does not exist";
    }

    if (results->rowsCount() > 1){
        throw std::runtime_error("Somehow got two users in username query for: " + username);
    }

    std::string dbPassword;
    size_t userID = 0;
    while (results->next()){
        userID = results->getUInt64(1);
        dbPassword = results->getString(2);
    }

    std::string hashedPassword = sha256(password);

    if (hashedPassword != dbPassword){
        return "Invalid credentials";
    }

    deleteOldUserSessions(userID);

    std::string newToken = generateUUID();
    std::unique_ptr<sql::PreparedStatement> insertTokenStatement(conn->prepareStatement("insert into user_session (user, token, dateGranted, lastUsed) values (?, ?, NOW(),NOW())"));
    insertTokenStatement->setUInt64(1, userID);
    insertTokenStatement->setString(2, newToken);
    delete insertTokenStatement->executeQuery();

    outToken = newToken;
    return "";
}

void MariaDBConnection::deleteUserSession(const std::string& username, const std::string & token){
    size_t userID = userIdFromName(username);
    std::unique_ptr<sql::PreparedStatement> deleteSessionStmnt(conn->prepareStatement("delete from user_session where user = ? and token = ?"));
    deleteSessionStmnt->setUInt64(1, userID);
    deleteSessionStmnt->setString(2, token);

    delete deleteSessionStmnt->executeQuery();
}

size_t MariaDBConnection::userIdFromName(const std::string& username){
    std::unique_ptr<sql::PreparedStatement> passwordStatement(conn->prepareStatement("select id from user where name = ?"));
    passwordStatement->setString(1, username);
    std::unique_ptr<sql::ResultSet> results(passwordStatement->executeQuery());

    if (results->rowsCount() < 1){
        return 0;
    }

    if (results->rowsCount() > 1){
        throw std::runtime_error("Somehow got two users in username query for: " + username);
    }

    size_t userID = 0;
    while (results->next()){
        userID = results->getUInt64(1);
    }
    return userID;
}

void MariaDBConnection::deleteOldUserSessions(size_t userID){
    std::unique_ptr<sql::PreparedStatement> countStatement(conn->prepareStatement("select count(*) from user_session where user = ?"));
    countStatement->setUInt64(1, userID);
    std::unique_ptr<sql::ResultSet> countResults(countStatement->executeQuery());
    countResults->next();
    int numSessions = countResults->getInt(1);

    int numToRemove = (numSessions - maxUserSessions) + 1; // Want to reduce them to maxUserSessions - 1

    if (numToRemove < 1) return;

    std::unique_ptr<sql::PreparedStatement> deleteOldSessionsStatement(conn->prepareStatement("delete from user_session where user = ? order by lastUsed limit ?"));
    deleteOldSessionsStatement->setUInt64(1, userID);
    deleteOldSessionsStatement->setInt(2, numToRemove);

    delete deleteOldSessionsStatement->executeQuery();
}

void MariaDBConnection::updateTokenLastUsed(const std::string& username, const std::string& token){
    size_t userID = userIdFromName(username);
    std::unique_ptr<sql::PreparedStatement> updateStatement(conn->prepareStatement("update user_session set lastUsed=NOW() where user = ? and token = ?"));
    updateStatement->setUInt64(1, userID);
    updateStatement->setString(2, token);
    delete updateStatement->executeQuery();
}

void MariaDBConnection::saveTeam(const std::vector<PokemonBlueprint>& team, size_t trainer){
    std::string sql = "insert into pokemon (trainer, position, species, level, move1, move2, move3, move4, abilityID, gender, nature, itemID, nickname, hpIV, atkIV, defIV, spaIV, spdIV, speIV, hpEV, atkEV, defEV, spaEV, spdEV, speEV) values ";

    for (const auto& poke : team){
        sql += "(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?),";
    }
    sql.pop_back();

    std::unique_ptr<sql::PreparedStatement> stmnt(conn->prepareStatement(sql));

    int32_t columnIndex = 1;
    int32_t teamIndex = 0;
    for (const auto& poke : team){
        stmnt->setUInt64(columnIndex++, trainer);
        stmnt->setInt(columnIndex++, teamIndex);
        stmnt->setShort(columnIndex++, speciesFromString(poke.species)->id);
        stmnt->setUInt(columnIndex++, poke.level);
        stmnt->setShort(columnIndex++, moveFromString(poke.moves[0])->id);
        stmnt->setShort(columnIndex++, moveFromString(poke.moves[1])->id);
        stmnt->setShort(columnIndex++, moveFromString(poke.moves[2])->id);
        stmnt->setShort(columnIndex++, moveFromString(poke.moves[3])->id);
        stmnt->setShort(columnIndex++, abilityFromString(poke.abilityName)->id);
        stmnt->setString(columnIndex++, poke.gender); //This might not work?
        stmnt->setInt(columnIndex++, (int8_t) natureFromString(poke.nature));
        stmnt->setShort(columnIndex++, itemFromString(poke.itemName)->id);
        stmnt->setString(columnIndex++, poke.nickname);
        stmnt->setUInt(columnIndex++, poke.ivs[0]);
        stmnt->setUInt(columnIndex++, poke.ivs[1]);
        stmnt->setUInt(columnIndex++, poke.ivs[2]);
        stmnt->setUInt(columnIndex++, poke.ivs[3]);
        stmnt->setUInt(columnIndex++, poke.ivs[4]);
        stmnt->setUInt(columnIndex++, poke.ivs[5]);
        stmnt->setUInt(columnIndex++, poke.evs[0]);
        stmnt->setUInt(columnIndex++, poke.evs[1]);
        stmnt->setUInt(columnIndex++, poke.evs[2]);
        stmnt->setUInt(columnIndex++, poke.evs[3]);
        stmnt->setUInt(columnIndex++, poke.evs[4]);
        stmnt->setUInt(columnIndex++, poke.evs[5]);
        teamIndex++;
    }

    executeInsert(stmnt.get());
}

size_t MariaDBConnection::executeInsertAndGetID(sql::PreparedStatement* stmnt){
    std::unique_lock lk(mostRecentKeyMutex);
    delete stmnt->executeQuery();

    std::unique_ptr<sql::PreparedStatement> keyStmnt(conn->prepareStatement("select LAST_INSERT_ID()"));
    std::unique_ptr<sql::ResultSet> results(keyStmnt->executeQuery());
    results->next();
    size_t key = results->getUInt64(1);
    return key;
}

void MariaDBConnection::executeInsert(sql::PreparedStatement* stmnt){
    std::unique_lock lk(mostRecentKeyMutex);
    delete stmnt->executeQuery();
}

std::optional<User> MariaDBConnection::getUserData(const std::string& username){
    std::unique_ptr<sql::PreparedStatement> selectStmnt(conn->prepareStatement("select id,name,accountCreated,lastPasswordChange,email from user where name = ?"));
    selectStmnt->setString(1, username);
    std::unique_ptr<sql::ResultSet> results(selectStmnt->executeQuery());
    std::optional<User> result;
    if (results->rowsCount() != 1){
        return result;
    }
    results->next();

    result.emplace(
        results->getUInt64(1),
        std::string(results->getString(2)),
        std::string(results->getString(3)),
        std::string(results->getString(4)),
        std::string(results->getString(5))
    );

    return result;
}

std::vector<Trainer> MariaDBConnection::getUserTrainers(const std::string& username){
    size_t userID = userIdFromName(username);
    std::unique_ptr<sql::PreparedStatement> selectStmnt(conn->prepareStatement("select id from trainer where user = ?"));
    selectStmnt->setUInt64(1, userID);

    std::unique_ptr<sql::ResultSet> results(selectStmnt->executeQuery());
    
    std::vector<Trainer> result;
    result.reserve(results->rowsCount());
    while (results->next()){
        result.push_back(getTrainer(results->getUInt64(1)).value());
    }
    return result;
}

std::vector<TournamentResults> MariaDBConnection::getUserTournaments(const std::string& username){
    size_t userID = userIdFromName(username);
    std::unique_ptr<sql::PreparedStatement> selectStmnt(conn->prepareStatement("select tournament from saved_tournaments where user = ?"));
    selectStmnt->setUInt64(1, userID);

    std::unique_ptr<sql::ResultSet> results(selectStmnt->executeQuery());
    
    std::vector<TournamentResults> result;
    result.reserve(results->rowsCount());
    while (results->next()){
        result.push_back(getTournament(results->getUInt64(1)).value());
    }
    return result;
}

void MariaDBConnection::saveTournamentToUser(size_t user, size_t tournament){
    std::unique_ptr<sql::PreparedStatement> insertStmnt(conn->prepareStatement("insert into saved_tournaments (user,tournament) values (?,?)"));
    insertStmnt->setUInt64(1, user);
    insertStmnt->setUInt64(2, tournament);

    executeInsert(insertStmnt.get());
}

void MariaDBConnection::deleteSavedTrainer(size_t user, size_t trainer){
    std::unique_ptr<sql::PreparedStatement> deleteStmnt(conn->prepareStatement("delete from trainer where user = ? and id = ? and tournament is null"));
    deleteStmnt->setUInt64(1, user);
    deleteStmnt->setUInt64(2, trainer);

    delete deleteStmnt->executeQuery();
}
void MariaDBConnection::deleteSavedTournament(size_t user, size_t tournament){
    std::unique_ptr<sql::PreparedStatement> deleteStmnt(conn->prepareStatement("delete from saved_tournaments where user = ? and tournament = ?"));
    deleteStmnt->setUInt64(1, user);
    deleteStmnt->setUInt64(2, tournament);

    delete deleteStmnt->executeQuery();
}

bool MariaDBConnection::tournamentExists(size_t id){
    std::unique_ptr<sql::PreparedStatement> selectStmnt(conn->prepareStatement("select id from tournament where id = ?"));
    selectStmnt->setUInt64(1, id);

    std::unique_ptr<sql::ResultSet> results(selectStmnt->executeQuery());

    return results->rowsCount();
}

bool MariaDBConnection::trainerExists(size_t id, size_t user){
    std::string sql = "select id from trainer where id = ?";
    if (user){
        sql += "and user = ?";
    }
    std::unique_ptr<sql::PreparedStatement> selectStmnt(conn->prepareStatement(sql));
    selectStmnt->setUInt64(1, id);
    if (user){
        selectStmnt->setUInt64(2, user);
    }

    std::unique_ptr<sql::ResultSet> results(selectStmnt->executeQuery());

    return results->rowsCount();
}