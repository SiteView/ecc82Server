VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "Exchange"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Private Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)
'======================================================================================================
' 功能 : 取 Exchange Server 参数
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getExchange = 符合条件的串
'        无取得值时：getExchange = 空串
'======================================================================================================
Public Function getMailboxName(vServer, vUser, vPwd)
    On Error Resume Next
    
    Dim locator As New SWbemLocator
    Dim service As SWbemServices
    
    If Len(vServer) = 0 And Len(vUser) = 0 And Len(vPwd) = 0 Then
        Set service = locator.ConnectServer
    Else
        Set service = locator.ConnectServer(vServer, "root\MicrosoftExchangeV2", vUser, vPwd)
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        getMailboxName = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim objset As SWbemObjectSet
    
    Dim x As SWbemObject, str As String
    
    '===================================================================================================
    ' 取数据开始
    '===================================================================================================
    Set objset = service.ExecQuery("select * from Exchange_Mailbox")
    If objset.Count = 0 Then
            str = "NULL?"
    Else
    
        For Each x In objset
           str = str & x.MailboxDisplayName & "?"
        Next
    
    End If
    
    '===================================================================================================
    ' 取数据结束
    '===================================================================================================
    
    Err.Clear
    
    ' 错误处理
    If Err.Number <> 0 Then
        getMailboxName = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        getMailboxName = str
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
    
End Function

Public Function getMailbox(vServer, vUser, vPwd, vMailboxName)
    On Error Resume Next
    
    Dim locator As New SWbemLocator
    Dim service As SWbemServices
    
    If Len(vServer) = 0 And Len(vUser) = 0 And Len(vPwd) = 0 Then
        Set service = locator.ConnectServer
    Else
        Set service = locator.ConnectServer(vServer, "root\MicrosoftExchangeV2", vUser, vPwd)
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        getMailbox = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim objset As SWbemObjectSet
    
    Dim x As SWbemObject, str As String
    
    '===================================================================================================
    ' 取数据开始
    '===================================================================================================
    Dim sql As String
    sql = "select * from Exchange_Mailbox where MailboxDisplayName ='"
    sql = sql & vMailboxName
    sql = sql & "'"
    
    Set objset = service.ExecQuery(sql)
    If objset.Count = 0 Then
            Size = 0
            TotalItems = 0
            LastLogonTime = "Null"
            LastLogoffTime = "Null"
            LastLoggedOnUserAccount = "Null"
            DeletedMessageSizeExtended = 0
            StorageGroupName = "Null"
            ServerName = "Null"
    Else
    
        For Each x In objset
            Size = x.Size
            TotalItems = x.TotalItems
            LastLogonTime = x.LastLogonTime
            LastLogoffTime = x.LastLogoffTime
            LastLoggedOnUserAccount = x.LastLoggedOnUserAccount
            DeletedMessageSizeExtended = x.DeletedMessageSizeExtended
            StorageGroupName = x.StorageGroupName
            ServerName = x.ServerName
        Next
    
    End If
    
    '===================================================================================================
    ' 取数据结束
    '===================================================================================================
    
    Err.Clear
    
    ' 错误处理
    If Err.Number <> 0 Then
        getMailbox = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        Dim strResult As String
        
        strResult = "Size=" & Size
        strResult = strResult & "$TotalItems=" & TotalItems
        strResult = strResult & "$LastLogonTime=" & LastLogonTime
        strResult = strResult & "$LastLogoffTime=" & LastLogoffTime
        strResult = strResult & "$LastLoggedOnUserAccount=" & LastLoggedOnUserAccount
        strResult = strResult & "$DeletedMessageSizeExtended=" & DeletedMessageSizeExtended
        strResult = strResult & "$StorageGroupName=" & StorageGroupName
        strResult = strResult & "$ServerName=" & ServerName
        strResult = strResult & "$"

        getMailbox = strResult
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
    
End Function


Public Function getMTA(vServer, vUser, vPwd)
    On Error Resume Next
    
    Dim locator As New SWbemLocator
    Dim service As SWbemServices
    
    If Len(vServer) = 0 And Len(vUser) = 0 And Len(vPwd) = 0 Then
        Set service = locator.ConnectServer
    Else
        Set service = locator.ConnectServer(vServer, "root\cimv2", vUser, vPwd)
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        getMTA = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim objset As SWbemObjectSet
    
    Dim x As SWbemObject, str As String
    
    '===================================================================================================
    ' 取数据开始
    '===================================================================================================
    Dim sql As String
    sql = "select * from Win32_PerfRawData_MSExchangeMTA_MSExchangeMTA "
    Set objset = service.ExecQuery(sql)
    If objset.Count = 0 Then
            MessageBytesPerSec = 0
            MessagesPerSec = 0
            TotalFailedConversions = 0
            X25ReceiveBytesPersec = 0
            X25TransmitBytesPersec = 0
            XAPIReceiveBytesPersec = 0
            XAPITransmitBytesPersec = 0
            TP4ReceiveBytesPersec = 0
            TP4TransmitBytesPersec = 0
            TCPIPReceiveBytesPersec = 0
            TCPIPTransmitBytesPersec = 0
            RASReceiveBytesPersec = 0
            RASTransmitBytesPersec = 0
            Timestamp_PerfTime = 1
    Else
    
        For Each x In objset
            MessageBytesPerSec = x.MessageBytesPerSec
            MessagesPerSec = x.MessagesPerSec
            TotalFailedConversions = x.TotalFailedConversions
            X25ReceiveBytesPersec = x.X25ReceiveBytesPersec
            X25TransmitBytesPersec = x.X25TransmitBytesPersec
            XAPIReceiveBytesPersec = x.XAPIReceiveBytesPersec
            XAPITransmitBytesPersec = x.XAPITransmitBytesPersec
            TP4ReceiveBytesPersec = x.TP4ReceiveBytesPersec
            TP4TransmitBytesPersec = x.TP4TransmitBytesPersec
            TCPIPReceiveBytesPersec = x.TCPIPReceiveBytesPersec
            TCPIPTransmitBytesPersec = x.TCPIPTransmitBytesPersec
            RASReceiveBytesPersec = x.RASReceiveBytesPersec
            RASTransmitBytesPersec = x.RASTransmitBytesPersec
            Timestamp_PerfTime = x.Timestamp_PerfTime
        Next
    
    End If
    
    '===================================================================================================
    ' 取数据结束
    '===================================================================================================
    
     '===================================================================================================
    ' 取数据开始
    '===================================================================================================
    sql = "select * from Win32_PerfRawData_MSExchangeMTA_MSExchangeMTA "
    Set objset = service.ExecQuery(sql)
    If objset.Count = 0 Then
            MessageBytesPerSec1 = 0
            MessagesPerSec1 = 0
            TotalFailedConversions1 = 0
            X25ReceiveBytesPersec1 = 0
            X25TransmitBytesPersec1 = 0
            XAPIReceiveBytesPersec1 = 0
            XAPITransmitBytesPersec1 = 0
            TP4ReceiveBytesPersec1 = 0
            TP4TransmitBytesPersec1 = 0
            TCPIPReceiveBytesPersec1 = 0
            TCPIPTransmitBytesPersec1 = 0
            RASReceiveBytesPersec1 = 0
            RASTransmitBytesPersec1 = 0
            Timestamp_PerfTime1 = 2
    Else
    
        For Each x In objset
            MessageBytesPerSec1 = x.MessageBytesPerSec
            MessagesPerSec1 = x.MessagesPerSec
            TotalFailedConversions1 = x.TotalFailedConversions
            X25ReceiveBytesPersec1 = x.X25ReceiveBytesPersec
            X25TransmitBytesPersec1 = x.X25TransmitBytesPersec
            XAPIReceiveBytesPersec1 = x.XAPIReceiveBytesPersec
            XAPITransmitBytesPersec1 = x.XAPITransmitBytesPersec
            TP4ReceiveBytesPersec1 = x.TP4ReceiveBytesPersec
            TP4TransmitBytesPersec1 = x.TP4TransmitBytesPersec
            TCPIPReceiveBytesPersec1 = x.TCPIPReceiveBytesPersec
            TCPIPTransmitBytesPersec1 = x.TCPIPTransmitBytesPersec
            RASReceiveBytesPersec1 = x.RASReceiveBytesPersec
            RASTransmitBytesPersec1 = x.RASTransmitBytesPersec
            Timestamp_PerfTime1 = x.Timestamp_PerfTime
            PerfFreq = CDbl(x.Frequency_PerfTime)
        Next
    
    End If
    
    '===================================================================================================
    ' 取数据结束
    '===================================================================================================
     PerfFreq = CLng(Timestamp_PerfTime1 - Timestamp_PerfTime) / PerfFreq
     
    MessageBytesPerSec = MessageBytesPerSec1 - MessageBytesPerSec
    MessagesPerSec = MessagesPerSec1 - MessagesPerSec
    TotalFailedConversions = TotalFailedConversions1 - TotalFailedConversions
    X25ReceiveBytesPersec = X25ReceiveBytesPersec1 - X25ReceiveBytesPersec
    X25TransmitBytesPersec = X25TransmitBytesPersec1 - X25TransmitBytesPersec
    XAPIReceiveBytesPersec = XAPIReceiveBytesPersec1 - XAPIReceiveBytesPersec
    XAPITransmitBytesPersec = XAPITransmitBytesPersec1 - XAPITransmitBytesPersec
    TP4ReceiveBytesPersec = TP4ReceiveBytesPersec1 - TP4ReceiveBytesPersec
    TP4TransmitBytesPersec = TP4TransmitBytesPersec1 - TP4TransmitBytesPersec
    TCPIPReceiveBytesPersec = TCPIPReceiveBytesPersec1 - TCPIPReceiveBytesPersec
    TCPIPTransmitBytesPersec = TCPIPTransmitBytesPersec1 - TCPIPTransmitBytesPersec
    RASReceiveBytesPersec = RASReceiveBytesPersec1 - RASReceiveBytesPersec
    RASTransmitBytesPersec = RASTransmitBytesPersec1 - RASTransmitBytesPersec
     
    MessageBytesPerSec = CLng(MessageBytesPerSec / PerfFreq)
    MessagesPerSec = CLng(MessagesPerSec / PerfFreq)
    TotalFailedConversions = CLng(TotalFailedConversions / PerfFreq)
    X25ReceiveBytesPersec = CLng(X25ReceiveBytesPersec / PerfFreq)
    X25TransmitBytesPersec = CLng(X25TransmitBytesPersec / PerfFreq)
    XAPIReceiveBytesPersec = CLng(XAPIReceiveBytesPersec / PerfFreq)
    XAPITransmitBytesPersec = CLng(XAPITransmitBytesPersec / PerfFreq)
    TP4ReceiveBytesPersec = CLng(TP4ReceiveBytesPersec / PerfFreq)
    TP4TransmitBytesPersec = CLng(TP4TransmitBytesPersec / PerfFreq)
    TCPIPReceiveBytesPersec = CLng(TCPIPReceiveBytesPersec / PerfFreq)
    TCPIPTransmitBytesPersec = CLng(TCPIPTransmitBytesPersec / PerfFreq)
    RASReceiveBytesPersec = CLng(RASReceiveBytesPersec / PerfFreq)
    RASTransmitBytesPersec = CLng(RASTransmitBytesPersec / PerfFreq)
            
    Err.Clear
    
    ' 错误处理
    If Err.Number <> 0 Then
        getMTA = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        Dim strResult As String
        
        strResult = "MessageBytesPerSec=" & MessageBytesPerSec
        strResult = strResult & "$MessagesPerSec=" & MessagesPerSec
        strResult = strResult & "$TotalFailedConversions=" & TotalFailedConversions
        strResult = strResult & "$X25ReceiveBytesPersec=" & X25ReceiveBytesPersec
        strResult = strResult & "$X25TransmitBytesPersec=" & X25TransmitBytesPersec
        strResult = strResult & "$XAPIReceiveBytesPersec=" & XAPIReceiveBytesPersec
        strResult = strResult & "$XAPITransmitBytesPersec=" & XAPITransmitBytesPersec
        strResult = strResult & "$TP4ReceiveBytesPersec=" & TP4ReceiveBytesPersec
        strResult = strResult & "$TP4TransmitBytesPersec=" & TP4TransmitBytesPersec
        strResult = strResult & "$TCPIPReceiveBytesPersec=" & TCPIPReceiveBytesPersec
        strResult = strResult & "$TCPIPTransmitBytesPersec=" & TCPIPTransmitBytesPersec
        strResult = strResult & "$RASReceiveBytesPersec=" & RASReceiveBytesPersec
        strResult = strResult & "$RASTransmitBytesPersec=" & RASTransmitBytesPersec
     
        strResult = strResult & "$"

        getMTA = strResult
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
    
End Function
Public Function getSMTPServer(vServer, vUser, vPwd)
    On Error Resume Next
    
    Dim locator As New SWbemLocator
    Dim service As SWbemServices
    
    If Len(vServer) = 0 And Len(vUser) = 0 And Len(vPwd) = 0 Then
        Set service = locator.ConnectServer
    Else
        Set service = locator.ConnectServer(vServer, "root\cimv2", vUser, vPwd)
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        getSMTPServer = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim objset As SWbemObjectSet
    
    Dim x As SWbemObject, str As String
    
    '===================================================================================================
    ' 取数据开始
    '===================================================================================================
    Dim sql As String
    sql = "select * from Win32_PerfRawData_SMTPSVC_SMTPServer "
    Set objset = service.ExecQuery(sql)
    If objset.Count = 0 Then
            MessagesSentPersec = 0
            MessagesReceivedPersec = 0
            TotalConnectionErrors = 0
            OutboundConnectionsCurrent = 0
            OutboundConnectionsRefused = 0
            BytesReceivedPersec = 0
            BytesSentPersec = 0
            BytesTotalPersec = 0
            MessageBytesReceivedPersec = 0
            MessageBytesSentPersec = 0
            MessageBytesTotalPersec = 0
            Timestamp_PerfTime = 1
    Else
    
        For Each x In objset
            MessagesSentPersec = x.MessagesSentPersec
            MessagesReceivedPersec = x.MessagesReceivedPersec
            TotalConnectionErrors = x.TotalConnectionErrors
            OutboundConnectionsCurrent = x.OutboundConnectionsCurrent
            OutboundConnectionsRefused = x.OutboundConnectionsRefused
            BytesReceivedPersec = x.BytesReceivedPersec
            BytesSentPersec = x.BytesSentPersec
            BytesTotalPersec = x.BytesTotalPersec
            MessageBytesReceivedPersec = x.MessageBytesReceivedPersec
            MessageBytesSentPersec = x.MessageBytesSentPersec
            MessageBytesTotalPersec = x.MessageBytesTotalPersec
            Timestamp_PerfTime = x.Timestamp_PerfTime
        Next
    
    End If
    
    '===================================================================================================
    ' 取数据结束
    '===================================================================================================
    
     '===================================================================================================
    ' 取数据开始
    '===================================================================================================
    sql = "select * from Win32_PerfRawData_SMTPSVC_SMTPServer "
    Set objset = service.ExecQuery(sql)
    If objset.Count = 0 Then
            MessagesSentPersec1 = 0
            MessagesReceivedPersec1 = 0
            TotalConnectionErrors1 = 0
            OutboundConnectionsCurrent1 = 0
            OutboundConnectionsRefused1 = 0
            BytesReceivedPersec1 = 0
            BytesSentPersec1 = 0
            BytesTotalPersec1 = 0
            MessageBytesReceivedPersec1 = 0
            MessageBytesSentPersec1 = 0
            MessageBytesTotalPersec1 = 0
            Timestamp_PerfTime1 = 2
            PerfFreq = 1
    Else
    
        For Each x In objset
            MessagesSentPersec1 = x.MessagesSentPersec
            MessagesReceivedPersec1 = x.MessagesReceivedPersec
            TotalConnectionErrors1 = x.TotalConnectionErrors
            OutboundConnectionsCurrent1 = x.OutboundConnectionsCurrent
            OutboundConnectionsRefused1 = x.OutboundConnectionsRefused
            BytesReceivedPersec1 = x.BytesReceivedPersec
            BytesSentPersec1 = x.BytesSentPersec
            BytesTotalPersec1 = x.BytesTotalPersec
            MessageBytesReceivedPersec1 = x.MessageBytesReceivedPersec
            MessageBytesSentPersec1 = x.MessageBytesSentPersec
            MessageBytesTotalPersec1 = x.MessageBytesTotalPersec
            Timestamp_PerfTime1 = x.Timestamp_PerfTime
            PerfFreq = CDbl(x.Frequency_PerfTime)
        Next
    
    End If
    
    '===================================================================================================
    ' 取数据结束
    '===================================================================================================
    PerfFreq = CLng(Timestamp_PerfTime1 - Timestamp_PerfTime) / PerfFreq
    
    MessagesSentPersec = MessagesSentPersec1 - MessagesSentPersec
    MessagesReceivedPersec = MessagesReceivedPersec1 - MessagesReceivedPersec
    TotalConnectionErrors = TotalConnectionErrors1 - TotalConnectionErrors
    OutboundConnectionsCurrent = OutboundConnectionsCurrent1 - OutboundConnectionsCurrent
    OutboundConnectionsRefused = OutboundConnectionsRefused1 - OutboundConnectionsRefused
    BytesReceivedPersec = BytesReceivedPersec1 - BytesReceivedPersec
    BytesSentPersec = BytesSentPersec1 - BytesSentPersec
    BytesTotalPersec = BytesTotalPersec1 - BytesTotalPersec
    MessageBytesReceivedPersec = MessageBytesReceivedPersec1 - MessageBytesReceivedPersec
    MessageBytesSentPersec = MessageBytesSentPersec1 - MessageBytesSentPersec
    MessageBytesTotalPersec = MessageBytesTotalPersec1 - MessageBytesTotalPersec
    
    MessagesSentPersec = CLng(MessagesSentPersec / PerfFreq)
    MessagesReceivedPersec = CLng(MessagesReceivedPersec / PerfFreq)
    TotalConnectionErrors = CLng(TotalConnectionErrors / PerfFreq)
    OutboundConnectionsCurrent = CLng(OutboundConnectionsCurrent / PerfFreq)
    OutboundConnectionsRefused = CLng(OutboundConnectionsRefused / PerfFreq)
    BytesReceivedPersec = CLng(BytesReceivedPersec / PerfFreq)
    BytesSentPersec = CLng(BytesSentPersec / PerfFreq)
    BytesTotalPersec = CLng(BytesTotalPersec / PerfFreq)
    MessageBytesReceivedPersec = CLng(MessageBytesReceivedPersec / PerfFreq)
    MessageBytesSentPersec = CLng(MessageBytesSentPersec / PerfFreq)
    MessageBytesTotalPersec = CLng(MessageBytesTotalPersec / PerfFreq)
    
    
    Err.Clear
    
    ' 错误处理
    If Err.Number <> 0 Then
        getSMTPServer = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        Dim strResult As String
        
        strResult = "MessagesSentPersec=" & MessagesSentPersec
        strResult = strResult & "$MessagesReceivedPersec=" & MessagesReceivedPersec
        strResult = strResult & "$TotalConnectionErrors=" & TotalConnectionErrors
        strResult = strResult & "$OutboundConnectionsCurrent=" & OutboundConnectionsCurrent
        strResult = strResult & "$OutboundConnectionsRefused=" & OutboundConnectionsRefused
        strResult = strResult & "$BytesReceivedPersec=" & BytesReceivedPersec
        strResult = strResult & "$BytesSentPersec=" & BytesSentPersec
        strResult = strResult & "$BytesTotalPersec=" & BytesTotalPersec
        strResult = strResult & "$MessageBytesReceivedPersec=" & MessageBytesReceivedPersec
        strResult = strResult & "$MessageBytesSentPersec=" & MessageBytesSentPersec
        strResult = strResult & "$MessageBytesTotalPersec=" & MessageBytesTotalPersec

        strResult = strResult & "$"

        getSMTPServer = strResult
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
    
End Function
Public Function getPOP3(a, b, c)
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
        getPOP3 = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim objset As SWbemObjectSet
    
    Dim x As SWbemObject, str As String
    
    '===================================================================================================
    ' 取数据开始
    '===================================================================================================
    Set objset = service.ExecQuery("select * from Win32_PerfRawData_MSExchangeMTA_MSExchangeMTA")
    
    If objset.Count > 0 Then
        If Err.Number = 0 Then
        Else
            getPOP3 = "error=指定主机没有Exchange Server$"
            Exit Function
        End If
    End If
    
    Err.Clear
    
    ' Win32_PerfRawData_POP3Svc_MSExchangePOP3
    Set objset = service.ExecQuery("select * from Win32_PerfRawData_POP3Svc_MSExchangePOP3 where name='_Total'")
    If objset.Count = 0 Then
        'getExchange = "error=指定主机没有Exchange Server$"
        'Exit Function
            POP3_ConnectionsCurrent = 0
            POP3_AUTHFailures = 0
            POP3_ConnectionsRejected = 0
            POP3_ConnectionsTotal = 0
    Else
    
        For Each x In objset
            POP3_ConnectionsCurrent = x.ConnectionsCurrent
            POP3_AUTHFailures = x.AUTHFailures
            POP3_ConnectionsRejected = x.ConnectionsRejected
            POP3_ConnectionsTotal = x.ConnectionsTotal
        Next
    
    End If
    
    '===================================================================================================
    ' 取数据结束
    '===================================================================================================
    
    Err.Clear
    
    ' 错误处理
    If Err.Number <> 0 Then
        getPOP3 = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        Dim strResult As String
        
        strResult = "POP3_ConnectionsCurrent=" & POP3_ConnectionsCurrent
        strResult = strResult & "$POP3_AUTHFailures=" & POP3_AUTHFailures
        strResult = strResult & "$POP3_ConnectionsRejected=" & POP3_ConnectionsRejected
        strResult = strResult & "$POP3_ConnectionsTotalt=" & POP3_ConnectionsTotal
        strResult = strResult & "$"

        getPOP3 = strResult
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function


Public Function getSMTP(a, b, c)
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
        getSMTP = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim objset As SWbemObjectSet
    
    Dim x As SWbemObject, str As String
    
    '===================================================================================================
    ' 取数据开始
    '===================================================================================================
    Set objset = service.ExecQuery("select * from Win32_PerfRawData_MSExchangeMTA_MSExchangeMTA")
    
    If objset.Count > 0 Then
        If Err.Number = 0 Then
        Else
            getSMTP = "error=指定主机没有Exchange Server$"
            Exit Function
        End If
    End If
    
    Err.Clear
    
    
    ' Win32_PerfRawData_SMTPSVC_SMTPServer
    Set objset = service.ExecQuery("select * from Win32_PerfRawData_SMTPSVC_SMTPServer where name='_Total'")
    
    If objset.Count = 0 Then
            SMTPServer_InboundConnectionsCurrent = 0
            SMTPServer_OutboundConnectionsCurrent = 0
            SMTPServer_Totalmessagessubmitted = 0
            SMTPServer_MessagesSentTotal = 0
    Else
        For Each x In objset
            SMTPServer_InboundConnectionsCurrent = x.InboundConnectionsCurrent
            SMTPServer_OutboundConnectionsCurrent = x.OutboundConnectionsCurrent
            SMTPServer_Totalmessagessubmitted = x.Totalmessagessubmitted
            SMTPServer_MessagesSentTotal = x.MessagesSentTotal
        Next
    End If
    
    '===================================================================================================
    ' 取数据结束
    '===================================================================================================
    
    Err.Clear
    
    ' 错误处理
    If Err.Number <> 0 Then
        getSMTP = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        Dim strResult As String
       
       strResult = "OutboundConnectionsCurrent=" & SMTPServer_OutboundConnectionsCurrent
       strResult = strResult & "$MessagesSentTotal=" & SMTPServer_MessagesSentTotal
       strResult = strResult & "$Totalmessagessubmitted=" & SMTPServer_Totalmessagessubmitted
       strResult = strResult & "$InboundConnectionsCurrent=" & SMTPServer_InboundConnectionsCurrent
       strResult = strResult & "$"
       getSMTP = strResult
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function
Public Function getWebMail(a, b, c)
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
        getWebMail = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim objset As SWbemObjectSet
    
    Dim x As SWbemObject, str As String
    
    '===================================================================================================
    ' 取数据开始
    '===================================================================================================
    Set objset = service.ExecQuery("select * from Win32_PerfRawData_MSExchangeMTA_MSExchangeMTA")
    
    If objset.Count > 0 Then
        If Err.Number = 0 Then
        Else
            getWebMail = "error=指定主机没有Exchange Server$"
            Exit Function
        End If
    End If
    
    Err.Clear
    
    
    ' Win32_PerfRawData_EXOLEDB_MSExchangeWebMail
    Set objset = service.ExecQuery("select * from Win32_PerfRawData_EXOLEDB_MSExchangeWebMail where name='_Total'")
    
    If objset.Count = 0 Then
            WebMail_Messageopenstotal = 0
            WebMail_Messagesavestotal = 0
            WebMail_Messagesendstotal = 0
    Else
        For Each x In objset
            WebMail_Messageopenstotal = x.Messageopenstotal
            WebMail_Messagesavestotal = x.Messagesavestotal
            WebMail_Messagesendstotal = x.Messagesendstotal
        Next
    End If
    
    '===================================================================================================
    ' 取数据结束
    '===================================================================================================
    
    Err.Clear
    
    ' 错误处理
    If Err.Number <> 0 Then
        getWebMail = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        Dim strResult As String
        strResult = "WebMail_Messageopenstotal=" & WebMail_Messageopenstotal
        strResult = strResult & "$WebMail_Messagesavestotal=" & WebMail_Messagesavestotal
        strResult = strResult & "$WebMail_Messagesendstotal=" & WebMail_Messagesendstotal
        strResult = strResult & "$"
        getWebMail = strResult
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function



Public Function getExchange(a, b, c)
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
        getExchange = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim objset As SWbemObjectSet
    
    Dim x As SWbemObject, str As String
    
    '===================================================================================================
    ' 取数据开始
    '===================================================================================================
    Set objset = service.ExecQuery("select * from Win32_PerfRawData_MSExchangeMTA_MSExchangeMTA")
    
    If objset.Count > 0 Then
        If Err.Number = 0 Then
        Else
            getExchange = "error=指定主机没有Exchange Server$"
            Exit Function
        End If
    End If
    
    Err.Clear
    
    ' Win32_PerfRawData_POP3Svc_MSExchangePOP3
    Set objset = service.ExecQuery("select * from Win32_PerfRawData_POP3Svc_MSExchangePOP3 where name='_Total'")
    If objset.Count = 0 Then
        'getExchange = "error=指定主机没有Exchange Server$"
        'Exit Function
            POP3_ConnectionsCurrent = 0
            POP3_AUTHFailures = 0
            POP3_ConnectionsRejected = 0
            POP3_ConnectionsTotal = 0
    Else
    
        For Each x In objset
            POP3_ConnectionsCurrent = x.ConnectionsCurrent
            POP3_AUTHFailures = x.AUTHFailures
            POP3_ConnectionsRejected = x.ConnectionsRejected
            POP3_ConnectionsTotal = x.ConnectionsTotal
        Next
    
    End If
    
    ' Win32_PerfRawData_IMAP4Svc_MSExchangeIMAP4
    Set objset = service.ExecQuery("select * from Win32_PerfRawData_IMAP4Svc_MSExchangeIMAP4 where name='_Total'")
    
    If objset.Count = 0 Then
            IMAP4_AUTHENTICATEFailures = 0
            IMAP4_ConnectionsCurrent = 0
            IMAP4_ConnectionsTotal = 0
    Else
        For Each x In objset
            IMAP4_AUTHENTICATEFailures = x.AUTHENTICATEFailures
            IMAP4_ConnectionsCurrent = x.ConnectionsCurrent
            IMAP4_ConnectionsTotal = x.ConnectionsTotal
        Next
    End If
    
    
    ' Win32_PerfRawData_EXOLEDB_MSExchangeWebMail
    Set objset = service.ExecQuery("select * from Win32_PerfRawData_EXOLEDB_MSExchangeWebMail where name='_Total'")
    
    If objset.Count = 0 Then
            WebMail_Messageopenstotal = 0
            WebMail_Messagesavestotal = 0
            WebMail_Messagesendstotal = 0
    Else
        For Each x In objset
            WebMail_Messageopenstotal = x.Messageopenstotal
            WebMail_Messagesavestotal = x.Messagesavestotal
            WebMail_Messagesendstotal = x.Messagesendstotal
        Next
    End If
    
    
    ' Win32_PerfRawData_EXOLEDB_MSExchangeOledbEvents
    Set objset = service.ExecQuery("select * from Win32_PerfRawData_EXOLEDB_MSExchangeOledbEvents where name='_Total'")
    
    If objset.Count = 0 Then
            OledbEvents_EventsCompletionTotal = 0
            OledbEvents_EventsSubmissionTotal = 0
    Else
        For Each x In objset
            OledbEvents_EventsCompletionTotal = x.EventsCompletionTotal
            OledbEvents_EventsSubmissionTotal = x.EventsSubmissionTotal
        Next
    End If
    
    
    ' Win32_PerfRawData_EXOLEDB_MSExchangeOledbResource
    Set objset = service.ExecQuery("select * from Win32_PerfRawData_EXOLEDB_MSExchangeOledbResource where name='_Total'")
    
    If objset.Count = 0 Then
            OledbResource_ActiveSessions = 0
            OledbResource_TransactionsAbortedTotal = 0
            OledbResource_TransactionsCommittedTotal = 0
            OledbResource_TransactionsStartedTotal = 0
    Else
        For Each x In objset
            OledbResource_ActiveSessions = x.ActiveSessions
            OledbResource_TransactionsAbortedTotal = x.TransactionsAbortedTotal
            OledbResource_TransactionsCommittedTotal = x.TransactionsCommittedTotal
            OledbResource_TransactionsStartedTotal = x.TransactionsStartedTotal
        Next
    End If
    
    ' Win32_PerfRawData_SMTPSVC_SMTPServer
    Set objset = service.ExecQuery("select * from Win32_PerfRawData_SMTPSVC_SMTPServer where name='_Total'")
    
    If objset.Count = 0 Then
            SMTPServer_InboundConnectionsCurrent = 0
            SMTPServer_OutboundConnectionsCurrent = 0
            SMTPServer_Totalmessagessubmitted = 0
            SMTPServer_MessagesSentTotal = 0
    Else
        For Each x In objset
            SMTPServer_InboundConnectionsCurrent = x.InboundConnectionsCurrent
            SMTPServer_OutboundConnectionsCurrent = x.OutboundConnectionsCurrent
            SMTPServer_Totalmessagessubmitted = x.Totalmessagessubmitted
            SMTPServer_MessagesSentTotal = x.MessagesSentTotal
        Next
    End If
    
    '===================================================================================================
    ' 取数据结束
    '===================================================================================================
    
    Err.Clear
    
    ' 错误处理
    If Err.Number <> 0 Then
        getExchange = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        Dim strResult As String
        strResult = "POP3_AUTHFailures=" & POP3_AUTHFailures & "$POP3_ConnectionsRejected=" & POP3_ConnectionsRejected & "$POP3_ConnectionsTotal=" & POP3_ConnectionsTotal & "$IMAP4_AUTHENTICATEFailures=" & IMAP4_AUTHENTICATEFailures & "$IMAP4_ConnectionsCurrent=" & IMAP4_ConnectionsCurrent & "$IMAP4_ConnectionsTotal=" & IMAP4_ConnectionsTotal & "$WebMail_Messageopenstotal=" & WebMail_Messageopenstotal & "$WebMail_Messagesavestotal=" & WebMail_Messagesavestotal & "$WebMail_Messagesendstotal=" & WebMail_Messagesendstotal & "$OledbEvents_EventsCompletionTotal=" & OledbEvents_EventsCompletionTotal & "$OledbEvents_EventsSubmissionTotal=" & OledbEvents_EventsSubmissionTotal & "$OledbResource_ActiveSessions=" & OledbResource_ActiveSessions & "$OledbResource_TransactionsAbortedTotal=" & OledbResource_TransactionsAbortedTotal & "$OledbResource_TransactionsCommittedTotal=" & OledbResource_TransactionsCommittedTotal & "$OledbResource_TransactionsStartedTotal=" & OledbResource_TransactionsStartedTotal & "$"
        getExchange = "SMTPServer_OutboundConnectionsCurrent=" & SMTPServer_OutboundConnectionsCurrent & "$SMTPServer_MessagesSentTotal=" & SMTPServer_MessagesSentTotal & "$SMTPServer_Totalmessagessubmitted=" & SMTPServer_Totalmessagessubmitted & "$SMTPServer_InboundConnectionsCurrent=" & SMTPServer_InboundConnectionsCurrent & "$POP3_ConnectionsCurrent=" & POP3_ConnectionsCurrent & "$" & strResult

    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function


Public Function getExchangeConnectorName(host, user, psw)

    On Error Resume Next
    
    Dim locator As New SWbemLocator
    Dim service As SWbemServices
    Dim List As SWbemObjectSet
    Dim obj As SWbemObject
    Dim str As String
    
    If Len(host) = 0 And Len(user) = 0 And Len(psw) = 0 Then
        Set service = locator.ConnectServer
    Else
        Set service = locator.ConnectServer(host, "root/cimv2/applications/exchange", user, psw)
    End If
    
    Set List = service.InstancesOf("ExchangeConnectorState")
    For Each obj In List
        str = str & obj.Name & "?"
    Next
    
    ' 错误处理
    If Err.Number <> 0 Then
        getExchangeConnectorName = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        getExchangeConnectorName = str
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set List = Nothing
    Set obj = Nothing

End Function


Public Function getExchangeConnectorState(host, user, psw, param)
    On Error Resume Next
    
    Dim locator As New SWbemLocator
    Dim service As SWbemServices
    Dim List As SWbemObjectSet
    Dim obj As SWbemObject
    Dim str As String
    
    Set service = locator.ConnectServer(host, "root/cimv2/applications/exchange", user, psw)
 
    Set List = service.InstancesOf("ExchangeConnectorState")

    For Each obj In List
        If obj.Name Like param Then
            str = "IsUp=" & obj.IsUp & "$"
        End If
    Next
    
    ' 错误处理
    If Err.Number <> 0 Then
        getExchangeConnectorState = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        getExchangeConnectorState = str
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set List = Nothing
    Set obj = Nothing
    
End Function


Public Function getExchangeQueueName(host, user, psw)
    On Error Resume Next
    
    Dim locator As New SWbemLocator
    Dim service As SWbemServices
    Dim List As SWbemObjectSet
    Dim obj As SWbemObject
    Dim str As String
    
    If Len(host) = 0 And Len(user) = 0 And Len(psw) = 0 Then
        Set service = locator.ConnectServer
    Else
        Set service = locator.ConnectServer(host, "root/cimv2/applications/exchange", user, psw)
    End If
    
    Set List = service.InstancesOf("ExchangeQueue")
    For Each obj In List
        str = str & obj.QueueName & "?"
    Next
    
    ' 错误处理
    If Err.Number <> 0 Then
        getExchangeQueueName = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        getExchangeQueueName = str
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set List = Nothing
    Set obj = Nothing
    
End Function

Public Function getExchangeQueue(vServer, vUser, vPwd, vQueueName)
    On Error Resume Next
    
    Dim locator As New SWbemLocator
    Dim service As SWbemServices
    Dim str As String
    Dim List As SWbemObjectSet
    Dim obj As SWbemObject
    
    str = "host:" & vServer & "user:" & vUser & "psw:" & vPwd & "param" & vQueueName
    
    Set service = locator.ConnectServer(vServer, "root/cimv2/applications/exchange", vUser, vPwd)
 
    Set List = service.InstancesOf("ExchangeQueue")
    
    If List.Count = 0 Then
        str = "QueueName=NULL" & "$"
        str = str & "VirtualMachine=0" & "$"
        str = str & "VirtualServerName=0" & "$"
        str = str & "LinkName=0" & "$"
        str = str & "SizeOfQueue=0" & "$"
        str = str & "NumberOfMessages=0" & "$"
    Else
        For Each obj In List
            If obj.QueueName Like vQueueName Then
                str = "QueueName=" & obj.QueueName & "$"
                str = str & "VirtualMachine=" & obj.VirtualMachine & "$"
                str = str & "VirtualServerName=" & obj.VirtualServerName & "$"
                str = str & "LinkName=" & obj.LinkName & "$"
                str = str & "SizeOfQueue=" & obj.SizeOfQueue & "$"
                str = str & "NumberOfMessages=" & obj.NumberOfMessages & "$"
            End If
        Next
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        getExchangeQueue = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        getExchangeQueue = str
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set List = Nothing
    Set obj = Nothing
    
End Function

'ExchangeServerState
Public Function getExchangeServerState(host, user, psw)
    On Error Resume Next
    
    Dim locator As New SWbemLocator
    Dim service As SWbemServices
    
    If Len(host) = 0 And Len(user) = 0 And Len(psw) = 0 Then
        Set service = locator.ConnectServer
    Else
        Set service = locator.ConnectServer(host, "root/cimv2/applications/exchange", user, psw)
    End If
 
    Dim str As String
    
    Set listExchangeServerStates = service.InstancesOf("ExchangeServerState")
    For Each objExchangeServerState In listExchangeServerStates
        str = "DN=" & objExchangeServerState.DN & "$"
        str = str & "ClusterStateString=" & objExchangeServerState.ClusterStateString & "$"
        str = str & "CPUStateString=" & objExchangeServerState.CPUStateString & "$"
        str = str & "DisksStateString=" & objExchangeServerState.DisksStateString & "$"
        str = str & "MemoryStateString=" & objExchangeServerState.MemoryStateString & "$"
        str = str & "QueuesStateString=" & objExchangeServerState.QueuesStateString & "$"
        str = str & "ServerStateString=" & objExchangeServerState.ServerStateString & "$"
        str = str & "ServicesStateString=" & objExchangeServerState.ServicesStateString & "$"
        str = str & "ServerMaintenance=" & objExchangeServerState.ServerMaintenance & "$"
    Next
    
    ' 错误处理
    If Err.Number <> 0 Then
        getExchangeServerState = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        getExchangeServerState = str
    End If
    
    Set service = Nothing
    Set locator = Nothing
    
End Function
