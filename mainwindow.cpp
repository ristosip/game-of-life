#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "graphicsscene.h"
#include <QGraphicsView>
#include <QPushButton>
#include <QSpinBox>
#include <QSlider>
#include <QRadioButton>
#include <QGroupBox>
#include <QLabel>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
    m_manual_mode = false;
    m_timer_running = false;

    QVBoxLayout *vLayout = new QVBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();

    QSpinBox *gameSizeX = new QSpinBox(this);
    gameSizeX->setValue(50);
    gameSizeX->setMaximum(200);
    QSpinBox *gameSizeY = new QSpinBox(this);
    gameSizeY->setValue(50);
    gameSizeY->setMaximum(200);
    QPushButton *initButton = new QPushButton("Initialize", this);

    QVBoxLayout *initSettingLayout = new QVBoxLayout();
    initSettingLayout->addWidget(gameSizeX);
    initSettingLayout->addWidget(gameSizeY);
    initSettingLayout->addWidget(initButton);

    QGroupBox *initSettings = new QGroupBox("Game Size", this);
    initSettings->setLayout(initSettingLayout);
    hLayout->addWidget(initSettings);

    QRadioButton *automaticRadioButton = new QRadioButton("Automatic", this);
    automaticRadioButton->toggle();
    QRadioButton *manualRadioButton = new QRadioButton("Manual", this);

    QVBoxLayout *modeSelectLayout = new QVBoxLayout();
    modeSelectLayout->addWidget(automaticRadioButton);
    modeSelectLayout->addWidget(manualRadioButton);

    QGroupBox *modeButtons = new QGroupBox("Game mode", this);
    modeButtons->setLayout(modeSelectLayout);
    hLayout->addWidget(modeButtons);

    QPushButton *startButton = new QPushButton(this);
    startButton->setText("Start/Stop");
    hLayout->addWidget(startButton);

    QVBoxLayout *speedSelectLayout = new  QVBoxLayout();

    QSlider *intervalSlider = new QSlider(Qt::Horizontal, this);
    intervalSlider->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    intervalSlider->setMaximum(1500);
    intervalSlider->setMinimum(1);
    intervalSlider->setValue(750);
    speedSelectLayout->addWidget(intervalSlider);
    m_timer.setInterval(intervalSlider->value());

    QGroupBox *speedOptions = new QGroupBox("Simulation interval", this);
    speedOptions->setLayout(speedSelectLayout);
    hLayout->addWidget(speedOptions);

    QGraphicsView *view = new QGraphicsView(this);
    vLayout->addWidget(view);
    vLayout->addLayout(hLayout);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(vLayout);

    setCentralWidget(centralWidget);

    m_scene = new GraphicsScene();
    view->setScene(m_scene);

    connect(initButton, &QPushButton::clicked, this, [=]{
        m_timer.stop();
        m_timer_running = false;
        m_scene->populateScene(gameSizeX->value(), gameSizeY->value());
    });

    connect(intervalSlider, &QSlider::valueChanged, this, [=]{
        m_timer.setInterval(intervalSlider->value());
    });

    connect(manualRadioButton, &QRadioButton::toggled, this, [=]{
        m_manual_mode = true;
        m_timer.stop();
        m_timer_running = false;
        startButton->setText("Advance");
    });

    connect(automaticRadioButton, &QRadioButton::toggled, this, [=]{
        m_manual_mode = false;
        startButton->setText("Start/Stop");
    });

    connect(startButton, &QPushButton::clicked, this, &MainWindow::startOrAdvanceGame);
    connect(&m_timer, &QTimer::timeout, m_scene, &GraphicsScene::advanceGame);
}

void MainWindow::startOrAdvanceGame(){
    if(!m_manual_mode){
        if(!m_timer_running){
            m_timer.start();
            m_timer_running = true;
        }
        else{
            m_timer.stop();
            m_timer_running = false;
        }
    }
    else{
        m_scene->advanceGame();
    }
}
