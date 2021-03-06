// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*- 
// vim: ts=8 sw=2 smarttab
/*
 * Ceph - scalable distributed file system
 *
 * Copyright (C) 2004-2006 Sage Weil <sage@newdream.net>
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software 
 * Foundation.  See file COPYING.
 * 
 */

#ifndef CEPH_MOSDPGBACKFILL_H
#define CEPH_MOSDPGBACKFILL_H

#include "msg/Message.h"
#include "osd/osd_types.h"

class MOSDPGBackfill : public Message {
public:
  enum {
    OP_BACKFILL_PROGRESS = 2,
    OP_BACKFILL_FINISH = 3,
    OP_BACKFILL_FINISH_ACK = 4,
  };
  const char *get_op_name(int o) const {
    switch (o) {
    case OP_BACKFILL_PROGRESS: return "progress";
    case OP_BACKFILL_FINISH: return "finish";
    case OP_BACKFILL_FINISH_ACK: return "finish_ack";
    default: return "???";
    }
  }

  __u32 op;
  epoch_t map_epoch, query_epoch;
  pg_t pgid;
  hobject_t last_backfill;
  object_stat_collection_t stats;

  virtual void decode_payload() {
    bufferlist::iterator p = payload.begin();
    ::decode(op, p);
    ::decode(map_epoch, p);
    ::decode(query_epoch, p);
    ::decode(pgid, p);
    ::decode(last_backfill, p);
    ::decode(stats, p);
  }

  virtual void encode_payload(uint64_t features) {
    ::encode(op, payload);
    ::encode(map_epoch, payload);
    ::encode(query_epoch, payload);
    ::encode(pgid, payload);
    ::encode(last_backfill, payload);
    ::encode(stats, payload);
  }

  MOSDPGBackfill() : Message(MSG_OSD_PG_BACKFILL) {}
  MOSDPGBackfill(__u32 o, epoch_t e, epoch_t qe, pg_t p)
    : Message(MSG_OSD_PG_BACKFILL),
      op(o),
      map_epoch(e), query_epoch(e),
      pgid(p) {
  }
private:
  ~MOSDPGBackfill() {}

public:
  const char *get_type_name() const { return "pg_backfill"; }
  void print(ostream& out) const {
    out << "pg_backfill(" << get_op_name(op)
	<< " " << pgid
	<< " e " << map_epoch << "/" << query_epoch
	<< " lb " << last_backfill
	<< ")";
  }
};

#endif
