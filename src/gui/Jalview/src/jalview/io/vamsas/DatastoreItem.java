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

import jalview.bin.Cache;
import jalview.gui.TreePanel;
import jalview.io.VamsasAppDatastore;

import java.util.Enumeration;
import java.util.Hashtable;
import java.util.IdentityHashMap;
import java.util.Vector;

import uk.ac.vamsas.client.IClientDocument;
import uk.ac.vamsas.client.Vobject;
import uk.ac.vamsas.client.VorbaId;
import uk.ac.vamsas.objects.core.Entry;
import uk.ac.vamsas.objects.core.Provenance;
import uk.ac.vamsas.objects.core.Seg;

/**
 * Holds all the common machinery for binding objects to vamsas objects
 * 
 * @author JimP
 * 
 */
public class DatastoreItem
{
  /**
   * 
   */
  Entry provEntry = null;

  IClientDocument cdoc;

  Hashtable vobj2jv;

  IdentityHashMap jv2vobj;

  /**
   * @return the Vobject bound to Jalview datamodel object
   */
  protected Vobject getjv2vObj(Object jvobj)
  {
    if (jv2vobj.containsKey(jvobj))
    {
      return cdoc.getObject((VorbaId) jv2vobj.get(jvobj));
    }
    if (Cache.log.isDebugEnabled())
    {
      Cache.log.debug("Returning null VorbaID binding for jalview object "
              + jvobj);
    }
    return null;
  }

  /**
   * 
   * @param vobj
   * @return Jalview datamodel object bound to the vamsas document object
   */
  protected Object getvObj2jv(uk.ac.vamsas.client.Vobject vobj)
  {
    if (vobj2jv == null)
      return null;
    VorbaId id = vobj.getVorbaId();
    if (id == null)
    {
      id = cdoc.registerObject(vobj);
      Cache.log
              .debug("Registering new object and returning null for getvObj2jv");
      return null;
    }
    if (vobj2jv.containsKey(vobj.getVorbaId()))
    {
      return vobj2jv.get(vobj.getVorbaId());
    }
    return null;
  }

  protected void bindjvvobj(Object jvobj, uk.ac.vamsas.client.Vobject vobj)
  {
    VorbaId id = vobj.getVorbaId();
    if (id == null)
    {
      id = cdoc.registerObject(vobj);
      if (id == null || vobj.getVorbaId() == null
              || cdoc.getObject(id) != vobj)
      {
        Cache.log.error("Failed to get id for "
                + (vobj.isRegisterable() ? "registerable"
                        : "unregisterable") + " object " + vobj);
      }
    }

    if (vobj2jv.containsKey(vobj.getVorbaId())
            && !((VorbaId) vobj2jv.get(vobj.getVorbaId())).equals(jvobj))
    {
      Cache.log.debug(
              "Warning? Overwriting existing vamsas id binding for "
                      + vobj.getVorbaId(), new Exception(
                      "Overwriting vamsas id binding."));
    }
    else if (jv2vobj.containsKey(jvobj)
            && !((VorbaId) jv2vobj.get(jvobj)).equals(vobj.getVorbaId()))
    {
      Cache.log.debug(
              "Warning? Overwriting existing jalview object binding for "
                      + jvobj, new Exception(
                      "Overwriting jalview object binding."));
    }
    /*
     * Cache.log.error("Attempt to make conflicting object binding! "+vobj+" id "
     * +vobj.getVorbaId()+" already bound to "+getvObj2jv(vobj)+" and "+jvobj+"
     * already bound to "+getjv2vObj(jvobj),new Exception("Excessive call to
     * bindjvvobj")); }
     */
    // we just update the hash's regardless!
    Cache.log.debug("Binding " + vobj.getVorbaId() + " to " + jvobj);
    vobj2jv.put(vobj.getVorbaId(), jvobj);
    // JBPNote - better implementing a hybrid invertible hash.
    jv2vobj.put(jvobj, vobj.getVorbaId());
  }

  public DatastoreItem()
  {
    super();
  }

  public DatastoreItem(VamsasAppDatastore datastore)
  {
    this();
    initDatastoreItem(datastore);
    // TODO Auto-generated constructor stub
  }

  VamsasAppDatastore datastore = null;

  public void initDatastoreItem(VamsasAppDatastore ds)
  {
    datastore = ds;
    initDatastoreItem(ds.getProvEntry(), ds.getClientDocument(), ds
            .getVamsasObjectBinding(), ds.getJvObjectBinding());
  }

  public void initDatastoreItem(Entry provEntry, IClientDocument cdoc,
          Hashtable vobj2jv, IdentityHashMap jv2vobj)
  {
    this.provEntry = provEntry;
    this.cdoc = cdoc;
    this.vobj2jv = vobj2jv;
    this.jv2vobj = jv2vobj;
  }

  protected boolean isModifiable(String modifiable)
  {
    return modifiable == null; // TODO: USE VAMSAS LIBRARY OBJECT LOCK METHODS)
  }

  protected Vector getjv2vObjs(Vector alsq)
  {
    Vector vObjs = new Vector();
    Enumeration elm = alsq.elements();
    while (elm.hasMoreElements())
    {
      vObjs.addElement(getjv2vObj(elm.nextElement()));
    }
    return vObjs;
  }

  // utility functions
  /**
   * get start<end range of segment, adjusting for inclusivity flag and
   * polarity.
   * 
   * @param visSeg
   * @param ensureDirection
   *                when true - always ensure start is less than end.
   * @return int[] { start, end, direction} where direction==1 for range running
   *         from end to start.
   */
  public int[] getSegRange(Seg visSeg, boolean ensureDirection)
  {
    boolean incl = visSeg.getInclusive();
    // adjust for inclusive flag.
    int pol = (visSeg.getStart() <= visSeg.getEnd()) ? 1 : -1; // polarity of
    // region.
    int start = visSeg.getStart() + (incl ? 0 : pol);
    int end = visSeg.getEnd() + (incl ? 0 : -pol);
    if (ensureDirection && pol == -1)
    {
      // jalview doesn't deal with inverted ranges, yet.
      int t = end;
      end = start;
      start = t;
    }
    return new int[]
    { start, end, pol < 0 ? 1 : 0 };
  }

  /**
   * provenance bits
   */
  protected jalview.datamodel.Provenance getJalviewProvenance(
          Provenance prov)
  {
    // TODO: fix App and Action entries and check use of provenance in jalview.
    jalview.datamodel.Provenance jprov = new jalview.datamodel.Provenance();
    for (int i = 0; i < prov.getEntryCount(); i++)
    {
      jprov.addEntry(prov.getEntry(i).getUser(), prov.getEntry(i)
              .getAction(), prov.getEntry(i).getDate(), prov.getEntry(i)
              .getId());
    }

    return jprov;
  }

  /**
   * 
   * @return default initial provenance list for a Jalview created vamsas
   *         object.
   */
  Provenance dummyProvenance()
  {
    return dummyProvenance(null);
  }

  protected Entry dummyPEntry(String action)
  {
    Entry entry = new Entry();
    entry.setApp(this.provEntry.getApp());
    if (action != null)
    {
      entry.setAction(action);
    }
    else
    {
      entry.setAction("created.");
    }
    entry.setDate(new java.util.Date());
    entry.setUser(this.provEntry.getUser());
    return entry;
  }

  protected Provenance dummyProvenance(String action)
  {
    Provenance prov = new Provenance();
    prov.addEntry(dummyPEntry(action));
    return prov;
  }

  protected void addProvenance(Provenance p, String action)
  {
    p.addEntry(dummyPEntry(action));
  }
}
