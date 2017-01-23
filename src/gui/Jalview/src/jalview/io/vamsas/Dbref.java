/*
 * Jalview - A Sequence Alignment Editor and Viewer (Version 2.4)
 * Copyright (C) 2008 AM Waterhouse, J Procter, G Barton, M Clamp, S Searle
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */
package jalview.io.vamsas;

import jalview.datamodel.DBRefEntry;
import jalview.datamodel.SequenceI;
import uk.ac.vamsas.objects.core.DbRef;
import uk.ac.vamsas.objects.core.Map;
import uk.ac.vamsas.objects.core.Sequence;
import jalview.io.VamsasAppDatastore;

public class Dbref extends Rangetype
{
  jalview.datamodel.SequenceI sq = null;

  uk.ac.vamsas.objects.core.Sequence sequence = null;

  DbRef dbref = null;

  DBRefEntry dbentry = null;

  public Dbref(VamsasAppDatastore datastore, DBRefEntry dbentry,
          jalview.datamodel.SequenceI sq2,
          uk.ac.vamsas.objects.core.Sequence sequence2)
  {
    super(datastore);
    dbref = (DbRef) getjv2vObj(dbentry);
    sq = sq2;
    sequence = sequence2;
    this.dbentry = dbentry;
    if (dbref == null)
    {
      add();
    }
    else
    {
      if (dbref.isUpdated())
      {
        conflict();
      }
      else
      {
        update();
      }

    }

  }

  public Dbref(VamsasAppDatastore datastore, DbRef ref, Sequence vdseq,
          SequenceI dsseq)
  {
    super(datastore);
    dbref = ref;
    sequence = vdseq;
    sq = dsseq;
    dbentry = (jalview.datamodel.DBRefEntry) getvObj2jv(dbref);
    if (dbentry == null)
    {
      addFromDocument();
    }
    else
    {
      if (dbref.isUpdated())
      {
        update();
      }
    }
  }

  private void update()
  {
    // TODO: verify and update dbrefs in vamsas document
    // there will be trouble when a dataset sequence is modified to
    // contain more residues than were originally referenced - we must
    // then make a number of dataset sequence entries - this info is already
    // stored
    jalview.bin.Cache.log
            .debug("TODO verify update of dataset sequence database references.");
  }

  private void conflict()
  {
    jalview.bin.Cache.log.debug("Conflict in dbentry update for "
            + dbref.getAccessionId() + dbref.getSource() + " "
            + dbref.getVorbaId());
    // TODO Auto-generated method stub

  }

  private void addFromDocument()
  {
    // add new dbref
    sq.addDBRef(dbentry = new jalview.datamodel.DBRefEntry(dbref
            .getSource().toString(), dbref.getVersion().toString(), dbref
            .getAccessionId().toString()));
    if (dbref.getMapCount() > 0)
    {
      // TODO: Jalview ignores all the other maps
      if (dbref.getMapCount() > 1)
      {
        jalview.bin.Cache.log
                .debug("Ignoring additional mappings on DbRef: "
                        + dbentry.getSource() + ":"
                        + dbentry.getAccessionId());
      }
      jalview.datamodel.Mapping mp = new jalview.datamodel.Mapping(
              parsemapType(dbref.getMap(0)));
      dbentry.setMap(mp);
    }
    // TODO: jalview ignores links and properties because it doesn't know what
    // to do with them.

    bindjvvobj(dbentry, dbref);
  }

  private void add()
  {
    DbRef dbref = new DbRef();
    bindjvvobj(dbentry, dbref);
    dbref.setAccessionId(dbentry.getAccessionId());
    dbref.setSource(dbentry.getSource());
    dbref.setVersion(dbentry.getVersion());
    if (dbentry.getMap() != null)
    {
      jalview.datamodel.Mapping mp = dbentry.getMap();
      if (mp.getMap() != null)
      {
        Map vMap = new Map();
        initMapType(vMap, mp.getMap(), true);
        dbref.addMap(vMap);
      }
      else
      {
        jalview.bin.Cache.log.debug("Ignoring mapless DbRef.Map "
                + dbentry.getSrcAccString());
      }
    }
    sequence.addDbRef(dbref);
  }

}
