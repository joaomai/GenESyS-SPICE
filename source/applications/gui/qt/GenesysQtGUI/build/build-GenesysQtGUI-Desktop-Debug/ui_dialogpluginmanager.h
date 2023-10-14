/********************************************************************************
** Form generated from reading UI file 'dialogpluginmanager.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGPLUGINMANAGER_H
#define UI_DIALOGPLUGINMANAGER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DialogPluginManager
{
public:
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_2;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogPluginManager)
    {
        if (DialogPluginManager->objectName().isEmpty())
            DialogPluginManager->setObjectName(QString::fromUtf8("DialogPluginManager"));
        DialogPluginManager->setWindowModality(Qt::WindowModal);
        DialogPluginManager->resize(400, 300);
        verticalLayout = new QVBoxLayout(DialogPluginManager);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));

        verticalLayout->addLayout(verticalLayout_2);

        buttonBox = new QDialogButtonBox(DialogPluginManager);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(DialogPluginManager);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogPluginManager, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogPluginManager, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogPluginManager);
    } // setupUi

    void retranslateUi(QDialog *DialogPluginManager)
    {
        DialogPluginManager->setWindowTitle(QCoreApplication::translate("DialogPluginManager", "Plugin Manager", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogPluginManager: public Ui_DialogPluginManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGPLUGINMANAGER_H
