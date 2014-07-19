/*
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

/*
  Copyright (C) 2003 Dario Abatianni <eisfuchs@tigress.com>
  Copyright (C) 2004 Peter Simonsson <psn@linux.se>
*/

#include "scriptlauncher.h"
#include "channel.h"
#include "application.h"
#include "server.h"

#include <QFileInfo>

#include <KGlobal>
#include <KProcess>
#include <KLocale>
#include <QStandardPaths>


ScriptLauncher::ScriptLauncher(QObject* parent) : QObject(parent)
{
    qputenv("KONVERSATION_LANG", KLocale::global()->language().toAscii());

    QStringList pythonPath(QProcessEnvironment::systemEnvironment().value("PYTHONPATH").split(':', QString::SkipEmptyParts));
    pythonPath << QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, "konversation/scripting_support/python");
    qputenv("PYTHONPATH", pythonPath.join(":").toLocal8Bit());
}

ScriptLauncher::~ScriptLauncher()
{
}

QString ScriptLauncher::scriptPath(const QString& script)
{
    return QStandardPaths::locate(QStandardPaths::GenericDataLocation, "konversation/scripts/" + script);
}

void ScriptLauncher::launchScript(int connectionId, const QString& target, const QString &parameter)
{
    // send the script all the information it will need
    QStringList parameterList = parameter.split(' ');

    // find script path (could be installed for all users in $KDEDIR/share/apps/ or
    // for one user alone in $HOME/.kde/share/apps/)
    QString script(parameterList.takeFirst());
    QString path = scriptPath(script);

    parameterList.prepend(target);
    parameterList.prepend(QString::number(connectionId));

    QFileInfo fileInfo(path);

    KProcess proc;
    proc.setWorkingDirectory(fileInfo.path());
    proc.setProgram(path, parameterList);

    QStringList pythonPath = QProcessEnvironment::systemEnvironment().value("PYTHONPATH").split(':', QString::SkipEmptyParts);
    pythonPath << QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, "konversation/scripting_support/python");
    proc.setEnv("PYTHONPATH", pythonPath.join(":"));

    if (proc.startDetached() == 0)
    {
        if (!fileInfo.exists())
           emit scriptNotFound(script);
        else
           emit scriptExecutionError(script);
    }
}

#include "scriptlauncher.moc"
