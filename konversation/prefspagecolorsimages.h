/*
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

/*
  prefspagecolorsimages.h  -  Color and image preferences
  begin:     Don Jun 5 2003
  copyright: (C) 2003 by Dario Abatianni
  email:     eisfuchs@tigress.com

  $Id$
*/

#ifndef PREFSPAGECOLORSIMAGES_H
#define PREFSPAGECOLORSIMAGES_H

#include <qstringlist.h>

#include "prefspage.h"

/*
  @author Dario Abatianni
*/

class QColor;

class PrefsPageColorsImages : public PrefsPage
{
  Q_OBJECT

  public:
    PrefsPageColorsImages(QFrame* newParent,Preferences* newPreferences);
    ~PrefsPageColorsImages();

  protected slots:
    void colorChanged(const QColor& color);

  protected:
    QStringList colorList;
};

#endif
