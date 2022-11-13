import sys # модуль для приёма программой входных параметров (argv) при запуске
# Модули для работы в этом скрипте инструментов библиотеки MathPlotLib
import matplotlib as mpl
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.axes_grid1.anchored_artists import AnchoredText
import matplotlib.pyplot as plt

# Описание функций

# получение некоторых входных параметров задачи.
# Данные возвратятся из функции в виде списка строк
def getParameters(file_parameters):
    res = []
    with open(file_parameters, 'r') as f: # открытие файла и его закрытие по выходу из этого цикла
        while 1:
            line = f.readline()
            if not line: # EOF
                break
            res = line.split()
    return res

# установка шрифта написания символов в виджете координатного пространства
def setPlotFont():
    mpl.rc('font', family='Arial')

# отрисовка метки с входными параметрами задачи
def drawParametersLabel(coord_space, file_parameters):
    dim, alpha, beta, d, h, eps, lines = getParameters(file_parameters)
    # текст метки
    label_text = u'α = '+ str(alpha) + u'\nβ = ' + str(beta) + '\nd = ' + \
                 str(d) + '\n\nh = ' + str(h) + u'\nε = ' + str(eps) + '\n\nlines: ' + str(lines)
    setPlotFont()
    label = AnchoredText(label_text, loc=2, prop=dict(size=12), frameon=True) # добавление текста в метку
    label.patch.set_boxstyle("round,pad=0.,rounding_size=1.2") # отрисовка рамочки вокруг метки
    coord_space.add_artist(label) # добавление метки в координатное пространство

# отрисовка подписей к осям координатного пространства
def drawAxisSignature(coord_space):
    coord_space.set_xlabel('x')
    coord_space.set_ylabel('y')
    coord_space.set_zlabel('z')

# извлечение вещественных координат трехмерной точки из списка
def getPointCoordinatesFromList(lst_coor):
    return float(lst_coor[0]), float(lst_coor[1]), float(lst_coor[2])

# внешняя настройка окна с фазовым портретом
def adjustPlotFigure(fig):
    fig.canvas.set_window_title('phaseportraitdrawer3d.py') # титульный текст окна приложения
    # подстройка в окне приложения границ отображения виджета с фазовым портретом
    fig.subplots_adjust(left=0.01)
    fig.subplots_adjust(right=0.99)
    fig.subplots_adjust(top=0.99)
    fig.subplots_adjust(bottom=0.01)

# отрисовка меток в виджете координатного пространства
def drawPlotLabels(coord_space, file_parameters):
    drawParametersLabel(coord_space, file_parameters)
    drawAxisSignature(coord_space)

# добавление линии-траектории в окно отображения фазового портрета
def addTrajectoryLine(coord_space, line_x, line_y, line_z):
    coord_space.plot_wireframe(line_x, line_y, line_z, color='red')

# добавление стационарной точки в окно отображения фазового портрета
def addStationaryPoint(coord_space, px, py, pz):
    coord_space.scatter(px, py, pz, c='blue')

# добавление траектории в окно отображения фазового портрета
def addTrajectory(coord_space, trajec_x, trajec_y, trajec_z):
    addTrajectoryLine(coord_space, trajec_x, trajec_y, trajec_z)
    # индекс -1 указывает на последний элемент списка
    addStationaryPoint(coord_space, trajec_x[-1], trajec_y[-1], trajec_z[-1])

# установка данных о фазовом портрете в виджет координатного пространства
def setPhasePortrait(coord_space, file_trajectories):
    x_coor = []
    y_coor = []
    z_coor = []
    with open(file_trajectories, 'r') as f:
        while 1:
            file_line = f.readline()
            if not file_line:
                break
            point = file_line.split()
            if len(point):
                x, y, z = getPointCoordinatesFromList(point)
                x_coor.append(x)
                y_coor.append(y)
                z_coor.append(z)
            else:
                if len(x_coor) and len(y_coor) and len(z_coor):
                    addTrajectory(coord_space, x_coor, y_coor, z_coor)
                    x_coor.clear()
                    y_coor.clear()
                    z_coor.clear()

# главная (основная функция)
def main(file_trajectories, file_parameters):
    fig = plt.figure(figsize=(9.2, 8.0))
    adjustPlotFigure(fig)
    coord_space = fig.gca(projection='3d')
    setPhasePortrait(coord_space, file_trajectories)
    drawPlotLabels(coord_space, file_parameters)
    plt.show()

# Выполнение скрипта

if len(sys.argv) != 3: # в том случае, если не все параметры для работы приложения поступили на вход скрипту
    print("Error! Script must be run in the format:\n python parser.py file_trajectories(.txt) file_parameters(.txt)\n")
else:
    file_trajectories = str(sys.argv[1])
    file_parameters = str(sys.argv[2])
    main(file_trajectories, file_parameters)