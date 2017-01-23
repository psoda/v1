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
package jalview.ws;

import javax.swing.JMenu;

import ext.vamsas.*;
import jalview.gui.*;

public abstract class WSClient
{
  /**
   * WSClient holds the basic attributes that are displayed to the user for all
   * jalview web service clients
   */
  /**
   * displayed name for this web service
   */
  protected String WebServiceName;

  /**
   * specific job title (e.g. 'ClustalW Alignment of Selection from Aligment
   * from Cut and Paste input')
   */
  protected String WebServiceJobTitle;

  /**
   * String giving additional information such as method citations for this
   * service
   */
  protected String WebServiceReference;

  /**
   * Service endpoint
   */
  protected String WsURL;

  /**
   * Web service information used to initialise the WSClient attributes
   */
  protected WebserviceInfo wsInfo;

  /**
   * total number of jobs managed by this web service client instance.
   */
  int jobsRunning = 0;

  /**
   * TODO: this is really service metadata, and should be moved elsewhere.
   * mappings between abstract interface names and menu entries
   */
  protected java.util.Hashtable ServiceActions;
  {
    ServiceActions = new java.util.Hashtable();
    ServiceActions.put("MsaWS", "Multiple Sequence Alignment");
    ServiceActions.put("SecStrPred", "Secondary Structure Prediction");
  };

  public WSClient()
  {
  }

  /**
   * initialise WSClient service information attributes from the service handle
   * 
   * @param sh
   * @return the service instance information GUI for this client and job.
   */
  protected WebserviceInfo setWebService(ServiceHandle sh)
  {
    return setWebService(sh, false);
  }

  /**
   * original service handle that this client was derived from
   */
  ServiceHandle serviceHandle = null;

  /**
   * initialise WSClient service information attributes from the service handle
   * 
   * @param sh
   * @param headless
   *                true implies no GUI objects will be created.
   * @return the service instance information GUI for this client and job.
   */
  protected WebserviceInfo setWebService(ServiceHandle sh, boolean headless)
  {
    WebServiceName = sh.getName();
    if (ServiceActions.containsKey(sh.getAbstractName()))
    {
      WebServiceJobTitle = sh.getName(); // TODO: control sh.Name specification
                                          // properly
      // add this for short names. +(String)
      // ServiceActions.get(sh.getAbstractName());
    }
    else
    {
      WebServiceJobTitle = sh.getAbstractName() + " using " + sh.getName();

    }
    WebServiceReference = sh.getDescription();
    WsURL = sh.getEndpointURL();
    WebserviceInfo wsInfo = null;
    if (!headless)
    {
      wsInfo = new WebserviceInfo(WebServiceJobTitle, WebServiceReference);
    }
    return wsInfo;
  }

  /**
   * convenience method to pass the serviceHandle reference that instantiated
   * this service on to the menu entry constructor
   * 
   * @param wsmenu
   *                the menu to which any menu entries/sub menus are to be
   *                attached
   * @param alignFrame
   *                the alignFrame instance that provides input data for the
   *                service
   */
  public void attachWSMenuEntry(JMenu wsmenu, final AlignFrame alignFrame)
  {
    if (serviceHandle == null)
    {
      throw new Error(
              "IMPLEMENTATION ERROR: cannot attach WS Menu Entry without service handle reference!");
    }
    attachWSMenuEntry(wsmenu, serviceHandle, alignFrame);
  }

  /**
   * method implemented by each WSClient implementation that creates menu
   * entries that enact their service using data from alignFrame.
   * 
   * @param wsmenu
   *                where new menu entries (and submenus) are to be attached
   * @param serviceHandle
   *                the serviceHandle document for the service that entries are
   *                created for
   * @param alignFrame
   */
  public abstract void attachWSMenuEntry(JMenu wsmenu,
          final ServiceHandle serviceHandle, final AlignFrame alignFrame);
}
