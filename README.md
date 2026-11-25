# Khu Phố 5 Roleplay - Next Generation Gaming trên OPEN.MP

Đây là gamemode roleplay cho San Andreas Multiplayer chạy trên nền tảng [open.mp](https://open.mp/). Dự án được phát triển dựa trên gamemode **Next Generation Gaming open.mp (NGG)** của [Ausil](https://github.com/aujiz11) và có các phần chỉnh sửa, bổ sung riêng trong repository này.

> Đây là một bản tùy biến từ dự án gốc, không phải bản phát hành chính thức của upstream. Khi sử dụng hoặc tiếp tục phát triển, vui lòng giữ lại ghi nhận tác giả và tuân thủ giấy phép của mã nguồn/thư viện đi kèm.

## Nguồn gốc

- Gamemode upstream: [aujiz11/NGG-open.mp](https://github.com/aujiz11/NGG-open.mp)
- Repository của bản này: [W-Codyz/KHUPHO5_OMP](https://github.com/W-Codyz/KHUPHO5_OMP)

## Tính năng

Các tính năng dưới đây được tổng hợp từ những module đang được nạp trong `gamemodes/main.pwn`:

### Nhân vật và đời sống roleplay

- Tạo và tải nhân vật, hướng dẫn người chơi mới, nhiệm vụ newbie, thống kê nhân vật và trợ giúp trong game.
- Hệ thống nhà, phương tiện cá nhân, thuê xe, đồng hồ tốc độ, nhiên liệu và bình xăng.
- Inventory, tương tác vật phẩm, thẻ nhân vật, điện thoại, GPS và chia sẻ vị trí.
- Ăn uống, sức khỏe, cứu thương, thu thập thảo dược và chế tạo/sử dụng medkit.
- Tòa thị chính, giấy phép và các tiện ích di chuyển vào/ra khu vực, công trình.

### Nghề nghiệp và hoạt động

- Taxi (có đánh giá tài xế), tài xế xe tải, vận chuyển hàng và giao container.
- Giao pizza, xây dựng, thợ điện, khai thác mỏ và chế tạo vật phẩm/vũ khí.
- Đánh cá, khai thác gỗ, thợ sơn và quét đường.
- Hoạt động cần sa trong game: trồng/thu hoạch, chế biến, bán hoặc giao hàng; có cảnh báo cho cảnh sát và rủi ro bị bắt.
- Mechanic: kéo xe, sửa chữa, chế tạo dụng cụ và quản lý điểm sửa chữa/kho vật liệu.
- Một số hoạt động và sự kiện riêng và các bàn roulette/cược.

### Tổ chức, kinh tế và quản trị

- Faction/tổ chức với quản lý thành viên, cấp bậc và các chức năng dành cho cảnh sát.
- Gia đình/gangster: quản lý tài sản, kho vũ khí và hoạt động chiếm đóng địa bàn (turf war).
- Ngân hàng, thanh toán hóa đơn, lương/payday và các tính năng ngân hàng qua điện thoại.
- Lao động trong tù, khu vực an toàn, thông báo hỗ trợ và các công cụ quản trị máy chủ.

### Giao diện và tích hợp

- Dialog, textdraw, menu/panel và giao diện điện thoại.
- Voice chat và tích hợp Discord.
- Bản đồ, actor và các nội dung thế giới được tổ chức thành module riêng.
- Các kiểm tra chống gian lận trong những nhóm như tốc độ, dịch chuyển, vũ khí, spam và VPN.

Một số module trong repository chưa được nạp hoặc đang bị comment trong `main.pwn`; chúng không được tính là tính năng đang bật ở danh sách này. Hoạt động thực tế còn phụ thuộc cấu hình, database và các plugin/component tương ứng.

## Đóng góp và báo lỗi

Nếu gặp lỗi trong bản tùy biến này, hãy tạo [GitHub Issue](https://github.com/W-Codyz/KHUPHO5_OMP/issues) và cung cấp log liên quan cùng các bước tái hiện. Không đăng thông tin đăng nhập hoặc dữ liệu người chơi trong issue.

## Credits

- **Tác giả bản tùy biến này:** [Codyz](https://github.com/W-Codyz).
- **Tác giả gamemode nền tảng NGG-open.mp:** [Ausil](https://github.com/aujiz11).
- Cảm ơn cộng đồng open.mp và các tác giả plugin, thư viện được sử dụng trong dự án.
