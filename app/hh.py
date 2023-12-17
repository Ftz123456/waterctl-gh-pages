# 导入模块
import turtle
import math
import random

# 设置背景
turtle.bgcolor("black")
turtle.title("跳动的爱心")
turtle.setup(800, 600)
turtle.tracer(0)

# 创建画笔
pen = turtle.Turtle()
pen.hideturtle()
pen.speed(0)
pen.pensize(2)

# 创建音乐播放器
player = turtle.Turtle()
player.hideturtle()
player.speed(0)

# 加载音乐文件
turtle.register_shape("music.wav")
player.shape("music.wav")

# 定义爱心的参数
heart_radius = 100
heart_angle = 2 * math.pi / 100
heart_scale = 0.8
heart_color = "pink"

# 定义背景的参数
bg_num = 50
bg_list = []
bg_colors = ["red", "orange", "yellow", "green", "blue", "purple", "white"]

# 创建背景的星星
for _ in range(bg_num):
  star = turtle.Turtle()
  star.hideturtle()
  star.speed(0)
  star.color(random.choice(bg_colors))
  star.penup()
  star.goto(random.randint(-400, 400), random.randint(-300, 300))
  star.pendown()
  bg_list.append(star)

# 定义动画的函数
def draw_frame():
  # 清除画布
  pen.clear()

  # 绘制背景的星星
  for star in bg_list:
    # 随机改变星星的颜色和位置
    star.color(random.choice(bg_colors))
    star.setheading(random.randint(0, 360))
    star.forward(random.randint(10, 20))
    # 如果星星超出边界，就重新放置
    if star.xcor() > 400 or star.xcor() < -400 or star.ycor() > 300 or star.ycor() < -300:
      star.goto(random.randint(-400, 400), random.randint(-300, 300))

  # 绘制爱心的轮廓
  pen.color(heart_color)
  pen.penup()
  # 移动到爱心的起点
  x = heart_radius * math.cos(heart_angle)
  y = heart_radius * math.sin(heart_angle) - heart_radius
  pen.goto(x, y)
  pen.pendown()
  # 画出爱心的曲线
  for i in range(100):
    # 计算爱心的坐标
    t = i * heart_angle
    x = heart_radius * (16 * math.sin(t) ** 3) * heart_scale
    y = heart_radius * (13 * math.cos(t) - 5 * math.cos(2 * t) - 2 * math.cos(3 * t) - math.cos(4 * t)) * heart_scale - heart_radius
    # 画出爱心的点
    pen.goto(x, y)
  # 填充爱心的颜色
  pen.begin_fill()
  pen.circle(3)
  pen.end_fill()

  # 更新画布
  turtle.update()

# 定义主循环
def main():
  # 播放音乐
  player.stamp()
  # 进入动画循环
  while True:
    # 绘制一帧动画
    draw_frame()

# 调用主函数
main()
