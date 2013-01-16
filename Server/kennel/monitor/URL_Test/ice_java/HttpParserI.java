import org.htmlparser.NodeFilter;
import org.htmlparser.Parser;
import org.htmlparser.filters.TagNameFilter;
import org.htmlparser.tags.FormTag;
import org.htmlparser.tags.InputTag;
import org.htmlparser.tags.LinkTag;
import org.htmlparser.util.NodeList;
import org.htmlparser.util.ParserException;

import Ice.Current;
import site.*;

/*HTML文本解析类
 * 用ICE实现
 * 刘斌
 */

public class HttpParserI extends _HttpParserDisp{

	//得到文本中相应的FORM
	//返回{[1]}form$$$action
	//其中{[1]}为自定义的格式，有多个FORM时，会有{[2]} {[3]}
	//form为文本中<input type="submit" name="login">的login名称
	//action为文本中form的action
	public String[] getForm(String HTML, Current __current) {
		
        Parser parser;   
        parser = Parser.createParser(HTML,"GBK");   
                 
        String filterStr = "form";   
        NodeFilter filter = new TagNameFilter(filterStr);   
           
        try {
			NodeList forms = parser.extractAllNodesThatMatch(filter);
			String[] lists = new String[forms.size()];
			System.out.println("forms------------------------------------");
			for(int i=0;i<forms.size();i++){
		    	String ALinks="";
				FormTag formTag =(FormTag)forms.elementAt(i);
				ALinks+=("{["+(i+1)+"]}"+this.getFormName(formTag.toHtml().toString())+"$$$"+formTag.getFormLocation());
				System.out.println(ALinks);
				lists[i] = ALinks;
			}
			return lists;
		} catch (ParserException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return null;
		}   
	}

	//得到文本中相应form中的INPUT标记数组
	//number为指定的form，只有一个form时为1
	public String[] getFormInput(String HTML, int number, Current __current) {
		
        Parser parser;   
        parser = Parser.createParser(HTML,"GBK");   
                 
        String filterStr = "form";   
        NodeFilter filter = new TagNameFilter(filterStr);   
           
        try {
			NodeList forms = parser.extractAllNodesThatMatch(filter);
			String[] lists = new String[forms.size()];
			System.out.println("inputs---------------------------------------");
			for(int i=0;i<forms.size();i++){
				if (i == number-1 ){
				    FormTag formTag =(FormTag)forms.elementAt(i);
				    lists = this.getFormInputValue(formTag.toHtml().toString(),number);
				}
			}
			return lists;
		} catch (ParserException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return null;
		}   
	}
	
	//得到文本中相应form中的INPUT标记数组
	//number为指定的form，只有一个form时为1
	//返回{[1]}name=value
	//{[1]}表示为第一个FORM
	//name表示为input标记的名称
	//value表示为input标记的值
	public String[] getFormInputValue(String HTML, int number, Current __current) {

        Parser parser;   
        parser = Parser.createParser(HTML,"GBK");   
                 
        String filterStr = "input";   
        NodeFilter filter = new TagNameFilter(filterStr);   
		
        try {
			NodeList inputs = parser.extractAllNodesThatMatch(filter);
			String[] lists = new String[inputs.size()];
			for(int i=0;i<inputs.size();i++){
		        String name = "";
				String value = "";
				InputTag inputTag =(InputTag)inputs.elementAt(i);			
				String text = inputTag.getText().toString();
                
				//过滤掉类型为submit的标记<input type="submit" name="" value=""/>
				if (text.indexOf("submit") == -1){
					if (text.indexOf("value") >=0) {
						String valueText = text.substring(text.indexOf("value"));
                        String[] values = valueText.split(" ");
						if (values[0] != null && values[0].length()>0){
						    value = values[0].substring(values[0].indexOf("=")+1);
						    value = value.replaceAll("\"", "");
						    value = value.replaceAll("'", "");
						    value = value.trim();
						}
					}
					
					if (text.indexOf("name") >=0) {
						String nameText = text.substring(text.indexOf("name"));
						 String[] names = nameText.split(" ");
						if (names[0] != null && names[0].length()>0){
						    name = names[0].substring(names[0].indexOf("=")+1);
						    name = name.replaceAll("\"", "");
						    name = name.replaceAll("'", "");
						    name = name.trim();
						    
						}
					}
					System.out.println("{["+number+"]}"+name+"="+value);
					lists[i] = "{["+number+"]}"+name+"="+value;
					
				}
			}
			return lists;
		} catch (ParserException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return null;
		}   
	}

	//返回为文本中<input type="submit" name="login">的login名称
	public String getFormName(String HTML, Current __current) {
        Parser parser;   
        parser = Parser.createParser(HTML,"GBK");   
                 
        String filterStr = "input";
        NodeFilter filter = new TagNameFilter(filterStr);   
           
        try {
			NodeList inputs = parser.extractAllNodesThatMatch(filter);
			for(int i=0;i<inputs.size();i++){
		        String value = "";
				InputTag inputTag =(InputTag)inputs.elementAt(i);			
				String text = inputTag.getText().toString();
				if (text.indexOf("submit") >= 0){
					if (text.indexOf("value") >=0) {
						String valueText = text.substring(text.indexOf("value"));
                        String[] values = valueText.split(" ");
						if (values[0] != null && values[0].length()>0){
						    value = values[0].substring(values[0].indexOf("=")+1);
						    value = value.replaceAll("\"", "");
						    value = value.replaceAll("'", "");
						    value = value.trim();
						}
					}
					return value;
				}

				//System.out.println(text.indexOf("submit"));
			}
			return "";
		} catch (ParserException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return "";
		}   
	}

	//得到文本中所有的<a>标记
	//返回name$$$href
	//name为<a>标记的文本
	//href为<a>标记的href
	public String[] getLink(String HTML, Current __current) {
		//System.out.println(HTML);		
        Parser parser;   
        parser = Parser.createParser(HTML,"GBK");   
                 
        String filterStr = "a";   
        NodeFilter filter = new TagNameFilter(filterStr);   
           
        try {
			NodeList links = parser.extractAllNodesThatMatch(filter);
			String[] lists = new String[links.size()];
			System.out.println("links-----------------------------------");
			for(int i=0;i<links.size();i++){
		    	String ALinks="";
				LinkTag linkTag =(LinkTag)links.elementAt(i);
				ALinks+=(linkTag.getChildrenHTML()+"$$$"+linkTag.getLink());
				System.out.println(ALinks);
				lists[i] = ALinks;

			}
			return lists;
		} catch (ParserException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return null;
		}  
	}

}
