/*
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

/*
  serverentry.cpp  -  description
  begin:     Sun Feb 10 2002
  copyright: (C) 2002 by Dario Abatianni
  email:     eisfuchs@tigress.com
*/
#include <iostream>

#include <qstringlist.h>

#include "serverentry.h"

int ServerEntry::newId=0;

ServerEntry::ServerEntry(const QString newDefinition)
{
  /* Every entry gets a unique ID */
  id=newId++;
  definition=newDefinition;
  cerr << "definition(" << id << ") = " << definition << endl;
}

void ServerEntry::updateProperty(int property,const QString& value)
{
  QStringList properties=QStringList::split(',',getDefinition(),true);
  properties[property]=value;
  setDefinition(properties.join(","));
}

ServerEntry::~ServerEntry()
{
}
