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
package jalview.datamodel;

/**
 * Defines internal constants for unambiguous annotation of DbRefEntry source
 * strings and describing the data retrieved from external database sources (see
 * jalview.ws.DbSourcProxy)
 * 
 * @author JimP
 * 
 */
public class DBRefSource
{
  /**
   * UNIPROT Accession Number
   */
  public static String UNIPROT = "UNIPROT";

  /**
   * UNIPROT Entry Name
   */
  public static String UP_NAME = "UNIPROT_NAME";

  /**
   * Uniprot Knowledgebase/TrEMBL as served from EMBL protein products.
   */
  public static final String UNIPROTKB = "UniProtKB/TrEMBL";

  /**
   * PDB Entry Code
   */
  public static String PDB = "PDB";

  /**
   * EMBL ID
   */
  public static String EMBL = "EMBL";

  /**
   * EMBLCDS ID
   */
  public static String EMBLCDS = "EMBLCDS";

  /**
   * PFAM ID
   */
  public static String PFAM = "PFAM";

  /**
   * GeneDB ID
   */
  public static final String GENEDB = "GeneDB";

  /**
   * List of databases whose sequences might have coding regions annotated
   */
  public static final String[] DNACODINGDBS =
  { EMBL, EMBLCDS, GENEDB };

  public static final String[] CODINGDBS =
  { EMBLCDS, GENEDB };

  public static final String[] PROTEINDBS =
  { UNIPROT, PDB, UNIPROTKB };

  public static final String[] PROTEINSEQ =
  { UNIPROT, UNIPROTKB };

  public static final String[] PROTEINSTR =
  { PDB };

  public static final String[] DOMAINDBS =
  { PFAM };

  /**
   * set of unique DBRefSource property constants. These could be used to
   * reconstruct the above groupings
   */
  public static final Object SEQDB = "SQ";

  /**
   * database of nucleic acid sequences
   */
  public static final Object DNASEQDB = "NASQ";

  /**
   * database of amino acid sequences
   */
  public static final Object PROTSEQDB = "PROTSQ";

  /**
   * database of cDNA sequences
   */
  public static final Object CODINGSEQDB = "CODING";

  /**
   * database of na sequences with exon annotation
   */
  public static final Object DNACODINGSEQDB = "XONCODING";

  /**
   * DB returns several sequences associated with a protein domain
   */
  public static final Object DOMAINDB = "DOMAIN";

  /**
   * DB query can take multiple accession codes concatenated by a separator.
   * Value of property indicates maximum number of accession codes to send at a
   * time.
   */
  public static final Object MULTIACC = "MULTIACC";
}
