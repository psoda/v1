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

public interface WSClientI
{
  /**
   * basic interface supported by web service clients used by
   * jalview.gui.WebserviceInfo to discover GUI properties and pass events back
   * to the client.
   * 
   */
  /**
   * TODO: change this to be a WS Job Panel GUI 'attribute'
   * 
   * @return boolean true if a job cancel button should be shown
   */
  boolean isCancellable();

  /**
   * TODO: change this to be a WS Job Panel GUI 'attribute'
   * 
   * @return boolean true if results can be merged into the source of input data
   */
  boolean canMergeResults();

  /**
   * instruct client to cancel the job. This is also used by the GUI to
   */
  void cancelJob();
}
