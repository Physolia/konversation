/*
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

/*
  prefsdialog.h  -  This class holds the subpages for the preferences dialog
  begin:     Sun Feb 10 2002
  copyright: (C) 2002 by Dario Abatianni
  email:     eisfuchs@tigress.com
*/

#ifndef PREFSDIALOG_H
#define PREFSDIALOG_H

#include <kdialogbase.h>

#include "preferences.h"
#include "prefsdialog.h"
#include "prefspageidentity.h"
#include "prefspagebuttons.h"
#include "prefspagelog.h"
#include "prefspagedccsettings.h"
#include "prefspagedialogs.h"
#include "prefspagehighlight.h"
#include "prefspagenotify.h"
#include "prefspageosd.h"
#include "prefspageignore.h"
#include "prefspagealiases.h"
#include "prefspagetabbehavior.h"
// TODO: uncomment this when it's ready to go
// #include "prefspagescripts.h"


/*
 *@author Dario Abatianni
*/

class QFrame;

class PrefsDialog : public KDialogBase
{
  Q_OBJECT

  public:
    PrefsDialog(Preferences* preferences);
    ~PrefsDialog();

    void openPage(Preferences::Pages page);

  signals:
    void applyPreferences();
    void prefsChanged();
    void closed();

  protected slots:
    void slotOk();
    void slotApply();
    void slotCancel();
    
    void slotAboutToShowPage(QWidget* page);

  protected:
    Preferences* preferences;

    PrefsPageIdentity*        identityPage;
    PrefsPageTabBehavior*     tabBehaviorPage;
    PrefsPageButtons*         buttonsPage;
    PrefsPageNotify*          notifyPage;
    PrefsPageHighlight*       highlightPage;
    PrefsPageOSD*             OSDPage;
    PrefsPageIgnore*          ignorePage;
    PrefsPageAliases*         aliasesPage;
    PrefsPageLog*             logSettingsPage;
    PrefsPageDccSettings*     dccSettingsPage;
    PrefsPageDialogs*         dialogsPage;

    // for openPage();
    QFrame* notifyPane;
    QFrame* identityPane;
    
    // for slotAboutToShowPage()
    QFrame* OSDPane;
    
    QWidget* lastPane;

    void setPreferences(Preferences* newPrefs);
};

#endif
