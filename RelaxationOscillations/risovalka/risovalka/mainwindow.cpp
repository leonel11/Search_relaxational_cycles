#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent, QApplication *app)
    : QMainWindow(parent)
{
    ap = app;
    maxX = maxY = 1 - DBL_MAX;
    minX = minY = DBL_MAX;
    screenWidth = MAX_APPSCREEN_SIZE;
    params = new TaskParameters(path_to_file_parameters);
    file_trajectories = fopen(path_to_file_trajectories, "rt");
    if (!file_trajectories)
        return;
    assemblyMainWindow();
}

void MainWindow::createDoubleSpinBoxes() {
    aEdit = new QDoubleSpinBox();
    bEdit = new QDoubleSpinBox();
    cEdit = new QDoubleSpinBox();
    dEdit = new QDoubleSpinBox();
    aEdit->setRange(-100, 100);
    aEdit->setValue(-3);
    bEdit->setRange(-100, 100);
    bEdit->setValue(3);
    cEdit->setRange(-100, 100);
    cEdit->setValue(-3);
    dEdit->setRange(-100, 100);
    dEdit->setValue(3);
    // установка минимальной величины изменения значения в QDoubleSpinBox
    aEdit->setSingleStep(0.1);
    bEdit->setSingleStep(0.1);
    cEdit->setSingleStep(0.1);
    dEdit->setSingleStep(0.1);
}

void MainWindow::createPlotForDrawing() {
    plot = new QwtPlot(tr(" "), 0);
}

void MainWindow::createPlotGrid() {
    // создание тетрадной сетки
    plotGrid = new QwtPlotGrid;
    plotGrid->enableXMin(true);
    plotGrid->enableYMin(true);
    // установка тетрадной сетки на координатную плоскость
    plotGrid->setMajPen(QPen(Qt::gray,0,Qt::DashDotDotLine));
    plotGrid->setMinPen(QPen(Qt::gray,0,Qt::DotLine));
    plotGrid->attach(plot);
}

void MainWindow::createButtons() {
    screenshotButton = new QPushButton("Take screenshot");
}

void MainWindow::createLabels() {
    aLabel = new QLabel("A:");
    dLabel = new QLabel("D:");
    dLabel->setAlignment(Qt::AlignRight);
    bLabel = new QLabel("B:");
    bLabel->setAlignment(Qt::AlignRight);
    cLabel = new QLabel("C:");
    cLabel->setAlignment(Qt::AlignRight);
    QString str_alpha = QString::fromStdString("                       ") +
                        QChar(0x03B1) + QString::fromStdString("   =   ") +
                        QString(TypesChange::doubleToString(params->Alpha()).c_str());
    alphaLabel = new QLabel(str_alpha);
    QString str_beta = QString::fromStdString("                       ") +
                       QChar(0x03B2) + QString::fromStdString("   =   ") +
                       QString(TypesChange::doubleToString(params->Beta()).c_str());
    betaLabel = new QLabel(str_beta);
    QString str_d = "                       d   =   " + QString(TypesChange::doubleToString(params->D()).c_str());
    dprogLabel = new QLabel(str_d);
    QString str_h = "                      h    =   " + QString(TypesChange::doubleToString(params->H()).c_str());
    hLabel = new QLabel(str_h);
    QString str_eps = QString::fromStdString("                      ") +
                      QChar(0x03B5) + QString::fromStdString("    =   ") +
                      QString(TypesChange::doubleToString(params->Eps()).c_str());
    epsLabel = new QLabel(str_eps);
    QString str_trajec = QString::fromStdString("                     Trajectories:   ") +
                         QString(TypesChange::intToString(params->Trajectories()).c_str());
    trajecLabel = new QLabel(str_trajec);
}

void MainWindow::createLayouts() {
    spaceLay = new QFormLayout();
    editLay = new QGridLayout();
    labelLay = new QVBoxLayout();
    vertLay = new QVBoxLayout;
    mainLay = new QHBoxLayout();
}

void MainWindow::createApplicationWidget() {
    appWidget = new QWidget();
}

/**
  * @brief Установка вида основного окна приложения
  */
void MainWindow::setAxisTitles() {
    // установка шрифта и его размера для подписей к координатным осям
    xtitle.setFont(QFont("Helvetica", 8));
    ytitle.setFont(QFont("Helvetica", 8));
    // запись текстового содержимого в подписи к осям
    switch (params->Dim()) {
        case 1:
            bottom_str = QString::fromStdString("z");
            left_str = QChar(0x03A6) + QString::fromStdString("(z)");
            break;
        case 2:
            bottom_str = QString::fromStdString("x");
            left_str = QString::fromStdString("y");
            break;
        default:
            break;
    }
    // установка подписей к осям
    xtitle.setText(bottom_str, QwtText::TeXText);
    ytitle.setText(left_str, QwtText::TeXText);
    // определение расположения подписей к координатным осям
    plot->setAxisTitle(QwtPlot::xBottom, xtitle);
    plot->setAxisTitle(QwtPlot::yLeft, ytitle);
}

/**
  * @brief Сборка элементов приложения
  */
void MainWindow::assemblyApplicationElements() {
    spaceLay->setHorizontalSpacing(10); // установка промежутка по горизонтали между элементами приложения
    editLay->addWidget(dLabel, 0, 1);
    editLay->addWidget(dEdit, 0, 2);
    editLay->addWidget(aLabel, 1, 0);
    editLay->addWidget(aEdit, 1, 1);
    editLay->addWidget(bLabel, 1, 2);
    editLay->addWidget(bEdit, 1, 3);
    editLay->addWidget(cLabel, 2, 1);
    editLay->addWidget(cEdit, 2, 2);
    labelLay->insertSpacing(0, 50);
    labelLay->addWidget(dprogLabel);
    labelLay->addWidget(alphaLabel);
    labelLay->addWidget(betaLabel);
    labelLay->insertSpacing(4, 50);
    labelLay->addWidget(hLabel);
    labelLay->addWidget(epsLabel);
    vertLay->addStretch();
    vertLay->addLayout(editLay);
    vertLay->addStretch();
    vertLay->addLayout(labelLay);
    vertLay->addStretch();
    vertLay->addLayout(spaceLay);
    vertLay->addStretch();
    vertLay->addWidget(screenshotButton);
    vertLay->addStretch();
    vertLay->addWidget(trajecLabel);
    vertLay->addStretch();
    mainLay->addWidget(plot);
    mainLay->addLayout(vertLay);
    appWidget->setLayout(mainLay);
}

/**
  * @brief Соединение элементов окна с событиями
  */
void MainWindow::setConnections() {
    connect(aEdit, SIGNAL(editingFinished()), this, SLOT(refreshWindow()));
    connect(bEdit, SIGNAL(editingFinished()), this, SLOT(refreshWindow()));
    connect(cEdit, SIGNAL(editingFinished()), this, SLOT(refreshWindow()));
    connect(dEdit, SIGNAL(editingFinished()), this, SLOT(refreshWindow()));
    connect(dEdit, SIGNAL(editingFinished()), this, SLOT(refreshWindow()));
    connect(screenshotButton, SIGNAL(clicked()), this, SLOT(takeScreenshot()));
}

/**
  * @brief Установка вида основного окна приложения
  */
void MainWindow::setViewMainWindow() {
    this->setWindowTitle("PhasePortraitDrawer");
    this->resize(950,750);
}

void MainWindow::UpdateDoubleSpinBoxes() {
    aEdit->setValue(minX);
    bEdit->setValue(maxX);
    cEdit->setValue(minY);
    dEdit->setValue(maxY);
}

/**
  * @brief Сборка основного окна приложения
  */
void MainWindow::assemblyMainWindow(){
    createPlotForDrawing();
    createDoubleSpinBoxes();
    createButtons();
    createPlotGrid();
    createLabels();
    createLayouts();
    createApplicationWidget();
    setAxisTitles();
    assemblyApplicationElements();
    this->setCentralWidget(appWidget); // установка виджета в окно приложения
    setViewMainWindow();
    setConnections();
    refreshPhasePortrait();
    UpdateDoubleSpinBoxes();
    refreshWindow();
}

/**
  * @brief установка краев области, отображающей фазовый портрет
  * @param x - краевое значение по горизонтальной оси координатной плоскости фазового портрета
  * @param y - краевое значение по вертикальной оси координатной плоскости фазового портрета
  */
void MainWindow::setPlotEdges(double x,double y) {
    if (x < minX)
        minX = x;
    if (x > maxX)
        maxX = x;
    if (y < minY)
        minY = y;
    if (y > maxY)
        maxY = y;
}

/**
  * @brief Отрисовка/перерисовка фазового портрета
  */
void MainWindow::refreshPhasePortrait(){
    for( ; !feof(file_trajectories) ; ){
        double *x = new double [screenWidth];
        double *y = new double [screenWidth];
        QwtPlotCurve *trajectories = new QwtPlotCurve(); // траектория фазового портрета
        int i = 0;
        // считывание траектории
        for ( ; i < screenWidth; i++) {
            if (feof(file_trajectories))
                break;
            char symb1, symb2;
            fscanf(file_trajectories, "%c%c", &symb1, &symb2);
            if (symb2 != ' ') // признак окончания траектории
                break;
            else { // считывание координат каждой точки траектории
                if (fscanf(file_trajectories, "%lf %lf", &(x[i]), &(y[i])) !=2)
                    break;
                setPlotEdges(x[i],y[i]);
            }
        }
        if (i==0)
            continue;
        // установка внешнего вида траектории
        QPen trajec_pen = QPen(Qt::red);
        trajec_pen.setWidth(1);
        // добавление траектории на координатную плоскость
        trajectories->setData(x, y, i);
        trajectories->setPen(trajec_pen);
        trajectories->setRenderHint(QwtPlotItem::RenderAntialiased); // сглаживание траекторий при отрисовке
        trajectories->attach(plot);
        // координаты стационарной (или последней посчитанной) точки, она считается концом траектории
        double *end_x = new double[1];
        double *end_y = new double[1];
        QwtPlotCurve *stationary_points = new QwtPlotCurve();
        end_x[0] = x[i-1];
        end_y[0] = y[i-1];
        // установка внешнего вида концевой точки траектории
        QPen endpoint_pen = QPen(Qt::blue);
        endpoint_pen.setWidth(4);
        // добавление траектории на координатную плоскость
        stationary_points->setData(end_x, end_y, 1);
        stationary_points->setStyle(QwtPlotCurve::Dots);
        stationary_points->setPen(endpoint_pen);
        stationary_points->setRenderHint(QwtPlotItem::RenderAntialiased); // сглаживание стационарных точек при отрисовке
        stationary_points->attach(plot);
    }
}

/**
  * @brief Обновление содержимого окна приложения
  */
void MainWindow::refreshWindow(){
    a = aEdit->value();
    b = bEdit->value();
    c = cEdit->value();
    d = dEdit->value();
    screenWidth = MAX_APPSCREEN_SIZE;
    refreshPhasePortrait();
    // регулирование масштаба области отображения фазового рисунка
    plot->setAxisScale(QwtPlot::xBottom, a,b);
    plot->setAxisScale(QwtPlot::yLeft, c,d);
    plot->replot();
}

/**
  * @brief Формирование скриншота окна программы
  */
void MainWindow::takeScreenshot() {
    QString save_dir_path = QString::fromStdString(path_to_folder_with_screenshots);
    QDir save_dir(save_dir_path);
    if (!save_dir.exists()) { // в том случае, если папки для сохранения скриншотов программы не существует
        save_dir.mkdir("."); // эта самая папка создается
    }
    QPixmap pixmap = QPixmap::grabWindow(this->winId());
    // формирование название файла-скриншота
    QString img_path = QString::fromStdString(params->getScreenshotName(path_to_folder_with_screenshots));
    QFile phase_portrait(img_path);
    if (phase_portrait.exists()) { // в том случае, если файл с таким же названием скориншота уже существует
        // вылетает специальное диалоговое окно, которое позволяет пользователю самому определить,
        // пересохранять ли новый скриншот вместо старого
        int ret = QMessageBox::warning(this, tr("Resaving of picture"),
                                       tr("Do you want to resave the phase portrait?"),
                                       QMessageBox::Yes, QMessageBox::No);
        if (ret == QMessageBox::No) // не пересохранять
            return;
        else
            phase_portrait.remove();
    }
    pixmap.save(img_path); // сохранение файла-скриншота программы
}

MainWindow::~MainWindow() {
    if (file_trajectories)
        fclose(file_trajectories);
}
