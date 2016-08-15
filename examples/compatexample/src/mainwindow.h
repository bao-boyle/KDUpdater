/****************************************************************************
** Copyright (C) 2001-2012 Klaralvdalens Datakonsult AB.  All rights reserved.
**
** This file is part of the KD Tools library.
**
** Licensees holding valid commercial KD Tools licenses may use this file in
** accordance with the KD Tools Commercial License Agreement provided with
** the Software.
**
**
** This file may be distributed and/or modified under the terms of the GNU
** Lesser General Public License version 2 and version 3 as published by the
** Free Software Foundation and appearing in the file LICENSE.LGPL included.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Contact info@kdab.net if any conditions of this licensing are not
** clear to you.
**
**********************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDir>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QTableWidget;
QT_END_NAMESPACE

namespace KDUpdater {
    class Target;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( KDUpdater::Target * target, QWidget * parent=0 );
    ~MainWindow();

private Q_SLOTS:
    void onAdd1();
    void onAdd2();
    void onUpdateSelected();
    void onUpdate();
    void onUpdateCompat();

private:
    void loadApplicationData();
    void updateObject(const QString &objectName, int object);
    void displayObject(int object, int row);
    void updatePackage( bool findCompat );

    KDUpdater::Target * m_target;
    QTableWidget *m_table;
    int m_lastId;
    QDir m_appDir;
    QDir m_objectsDir;
};

#endif
