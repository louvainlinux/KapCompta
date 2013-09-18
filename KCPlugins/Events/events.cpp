/*
 * Copyright (c) 2012-2013, Olivier Tilmans
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **/

#include "events.h"
#include "eventspanel.h"
#include <kcdatabase.h>

Events::Events()
{
    Q_INIT_RESOURCE(rsrc);
}

const QList<KCPanel*> Events::panels(KCAccountFile *account)
{
    QList<KCPanel*> l;
    l << new EventsPanel(account);
    return l;
}

void Events::init(KCAccountFile *account)
{
    KCDatabase db(account);
    db.query("CREATE TABLE events ("
             "id INTEGER PRIMARY KEY, "
             "name TEXT, "
             "date TEXT, "
             "misc TEXT"
             ")");
}

void Events::initDone(KCAccountFile *account)
{
    Q_UNUSED(account)
}
