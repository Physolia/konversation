/*
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/

/*
  server.h  -  description
  begin:     Sun Jan 20 2002
  copyright: (C) 2002 by Dario Abatianni
  email:     eisfuchs@tigress.com
*/

#include <kapplication.h>

#include <qlist.h>
#include <qtimer.h>

#ifndef SERVER_H
#define SERVER_H

#include "ircserversocket.h"
#include "channel.h"
#include "query.h"
#include "inputfilter.h"
#include "outputfilter.h"
#include "server.h"

#include "serverwindow.h"

/*
  Server Class to handle connection to the IRC server
  @author Dario Abatianni
*/

class Channel;
class Query;
class ServerWindow;
class InputFilter;

class Server : public QObject
{
  Q_OBJECT

  public:
    Server(int number);
    ~Server();

    QString getNextNickname();

    void setIrcName(QString& newIrcName) { ircName=*newIrcName; };
    void addNickToChannel(QString& channelName,QString& nickname,QString& hostmask,bool op,bool voice);
    void addHostmaskToNick(QString& sourceNick,QString& sourceHostmask);
    void nickJoinsChannel(QString& channelName,QString& nickname,QString& hostmask);
    void renameNick(QString& nickname,QString& newNick);
    void removeNickFromChannel(QString& channelName,QString& nickname,QString& reason,bool quit=false);
    void nickWasKickedFromChannel(QString& channelName,QString& nickname,QString& kicker,QString& reason);
    void removeNickFromServer(QString& nickname,QString& reason);

    bool isNickname(QString& compare);
    QString& getNickname();
    OutputFilter& getOutputFilter() { return outputFilter; };

    void joinChannel(QString& name,QString& hostmask);
    void removeChannel(Channel* channel);
    void appendToChannel(const char* channel,const char* nickname,const char* message);
    void appendActionToChannel(const char* channel,const char* nickname,const char* message);
    void appendServerMessageToChannel(const char* channel,const char* type,const char* message);
    void appendCommandMessageToChannel(const char* channel,const char* command,const char* message);
    void appendStatusMessage(const char* type,const char* message);

    void ctcpReply(QString& receiver,const QString& text);

    void setChannelTopic(QString& channel,QString &topic);
    void setChannelTopic(QString& nickname,QString& channel,QString &topic); // Overloaded
    void updateChannelMode(QString& nick,QString& channel,char mode,bool plus,QString& parameter);
    void updateChannelModeWidgets(QString& channel,char mode,QString& parameter);
    void updateChannelQuickButtons(QStringList newButtons);

    QString getNextQueryName();
    ServerWindow* getServerWindow() { return serverWindow; };

    void appendToQuery(const char* queryName,const char* message);
    void appendActionToQuery(const char* queryName,const char* message);
    void appendServerMessageToQuery(const char* queryName,const char* type,const char* message);
    void appendCommandMessageToQuery(const char* queryName,const char* command,const char* message);

    Channel* getChannelByName(const char* name);
    Query* getQueryByName(const char* name);
    QString parseWildcards(const QString& toParse,const QString& nickname,const QString& channelName,QStringList* nickList,const QString& queryName,const QString& parameter);

    QString getAutoJoinCommand();

  signals:
    void nicknameChanged(const QString&);

  public slots:
    void queue(const QString& buffer);
    void setNickname(const QString& newNickname);
    void addQuery(const QString& nickname,const QString& hostmask);
    void removeQuery(Query* query);

  protected slots:
    void incoming(KSocket *);
    void processIncomingData();
    void send(KSocket *);
    void broken(KSocket *);

  protected:
    void connectToIRCServer();

    unsigned int completeQueryPosition;
    unsigned int tryNickNumber;

    QString serverName;
    int serverPort;

    bool autoJoin;
    bool autoRejoin;
    bool autoReconnect;

    QString autoJoinChannel;
    QString autoJoinChannelKey;

    ServerWindow* serverWindow;
    IRCServerSocket* serverSocket;

    QTimer incomingTimer;

    QString ircName;
    QString inputBuffer;
    QString outputBuffer;
    QString nickname;

    QList<Channel> channelList;
    QList<Query> queryList;

    InputFilter inputFilter;
    OutputFilter outputFilter;
};

#endif
