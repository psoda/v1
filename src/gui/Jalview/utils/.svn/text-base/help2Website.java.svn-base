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
import java.io.*;
import java.util.*;

public class help2Website
{

	public static void main(String [] args)
	{
		String line = "";
		try{
			Hashtable targets = new Hashtable();

			File toc = new File("helpTOC.xml");
			File jhm = new File("help.jhm");

			BufferedReader in = new BufferedReader(new FileReader(jhm));

			PrintWriter out = new PrintWriter(new FileWriter("helpTOC.html"));
			out.println("<html><head><title>Jalview - Help </title></head>\n"
			+"<body bgcolor=#F1F1F1>\n"
			+"<p><center><strong>Contents</strong></center></p>\n");


			StringTokenizer st;
			StringBuffer indent = new StringBuffer();
			String target, url, text;
			while( (line = in.readLine()) != null)
			{
				if(line.indexOf("target")==-1)
					continue;


				st = new StringTokenizer(line, "\"");
				st.nextToken(); //<mapID target="

				target = st.nextToken();
				st.nextToken(); //" url="

				url = st.nextToken();
				targets.put(target, url);
			}

			in = new BufferedReader(new FileReader(toc));
			while( (line = in.readLine()) != null)
			{
				if(line.indexOf("</tocitem>")!=-1)
					indent.setLength(indent.length()-18);

				if(line.indexOf("<tocitem")==-1)
					continue;

				st = new StringTokenizer(line, "\"");
				st.nextToken();

				text = st.nextToken();
				st.nextToken();

				target = st.nextToken();

				if(targets.get(target)!=null)
					out.println("<br>"+indent+"<a href=\""
							+ targets.get(target)
							+"\" target=bodyframe>"
							+text
							+"</a>");
				else
					out.println("<br>"+indent+text);


				if(line.indexOf("/>")==-1)
					indent.append("&nbsp;&nbsp;&nbsp;");

			}

			out.println("</body>\n</html>");


			out.close();

		}

		catch(Exception ex)
		{

			ex.printStackTrace();

			System.out.println("\n"+line+"\n");

			System.out.println("Usage: move to Help directory. help2Website will read"
			+"\nhelpTOC.xml and help.jhm producing output helpTOC.html");
		}
	}


}
