#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPixmap>

QT_BEGIN_NAMESPACE
class QDockWidget;
class QCheckBox;
class QLabel;
class QScrollArea;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openImage();
    void toggleInvert(bool checked);
    void toggleGrayscale(bool checked);
    void resetImage();
    void toggleDock(bool visible);

private:
    void createDockWidget();
    void loadDefaultImage();
    void updateImage();

    QLabel *m_imageLabel;
    QScrollArea *m_scrollArea;
    QImage m_originalImage;
    QImage m_currentImage;
    bool m_isInverted = false;
    bool m_isGrayscale = false;

    QDockWidget *m_dockWidget;
    QCheckBox *m_invertCheckBox;
    QCheckBox *m_grayscaleCheckBox;
};

#endif
