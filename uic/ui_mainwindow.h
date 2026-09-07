/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "starrybackground.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    StarryBackground *centralWidget;
    QVBoxLayout *bgLayout;
    QScrollArea *scrollArea;
    QWidget *contentContainer;
    QVBoxLayout *mainLayout;
    QHBoxLayout *headerLayout;
    QVBoxLayout *titleTextLayout;
    QLabel *headerTitle;
    QLabel *headerSubtitle;
    QSpacerItem *headerSpacer;
    QGroupBox *inputGroup;
    QVBoxLayout *inputGroupLayout;
    QHBoxLayout *inputRow;
    QLineEdit *inputVideoEdit;
    QPushButton *browseInputBtn;
    QLabel *fileInfoLabel;
    QGroupBox *settingsGroup;
    QVBoxLayout *settingsGroupLayout;
    QHBoxLayout *modeRow;
    QRadioButton *radioTranscode;
    QRadioButton *radioDirectCopy;
    QSpacerItem *modeSpacer;
    QGridLayout *paramGrid;
    QLabel *formatLbl;
    QComboBox *formatCombo;
    QLabel *bitrateLbl;
    QComboBox *bitrateCombo;
    QLabel *sampleRateLbl;
    QComboBox *sampleRateCombo;
    QLabel *channelLbl;
    QComboBox *channelCombo;
    QCheckBox *overwriteCheck;
    QGroupBox *pathGroup;
    QGridLayout *pathGrid;
    QLabel *outputLbl;
    QLineEdit *outputPathEdit;
    QPushButton *browseOutputBtn;
    QLabel *ffmpegLbl;
    QHBoxLayout *ffmpegRow;
    QLineEdit *ffmpegPathEdit;
    QLabel *ffmpegStatusBadge;
    QPushButton *browseFFmpegBtn;
    QVBoxLayout *actionSection;
    QHBoxLayout *progressInfoRow;
    QLabel *progressTitle;
    QLabel *progressDetailLabel;
    QSpacerItem *progressSpacer;
    QProgressBar *progressBar;
    QHBoxLayout *actionBtnRow;
    QPushButton *startBtn;
    QPushButton *openFolderBtn;
    QPushButton *playAudioBtn;
    QGroupBox *logGroup;
    QVBoxLayout *logLayout;
    QTextEdit *consoleLog;
    QHBoxLayout *logBottomRow;
    QSpacerItem *logSpacer;
    QPushButton *clearLogBtn;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(860, 680);
        MainWindow->setMinimumSize(QSize(620, 480));
        centralWidget = new StarryBackground(MainWindow);
        centralWidget->setObjectName("centralWidget");
        bgLayout = new QVBoxLayout(centralWidget);
        bgLayout->setSpacing(0);
        bgLayout->setObjectName("bgLayout");
        bgLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea = new QScrollArea(centralWidget);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setStyleSheet(QString::fromUtf8("QScrollArea { background: transparent; border: none; }"));
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->setWidgetResizable(true);
        contentContainer = new QWidget();
        contentContainer->setObjectName("contentContainer");
        contentContainer->setGeometry(QRect(0, 0, 860, 680));
        contentContainer->setStyleSheet(QString::fromUtf8("background: transparent;"));
        mainLayout = new QVBoxLayout(contentContainer);
        mainLayout->setSpacing(12);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(20, 16, 20, 16);
        headerLayout = new QHBoxLayout();
        headerLayout->setObjectName("headerLayout");
        titleTextLayout = new QVBoxLayout();
        titleTextLayout->setSpacing(2);
        titleTextLayout->setObjectName("titleTextLayout");
        headerTitle = new QLabel(contentContainer);
        headerTitle->setObjectName("headerTitle");

        titleTextLayout->addWidget(headerTitle);

        headerSubtitle = new QLabel(contentContainer);
        headerSubtitle->setObjectName("headerSubtitle");

        titleTextLayout->addWidget(headerSubtitle);


        headerLayout->addLayout(titleTextLayout);

        headerSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        headerLayout->addItem(headerSpacer);


        mainLayout->addLayout(headerLayout);

        inputGroup = new QGroupBox(contentContainer);
        inputGroup->setObjectName("inputGroup");
        inputGroupLayout = new QVBoxLayout(inputGroup);
        inputGroupLayout->setSpacing(6);
        inputGroupLayout->setObjectName("inputGroupLayout");
        inputGroupLayout->setContentsMargins(12, 12, 12, 10);
        inputRow = new QHBoxLayout();
        inputRow->setSpacing(8);
        inputRow->setObjectName("inputRow");
        inputVideoEdit = new QLineEdit(inputGroup);
        inputVideoEdit->setObjectName("inputVideoEdit");

        inputRow->addWidget(inputVideoEdit);

        browseInputBtn = new QPushButton(inputGroup);
        browseInputBtn->setObjectName("browseInputBtn");
        browseInputBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        inputRow->addWidget(browseInputBtn);


        inputGroupLayout->addLayout(inputRow);

        fileInfoLabel = new QLabel(inputGroup);
        fileInfoLabel->setObjectName("fileInfoLabel");
        fileInfoLabel->setStyleSheet(QString::fromUtf8("color: #64748b; font-size: 11px;"));

        inputGroupLayout->addWidget(fileInfoLabel);


        mainLayout->addWidget(inputGroup);

        settingsGroup = new QGroupBox(contentContainer);
        settingsGroup->setObjectName("settingsGroup");
        settingsGroupLayout = new QVBoxLayout(settingsGroup);
        settingsGroupLayout->setSpacing(8);
        settingsGroupLayout->setObjectName("settingsGroupLayout");
        settingsGroupLayout->setContentsMargins(12, 12, 12, 10);
        modeRow = new QHBoxLayout();
        modeRow->setObjectName("modeRow");
        radioTranscode = new QRadioButton(settingsGroup);
        radioTranscode->setObjectName("radioTranscode");
        radioTranscode->setChecked(true);

        modeRow->addWidget(radioTranscode);

        radioDirectCopy = new QRadioButton(settingsGroup);
        radioDirectCopy->setObjectName("radioDirectCopy");

        modeRow->addWidget(radioDirectCopy);

        modeSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        modeRow->addItem(modeSpacer);


        settingsGroupLayout->addLayout(modeRow);

        paramGrid = new QGridLayout();
        paramGrid->setObjectName("paramGrid");
        paramGrid->setHorizontalSpacing(14);
        paramGrid->setVerticalSpacing(6);
        formatLbl = new QLabel(settingsGroup);
        formatLbl->setObjectName("formatLbl");

        paramGrid->addWidget(formatLbl, 0, 0, 1, 1);

        formatCombo = new QComboBox(settingsGroup);
        formatCombo->setObjectName("formatCombo");

        paramGrid->addWidget(formatCombo, 0, 1, 1, 1);

        bitrateLbl = new QLabel(settingsGroup);
        bitrateLbl->setObjectName("bitrateLbl");

        paramGrid->addWidget(bitrateLbl, 0, 2, 1, 1);

        bitrateCombo = new QComboBox(settingsGroup);
        bitrateCombo->setObjectName("bitrateCombo");

        paramGrid->addWidget(bitrateCombo, 0, 3, 1, 1);

        sampleRateLbl = new QLabel(settingsGroup);
        sampleRateLbl->setObjectName("sampleRateLbl");

        paramGrid->addWidget(sampleRateLbl, 1, 0, 1, 1);

        sampleRateCombo = new QComboBox(settingsGroup);
        sampleRateCombo->setObjectName("sampleRateCombo");

        paramGrid->addWidget(sampleRateCombo, 1, 1, 1, 1);

        channelLbl = new QLabel(settingsGroup);
        channelLbl->setObjectName("channelLbl");

        paramGrid->addWidget(channelLbl, 1, 2, 1, 1);

        channelCombo = new QComboBox(settingsGroup);
        channelCombo->setObjectName("channelCombo");

        paramGrid->addWidget(channelCombo, 1, 3, 1, 1);


        settingsGroupLayout->addLayout(paramGrid);

        overwriteCheck = new QCheckBox(settingsGroup);
        overwriteCheck->setObjectName("overwriteCheck");
        overwriteCheck->setChecked(true);

        settingsGroupLayout->addWidget(overwriteCheck);


        mainLayout->addWidget(settingsGroup);

        pathGroup = new QGroupBox(contentContainer);
        pathGroup->setObjectName("pathGroup");
        pathGrid = new QGridLayout(pathGroup);
        pathGrid->setObjectName("pathGrid");
        pathGrid->setHorizontalSpacing(10);
        pathGrid->setVerticalSpacing(6);
        pathGrid->setContentsMargins(12, 12, 12, 10);
        outputLbl = new QLabel(pathGroup);
        outputLbl->setObjectName("outputLbl");

        pathGrid->addWidget(outputLbl, 0, 0, 1, 1);

        outputPathEdit = new QLineEdit(pathGroup);
        outputPathEdit->setObjectName("outputPathEdit");

        pathGrid->addWidget(outputPathEdit, 0, 1, 1, 1);

        browseOutputBtn = new QPushButton(pathGroup);
        browseOutputBtn->setObjectName("browseOutputBtn");
        browseOutputBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        pathGrid->addWidget(browseOutputBtn, 0, 2, 1, 1);

        ffmpegLbl = new QLabel(pathGroup);
        ffmpegLbl->setObjectName("ffmpegLbl");

        pathGrid->addWidget(ffmpegLbl, 1, 0, 1, 1);

        ffmpegRow = new QHBoxLayout();
        ffmpegRow->setSpacing(8);
        ffmpegRow->setObjectName("ffmpegRow");
        ffmpegPathEdit = new QLineEdit(pathGroup);
        ffmpegPathEdit->setObjectName("ffmpegPathEdit");

        ffmpegRow->addWidget(ffmpegPathEdit);

        ffmpegStatusBadge = new QLabel(pathGroup);
        ffmpegStatusBadge->setObjectName("ffmpegStatusBadge");
        ffmpegStatusBadge->setStyleSheet(QString::fromUtf8("color: #ffaa00; font-weight: bold;"));

        ffmpegRow->addWidget(ffmpegStatusBadge);

        browseFFmpegBtn = new QPushButton(pathGroup);
        browseFFmpegBtn->setObjectName("browseFFmpegBtn");
        browseFFmpegBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        ffmpegRow->addWidget(browseFFmpegBtn);


        pathGrid->addLayout(ffmpegRow, 1, 1, 1, 2);


        mainLayout->addWidget(pathGroup);

        actionSection = new QVBoxLayout();
        actionSection->setSpacing(6);
        actionSection->setObjectName("actionSection");
        progressInfoRow = new QHBoxLayout();
        progressInfoRow->setObjectName("progressInfoRow");
        progressTitle = new QLabel(contentContainer);
        progressTitle->setObjectName("progressTitle");

        progressInfoRow->addWidget(progressTitle);

        progressDetailLabel = new QLabel(contentContainer);
        progressDetailLabel->setObjectName("progressDetailLabel");
        progressDetailLabel->setStyleSheet(QString::fromUtf8("color: #00f0ff; font-weight: bold;"));

        progressInfoRow->addWidget(progressDetailLabel);

        progressSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        progressInfoRow->addItem(progressSpacer);


        actionSection->addLayout(progressInfoRow);

        progressBar = new QProgressBar(contentContainer);
        progressBar->setObjectName("progressBar");
        progressBar->setValue(0);

        actionSection->addWidget(progressBar);

        actionBtnRow = new QHBoxLayout();
        actionBtnRow->setSpacing(10);
        actionBtnRow->setObjectName("actionBtnRow");
        startBtn = new QPushButton(contentContainer);
        startBtn->setObjectName("startBtn");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(3);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(startBtn->sizePolicy().hasHeightForWidth());
        startBtn->setSizePolicy(sizePolicy);
        startBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        actionBtnRow->addWidget(startBtn);

        openFolderBtn = new QPushButton(contentContainer);
        openFolderBtn->setObjectName("openFolderBtn");
        openFolderBtn->setEnabled(false);
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(2);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(openFolderBtn->sizePolicy().hasHeightForWidth());
        openFolderBtn->setSizePolicy(sizePolicy1);
        openFolderBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        actionBtnRow->addWidget(openFolderBtn);

        playAudioBtn = new QPushButton(contentContainer);
        playAudioBtn->setObjectName("playAudioBtn");
        playAudioBtn->setEnabled(false);
        sizePolicy1.setHeightForWidth(playAudioBtn->sizePolicy().hasHeightForWidth());
        playAudioBtn->setSizePolicy(sizePolicy1);
        playAudioBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        actionBtnRow->addWidget(playAudioBtn);


        actionSection->addLayout(actionBtnRow);


        mainLayout->addLayout(actionSection);

        logGroup = new QGroupBox(contentContainer);
        logGroup->setObjectName("logGroup");
        logLayout = new QVBoxLayout(logGroup);
        logLayout->setSpacing(6);
        logLayout->setObjectName("logLayout");
        logLayout->setContentsMargins(10, 10, 10, 8);
        consoleLog = new QTextEdit(logGroup);
        consoleLog->setObjectName("consoleLog");
        consoleLog->setMinimumSize(QSize(0, 90));
        consoleLog->setReadOnly(true);

        logLayout->addWidget(consoleLog);

        logBottomRow = new QHBoxLayout();
        logBottomRow->setObjectName("logBottomRow");
        logSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        logBottomRow->addItem(logSpacer);

        clearLogBtn = new QPushButton(logGroup);
        clearLogBtn->setObjectName("clearLogBtn");
        clearLogBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        logBottomRow->addWidget(clearLogBtn);


        logLayout->addLayout(logBottomRow);


        mainLayout->addWidget(logGroup);

        scrollArea->setWidget(contentContainer);

        bgLayout->addWidget(scrollArea);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\360\237\214\214 \346\230\237\347\251\272\351\237\263\351\242\221\345\210\206\347\246\273\347\263\273\347\273\237 - Starry Audio Extractor (FFmpeg Powered)", nullptr));
        headerTitle->setText(QCoreApplication::translate("MainWindow", "\360\237\214\214 STARRY AUDIO EXTRACTOR", nullptr));
        headerSubtitle->setText(QCoreApplication::translate("MainWindow", "\342\234\246 \346\267\261\345\272\246\346\227\266\347\251\272\345\244\232\346\240\274\345\274\217\351\237\263\350\247\206\351\242\221\351\237\263\351\242\221\346\212\275\345\217\226\344\270\216\351\253\230\347\262\276\350\275\254\347\240\201\346\216\247\345\210\266\345\217\260 \342\234\246", nullptr));
        inputGroup->setTitle(QCoreApplication::translate("MainWindow", "\360\237\223\201 \350\276\223\345\205\245\346\272\220\350\247\206\351\242\221 (Source Video)", nullptr));
        inputVideoEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\257\267\351\200\211\346\213\251\346\210\226\346\213\226\346\213\275\351\237\263\350\247\206\351\242\221\346\226\207\344\273\266\350\207\263\346\255\244 (\346\224\257\346\214\201 MP4, MKV, AVI, MOV, FLV, TS, RMVB, WebM \347\255\211)...", nullptr));
        browseInputBtn->setText(QCoreApplication::translate("MainWindow", "\360\237\223\202 \346\265\217\350\247\210\350\247\206\351\242\221...", nullptr));
        fileInfoLabel->setText(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266\347\212\266\346\200\201: \346\234\252\350\275\275\345\205\245\350\247\206\351\242\221", nullptr));
        settingsGroup->setTitle(QCoreApplication::translate("MainWindow", "\342\232\231\357\270\217 \345\210\206\347\246\273\346\217\220\345\217\226\345\217\202\346\225\260 (Extraction Settings)", nullptr));
        radioTranscode->setText(QCoreApplication::translate("MainWindow", "\360\237\216\233\357\270\217 \346\240\274\345\274\217\351\207\215\347\274\226\347\240\201\350\275\254\346\215\242 (\350\207\252\345\256\232\344\271\211\346\240\274\345\274\217\343\200\201\347\240\201\347\216\207\344\270\216\351\207\207\346\240\267\347\216\207)", nullptr));
        radioDirectCopy->setText(QCoreApplication::translate("MainWindow", "\342\232\241 \346\236\201\351\200\237\346\227\240\346\215\237\346\265\201\346\213\267\350\264\235 (-c:a copy \346\257\253\347\247\222\347\272\247\346\217\220\345\217\226\345\216\237\351\237\263\350\275\250)", nullptr));
        formatLbl->setText(QCoreApplication::translate("MainWindow", "\347\233\256\346\240\207\351\237\263\351\242\221\346\240\274\345\274\217:", nullptr));
        bitrateLbl->setText(QCoreApplication::translate("MainWindow", "\351\237\263\351\242\221\346\257\224\347\211\271\347\216\207:", nullptr));
        sampleRateLbl->setText(QCoreApplication::translate("MainWindow", "\351\237\263\351\242\221\351\207\207\346\240\267\347\216\207:", nullptr));
        channelLbl->setText(QCoreApplication::translate("MainWindow", "\345\243\260\351\201\223\351\205\215\347\275\256:", nullptr));
        overwriteCheck->setText(QCoreApplication::translate("MainWindow", "\345\246\202\346\236\234\347\233\256\346\240\207\346\226\207\344\273\266\345\267\262\345\255\230\345\234\250\345\210\231\347\233\264\346\216\245\350\246\206\347\233\226 (Overwrite)", nullptr));
        pathGroup->setTitle(QCoreApplication::translate("MainWindow", "\360\237\222\276 \347\233\256\346\240\207\350\276\223\345\207\272\344\270\216 FFmpeg \347\216\257\345\242\203 (Output & Environment)", nullptr));
        outputLbl->setText(QCoreApplication::translate("MainWindow", "\351\237\263\351\242\221\350\276\223\345\207\272\350\267\257\345\276\204:", nullptr));
        outputPathEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\350\247\206\351\242\221\345\220\216\350\207\252\345\212\250\347\224\237\346\210\220\350\276\223\345\207\272\350\267\257\345\276\204\357\274\214\344\271\237\345\217\257\346\211\213\345\212\250\344\277\256\346\224\271...", nullptr));
        browseOutputBtn->setText(QCoreApplication::translate("MainWindow", "\360\237\223\202 \346\233\264\346\224\271\347\233\256\346\240\207...", nullptr));
        ffmpegLbl->setText(QCoreApplication::translate("MainWindow", "FFmpeg \346\211\247\350\241\214\350\267\257\345\276\204:", nullptr));
        ffmpegPathEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\207\252\345\212\250\346\243\200\346\265\213\347\263\273\347\273\237 PATH \346\210\226\347\250\213\345\272\217\345\220\214\347\272\247 ffmpeg.exe...", nullptr));
        ffmpegStatusBadge->setText(QCoreApplication::translate("MainWindow", "\360\237\224\215 \346\243\200\346\265\213\344\270\255...", nullptr));
        browseFFmpegBtn->setText(QCoreApplication::translate("MainWindow", "\360\237\233\240\357\270\217 \346\214\207\345\256\232 FFmpeg...", nullptr));
        progressTitle->setText(QCoreApplication::translate("MainWindow", "\342\232\241 \345\244\204\347\220\206\350\277\233\345\272\246:", nullptr));
        progressDetailLabel->setText(QCoreApplication::translate("MainWindow", "\345\260\261\347\273\252 (0%) - 00:00 / 00:00", nullptr));
        startBtn->setText(QCoreApplication::translate("MainWindow", "\360\237\232\200 \345\274\200\345\247\213\345\210\206\347\246\273\346\217\220\345\217\226\351\237\263\351\242\221", nullptr));
        openFolderBtn->setText(QCoreApplication::translate("MainWindow", "\360\237\223\201 \346\211\223\345\274\200\350\276\223\345\207\272\347\233\256\345\275\225", nullptr));
        playAudioBtn->setText(QCoreApplication::translate("MainWindow", "\360\237\216\265 \346\222\255\346\224\276\346\217\220\345\217\226\351\237\263\351\242\221", nullptr));
        logGroup->setTitle(QCoreApplication::translate("MainWindow", "\360\237\223\237 \344\273\273\345\212\241\346\216\247\345\210\266\345\217\260\346\227\245\345\277\227 (Terminal Logs)", nullptr));
        clearLogBtn->setText(QCoreApplication::translate("MainWindow", "\360\237\227\221\357\270\217 \346\270\205\347\251\272\346\216\247\345\210\266\345\217\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
