/*
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

/*
  konversationapplication.cpp  -  description
  begin:     Mon Jan 28 2002
  copyright: (C) 2002 by Dario Abatianni
  email:     eisfuchs@tigress.com
*/

#include <iostream>

#include "konversationapplication.h"

/* include static variables */
Preferences KonversationApplication::preferences;
QStringList KonversationApplication::urlList;

KonversationApplication::KonversationApplication()
{
  cerr << "KonversationApplication::KonversationApplication()" << endl;

  config=new KSimpleConfig("konversationrc");
  readOptions();
  prefsDialog=new PrefsDialog(&preferences,true);

  connect(prefsDialog,SIGNAL (connectToServer(int)),this,SLOT (connectToServer(int)) );
  connect(prefsDialog,SIGNAL (cancelClicked()),this,SLOT (quitKonversation()) );
  connect(prefsDialog,SIGNAL (prefsChanged()),this,SLOT (saveOptions()) );

  serverList.setAutoDelete(true);     // delete items when they are removed

  prefsDialog->show();

  connect(&preferences,SIGNAL (requestServerConnection(int)),this,SLOT (connectToAnotherServer(int)) );
  connect(&preferences,SIGNAL (requestSaveOptions()),this,SLOT (saveOptions()) );
}

KonversationApplication::~KonversationApplication()
{
  saveOptions();
  cerr << "KonversationApplication::~KonversationApplication()" << endl;
}

void KonversationApplication::connectToServer(int id)
{
  cerr << "KonversationApplication::connectToServer(" << id << ")" << endl;

  connectToAnotherServer(id);
  // to prevent doubleClicked() to crash the dialog
  prefsDialog->delayedDestruct();
  prefsDialog=0;
}

void KonversationApplication::connectToAnotherServer(int id)
{
  cerr << "KonversationApplication::connectToAnotherServer(" << id << ")" << endl;

  Server* newServer=new Server(id);
  serverList.append(newServer);
  connect(newServer->getServerWindow(),SIGNAL(prefsChanged()),this,SLOT(saveOptions()));
}

void KonversationApplication::quitKonversation()
{
  cerr << "KonversationApplication::quitKonversation()" << endl;
  delete prefsDialog;
  this->exit();
}

void KonversationApplication::readOptions()
{
  cerr << "KonversationApplication::readOptions()" << endl;

  /* Read configuration and provide the default values */
  config->setGroup("General Options");

//  bool bViewStatusbar = config->readBoolEntry("Show Statusbar", true);
//  viewStatusBar->setChecked(bViewStatusbar);
//  slotViewStatusBar();

  /* bar position settings */
  preferences.serverWindowToolBarPos     =config->readNumEntry("ServerWindowToolBarPos",KToolBar::Top);
  preferences.serverWindowToolBarStatus  =config->readNumEntry("ServerWindowToolBarStatus",KToolBar::Show);
  preferences.serverWindowToolBarIconText=config->readNumEntry("ServerWindowToolBarIconText",KToolBar::IconTextBottom);
  preferences.serverWindowToolBarIconSize=config->readNumEntry("ServerWindowToolBarIconSize",0);

  /* Window geometries */
  preferences.serverWindowSize=config->readSizeEntry("Geometry");
  preferences.hilightSize=config->readSizeEntry("HilightGeometry");
  preferences.buttonsSize=config->readSizeEntry("ButtonsGeometry");

  /* Reasons */
  QString reason;
  reason=config->readEntry("PartReason","");
  if(reason!="") preferences.setPartReason(reason);
  reason=config->readEntry("KickReason","");
  if(reason!="") preferences.setKickReason(reason);

  /* User identity */
  config->setGroup("User Identity");
  preferences.ident=config->readEntry("Ident",preferences.ident);
  preferences.realname=config->readEntry("Realname",preferences.realname);

  QString nickList=config->readEntry("Nicknames",preferences.nicknameList.join(","));
  preferences.nicknameList=QStringList::split(",",nickList);

  /* Server List */
  config->setGroup("Server List");

  int index=0;
  /* Remove all default entries if there is at least one Server in the preferences file */
  if(config->hasKey("Server0")) preferences.clearServerList();
  /* Read all servers */
  while(config->hasKey(QString("Server%1").arg(index)))
  {
    preferences.addServer(config->readEntry(QString("Server%1").arg(index++)));
  }

  /* Quick Buttons List */
  config->setGroup("Button List");
  /* Read all buttons and overwrite default entries  */
  for(index=0;index<8;index++)
  {
    QString buttonKey(QString("Button%1").arg(index));
    if(config->hasKey(buttonKey)) preferences.buttonList[index]=config->readEntry(buttonKey);
  }

  /* Hilight List  */
  config->setGroup("Hilight List");
  QString hilight=config->readEntry("Hilight");
  QStringList hiList=QStringList::split(' ',hilight);

  unsigned int hiIndex;
  for(hiIndex=0;hiIndex<hiList.count();hiIndex++)
  {
    preferences.addHilight(hiList[hiIndex]);
  }

  if(config->hasKey("HilightColor"))
  {
    QString color=config->readEntry("HilightColor");
    preferences.setHilightColor(color);
  }

  /* Path settings */
  config->setGroup("Path Settings");
  preferences.logPath=config->readEntry("LogfilePath",preferences.logPath);

  /* Miscellaneous Flags */
  config->setGroup("Flags");
  preferences.setLog(config->readBoolEntry("Log",true));
  preferences.setBlinkingTabs(config->readBoolEntry("BlinkingTabs",true));
}

void KonversationApplication::saveOptions()
{
  cerr << "KonversationApplication::saveOptions()" << endl;

  config->setGroup("General Options");
  config->writeEntry("Geometry", preferences.serverWindowSize);
  config->writeEntry("HilightGeometry",preferences.hilightSize);
  config->writeEntry("ButtonsGeometry",preferences.buttonsSize);
//  config->writeEntry("Show Statusbar",viewStatusBar->isChecked());
  config->writeEntry("ServerWindowToolBarPos",preferences.serverWindowToolBarPos);
  config->writeEntry("ServerWindowToolBarStatus",preferences.serverWindowToolBarStatus);
  config->writeEntry("ServerWindowToolBarIconText",preferences.serverWindowToolBarIconText);
  config->writeEntry("ServerWindowToolBarIconSize",preferences.serverWindowToolBarIconSize);

  config->writeEntry("PartReason",preferences.getPartReason());
  config->writeEntry("KickReason",preferences.getKickReason());

  config->setGroup("User Identity");

  config->writeEntry("Ident",preferences.ident);
  config->writeEntry("Realname",preferences.realname);
  config->writeEntry("Nicknames",preferences.nicknameList);

  config->deleteGroup("Server List");
  config->setGroup("Server List");

  int index=0;
  QString serverEntry=preferences.getServerByIndex(0);

  while(serverEntry)
  {
    config->writeEntry(QString("Server%1").arg(index),serverEntry);
    serverEntry=preferences.getServerByIndex(++index);
  }

  config->setGroup("Button List");

  for(index=0;index<8;index++)
  {
    config->writeEntry(QString("Button%1").arg(index),preferences.buttonList[index]);
  }

  /* Write all hilight entries  */
  config->setGroup("Hilight List");

  QStringList hiList=preferences.getHilightList();
  QString hilight=hiList.join(" ");

  config->writeEntry("Hilight",hilight);

  QString color=preferences.getHilightColor();
  config->writeEntry("HilightColor",color);

  config->setGroup("Path Settings");
  config->writeEntry("LogfilePath",preferences.logPath);

  config->setGroup("Flags");
  config->writeEntry("Log",preferences.getLog());
  config->writeEntry("BlinkingTabs",preferences.getBlinkingTabs());

  config->sync();
}

void KonversationApplication::storeURL(QString& url)
{
  urlList.append(url);
}
