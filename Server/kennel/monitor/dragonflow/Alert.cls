VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "Alert"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Private Const EWX_SHUTDOWN = 1
Private Const EWX_REBOOT = 2
Private Declare Function ExitWindowsEx Lib "user32" (ByVal uFlags As Long, ByVal dwReserved As Long) As Long

Public Sub reboot()
    ExitWindowsEx EWX_REBOOT, 0
End Sub


Public Sub shutdown()
    ExitWindowsEx EWX_SHUTDOWN, 0
End Sub


'======================================================================================================
' 功能 : 重起指定服务
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'        d  服务名称
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：alertRestartService = 符合条件的串
'        无取得值时：alertRestartService = 空串
'======================================================================================================
Public Function alertRestartService(a, b, c, d)
    On Error Resume Next
    
    
    Dim locator As New SWbemLocator
    Dim service As SWbemServices
    
    If Len(a) = 0 And Len(b) = 0 And Len(c) = 0 Then
        Set service = locator.ConnectServer
    Else
        Set service = locator.ConnectServer(a, "root\cimv2", b, c)
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getNetworkInterfaceNames = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        alertRestartService = "error= service error! Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim objset As SWbemObjectSet
    
    ' 查询连接到的主机上是否有指定服务
    Set objset = service.ExecQuery("select state from Win32_Service where name='" & d & "'")
    If objset.Count = 0 Then
        alertRestartService = "error=指定主机无" & d & "服务!"
        Exit Function
    End If
    
    
    Dim objproc As Object
    ' 创建进程类
    Set objproc = service.Get("Win32_Process")
    
    Dim str As String
    str = "ok"
    
    ' 关闭指定服务
    result = objproc.Create("net stop " & d, Null, Null, processid)
    
    ' 循环测试指定服务是否关闭了?
    Do While True
        Set objset = service.ExecQuery("select state from Win32_Service where name='" & d & "'")
        
        For Each tmp In objset
            If LCase(tmp.state) = "stopped" Then
                Exit Do
            End If
        Next
    Loop
    
    
    ' 重新启动指定服务
    result2 = objproc.Create("net start " & d, Null, Null, processid2)
    
    
    ' 错误处理
    If Err.Number <> 0 Then
        alertRestartService = "error= get Date error! Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        alertRestartService = str
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function



'======================================================================================================
' 功能 : 重起IIS
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：alertRestartIIS = 符合条件的串
'        无取得值时：alertRestartIIS = 空串
'======================================================================================================
Public Function alertRestartIIS(a, b, c)
    On Error Resume Next
    
    
    Dim locator As New SWbemLocator
    Dim service As SWbemServices
    
    If Len(a) = 0 And Len(b) = 0 And Len(c) = 0 Then
        Set service = locator.ConnectServer
    Else
        Set service = locator.ConnectServer(a, "root\cimv2", b, c)
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getNetworkInterfaceNames = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        alertRestartIIS = "error= service error! Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim objset As SWbemObjectSet
    
    ' 查询连接到的主机上是否有IIS服务
    Set objset = service.ExecQuery("select state from Win32_Service where name='w3svc'")
    If objset.Count = 0 Then
        alertRestartIIS = "error=指定主机无WWW服务!"
        Exit Function
    End If
    
    
    Dim objproc As Object
    ' 创建进程类
    Set objproc = service.Get("Win32_Process")
    
    Dim str As String
    str = "ok"
    
    ' 判断IIS当前状态,如果当前就已经是关闭的了,则直接启动即可
    Set objset = service.ExecQuery("select state from Win32_Service where name='w3svc'")
    For Each tmp In objset
        If LCase(tmp.state) = "stopped" Then
            ' 重新启动IIS
            result2 = objproc.Create("net start w3svc", Null, Null, processid2)
            alertRestartIIS = str
            Exit Function
        End If
    Next
    
    
    ' 关闭IIS
    result = objproc.Create("net stop w3svc", Null, Null, processid)
    
    ' 循环测试IIS是否关闭了?
    Do While True
        Set objset = service.ExecQuery("select state from Win32_Service where name='w3svc'")
        
        For Each tmp In objset
            If LCase(tmp.state) = "stopped" Then
                Exit Do
            End If
        Next
    Loop
    
    
    ' 重新启动IIS
    result2 = objproc.Create("net start w3svc", Null, Null, processid2)
    
    
    ' 错误处理
    If Err.Number <> 0 Then
        alertRestartIIS = "error= get Date error! Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        alertRestartIIS = str
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function



'======================================================================================================
' 功能 : 发消息
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'        targetMachine  目标主机
'        strMsg  消息内容
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：alertSendMessage = 符合条件的串
'        无取得值时：alertSendMessage = 空串
'======================================================================================================
Public Function alertSendMessage(a, b, c, targetMachine, strMsg)
    On Error Resume Next
    
    
    Dim locator As New SWbemLocator
    Dim service As SWbemServices
    
    If Len(a) = 0 And Len(b) = 0 And Len(c) = 0 Then
        Set service = locator.ConnectServer
    Else
        Set service = locator.ConnectServer(a, "root\cimv2", b, c)
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getNetworkInterfaceNames = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        alertSendMessage = "error= service error! Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim objset As Object
    Set objset = service.Get("Win32_Process")
    
    Dim str As String
    str = "net send " & targetMachine & " """ & strMsg & """"
    result = objset.Create(str, Null, Null, processid)
    
'    Dim x As SWbemObject, str As String
'    str = "ok"
'    If objset.Count > 0 Then
'        If Err.Number = 0 Then
'            For Each x In objset
'                x.shutdown
'            Next
'        End If
'    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        alertSendMessage = "error= get Date error! Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        str = "ok"
        alertSendMessage = str
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function



'======================================================================================================
' 功能 : 关机
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：alertShutdown = 符合条件的串
'        无取得值时：alertShutdown = 空串
'======================================================================================================
Public Function alertShutdown(a, b, c)
    On Error Resume Next
    
    
    Dim locator As New SWbemLocator
    Dim service As SWbemServices
    
    If Len(a) = 0 And Len(b) = 0 And Len(c) = 0 Then
        Set service = locator.ConnectServer
    Else
        Set service = locator.ConnectServer(a, "root\cimv2", b, c)
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getNetworkInterfaceNames = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        alertShutdown = "error= service error! Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim objset As SWbemObjectSet
    Set objset = service.ExecQuery("select * from Win32_OperatingSystem where Primary=true")
    
    Dim x As SWbemObject, str As String
    str = "ok"
    If objset.Count > 0 Then
        If Err.Number = 0 Then
            For Each x In objset
                x.shutdown
            Next
        End If
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        alertShutdown = "error= get Date error! Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        alertShutdown = str
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function



'======================================================================================================
' 功能 : 重起主机
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：alertReboot = 符合条件的串
'        无取得值时：alertReboot = 空串
'======================================================================================================
Public Function alertReboot(a, b, c)
    On Error Resume Next
    
    
    Dim locator As New SWbemLocator
    Dim service As SWbemServices
    
    If Len(a) = 0 And Len(b) = 0 And Len(c) = 0 Then
        Set service = locator.ConnectServer
    Else
        Set service = locator.ConnectServer(a, "root\cimv2", b, c)
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getNetworkInterfaceNames = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        alertReboot = "error= service error! Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim objset As SWbemObjectSet
    Set objset = service.ExecQuery("select * from Win32_OperatingSystem where Primary=true")
    
    Dim x As SWbemObject, str As String
    str = "ok"
    If objset.Count > 0 Then
        If Err.Number = 0 Then
            For Each x In objset
                x.reboot
            Next
        End If
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        alertReboot = "error= get Date error! Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        alertReboot = str
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function


'======================================================================================================
' 功能 : 播放声音文件
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'        e  播放器路径和文件名
'        f  声音文件路径和名称
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：alertPlaysound = "ok"
'        无取得值时：alertPlaysound = 空串
'======================================================================================================
Public Function alertPlaysound(a, b, c, e, f)
    On Error Resume Next
    
    
    Dim locator As New SWbemLocator
    Dim service As SWbemServices
    
    If Len(a) = 0 And Len(b) = 0 And Len(c) = 0 Then
        Set service = locator.ConnectServer
    Else
        Set service = locator.ConnectServer(a, "root\cimv2", b, c)
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getNetworkInterfaceNames = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        alertPlaysound = "error= service error! Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim objj As SWbemObject
    Set objj = service.Get("Win32_Process")
    'MsgBox Chr(34) & e & Chr(34) & " " & Chr(34) & f & Chr(34)
    result = objj.Create(Chr(34) & e & Chr(34) & " " & Chr(34) & f & Chr(34), Null, Null, processid)
    ' 错误处理
    If Err.Number <> 0 Then
        alertPlaysound = "error= get Date error! Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        alertPlaysound = "ok"
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objj = Nothing
End Function

'======================================================================================================
' 功能 : 播放Beep
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：alertPlaysound = "ok"
'        无取得值时：alertPlaysound = 空串
'======================================================================================================
Public Function alertPlayMsgBeep(a, b, c)
    On Error Resume Next
    
    
    Dim locator As New SWbemLocator
    Dim service As SWbemServices
    
    If Len(a) = 0 And Len(b) = 0 And Len(c) = 0 Then
        Set service = locator.ConnectServer
    Else
        Set service = locator.ConnectServer(a, "root\cimv2", b, c)
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getNetworkInterfaceNames = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        alertPlayMsgBeep = "error= service error! Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim objj As SWbemObject
    Set objj = service.Get("Win32_Process")
    'MsgBox Chr(34) & e & Chr(34) & " " & Chr(34) & f & Chr(34)
     result = objj.Create("cmd.exe /C" & Chr(7) & Chr(7) & Chr(7) & Chr(7) & Chr(7) & Chr(7) & Chr(7), Null, Null, processid)
    ' 错误处理
    If Err.Number <> 0 Then
        alertPlayMsgBeep = "error= get Date error! Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        alertPlayMsgBeep = "ok"
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objj = Nothing
End Function
