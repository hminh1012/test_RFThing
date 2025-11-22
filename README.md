<img width="1287" height="725" alt="image" src="https://github.com/user-attachments/assets/57c24ff4-8b55-4df0-8543-341e82b0b038" />

# Hướng dẫn sử dụng RFThings

Bài viết này ghi lại cách nạp và test firmware RAK3172 trên bo mạch RFThings

## Getting Started

### Phần cứng
  - Bo mạch RFThings RF210 ![bot](https://github.com/user-attachments/assets/5f9fd242-d366-48ce-adb6-16611cc1fb12)

  - Bộ chuyển đổi USB sang UART (chip CH340 hoặc CP2102) ![23b6763e-cdb4-42fc-a400-a1afcd155419](https://github.com/user-attachments/assets/fdc31dfc-c0dc-4ad9-ad95-024f5fe23aaf)





### Phần mềm

  - Arduino IDE (khuyến nghị phiên bản v1.8.13 trở lên)
  - Firmware RUI3 mới nhất cho RAK3172: [RAK3172-E\_latest\_final.hex](https://downloads.rakwireless.com/RUI/RUI3/Image/RAK3172-E_latest_final.hex)
  - RF210 firmware ATC version 0.1.1 cho RAK3172 (trong bài viết này mình sẽ sử dụng firmware này) [firmware_rak.bin](https://github.com/hminh1012/RFThings_how_to_use/blob/1cc496a3db2c0e8acee8bd96f6cd9588102f19a2/firmware_rak.bin)
  - Phần mềm nạp firmware [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html)



## Cài đặt


### Trên bo mạch RF210:
  
  - Cắm dây như hình vẽ. ![c00561db-e13e-46f0-b7a2-3f3b5e9c03f3](https://github.com/user-attachments/assets/94624edf-67bd-419b-9b4b-7839f478e489)



  - Bật Device Manager để kiểm tra xem máy tính nhận tín hiệu của mạch nạp UART (mình hiện là COM3)
![08ebbc49-0ed0-49d8-b59b-102185328632](https://github.com/user-attachments/assets/287307df-0ac3-467d-9904-14274c778192)




### Nạp Firmware cho RAK3172 Trong STM32CubeProgrammer:

  - Nhấn giữ nút **B\_RAK (boot)** và nhấn nút **R\_RAK (reset)** sau đó thả nút **B\_RAK (boot)** để vào chế độ bootmode.
  - Chọn UART, Baudrate 115200 và nhấn Connect.
  - Mở tệp firmware_rak.bin
  - Chọn địa chỉ như trong hình ảnh sau
  - Nhấn Download để tải firmware lên

<img width="1203" height="699" alt="Untitled123" src="https://github.com/user-attachments/assets/995c229d-ae07-40e6-a4cd-0e9f7ed044aa" />


  - Sau khi tải lên thành công, nhấn nút **R\_RAK (reset)** để thoát chế độ bootmode
  - Nếu bạn bị lỗi như này thì là do lỏng dây, máy không nhận port,... Nên reset mạch, reset máy, cắm lại dây, test máy khác, test mạch khác [trust me bro!!!]

![924079c5-9fd2-4db0-8a65-740d619f5e36](https://github.com/user-attachments/assets/3c0add07-ec89-4628-99b4-b1c661e47060)


  - Bật Serial Monitor ở ArduinoIDE (hoặc Hercules,...), nếu thấy dòng chữ "Current Work Mode: LoRaWAN." thì bạn đã nạp thành công.
    
<img width="1218" height="227" alt="image" src="https://github.com/user-attachments/assets/12c3fef8-4caf-4a58-9195-3d902608ac4c" />



### Cài đặt Arduino IDE & Board ESP32

1.  Tải về và cài đặt Arduino IDE (ít nhất phiên bản v1.6.8)
2.  Khởi động Arduino IDE
3.  Vào File/Preferences --\> **Additional Board Manager URLs** (URL Trình quản lý Board bổ sung) --\> **Add URL** (Thêm URL)

<!-- end list -->

```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

4.  Mở Boards Manager. Tới Tools \> Board \> Boards Manager… --\> Tìm kiếm ESP32 và nhấn nút cài đặt cho **ESP32 by Espressif Systems** & **Install** (Cài đặt)

### Nạp code kiểm tra: RF210\_RAK3172\_bridge
RF210\_RAK3172\_bridge là file để ESP32C3 giao tiếp với RAK3172 thông qua UART. Từ đó ESP32C3 gửi lệnh cho RAK3172 đọc các cảm biến

1.  Kết nối bo mạch RF210 của bạn (cổng microUSB) vào cổng USB của máy tính (thông thường Windows sẽ tự động cài đặt driver, nếu không thì truy cập và tải [Driver CH340](https://sparks.gogo.co.nz/ch340.html) )
2.  Tải về sketch mẫu từ Github --\> [RF210\_RAK3172\_bridge](https://github.com/FabienFerrero/SUCA/tree/main/Examples/RF210_RAK3172_bridge)
3.  Giải nén và mở tệp RF210\_RAK3172\_bridge.ino (lưu ý rằng tên tệp phải giống với tên thư mục)
4.  Mở Boards Manager từ **Tools** --\> **Board** --\> **esp32** --\> **ESP32C3 Dev Module** và Chọn.
5.  Mở Boards Manager từ **Tools** --\> **Port** và Chọn Cổng (kiểm tra bước 1)
6.  Mở Boards Manager từ **Tools** --\> **USB-CDC on boot** --\> **Enable** (Bật) (cấu hình mạch ESP32C3 bật Serial Monitor)
7.  Nhấn giữ nút **B\_ESP1** (boot) và nhấn nút **ESP\_EN1** (reset), sau đó thả nút B\_ESP1 (boot) để vào chế độ bootmode.
8.  Tải lên Sketch (Ctrl+U)
9.  Sau khi tải lên sketch, nhấn **ESP\_EN1** để khởi động lại

Lúc này Serial Monitor sẽ hiện
```
ATC
setup at command
OK
```
Lưu ý phải có chữ ATC thì mới thành công.

10. Sử dụng Serial Monitor để giám sát Serial và gửi lệnh (CMD)
11. Nhập **AT?** và nhấn **Enter** và xem kết quả
12. Tiếp tục test bằng các lệnh sau

## Cảm biến nhiệt độ và độ ẩm SHT

1.  Kích hoạt cảm biến SHT, gõ:

-----

### ATC+SHT

Nó trả về:

```
ATC+SHT=1
OK
```

2.  Kiểm tra nhiệt độ, gõ:

    ATC+TEMP=?

Nó trả về kết quả bằng độ C:

```
ATC+TEMP=xx.yy degree C
OK
```

3.  Kiểm tra độ ẩm, gõ:

    ATC+HUM=?

Nó trả về kết quả bằng phần trăm độ ẩm:

```
ATC+HUM=xx%
OK
```

## Cảm biến gia tốc KX023

1.  Kích hoạt cảm biến KX023, gõ:

    ATC+KX023

Nó trả về:

```
ATC+KX023=1
OK
```

2.  Kiểm tra giá trị gia tốc trên trục x, gõ:

    ATC+AX=?

Nó trả về kết quả bằng đơn vị G:

```
ATC+AX=-x.yy
OK
```

3.  Kiểm tra giá trị gia tốc trên trục y, gõ:

    ATC+AY=?

Nó trả về kết quả bằng đơn vị G:

```
ATC+AY=x.yy
OK
```

4.  Kiểm tra giá trị gia tốc trên trục z, gõ:

    ATC+AZ=?

Nó trả về kết quả bằng đơn vị G:

```
ATC+AZ=x.yy
OK
```

![854c61d4-dea2-4a72-92ac-08ce41870bab](https://github.com/user-attachments/assets/fc85d289-67ea-48cc-82c6-8103e23d0fb4)


### Lệnh AT (mình chưa test phần này, tất nhiên rồi :Đ )

```
AT+<CMD>?: trợ giúp về <CMD>
AT+<CMD>: chạy <CMD>
AT+<CMD>=<value>: đặt giá trị
AT+<CMD>=?: lấy giá trị

AT+BOOT             R: vào chế độ bootloader để nâng cấp firmware
AT+BOOTVER          R: lấy phiên bản của RUI Bootloader
ATZ                 R: kích hoạt reset MCU
ATR                 R: khôi phục tham số mặc định
AT+DEBUG            RW: đặt log gỡ lỗi (debug)
ATE                 R: hiển thị hoặc ẩn đầu vào lệnh AT (cài đặt được lưu & không thay đổi sau khi reset)
AT+SN               R: lấy số sê-ri của thiết bị (tối đa 18 ký tự)
AT+BAT              R: lấy mức pin (volt)
AT+BUILDTIME        R: lấy thời gian build của firmware
AT+REPOINFO         R: lấy commit ID của firmware
AT+VER              R: lấy phiên bản của firmware
AT+FIRMWAREVER      R: lấy phiên bản tùy chỉnh của firmware
AT+CLIVE            R: lấy phiên bản của lệnh AT
AT+APIVER           R: lấy phiên bản của RUI API
AT+HWMODEL          R: lấy chuỗi (string) của mẫu phần cứng
AT+HWID             R: lấy chuỗi (string) của ID phần cứng
AT+ALIAS            RW: thêm một tên bí danh (alias) cho thiết bị
AT+SYSV             R: lấy Điện áp Hệ thống (System Voltage)
AT+SLEEP            W: vào chế độ ngủ trong một khoảng thời gian (ms)
AT+LPM              RW: lấy hoặc đặt chế độ năng lượng thấp (0 = tắt, 1 = bật)
AT+LPMLVL           RW: lấy hoặc đặt mức chế độ năng lượng thấp
AT+LOCK             R: khóa cổng serial
AT+PWORD            RW: đặt mật khẩu khóa cổng serial (tối đa 8 ký tự)
AT+BAUD             RW: lấy hoặc đặt tốc độ baud của cổng serial
AT+ATM              R: chuyển sang chế độ lệnh AT
AT+APPEUI           RW: lấy hoặc đặt application EUI (8 byte ở dạng hex)
AT+APPKEY           RW: lấy hoặc đặt application key (16 byte ở dạng hex)
AT+APPSKEY          RW: lấy hoặc đặt application session key (16 byte ở dạng hex)
AT+DEVADDR          RW: lấy hoặc đặt địa chỉ thiết bị (device address) (4 byte ở dạng hex)
AT+DEVEUI           RW: lấy hoặc đặt device EUI (8 byte ở dạng hex)
AT+NETID            RW: lấy định danh mạng (NetID) (3 byte ở dạng hex)
AT+NWKSKEY          RW: lấy hoặc đặt network session key (16 byte ở dạng hex)
AT+MCROOTKEY        RW: lấy Mc Root key (16 byte ở dạng hex)
AT+CFM              RW: lấy hoặc đặt chế độ yêu cầu xác nhận (0 = tắt, 1 = bật)
AT+CFS              R: lấy trạng thái xác nhận của lệnh AT+SEND cuối cùng (0 = thất bại, 1 = thành công)
AT+JOIN             RW: tham gia mạng
AT+NJM              W: lấy hoặc đặt chế độ tham gia mạng (0 = ABP, 1 = OTAA)
AT+NJS              R: lấy trạng thái tham gia (0 = chưa tham gia, 1 = đã tham gia)
AT+RECV             R: in dữ liệu nhận được cuối cùng ở định dạng hex
AT+SEND             W: gửi dữ liệu cùng với cổng ứng dụng (application port)
AT+LPSEND           W: gửi gói dữ liệu dài (tối đa 1000 byte)
AT+RETY             RW: lấy hoặc đặt số lần truyền lại của gói dữ liệu cần xác nhận
AT+ADR              RW: lấy hoặc đặt cài đặt tốc độ dữ liệu thích ứng (ADR) (0 = tắt, 1 = bật)
AT+CLASS            RW: lấy hoặc đặt lớp (class) của thiết bị (A = class A, B = class B, C = class C)
AT+DCS              RW: lấy cài đặt ETSI duty cycle (0 = vô hiệu hóa, 1 = kích hoạt)
AT+DR               RW: lấy hoặc đặt tốc độ dữ liệu (data rate)
AT+JN1DL            RW: lấy hoặc đặt độ trễ chấp nhận tham gia (join accept delay) giữa lúc kết thúc TX và cửa sổ join RX 1, tính bằng giây
AT+JN2DL            RW: lấy hoặc đặt độ trễ chấp nhận tham gia giữa lúc kết thúc TX và cửa sổ join RX 2, tính bằng giây
AT+PNM              RW: lấy hoặc đặt chế độ mạng công cộng (0 = tắt, 1 = bật)
AT+RX1DL            RW: lấy hoặc đặt độ trễ giữa lúc kết thúc TX và cửa sổ RX 1, tính bằng giây (1-15)
AT+RX2DL            RW: lấy hoặc đặt độ trễ giữa lúc kết thúc TX và cửa sổ RX 2, tính bằng giây (2-16)
AT+RX2DR            RW: lấy hoặc đặt tốc độ dữ liệu của cửa sổ RX2
AT+RX2FQ            RW: lấy tần số của cửa sổ RX2 (Hz)
AT+TXP              RW: lấy hoặc đặt công suất phát
AT+LINKCHECK        RW: lấy hoặc đặt cài đặt kiểm tra liên kết (0 = vô hiệu hóa, 1 = một lần, 2 = mỗi lần)
AT+TIMEREQ          RW: yêu cầu ngày và giờ hiện tại (0 = vô hiệu hóa, 1 = kích hoạt)
AT+LBT      _         RW: lấy hoặc đặt LoRaWAN LBT (hỗ trợ Hàn Quốc, Nhật Bản)
AT+LBTRSSI          RW: lấy hoặc đặt LoRaWAN LBT rssi (hỗ trợ Hàn Quốc, Nhật Bản)
AT+LBTSCANTIME      RW: lấy hoặc đặt thời gian quét LoRaWAN LBT (hỗ trợ Hàn Quốc, Nhật Bản)
AT+PGSLOT           RW: lấy hoặc đặt chu kỳ khe ping unicast (0-7)
AT+BFREQ            R: lấy tốc độ dữ liệu và tần số beacon (MHz)
AT+BTIME            R: lấy thời gian beacon (số giây kể từ thời điểm GPS Epoch)
AT+BGW              R: lấy tọa độ GPS của gateway, NetID và GwID
AT+LTIME            R: lấy thời gian địa phương
AT+RSSI             R: lấy RSSI của gói tin nhận được cuối cùng
AT+ARSSI            R: truy cập RSSI của tất cả các kênh đang mở
AT+SNR              R: lấy SNR của gói tin nhận được cuối cùng
AT+MASK             RW: lấy hoặc đặt channel mask để đóng hoặc mở kênh (chỉ dành cho US915, AU915, CN470)
AT+CHE              RW: lấy hoặc đặt chế độ tám kênh (chỉ dành cho US915, AU915, CN470)
AT+CHS              RW: lấy hoặc đặt chế độ một kênh (chỉ dành cho US915, AU915, CN470)
AT+BAND             RW: lấy hoặc đặt khu vực hoạt động (0 = EU433, 1 = CN470, 2 = RU864, 3 = IN865, 4 = EU868, 5 = US915,
                        6 = AU915, 7 = KR920, 8 = AS923-1 , 9 = AS923-2 , 10 = AS923-3 , 11 = AS923-4)
AT+NWM              RW: lấy hoặc đặt chế độ làm việc của mạng (0 = P2P_LORA, 1 = LoRaWAN, 2 = P2P_FSK)
AT+PFREQ            RW: cấu hình Tần số P2P (Lưu ý: Lệnh này sẽ bị xóa trong tương lai)
AT+PSF              RW: cấu hình Hệ số trải phổ (Spreading Factor) P2P (5-12)(Lưu ý: Lệnh này sẽ bị xóa trong tương lai)
AT+PBW              RW: cấu hình Băng thông P2P (LORA: 0 = 125, 1 = 250, 2 = 500, 3 = 7.8, 4 = 10.4, 5 = 15.63, 6 = 20.83,
                        7 = 31.25, 8 = 41.67, 9 = 62.5  FSK: 4800-467000)
                        (Lưu ý: Lệnh này sẽ bị xóa trong tương lai)
AT+PCR              RW: cấu hình Tỷ lệ mã hóa (Code Rate) P2P (0=4/5, 1=4/6, 2=4/7, 3=4/8)(Lưu ý: Lệnh này sẽ bị xóa trong tương lai)
AT+PPL              RW: cấu hình Độ dài Preamble P2P (5-65535)(Lưu ý: Lệnh này sẽ bị xóa trong tương lai)
AT+PTP              RW: cấu hình Công suất phát TX P2P (5-22)(Lưu ý: Lệnh này sẽ bị xóa trong tương lai)
AT+PSEND            W: gửi dữ liệu ở chế độ P2P
AT+PRECV            RW: vào chế độ P2P RX trong một khoảng thời gian (ms)
AT+ENCRY            RW: lấy hoặc đặt trạng thái mã hóa của chế độ P2P
AT+CAD              RW: lấy hoặc đặt trạng thái Phát hiện Hoạt động Kênh (CAD) của chế độ P2P
AT+ENCKEY           RW: lấy hoặc đặt khóa mã hóa của chế độ P2P (16 byte ở dạng hex)
AT+CRYPIV           RW: lấy hoặc đặt IV mã hóa của chế độ P2P (16 byte ở dạng hex)
AT+P2P              RW: lấy hoặc đặt tất cả các tham số P2P
AT+PBR              RW: lấy hoặc đặt tốc độ bit (bitrate) của modem P2P FSK (600-300000 b/s)
AT+PFDEV            RW: lấy hoặc đặt độ lệch tần số (frequency deviation) của modem P2P FSK (600-200000 hz)
AT+IQINVER          RW: lấy hoặc đặt đảo ngược IQ P2P (1 = bật, 0 = tắt)
AT+SYNCWORD         RW: lấy hoặc đặt syncword P2P (0x0000 - 0xffff)
AT+RFFREQUENCY      RW: lấy hoặc đặt Tần số P2P
AT+TXOUTPUTPOWER    RW: lấy hoặc đặt Công suất phát Tx P2P (5-22)
AT+BANDWIDTH        RW: lấy hoặc đặt Băng thông P2P (LORA: 0 = 125, 1 = 250, 2 = 500, 3 = 7.8, 4 = 10.4, 5 = 15.63, 6 = 20.83,
                        7 = 31.25, 8 = 41.67, 9 = 62.5  FSK: 4800-467000)
AT+SPREADINGFACTOR  RW: lấy hoặc đặt Hệ số trải phổ P2P (5-12)
AT+CODINGRATE       RW: lấy hoặc đặt Tỷ lệ mã hóa P2P (0=4/5, 1=4/6, 2=4/7, 3=4/8)
AT+PREAMBLELENGTH   RW: lấy hoặc đặt Độ dài Preamble P2P (5-65535)
AT+SYMBOLTIMEOUT    RW: lấy hoặc đặt symbolTimeout P2P (0-248)
AT+FIXLENGTHPAYLOAD RW: lấy hoặc đặt bật/tắt payload có độ dài cố định P2P (1 = bật, 0 = tắt)
AT+ADDMULC          W: thêm một nhóm multicast mới
AT+RMVMULC          W: xóa một nhóm multicast
AT+LSTMULC          R: xem thông tin nhóm multicast
AT+TRSSI            RW: bắt đầu kiểm tra tone RF RSSI
AT+TTONE            RW: bắt đầu kiểm tra tone RF
AT+TTX              RW: đặt số lượng gói tin được gửi để kiểm tra PER RF TX
AT+TRX              RW: đặt số lượng gói tin được nhận để kiểm tra PER RF RX
AT+TCONF            RW: cấu hình kiểm tra LoRa RF
AT+TTH              RW: bắt đầu kiểm tra nhảy tần RF TX từ Fstart đến Fstop, với các bước Fdelta
AT+TRTH             RW: bắt đầu kiểm tra nhảy tần RF TX từ Fstart đến Fstop, với khoảng Fdelta theo trình tự ngẫu nhiên
AT+TOFF             RW: dừng kiểm tra RF đang diễn ra
AT+CERTIF           RW: đặt mô-đun ở chế độ chứng nhận LoraWAN (0 = chế độ bình thường, 1 = chế độ chứng nhận)
AT+CW               RW: bắt đầu phát sóng liên tục (continuous wave)
```

### Lệnh AT tùy chỉnh (Custom)

```
ATC+VER         R: VER, Trả về phiên bản firmware

ATC+SHT         R: SHT, Trả về trạng thái của cảm biến SHTC3. 1 nếu có sẵn.
ATC+TEMP        R: TEMP, Trả về giá trị nhiệt độ với độ phân giải 0.01°
ATC+HUM         R: HUM, Trả về giá trị độ ẩm với độ phân giải 1 %

ATC+KX023       R: KX023, Trả về trạng thái của cảm biến KX023. 1 nếu có sẵn.
ATC+AX          R: AX, Trả về giá trị gia tốc trục X với độ phân giải 0.01G
ATC+AY          R: AY, Trả về giá trị gia tốc trục Y với độ phân giải 0.01G
ATC+AZ          R: AZ, Trả về giá trị gia tốc trục Z với độ phân giải 0.01G

ATC+LTR         R: LTR, Trả về trạng thái của cảm biến LTR - 303. 1 nếu có sẵn.
ATC+LUMCH0      R: LUMCH0, Trả về giá trị CHANNEL0 của cảm biến LTR - 303
ATC+LUMCH1      R: LUMCH1, Trả về giá trị CHANNEL1 của cảm biến LTR - 303
ATC+LUM         R: LUM, Trả về giá trị CHANNEL1 của cảm biến LTR - 303

ATC+GPS         R: GPS, Trả về trạng thái của mô-đun GNSS. 1 nếu có sẵn.
ATC+GPSON       W: GPSON, Trả về nguồn mô-đun GNSS | = 1 : GNSS BẬT | = 0 : GNSS TẮT
ATC+GPSPWR      W: GPSPWR, Trả về nguồn mô-đun GNSS | = 1 : GNSS BẬT | = 0 : GNSS TẮT
ATC+GPSSAT      R: GPSSAT, Trả về số lượng vệ tinh có sẵn
ATC+GPSTIME     R: GPSTIME, Trả về thời gian GPS ở định dạng EPOCH
ATC+GPSLAT      R: GPSLAT, Trả về vĩ độ | Trả về 0 nếu tọa độ không có sẵn
ATC+GPSLON      R: GPSLON, Trả về kinh độ | Trả về 0 nếu tọa độ không có sẵn
ATC+GPSALT      R: GPSALT, Trả về độ cao | Trả về 0 nếu tọa độ không có sẵn
ATC+GPSNMEA     W: GPSON, Kích hoạt log NMEA từ mô-đun GNSS | = 1 : NMEA BẬT | = 0 : NMEA TẮT
ATC+GPSLOG      W: GPSLOG, Kích hoạt kết quả từ mô-đun GNSS mỗi 5 giây | = 1 : LOG BẬT | = 0 : LOG TẮT
ATC+GPSDC       W: GPSDC, Đặt mô-đun GNSS ở chế độ duty cycle với 30 giây ngủ | = 1 : DC BẬT | = 0 : DC TẮT

ATC+BAT         R: BAT, Trả về điện áp pin tính bằng mV | Trả về 0 nếu không có sẵn
ATC+LDO         R: LDO, Trả về điện áp LDO tính bằng mV | Trả về 0 nếu không có sẵn
```

***Phiên bản firmware tùy chỉnh RAK3172 mới (chưa có sẵn)***

```
ATC+GPSCONST    R: GPSCONST, Kích hoạt các chòm sao Galileo, Beidu, GPS và Glonass
```

### Kết nối với cảm biến bằng I2C

Ở đoạn trước, bạn đã nạp firmware cầu nối RF210_RAK3172_bridge.ino để ESP32-C3 giao tiếp với RAK3172 bằng lệnh AT. Tuy nhiên, để đọc trực tiếp các cảm biến tích hợp trên bo mạch RF210 bằng ESP32-C3, chúng ta sẽ cần một sketch khác sử dụng giao tiếp I2C.
Tải thư viện [SHTC3](https://github.com/adafruit/Adafruit_SHTC3) và [Kionix KX023](https://github.com/nguyenmanhthao996tn/Kionix_KX023/)

Bạn có thể tham khảo example của thư viện. Lưu ý bạn phải set chân 
```
#include <Wire.h> // Bắt buộc phải có để sử dụng giao tiếp I2C.
```
```
Wire.begin(5, 6); // Đây là dòng code quan trọng nhất. Nó khởi tạo I2C trên ESP32-C3, gán GPIO 5 làm chân SDA và GPIO 6 làm chân SCL, khớp với kết nối trên bo mạch RF210.
```
Mình có hai code để test code cho hai cảm biến tích hợp trên bo mạch RFThings RF210: [SHTC3 (Nhiệt độ/Độ ẩm)](https://github.com/hminh1012/test_RFThing/blob/1dabc4eaf08ac55ece90685bae1e95b4c88b1523/test_SHTC3.ino) và [Kionix KX023 (Gia tốc)](https://github.com/hminh1012/test_RFThing/blob/1dabc4eaf08ac55ece90685bae1e95b4c88b1523/test_Kionix_KX023.ino).


### Dự án

Tổng quan trong dự án của mình, mình đã kết hợp cả hai cảm biến SHTC3 và KX023, sau đó gửi dữ liệu lên nền tảng Blynk và sử dụng tính năng Email để cảnh báo.
Bạn có thể tham khảo cách để thiết bị kết nối với [Blynk](https://www.blynk.io/blueprints/blink-an-led-with-esp32) và [Email](https://randomnerdtutorials.com/esp32-send-email-smtp-server-arduino-ide/)

Code này thực hiện các chức năng:
1. Đọc Nhiệt độ/Độ ẩm (SHTC3) và Gia tốc (KX023).
2. Gửi 5 giá trị cảm biến lên Blynk Cloud (V0 - V4).
3. Sử dụng gia tốc tổng hợp để phát hiện Rơi Tự do (Free Fall).
4. Nếu phát hiện Rơi Tự do (gia tốc tổng < $0.4g$), gửi cảnh báo qua Email.
<img width="1287" height="725" alt="image" src="https://github.com/user-attachments/assets/4ef5b9ce-f88d-4cb5-9721-847c09730f67" />
<img width="1161" height="410" alt="image" src="https://github.com/user-attachments/assets/a57cba75-9736-4029-b848-df97865201f5" />









### Lời cảm ơn

Dự án này sẽ không thể hoàn thành nếu không có sự đóng góp và hỗ trợ từ các bài tham khảo và trải nghiệm từ những người đi trước, cũng như Gemini pro đã đồng hành cùng mình trong việc giải thích code và vietsub lại. Cảm ơn các tác giả và team RFThings (Google nữa :Đ).

### Các liên kết tham khảo

***RFThings RF210***

  - [https://github.com/XuanMinh201/RF210](https://github.com/XuanMinh201/RF210)
  - [https://github.com/FabienFerrero/SUCA](https://github.com/FabienFerrero/SUCA)
  - [https://github.com/RFThings/rfthings-stm32l4](https://github.com/RFThings/rfthings-stm32l4)

***Hướng dẫn Arduino IDE & board ESP32***

  - [https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions)
  - [https://randomnerdtutorials.com/installing-esp32-arduino-ide-2-0](https://randomnerdtutorials.com/installing-esp32-arduino-ide-2-0)
  - [https://randomnerdtutorials.com/getting-started-with-esp32](https://randomnerdtutorials.com/getting-started-with-esp32)

***Khác***

  - [https://github.com/NamNamIoT/RAK3172\_CANOPUS](https://github.com/NamNamIoT/RAK3172_CANOPUS)
  - [https://github.com/RAKWireless/RAK-APOLLO3-RUI](https://github.com/RAKWireless/RAK-APOLLO3-RUI)
  - [https://news.rakwireless.com/stm32wl-custom-firmware-via-arduino-using-rak3172-module-for-lorawan](https://news.rakwireless.com/stm32wl-custom-firmware-via-arduino-using-rak3172-module-for-lorawan)
  - [https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Deprecated-AT-Command](https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Deprecated-AT-Command)
  - [https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Low-Level-Development](https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Low-Level-Development)
  - [https://manuals.plus/lacuna/ls200-sensor-and-relay-manual\#axzz8IXdHpl1t](https://manuals.plus/lacuna/ls200-sensor-and-relay-manual#axzz8IXdHpl1t)
  - [https://github.com/stevemarple/MicroNMEA](https://github.com/stevemarple/MicroNMEA)
  - [https://github.com/automote/LTR303](https://github.com/automote/LTR303)
