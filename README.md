<img width="1743" height="777" alt="image" src="https://github.com/user-attachments/assets/9a4ea565-4be0-4cfb-87de-61ab7c0dbb02" />STM32-CAN :

Đối với project này, mình sử dụng màn hình RGB043M1-480×272-V1.4 kích thước 4.3 inch, kết nối với STM32 thông qua giao tiếp LTDC. Giao diện người dùng (UI) được thiết kế bằng TouchGFX, đây là một công cụ thuộc hệ sinh thái STM32, nên khá quen thuộc và dễ tích hợp với STM32.

Lưu ý:

Giao diện Dashboard của project này nằm trong thư mục TouchGFX/gui. Thư mục chứa phần UI được thiết kế và export bằng TouchGFX Designer.


<img width="942" height="527" alt="image" src="https://github.com/user-attachments/assets/3b59480b-5225-4028-bccf-b05ab39f8f12" />


Driver của màn hình nằm trong thư mục UserLibrary.

Model nhận/xử lý dữ liệu → ModelListener chuyển tiếp event đến Presenter → Presenter xử lý/quyết định dữ liệu nào cần được truyền đến View → View cập nhật các widget, sau đó TouchGFX render giao diện.

Trong hàm main.c, chúng ta chỉ cần khởi tạo TouchGFX bằng MX_TouchGFX_Init(); và liên tục gọi MX_TouchGFX_Process(); bên trong vòng lặp while.

Giao Tiếp QSPI & Chế độ Memory-Mapped:

STM32H7 có một chế độ thần thánh gọi là Memory-Mapped Mode. Khi cấu hình QSPI vào chế độ này, MCU sẽ tự động "ảo hóa" toàn bộ nội dung của con chip QSPI (W25Q64) thành một vùng nhớ nội bộ bắt đầu tại địa chỉ 0x90000000. Nhờ vậy, TouchGFX và DMA2D có thể đọc dữ liệu ảnh trực tiếp như thể nó nằm trong chip MCU, không cần phải gọi lệnh SPI Read rườm rà.

<img width="1743" height="777" alt="image" src="https://github.com/user-attachments/assets/89eae1b3-cd36-409a-a102-05a76b5fdadc" />
<img width="1753" height="275" alt="image" src="https://github.com/user-attachments/assets/5a52b137-b157-4c9c-88fd-c5107d3a0b70" />

link youtube:https://youtu.be/OMPtFaEfwmk?si=MM4eXs9ruhRzD4XI

