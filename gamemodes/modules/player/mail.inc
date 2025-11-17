stock SendMailVerify(playerid, const email[], code)
{
    new message[2038];
    format(message, _, "\
    <!DOCTYPE html>\n\
    <html lang='vi'>\n\
    <head>\n\
        <meta charset='UTF-8'>\n\
        <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n\
        <title>Email Xác Thực</title>\n\
        <style>\n\
            body {\n\
                font-family: Arial, sans-serif;\n\
                background-color: #f4f4f4;\n\
                margin: 0;\n\
                padding: 0;\n\
            }\n\
            .container {\n\
                max-width: 600px;\n\
                margin: 50px auto;\n\
                background-color: #ffffff;\n\
                padding: 20px;\n\
                border-radius: 10px;\n\
                box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);\n\
            }\n\
            h1 {\n\
                text-align: center;\n\
                color: #333333;\n\
            }\n\
            p {\n\
                font-size: 16px;\n\
                color: #666666;\n\
            }\n\
            .code {\n\
                display: block;\n\
                width: fit-content;\n\
                margin: 20px auto;\n\
                padding: 10px 20px;\n\
                background-color: #f0f0f0;\n\
                border: 1px solid #dddddd;\n\
                border-radius: 5px;\n\
                font-size: 24px;\n\
                color: #333333;\n\
            }\n\
        </style>\n\
    </head>\n\
    <body>\n\
        <div class='container'>\n\
            <h1>Xác Thực Email</h1>\n\
            <p>Xin chào <b><i>%s</i></b>,</p>\n\
            <p>Chúng tôi đã nhận được yêu cầu xác thực email của bạn. Vui lòng sử dụng mã xác thực dưới đây để hoàn tất quá trình:</p>\n\
            <div class='code'>%d</div>\n\
            <p>Mã xác thực có thời hạn là 10 phút kể từ khi gửi.</p>\n\
            <p>Nếu bạn không yêu cầu xác thực email này, vui lòng bỏ qua email này.</p>\n\
        </div>\n\
    </body>\n\
    </html>", GetPlayerNameEx(playerid), code);

    Mail_Send(playerid, Mail:MAIL_VERIFY, email, M_MAIL_NAME, "Verify your Email", message, MAIL_TYPE_HTML);
}

Mail_Response:MAIL_VERIFY(playerid, response_code, const data[])
{
    switch (response_code) {
        case 200: {
            new email[128];
            GetPVarString(playerid, "pTmpEmail", email, sizeof(email));
            ShowPlayerDialog(playerid, DIALOG_MAIL_VERIFY, DIALOG_STYLE_INPUT, "{FFFFFF}Xac thuc Email", "{FFFFFF}Ma xac thuc da duoc gui den {974CED}%s{FFFFFF}, hay kiem\ntra hop thu Gmail cua ban de lay ma xac thuc, neu khong thay\nhay truy cap vao hop thu rac (spam) de xem.\n\n{F3E351}Luu y: Ma xac thuc co thoi gian la 10 phut.", "Xac thuc", "Dong", email);
        }
        default: {
            DeletePVar(playerid, "pTmpCodeTime");
            DeletePVar(playerid, "pTmpCode");
            DeletePVar(playerid, "pTmpEmail");
            SendClientMessageEx(playerid, COLOR_LIGHTRED, "Khong the gui Email xac thuc, vui long thu lai hoac lien he nha phat trien.");
        }
    }
    return 1;
}