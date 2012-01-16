/** -*- c++ -*-
 * Copyright (C) 2009 Doug Judd (Zvents, Inc.)
 *
 * This file is part of Hypertable.
 *
 * Hypertable is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 3 of the
 * License, or any later version.
 *
 * Hypertable is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#ifndef HYPERTABLE_REQUESTHANDLERDUMP_H
#define HYPERTABLE_REQUESTHANDLERDUMP_H

#include "Common/Runnable.h"

#include "AsyncComm/ApplicationHandler.h"
#include "AsyncComm/Comm.h"
#include "AsyncComm/Event.h"


namespace Hypertable {

  class RangeServer;

  class RequestHandlerDump : public ApplicationHandler {
  public:
    RequestHandlerDump(Comm *comm, RangeServer *rs, EventPtr &event_ptr)
      : ApplicationHandler(event_ptr), m_comm(comm), m_range_server(rs) { }

    virtual void run();

  private:
    Comm        *m_comm;
    RangeServer *m_range_server;
  };

}

#endif // HYPERTABLE_REQUESTHANDLERDUMP_H
