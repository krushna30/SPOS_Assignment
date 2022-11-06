import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Iterator;
import java.util.LinkedHashMap;

public class Pass1 {

	public static void main(String[] args) throws IOException{
		BufferedReader buff_reader=new BufferedReader(new FileReader("input.asm"));
		
		FileWriter Macro_name_table=new FileWriter("Macro_name_table.txt");
		FileWriter Macro_def_table=new FileWriter("Macro_def_table.txt");
		FileWriter keyw_paradt=new FileWriter("keyw_paradt.txt");
		FileWriter pnt=new FileWriter("para_name_table.txt");
		FileWriter inter_writer=new FileWriter("intermediate.txt");
		LinkedHashMap<String, Integer> para_name_table=new LinkedHashMap<>();
		String line;
		String Macroname = null;
		int Macro_def_tablep=1,keyw_paradtp=0,paramNo=1,posi_para=0,keyw_para=0,check=0;
		while((line=buff_reader.readLine())!=null)
		{
			
			String subparts[]=line.split("\\s+");
			if(subparts[0].equalsIgnoreCase("MACRO"))
			{
				check=1;
				line=buff_reader.readLine();
				subparts=line.split("\\s+");
				Macroname=subparts[0];
				if(subparts.length<=1)
				{
					Macro_name_table.write(subparts[0]+"\t"+posi_para+"\t"+keyw_para+"\t"+Macro_def_tablep+"\t"+(keyw_para==0?keyw_paradtp:(keyw_paradtp+1))+"\n");
					continue;
				}
				for(int i=1;i<subparts.length;i++) //processing of parameters
				{
					subparts[i]=subparts[i].replaceAll("[&,]", "");
					//System.out.println(subparts[i]);
					if(subparts[i].contains("="))
					{
						++keyw_para;
						String keywordParam[]=subparts[i].split("=");
						para_name_table.put(keywordParam[0], paramNo++);
						if(keywordParam.length==2)
						{
							keyw_paradt.write(keywordParam[0]+"\t"+keywordParam[1]+"\n");
						}
						else
						{
							keyw_paradt.write(keywordParam[0]+"\t-\n");
						}
					}
					else
					{
						para_name_table.put(subparts[i], paramNo++);
						posi_para++;
					}
				}
				Macro_name_table.write(subparts[0]+"\t"+posi_para+"\t"+keyw_para+"\t"+Macro_def_tablep+"\t"+(keyw_para==0?keyw_paradtp:(keyw_paradtp+1))+"\n");
				keyw_paradtp=keyw_paradtp+keyw_para;
			
				
				
			}
			else if(subparts[0].equalsIgnoreCase("MEND"))
			{
				Macro_def_table.write(line+"\n");
				check=keyw_para=posi_para=0;
				Macro_def_tablep++;
				paramNo=1;
				pnt.write(Macroname+":\t");
				Iterator<String> it=para_name_table.keySet().iterator();
				while(it.hasNext())
				{
					pnt.write(it.next()+"\t");
				}
				pnt.write("\n");
				para_name_table.clear();
			}
			else if(check==1)
			{
				for(int i=0;i<subparts.length;i++)
				{
					if(subparts[i].contains("&"))
					{
						subparts[i]=subparts[i].replaceAll("[&,]", "");
						Macro_def_table.write("(P,"+para_name_table.get(subparts[i])+")\t");
					}
					else
					{
						Macro_def_table.write(subparts[i]+"\t");
					}
				}
				Macro_def_table.write("\n");
				Macro_def_tablep++;
			}
			else
			{
				inter_writer.write(line+"\n");
			}
		}
		buff_reader.close();
		Macro_def_table.close();
		Macro_name_table.close();
		inter_writer.close();
		pnt.close();
		keyw_paradt.close();
		System.out.println("Macro Pass 1 is completed !");
	}

}