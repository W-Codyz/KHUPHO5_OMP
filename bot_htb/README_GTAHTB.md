# GTA-HTB Discord Bot

Bot Discord cho máy chủ GTA-HTB Roleplay với các chức năng xác minh tài khoản và quản lý người chơi.

## 📋 Các lệnh đã được cập nhật

### 1. `/xacminh` - Liên kết tài khoản Discord với Game
**File:** `commands/xacminh_new.js`

**Cách hoạt động:**
- Chỉ sử dụng được trong kênh xác minh (#xac-minh-tai-khoan)
- Người chơi nhập mã 6 chữ số nhận được từ game
- Bot kiểm tra mã trong database (bảng `accounts`, cột `DiscordCode`)
- Nếu đúng:
  - Cập nhật `DiscordCode = 0` và `DiscordID = <discord_id>` trong database
  - Thêm role "Verified" cho người dùng
  - Đổi nickname thành tên nhân vật trong game
- Gửi thông báo thành công/thất bại

**Sử dụng:**
```
/xacminh code:123456
```

### 2. `/playerinfo` - Xem thông tin người chơi (Admin only)
**File:** `commands/playerinfo_new.js`

**Cách hoạt động:**
- Chỉ admin mới sử dụng được (có role ID: `1403988721266856059`)
- Truy vấn database lấy thông tin chi tiết người chơi
- Hiển thị:
  - Thông tin cơ bản (tên, ngày tạo, đăng nhập cuối)
  - Tài chính (tiền mặt, ngân hàng)
  - Chỉ số (máu, giáp)
  - Thông tin khác (số điện thoại, skin ID)
  - Trạng thái xác minh Discord

**Sử dụng:**
```
/playerinfo username:TenNhanVat
```

## 🔧 Cài đặt

### 1. Cài đặt dependencies
```bash
cd bot_htb
npm install
```

### 2. Cấu hình môi trường (.env)
File `.env` đã được cập nhật với thông tin GTAHTB:
- `BOT_TOKEN`: Token của bot Discord
- `DB_DATABASE`: `gtahtb` (tên database)
- `VERIFICATION_CHANNEL_ID`: `1399386568468463663` (kênh xác minh)
- `VERIFIED_ROLE_ID`: `1430358438701174804` (role verified)
- `ADMIN_ROLE_ID`: `1403988721266856059` (role admin)
- `GUILD_ID`: `1399386558968238242` (ID server Discord)

### 3. Thay thế file commands cũ
```bash
# Backup file cũ
mv commands/xacminh.js commands/xacminh_old.js
mv commands/thongtin.js commands/thongtin_old.js

# Đổi tên file mới
mv commands/xacminh_new.js commands/xacminh.js
mv commands/playerinfo_new.js commands/playerinfo.js
```

### 4. Chạy bot
```bash
node index.js
```

## 📊 Database Schema

### Bảng `accounts`
Các cột được sử dụng:
- `id`: ID tài khoản
- `Username`: Tên nhân vật
- `DiscordCode`: Mã xác minh 6 chữ số (0 nếu đã xác minh)
- `DiscordID`: Discord User ID (0 hoặc rỗng nếu chưa liên kết)
- `RegiDate`: Ngày đăng ký
- `LastLogin`: Lần đăng nhập cuối
- `Money`: Tiền mặt
- `Bank`: Tiền ngân hàng
- `pHealth`: Máu (%)
- `pArmor`: Giáp (%)
- `Model`: Skin ID
- `PhoneNr`: Số điện thoại

## 🔄 So sánh với DCMD (Pawn)

### DCMD:xacminh → /xacminh
| DCMD (Pawn) | Discord.js Bot |
|-------------|----------------|
| `DCC_GetInteractionContent()` | `interaction.options.getString()` |
| `DCC_HasGuildMemberRole()` | `member.roles.cache.has()` |
| `DCC_AddGuildMemberRole()` | `member.roles.add()` |
| `DCC_SetGuildMemberNickname()` | `member.setNickname()` |
| `mysql_query()` | `connection.execute()` |

### DCMD:playerinfo → /playerinfo
| DCMD (Pawn) | Discord.js Bot |
|-------------|----------------|
| `SendInteractionEmbed()` | `EmbedBuilder` |
| `cache_get_field_content()` | `results[0].fieldName` |
| `FormatMoney()` | `Intl.NumberFormat()` |

## 🎨 Features

✅ Xác minh tài khoản tự động  
✅ Kiểm tra quyền admin  
✅ Hiển thị thông tin đẹp mắt với Embed  
✅ Format tiền tệ chuẩn  
✅ Hiển thị ảnh skin từ open.mp  
✅ Xử lý lỗi an toàn  
✅ Ephemeral replies (tin nhắn riêng tư)  

## 📝 Lưu ý

1. **Bot Token**: Đã được cập nhật trong `.env`, nếu bot không hoạt động, cần regenerate token mới
2. **Database**: Đảm bảo database `gtahtb` đã tồn tại và có bảng `accounts`
3. **Permissions**: Bot cần quyền:
   - Manage Roles
   - Manage Nicknames
   - Send Messages
   - Embed Links

## 🚀 Nâng cấp từ bot cũ

1. Logic xác minh giống hệt DCMD trong game
2. Sử dụng `DiscordCode` thay vì `Code_XacMinh`
3. Hiển thị skin avatar từ open.mp
4. Format money chuẩn quốc tế
5. Ephemeral replies để bảo mật

---
**Phát triển bởi:** Team GTAHTB  
**Phiên bản:** 2.0 (Cập nhật theo DCMD)
