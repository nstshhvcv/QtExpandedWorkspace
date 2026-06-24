#include "mainwindow.h"
#include <QDockWidget>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>
#include <QApplication>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Synthetic 3D workspace");
    resize(900, 700);

    // Central widget
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setBackgroundRole(QPalette::Dark);
    m_scrollArea->setWidgetResizable(true);
    
    m_imageLabel = new QLabel();
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setText("Load 3D model image\n(File → Open)");
    m_scrollArea->setWidget(m_imageLabel);
    setCentralWidget(m_scrollArea);

    // Menu
    QMenuBar *menuBar = this->menuBar();
    
    QMenu *fileMenu = menuBar->addMenu("&File");
    QAction *openAction = new QAction("&Open...", this);
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::openImage);
    fileMenu->addAction(openAction);
    
    fileMenu->addSeparator();
    
    QAction *exitAction = new QAction("&Exit", this);
    connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);
    fileMenu->addAction(exitAction);

    QMenu *viewMenu = menuBar->addMenu("&View");
    QAction *toggleDockAction = new QAction("Show/Hide Dock", this);
    toggleDockAction->setShortcut(QKeySequence("Ctrl+D"));
    toggleDockAction->setCheckable(true);
    toggleDockAction->setChecked(true);
    connect(toggleDockAction, &QAction::triggered, this, &MainWindow::toggleDock);
    viewMenu->addAction(toggleDockAction);

    QMenu *imageMenu = menuBar->addMenu("&Image");
    QAction *resetAction = new QAction("&Reset", this);
    resetAction->setShortcut(QKeySequence("Ctrl+R"));
    connect(resetAction, &QAction::triggered, this, &MainWindow::resetImage);
    imageMenu->addAction(resetAction);

    // Dock Widget
    createDockWidget();

    // Toolbar
    QToolBar *toolBar = new QToolBar("Toolbar", this);
    toolBar->addAction(openAction);
    toolBar->addSeparator();
    toolBar->addAction(resetAction);
    toolBar->addSeparator();
    toolBar->addAction(toggleDockAction);
    addToolBar(Qt::TopToolBarArea, toolBar);

    statusBar()->showMessage("Ready");

    // Styles with transparency
    setStyleSheet(R"(
        QMainWindow { background-color: #2b2b2b; }
        QDockWidget { 
            background-color: rgba(50, 50, 50, 220);
            border: 1px solid #555;
        }
        QDockWidget::title {
            background-color: rgba(70, 70, 70, 200);
            padding: 6px;
            font-weight: bold;
            color: white;
        }
        QCheckBox {
            color: white;
            padding: 5px;
            font-size: 13px;
        }
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
        }
        QToolBar {
            background-color: rgba(60, 60, 60, 200);
            spacing: 5px;
            padding: 5px;
        }
        QMenuBar {
            background-color: rgba(45, 45, 45, 220);
            color: white;
        }
        QMenuBar::item:selected {
            background-color: #4a6a8a;
        }
        QMenu {
            background-color: rgba(45, 45, 45, 230);
            color: white;
        }
        QMenu::item:selected {
            background-color: #4a6a8a;
        }
        QStatusBar {
            background-color: rgba(45, 45, 45, 200);
            color: #aaa;
        }
        QLabel {
            color: white;
        }
    )");

    loadDefaultImage();
}

MainWindow::~MainWindow() {}

void MainWindow::createDockWidget()
{
    m_dockWidget = new QDockWidget("Display settings", this);
    m_dockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);

    QWidget *content = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(content);
    layout->setSpacing(15);
    layout->setContentsMargins(15, 15, 15, 15);

    QLabel *title = new QLabel("Display settings");
    title->setStyleSheet("font-weight: bold; color: #4CAF50; font-size: 15px;");
    layout->addWidget(title);

    layout->addSpacing(10);

    m_invertCheckBox = new QCheckBox("Invert colors");
    m_invertCheckBox->setToolTip("Invert all image colors");
    connect(m_invertCheckBox, &QCheckBox::toggled, this, &MainWindow::toggleInvert);
    layout->addWidget(m_invertCheckBox);

    m_grayscaleCheckBox = new QCheckBox("Grayscale");
    m_grayscaleCheckBox->setToolTip("Convert image to grayscale");
    connect(m_grayscaleCheckBox, &QCheckBox::toggled, this, &MainWindow::toggleGrayscale);
    layout->addWidget(m_grayscaleCheckBox);

    layout->addStretch();

    QLabel *info = new QLabel("Dock widget can be moved\nand hidden via View menu");
    info->setStyleSheet("color: #888; font-size: 11px;");
    info->setAlignment(Qt::AlignCenter);
    layout->addWidget(info);

    m_dockWidget->setWidget(content);
    addDockWidget(Qt::RightDockWidgetArea, m_dockWidget);
}

void MainWindow::loadDefaultImage()
{
    QImage defaultImage(800, 600, QImage::Format_RGB32);
    defaultImage.fill(Qt::darkGray);
    
    QPainter painter(&defaultImage);
    painter.setRenderHint(QPainter::Antialiasing);
    
    QLinearGradient gradient(100, 100, 700, 500);
    gradient.setColorAt(0.0, QColor(66, 133, 244));
    gradient.setColorAt(0.3, QColor(52, 168, 83));
    gradient.setColorAt(0.6, QColor(251, 188, 5));
    gradient.setColorAt(0.9, QColor(234, 67, 53));
    
    painter.setBrush(gradient);
    painter.setPen(QPen(Qt::white, 3));
    painter.drawRoundedRect(150, 100, 500, 400, 30, 30);
    
    painter.setBrush(QColor(255, 255, 255, 50));
    painter.drawRoundedRect(200, 150, 100, 80, 10, 10);
    painter.drawRoundedRect(500, 350, 80, 100, 10, 10);
    painter.drawEllipse(400, 250, 60, 60);
    
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 18));
    painter.drawText(defaultImage.rect(), Qt::AlignCenter, "3D MODEL\n(Synthetic workspace)");
    
    painter.end();
    
    m_originalImage = defaultImage;
    m_currentImage = defaultImage;
    updateImage();
}

void MainWindow::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, 
        "Open Image", "", 
        "Images (*.png *.jpg *.jpeg *.bmp *.tiff)");
    
    if (fileName.isEmpty()) return;
    
    QImage image;
    if (image.load(fileName)) {
        m_originalImage = image;
        m_currentImage = image;
        m_isInverted = false;
        m_isGrayscale = false;
        m_invertCheckBox->setChecked(false);
        m_grayscaleCheckBox->setChecked(false);
        updateImage();
        statusBar()->showMessage("Loaded: " + fileName, 3000);
    } else {
        QMessageBox::warning(this, "Error", "Failed to load image");
    }
}

void MainWindow::toggleInvert(bool checked)
{
    m_isInverted = checked;
    updateImage();
    statusBar()->showMessage(checked ? "Invert ON" : "Invert OFF", 2000);
}

void MainWindow::toggleGrayscale(bool checked)
{
    m_isGrayscale = checked;
    updateImage();
    statusBar()->showMessage(checked ? "Grayscale ON" : "Grayscale OFF", 2000);
}

void MainWindow::resetImage()
{
    m_currentImage = m_originalImage;
    m_isInverted = false;
    m_isGrayscale = false;
    m_invertCheckBox->setChecked(false);
    m_grayscaleCheckBox->setChecked(false);
    updateImage();
    statusBar()->showMessage("Reset", 2000);
}

void MainWindow::toggleDock(bool visible)
{
    m_dockWidget->setVisible(visible);
}

void MainWindow::updateImage()
{
    if (m_originalImage.isNull()) return;
    
    m_currentImage = m_originalImage;
    
    if (m_isGrayscale) {
        m_currentImage = m_currentImage.convertToFormat(QImage::Format_Grayscale8);
    }
    
    if (m_isInverted) {
        m_currentImage.invertPixels(QImage::InvertRgb);
    }
    
    QPixmap pixmap = QPixmap::fromImage(m_currentImage);
    QSize labelSize = m_imageLabel->size();
    
    if (!pixmap.isNull() && labelSize.width() > 0 && labelSize.height() > 0) {
        pixmap = pixmap.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    
    m_imageLabel->setPixmap(pixmap);
}
