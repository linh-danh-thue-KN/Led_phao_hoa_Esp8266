# Led_phao_hoa_Esp8266
Led_ws2812/UC1903-800khz
# 🎇 Dự án ESP8266 Điều Khiển LED WS2812 qua Chân RX

Đây là một dự án sử dụng ESP8266 để điều khiển dải LED WS2812 (NeoPixel) với hiệu ứng pháo hoa. Hệ thống hỗ trợ cập nhật firmware OTA (Over-The-Air) thông qua trình duyệt tại địa chỉ `/update`.

---

## 🧰 Phần cứng sử dụng

| Thiết bị           | Mô tả                         |
|--------------------|-------------------------------|
| ESP8266            | NodeMCU / Wemos D1 mini       |
| LED WS2812         | Dải LED RGB địa chỉ hóa       |
| Nguồn 5V           | Cấp nguồn cho LED             |
| Điện trở 330Ω      | Nối giữa RX và chân DIN của LED |
| Tụ 1000uF          | (Khuyến nghị) lọc nguồn LED   |

> ⚠️ LED được nối vào chân **RX (GPIO3)** – cần lưu ý khi nạp và debug.

---

## 🔌 Kết nối phần cứng

| ESP8266 Pin | LED WS2812 |
|-------------|-------------|
| RX (GPIO3)  | DIN         |
| GND         | GND         |
| 5V          | VCC         |

---

## 🧠 Tính năng phần mềm

- 🌈 Hiển thị hiệu ứng LED pháo hoa
- 🌐 Tạo webserver điều khiển hiệu ứng (tùy chọn)
- 🛠️ Hỗ trợ cập nhật firmware qua OTA (`/update`)
- 🚀 Tự động kết nối WiFi, có thể cấu hình qua mã nguồn

---

## ⚙️ Cài đặt

1. Clone dự án:

   ```bash
   git clone https://github.com/linh-danh-thue-KN/Led_phao_hoa_Esp8266.git
