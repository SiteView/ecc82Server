package test;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;

import site.*;

public class HTTPClientTest {
	public static void main(String[] args) throws UnsupportedEncodingException {
		  List list = new ArrayList();
		  List URLList = new ArrayList();
		  List ValveList = new ArrayList();
		  List LoginList = new ArrayList();
          HtmlParser parser = new HtmlParser();
          HttpClinetBean httpbean = new HttpClinetBean("220.249.96.131",80,"http");
          String html = httpbean.getHTML("/");
          if (html != null && html.length() != 0) {
        	 list =  parser.getLink(html);
        	 for (int i=0;i<list.size();i++) {
        		 System.out.println(list.get(i).toString());
        	 }
        	 
        	 System.out.println("form login name =");
        	 URLList =  parser.getForm(html);
        	 for (int i=0;i<URLList.size();i++) {
        		 System.out.println(URLList.get(i).toString());
        	 }
        	 
        	 System.out.println("form input name value =");
        	 ValveList =  parser.getFormInput(html, 1);
        	 for (int i=0;i<ValveList.size();i++) {
        		 System.out.println(ValveList.get(i));
        	 }
          }
          
            
          System.out.println();
          System.out.println("第2步");
        //第2步
          html = "";
          list = null;
          URLList = null;
          ValveList = null;
          html = httpbean.getHTML("/projects/ecc");
          if (html != null && html.length() != 0) {
        	 list =  parser.getLink(html);
        	 for (int i=0;i<list.size();i++) {
        		 System.out.println(list.get(i));
        	 }
        	 
        	 System.out.println("form login name =");
        	 URLList =  parser.getForm(html);
        	 for (int i=0;i<URLList.size();i++) {
        		 System.out.println(URLList.get(i));
        	 }
        	 
        	 System.out.println("form input name value =");
        	 ValveList =  parser.getFormInput(html, 1);
        	 for (int i=0;i<ValveList.size();i++) {
        		 System.out.println(ValveList.get(i));
        	 }
          }
        
          
          
     
          System.out.println();
          System.out.println("第3步");
          //第3步。登陆
	      String URL="/projects/ecc/login";
	      LoginList.add("{[1]}__FORM_TOKEN="+ValveList.get(0).toString().substring(18));
	      LoginList.add("{[1]}referer=");
	      LoginList.add("{[1]}user=bin.liu");
	      LoginList.add("{[1]}password=binliu");
    	  String path = httpbean.Login(LoginList,URL);
          //System.out.println("path="+path);
          
          System.out.println();
          System.out.println("第4步");
    	  //第4步
    	  html = "";
          html = httpbean.getHTML(path);
          list = null;
          URLList = null;
          ValveList = null;
          if (html != null && html.length() != 0) {
        	 list =  parser.getLink(html);
        	 for (int i=0;i<list.size();i++) {
        		 System.out.println(list.get(i));
        	 }
        	 
        	 System.out.println("form login name =");
        	 URLList =  parser.getForm(html);
        	 for (int i=0;i<URLList.size();i++) {
        		 System.out.println(URLList.get(i));
        	 }
        	 
        	 System.out.println("form input name value =");
        	 ValveList =  parser.getFormInput(html, 1);
        	 for (int i=0;i<ValveList.size();i++) {
        		 System.out.println(ValveList.get(i));
        	 }
          }
          
          System.out.println();
          System.out.println("第5步");
    	  //第5步
    	  html = "";
          html = httpbean.getHTML("/projects/ecc");
          list = null;
          URLList = null;
          ValveList = null;
          if (html != null && html.length() != 0) {
        	 list =  parser.getLink(html);
        	 for (int i=0;i<list.size();i++) {
        		 System.out.println(list.get(i));
        	 }
        	 
        	 System.out.println("form login name =");
        	 URLList =  parser.getForm(html);
        	 for (int i=0;i<URLList.size();i++) {
        		 System.out.println(URLList.get(i));
        	 }
        	 
        	 System.out.println("form input name value =");
        	 ValveList =  parser.getFormInput(html, 1);
        	 for (int i=0;i<ValveList.size();i++) {
        		 System.out.println(ValveList.get(i));
        	 }
          }
        
	}
}
