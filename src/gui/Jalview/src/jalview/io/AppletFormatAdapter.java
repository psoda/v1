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
package jalview.io;

import java.io.File;

import jalview.datamodel.*;

/**
 * A low level class for alignment and feature IO with alignment formatting
 * methods used by both applet and application for generating flat alignment
 * files. It also holds the lists of magic format names that the applet and
 * application will allow the user to read or write files with.
 * 
 * @author $author$
 * @version $Revision: 1.22.2.2 $
 */
public class AppletFormatAdapter
{
  /**
   * List of valid format strings used in the isValidFormat method
   */
  public static final String[] READABLE_FORMATS = new String[]
  { "BLC", "CLUSTAL", "FASTA", "MSF", "PileUp", "PIR", "PFAM", "STH",
      "PDB", "JnetFile" };

  /**
   * List of valid format strings for use by callers of the formatSequences
   * method
   */
  public static final String[] WRITEABLE_FORMATS = new String[]
  { "BLC", "CLUSTAL", "FASTA", "MSF", "PileUp", "PIR", "PFAM", "AMSA" };

  /**
   * List of extensions corresponding to file format types in WRITABLE_FNAMES
   * that are writable by the application.
   */
  public static final String[] WRITABLE_EXTENSIONS = new String[]
  { "fa, fasta, fastq", "aln", "pfam", "msf", "pir", "blc", "amsa", "jar" };

  /**
   * List of writable formats by the application. Order must correspond with the
   * WRITABLE_EXTENSIONS list of formats.
   */
  public static final String[] WRITABLE_FNAMES = new String[]
  { "Fasta", "Clustal", "PFAM", "MSF", "PIR", "BLC", "AMSA", "Jalview" };

  /**
   * List of readable format file extensions by application in order
   * corresponding to READABLE_FNAMES
   */
  public static final String[] READABLE_EXTENSIONS = new String[]
  { "fa, fasta, fastq", "aln", "pfam", "msf", "pir", "blc", "amsa", "jar" };

  /**
   * List of readable formats by application in order corresponding to
   * READABLE_EXTENSIONS
   */
  public static final String[] READABLE_FNAMES = new String[]
  { "Fasta", "Clustal", "PFAM", "MSF", "PIR", "BLC", "AMSA", "Jalview" };

  public static String INVALID_CHARACTERS = "Contains invalid characters";

  // TODO: make these messages dynamic
  public static String SUPPORTED_FORMATS = "Formats currently supported are\n"
          + prettyPrint(READABLE_FORMATS);

  /**
   * 
   * @param els
   * @return grammatically correct(ish) list consisting of els elements.
   */
  public static String prettyPrint(String[] els)
  {
    StringBuffer list = new StringBuffer();
    for (int i = 0, iSize = els.length - 1; i < iSize; i++)
    {
      list.append(els[i]);
      list.append(",");
    }
    list.append(" and " + els[els.length - 1] + ".");
    return list.toString();
  }

  public static String FILE = "File";

  public static String URL = "URL";

  public static String PASTE = "Paste";

  public static String CLASSLOADER = "ClassLoader";

  AlignFile afile = null;

  String inFile;

  /**
   * check that this format is valid for reading
   * 
   * @param format
   *                a format string to be compared with READABLE_FORMATS
   * @return true if format is readable
   */
  public static final boolean isValidFormat(String format)
  {
    return isValidFormat(format, false);
  }

  /**
   * validate format is valid for IO
   * 
   * @param format
   *                a format string to be compared with either READABLE_FORMATS
   *                or WRITEABLE_FORMATS
   * @param forwriting
   *                when true, format is checked for containment in
   *                WRITEABLE_FORMATS
   * @return true if format is valid
   */
  public static final boolean isValidFormat(String format,
          boolean forwriting)
  {
    boolean valid = false;
    String[] format_list = (forwriting) ? WRITEABLE_FORMATS
            : READABLE_FORMATS;
    for (int i = 0; i < format_list.length; i++)
    {
      if (format_list[i].equalsIgnoreCase(format))
      {
        return true;
      }
    }

    return valid;
  }

  /**
   * Constructs the correct filetype parser for a characterised datasource
   * 
   * @param inFile
   *                data/data location
   * @param type
   *                type of datasource
   * @param format
   *                File format of data provided by datasource
   * 
   * @return DOCUMENT ME!
   */
  public Alignment readFile(String inFile, String type, String format)
          throws java.io.IOException
  {
    // TODO: generalise mapping between format string and io. class instances
    // using Constructor.invoke reflection
    this.inFile = inFile;
    try
    {
      if (format.equals("FASTA"))
      {
        afile = new FastaFile(inFile, type);
      }
      else if (format.equals("MSF"))
      {
        afile = new MSFfile(inFile, type);
      }
      else if (format.equals("PileUp"))
      {
        afile = new PileUpfile(inFile, type);
      }
      else if (format.equals("CLUSTAL"))
      {
        afile = new ClustalFile(inFile, type);
      }
      else if (format.equals("BLC"))
      {
        afile = new BLCFile(inFile, type);
      }
      else if (format.equals("PIR"))
      {
        afile = new PIRFile(inFile, type);
      }
      else if (format.equals("PFAM"))
      {
        afile = new PfamFile(inFile, type);
      }
      else if (format.equals("JnetFile"))
      {
        afile = new JPredFile(inFile, type);
        ((JPredFile) afile).removeNonSequences();
      }
      else if (format.equals("PDB"))
      {
        afile = new MCview.PDBfile(inFile, type);
      }
      else if (format.equals("STH"))
      {
        afile = new StockholmFile(inFile, type);
      }

      Alignment al = new Alignment(afile.getSeqsAsArray());

      afile.addAnnotations(al);

      return al;
    } catch (Exception e)
    {
      e.printStackTrace();
      System.err.println("Failed to read alignment using the '" + format
              + "' reader.\n" + e);

      if (e.getMessage() != null
              && e.getMessage().startsWith(INVALID_CHARACTERS))
      {
        throw new java.io.IOException(e.getMessage());
      }

      // Finally test if the user has pasted just the sequence, no id
      if (type.equalsIgnoreCase("Paste"))
      {
        try
        {
          // Possible sequence is just residues with no label
          afile = new FastaFile(">UNKNOWN\n" + inFile, "Paste");
          Alignment al = new Alignment(afile.getSeqsAsArray());
          afile.addAnnotations(al);
          return al;

        } catch (Exception ex)
        {
          if (ex.toString().startsWith(INVALID_CHARACTERS))
          {
            throw new java.io.IOException(e.getMessage());
          }

          ex.printStackTrace();
        }
      }

      // If we get to this stage, the format was not supported
      throw new java.io.IOException(SUPPORTED_FORMATS);
    }
  }

  /**
   * Constructs the correct filetype parser for an already open datasource
   * 
   * @param source
   *                an existing datasource
   * @param format
   *                File format of data that will be provided by datasource
   * 
   * @return DOCUMENT ME!
   */
  public Alignment readFromFile(FileParse source, String format)
          throws java.io.IOException
  {
    // TODO: generalise mapping between format string and io. class instances
    // using Constructor.invoke reflection
    // This is exactly the same as the readFile method except we substitute
    // 'inFile, type' with 'source'
    this.inFile = source.getInFile();
    String type = source.type;
    try
    {
      if (format.equals("FASTA"))
      {
        afile = new FastaFile(source);
      }
      else if (format.equals("MSF"))
      {
        afile = new MSFfile(source);
      }
      else if (format.equals("PileUp"))
      {
        afile = new PileUpfile(source);
      }
      else if (format.equals("CLUSTAL"))
      {
        afile = new ClustalFile(source);
      }
      else if (format.equals("BLC"))
      {
        afile = new BLCFile(source);
      }
      else if (format.equals("PIR"))
      {
        afile = new PIRFile(source);
      }
      else if (format.equals("PFAM"))
      {
        afile = new PfamFile(source);
      }
      else if (format.equals("JnetFile"))
      {
        afile = new JPredFile(source);
        ((JPredFile) afile).removeNonSequences();
      }
      else if (format.equals("PDB"))
      {
        afile = new MCview.PDBfile(source);
      }
      else if (format.equals("STH"))
      {
        afile = new StockholmFile(source);
      }

      Alignment al = new Alignment(afile.getSeqsAsArray());

      afile.addAnnotations(al);

      return al;
    } catch (Exception e)
    {
      e.printStackTrace();
      System.err.println("Failed to read alignment using the '" + format
              + "' reader.\n" + e);

      if (e.getMessage() != null
              && e.getMessage().startsWith(INVALID_CHARACTERS))
      {
        throw new java.io.IOException(e.getMessage());
      }

      // Finally test if the user has pasted just the sequence, no id
      if (type.equalsIgnoreCase("Paste"))
      {
        try
        {
          // Possible sequence is just residues with no label
          afile = new FastaFile(">UNKNOWN\n" + inFile, "Paste");
          Alignment al = new Alignment(afile.getSeqsAsArray());
          afile.addAnnotations(al);
          return al;

        } catch (Exception ex)
        {
          if (ex.toString().startsWith(INVALID_CHARACTERS))
          {
            throw new java.io.IOException(e.getMessage());
          }

          ex.printStackTrace();
        }
      }

      // If we get to this stage, the format was not supported
      throw new java.io.IOException(SUPPORTED_FORMATS);
    }
  }

  /**
   * Construct an output class for an alignment in a particular filetype
   * 
   * @param format
   *                string name of alignment format
   * @param alignment
   *                the alignment to be written out
   * @param jvsuffix
   *                passed to AlnFile class controls whether /START-END is added
   *                to sequence names
   * 
   * @return alignment flat file contents
   */
  public String formatSequences(String format, AlignmentI alignment,
          boolean jvsuffix)
  {
    try
    {
      AlignFile afile = null;

      if (format.equalsIgnoreCase("FASTA"))
      {
        afile = new FastaFile();
      }
      else if (format.equalsIgnoreCase("MSF"))
      {
        afile = new MSFfile();
      }
      else if (format.equalsIgnoreCase("PileUp"))
      {
        afile = new PileUpfile();
      }
      else if (format.equalsIgnoreCase("CLUSTAL"))
      {
        afile = new ClustalFile();
      }
      else if (format.equalsIgnoreCase("BLC"))
      {
        afile = new BLCFile();
      }
      else if (format.equalsIgnoreCase("PIR"))
      {
        afile = new PIRFile();
      }
      else if (format.equalsIgnoreCase("PFAM"))
      {
        afile = new PfamFile();
      }
      else if (format.equalsIgnoreCase("STH"))
      {
        afile = new StockholmFile();
      }
      else if (format.equalsIgnoreCase("AMSA"))
      {
        afile = new AMSAFile(alignment);
      }
      else
      {
        throw new Exception(
                "Implementation error: Unknown file format string");
      }

      afile.addJVSuffix(jvsuffix);

      afile.setSeqs(alignment.getSequencesArray());

      return afile.print();
    } catch (Exception e)
    {
      System.err.println("Failed to write alignment as a '" + format
              + "' file\n");
      e.printStackTrace();
    }

    return null;
  }

  public static void main(String[] args)
  {
    int i = 0;
    while (i < args.length)
    {
      File f = new File(args[i]);
      if (f.exists())
      {
        try
        {
          System.out.println("Reading file: " + f);
          AppletFormatAdapter afa = new AppletFormatAdapter();
          Runtime r = Runtime.getRuntime();
          System.gc();
          long memf = -r.totalMemory() + r.freeMemory();
          long t1 = -System.currentTimeMillis();
          Alignment al = afa.readFile(args[i], FILE, new IdentifyFile()
                  .Identify(args[i], FILE));
          t1 += System.currentTimeMillis();
          System.gc();
          memf += r.totalMemory() - r.freeMemory();
          if (al != null)
          {
            System.out.println("Alignment contains " + al.getHeight()
                    + " sequences and " + al.getWidth() + " columns.");
          }
          else
          {
            System.out.println("Couldn't read alignment");
          }
          System.out.println("Read took " + (t1 / 1000.0) + " seconds.");
          System.out
                  .println("Difference between free memory now and before is "
                          + (memf / (1024.0 * 1024.0) * 1.0) + " MB");

        } catch (Exception e)
        {
          System.err.println("Exception when dealing with " + i
                  + "'th argument: " + args[i] + "\n" + e);
        }
      }
      else
      {
        System.err.println("Ignoring argument '" + args[i] + "' (" + i
                + "'th)- not a readable file.");
      }
      i++;
    }

  }
}
