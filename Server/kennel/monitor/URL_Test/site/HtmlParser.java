package site;

import java.util.ArrayList;
import java.util.List;

import org.htmlparser.NodeFilter;
import org.htmlparser.Parser;
import org.htmlparser.filters.TagNameFilter;
import org.htmlparser.tags.FormTag;
import org.htmlparser.tags.InputTag;
import org.htmlparser.tags.LinkTag;
import org.htmlparser.util.NodeList;
import org.htmlparser.util.ParserException;

public class HtmlParser {
	//�õ�����<a>��ǵļ���
    public List getLink(String HTML){
    	List list = new ArrayList();
        Parser parser;   
        parser = Parser.createParser(HTML,"GBK");   
                 
        String filterStr = "a";   
        NodeFilter filter = new TagNameFilter(filterStr);   
           
        try {
			NodeList links = parser.extractAllNodesThatMatch(filter);
			for(int i=0;i<links.size();i++){
		    	String ALinks="";
				LinkTag linkTag =(LinkTag)links.elementAt(i);
				ALinks+=(linkTag.getChildrenHTML()+"$$$"+linkTag.getLink());
				list.add(ALinks);
			    //System.out.println(linkTag.getLink());
			    //System.out.println(linkTag.getChildrenHTML());
			}
			return list;
		} catch (ParserException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return null;
		}   
    }
    
    //ȡ����form��submit������,form��ation����ֵ����
    public List getForm(String HTML){
    	List list = new ArrayList();
        Parser parser;   
        parser = Parser.createParser(HTML,"GBK");   
                 
        String filterStr = "form";   
        NodeFilter filter = new TagNameFilter(filterStr);   
           
        try {
			NodeList forms = parser.extractAllNodesThatMatch(filter);
			for(int i=0;i<forms.size();i++){
		    	String ALinks="";
				FormTag formTag =(FormTag)forms.elementAt(i);
				//System.out.println(formTag.toHtml());
				ALinks+=("{["+(i+1)+"]}"+this.getFormName(formTag.toHtml().toString())+"$$$"+formTag.getFormLocation());
				list.add(ALinks);
			}
			return list;
		} catch (ParserException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return null;
		}   
    }
    
    //ȡ����form��submit������
    public String getFormName(String HTML){
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
    
    //ȡnumber����ָ����form��input������,ֵ����
    public List getFormInput(String HTML,int number){
    	List list = new ArrayList();
        Parser parser;   
        parser = Parser.createParser(HTML,"GBK");   
                 
        String filterStr = "form";   
        NodeFilter filter = new TagNameFilter(filterStr);   
           
        try {
			NodeList forms = parser.extractAllNodesThatMatch(filter);
			for(int i=0;i<forms.size();i++){
				if (i == number-1 ){
				    FormTag formTag =(FormTag)forms.elementAt(i);
                    list = this.getFormInputValue(formTag.toHtml().toString(),number);
				}
			}
			return list;
		} catch (ParserException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return null;
		}   
    }
    
    //ȡ����form��input������,ֵ
    public List getFormInputValue(String HTML,int number){
    	List list = new ArrayList();
        Parser parser;   
        parser = Parser.createParser(HTML,"GBK");   
                 
        String filterStr = "input";   
        NodeFilter filter = new TagNameFilter(filterStr);   
		
        try {
			NodeList inputs = parser.extractAllNodesThatMatch(filter);
			for(int i=0;i<inputs.size();i++){
		        String name = "";
				String value = "";
				InputTag inputTag =(InputTag)inputs.elementAt(i);			
				String text = inputTag.getText().toString();

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
					//System.out.println("{["+number+"]}"+name+"="+value);
					list.add("{["+number+"]}"+name+"="+value);
					
				}
			}
			return list;
		} catch (ParserException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return null;
		}   
    }

}
