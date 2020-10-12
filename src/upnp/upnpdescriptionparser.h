/*
    SPDX-License-Identifier: GPL-2.0-or-later

    SPDX-FileCopyrightText: 2005-2007 Joris Guisson <joris.guisson@gmail.com>
    SPDX-FileCopyrightText: 2009 Michael Kreitzer <mrgrim@gr1m.org>
*/

#ifndef KTUPNPDESCRIPTIONPARSER_H
#define KTUPNPDESCRIPTIONPARSER_H

#include <QString>


namespace Konversation
{
    namespace UPnP
    {
        class UPnPRouter;

        /**
        * @author Joris Guisson
        *
        * Parses the xml description of a router.
        */
        class UPnPDescriptionParser
        {
        public:
            UPnPDescriptionParser();
            virtual ~UPnPDescriptionParser();

            /**
            * Parse the xml description.
            * @param file File it is located in
            * @param router The router off the xml description
            * @return true upon success
            */
            bool parse(const QString & file,UPnPRouter* router);


            /**
            * Parse the xml description.
            * @param data QByteArray with the data
            * @param router The router off the xml description
            * @return true upon success
            */
            bool parse(const QByteArray & data,UPnPRouter* router);
        };
    }
}

#endif
