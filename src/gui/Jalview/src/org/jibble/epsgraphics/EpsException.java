/*
 Copyright Paul James Mutton, 2001-2004, http://www.jibble.org/

 This file is part of EpsGraphics2D.

 This software is dual-licensed, allowing you to choose between the GNU
 General Public License (GPL) and the www.jibble.org Commercial License.
 Since the GPL may be too restrictive for use in a proprietary application,
 a commercial license is also provided. Full license information can be
 found at http://www.jibble.org/licenses/

 $Author: jprocter $
 $Id: EpsException.java,v 1.2.4.1 2008/08/29 11:09:51 jprocter Exp $

 */

package org.jibble.epsgraphics;

public class EpsException extends RuntimeException
{

  public EpsException(String message)
  {
    super(message);
  }

}