CreateIDCard(playerid)
{
    new rand_id = Random(10000000, 99999999),
        Cache:sqlCache, query[256], result;

    mysql_format(MainPipeline, query, sizeof(query), "SELECT COUNT(*) FROM accounts WHERE IDCard=%d", rand_id);
    sqlCache = mysql_query(MainPipeline, query);

    result = cache_get_field_content_int(0, "COUNT(*)");

    cache_delete(sqlCache);

    if(result > 0) return CreateIDCard(playerid);
    
    PlayerInfo[playerid][pIDCard] = rand_id;
    UpdatePlayerInteger(playerid, "IDCard", rand_id);
    return 1;
}

ShowCityHallDialog(playerid)
{
    ShowPlayerDialog(playerid, DIALOG_CITYHALL, DIALOG_STYLE_LIST, "{FFFFFF}W_Cody", "Lam the can cuoc cong dan", "Chon", "Dong");
    return 1;
}