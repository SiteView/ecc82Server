package site;

import java.io.IOException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.apache.commons.httpclient.*;
import org.apache.commons.httpclient.cookie.CookiePolicy;
import org.apache.commons.httpclient.cookie.CookieSpec;
import org.apache.commons.httpclient.methods.GetMethod;
import org.apache.commons.httpclient.methods.PostMethod;
import org.apache.commons.httpclient.params.HttpMethodParams;

public class HttpClinetBean {
	public static HttpClient httpClient ;
	public        PostMethod post_method ;
	public        GetMethod  getMethod ;
	
    public HttpClinetBean() { 	
    	httpClient = new HttpClient();
    }
    
    public HttpClinetBean(String HOST,int PORT,String HTTP) { 	
		httpClient = new HttpClient();
		httpClient.getHostConfiguration().setHost(HOST, PORT, HTTP);
		httpClient.getParams().setCookiePolicy(CookiePolicy.BROWSER_COMPATIBILITY);
 
    }
    
    public String getHTML(String URL){
		  //创建GET方法的实例
    	  getMethod = new GetMethod(URL);
		  //使用系统提供的默认的恢复策略
		  getMethod.getParams().setParameter(HttpMethodParams.RETRY_HANDLER,
		    new DefaultHttpMethodRetryHandler());
		  try {
		   //执行getMethod
		   int statusCode = httpClient.executeMethod(getMethod);
		   if (statusCode != HttpStatus.SC_OK) {
		    System.err.println("Method failed: "
		      + getMethod.getStatusLine());
		   }
		   //读取内容 
		   String responseBody = getMethod.getResponseBodyAsString();
		   
		   if (responseBody.indexOf("META HTTP-EQUIV=\"Refresh\"")!= -1){
			   URL ="/projects";
			   responseBody=this.getHTML(URL);
		   }
		   return responseBody;
		  } catch (HttpException e) {
		   //发生致命的异常，可能是协议不对或者返回的内容有问题
		   System.out.println("Please check your provided http address!");
		   e.printStackTrace();
		  } catch (IOException e) {
		   //发生网络异常
		   e.printStackTrace();
		  } finally {
		   //释放连接
		   getMethod.releaseConnection();	   
		  }
		  return "";
    }
    
    public String Login(List list,String URL){
    	String name;
    	String value;
    	String location = ""; 
    	 	
        post_method = new PostMethod(URL);
        post_method.getParams().setParameter(HttpMethodParams.RETRY_HANDLER,
				    new DefaultHttpMethodRetryHandler());
        post_method.getParams().setCookiePolicy(CookiePolicy.BROWSER_COMPATIBILITY);
		//post_method.setRequestHeader("Content-Length", "75");
        //post_method.setRequestHeader("Pragma", "No-Cache");
       //post_method.setRequestHeader("Referer","http://220.249.96.131/projects/ecc/login");
       // post_method.setRequestHeader("Accept", "*/*");
       // post_method.setRequestHeader("User-Agent","Mozilla/4.0 (compatible; MSIE 4.01; Windows NT)");        
        
        for (int i=0;i<list.size();i++){
        	name = "";
        	value = "";
        	String team = (String)list.get(i);
        	String[] teams = team.split("=");
        	if ( teams[0] != null )  name = teams[0].substring(teams[0].indexOf("}")+1);
        	if ( teams.length > 1)  {
        		if (teams[1] != null) {
	        		value = teams[1];
        		}
        	}
        	if (name == null ) name = "";
        	if (value == null ) value = "";
        	post_method.addParameter(name,value);
         }
        
        

       /* NameValuePair[] data ={ 
        		new NameValuePair("__FORM_TOKEN","0e22d68990daebb1b4a4672d"),
        		new NameValuePair("referer",""),
        		new NameValuePair("user","bin.liu"), 
        		new NameValuePair("password","binliu")
        };
        
        post_method.setRequestBody(data);*/
        post_method.setFollowRedirects(false);
        try {

        	int statusCode = httpClient.executeMethod(post_method);
            
        	
            if (statusCode == HttpStatus.SC_MOVED_PERMANENTLY || 
            		statusCode == HttpStatus.SC_MOVED_TEMPORARILY ||
            		statusCode == HttpStatus.SC_SEE_OTHER
            		) { 
            	Header locationHeader = post_method.getResponseHeader("location");  
            	if (locationHeader != null) {   
            		location = locationHeader.getValue();   
            		System.out.println("The page was redirected to:" + location);  
            	} 
            	else {   
            		System.err.println("Location field value is null.");    
            	}
            }else if(statusCode == HttpStatus.SC_OK) {
            	 System.out.println(post_method.getResponseBodyAsString());
            }
            else {
            	 System.err.println("Method failed: " + post_method.getStatusLine());
            }
            
          /*  CookieSpec cookiespec = CookiePolicy.getDefaultSpec();

            Cookie[] cookies = cookiespec.match(location, 80, "/", false, httpClient.getState().getCookies());

           if (cookies.length == 0) {

               System.out.println("None");    

           } else {

               for (int i = 0; i < cookies.length; i++) {

                   System.out.println(cookies[i].toString());    

               }

           }*/
            
            return location;
            
        } catch (HttpException e) {
            System.err.println("Fatal protocol violation: " + e.getMessage());
            e.printStackTrace();
        } catch (IOException e) {
            System.err.println("Fatal transport error: " + e.getMessage());
            e.printStackTrace();
        } finally {
            // Release the connection.
        	post_method.releaseConnection();
        }

    	return "";
    }    
    
}
