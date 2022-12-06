'''
File: _Gobang_
Autor: yuen
time: 2022/10/8
'''

# coding: utf-8
import simpleguitk as simplegui
import random

# 定义全局用户变量
WIDTH = 800
HEIGHT = 600
white_label = ['张角', '张飞', '曹操', '张辽', '刘备', '刘婵', '王允', '王朗', '孙权', '孙策', '曹仁', '李肃', '李丰',
               '杨怀', '关羽', '吕布', '陈琳', '赵云', '马超', '董卓',
               '唐僧', '白马', '悟空', '悟净', '观音', '如来', '菩提', '玉帝', '王母', '灵吉', '寿星', '老君', '文殊',
               '悟能', '龙王', '哪吒', '嫦娥', '唐王', '阎王', '李靖']
black_label = ['史进', '朱武', '林冲', '周通', '李忠', '柴进', '吴用', '刘唐', '索超', '白胜', '曹正', '武松', '孔明',
               '王英', '黄信', '秦明', '吕方', '李俊', '石勇', '张横',
               '贾演', '四姐', '袭人', '鸳鸯', '史鼎', '紫鹃', '李纨', '尤氏', '碧痕', '平儿', '香菱', '金钏', '司棋',
               '贾政', '贾敬', '媚人', '薛蟠', '焦大', '彩云', '小蝉']
piecesID = 0
wlabelID = random.randrange(0, 40)
blabelID = random.randrange(0, 40)
oldlabelID = -1
win = 0
wwinCnt = 0
bwinCnt = 0
started = False
white_group = list()
black_group = list()
matrix = [[0 for col in range(15)] for row in range(15)]


# 定义类
class ImageInfo:
    def __init__(self, center, size):
        self.center = center
        self.size = size

    def get_center(self):
        return self.center

    def get_size(self):
        return self.size


class Sprite:
    def __init__(self, pos, image, info, label):
        self.pos = [pos[0], pos[1]]
        self.image = image
        self.image_center = info.get_center()
        self.image_size = info.get_size()
        self.label = label

    def get_position(self):
        return self.pos

    def set_y(self, y):
        self.pos[1] = y

    def draw(self, canvas):
        canvas.draw_image(self.image, self.image_center, self.image_size, self.pos, [35, 35])
        canvas.draw_text(self.label, [self.pos[0] - 12, self.pos[1] + 7], 10, 'gray', 'serif')

# 加载图片
background_info = ImageInfo([300, 203], [600, 407])
background_image = simplegui.load_image("image/gb_background.jpg")

chessboard_info = ImageInfo([250, 250], [500, 500])
chessboard_image = simplegui.load_image("image/chessboard.jpg")

white_info = ImageInfo([71, 68], [143, 136])
white_image = simplegui.load_image("image/white.png")

black_info = ImageInfo([71, 68], [143, 136])
black_image = simplegui.load_image("image/black.png")

white_win_info = ImageInfo([266, 39], [532, 78])
white_win_image = simplegui.load_image("image/white_win.png")

black_win_info = ImageInfo([266, 39], [532, 78])
black_win_image = simplegui.load_image("image/black_win.png")


# 函数

def start(): #开始函数
    global started, win, piecesID, wlabelID, blabelID, oldlabelID
    white_group.clear()
    black_group.clear()
    started = True
    win = 0
    piecesID = 0
    wlabelID = random.randrange(0, 40)
    blabelID = random.randrange(0, 40)
    oldlabelID = -1
    init_matrix()


def init_matrix():
    for i in range(15):
        for j in range(15):
            matrix[i][j] = '0'


def check_line(line):
    s = ''.join(line)
    if s.find('11111') > -1 or s.find('22222') > -1:
        return 1  # white win
    if s.find('33333') > -1 or s.find('44444') > -1:
        return 2  # black win
    return 0


def check_matrix():
    idx = 0
    tl = list()
    # check rows
    for row in range(15):
        idx = check_line(matrix[row]) if check_line(matrix[row]) > idx else idx
    # check columns
    for col in range(15):
        for row in range(15):
            tl.append(matrix[row][col])
            idx = check_line(tl) if check_line(tl) > idx else idx
    # left down to right up
    for row in range(4, 15):
        for i in range(15):
            if (row - i >= 0):
                tl.append(matrix[row - i][i])
                idx = check_line(tl) if check_line(tl) > idx else idx
    for col in range(0, 11):
        for i in range(15):
            if (col + i < 15):
                tl.append(matrix[14 - i][col + i])
                idx = check_line(tl) if check_line(tl) > idx else idx
    # left up to right down
    for row in range(0, 11):
        for i in range(15):
            if (row + i < 15):
                tl.append(matrix[row + i][i])
                idx = check_line(tl) if check_line(tl) > idx else idx
    for col in range(0, 11):
        for i in range(15):
            if (col + i < 15):
                tl.append(matrix[i][col + i])
                idx = check_line(tl) if check_line(tl) > idx else idx
    return idx


def pieces_spawner(piecesID, pos):
    global started, win, wwinCnt, bwinCnt
    if started:
        tlist = list()
        tlist.extend(white_group)
        tlist.extend(black_group)
        for p in tlist:
            if (p.get_position() == pos):
                return
        if piecesID == 1 or piecesID == 2:
            pieces = Sprite(pos, white_image, white_info, white_label[oldlabelID])
            white_group.append(pieces)
        if piecesID == 3 or piecesID == 4:
            pieces = Sprite(pos, black_image, black_info, black_label[oldlabelID])
            black_group.append(pieces)
        win = check_matrix()
        if win == 1:
            wwinCnt += 1
            started = False
        if win == 2:
            bwinCnt += 1
            started = False


def draw(canvas):
    global started
    canvas.draw_image(background_image, background_info.get_center(), background_info.get_size(),
                      [WIDTH / 2, HEIGHT / 2], [WIDTH, HEIGHT])
    canvas.draw_image(chessboard_image, chessboard_info.get_center(), chessboard_info.get_size(), [300, HEIGHT / 2],
                      [600, 600])
    canvas.draw_image(white_image, white_info.get_center(), white_info.get_size(), [700, 100], [100, 100])
    canvas.draw_image(black_image, black_info.get_center(), black_info.get_size(), [700, 250], [100, 100])
    canvas.draw_text('白方胜局数：' + str(wwinCnt), (630, 500), 16, 'black', 'serif')
    canvas.draw_text('黑方胜局数：' + str(bwinCnt), (630, 550), 16, 'black', 'serif')
    for s in list(white_group):
        s.draw(canvas)
    for s in list(black_group):
        s.draw(canvas)
    if started:
        canvas.draw_text(white_label[wlabelID], (675, 110), 20, 'gray', 'serif')
        canvas.draw_text(black_label[blabelID], (675, 260), 20, 'gray', 'serif')
    if win == 1:
        canvas.draw_image(white_win_image, white_win_info.get_center(), white_win_info.get_size(), [300, HEIGHT / 2],
                          white_win_info.get_size())
    if win == 2:
        canvas.draw_image(black_win_image, black_win_info.get_center(), black_win_info.get_size(), [300, HEIGHT / 2],
                          black_win_info.get_size())


def mouseclick(pos): # 鼠标点击判断
    global piecesID, wlabelID, blabelID, oldlabelID
    if piecesID > 0 and pos[0] < 600 and pos[1] < 600:
        # modify the matrix
        matrix[pos[1] // 40][pos[0] // 40] = str(piecesID)
        plant_x = (pos[0] // 40) * 40 + 20
        plant_y = (pos[1] // 40) * 40 + 20
        pieces_spawner(piecesID, [plant_x, plant_y])
    if (pos[0] > 650) and (pos[0] < 750) and (pos[1] > 50) and (pos[1] < 150):
        oldlabelID = wlabelID
        if oldlabelID < 20:
            piecesID = 1
        else:
            piecesID = 2
        wlabelID = random.randrange(0, 40)
    elif (pos[0] > 650) and (pos[0] < 750) and (pos[1] > 200) and (pos[1] < 300):
        oldlabelID = blabelID
        if oldlabelID < 20:
            piecesID = 3
        else:
            piecesID = 4
        blabelID = random.randrange(0, 40)
    else:
        piecesID = 0

# 初始化
frame = simplegui.create_frame("四大名著五子棋", WIDTH, HEIGHT)
button = frame.add_button('开始游戏', start, 100)

# 开始
frame.set_draw_handler(draw)
frame.set_mouseclick_handler(mouseclick)
frame.start()