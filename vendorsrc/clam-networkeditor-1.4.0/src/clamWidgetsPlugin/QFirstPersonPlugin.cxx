/****************************************************************************
**
** Copyright (C) 2005-2006 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "QFirstPersonPlugin.hxx"
#include "QFirstPerson.hxx"

#include <QtCore/QtPlugin>

QFirstPersonPlugin::QFirstPersonPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void QFirstPersonPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}

bool QFirstPersonPlugin::isInitialized() const
{
    return initialized;
}

QWidget *QFirstPersonPlugin::createWidget(QWidget *parent)
{
    return new QFirstPerson(parent);
}

QString QFirstPersonPlugin::name() const
{
    return "QFirstPerson";
}

QString QFirstPersonPlugin::group() const
{
    return "CLAM Widgets";
}

QIcon QFirstPersonPlugin::icon() const
{
    return QIcon(":/icons/images/firstperson.svg");
}

QString QFirstPersonPlugin::toolTip() const
{
    return "";
}

QString QFirstPersonPlugin::whatsThis() const
{
    return "";
}

bool QFirstPersonPlugin::isContainer() const
{
    return false;
}

QString QFirstPersonPlugin::domXml() const
{
    return "<widget class=\"QFirstPerson\" name=\"navigator\">\n"
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>300</width>\n"
           "   <height>200</height>\n"
           "  </rect>\n"
           " </property>\n"
           "</widget>\n";
}

QString QFirstPersonPlugin::includeFile() const
{
    return "QFirstPerson.hxx";
}

// This is just for a singleton widget plugin
// Q_EXPORT_PLUGIN2(clamwidgets, QFirstPersonPlugin)

