/*
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

/*
  scriptlauncher.cpp  -  Launches shell scripts
  begin:     Mit M�r 12 2003
  copyright: (C) 2003 by Dario Abatianni
  email:     eisfuchs@tigress.com

  $Id$
*/

#include <qstringlist.h>
#include <qfile.h>

#include <kdebug.h>
#include <kstddirs.h>
#include <kprocess.h>
#include <dcopclient.h>

#include "scriptlauncher.h"
#include "konversationapplication.h"

ScriptLauncher::ScriptLauncher()
{
  server=QString::null;
  target=QString::null;
}

ScriptLauncher::~ScriptLauncher()
{
}

void ScriptLauncher::setServerName(const QString& newName)
{
  server=newName;
}

void ScriptLauncher::setTargetName(const QString& newName)
{
  target=newName;
}

void ScriptLauncher::launchScript(const QString &parameter)
{
  KStandardDirs kstddir;
  QString scriptPath(kstddir.saveLocation("data",QString("konversation/scripts")));
  KProcess process;

  // send the script all the information it will need
  QStringList parameterList=QStringList::split(' ',parameter);

  process << scriptPath+QString("/")+parameterList[0]  // script path / name
          << kapp->dcopClient()->appId()      // our dcop port
          << server                           // the server we are connected to
          << target;                          // the target where the call came from

  // send remaining parameters to the script
  for(unsigned int index=1;index<parameterList.count();index++)
    process << parameterList[index];

  process.setWorkingDirectory(scriptPath);
  if(process.start()==false)
  {
    QFile file(scriptPath+QString("/")+parameterList[0]);
    if(!file.exists()) emit scriptNotFound(file.name());
    else emit scriptExecutionError(file.name());
  }

  // to free the script's stdin, otherwise backticks won't work
  process.detach();
}

#include "scriptlauncher.moc"

