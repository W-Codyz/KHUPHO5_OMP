/* Discord_SlashCommand()
{
    DCC_CreateCommand("xacminh", "Liên kết tài khoản.", "DCMD_xacminh", _, mainGuild);
}

DCMD:xacminh(interaction, user)
{
    new DCC_Channel:channelid, bool:hasRole;
    DCC_GetInteractionChannel(interaction, channelid);
    DCC_HasGuildMemberRole(mainGuild, user, dcRole_Verified, hasRole);

    #if defined BETA_TEST
        SendInteractionEmbed(interaction, user, 0x83F752, "❌ **Liên kết không thành công...**", "Chức năng liên kết tài khoản không thể sử dụng trong giai đoạn Beta Test.");
        return 1;
    #else
        if(hasRole) 
            return SendInteractionEmbed(interaction, user, 0x83F752, "❌ **Liên kết không thành công...**", "Bạn đã liên kết tài khoản rồi, không cần thực hiện lại.\n\n*Nếu có vấn đề gì, vui lòng liên hệ đội ngũ phát triển!*");

        if(channelid != dc_Verify) 
            return SendInteractionEmbed(interaction, user, 0x83F752, "❌ **Liên kết không thành công...**", "Câu lệnh chỉ có thể sử dụng tại kênh xác thực.");
        
        new content[32];
        DCC_GetInteractionContent(interaction, content);
        if(!IsNumeric(content) || strlen(content) != 6)
            return SendInteractionEmbed(interaction, user, 0x83F752, "❌ **Liên kết không thành công...**", "Mã liên kết không hợp lệ, vui lòng không để trống và nhập đúng 6 mã số.");
        
        new realCode = strval(content);
        new Cache:sqlCache, query[256];
        mysql_format(MainPipeline, query, sizeof(query), "SELECT `id`, `Username`, `DiscordCode` FROM `accounts` WHERE DiscordCode = '%d'", realCode);
        sqlCache = mysql_query(MainPipeline, query);

        if(cache_num_rows() == 0) {
            cache_delete(sqlCache);
            return SendInteractionEmbed(interaction, user, 0x83F752, "❌ **Liên kết không thành công...**", "Không tìm thấy tài khoản với mã liên kết này.");
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

            SendInteractionEmbed(interaction, user, 0x83F752, "✅ **Liên kết thành công...**", "Tài khoản Discord của bạn đã được liên kết thành công đến tài khoản trò chơi là ***%s***, bây giờ bạn có thể thực hiện các thao tác riêng tư/bảo mật với tài khoản của mình.\n\n***Chúc bạn có một trải nghiệm thật tuyệt tại GTA HTB Roleplay***", accountName);
        }
        else {
            SendInteractionEmbed(interaction, user, 0x83F752, "❌ **Liên kết không thành công...**", "Mã liên kết không hợp lệ.");
        }
        return 1;
    #endif
} */