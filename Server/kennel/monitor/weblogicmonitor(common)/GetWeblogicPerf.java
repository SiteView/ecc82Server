/*
 * GetWeblogicPerf.java
 *
 * Created on 2007��9��12��, ����10:24 By �պ�
 * Modified on 2007-09-18 By �պ�
 *     1�����Ӽ�������
 *     2��������ʾ�����̨ʵ�����������¼��Ӳ������ݵ�Bug
 *
 * ͨ��MBeanȡ��Weblogic��һЩ���ܲ���
 * ���и�ʽΪ��java GetWeblogicPerf.class �û��� ���� ������ �˿ں� ���������� ���������
 */

import java.io.IOException;
import java.net.MalformedURLException;
import java.util.Hashtable;

import javax.management.MBeanServerConnection;
import javax.management.MalformedObjectNameException;
import javax.management.ObjectName;
import javax.management.remote.JMXConnector;
import javax.management.remote.JMXConnectorFactory;
import javax.management.remote.JMXServiceURL;
import javax.naming.Context;

import weblogic.management.runtime.TransactionNameRuntimeMBean;

/**
 *
 * @author Su he
 */
public class GetWeblogicPerf 
{ 
  private static MBeanServerConnection connection;
  private static JMXConnector connector;
  private static ObjectName service;
  private final static String RUNTIME_URI = "weblogic.management.mbeanservers.runtime"; 
  private final static String DOMAINRUNTIME_URI = "weblogic.management.mbeanservers.domainruntime";
  private final static String RUNTIME_SERVICE = "com.bea:Name=RuntimeService,Type=weblogic.management.mbeanservers.runtime.RuntimeServiceMBean"; 
  private final static String DOMAINRUNTIME_SERVICE = "com.bea:Name=DomainRuntimeService,Type=weblogic.management.mbeanservers.domainruntime.DomainRuntimeServiceMBean";
  private final static String MBEAN_TYPE_SERVICE = "com.bea:Name=MBeanTypeService,Type=weblogic.management.mbeanservers.MBeanTypeService";
    
  // ʵ���� DomainRuntimeServiceMBean ������ 
  // ��������ͨ����ʹ�ô˶������� 
  private static ObjectName getService(String strservice) throws MalformedObjectNameException, NullPointerException{
      service = new ObjectName(strservice);
      return service;
  }
  
  /**
   * ʵ������ Domain Runtime MBean Server ������
   */
  public static void initConnection(String hostname, String portString,
          String username, String password,String uri)
          throws IOException, MalformedURLException {
    String protocol = "t3";
    int port = Integer.valueOf(portString);
    String jndiroot = "/jndi/";
    JMXServiceURL serviceURL = new JMXServiceURL(protocol, hostname, port, jndiroot + uri);
    Hashtable h = new Hashtable();
    h.put(Context.SECURITY_PRINCIPAL, username);
    h.put(Context.SECURITY_CREDENTIALS, password);
    h.put(JMXConnectorFactory.PROTOCOL_PROVIDER_PACKAGES, "weblogic.management.remote");
    connector = JMXConnectorFactory.connect(serviceURL, h);
    connection = connector.getMBeanServerConnection();
  }

  public static void die() 
  {
    System.err.println("Error=Input Error!$");
    System.exit(0);
  }
  
  /**
   * ����һ�� ServerRuntimeMBeans��
   * �� MBean ������ʱ MBean ��εĸ���
   * �����е�ÿ�������������Լ���ʵ����
   */
  public static ObjectName[] getServerRuntimes() throws Exception 
  {
    return (ObjectName[]) connection.getAttribute(service, "ServerRuntimes");
  }
  
  //��ȡ������״̬
  public static void getServerStatus() throws Exception 
  {
    ObjectName[] serverRT = getServerRuntimes();
    int length = (int) serverRT.length;
    if (length == 0) 
    {
      System.out.println("FileSplitFlagWLSRunStatus=ShutDown$EndSplitFlag");
    } 
    else 
    {
      System.out.println("FileSplitFlagWLSRunStatus=Running$EndSplitFlag");
    }   
  }
  
  //��ȡjms״̬
  public static void GetJmsStatus() throws Exception
  {
    String strReturn = "FileSplitFlag";
    ObjectName[] serverRT = getServerRuntimes();
    int length = (int) serverRT.length;
    if (length == 0) 
    {
      System.out.println("FileSplitFlagWLSRunStatus=ShutDown$EndSplitFlag");
      return;
    }
    for (int i = 0; i < length; i++) 
    {
      ObjectName jmsRT = (ObjectName) connection.getAttribute(serverRT[i], "JMSRuntime");
      strReturn = strReturn + "ConnectionsTotalCoun=" + connection.getAttribute(jmsRT, "ConnectionsTotalCount") + "$";
      strReturn = strReturn + "ConnectionsCurrentCount=" + connection.getAttribute(jmsRT, "ConnectionsCurrentCount") + "$";
      strReturn = strReturn + "ConnectionsHighCount=" + connection.getAttribute(jmsRT, "ConnectionsHighCount") + "$";
      strReturn = strReturn + "JMSServersTotalCount=" + connection.getAttribute(jmsRT, "JMSServersTotalCount") + "$";
      strReturn = strReturn + "JMSServersCurrentCount=" + connection.getAttribute(jmsRT, "JMSServersCurrentCount") + "$";
      strReturn = strReturn + "JMSServersHighCount=" + connection.getAttribute(jmsRT, "JMSServersHighCount") + "$";
      
      ObjectName[] jmsConRT = (ObjectName[]) connection.getAttribute(jmsRT, "Connections");
      long SessionsTotalCount = 0L;
      long SessionsCurrentCount = 0L;
      long SessionsHighCount = 0L;
      long MessagesReceivedCount = 0L;
      long MessagesPendingCount = 0L;
      long MessagesSentCount = 0L;
      long tmpLong = 0L;
      int lengthConRT = (int) jmsConRT.length;
      for(int j = 0; j < lengthConRT; j++) 
      {
        tmpLong  = (Long) connection.getAttribute(jmsConRT[j], "SessionsTotalCount");
        SessionsTotalCount += tmpLong;
        tmpLong  = (Long) connection.getAttribute(jmsConRT[j], "SessionsCurrentCount");
        SessionsCurrentCount += tmpLong;
        tmpLong  = (Long) connection.getAttribute(jmsConRT[j], "SessionsHighCount");
        SessionsHighCount += tmpLong;
        
        ObjectName[] jmsSesRT = (ObjectName[]) connection.getAttribute(jmsConRT[j], "Sessions");
        int lengthSesRT = (int) jmsSesRT.length;
        for(int k = 0; k < lengthSesRT; k++) 
        {
          tmpLong  = (Long) connection.getAttribute(jmsSesRT[k], "MessagesReceivedCount");
          MessagesReceivedCount += tmpLong;
          tmpLong  = (Long) connection.getAttribute(jmsSesRT[k], "MessagesPendingCount");
          MessagesPendingCount += tmpLong;
          tmpLong  = (Long) connection.getAttribute(jmsSesRT[k], "MessagesSentCount");
          MessagesSentCount += tmpLong;
        }    
      }
      
      strReturn = strReturn + "SessionsTotalCount=" + SessionsTotalCount + "$";
      strReturn = strReturn + "SessionsCurrentCount=" + SessionsCurrentCount + "$";
      strReturn = strReturn + "SessionsHighCount=" + SessionsHighCount + "$";
      strReturn = strReturn + "MessagesReceivedCount=" + MessagesReceivedCount + "$";
      strReturn = strReturn + "MessagesPendingCount=" + MessagesPendingCount + "$";
      strReturn = strReturn + "MessagesSentCount=" + MessagesSentCount + "$";
      strReturn = strReturn + "EndSplitFlag";
      
      System.out.println(strReturn);
    }
  }
 
  //��ȡִ�ж�����Ϣ 
  public static void GetExecuteQueueInfo() throws Exception
  {
    StringBuffer stringbuffer = new StringBuffer();
    stringbuffer.append("FileSplitFlag");
    ObjectName[] serverRT = getServerRuntimes();
    int length = (int) serverRT.length;
    if (length == 0) 
    {
      System.out.println("FileSplitFlagWLSRunStatus=ShutDown$EndSplitFlag");
      return;
    }
    for (int i = 0; i < length; i++) 
    {
      ObjectName[] execQueueRT = (ObjectName[]) connection.getAttribute(serverRT[i], "ExecuteQueueRuntimes");
      int lengthExecQueueRT = (int) execQueueRT.length;
      for (int j = 0; j < lengthExecQueueRT; j++) 
      {
        String strTmp = (String) connection.getAttribute(execQueueRT[j], "Name");
        stringbuffer.append(strTmp).append("=").append(strTmp);
        stringbuffer.append("$");
      }
    }
    
    stringbuffer.append("EndSplitFlag");
    System.out.println(stringbuffer.toString());
  }
  
  public static void GetExecuteQueueInfo(String strName) throws Exception
  {
    StringBuffer stringbuffer = new StringBuffer();
    stringbuffer.append("FileSplitFlag");
    ObjectName[] serverRT = getServerRuntimes();
    int length = (int) serverRT.length;
    if (length == 0) 
    {
      System.out.println("FileSplitFlagWLSRunStatus=ShutDown$EndSplitFlag");
      return;
    }
    for (int i = 0; i < length; i++) 
    {
      ObjectName[] execQueueRT = (ObjectName[]) connection.getAttribute(serverRT[i], "ExecuteQueueRuntimes");
      int lengthExecQueueRT = (int) execQueueRT.length;
      for (int j = 0; j < lengthExecQueueRT; j++) 
      {
        String strTmp = (String) connection.getAttribute(execQueueRT[j], "Name");        
        if(strName.compareTo(strTmp) == 0)
        {
          int iTmp = 0;
          long lTmp = 0L;
        
          stringbuffer.append("executeQueueName=").append(strTmp).append("$");
          
          iTmp = (Integer) connection.getAttribute(execQueueRT[j], "ExecuteThreadTotalCount");
          stringbuffer.append("ExecuteThreadTotalCount=").append(iTmp).append("$");
          
          iTmp = (Integer) connection.getAttribute(execQueueRT[j], "ExecuteThreadCurrentIdleCount");
          stringbuffer.append("ExecuteThreadCurrentIdleCount=").append(iTmp).append("$");
          
          lTmp = (Long) connection.getAttribute(execQueueRT[j], "PendingRequestOldestTime");
          stringbuffer.append("PendingRequestOldestTime=").append(lTmp).append("$");
          
          iTmp = (Integer) connection.getAttribute(execQueueRT[j], "PendingRequestCurrentCount");
          stringbuffer.append("PendingRequestCurrentCount=").append(iTmp).append("$");
          
          iTmp = (Integer) connection.getAttribute(execQueueRT[j], "ServicedRequestTotalCount");
          stringbuffer.append("ServicedRequestTotalCount=").append(iTmp).append("$");
          
          stringbuffer.append("EndSplitFlag");
          
          System.out.println(stringbuffer.toString());
          return;
        }
      }
    }       
    System.out.println("FileSplitFlagError=Query Param Error!$EndSplitFlag");
  }
  
  //��ȡ���ӳ���Ϣ 
  public static void GetConnectionPoolInfo() throws Exception
  {
    StringBuffer stringbuffer = new StringBuffer();
    stringbuffer.append("FileSplitFlag");
    ObjectName[] serverRT = getServerRuntimes();
    int length = (int) serverRT.length;
    if (length == 0) 
    {
      System.out.println("FileSplitFlagWLSRunStatus=ShutDown$EndSplitFlag");
      return;
    }
    for (int i = 0; i < length; i++) 
    {
      ObjectName JDBCServiceRT = (ObjectName) connection.getAttribute(serverRT[i], "JDBCServiceRuntime");
      ObjectName[] poolRT = (ObjectName[]) connection.getAttribute(JDBCServiceRT, "JDBCDataSourceRuntimeMBeans");
      int lengthPoolRT = (int) poolRT.length;
      for (int j = 0; j < lengthPoolRT; j++) 
      {
        String strTmp = (String) connection.getAttribute(poolRT[j], "Name");
        stringbuffer.append(strTmp).append("=").append(strTmp);
        stringbuffer.append("$");
      }
    }
    
    stringbuffer.append("EndSplitFlag");
    System.out.println(stringbuffer.toString());
  }

   public static void GetConnectionPoolInfo(String strName) throws Exception
   {
     StringBuffer stringbuffer = new StringBuffer();
    stringbuffer.append("FileSplitFlag");
    ObjectName[] serverRT = getServerRuntimes();
    int length = (int) serverRT.length;
    if (length == 0) 
    {
      System.out.println("FileSplitFlagWLSRunStatus=ShutDown$EndSplitFlag");
      return;
    }
    for (int i = 0; i < length; i++) 
    {
      ObjectName JDBCServiceRT = (ObjectName) connection.getAttribute(serverRT[i], "JDBCServiceRuntime");
      ObjectName[] poolRT = (ObjectName[]) connection.getAttribute(JDBCServiceRT, "JDBCDataSourceRuntimeMBeans");
      int lengthPoolRT = (int) poolRT.length;
      for (int j = 0; j < lengthPoolRT; j++) 
      {
        String strTmp = (String) connection.getAttribute(poolRT[j], "Name");
        if (strName.compareTo(strTmp) == 0)
        {
          int iTmp = 0;
          
          stringbuffer.append("ConnectionPoolName=").append(strTmp).append("$");
          
          strTmp = (String) connection.getAttribute(JDBCServiceRT, "Name");         
          stringbuffer.append("Server=").append(strTmp).append("$");
          
          strTmp = (String) connection.getAttribute(poolRT[j], "State");
          stringbuffer.append("Status=").append(strTmp).append("$");

	
	  //-------------------------------------------------------------------------------------
	  //���Ӽ��ָ�� (zouXiao 2008.12.24)
	  //------------------------------------------------------------------------------------- 
	  iTmp = (Integer) connection.getAttribute(poolRT[j], "ConnectionsTotalCount");//��������
          stringbuffer.append("ConnectionsTotalCount=").append(iTmp).append("$");

	  iTmp = (Integer) connection.getAttribute(poolRT[j], "CurrCapacity");//��ǰ���ӳ�����
          stringbuffer.append("CurrCapacity=").append(iTmp).append("$");       
          iTmp = (Integer) connection.getAttribute(poolRT[j], "CurrCapacityHighCount");//��ʷ������ӳ�����,ע��9.x��8.x�����Ʋ�ͬ
          stringbuffer.append("MaxCapacity=").append(iTmp).append("$");


	  iTmp = (Integer) connection.getAttribute(poolRT[j], "ActiveConnectionsCurrentCount");//��ǰ�������
          stringbuffer.append("ActiveConnectionsCurrentCount=").append(iTmp).append("$");
	  iTmp = (Integer) connection.getAttribute(poolRT[j], "ActiveConnectionsAverageCount");//��ʷƽ���������
          stringbuffer.append("ActiveConnectionsAverageCount=").append(iTmp).append("$");         
          iTmp = (Integer) connection.getAttribute(poolRT[j], "ActiveConnectionsHighCount");//��ʷ���������
          stringbuffer.append("ActiveConnectionsHighCount=").append(iTmp).append("$");


	  iTmp = (Integer) connection.getAttribute(poolRT[j], "NumAvailable");//��ǰ�����õ�������
          stringbuffer.append("NumAvailable=").append(iTmp).append("$");
	  iTmp = (Integer) connection.getAttribute(poolRT[j], "HighestNumAvailable");//��ʷ�������õ�������
          stringbuffer.append("HighestNumAvailable=").append(iTmp).append("$");         
          iTmp = (Integer) connection.getAttribute(poolRT[j], "NumUnavailable");//��ǰ�������õ�������
          stringbuffer.append("NumUnavailable=").append(iTmp).append("$");
	  iTmp = (Integer) connection.getAttribute(poolRT[j], "HighestNumUnavailable");//��ʷ��󲻿����õ�������
          stringbuffer.append("HighestNumUnavailable=").append(iTmp).append("$");



	  iTmp = (Integer) connection.getAttribute(poolRT[j], "WaitingForConnectionCurrentCount");//��ǰ�ȴ�������
          stringbuffer.append("WaitingForConnectionCurrentCount=").append(iTmp).append("$");
	  iTmp = (Integer) connection.getAttribute(poolRT[j], "WaitingForConnectionHighCount");//��ʷ���ȴ�������
          stringbuffer.append("WaitingForConnectionHighCount=").append(iTmp).append("$");         
          iTmp = (Integer) connection.getAttribute(poolRT[j], "WaitSecondsHighCount");//��ʷ��ȴ�����ʱ��
          stringbuffer.append("WaitSecondsHighCount=").append(iTmp).append("$");

        
          iTmp = (Integer) connection.getAttribute(poolRT[j], "FailuresToReconnectCount");//��������ʧ�ܵĴ���
          stringbuffer.append("FailuresToReconnectCount=").append(iTmp).append("$");

       
          iTmp = (Integer) connection.getAttribute(poolRT[j], "ConnectionDelayTime");//����ƽ����ʱ
          stringbuffer.append("ConnectionDelayTime=").append(iTmp).append("$");

 
	  
	  strTmp = (String) connection.getAttribute(poolRT[j], "VersionJDBCDriver");//JDBC�����汾
          stringbuffer.append("VersionJDBCDriver=").append(strTmp).append("$");

          
          stringbuffer.append("EndSplitFlag");
          
          System.out.println(stringbuffer.toString());
          return;
        }
      }
    }
    
    System.out.println("FileSplitFlagError=ConnectionPool Param Error!$EndSplitFlag");
   }
   

  //��ȡweb�����Ϣ
  public static void GetWebAppComponentInfo() throws Exception
  {
    StringBuffer stringbuffer = new StringBuffer();
    stringbuffer.append("FileSplitFlag");
    ObjectName[] serverRT = getServerRuntimes();
    int length = (int) serverRT.length;
    if (length == 0) 
    {
      System.out.println("FileSplitFlagWLSRunStatus=ShutDown$EndSplitFlag");
      return;
    }
    for (int i = 0; i < length; i++) 
    {
      ObjectName[] appRT = (ObjectName[]) connection.getAttribute(serverRT[i], "ApplicationRuntimes");
      int lengthAppRT = (int) appRT.length;
      for (int j = 0; j < lengthAppRT; j++) 
      {
        ObjectName[] compRT = (ObjectName[]) connection.getAttribute(appRT[j], "ComponentRuntimes");
        int lengthCompRT = (int) compRT.length;
        for (int k = 0; k < lengthCompRT; k++)
        {
          String strTmp = (String) connection.getAttribute(compRT[k], "Type");
          if (strTmp.compareTo("WebAppComponentRuntime") == 0) 
          {
            strTmp = (String) connection.getAttribute(compRT[k], "Name");
            stringbuffer.append(strTmp).append("=").append(strTmp);
            stringbuffer.append("$");
          }
        }
      }
    }
    
    stringbuffer.append("EndSplitFlag");
    System.out.println(stringbuffer.toString());
  }
  
  public static void GetWebAppComponentInfo(String strName) throws Exception
  {
    StringBuffer stringbuffer = new StringBuffer();
    stringbuffer.append("FileSplitFlag");
    ObjectName[] serverRT = getServerRuntimes();
    int length = (int) serverRT.length;
    if (length == 0) 
    {
      System.out.println("FileSplitFlagWLSRunStatus=ShutDown$EndSplitFlag");
      return;
    }
    for (int i = 0; i < length; i++) 
    {
      ObjectName[] appRT = (ObjectName[]) connection.getAttribute(serverRT[i], "ApplicationRuntimes");
      int lengthAppRT = (int) appRT.length;
      for (int j = 0; j < lengthAppRT; j++) 
      {
        ObjectName[] compRT = (ObjectName[]) connection.getAttribute(appRT[j], "ComponentRuntimes");
        int lengthCompRT = (int) compRT.length;
        for (int k = 0; k < lengthCompRT; k++)
        {
          String strTmp = (String) connection.getAttribute(compRT[k], "Name");
          if (strTmp.compareTo(strName) == 0) 
          {
            int iTmp = 0;
            
            strTmp = (String) connection.getAttribute(compRT[k], "ComponentName");
            stringbuffer.append("WebAppName=").append(strTmp).append("$");
                    
            stringbuffer.append("mbeanName=").append(strName).append("$");
            
            strTmp = (String) connection.getAttribute(compRT[k], "Status");
            stringbuffer.append("Status=").append(strTmp).append("$");
            
            iTmp = (Integer) connection.getAttribute(compRT[k], "OpenSessionsCurrentCount");
            stringbuffer.append("OpenSessionsCurrentCount=").append(iTmp).append("$");
            
            iTmp = (Integer) connection.getAttribute(compRT[k], "OpenSessionsHighCount");
            stringbuffer.append("OpenSessionsHighCount=").append(iTmp).append("$");
            
            iTmp = (Integer) connection.getAttribute(compRT[k], "SessionsOpenedTotalCount");
            stringbuffer.append("SessionsOpenedTotalCount=").append(iTmp).append("$");
            
            stringbuffer.append("EndSplitFlag");
            
            System.out.println(stringbuffer.toString());
            return;
          }
        }
      }
    }
    
    System.out.println("FileSplitFlagError=ConnectionPool Param Error!$EndSplitFlag");
  }

  //��ȡ����Ϣ
  public static void GetClusterInfo() throws Exception
  {
    StringBuffer stringbuffer = new StringBuffer();
    stringbuffer.append("FileSplitFlag");
    ObjectName[] serverRT = getServerRuntimes();
    int length = (int) serverRT.length;
    if (length == 0) 
    {
      System.out.println("FileSplitFlagWLSRunStatus=ShutDown$EndSplitFlag");
      return;
    }
    for (int i = 0; i < length; i++) 
    {
      ObjectName cltRT = (ObjectName) connection.getAttribute(serverRT[i], "ClusterRuntime");
      if (cltRT == null)
      {
        System.out.println("FileSplitFlagError=No Found!$EndSplitFlag");
        return;
      }
      String strTmp = (String) connection.getAttribute(cltRT, "Name");
      stringbuffer.append(strTmp).append("=").append(strTmp);
      stringbuffer.append("$");
    }
    
    stringbuffer.append("EndSplitFlag");
    System.out.println(stringbuffer.toString());
  }
  
  public static void GetClusterInfo(String strName) throws Exception
  {
    StringBuffer stringbuffer = new StringBuffer();
    stringbuffer.append("FileSplitFlag");
    ObjectName[] serverRT = getServerRuntimes();
    int length = (int) serverRT.length;
    if (length == 0) 
    {
      System.out.println("FileSplitFlagWLSRunStatus=ShutDown$EndSplitFlag");
      return;
    }
    for (int i = 0; i < length; i++) 
    {
      ObjectName cltRT = (ObjectName) connection.getAttribute(serverRT[i], "ClusterRuntime");    
      String strTmp = (String) connection.getAttribute(cltRT, "Name");
      if (strTmp.compareTo(strName) == 0)
      {      
        int iTmp = (Integer) connection.getAttribute(cltRT, "AliveServerCount");
        stringbuffer.append("AliveServerCount=").append(iTmp).append("$");
        
        stringbuffer.append("ClusterName=").append(strTmp).append("$");
        
        long lTmp = (Long) connection.getAttribute(cltRT, "MulticastMessagesLostCount");
        stringbuffer.append("MulticastMessagesLostCount=").append(lTmp).append("$");
        
        stringbuffer.append("EndSplitFlag");
        
        System.out.println(stringbuffer.toString());
        return;
      }
    }
    
    System.out.println("FileSplitFlagError=ConnectionPool Param Error!$EndSplitFlag");
  }
  
  //��ȡJVMI��Ϣ
  public static void GetJVMInfo() throws Exception
  {
    StringBuffer stringbuffer = new StringBuffer();
    stringbuffer.append("FileSplitFlag");
    ObjectName[] serverRT = getServerRuntimes();
    int length = (int) serverRT.length;
    if (length == 0) 
    {
      System.out.println("FileSplitFlagWLSRunStatus=ShutDown$EndSplitFlag");
      return;
    }
    for (int i = 0; i < length; i++) 
    {
      ObjectName jvmRT = (ObjectName) connection.getAttribute(serverRT[i], "JVMRuntime");    
      String strTmp = (String) connection.getAttribute(jvmRT, "Name");
      stringbuffer.append(strTmp).append("=").append(strTmp);
      stringbuffer.append("$");
    }
    
    stringbuffer.append("EndSplitFlag");
    System.out.println(stringbuffer.toString());
  }
  
  public static void GetJVMInfo(String strName) throws Exception
  {
    StringBuffer stringbuffer = new StringBuffer();
    stringbuffer.append("FileSplitFlag");
    ObjectName[] serverRT = getServerRuntimes();
    int length = (int) serverRT.length;
    if (length == 0) 
    {
      System.out.println("FileSplitFlagWLSRunStatus=ShutDown$EndSplitFlag");
      return;
    }
    for (int i = 0; i < length; i++) 
    {
      ObjectName jvmRT = (ObjectName) connection.getAttribute(serverRT[i], "JVMRuntime");    
      String strTmp = (String) connection.getAttribute(jvmRT, "Name");
      if (strTmp.compareTo(strName) == 0)
      {      
        long lTmp = (Long) connection.getAttribute(jvmRT, "HeapSizeCurrent");
        stringbuffer.append("HeapSizeCurrent=").append(lTmp).append("$");
        
        lTmp = (Long) connection.getAttribute(jvmRT, "HeapFreeCurrent");
        stringbuffer.append("HeapFreeCurrent=").append(lTmp).append("$");
        
        stringbuffer.append("Name=").append(strTmp).append("$");
        
        stringbuffer.append("EndSplitFlag");
        
        System.out.println(stringbuffer.toString());
        return;
      }
    }
    
    System.out.println("FileSplitFlagError=ConnectionPool Param Error!$EndSplitFlag");
  }
  

   //��ȡ������Ϣ
    public static void GetServerInfo() throws Exception
  {
    StringBuffer stringbuffer = new StringBuffer();
    stringbuffer.append("FileSplitFlag");
    ObjectName[] serverRT = getServerRuntimes();
    int length = (int) serverRT.length;
    if (length == 0) 
    {
      System.out.println("FileSplitFlagWLSRunStatus=ShutDown$EndSplitFlag");
      return;
    }
    for (int i = 0; i < length; i++) 
    {    
      String strTmp = (String) connection.getAttribute(serverRT[i], "Name");
      stringbuffer.append(strTmp).append("=").append(strTmp);
      stringbuffer.append("$");
    }
    
    stringbuffer.append("EndSplitFlag");
    System.out.println(stringbuffer.toString());
  }
  
  public static void GetServerInfo(String strName) throws Exception
  {
    StringBuffer stringbuffer = new StringBuffer();
    stringbuffer.append("FileSplitFlag");
    ObjectName[] serverRT = getServerRuntimes();
    int length = (int) serverRT.length;
    if (length == 0) 
    {
      System.out.println("FileSplitFlagWLSRunStatus=ShutDown$EndSplitFlag");
      return;
    }
    for (int i = 0; i < length; i++) 
    {    
      String strTmp = (String) connection.getAttribute(serverRT[i], "Name");
      if (strTmp.compareTo(strName) == 0)
      {      
        int iTmp = (Integer) connection.getAttribute(serverRT[i], "RestartsTotalCount");
        stringbuffer.append("RestartsTotalCount=").append(iTmp).append("$");
        
        iTmp = (Integer) connection.getAttribute(serverRT[i], "OpenSocketsCurrentCount");
        stringbuffer.append("OpenSocketsCurrentCount=").append(iTmp).append("$");
        
        long lTmp = (Long) connection.getAttribute(serverRT[i], "SocketsOpenedTotalCount");
        stringbuffer.append("SocketsOpenedTotalCount=").append(lTmp).append("$");

        ObjectName obj = (ObjectName) connection.getAttribute(serverRT[i], "ConnectorServiceRuntime");
        iTmp = (Integer) connection.getAttribute(obj, "ConnectionPoolCurrentCount");
        stringbuffer.append("ConnectionsCount=").append(iTmp).append("$");
        
        strTmp = (String) connection.getAttribute(serverRT[i], "WeblogicVersion");
        stringbuffer.append("WeblogicVersion=").append(strTmp).append("$");
        
        stringbuffer.append("Name=").append(strName).append("$");
        
        strTmp = (String) connection.getAttribute(serverRT[i], "ListenAddress");
        stringbuffer.append("ListenAddress=").append(strTmp).append("$");
        
        iTmp = (Integer) connection.getAttribute(serverRT[i], "ListenPort");
        stringbuffer.append("ListenPort=").append(iTmp).append("$");
        
        iTmp = (Integer) connection.getAttribute(serverRT[i], "SSLListenPort");
        stringbuffer.append("SSLListenPort=").append(iTmp).append("$");
        
        iTmp = (Integer) connection.getAttribute(serverRT[i], "AdministrationPort");
        stringbuffer.append("AdministrationPort=").append(iTmp).append("$");
        
        iTmp = (Integer) connection.getAttribute(serverRT[i], "AdminServerListenPort");
        stringbuffer.append("AdminServerListenPort=").append(iTmp).append("$");
        
        stringbuffer.append("EndSplitFlag");
        
        System.out.println(stringbuffer.toString());
        return;
      }
    }
    
    System.out.println("FileSplitFlagError=ConnectionPool Param Error!$EndSplitFlag");
  }
 
   //��ȡJB��Ϣ
  public static void GetEJBCacheInfo() throws Exception
  {
    StringBuffer stringbuffer = new StringBuffer();
    stringbuffer.append("FileSplitFlag");
    ObjectName[] serverRT = getServerRuntimes();
    int length = (int) serverRT.length;
    if (length == 0) 
    {
      System.out.println("FileSplitFlagWLSRunStatus=ShutDown$EndSplitFlag");
      return;
    }
    for (int i = 0; i < length; i++) 
    {
      ObjectName[] appRT = (ObjectName[]) connection.getAttribute(serverRT[i], "ApplicationRuntimes");
      int lengthAppRT = (int) appRT.length;
      for (int j = 0; j < lengthAppRT; j++) 
      {
        ObjectName[] compRT = (ObjectName[]) connection.getAttribute(appRT[j], "ComponentRuntimes");
        int lengthCompRT = (int) compRT.length;
        for (int k = 0; k < lengthCompRT; k++)
        {
          String strTmp = (String) connection.getAttribute(compRT[k], "Type");
          if (strTmp.compareTo("EJBComponentRuntimeMBean") == 0) 
          {
            ObjectName[] ejbRT = (ObjectName[]) connection.getAttribute(compRT[k], "EJBRuntimes");
            int lengthEjbRT = (int) ejbRT.length;
            for (int m = 0; m < lengthEjbRT; m++)
            {
              strTmp = (String) connection.getAttribute(ejbRT[m], "Type");
              if ((strTmp.compareTo("EntityEJBRuntimeMBean") == 0) || (strTmp.compareTo("StatefulEJBRuntimeMBean") == 0))
              {
                ObjectName cacheRT = (ObjectName) connection.getAttribute(ejbRT[m], "CacheRuntime");
                strTmp = (String) connection.getAttribute(cacheRT, "Name");
                stringbuffer.append(strTmp).append("=").append(strTmp);
                stringbuffer.append("$");
              }
            }        
          }
        }
      }
    }
    
    stringbuffer.append("EndSplitFlag");
    System.out.println(stringbuffer.toString());
  }
  
  public static void GetEJBCacheInfo(String strName) throws Exception
  {
    StringBuffer stringbuffer = new StringBuffer();
    stringbuffer.append("FileSplitFlag");
    ObjectName[] serverRT = getServerRuntimes();
    int length = (int) serverRT.length;
    if (length == 0) 
    {
      System.out.println("FileSplitFlagWLSRunStatus=ShutDown$EndSplitFlag");
      return;
    }
    for (int i = 0; i < length; i++) 
    {
      ObjectName[] appRT = (ObjectName[]) connection.getAttribute(serverRT[i], "ApplicationRuntimes");
      int lengthAppRT = (int) appRT.length;
      for (int j = 0; j < lengthAppRT; j++) 
      {
        ObjectName[] compRT = (ObjectName[]) connection.getAttribute(appRT[j], "ComponentRuntimes");
        int lengthCompRT = (int) compRT.length;
        for (int k = 0; k < lengthCompRT; k++)
        {
          String strTmp = (String) connection.getAttribute(compRT[k], "Type");
          if (strTmp.compareTo("EJBComponentRuntimeMBean") == 0) 
          {
            ObjectName[] ejbRT = (ObjectName[]) connection.getAttribute(compRT[k], "EJBRuntimes");
            int lengthEjbRT = (int) ejbRT.length;
            for (int m = 0; m < lengthEjbRT; m++)
            {
              strTmp = (String) connection.getAttribute(ejbRT[m], "Type");
              if ((strTmp.compareTo("EntityEJBRuntimeMBean") == 0) || (strTmp.compareTo("StatefulEJBRuntimeMBean") == 0))
              {
                ObjectName cacheRT = (ObjectName) connection.getAttribute(ejbRT[m], "CacheRuntime");
                strTmp = (String) connection.getAttribute(cacheRT, "Name");
                if (strTmp.compareTo(strName) == 0)
                {
                  stringbuffer.append("EJBCacheName=").append(strTmp).append("$");
                  
                  int iTmp = (Integer) connection.getAttribute(cacheRT, "CachedBeansCurrentCount");
                  stringbuffer.append("CurrentCount=").append(iTmp).append("$");
                  
                  long lTmp = (Long) connection.getAttribute(cacheRT, "CacheAccessCount");
                  stringbuffer.append("AccessCount=").append(lTmp).append("$");
                  
                  lTmp = (Long) connection.getAttribute(cacheRT, "CacheHitCount");
                  stringbuffer.append("HitCount=").append(lTmp).append("$");
                  
                  lTmp = (Long) connection.getAttribute(cacheRT, "ActivationCount");
                  stringbuffer.append("ActivationCount=").append(lTmp).append("$");
                  
                  lTmp = (Long) connection.getAttribute(cacheRT, "PassivationCount");
                  stringbuffer.append("PassivationCount=").append(lTmp).append("$");
                  
                  stringbuffer.append("EndSplitFlag");
                  
                  System.out.println(stringbuffer.toString());
                  return;
                }             
              }
            }          
          }
        }
      }
    }
    
    System.out.println("FileSplitFlagError=ConnectionPool Param Error!$EndSplitFlag");
  }
  
  //��ȡJB�¼���Ϣ
  public static void GetEJBTransactionInfo() throws Exception
  {
    StringBuffer stringbuffer = new StringBuffer();
    stringbuffer.append("FileSplitFlag");
    ObjectName[] serverRT = getServerRuntimes();
    int length = (int) serverRT.length;
    if (length == 0) 
    {
      System.out.println("FileSplitFlagWLSRunStatus=ShutDown$EndSplitFlag");
      return;
    }
    for (int i = 0; i < length; i++) 
    {
      ObjectName[] appRT = (ObjectName[]) connection.getAttribute(serverRT[i], "ApplicationRuntimes");
      int lengthAppRT = (int) appRT.length;
      for (int j = 0; j < lengthAppRT; j++) 
      {
        ObjectName[] compRT = (ObjectName[]) connection.getAttribute(appRT[j], "ComponentRuntimes");
        int lengthCompRT = (int) compRT.length;
        for (int k = 0; k < lengthCompRT; k++)
        {
          String strTmp = (String) connection.getAttribute(compRT[k], "Type");
         // System.out.println(strTmp);
          if (strTmp.compareTo("EJBComponentRuntime") == 0) 
          {
            ObjectName[] ejbRT = (ObjectName[]) connection.getAttribute(compRT[k], "EJBRuntimes");
            int lengthEjbRT = (int) ejbRT.length;
            for (int m = 0; m < lengthEjbRT; m++)
            {           
              ObjectName tranRT = (ObjectName) connection.getAttribute(ejbRT[m], "TransactionRuntime");
              strTmp = (String) connection.getAttribute(tranRT, "Name");
              stringbuffer.append(strTmp).append("=").append(strTmp);
              stringbuffer.append("$");
            }
          }
        }
      }
    }
    
    stringbuffer.append("EndSplitFlag");
    System.out.println(stringbuffer.toString());
  }
  
  public static void GetEJBTransactionInfo(String strName) throws Exception
  {
    StringBuffer stringbuffer = new StringBuffer();
    stringbuffer.append("FileSplitFlag");
    ObjectName[] serverRT = getServerRuntimes();
    int length = (int) serverRT.length;
    if (length == 0) 
    {
      System.out.println("FileSplitFlagWLSRunStatus=ShutDown$EndSplitFlag");
      return;
    }
    for (int i = 0; i < length; i++) 
    {
      ObjectName[] appRT = (ObjectName[]) connection.getAttribute(serverRT[i], "ApplicationRuntimes");
      int lengthAppRT = (int) appRT.length;
      for (int j = 0; j < lengthAppRT; j++) 
      {
        ObjectName[] compRT = (ObjectName[]) connection.getAttribute(appRT[j], "ComponentRuntimes");
        int lengthCompRT = (int) compRT.length;
        for (int k = 0; k < lengthCompRT; k++)
        {
          String strTmp = (String) connection.getAttribute(compRT[k], "Type");
          if (strTmp.compareTo("EJBComponentRuntime") == 0) 
          {
            ObjectName[] ejbRT = (ObjectName[]) connection.getAttribute(compRT[k], "EJBRuntimes");
            int lengthEjbRT = (int) ejbRT.length;
            for (int m = 0; m < lengthEjbRT; m++)
            {
              ObjectName tranRT = (ObjectName) connection.getAttribute(ejbRT[m], "TransactionRuntime");
              strTmp = (String) connection.getAttribute(tranRT, "Name");
              if (strTmp.compareTo(strName) == 0) 
              {
                stringbuffer.append("Name=").append(strTmp).append("$");
                
                long lTmp = (Long) connection.getAttribute(tranRT, "TransactionsCommittedTotalCount");
                stringbuffer.append("CommittedTotalCount=").append(lTmp).append("$");
                
                lTmp = (Long) connection.getAttribute(tranRT, "TransactionsRolledBackTotalCount");
                stringbuffer.append("RolledBackTotalCount=").append(lTmp).append("$");
                
                lTmp = (Long) connection.getAttribute(tranRT, "TransactionsTimedOutTotalCount");
                stringbuffer.append("TimedOutTotalCount=").append(lTmp).append("$");
                
                stringbuffer.append("EndSplitFlag");
                
                System.out.println(stringbuffer.toString());
                return;
              }
            }
            //strTmp = (String) connection.getAttribute(ejbRT[k], "Type");           
          }
        }
      }
    }
    
    System.out.println("FileSplitFlagError=EJBTransaction Param Error!$EndSplitFlag");
  }
 
   //��ȡJB����Ϣ 
  public static void GetEJBPoolInfo() throws Exception
  {
    StringBuffer stringbuffer = new StringBuffer();
    stringbuffer.append("FileSplitFlag");
    ObjectName[] serverRT = getServerRuntimes();
    int length = (int) serverRT.length;
    if (length == 0) 
    {
      System.out.println("FileSplitFlagWLSRunStatus=ShutDown$EndSplitFlag");
      return;
    }
    for (int i = 0; i < length; i++) 
    {
      ObjectName[] appRT = (ObjectName[]) connection.getAttribute(serverRT[i], "ApplicationRuntimes");
      int lengthAppRT = (int) appRT.length;
      for (int j = 0; j < lengthAppRT; j++) 
      {
        ObjectName[] compRT = (ObjectName[]) connection.getAttribute(appRT[j], "ComponentRuntimes");
        int lengthCompRT = (int) compRT.length;
        for (int k = 0; k < lengthCompRT; k++)
        {
          String strTmp = (String) connection.getAttribute(compRT[k], "Type");
          if (strTmp.compareTo("EJBComponentRuntimeMBean") == 0) 
          {
            ObjectName[] ejbRT = (ObjectName[]) connection.getAttribute(compRT[k], "EJBRuntimes");
            int lengthEjbRT = (int) ejbRT.length;
            for (int m = 0; m < lengthEjbRT; m++)
            {
              strTmp = (String) connection.getAttribute(ejbRT[m], "Type");
              if ((strTmp.compareTo("EntityEJBRuntimeMBean") == 0) 
              || (strTmp.compareTo("StatefulEJBRuntimeMBean") == 0)
              || (strTmp.compareTo("MessageDrivenEJBRuntimeMBean") == 0))
              {
                ObjectName poolRT = (ObjectName) connection.getAttribute(ejbRT[m], "PoolRuntime");
                strTmp = (String) connection.getAttribute(poolRT, "Name");
                stringbuffer.append(strTmp).append("=").append(strTmp);
                stringbuffer.append("$");
              }
            }        
          }
        }
      }
    }
    
    stringbuffer.append("EndSplitFlag");
    System.out.println(stringbuffer.toString());
  }
  
  public static void GetEJBPoolInfo(String strName) throws Exception
  {
    StringBuffer stringbuffer = new StringBuffer();
    stringbuffer.append("FileSplitFlag");
    ObjectName[] serverRT = getServerRuntimes();
    int length = (int) serverRT.length;
    if (length == 0) 
    {
      System.out.println("FileSplitFlagWLSRunStatus=ShutDown$EndSplitFlag");
      return;
    }
    for (int i = 0; i < length; i++) 
    {
      ObjectName[] appRT = (ObjectName[]) connection.getAttribute(serverRT[i], "ApplicationRuntimes");
      int lengthAppRT = (int) appRT.length;
      for (int j = 0; j < lengthAppRT; j++) 
      {
        ObjectName[] compRT = (ObjectName[]) connection.getAttribute(appRT[j], "ComponentRuntimes");
        int lengthCompRT = (int) compRT.length;
        for (int k = 0; k < lengthCompRT; k++)
        {
          String strTmp = (String) connection.getAttribute(compRT[k], "Type");
          if (strTmp.compareTo("EJBComponentRuntimeMBean") == 0) 
          {
            ObjectName[] ejbRT = (ObjectName[]) connection.getAttribute(compRT[k], "EJBRuntimes");
            int lengthEjbRT = (int) ejbRT.length;
            for (int m = 0; m < lengthEjbRT; m++)
            {
              strTmp = (String) connection.getAttribute(ejbRT[m], "Type");
              if ((strTmp.compareTo("EntityEJBRuntimeMBean") == 0) 
              || (strTmp.compareTo("StatefulEJBRuntimeMBean") == 0)
              || (strTmp.compareTo("MessageDrivenEJBRuntimeMBean") == 0))
              {
                ObjectName poolRT = (ObjectName) connection.getAttribute(ejbRT[m], "CacheRuntime");
                strTmp = (String) connection.getAttribute(poolRT, "Name");
                if (strTmp.compareTo(strName) == 0)
                {
                  stringbuffer.append("EJBPoolName=").append(strTmp).append("$");
                  
                  int iTmp = (Integer) connection.getAttribute(poolRT, "IdleBeansCount");
                  stringbuffer.append("IdleBeansCount=").append(iTmp).append("$");
                  
                  iTmp = (Integer) connection.getAttribute(poolRT, "BeansInUseCount");
                  stringbuffer.append("InUseCount=").append(iTmp).append("$");
                  
                  long lTmp = (Long) connection.getAttribute(poolRT, "WaiterTotalCount");
                  stringbuffer.append("WaiterTotalCount=").append(lTmp).append("$");
                  
                  lTmp = (Long) connection.getAttribute(poolRT, "TimeoutTotalCount");
                  stringbuffer.append("TimeoutTotalCount=").append(lTmp).append("$");
                                  
                  stringbuffer.append("EndSplitFlag");
                  
                  System.out.println(stringbuffer.toString());
                  return;
                }             
              }
            }          
          }
        }
      }
    }
    
    System.out.println("FileSplitFlagError=ConnectionPool Param Error!$EndSplitFlag");
  }
  /**
   * �������
   */
  public static void main(String[] args) 
  {
    try//����쳣��׽��zouxiao_2008.11.13��
    {
    if(args.length < 5)
    {
      die();
    }
    String username = args[0];
    String password = args[1];
    String hostname = args[2];
    String portString = args[3];
    String strTaskType = args[4];
    String strTaskParam = "";
    if(args.length >= 6)
    {
      strTaskParam = args[5];
    }
    String mserver = DOMAINRUNTIME_URI;
    getService(DOMAINRUNTIME_SERVICE);
    initConnection(hostname, portString, username, password,mserver);
    
    if(strTaskType.compareTo("WlsStatus") == 0) 
    {
      getServerStatus();
      return;
    }
    else if(strTaskType.compareTo("JmsStatus") == 0) 
    {
      GetJmsStatus();
      return;
    } 
    else if(strTaskType.compareTo("ReqExecuteQueueInfo") == 0) 
    {   
      if(strTaskParam.compareTo("") == 0) 
      {
        GetExecuteQueueInfo();
      } 
      else 
      {
        GetExecuteQueueInfo(strTaskParam);
      }
    } 
    else if(strTaskType.compareTo("ConnectionPoolInfo") == 0) 
    {
      if(strTaskParam.compareTo("") == 0) 
      {
        GetConnectionPoolInfo();
      } 
      else 
      {
        GetConnectionPoolInfo(strTaskParam);
      }
    } 
    else if(strTaskType.compareTo("WebApp") == 0) 
    {
      if(strTaskParam.compareTo("") == 0) 
      {
        GetWebAppComponentInfo();
      } 
      else 
      {
        GetWebAppComponentInfo(strTaskParam);
      }
    } 
    else if(strTaskType.compareTo("Cluster") == 0) 
    {
      if(strTaskParam.compareTo("") == 0) 
      {
        GetClusterInfo();
      } 
      else 
      {
        GetClusterInfo(strTaskParam);
      }
    } 
    else if(strTaskType.compareTo("JVM") == 0) {
      if(strTaskParam.compareTo("") == 0) 
      {
        GetJVMInfo();
      } 
      else 
      {
        GetJVMInfo(strTaskParam);
      }
    } 
    else if(strTaskType.compareTo("Server") == 0) 
    {
      if(strTaskParam.compareTo("") == 0) 
      {
        GetServerInfo();
      } 
      else 
      {
        GetServerInfo(strTaskParam);
      }
    }
    else if(strTaskType.compareTo("EJBCacheInfo") == 0) 
    {
      if(strTaskParam.compareTo("") == 0) 
      {
        GetEJBCacheInfo();
      } 
      else 
      {
        GetEJBCacheInfo(strTaskParam);
      }
    }
    else if(strTaskType.compareTo("EJBTransactionInfo") == 0) 
    {
      if(strTaskParam.compareTo("") == 0) 
      {
        GetEJBTransactionInfo();
      } 
      else 
      {
        GetEJBTransactionInfo(strTaskParam);
      }
    }
    else if(strTaskType.compareTo("EJBPoolInfo") == 0) 
    {
      if(strTaskParam.compareTo("") == 0) 
      {
        GetEJBPoolInfo();
      } 
      else 
      {
        GetEJBPoolInfo(strTaskParam);
      }
    }
    else 
    {
      die();
    }
    
    connector.close();
    }
    catch(Exception e)
    {
    	//e.printStackTrace();
        System.err.println("FileSplitFlagError=Exception: " + e.getMessage() +"GetWeblogicperf" + "$EndSplitFlag");
        return;
    }
  }
}
