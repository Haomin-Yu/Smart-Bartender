import sensor,image,lcd
import KPU as kpu
from fpioa_manager import fm
from machine import UART
from board import board_info
import utime


lcd.init()
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing((224, 224))
sensor.set_vflip(1)
sensor.run(1)
fm.register(board_info.PIN15,fm.fpioa.UART1_TX)
fm.register(board_info.PIN17,fm.fpioa.UART1_RX)
#fm.register(board_info.PIN5,fm.fpioa.UART1_TX)
#fm.register(board_info.PIN4,fm.fpioa.UART1_RX)
#uart_A = UART(UART.UART1, 115200, 8, None, 1, timeout=1000, read_buf_len=4096)
uart_A = UART(UART.UART1, 9600, 8, None, 1, timeout=1000, read_buf_len=4096)


classes = ["license"]

task = kpu.load('/sd/m3.kmodel')
anchor = (0.57273, 0.677385, 1.87446, 2.06253, 3.33843, 5.47434, 7.88282, 3.52778, 9.77052, 9.16828)
a = kpu.init_yolo2(task, 0.3, 0.3, 5, anchor)
while(True):
    img = sensor.snapshot()
    #.rotation_corr(z_rotation=90.0)
    #a = img.pix_to_ai()
    code = kpu.run_yolo2(task, img)
    if code:
        for i in code:
            a = img.draw_rectangle(i.rect(),color = (0, 255, 0))
            a = img.draw_string(i.x(),i.y(), classes[i.classid()], color=(255,0,0), scale=3)
        uart_A.write(b'0xAA')
        a = lcd.display(img)
        a = lcd.draw_string(50, 50, "Detected", lcd.BLACK, lcd.WHITE)
        utime.sleep(5)
        #break
    else:
        a = lcd.display(img)
        #uart_A.write(b'0x55')
a = kpu.deinit(task)
uart_A.deinit()
del uart_A
