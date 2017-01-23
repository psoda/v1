/*
 *                  BioJava development code
 *
 * This code may be freely distributed and modified under the
 * terms of the GNU Lesser General Public Licence.  This should
 * be distributed with the code.  If you do not have a copy,
 * see:
 *
 *      http://www.gnu.org/copyleft/lesser.html
 *
 * Copyright for this code is held jointly by the individual
 * authors.  These should be listed in @author doc comments.
 *
 * For more information on the BioJava project and its aims,
 * or to join the biojava-l mailing list, visit the home page
 * at:
 *
 *      http://www.biojava.org/
 *
 * Created on Mar 23, 2006
 *
 */
package org.biojava.dasobert.das2;

// import org.biojava.bio.program.das.dasalignment.DASException;
import org.biojava.dasobert.dasregistry.*;

public class DasSourceConverter
{

  public DasSourceConverter()
  {
    super();

  }

  /**
   * convert a das2 source to a das 1 source. This only will work if is passes
   * the Das2Source.isDas1Source() test i.e. this is really a das1 server there
   * 
   * @param das2source
   *                a DAS2Source to be converted
   * @return a Das1Source
   * @throws DASException
   */
  public static Das1Source toDas1Source(Das2Source das2source)
          throws Exception
  {
    if (!das2source.hasDas1Capabilities())
    {
      throw new Exception(
              "this das source does not have das1 capabilitites");
    }

    Das1Source ds = new Das1Source();
    ds.setAdminemail(das2source.getAdminemail());
    ds.setDescription(das2source.getDescription());
    ds.setHelperurl(das2source.getHelperurl());
    ds.setRegisterDate(das2source.getRegisterDate());
    ds.setLeaseDate(das2source.getLeaseDate());
    ds.setLabels(das2source.getLabels());
    ds.setCoordinateSystem(das2source.getCoordinateSystem());
    ds.setNickname(das2source.getNickname());
    ds.setId(das2source.getId());
    ds.setLabels(das2source.getLabels());

    // convert the capabilitites to das1 capabiltities and get the url
    Das2Capability[] caps = das2source.getDas2Capabilities();
    String[] das1capabilitites = new String[caps.length];
    int DASPREFIXLENGTH = Das2CapabilityImpl.DAS1_CAPABILITY_PREFIX
            .length();

    for (int i = 0; i < caps.length; i++)
    {
      Das2Capability cap = caps[i];

      String c = cap.getCapability();

      das1capabilitites[i] = c.substring(DASPREFIXLENGTH, c.length());

      String query_uri = cap.getQueryUri();

      String url = query_uri.substring(0,
              (query_uri.length() - c.length() + DASPREFIXLENGTH));
      ds.setUrl(url);
    }
    ds.setCapabilities(das1capabilitites);

    return ds;
  }

}
