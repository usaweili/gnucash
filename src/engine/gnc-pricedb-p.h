/********************************************************************
 * gnc-pricedb-p.h -- a simple price database for gnucash.          *
 * Copyright (C) 2001 Rob Browning                                  *
 * Copyright (C) 2003 Linas Vepstas <linas@linas.org>               *
 *                                                                  *
 * This program is free software; you can redistribute it and/or    *
 * modify it under the terms of the GNU General Public License as   *
 * published by the Free Software Foundation; either version 2 of   *
 * the License, or (at your option) any later version.              *
 *                                                                  *
 * This program is distributed in the hope that it will be useful,  *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    *
 * GNU General Public License for more details.                     *
 *                                                                  *
 * You should have received a copy of the GNU General Public License*
 * along with this program; if not, contact:                        *
 *                                                                  *
 * Free Software Foundation           Voice:  +1-617-542-5942       *
 * 59 Temple Place - Suite 330        Fax:    +1-617-542-2652       *
 * Boston, MA  02111-1307,  USA       gnu@gnu.org                   *
 *                                                                  *
 *******************************************************************/

#ifndef GNC_PRICEDB_P_H
#define GNC_PRICEDB_P_H

#include <glib.h>

#include "Backend.h"
#include "GNCIdP.h"
#include "gnc-engine.h"
#include "gnc-pricedb.h"
#include "qofbook.h"

struct gnc_price_s
{
  /* 'public' data fields */
  GUID    guid;                  /* globally unique price id */
  QofBook *book;                 /* book to which this price belongs to */

  GNCPriceDB *db;
  gnc_commodity *commodity;
  gnc_commodity *currency;
  Timespec tmspec;
  char *source;
  char *type;
  gnc_numeric value;
  gint32   version;              /* version number, for syncing with backend */
  guint32  version_check;        /* data aging timestamp */

  /* 'private' object management fields */
  QofEntityTable *entity_table;  /* table in which price is stored */
  guint32  refcount;             /* garbage collection reference count */
  gint32   editlevel;            /* nesting level of begin/end edit calls */
  gboolean not_saved;            /* price edit saved flag */
  gboolean do_free;              /* price is going to be destroyed soon */
};



struct gnc_price_db_s
{
  GHashTable *commodity_hash;
  QofBook *book;   /* book to which this database and all the prices belong to */
  gboolean dirty;
};

/* These structs define the kind of price lookup being done
 * so that it can be passed to the backend.  This is a rather
 * cheesy, low-brow interface.  It could stand improvement.
 */
typedef enum {
  LOOKUP_LATEST = 1,
  LOOKUP_ALL,
  LOOKUP_AT_TIME,
  LOOKUP_NEAREST_IN_TIME,
  LOOKUP_LATEST_BEFORE,
  LOOKUP_EARLIEST_AFTER
} PriceLookupType;


struct gnc_price_lookup_s
{
  PriceLookupType type;
  GNCPriceDB     *prdb;
  gnc_commodity  *commodity;
  gnc_commodity  *currency;
  Timespec        date;
};


typedef struct gnc_price_lookup_helper_s
{
  GList    **return_list;
  Timespec time;
} GNCPriceLookupHelper;

void     gnc_pricedb_set_db(QofBook *book, GNCPriceDB *db);

void     gnc_pricedb_mark_clean(GNCPriceDB *db);
void     gnc_pricedb_substitute_commodity(GNCPriceDB *db,
                                          gnc_commodity *old_c,
                                          gnc_commodity *new_c);
void     gnc_price_set_guid (GNCPrice *p, const GUID *guid);

/** register the pricedb object with the gncObject system */
gboolean gnc_pricedb_register (void);

Backend * xaccPriceDBGetBackend (GNCPriceDB *prdb);

#endif
