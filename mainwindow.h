#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "graphicsscene.h"

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
private slots:
    void startOrAdvanceGame();
private:
    bool m_manual_mode;
    GraphicsScene *m_scene;
    QTimer m_timer;
    bool m_timer_running;
};

#endif // MAINWINDOW_H
