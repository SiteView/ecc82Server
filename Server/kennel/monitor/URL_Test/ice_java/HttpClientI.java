import java.io.IOException;

import org.apache.commons.httpclient.DefaultHttpMethodRetryHandler;
import org.apache.commons.httpclient.Header;
import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.HttpException;
import org.apache.commons.httpclient.HttpStatus;
import org.apache.commons.httpclient.cookie.CookiePolicy;
import org.apache.commons.httpclient.methods.GetMethod;
import org.apache.commons.httpclient.methods.PostMethod;
import org.apache.commons.httpclient.params.HttpMethodParams;

import Ice.Current;
import site.*;

/*URL���̼����,(���԰�)
 * ��ICEʵ��
 * ����
*/
public class HttpClientI extends _HttpClinetBeanDisp {	
	public static HttpClient httpClient;
	public GetMethod getMethod;
	public PostMethod post_method;
	
	//�õ�����URL��ַ��ҳ��HTML�ı�
	public String getHTML(String URL, Current __current) {
		  //����GET������ʵ��
		//System.out.println(URL);
		 getMethod = new GetMethod(URL);
		  //ʹ��ϵͳ�ṩ��Ĭ�ϵĻָ�����
		  getMethod.getParams().setParameter(HttpMethodParams.RETRY_HANDLER,
		    new DefaultHttpMethodRetryHandler());
		  try {
		   //ִ��getMethod
		   int statusCode = httpClient.executeMethod(getMethod);
		   if (statusCode != HttpStatus.SC_OK) {
		    System.err.println("Method failed: "
		      + getMethod.getStatusLine());
		   }
		   //��ȡ���� 
		   String responseBody = getMethod.getResponseBodyAsString();
		   
		   //��ʱ��ҳ���ı���
		   //META HTTP-EQUIV="Refresh" URL="/projects"
		   //����һ����ת���ͷ�����Ա������жϣ�������Ӧ��·��
		   if (responseBody.indexOf("META HTTP-EQUIV=\"Refresh\"")!= -1){
			   URL ="/projects";
			   responseBody=this.getHTML(URL);
		   }
		   return responseBody;
		  } catch (HttpException e) {
		   //�����������쳣��������Э�鲻�Ի��߷��ص�����������
		   System.out.println("Please check your provided http address!");
		   e.printStackTrace();
		  } catch (IOException e) {
		   //���������쳣
		   e.printStackTrace();
		  } finally {
		   //�ͷ�����
		   getMethod.releaseConnection();	   
		  }
		  return "";
	}

	//��ҳ��½���� Ϊpost�ύ
	//listΪ����ֵ ��ʽΪ{[1]}name=value
	//���ص�½��ҳ��Ҫ������·��
	public String login(String[] list, String URL, Current __current) {
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
        
        for (int i=0;i<list.length;i++){
        	name = "";
        	value = "";
        	String team = (String)list[i];
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

	public void deluetInit(Current __current) {
		// TODO Auto-generated method stub
		httpClient = new HttpClient();
	}

	//��ʼ��
	//httpClient����Ϊ��̬������SESSION��IDΪͬһ��
	//��Ϊ���ID��ͬ�ͻ��¼����
	//������һ��������һֱ��ͬһ��httpClient
	public void init(String url, int port, String http, Current __current) {
		// TODO Auto-generated method stub
		httpClient = new HttpClient();
		httpClient.getHostConfiguration().setHost(url, port, http);
		httpClient.getParams().setCookiePolicy(CookiePolicy.BROWSER_COMPATIBILITY);
	}
}
