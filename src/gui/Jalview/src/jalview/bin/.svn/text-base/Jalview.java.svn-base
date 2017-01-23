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
package jalview.bin;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.lang.reflect.Constructor;
import java.net.URL;
import java.util.*;

import javax.swing.*;

import jalview.gui.*;
import jalview.io.AppletFormatAdapter;

/**
 * Main class for Jalview Application <br>
 * <br>
 * start with java -Djava.ext.dirs=$PATH_TO_LIB$ jalview.bin.Jalview
 * 
 * @author $author$
 * @version $Revision: 1.56.2.2 $
 */
public class Jalview
{

  /**
   * main class for Jalview application
   * 
   * @param args
   *                open <em>filename</em>
   */
  public static void main(String[] args)
  {
    System.out.println("Java version: "
            + System.getProperty("java.version"));
    System.out.println(System.getProperty("os.arch") + " "
            + System.getProperty("os.name") + " "
            + System.getProperty("os.version"));

    ArgsParser aparser = new ArgsParser(args);
    boolean headless = false;

    if (aparser.contains("help") || aparser.contains("h"))
    {
      System.out
              .println("Usage: jalview -open [FILE] [OUTPUT_FORMAT] [OUTPUT_FILE]\n\n"
                      + "-nodisplay\tRun Jalview without User Interface.\n"
                      + "-props FILE\tUse the given Jalview properties file instead of users default.\n"
                      + "-annotations FILE\tAdd precalculated annotations to the alignment.\n"
                      + "-tree FILE\tLoad the given newick format tree file onto the alignment\n"
                      + "-features FILE\tUse the given file to mark features on the alignment.\n"
                      + "-fasta FILE\tCreate alignment file FILE in Fasta format.\n"
                      + "-clustal FILE\tCreate alignment file FILE in Clustal format.\n"
                      + "-pfam FILE\tCreate alignment file FILE in PFAM format.\n"
                      + "-msf FILE\tCreate alignment file FILE in MSF format.\n"
                      + "-pileup FILE\tCreate alignment file FILE in Pileup format\n"
                      + "-pir FILE\tCreate alignment file FILE in PIR format.\n"
                      + "-blc FILE\tCreate alignment file FILE in BLC format.\n"
                      + "-jalview FILE\tCreate alignment file FILE in Jalview format.\n"
                      + "-png FILE\tCreate PNG image FILE from alignment.\n"
                      + "-imgMap FILE\tCreate HTML file FILE with image map of PNG image.\n"
                      + "-eps FILE\tCreate EPS file FILE from alignment."
                      + "-questionnaire URL\tQueries the given URL for information about any Jalview user questionnaires."
                      + "-noquestionnaire\tTurn off questionnaire check."
                      + "-dasserver nickname=URL\tAdd and enable a das server with given nickname (alphanumeric or underscores only) for retrieval of features for all alignments."
                      + "-fetchfrom nickname\tQuery nickname for features for the alignments and display them."
                      + "-groovy FILE\tExecute groovy script in FILE, after all other arguments have been processed (if FILE is the text 'STDIN' then the file will be read from STDIN)"
                      + "\n\n~Read documentation in Application or visit http://www.jalview.org for description of Features and Annotations file~\n\n");
      System.exit(0);
    }

    Cache.loadProperties(aparser.getValue("props")); // must do this before
    // anything else!

    if (aparser.contains("nodisplay"))
    {
      System.setProperty("java.awt.headless", "true");
    }
    if (System.getProperty("java.awt.headless") != null
            && System.getProperty("java.awt.headless").equals("true"))
    {
      headless = true;
    }

    try
    {
      Cache.initLogger();
    } catch (java.lang.NoClassDefFoundError error)
    {
      error.printStackTrace();
      System.out
              .println("\nEssential logging libraries not found."
                      + "\nUse: java -Djava.ext.dirs=$PATH_TO_LIB$ jalview.bin.Jalview");
      System.exit(0);
    }

    Desktop desktop = null;

    try
    {
      UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName()
      // UIManager.getCrossPlatformLookAndFeelClassName()
              // "com.sun.java.swing.plaf.gtk.GTKLookAndFeel"
              // "javax.swing.plaf.metal.MetalLookAndFeel"
              // "com.sun.java.swing.plaf.windows.WindowsLookAndFeel"
              // "com.sun.java.swing.plaf.motif.MotifLookAndFeel"

              );
    } catch (Exception ex)
    {
    }
    if (!headless)
    {
      desktop = new Desktop();
      desktop.setVisible(true);
      desktop.discoverer.start();
      if (!aparser.contains("noquestionnaire"))
      {
        String url = aparser.getValue("questionnaire");
        if (url != null)
        {
          // Start the desktop questionnaire prompter with the specified
          // questionnaire
          Cache.log.debug("Starting questionnaire url at " + url);
          desktop.checkForQuestionnaire(url);
        }
        else
        {
          if (Cache.getProperty("NOQUESTIONNAIRES") == null)
          {
            // Start the desktop questionnaire prompter with the specified
            // questionnaire
            // String defurl =
            // "http://anaplog.compbio.dundee.ac.uk/cgi-bin/questionnaire.pl";
            // //
            String defurl = "http://www.jalview.org/cgi-bin/questionnaire.pl";
            Cache.log.debug("Starting questionnaire with default url: "
                    + defurl);
            desktop.checkForQuestionnaire(defurl);

          }
        }
      }
    }

    String file = null, protocol = null, format = null, data = null;
    jalview.io.FileLoader fileLoader = new jalview.io.FileLoader();
    Vector getFeatures = null; // vector of das source nicknames to fetch
                                // features from
    // loading is done.
    String groovyscript = null; // script to execute after all loading is
                                // completed one way or another
    // extract groovy argument and execute if necessary
    groovyscript = aparser.getValue("groovy");
    file = aparser.getValue("open");

    if (file == null && desktop == null)
    {
      System.out.println("No files to open!");
      System.exit(1);
    }

    if (file != null)
    {
      System.out.println("Opening file: " + file);

      if (!file.startsWith("http://"))
      {
        if (!(new java.io.File(file)).exists())
        {
          System.out.println("Can't find " + file);
          if (headless)
          {
            System.exit(1);
          }
        }
      }

      protocol = checkProtocol(file);

      format = new jalview.io.IdentifyFile().Identify(file, protocol);

      AlignFrame af = fileLoader.LoadFileWaitTillLoaded(file, protocol,
              format);

      if (af == null)
      {
        System.out.println("error");
        return;
      }

      data = aparser.getValue("colour");
      if (data != null)
      {
        data.replaceAll("%20", " ");

        jalview.schemes.ColourSchemeI cs = jalview.schemes.ColourSchemeProperty
                .getColour(af.getViewport().getAlignment(), data);

        if (cs == null)
        {
          jalview.schemes.UserColourScheme ucs = new jalview.schemes.UserColourScheme(
                  "white");
          ucs.parseAppletParameter(data);
          cs = ucs;
        }

        System.out.println("colour is " + data);
        af.changeColour(cs);
      }

      // Must maintain ability to use the groups flag
      data = aparser.getValue("groups");
      if (data != null)
      {
        af.parseFeaturesFile(data, checkProtocol(data));
        System.out.println("Added " + data);
      }
      data = aparser.getValue("features");
      if (data != null)
      {
        af.parseFeaturesFile(data, checkProtocol(data));
        System.out.println("Added " + data);
      }

      data = aparser.getValue("annotations");
      if (data != null)
      {
        af.loadJalviewDataFile(data);
        System.out.println("Added " + data);
      }
      data = aparser.getValue("tree");
      if (data != null)
      {
        jalview.io.NewickFile fin = null;
        try
        {
          fin = new jalview.io.NewickFile(data, checkProtocol(data));
          if (fin != null)
          {
            af.getViewport().setCurrentTree(
                    af.ShowNewickTree(fin, data).getTree());
            System.out.println("Added tree " + data);
          }
        } catch (IOException ex)
        {
          System.err.println("Couldn't add tree " + data);
          ex.printStackTrace(System.err);
        }
      }
      getFeatures = checkDasArguments(aparser);
      if (af != null && getFeatures != null)
      {
        startFeatureFetching(getFeatures);
        // need to block until fetching is complete.
        while (af.operationInProgress())
        {
          // wait around until fetching is finished.
          try
          {
            Thread.sleep(10);
          } catch (Exception e)
          {

          }
        }
      }
      if (groovyscript != null)
      {
        // Execute the groovy script after we've done all the rendering stuff
        // and before any images or figures are generated.
        if (jalview.bin.Cache.groovyJarsPresent())
        {
          System.out.println("Executing script " + groovyscript);
          executeGroovyScript(groovyscript, desktop);
        }
        else
        {
          System.err
                  .println("Sorry. Groovy Support is not available, so ignoring the provided groovy script "
                          + groovyscript);
        }
        groovyscript = null;
      }
      String imageName = "unnamed.png";
      while (aparser.getSize() > 1)
      {
        format = aparser.nextValue();
        file = aparser.nextValue();

        if (format.equalsIgnoreCase("png"))
        {
          af.createPNG(new java.io.File(file));
          imageName = (new java.io.File(file)).getName();
          System.out.println("Creating PNG image: " + file);
          continue;
        }
        else if (format.equalsIgnoreCase("imgMap"))
        {
          af.createImageMap(new java.io.File(file), imageName);
          System.out.println("Creating image map: " + file);
          continue;
        }
        else if (format.equalsIgnoreCase("eps"))
        {
          System.out.println("Creating EPS file: " + file);
          af.createEPS(new java.io.File(file));
          continue;
        }

        if (af.saveAlignment(file, format))
        {
          System.out.println("Written alignment in " + format
                  + " format to " + file);
        }
        else
        {
          System.out.println("Error writing file " + file + " in " + format
                  + " format!!");
        }

      }

      while (aparser.getSize() > 0)
      {
        System.out.println("Unknown arg: " + aparser.nextValue());
      }
    }
    AlignFrame startUpAlframe = null;
    // We'll only open the default file if the desktop is visible.
    // And the user
    // ////////////////////
    if (!headless && file == null
            && jalview.bin.Cache.getDefault("SHOW_STARTUP_FILE", true))
    {
      file = jalview.bin.Cache.getDefault("STARTUP_FILE",
              "http://www.jalview.org/examples/exampleFile_2_3.jar");

      protocol = "File";

      if (file.indexOf("http:") > -1)
      {
        protocol = "URL";
      }

      if (file.endsWith(".jar"))
      {
        format = "Jalview";
      }
      else
      {
        format = new jalview.io.IdentifyFile().Identify(file, protocol);
      }

      startUpAlframe = fileLoader.LoadFileWaitTillLoaded(file, protocol,
              format);
      getFeatures = checkDasArguments(aparser);
      // extract groovy arguments before anything else.
    }
    // Once all loading is done. Retrieve features.
    if (getFeatures != null)
    {
      if (startUpAlframe != null)
      {
        startFeatureFetching(getFeatures);
      }
    }
    if (groovyscript != null)
    {
      if (jalview.bin.Cache.groovyJarsPresent())
      {
        System.out.println("Executing script " + groovyscript);
        executeGroovyScript(groovyscript, desktop);
      }
      else
      {
        System.err
                .println("Sorry. Groovy Support is not available, so ignoring the provided groovy script "
                        + groovyscript);
      }
    }

    // Once all other stuff is done, execute any groovy scripts (in order)
  }

  /**
   * Locate the given string as a file and pass it to the groovy interpreter.
   * 
   * @param groovyscript
   *                the script to execute
   * @param jalviewContext
   *                the Jalview Desktop object passed in to the groovy binding
   *                as the 'Jalview' object.
   */
  private static void executeGroovyScript(String groovyscript,
          Object jalviewContext)
  {
    if (jalviewContext == null)
    {
      System.err
              .println("Sorry. Groovy support is currently only available when running with the Jalview GUI enabled.");
    }
    File sfile = null;
    if (groovyscript.trim().equals("STDIN"))
    {
      // read from stdin into a tempfile and execute it
      try
      {
        sfile = File.createTempFile("jalview", "groovy");
        PrintWriter outfile = new PrintWriter(new OutputStreamWriter(
                new FileOutputStream(sfile)));
        BufferedReader br = new BufferedReader(
                new java.io.InputStreamReader(System.in));
        String line = null;
        while ((line = br.readLine()) != null)
        {
          outfile.write(line + "\n");
        }
        br.close();
        outfile.flush();
        outfile.close();

      } catch (Exception ex)
      {
        System.err.println("Failed to read from STDIN into tempfile "
                + ((sfile == null) ? "(tempfile wasn't created)" : sfile
                        .toString()));
        ex.printStackTrace();
        return;
      }
    }
    else
    {
      sfile = new File(groovyscript);
    }
    if (!sfile.exists())
    {
      System.err.println("File '" + groovyscript + "' does not exist.");
      return;
    }
    if (!sfile.canRead())
    {
      System.err.println("File '" + groovyscript + "' cannot be read.");
      return;
    }
    if (sfile.length() < 1)
    {
      System.err.println("File '" + groovyscript + "' is empty.");
      return;
    }
    boolean success = false;
    try
    {
      /*
       * The following code performs the GroovyScriptEngine invocation using
       * reflection, and is equivalent to this fragment from the embedding
       * groovy documentation on the groovy site: <code> import
       * groovy.lang.Binding; import groovy.util.GroovyScriptEngine;
       * 
       * String[] roots = new String[] { "/my/groovy/script/path" };
       * GroovyScriptEngine gse = new GroovyScriptEngine(roots); Binding binding =
       * new Binding(); binding.setVariable("input", "world");
       * gse.run("hello.groovy", binding); </code>
       */
      ClassLoader cl = jalviewContext.getClass().getClassLoader();
      Class gbindingc = cl.loadClass("groovy.lang.Binding");
      Constructor gbcons = gbindingc.getConstructor(null);
      Object gbinding = gbcons.newInstance(null);
      java.lang.reflect.Method setvar = gbindingc.getMethod("setVariable",
              new Class[]
              { String.class, Object.class });
      setvar.invoke(gbinding, new Object[]
      { "Jalview", jalviewContext });
      Class gsec = cl.loadClass("groovy.util.GroovyScriptEngine");
      Constructor gseccons = gsec.getConstructor(new Class[]
      { URL[].class }); // String[].class });
      Object gse = gseccons.newInstance(new Object[]
      { new URL[]
      { sfile.toURL() } }); // .toString() } });
      java.lang.reflect.Method run = gsec.getMethod("run", new Class[]
      { String.class, gbindingc });
      run.invoke(gse, new Object[]
      { sfile.getName(), gbinding });
      success = true;
    } catch (Exception e)
    {
      System.err.println("Exception Whilst trying to execute file " + sfile
              + " as a groovy script.");
      e.printStackTrace(System.err);

    }
    if (success && groovyscript.equals("STDIN"))
    {
      // delete temp file that we made - but only if it was successfully
      // executed
      sfile.delete();
    }
  }

  /**
   * Check commandline for any das server definitions or any fetchfrom switches
   * 
   * @return vector of DAS source nicknames to retrieve from
   */
  private static Vector checkDasArguments(ArgsParser aparser)
  {
    Vector source = null;
    String data;
    String locsources = Cache.getProperty(Cache.DAS_LOCAL_SOURCE);
    while ((data = aparser.getValue("dasserver")) != null)
    {
      String nickname = null;
      String url = null;
      int pos = data.indexOf('=');
      if (pos > 0)
      {
        nickname = data.substring(0, pos);
      }
      url = data.substring(pos + 1);
      if (url != null && url.startsWith("http:"))
      {
        if (nickname == null)
        {
          nickname = url;
        }
        if (locsources == null)
        {
          locsources = "";
        }
        else
        {
          locsources += "\t";
        }
        locsources = locsources + nickname + "|" + url;
        System.err
                .println("NOTE! dasserver parameter not yet really supported (got args of "
                        + nickname + "|" + url);
        if (source == null)
        {
          source = new Vector();
        }
        source.addElement(nickname);
      }
    } // loop until no more server entries are found.
    if (locsources != null && locsources.indexOf('|') > -1)
    {
      Cache.log.debug("Setting local source list in properties file to:\n"
              + locsources);
      Cache.setProperty(Cache.DAS_LOCAL_SOURCE, locsources);
    }
    while ((data = aparser.getValue("fetchfrom")) != null)
    {
      System.out.println("adding source '" + data + "'");
      if (source == null)
      {
        source = new Vector();
      }
      source.addElement(data);
    }
    return source;
  }

  /**
   * start a feature fetcher for every alignment frame
   * 
   * @param dasSources
   */
  private static void startFeatureFetching(final Vector dasSources)
  {
    AlignFrame afs[] = Desktop.getAlignframes();
    if (afs == null || afs.length == 0)
    {
      return;
    }
    for (int i = 0; i < afs.length; i++)
    {
      final AlignFrame af = afs[i];
      SwingUtilities.invokeLater(new Runnable()
      {

        public void run()
        {
          af.featureSettings_actionPerformed(null);
          af.featureSettings.fetchDasFeatures(dasSources);
        }
      });
    }
  }

  private static String checkProtocol(String file)
  {
    String protocol = jalview.io.FormatAdapter.FILE;

    if (file.indexOf("http:") > -1 || file.indexOf("file:") > -1)
    {
      protocol = jalview.io.FormatAdapter.URL;
    }
    return protocol;
  }
}

/**
 * Notes: this argParser does not distinguish between parameter switches,
 * parameter values and argument text. If an argument happens to be identical to
 * a parameter, it will be taken as such (even though it didn't have a '-'
 * prefixing it).
 * 
 * @author Andrew Waterhouse and JBP documented.
 * 
 */
class ArgsParser
{
  Vector vargs = null;

  public ArgsParser(String[] args)
  {
    vargs = new Vector();
    for (int i = 0; i < args.length; i++)
    {
      String arg = args[i].trim();
      if (arg.charAt(0) == '-')
      {
        arg = arg.substring(1);
      }
      vargs.addElement(arg);
    }
  }

  /**
   * check for and remove first occurence of arg+parameter in arglist.
   * 
   * @param arg
   * @return return the argument following the given arg if arg was in list.
   */
  public String getValue(String arg)
  {
    int index = vargs.indexOf(arg);
    String ret = null;
    if (index != -1)
    {
      ret = vargs.elementAt(index + 1).toString();
      vargs.removeElementAt(index);
      vargs.removeElementAt(index);
    }
    return ret;
  }

  /**
   * check for and remove first occurence of arg in arglist.
   * 
   * @param arg
   * @return true if arg was present in argslist.
   */
  public boolean contains(String arg)
  {
    if (vargs.contains(arg))
    {
      vargs.removeElement(arg);
      return true;
    }
    else
    {
      return false;
    }
  }

  public String nextValue()
  {
    return vargs.remove(0).toString();
  }

  public int getSize()
  {
    return vargs.size();
  }

}
