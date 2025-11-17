/* Discord_SlashCommand()
{
    DCC_CreateCommand("xacminh", "Liên kết tài khoản", "DCMD_xacminh", _, mainGuild);
    DCC_CreateCommand("playerinfo", "Xem thông tin tài khoản", "DCMD_playerinfo", _, mainGuild);
} */

/* DCMD:playerinfo(interaction, user)
{
    new DCC_Guild:guild = mainGuild;
    new DCC_Role:role = dcRole_Admin;
    new bool:hasRole;
    
    DCC_HasGuildMemberRole(guild, user, role, hasRole);
    
    if(!hasRole)
        return SendInteractionEmbed(interaction, user, 0xFF0000, "❌ Lỗi quyền hạn", "Bạn không có quyền sử dụng lệnh này.");
    
    new content[MAX_PLAYER_NAME];
    DCC_GetInteractionContent(interaction, content);
    
    if(strlen(content) == 0)
        return SendInteractionEmbed(interaction, user, 0xFF0000, "❌ Sai cú pháp", "Vui lòng nhập tên tài khoản cần tra cứu.");
    
    new query[512];
    mysql_format(MainPipeline, query, sizeof(query), 
        "SELECT `RegiDate`, `LastLogin`, `Username`, `Money`, `Bank`, `pHealth`, `pArmor`, `Model`, `PhoneNr`, `DiscordID`, `DiscordCode` \
        FROM `accounts` WHERE `Username` = '%e' LIMIT 1", content);
    
    new Cache:Result = mysql_query(MainPipeline, query);
    
    if(!cache_num_rows())
    {
        cache_delete(Result);
        return SendInteractionEmbed(interaction, user, 0xFF0000, "❌ Không tìm thấy", "Không tìm thấy tài khoản với tên: ***%s***", content);
    }
    
    new RegiDate[64], LastLogin[64], Username[MAX_PLAYER_NAME], DiscordID[DCC_ID_SIZE];
    new Money, Bank, PhoneNr, Model, DiscordCode;
    new szHealth[32], szArmor[32];
    
    cache_get_field_content(0, "RegiDate", RegiDate, MainPipeline, sizeof(RegiDate));
    cache_get_field_content(0, "LastLogin", LastLogin, MainPipeline, sizeof(LastLogin));
    cache_get_field_content(0, "Username", Username, MainPipeline, sizeof(Username));
    cache_get_field_content(0, "DiscordID", DiscordID, MainPipeline, sizeof(DiscordID));
    cache_get_field_content(0, "pHealth", szHealth, MainPipeline);
    cache_get_field_content(0, "pArmor", szArmor, MainPipeline);
    
    Money = cache_get_field_content_int(0, "Money", MainPipeline);
    Bank = cache_get_field_content_int(0, "Bank", MainPipeline);
    PhoneNr = cache_get_field_content_int(0, "PhoneNr", MainPipeline);
    Model = cache_get_field_content_int(0, "Model", MainPipeline);
    DiscordCode = cache_get_field_content_int(0, "DiscordCode", MainPipeline);
    
    cache_delete(Result);
    
    new verifyStatus[64];
    if(strcmp(DiscordID, "0") != 0 && strlen(DiscordID) > 0)
        format(verifyStatus, sizeof(verifyStatus), "✅ Đã xác thực");
    else
        format(verifyStatus, sizeof(verifyStatus), "❌ Chưa xác thực (Code: %d)", DiscordCode);
    
    new discordTag[128];
    if(strcmp(DiscordID, "0") == 0 || strlen(DiscordID) == 0)
        format(discordTag, sizeof(discordTag), "Chưa liên kết");
    else
        format(discordTag, sizeof(discordTag), "<@%s>", DiscordID);
    
    SendInteractionEmbed(interaction, user, 0x3498DB, "📋 THÔNG TIN NHÂN VẬT", 
        "**👤 Tên nhân vật:** %s\n\
        **📅 Ngày tạo:** %s\n\
        **🕐 Đăng nhập cuối:** %s\n\
        **💵 Tiền mặt:** $%s\n\
        **🏦 Ngân hàng:** $%s\n\
        **❤️ Máu:** %s\n\
        **🛡️ Giáp:** %s\n\
        **📱 Số điện thoại:** %d\n\
        **🎭 Skin ID:** %d\n\
        **💬 Discord:** %s\n\
        **✅ Trạng thái:** %s",
        Username, RegiDate, LastLogin,
        FormatMoney(Money), FormatMoney(Bank),
        szHealth, szArmor, PhoneNr, Model,
        discordTag, verifyStatus
    );
    
    return 1;
}

DCMD:xacminh(interaction, user)
{
    new DCC_Channel:channelid, bool:hasRole;
    DCC_GetInteractionChannel(interaction, channelid);
    DCC_HasGuildMemberRole(mainGuild, user, dcRole_Verified, hasRole);

    if(hasRole) 
        return SendInteractionEmbed(interaction, user, 0xFF9900, "⚠️ Liên kết không thành công", "Bạn đã liên kết tài khoản rồi, không cần thực hiện lại.\n\n*Nếu có vấn đề gì, vui lòng liên hệ đội ngũ phát triển!*");

    if(channelid != dc_Verify) 
        return SendInteractionEmbed(interaction, user, 0xFF0000, "❌ Liên kết không thành công", "Câu lệnh chỉ có thể sử dụng tại kênh <#1399386568468463663>.");
    
    new content[32];
    DCC_GetInteractionContent(interaction, content);
    if(!IsNumeric(content) || strlen(content) != 6)
        return SendInteractionEmbed(interaction, user, 0xFF0000, "❌ Liên kết không thành công", "Mã liên kết không hợp lệ, vui lòng nhập đúng 6 chữ số.");
    
    new realCode = strval(content);
    new Cache:sqlCache, query[256];
    mysql_format(MainPipeline, query, sizeof(query), "SELECT `id`, `Username`, `DiscordCode` FROM `accounts` WHERE DiscordCode = '%d'", realCode);
    sqlCache = mysql_query(MainPipeline, query);

    if(cache_num_rows() == 0) {
        cache_delete(sqlCache);
        return SendInteractionEmbed(interaction, user, 0xFF0000, "❌ Liên kết không thành công", "Không tìm thấy tài khoản với mã liên kết này.");
    }

    new accountId, discordCode, accountName[MAX_PLAYER_NAME+1];
    accountId = cache_get_field_content_int(0, "id");
    discordCode = cache_get_field_content_int(0, "DiscordCode");
    cache_get_field_content(0, "Username", accountName);
    cache_delete(sqlCache);

    if(discordCode == realCode) {
        new discordId[DCC_ID_SIZE];
        DCC_GetUserId(user, discordId);
        DCC_AddGuildMemberRole(mainGuild, user, dcRole_Verified);
        DCC_SetGuildMemberNickname(mainGuild, user, accountName);

        mysql_format(MainPipeline, query, sizeof(query), "UPDATE `accounts` SET `DiscordCode` = 0, `DiscordID` = '%s' WHERE `id` = '%d'", discordId, accountId);
        mysql_tquery(MainPipeline, query, "", "");

        foreach(new playerid: Player) {
            if(GetPlayerSQLId(playerid) == accountId) {
                PlayerInfo[playerid][pDiscordCode] = 0;
                format(PlayerInfo[playerid][pDiscordID], _, "%s", discordId);
                SendClientMessageEx(playerid, 0x7DFFDEFF, "Tai khoan cua ban da duoc lien ket thanh cong voi tai khoan Discord.");
                SendClientMessageEx(playerid, 0x7DFFDEFF, "Neu day khong phai hanh dong cua ban, vui long bao cao ngay cho doi ngu phat trien.");
                break;
            }
        }

        SendInteractionEmbed(interaction, user, 0x00FF00, "✅ Liên kết thành công", "Tài khoản Discord của bạn đã được liên kết thành công đến tài khoản trò chơi ***%s***\n\nBây giờ bạn có thể thực hiện các thao tác riêng tư/bảo mật với tài khoản của mình.\n\n***Chúc bạn có một trải nghiệm thật tuyệt tại GTA-HTB Roleplay! 🎮***", accountName);
    }
    else {
        SendInteractionEmbed(interaction, user, 0xFF0000, "❌ Liên kết không thành công", "Mã liên kết không hợp lệ.");
    }
    return 1;
} */