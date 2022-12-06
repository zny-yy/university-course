'''
File: _turtle_
Autor: yuen
time: 2022/10/3
'''
import turtle as t
t.speed(0) # 调绘画速度

def draw():  #画小方格
    for i in range(4):
        t.forward(30)
        t.left(90)
    t.forward(30)

def pen_change(i):  # 更改坐标的位置
    t.penup()
    t.goto(-200+i*30, 200-i*30)
    t.pendown()

def chess_board(number, i):  # 主函数 画棋盘
    if number == 0:
        return
    else:
        pen_change(i) # 确定起始位置
        for k in range(number): # 行循环
            if k % 2 != 0:
                clr = 'black'
            else:
                clr = 'white'
            t.fillcolor(clr)
            t.begin_fill()
            draw()
            t.end_fill()
        pen_change(i)
        t.right(90)
        for j in range(number-1): # 列循环
            if j % 2 == 0:
                clr = 'black'
            else:
                clr = 'white'
            t.fillcolor(clr)
            t.begin_fill()
            draw()
            t.end_fill()
        t.left(90)
        chess_board(number - 1, i + 1)
i = 0
chess_board(8, i)
t.done()
