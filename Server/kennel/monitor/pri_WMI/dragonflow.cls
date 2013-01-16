VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "dragonflow"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Private Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)


Public Function about()
    about = "Developed by 俞雷 yulei@yeah.net 2002-2003."
End Function


Public Function trimkh(strSource)

    If InStr(strSource, "(") = 0 Or InStr(strSource, ")") = 0 Then
        ' if strSource not include ( or ) then return no change
        trimkh = strSource
    Else
        strSource = Right(strSource, Len(strSource) - InStr(strSource, "("))
        'wscript.echo strSource
        trimkh = Left(strSource, InStr(strSource, ")") - 1)
    End If

End Function

'==========================================================================================
' 目的: 实现www.logec.com所述的几个匹配规则, in, range, IPMatch(此函数用于ECS系统里)
' 输入：strSource  源字符串
'       strOperator  操作符（in, range, IPMatch）
'       strMatch  匹配串
' 返回：真/假
'==========================================================================================
Public Function fnLogecMatch(strSource, strOperator, strMatch)

    On Error Resume Next
    If Len(strSource) = 0 Or Len(strMatch) = 0 Or Len(strOperator) = 0 Then
        fnLogecMatch = False
        Exit Function
    End If
    
    strSource = Trim(strSource)
    strMatch = Trim(strMatch)
    strOperator = Trim(strOperator)
    
    ' 目前共有三种匹配操作符
    Select Case LCase(strOperator)
        Case "in" ' 在一个范围之内，格式 [1, 2, 3, 4, 5]或["a", "b", "c", "d"]
            If InStr(strMatch, strSource) > 0 Then
                fnLogecMatch = True
            Else
                fnLogecMatch = False
            End If
            
        Case "range" ' 在一个闭区间内，比如 (1, 10)
            a = Split(strMatch, ",")
            If UBound(a) <> 1 Then
                fnLogecMatch = False
            Else
                iStart = CLng(Right(a(0), Len(a(0)) - 1))
                iEnd = CLng(Left(a(1), Len(a(1)) - 1))
                
                'strRange = ""
                'For i = iStart To iEnd
                '    strRange = strRange & i & ","
                'Next
                
                'If InStr(strRange, strSource) Then
                If CLng(strSource) >= iStart And CLng(strSource) <= iEnd Then
                    fnLogecMatch = True
                Else
                    fnLogecMatch = False
                End If
            End If
            
        Case "ipmatch"
            ' 此时有三种可能的值
            ' 1. Hostname string（如：www.siteview.com）
            ' 2. IP address string（如：192.168.5.123）
            ' 3. Range of nodes using IP address wildcards（如：192.168.5.*或192.168.5.15-29）
            a = Split(strMatch, ".")
            Select Case UBound(a)
                Case 2 ' 是域名格式
                    If InStr(strMatch, strSource) > 0 Then
                        fnLogecMatch = True
                    Else
                        fnLogecMatch = False
                    End If
                Case 3 ' 是IP格式
                    If InStr(strMatch, "*") > 0 Or InStr(strMatch, "-") > 0 Then ' 第三种格式
                        If Right(strMatch, 1) = "*" Then ' 格式是：192.168.5.*
                            b = Split(strSource, ".")
                            aa = a(0) & "." & a(1) & "." & a(2)
                            bb = b(0) & "." & b(1) & "." & b(2)
                            
                            If aa = bb Then
                                fnLogecMatch = True
                            Else
                                fnLogecMatch = False
                            End If
                        Else ' 格式是：192.168.5.10-52
                            a = Split(strSource, ".")
                            b = Split(strMatch, ".")
                            c = Split(b(3), "-")
                            If CLng(a(3)) >= CLng(c(0)) And CLng(a(3)) <= CLng(c(1)) Then
                                fnLogecMatch = True
                            Else
                                fnLogecMatch = False
                            End If
                        End If
                    Else
                        If InStr(strMatch, strSource) > 0 Then
                            fnLogecMatch = True
                        Else
                            fnLogecMatch = False
                        End If
                    End If
                Case Else
                    fnLogecMatch = False
            End Select
        
        Case Else ' 其它情况
            fnLogecMatch = False
    End Select
    
    If Err.Number > 0 Then
            fnLogecMatch = False
    End If

End Function




'---------------------------------------------------------------------------
' 取得以当前年，月，日，时，分，秒的字串
'---------------------------------------------------------------------------
Public Function fn_getYMDHMS()

    dt = Now
    a = DatePart("yyyy", dt)
    b = DatePart("m", dt)
    If b <= 9 Then b = "0" & b
    c = DatePart("d", dt)
    If c <= 9 Then c = "0" & c
    d = DatePart("h", dt)
    If d <= 9 Then d = "0" & d
    e = DatePart("n", dt)
    If e <= 9 Then e = "0" & e
    f = DatePart("s", dt)
    If f <= 9 Then f = "0" & f

    fn_getYMDHMS = a & b & c & d & e & f

End Function


'---------------------------------------------------------------------------
' 查出所有符合条件的数据
' 参数:
'      起始日期
'      终止日期
'      GroupID
'      MonitorID
'      FSO对象
'      strPath LOG文件路径
'
' 返回：生成的包含所有符合条件数据的文件名
'---------------------------------------------------------------------------
Public Function fn_getAllMatchedData(dStart, dEnd, iGroupID, iMonitorID, fnFSO, strPath)

    If IsDate(dStart) And IsDate(dEnd) Then
        If IsNumeric(iGroupID) And IsNumeric(iMonitorID) Then
            ' 生成匹配串
            strMatch = Chr(9) & iGroupID & Chr(9) & iMonitorID & Chr(9)
            'strMatch = "    " & iGroupID & "    " & iMonitorID & "  "

            ' 计算起止日期间的间隔天数
            iFortimes = DateDiff("d", dStart, dEnd) + 1
            ' 如果为同一天，则间隔天数为1
            If CDate(dStart) = CDate(dEnd) Then iFortimes = 1

            dStart = CDate(dStart)

            ' 输出文件
            desFilename = strPath & fn_getYMDHMS & ".log"
            ' 检查输出文件是否存在，如存在，则删除
            If fnFSO.FileExists(desFilename) Then
                fnFSO.DeleteFile desFilename, True
            End If

            Set fout = fnFSO.OpenTextFile(desFilename, 2, True)

            ' 对每天的数据文件进行查找
            For x = 1 To iFortimes

                    ' 生成数据文件名
                    tmpD = DateAdd("d", x - 1, dStart)


                    ' 如果文件名小于10，则对文件进行格式化
                    If Len(tmpD) < 10 Then
                        tmpArr = Split(tmpD, "-")
                        If Len(tmpArr(1)) = 1 Then tmpArr(1) = "0" & tmpArr(1)
                        If Len(tmpArr(2)) = 1 Then tmpArr(2) = "0" & tmpArr(2)

                        'tmpD = tmpArr(0) & "-" & tmpArr(1) & "-" & tmpArr(2)
                        tmpD = Join(tmpArr, "-")
                    End If


                    ' 生成数据文件名
                    strFilename = strPath & tmpD & ".log"

                    If fnFSO.FileExists(strFilename) Then

                            ' 取出数据文件的内容进行匹配
                            Set fin = fnFSO.OpenTextFile(strFilename, 1, True)

                            Do While fin.AtEndOfStream <> True
                                strPerline = fin.ReadLine
                                
'                                arr = Split(strPerline, Chr(9))
'                                If UBound(arr) < 6 Then
'                                Else
'                                    If arr(5) = iGroupID And arr(6) = iMonitorID Then
'                                            fout.WriteLine strPerline
'                                    End If
'                                End If
                                
                                If Len(Trim(strMatch)) > 0 Then
                                    pos = InStr(strPerline, strMatch)
                                    If pos > 0 Then
                                        arr = Split(Left(strPerline, pos), Chr(9))
                                        If UBound(arr) >= 5 Then
                                            fout.WriteLine strPerline
                                        End If
                                    End If
                                End If
                            Loop

                            fin.Close

                    End If
                    'wscript.echo fnFSO.FileExists(strFilename)
                    'wscript.echo strFilename
            Next

            fout.Close
            
            fn_getAllMatchedData = desFilename
        Else
            fn_getAllMatchedData = "error=组ID和监测器ID不正确"
        End If
    Else
        fn_getAllMatchedData = "error=日期不正确"
    End If

End Function


'---------------------------------------------------------------------------
' 主函数，计日期时间串格式化成标准格式，如：2002-09-09 01:01:00
' 参数:
'      strD 日期字串
'
' 返回：返回格式化后的值
'---------------------------------------------------------------------------
Public Function formatDateTimeToCHS(strD)
    strD = Trim(strD)
    
    arr = Split(strD, " ")
    If UBound(arr) = 1 Then ' 含时间
        formatDateTimeToCHS = formatDate(arr(0)) & " " & formatTime(arr(1))
    Else
        formatDateTimeToCHS = formatDate(strD) & " 00:00:00"
    End If
End Function


Public Function formatDate(strD)
    strD = Trim(strD)
    arr = Split(strD, "-")
    If UBound(arr) = 2 Then
        If Len(arr(1)) = 1 Then arr(1) = "0" & arr(1)
        If Len(arr(2)) = 1 Then arr(2) = "0" & arr(2)

        formatDate = Join(arr, "-")
    Else
        formatDate = strD
    End If
End Function


Public Function formatTime(strD)
    strD = Trim(strD)
    arr = Split(strD, ":")
    If UBound(arr) = 2 Then
        If Len(arr(0)) = 1 Then arr(0) = "0" & arr(0)
        If Len(arr(1)) = 1 Then arr(1) = "0" & arr(1)
        If Len(arr(2)) = 1 Then arr(2) = "0" & arr(2)

        formatTime = Join(arr, ":")
    Else
        formatTime = strD
    End If
End Function


'---------------------------------------------------------------------------
' 主函数，计算baseline
' 参数:
'      周期
'      重复次数
'      起始日期
'      GroupID
'      MonitorID
'      计算哪个值，即第几个位置的值
'      strPath LOG文件路径
'
' 返回：生成的包含所有符合条件数据的文件名
'---------------------------------------------------------------------------
Public Function fn_main(strPeriod, strRepeat, strStart, strGroupID, strMonitorID, posValue, strPath)

        ' 检测参数是否符合条件
        If Len(strPeriod) = 0 Or Len(strPeriod) >= 3 Then
            fn_main = "error=周期参数不正确"
            Exit Function
        End If

        If Len(strRepeat) > 2 And (Not IsNumeric(strRepeat)) Then
            fn_main = "error=重复次数不正确"
            Exit Function
        End If

        If Not IsNumeric(posValue) Then
            fn_main = "error=计算值所在位置不正确"
            Exit Function
        Else
            If CInt(posValue) < 9 Or CInt(posValue) > 100 Then
                fn_main = "error=计算值所在位置不正确"
                Exit Function
            End If
        End If
        posValue = CInt(posValue) - 1

        strRepeat = CLng(strRepeat)

        ' 创建FSO对象
        Set fso = CreateObject("Scripting.FileSystemObject")

        ' 创建结果数组
        'dim result(), totalRows
        totalRows = 0

        ' 根据周期计算起止时间
        strPeriod = LCase(strPeriod)
        If strPeriod = "w" Then
            strStart = DateAdd("d", -(Weekday(CDate(Date), vbMonday) - 1), Date)
            strStart2 = DateAdd("d", -7, strStart)
            strEnd2 = DateAdd("d", 7, strStart2)
            strStart = DateAdd("d", -CLng(strRepeat) * 7, strStart)
            strEnd = DateAdd("d", 7 * CLng(strRepeat) - 1, strStart)
            totalRows = 7 * 24

            iPeriod = 7
        Else
            If strPeriod = "m" Then
                strStart = DateAdd("d", -(DatePart("d", CDate(Date)) - 1), Date)
                strStart2 = DateAdd("m", -1, strStart)
                strEnd2 = DateAdd("d", 31, strStart2)
                strStart = DateAdd("m", -CLng(strRepeat), strStart)
                strEnd = DateAdd("d", 31 * CLng(strRepeat), strStart)

                'if datediff("m", strStart, strEnd)=clng(strRepeat) then
                '   strEnd = dateadd("d", -(clng(datepart("d", strEnd))), strEnd)
                'end if

                totalRows = 31 * 24

                iPeriod = 31
            Else
                ' strStart 和 strEnd 是实际的计算天数，比如周期2天数2，strStart和strEnd之间的日期是4
                strEnd = DateAdd("d", -1, CDate(strStart))
                strEnd2 = DateAdd("d", -1, CDate(strStart))
                strStart2 = DateAdd("d", -CLng(strPeriod), CDate(strStart))
                strStart = DateAdd("d", -CLng(strPeriod) * CLng(strRepeat), CDate(strStart))
                totalRows = CLng(strPeriod) * 24

                iPeriod = CLng(strPeriod)
                
                ' strStart2 strEnd2 是存画图用的周期，比如周期2天数2，strStart2 strEnd2之间应该是2
            End If
        End If

        'wscript.echo totalRows
        Dim result()

        ' 重定义结果数组
        ReDim result(totalRows, 7)

        dDateTime = strStart2 & " 00:00:00"

        For xx = 1 To totalRows
            result(xx, 1) = dDateTime

            dDateTime = DateAdd("h", 1, dDateTime)

            result(xx, 2) = 0
            result(xx, 3) = -1  ' 均方差值
            result(xx, 4) = -1  ' 平均值
            result(xx, 5) = 0   ' 符合条件的值的个数
            result(xx, 6) = 0   ' 用于计算平均值的列
            result(xx, 7) = 0   ' 用于计算均方差值的列
        Next


        Dim arrMatchDate()
        ReDim arrMatchDate(totalRows, strRepeat)

        For xx = 1 To totalRows
            newCompareDate = result(xx, 1)
            For jj = 1 To strRepeat
                arrMatchDate(xx, jj) = formatDateTimeToCHS(newCompareDate)
                newCompareDate = DateAdd("d", iPeriod, newCompareDate)
            Next
        Next

        'for xx=1 to totalRows
        '   for jj=1 to strRepeat
        '       wscript.echo arrMatchDate(xx, jj)
        '   next
        'next
        'wscript.quit

        ' 首选选中所有符合条件的数据
        strAllDataFilename = fn_getAllMatchedData(strStart, strEnd, strGroupID, strMonitorID, fso, strPath)
        If InStr(strAllDataFilename, "error=") > 0 Then
            fn_main = "error=数据文件生成错误！"
            Exit Function
        End If
        'wscript.echo strAllDataFilename

        'for xx=1 to totalRows
        '   wscript.echo result(xx, 1) & "---" & result(xx, 4) & "---" & result(xx, 3)
        'next

        'exit function

        'oldtimer = timer

        If fso.FileExists(strAllDataFilename) Then

                ' 取出数据文件的内容计算平均值
                Set fin = fso.OpenTextFile(strAllDataFilename, 1, True)

                Do While fin.AtEndOfStream <> True
                    strPerline = fin.ReadLine

                    arrValue = Split(strPerline, Chr(9))

                    If UBound(arrValue) >= posValue Then
                        For xx = 1 To totalRows
                            For yy = 1 To strRepeat
                                'if datediff("h", newCompareDate, cdate(arrValue(0)))=0 then
                                If Left(arrMatchDate(xx, yy), 13) = Left(strPerline, 13) Then
                                    result(xx, 6) = result(xx, 6) + CDbl(arrValue(posValue))
                                    result(xx, 5) = result(xx, 5) + 1
                                End If
                            Next
                        Next
                    End If
                Loop

                fin.Close
                'wscript.quit
                

                ' 计算平均值
                For xx = 1 To totalRows
                   If result(xx, 5) > 0 Then
                       result(xx, 4) = CLng((result(xx, 6) / result(xx, 5)) * 100) / 100
                   Else
                       result(xx, 4) = -1
                   End If
                Next


                ' 计算均方差
                Set fin = fso.OpenTextFile(strAllDataFilename, 1, True)

                Do While fin.AtEndOfStream <> True
                    strPerline = fin.ReadLine

                    arrValue = Split(strPerline, Chr(9))

                    If UBound(arrValue) >= posValue Then
                        For xx = 1 To totalRows
                            For yy = 1 To strRepeat
                                'if datediff("h", newCompareDate, cdate(arrValue(0)))=0 then
                                If Left(arrMatchDate(xx, yy), 13) = Left(strPerline, 13) Then
                                    result(xx, 7) = result(xx, 7) + (CDbl(arrValue(posValue)) - result(xx, 4)) ^ 2
                                End If
                            Next
                        Next
                    End If
                Loop

                fin.Close

                ' 计算均方差
                'for xx=1 to totalRows
                '   if result(xx, 5)>0 then
                '       result(xx, 3) = sqr(result(xx, 7))
                '   else
                '       'result(xx, 3) = -1
                '   end if
                'next



                ' 生成最后的输入文件 GroupID_MonitorID.base
                outFilename = strPath & strGroupID & "_" & strMonitorID & ".base"
        
                If fso.FileExists(outFilename) Then
                    fso.DeleteFile outFilename, True
                End If
        
                Set fout = fso.OpenTextFile(outFilename, 2, True)
        
                ' 输入参数定义部分
                fout.WriteLine "[parameters]"
                fout.WriteLine "period=" & strPeriod
                fout.WriteLine "repeat=" & strRepeat
                fout.WriteLine "GroupID=" & strGroupID
                fout.WriteLine "MonitorID=" & strMonitorID
                fout.WriteLine "posValue=" & posValue + 1
'                fout.WriteLine "startdate=" & strStart
'                fout.WriteLine "enddate=" & strEnd
                fout.WriteLine "startdate=" & strStart2 '& " 00:00:00"
                fout.WriteLine "enddate=" & strEnd2 '& " 23:59:59"
                fout.WriteLine "thisfilecreate=" & Now
                fout.WriteLine ""
        
                ' 输出平均值
                Dim tmpAVG As Double
                fout.WriteLine "[avg]"
                For xx = 1 To totalRows
                    If result(xx, 5) > 0 Then
                        'tmpAVG = CLng((result(xx, 6) / result(xx, 5)) * 100) / 100
                        tmpAVG = result(xx, 4)
                        If tmpAVG < 1 Then tmpAVG = "0" & tmpAVG
                        fout.WriteLine formatDateTimeToCHS(result(xx, 1)) & "=" & tmpAVG
                    Else
                        'result(xx, 4) = -1
                        fout.WriteLine formatDateTimeToCHS(result(xx, 1)) & "=-1"
                    End If
                Next
                fout.WriteLine ""
        
                ' 输出均方差值
                fout.WriteLine "[avg_fc]"
                For xx = 1 To totalRows
                    If result(xx, 5) > 0 Then
                        tmpAVG = CLng((Sqr(result(xx, 7) / result(xx, 5))) * 100) / 100
                        If tmpAVG < 1 Then tmpAVG = "0" & tmpAVG
                        fout.WriteLine formatDateTimeToCHS(result(xx, 1)) & "=" & tmpAVG
                    Else
                        'result(xx, 4) = -1
                        fout.WriteLine formatDateTimeToCHS(result(xx, 1)) & "=-1"
                    End If
                Next
                fout.WriteLine ""
        
                fout.Close
        
                'Erase arrValue
                'Erase arrMatchDate
                'Erase result

                fso.DeleteFile (strAllDataFilename)
                
                Set fout = Nothing
                Set fin = Nothing
                Set fso = Nothing
                
                fn_main = outFilename
                
        Else
            fn_main = "error=所有符合条件的数据文件不存在！"
        End If

        'for xx=1 to totalRows
        '   wscript.echo result(xx, 1) & "===" & result(xx, 6) & "===" & result(xx, 5) & "===" & result(xx, 4) & "===" & result(xx, 3)
        'next

        'wscript.echo strStart
        'wscript.echo strEnd

End Function



'======================================================================================================
' 功能 : 取Sybase数据库参数
'
' 参数 :  dsource 数据源名称
'        uid 用户名
'        pwd 密码
'
' 返回 :
'       last_run 上次运行 sp_monitor 的时间。
'       current_run 本次运行 sp_monitor 的时间。
'       seconds sp_monitor 自运行以来所经过的时间（按秒计）。
'       cpu_busy 服务器计算机的 CPU 处理 SQL Server 工作所用的时间（按秒计）。
'       io_busy SQL Server 在输入和输出操作上花费的时间（按秒计）。
'       idle SQL Server 已空闲的时间（按秒计）。
'       packets_received SQL Server 读取的输入数据包数。
'       packets_sent SQL Server 写入的输出数据包数。
'       packet_errors SQL Server 在读取和写入数据包时遇到的错误数。
'       total_read SQL Server 读取的次数。
'       total_write SQL Server 写入的次数。
'       total_errors SQL Server 在读取和写入时遇到的错误数。
'       connections 登录或尝试登录 SQL Server 的次数。
'======================================================================================================
Public Function getSybase(dsource, uid, pwd)

        On Error Resume Next
        
        Set cnn = CreateObject("adodb.connection")
        cnn.Open dsource, uid, pwd
        
        If Err.Number > 0 Then
            getSybase = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
            Exit Function
        End If
        Err.Clear
        
        cnn.Execute "sp_monitor"
        
        Set rs = CreateObject("adodb.recordset")
        'rs.open "select * from spt_monitor", cnn, 1, 1
        rs.Open "sp_monitor", cnn, 1, 1
        
        'wscript.echo rs.recordcount & vbcrlf
        If rs.EOF And rs.BOF Then
            'wscript.echo "no data"
        Else
            'wscript.echo "has data"
            'rs.movefirst
            Do While Not rs.EOF
                'wscript.echo rs(0) & vbcrlf
                olddate = rs(0)
                'wscript.echo rs(1) & vbcrlf
                newdate = rs(1)
                'wscript.echo rs(2) & vbcrlf
                rs.MoveNext
            Loop
        
            Set rs1 = rs.NextRecordset
            Do While Not rs1.EOF
                'wscript.echo rs1(0) & vbcrlf
                'wscript.echo rs1(1) & vbcrlf
                'wscript.echo rs1(2) & vbcrlf
                cpu_busy = trimkh(rs1(0))
                io_busy = trimkh(rs1(1))
                idle = trimkh(rs1(2))
        
                rs1.MoveNext
            Loop
        
            Set rs2 = rs1.NextRecordset
            Do While Not rs2.EOF
                'wscript.echo rs2(0) & vbcrlf
                'wscript.echo rs2(1) & vbcrlf
                'wscript.echo rs2(2) & vbcrlf
                packets_received = trimkh(rs2(0))
                packets_sent = trimkh(rs2(1))
                packet_errors = trimkh(rs2(2))
        
                rs2.MoveNext
            Loop
        
            Set rs3 = rs2.NextRecordset
            Do While Not rs3.EOF
                'wscript.echo rs3(0) & vbcrlf
                'wscript.echo rs3(1) & vbcrlf
                'wscript.echo rs3(2) & vbcrlf
                'wscript.echo rs3(3) & vbcrlf
                total_read = trimkh(rs3(0))
                total_write = trimkh(rs3(1))
                total_errors = trimkh(rs3(2))
                connections = trimkh(rs3(3))
                
                rs3.MoveNext
            Loop
        
        End If
        
        'wscript.echo cpu_busy
        'wscript.echo trimkh(cpu_busy)
        
        rs.Close
        rs1.Close
        rs2.Close
        rs3.Close
        cnn.Close
        
        Set rs = Nothing
        Set rs1 = Nothing
        Set rs2 = Nothing
        Set rs3 = Nothing
        Set cnn = Nothing
        
        Dim per As Integer
        
        per = DateDiff("n", CDate(olddate), CDate(newdate))
        If per = 0 Then per = 1
        
        If Err.Number > 0 Then
            getSybase = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Else
            getSybase = "cpu_busy=" & (cpu_busy / per) & "$io_busy=" & (io_busy / per) & "$packets_received=" & (packets_received / per) & "$packets_sent=" & (packets_sent / per) & "$packet_errors=" & (packet_errors / per) & "$total_read=" & (total_read / per) & "$total_write=" & (total_write / per) & "$total_errors=" & (total_errors / per) & "$connections=" & (connections / per) & "$"
        End If
    
End Function



'======================================================================================================
' 功能 : 正则表达式匹配
'
' 参数 :  reg 正则表达式
'        strContent 要匹配的内容
'
' 返回 :
'        错误时：fnRegularExpression = Error
'        正常时：fnRegularExpression=555
'======================================================================================================
Public Function fnRegularExpression(reg, strContent)

        On Error Resume Next
        Dim regEx As New RegExp, matches As Object
        
        
        If Len(reg) > 1 Then
            If Left(reg, 1) = "/" And Right(reg, 1) = "/" Then
                reg = CStr(Mid(reg, 2, Len(reg) - 2))
            Else
                reg = CStr(reg)
            End If
        End If
        
        
        regEx.Pattern = reg   ' 设置模式。
        regEx.IgnoreCase = True  ' 设置是否区分字符大小写。
        regEx.Global = True   ' 设置全局可用性。
        
        Set matches = regEx.Execute(strContent)   ' 执行匹配
        
        If Err.Number > 0 Then
            'fnRegularExpression = "match=0$"
            fnRegularExpression = "Error"
        Else
            'fnRegularExpression = "match=" & matches.count & "$"
            fnRegularExpression = matches.count
        End If
    
End Function


'======================================================================================================
' 功能 : Log 文件匹配
'
' 参数 : fname  文件名
'        parten  正则表达式，或不包含正则表达式特殊字符的字串
'        direct  LOG文件追加数据方向（前，后） 向前时参数为 head, 向后时参数为 tail
'        lasttimerow 本次监测起始行号（即上次监测完后返回值 totaline）
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：totalmatches=8&totalline=218$
'        无取得值时：getLogFile = 空串
'======================================================================================================
Public Function getLogFile(fname, parten, direct, lasttimerow)
    On Error Resume Next
    
    Dim fso As New FileSystemObject
    Dim objF As File

    If fso.FileExists(fname) Then
        Set objF = fso.GetFile(fname)
        
        'getLogFile = "size=" & objF.Size & "$createdate=" & DateDiff("s", objF.DateCreated, Now) & "$ModifyDate=" & CDate(objF.DateLastModified) & "$"
        
        ' 处理 parten(正则表达式），如果是普通匹配则生成VB形式的正则表达式串，如果是JS或PERL类似的表格式，转换成VB形式的
        If Len(parten) > 1 Then
            If Left(parten, 1) = "/" And Right(parten, 1) = "/" Then
                parten = CStr(Mid(parten, 2, Len(parten) - 2))
            Else
                parten = CStr(parten)
            End If
        End If
        
        
        Dim objStream As TextStream, strAll As String, strThisTimeContent As String, totalLine As Long
        Set objStream = objF.OpenAsTextStream(ForReading)
        strAll = objStream.ReadAll  ' 取所有内容
        arrEachLine = Split(strAll, vbCrLf) ' 分成单行
        totalLine = UBound(arrEachLine)
        
        
        If Not IsNumeric(lasttimerow) Then
            lasttimerow = 1
        End If
        
        
        Dim i As Long, regEx As New RegExp, matches As Object
        
        regEx.Pattern = parten   ' 设置模式。
        regEx.IgnoreCase = True  ' 设置是否区分字符大小写。
        regEx.Global = True   ' 设置全局可用性。
        
        
        ' 判断追加方式，并转入相关的操作
        Dim resultMatch As Long
        Select Case direct
            Case "head"
            
                ' 生成这次检测的内容串
                For i = 0 To totalLine - lasttimerow - 1
                    strThisTimeContent = strThisTimeContent & arrEachLine(i) & vbCrLf
                Next
                
            Case "tail"
                lasttimerow = lasttimerow - 1
                
                ' 生成这次检测的内容串
                For i = lasttimerow To UBound(arrEachLine) - 1
                    strThisTimeContent = strThisTimeContent & arrEachLine(i) & vbCrLf
                Next
                
            Case Else
        End Select
        
        Set matches = regEx.Execute(strThisTimeContent)   ' 执行匹配
        resultMatch = matches.count
        
        If Err.Number > 0 Then
            getLogFile = "error= ! Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Else
            getLogFile = "totalmatches=" & resultMatch & "&totalline=" & totalLine & "$"
        End If
    Else
        getLogFile = "error=文件不存在$"
    End If
    
    Set objF = Nothing
    Set fso = Nothing
End Function


'======================================================================================================
' 功能 : 取指定文件的日期,大小
'
' 参数 : f  要检测的文件(包括路径)
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getFileInfo = "size=121078$createdate=25749647$"
'        无取得值时：getFileInfo = 空串
'======================================================================================================
Public Function getFileInfo(f)
    On Error Resume Next
    
    Dim fso As New FileSystemObject
    Dim objF As File
    
    If fso.FileExists(f) Then
        Set objF = fso.GetFile(f)
        
        getFileInfo = "size=" & objF.Size & "$createdate=" & DateDiff("s", objF.DateCreated, Now) & "$ModifyDate=" & CDate(objF.DateLastModified) & "$"
        
        If Err.Number > 0 Then
            getFileInfo = "error= ! Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        End If
    Else
        getFileInfo = "error=文件不存在$"
    End If
    Set objF = Nothing
    Set fso = Nothing
End Function



'======================================================================================================
' 功能 : 取指定目录下的文件总数,生存日期,文件总大小
'
' 参数 : f  要检测的目录(包括路径)
'        match 匹配字串,对目录内的文件根据match值进行文件名匹配,包含match内容的文件才计数
'               match只影响 totalfiles 参数, 不影响后两个, 后两个指目录的大小(包含目录内所有文件+子目录)和目录存在时间
'               当参f输入成：c:\ 这种形式时，目录生存时间取不到
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getDirInfo = "totalfiles=1$size=28691264$createdate=2439122$"
'        无取得值时：getDirInfo = 空串
'======================================================================================================
Public Function getDirInfo(f, match)
    'On Error Resume Next
    
    Dim fso As New FileSystemObject
    Dim objF As File, objD As Folder, totalsize  As Double
    
    If fso.FolderExists(f) Then
        Set objD = fso.GetFolder(f)
        Set fc = objD.Files
        Dim totalfiles As Long
       If Len(match) = 0 Then
            ' 不对文件名进行匹配
            totalfiles = fc.count
            If Err.Number > 0 Then
                totalfiles = 0
            End If
            Err.Clear
        Else
            ' 对文件名进行匹配
            totalfiles = 0
            
            For Each obj In fc
                If InStr(obj.Name, match) Then
                    totalfiles = totalfiles + 1
                End If
            Next
        End If
        
'        Dim tmpsize As Double
'        If Len(Trim(f)) <= 3 Then  ' 目录
'            For Each x In objD.Files
'                tmpsize = tmpsize + x.Size
'            Next
'            totalsize = tmpsize
'            totalsize = totalsize / 1024
'            totalsize = CLng(totalsize * 100) / 100
'        Else
            totalsize = objD.Size
            totalsize = totalsize / 1024
            totalsize = CLng(totalsize * 100) / 100
'        End If
        
        getDirInfo = "totalfiles=" & totalfiles & "$size=" & totalsize
        
        If Len(Trim(f)) <= 3 Then
            getDirInfo = getDirInfo & "$createdate=0$"
        Else
            getDirInfo = getDirInfo & "$createdate=" & DateDiff("s", objD.DateCreated, Now) & "$"
        End If
        
        If Err.Number > 0 Then
            getDirInfo = "error= ! Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        End If
    Else
        getDirInfo = "error=目录不存在$"
    End If
    Set objF = Nothing
    Set objD = Nothing
    Set fso = Nothing
End Function



'======================================================================================================
' 功能 : 取系统信息,返回版本号
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'        condition 条件串,恒为: " Win32_OperatingSystem where Primary=true"
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getSystemInfo = 5.0.2195
'        无取得值时：getSystemInfo = 空串
'======================================================================================================
Public Function getSystemInfo(a, b, c, condition)
    On Error Resume Next
    
    Dim locator As SWbemLocator, service As SWbemServices, objsets As SWbemObjectSet, obj As SWbemObject
    
    Set locator = CreateObject("WbemScripting.SWbemLocator")
    
    If a = "" And b = "" And c = "" Then
        Set service = locator.ConnectServer()
    Else
        Set service = locator.ConnectServer(a, "root\cimv2", b, c)
    End If
    
    Set objsets = service.ExecQuery("select * from " & condition)
    
    Dim objp As Object, m As Object, str As String, f As Double, t As Double
    For Each obj In objsets
        'Set objp = obj.Properties_
        'For Each m In objp
        '    str = str & m.Name & "===" & m.Value & vbCrLf
        'Next
        str = obj.Version
    Next
    
    If Err.Number <> 0 Then
        str = "Error Code: " & Err.Number & vbCrLf
        str = str & "Error Source: " & Err.Source & vbCrLf
        str = str & "Error Description: " & Err.Description & vbCrLf
    End If
    
    getSystemInfo = str
    
End Function




'======================================================================================================
' 功能 : 取PerfClass名称
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getPerfClasses = 符合条件的串
'        无取得值时：getPerfClasses = 空串
'======================================================================================================
Public Function getPerfClasses(a, b, c)
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
        'getPerfClasses = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getPerfClasses = "error= service error! Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim objset As SWbemObjectSet
    Set objset = service.SubclassesOf("Win32_PerfRawData")
    
    Dim x As SWbemObject, str As String
    
    For Each x In objset
        str = str & x.Path_.Class & vbCrLf
    Next
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getPerfClasses = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getPerfClasses = "error= get Date error! Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        getPerfClasses = str
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function



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
    Set objset = service.ExecQuery("select state from Win32_Service where name='w3svc'")
    If objset.count = 0 Then
        alertRestartService = "error=指定主机无" & d & "服务!"
        Exit Function
    End If
    
    
    Dim objproc As Object
    ' 创建进程类
    Set objproc = service.Get("Win32_Process")
    
    Dim str As String
    str = "ok"
    
    ' 关闭指定服务
    result = objproc.Create("net stop " & d, Null, Null, ProcessID)
    
    ' 循环测试指定服务是否关闭了?
    Do While True
        Set objset = service.ExecQuery("select state from Win32_Service where name='" & d & "'")
        
        For Each tmp In objset
            If LCase(tmp.State) = "stopped" Then
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
    If objset.count = 0 Then
        alertRestartIIS = "error=指定主机无WWW服务!"
        Exit Function
    End If
    
    
    Dim objproc As Object
    ' 创建进程类
    Set objproc = service.Get("Win32_Process")
    
    Dim str As String
    str = "ok"
    
    ' 关闭IIS
    result = objproc.Create("net stop w3svc", Null, Null, ProcessID)
    
    ' 循环测试IIS是否关闭了?
    Do While True
        Set objset = service.ExecQuery("select state from Win32_Service where name='w3svc'")
        
        For Each tmp In objset
            If LCase(tmp.State) = "stopped" Then
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
    result = objset.Create(str, Null, Null, ProcessID)
    
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
    If objset.count > 0 Then
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
    If objset.count > 0 Then
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
' 功能 : 取ASP Server参数
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getASPServer = 符合条件的串
'        无取得值时：getASPServer = 空串
'======================================================================================================
Public Function getASPServer(a, b, c)
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
        getASPServer = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    
    '-------------------------------------------------------------------------------------------------------------
    ' 取ASP Server的数据
    '-------------------------------------------------------------------------------------------------------------
    Dim PerfFreq As Long
    Dim aa(11, 2)
    For i = 1 To 5
        For j = 0 To 1
            aa(i, j) = 0
        Next
    Next
    
    PerfFreq = 1
    ' 在第一次取数据前清掉ERR对象
    Err.Clear
    
    ' flag标志用于确定是否有Win32_PerfRawData_ASP_ActiveServerPages对象,如果没有此对象,
    flag = False
    
    Dim o As SWbemObject, objsets As SWbemObjectSet
    
    ' 取第一次ASP Server数据
    sql = "select * from Win32_PerfRawData_ASP_ActiveServerPages"
    Set objsets = service.ExecQuery(sql)
    
    ' 当没有类 Win32_PerfRawData_ASP_ActiveServerPages 时,调用objsets.count会出错.但objsets仍是个对象,颇麻烦
    If objsets.count > 0 Then
        If Err.Number = 0 Then
            For Each o In objsets
                aa(1, 0) = o.ErrorsDuringScriptRuntime
                aa(2, 0) = o.ErrorsFromASPPreprocessor
                aa(3, 0) = o.ErrorsFromScriptCompilers
                aa(4, 0) = o.ErrorsPerSec
                aa(5, 0) = o.RequestBytesInTotal
                aa(6, 0) = o.RequestsFailedTotal
                aa(7, 0) = o.RequestsTotal
                aa(8, 0) = o.SessionsCurrent
                aa(9, 0) = o.RequestsSucceeded
                aa(10, 0) = o.TransactionsTotal
                aa(11, 0) = o.Timestamp_PerfTime
            Next
        Else
            flag = True
        End If
    End If
    
    Err.Clear
    
    ' 取第二次ASP Server数据
    sql = "select * from Win32_PerfRawData_ASP_ActiveServerPages"
    Set objsets = service.ExecQuery(sql)
    
    If objsets.count > 0 Then
        If Err.Number = 0 Then
            For Each o In objsets
                aa(1, 1) = o.ErrorsDuringScriptRuntime
                aa(2, 1) = o.ErrorsFromASPPreprocessor
                aa(3, 1) = o.ErrorsFromScriptCompilers
                aa(4, 1) = o.ErrorsPerSec
                aa(5, 1) = o.RequestBytesInTotal
                aa(6, 1) = o.RequestsFailedTotal
                aa(7, 1) = o.RequestsTotal
                aa(8, 1) = o.SessionsCurrent
                aa(9, 1) = o.RequestsSucceeded
                aa(10, 1) = o.TransactionsTotal
                aa(11, 1) = o.Timestamp_PerfTime
                PerfFreq = o.Frequency_PerfTime
            Next
        Else
            flag = True
        End If
    End If
    
    Err.Clear
   
    ' 错误处理
    If Err.Number <> 0 Then
        'getNetworkInterfaceNames = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getASPServer = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        If flag Then
            getASPServer = "error=无ASP计数器$"
        Else
            getASPServer = "ErrorsDuringScriptRuntime=" & aa(1, 1) & "$ErrorsFromASPPreprocessor=" & aa(2, 1) & "$ErrorsFromScriptCompilers=" & aa(3, 1) & "$ErrorsPerSec=" & CLng((aa(4, 1) - aa(4, 0)) / ((aa(11, 1) - aa(11, 0)) / PerfFreq)) & "$RequestBytesInTotal=" & aa(5, 1) & "$RequestsFailedTotal=" & aa(6, 1) & "$RequestsTotal=" & aa(7, 1) & "$SessionsCurrent=" & aa(8, 1) & "$RequestsSucceeded=" & aa(9, 1) & "$TransactionsTotal=" & aa(10, 1) & "$"
        End If
        'getASPServer = "BytesReceivedPerSec=" & CLng((aa(1, 1) - aa(1, 0)) / ((aa(5, 1) - aa(5, 0)) / PerfFreq)) & "$BytesSentPerSec=" & CLng((aa(2, 1) - aa(2, 0)) / ((aa(5, 1) - aa(5, 0)) / PerfFreq)) & "$PacketsErrors=" & (aa(3, 1) + aa(4, 1)) & "$"
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function


Private Function WriteLogFile(strMsg As String)
   Dim fso As FileSystemObject
   Dim MyFile As TextStream
   Set fso = CreateObject("Scripting.FileSystemObject")
   If (fso.FileExists("c:\temp\prj_wmi.log") = False) Then
   Set MyFile = fso.CreateTextFile("c:\temp\prj_wmi.log", False)
   Else
   Set MyFile = fso.OpenTextFile("c:\temp\prj_wmi.log", ForAppending)
   End If
   MyFile.WriteLine (strMsg)
   MyFile.Close
End Function
'======================================================================================================
' 功能 : 取NetworkInterface参数
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'        d  网卡名称，如: 3Com EtherLink PCI
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getNetworkInterface = 符合条件的串
'        无取得值时：getNetworkInterface = 空串
'======================================================================================================
Public Function getNetworkInterface(a, b, c, d)
    On Error Resume Next
    
    
    Dim locator As New SWbemLocator
    Dim service As SWbemServices
    
    If Len(a) = 0 And Len(b) = 0 And Len(c) = 0 Then
        Set service = locator.ConnectServer
    Else
        WriteLogFile "Dest server's IP is " + a
        WriteLogFile "User name is " + b
        WriteLogFile "Password is " + c
        WriteLogFile "Network is " + d
        Set service = locator.ConnectServer(a, "root\cimv2", b, c)
    End If
    
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getNetworkInterfaceNames = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getNetworkInterface = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    
    '-------------------------------------------------------------------------------------------------------------
    ' 取NetworkInterface的数据
    '-------------------------------------------------------------------------------------------------------------
    Dim PerfFreq As Double
    Dim aa(5, 2)
    For i = 1 To 5
        For j = 0 To 1
            aa(i, j) = 0
        Next
    Next
    
    PerfFreq = 1
    
    Dim o As SWbemObject, objsets As SWbemObjectSet
    
    ' 取第一次NetworkInterface数据
    sql = "select * from Win32_PerfRawData_Tcpip_NetworkInterface where Name='" & d & "'"
    Set objsets = service.ExecQuery(sql)

    If objsets.count > 0 Then
        For Each o In objsets
            aa(1, 0) = o.BytesReceivedPerSec
            aa(2, 0) = o.BytesSentPerSec
            aa(3, 0) = o.PacketsOutboundErrors
            aa(4, 0) = o.PacketsReceivedErrors
            aa(5, 0) = o.Timestamp_PerfTime
        Next
    End If
    
    ' 取第二次NetworkInterface数据
    sql = "select * from Win32_PerfRawData_Tcpip_NetworkInterface where Name='" & d & "'"
    Set objsets = service.ExecQuery(sql)
    
    If objsets.count > 0 Then
        For Each o In objsets
            aa(1, 1) = o.BytesReceivedPerSec
            aa(2, 1) = o.BytesSentPerSec
            aa(3, 1) = o.PacketsOutboundErrors
            aa(4, 1) = o.PacketsReceivedErrors
            aa(5, 1) = o.Timestamp_PerfTime
            PerfFreq = CDbl(o.Frequency_PerfTime)
        Next
    End If
    
    'Print "This is test"
    WriteLogFile "Setup Monitor result"
    ' 错误处理
    If (Err.Number = 0) Then
        'getNetworkInterfaceNames = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getNetworkInterface = "BytesReceivedPerSec=" & CLng((aa(1, 1) - aa(1, 0)) / ((aa(5, 1) - aa(5, 0)) / PerfFreq)) & "$BytesSentPerSec=" & CLng((aa(2, 1) - aa(2, 0)) / ((aa(5, 1) - aa(5, 0)) / PerfFreq)) & "$PacketsErrors=" & (aa(3, 1) + aa(4, 1)) & "$"
    Else
        getNetworkInterface = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    End If
    
    WriteLogFile "Network card monitor result :" + getNetworkInterface
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function



'======================================================================================================
' 功能 : 取NetworkInterface名称
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getNetworkInterfaceNames = 符合条件的串
'        无取得值时：getNetworkInterfaceNames = 空串
'======================================================================================================
Public Function getNetworkInterfaceNames(a, b, c)
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
        getNetworkInterfaceNames = "error= service error! Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim objset As SWbemObjectSet
    Set objset = service.ExecQuery("select * from Win32_PerfRawData_Tcpip_NetworkInterface")
    
    Dim x As SWbemObject, str As String
    str = ""
    If objset.count > 0 Then
        If Err.Number = 0 Then
            For Each x In objset
                str = str & x.Name & "?"
            Next
        End If
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getNetworkInterfaceNames = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getNetworkInterfaceNames = "error= get Date error! Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        getNetworkInterfaceNames = str
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function


'======================================================================================================
' 功能 : 取进程名称列表
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getProcessNames = 符合条件的串
'        无取得值时：getProcessNames = 空串
'======================================================================================================
Public Function getProcessNames(a, b, c)
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
        'getProcessNames = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getProcessNames = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim objset As SWbemObjectSet
    Set objset = service.ExecQuery("select * from Win32_Process")
    
    Dim x As SWbemObject, str As String
    str = ""
    If objset.count > 0 Then
        For Each x In objset
            str = str & x.Name & "?"
        Next
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getProcessNames = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getProcessNames = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        getProcessNames = str
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function


'======================================================================================================
' 功能 : 取进程名称列表(用性能计数器类取数据且返回参数不同)
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getProcessNames = 符合条件的串
'        无取得值时：getProcessNames = 空串
'======================================================================================================
Public Function getProcessNamesNew(a, b, c)
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
        getProcessNamesNew = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim objset As SWbemObjectSet
    Set objset = service.ExecQuery("select * from Win32_PerfRawData_PerfProc_Process")
    
    Dim x As SWbemObject, str As String
    str = ""
    If objset.count > 0 Then
        For Each x In objset
            ' 下面这句保留, 进程名称有相同的情况, 应该以进程ID来区分, 但一开始没这样做只好继续按名称来
            'str = str & x.IDProcess & "#" & x.Name & " (" & x.IDProcess & ")" & "?"
            str = str & x.Name & "#" & x.Name & " (" & x.IDProcess & ")" & "?"
            'str = str & x.Name & "?"
        Next
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        getProcessNamesNew = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        getProcessNamesNew = str
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function



'======================================================================================================
' 功能 : 取指定进程参数
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'        d  指定进程的名称，比如：foxmail.exe
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：ProcessInfo = 符合条件的串
'        无取得值时：ProcessInfo = 空串
'======================================================================================================
Public Function getProcessInfo(a, b, c, d)
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
        'getProcessInfo = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getProcessInfo = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim objset As SWbemObjectSet
    Set objset = service.ExecQuery("select * from Win32_Process where name='" & d & "'")
    If Err.Number <> 0 Then
        'getProcessInfo = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getProcessInfo = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim x As SWbemObject, str As String, objp As Object, m As Object
    
    str = ""
    Dim processcount As Integer, threadcount As Integer, workingsetsize As Long
    If objset.count > 0 Then
        For Each x In objset
            threadcount = threadcount + x.threadcount
            workingsetsize = workingsetsize + CLng(x.workingsetsize) / 1024
            ' 取所有参数
            'Set objp = x.Properties_
            'For Each m In objp
            '    str = str & m.Name & "===" & m.Value & "$"
            'Next
        Next
    End If
    
    ' 错误处理
    'If Err.Number <> 0 Then
        'getProcessInfo = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
    '    getProcessInfo = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    'Else
        getProcessInfo = "$ProcessCount=" & objset.count & "$ThreadCount=" & threadcount & "PercentProcessorTime=" & workingsetsize & "$" & "$WorkingSet=" & workingsetsize & "$"
        'getProcessInfo = "PercentProcessorTime=" & arrResult(resultRow, 7) & "$ProcessCount=" & processcount & "$ThreadCount=" & arrResult(resultRow, 5) & "$WorkingSet=" & arrResult(resultRow, 6) & "$"
   
    'End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function


'======================================================================================================
' 功能 : 取指定进程参数(用性能计数器类取数据且返回参数不同)
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'        d  进程名称，注意是不带exe扩展名的，比如 Foxmail.exe 应该是 Foxmail
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：ProcessInfoNew = 符合条件的串
'        无取得值时：ProcessInfoNew = 空串
'
' 注意：
'       在测本机时，发现有时不能正确取得数据，简单讲就是时好时坏。不知道是哪里有问题。所在里面用了一个
'       strName 来判断是否能正常取得数据。测远程时没遇到这种现象。至于原因有可能和各机器的配置有关系，
'       但无法确定具体原因。
'
'       另外一个要注意的地方是，进程名称有可能是相同的，比如都叫 svhost，最好通过进程ID来区分
'       但进程ID变化，重启动一次就都变了，所以也不适合。因此本函数保存下所有符合指定名称的进程
'       然后返回%Process Time是最大的那个进程
'======================================================================================================
Public Function getProcessInfoNew(a, b, c, d)
    '-------------------------------------------------------------------------------------------------------------
    ' 取Process的数据
    
    ' 参数列表:
    '   名称                        CounterType                     计算方法
    '   PercentProcessorTime    PERF_100NSEC_TIMER              100*(X1-X0)/(Y1-YO) Y是参数 Timestamp_Sys100NS X是取出值
    '   ThreadCount             PERF_COUNTER_RAWCOUNT           直接是取出值
    '   WorkingSet              PERF_COUNTER_LARGE_RAWCOUNT     直接是取出值
    '   ElapsedTime             PERF_ELAPSED_TIME               (Y-X)/TB  Y是PerfTime  X取出数据  TB是Time base(PerfFreq)
    '-------------------------------------------------------------------------------------------------------------
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
        getProcessInfoNew = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim objset As SWbemObjectSet
    
    Dim x As SWbemObject, str As String, objp As Object, m As Object
    
    str = ""
    Dim x0 As Double, y0 As Double, x1 As Double, y1 As Double, strName As String, count As Integer
    
    x0 = 0#
    y0 = 0#
    x1 = 0#
    y1 = 1#
    
    Dim arrResult()  ' 定义一个动态数组保存所有取得的值，因为同一名称的进程可能会有多个，所以要都保存下来。
    Counter = 1
    
    ' 第一次取数
'    Set objset = service.ExecQuery("select * from Win32_PerfRawData_PerfProc_Process where IDProcess=" & d & " and name<>'_Total'")
    Set objset = service.ExecQuery("select * from Win32_PerfRawData_PerfProc_Process where Name='" & d & "'")
    If objset.count > 0 Then
        ReDim arrResult(objset.count, 7)
        For Each o In objset
            arrResult(Counter, 1) = o.PercentProcessorTime
            'x0 = o.PercentProcessorTime
            arrResult(Counter, 2) = o.Timestamp_Sys100NS
            'y0 = o.Timestamp_Sys100NS
            strName = o.Name
            Counter = Counter + 1
        Next
    End If
    
    ' 暂停5秒
    Sleep 900
    
    Dim threadcount As Long, WorkingSet As Long, perftime As Long
    Dim processcount As Long
    processcount = 0
    ' 第二次取数
'    Set objset = service.ExecQuery("select * from Win32_PerfRawData_PerfProc_Process where IDProcess=" & d & " and name<>'_Total'")
    Set objset = service.ExecQuery("select * from Win32_PerfRawData_PerfProc_Process where Name='" & d & "'")
    If objset.count > 0 Then
        Counter = 1
        For Each o In objset
            arrResult(Counter, 3) = o.PercentProcessorTime
            'x1 = o.PercentProcessorTime
            arrResult(Counter, 4) = o.Timestamp_Sys100NS
            'y1 = o.Timestamp_Sys100NS
        
            arrResult(Counter, 5) = o.threadcount
            'threadcount = o.threadcount
            arrResult(Counter, 6) = o.WorkingSet / 1024
            'WorkingSet = o.WorkingSet / 1024
        
            'perftime = o.Timestamp_PerfTime
            'perftime = o.Timestamp_Object
            'ElapsedTime = o.ElapsedTime
            'TB = o.Frequency_PerfTime
            Counter = Counter + 1
            processcount = processcount + 1
        Next
    End If
    
    Dim resultRow As Integer
    Dim oldp As Double
    oldp = 0
    resultRow = 1
    For i = 1 To Counter - 1
        p = ((arrResult(i, 3) - arrResult(i, 1)) / (arrResult(i, 4) - arrResult(i, 2)))
        If p > 1 Then p = 1
        
        If p > oldp Then resultRow = i
    
        arrResult(i, 7) = Abs(p)
        oldp = p
    Next
    
'    p = ((x1 - x0) / (y1 - y0))
'    If p > 1 Then p = 1
    
'    str = "PercentProcessorTime=" & FormatPercent(p) & "$ThreadCount=" & threadcount & "$WorkingSet=" & WorkingSet & "$"
'    str = "PercentProcessorTime=" & FormatPercent(arrResult(resultRow, 7)) & "$ThreadCount=" & arrResult(resultRow, 5) & "$WorkingSet=" & arrResult(resultRow, 6) & "$"
    str = "PercentProcessorTime=" & arrResult(resultRow, 7) & "$ProcessCount=" & processcount & "$ThreadCount=" & arrResult(resultRow, 5) & "$WorkingSet=" & arrResult(resultRow, 6) & "$"
    
    ' 错误处理
    If Err.Number <> 0 Then
        getProcessInfoNew = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        If Len(strName) = 0 Then
            getProcessInfoNew = "error=计数器未启动或有错误，过会再试！$"
        Else
            getProcessInfoNew = str
        End If
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function


'======================================================================================================
' 功能 : 取Mysql的性能参数
'
' 参数 : a  数据源，需要安装一下MyODBC
'        b  数据源用户名
'        c  数据源密码
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getMysql = 符合条件的串
'        无取得值时：getMysql = 空串
'======================================================================================================
'+--------------------------+-------+
'| Variable_name            | Value |
'+--------------------------+-------+
'| Aborted_clients          | 1     |中断的客户数（客户端死掉且没有关闭连接的个数）
'| Aborted_connects         | 0     |尝试连接到Mysql Server但是失败的个数
'| Bytes_received           | 0     |接收的字节数（所有客户）
'| Bytes_sent               | 0     |发送的字节数（所有客户）
'| Connections              | 59    |尝试连接到Mysql Server的总数
'| Created_tmp_disk_tables  | 0     |创建的临时磁盘表个数
'| Created_tmp_tables       | 0     |创建的临时表个数
'| Created_tmp_files        | 0     |创建的临时文件个数
'| Delayed_insert_threads   | 0     |延迟插入的线程数
'| Delayed_writes           | 0     |延迟写入行数
'| Delayed_errors           | 0     |延迟错误行数
'| Flush_commands           | 1     |执行FLUSH命令的个数
'| Handler_delete           | 0     |当从表中删除一行时所使用的时间
'| Handler_read_first       | 1     |索引第一条被读的个数
'| Handler_read_key         | 2     |根据KEY来读取记录的请求数
'| Handler_read_next        | 0     |在KEY列表中请求读取下一条的个数
'| Handler_read_prev        | 0     |在KEY列表中请求读取上一条的个数
'| Handler_read_rnd         | 12    |根据固定位置（Fixed Position）读取记录的请求数
'| Handler_read_rnd_next    | 573   |在数据文件中读取下一行的请求数
'| Handler_update           | 0     |对表执行UPDATE的请求数
'| Handler_write            | 0     |对表执行INSERT的请求数
'| Key_blocks_used          | 1     |在KEY缓存中使用的块(block)数量
'| Key_read_requests        | 2     |从缓存中读取key block的请求数
'| Key_reads                | 1     |从磁盘物理读取一个key block的个数
'| Key_write_requests       | 0     |向缓存中写入一个key block请求的个数
'| Key_writes               | 0     |向磁盘中物理写入一个key block的个数
'| Max_used_connections     | 1     |最大使用的连接数
'| Not_flushed_key_blocks   | 0     |在key缓存中改变的key block但还没写入磁盘的key block个数
'| Not_flushed_delayed_rows | 0     |等待写入的行数
'| Open_tables              | 2     |打开的表数
'| Open_files               | 4     |打开的文件数
'| Open_streams             | 0     |打开的streams数（用于LOG）
'| Opened_tables            | 21    |已经被打开的表数
'| Questions                | 166   |发送到服务器的查询请求数
'| Select_full_join         | 0     |没有key的join个数
'| Select_full_range_join   | 0     |Number of joins where we used a range search on reference table.
'| Select_range             | 0     |Number of joins where we used ranges on the first table.
'| Select_range_check       | 0     |Number of joins without keys where we check for key usage after each row
'| Select_scan              | 49    |Number of joins where we did a full scann of the first table.
'| Slave_running            | OFF   |Is ON if this is a slave that is connected to a master.
'| Slave_open_temp_tables   | 0     |Number of temporary tables currently open by the slave thread
'| Slow_launch_threads      | 0     |大于slow_launch_time指定时间创建的线程个数
'| Slow_queries             | 0     |大于long_query_time指定时间查询个数
'| Sort_merge_passes        | 0     |Number of merges passes the sort algoritm have had to do. If this value is large you should consider increasing sort_buffer
'| Sort_range               | 0     |Number of sorts that where done with ranges
'| Sort_rows                | 12    |排序的行数
'| Sort_scan                | 6     |Number of sorts that where done by scanning the table
'| Threads_cached           | 0     |线程缓存中的线程数
'| Threads_connected        | 1     |当前打开的连接数
'| Threads_running          | 1     |当前不是sleeping的线程数
'| Uptime                   | 19860 |Mysql Server工作的总秒数
'+--------------------------+-------+
'======================================================================================================
Public Function getMysql(a, b, c)
    On Error Resume Next
    
    
    If Len(a) = 0 And Len(b) = 0 And Len(c) = 0 Then
        getMysql = "error=输入参数出错$"
        Exit Function
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getMysql = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getMysql = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim cnn As New ADODB.Connection, sql As String, result As String
    cnn.Open a, b, c
    
    If Err.Number <> 0 Then
        getMysql = "error=连接数据库的用户名或密码不对$"
        Exit Function
    End If
    
    result = ""
    
    'set rs=cnn.execute("SHOW STATUS;")
    sql = "SHOW STATUS;"
    Set rs = cnn.Execute(sql)
    'rs.Open sql, cnn, 1, 1
    
    If rs.EOF And rs.BOF Then
        getMysql = "error=获取参数出错（无数据）$"
    Else
        Do While Not rs.EOF
            result = result & rs(0) & "=" & rs(1) & "$"
            rs.MoveNext
        Loop
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getMysql = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getMysql = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        getMysql = result
    End If
    
    Set rs = Nothing
    Set cnn = Nothing
End Function



'======================================================================================================
' 功能 : 取磁盘列表
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getDisks = 符合条件的串
'        无取得值时：getDisks = 空串
'======================================================================================================
Public Function getDisks(a, b, c)
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
        'getDisks = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getDisks = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    
    Dim objset As SWbemObjectSet
    ' mediatype = 12 表示硬盘
    Set objset = service.ExecQuery("select * from Win32_LogicalDisk where MediaType=12")
    
    Dim x As SWbemObject, str As String
    
    str = ""
    If objset.count > 0 Then
    
            For Each x In objset
                str = str & x.deviceid & "$"
            Next
    
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getDisks = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getDisks = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        getDisks = str
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function


'======================================================================================================
' 功能 : 取磁盘使用率和剩余空间
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'        d  磁盘名称，如 C:
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getDiskInfo = 符合条件的串
'        无取得值时：getDiskInfo = 空串
'======================================================================================================
Public Function getDiskInfo(a, b, c, d)
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
        'getDiskInfo = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getDiskInfo = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    
    Dim objset As SWbemObjectSet
    Set objset = service.ExecQuery("select * from Win32_LogicalDisk where MediaType=12 and DeviceID='" & d & "'")
    
    Dim Process As SWbemObject, str As String
    str = ""
    If objset.count > 0 Then
        For Each Process In objset
            str = "percentFull=" & FormatNumber(((Process.Size - Process.FreeSpace) / Process.Size) * 100, 2) & "$Mbfree=" & CLng((Process.FreeSpace / 1048576)) & "$"
        Next
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getDiskInfo = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getDiskInfo = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        getDiskInfo = str
    End If
    
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set Process = Nothing
End Function


'======================================================================================================
' 功能 : 取CPU使用率（使用性能计数器类取得数据）
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getCPU = 符合条件的串
'        无取得值时：getCPU = 空串
'
'
'        对于多个CPU时，返回串如：utilization=1$utilization1=0$utilization2=2$
'======================================================================================================
Public Function getCPU(aa, b, c)
    On Error Resume Next
    
    
    Dim locator As New SWbemLocator
    Dim service As SWbemServices
    
    If Len(aa) = 0 And Len(b) = 0 And Len(c) = 0 Then
        Set service = locator.ConnectServer
    Else
        Set service = locator.ConnectServer(aa, "root\cimv2", b, c)
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getCPU = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getCPU = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim objset As SWbemObjectSet, cputotal As Integer, PerfFreq As Long
    
    Set objset = service.ExecQuery("select * from Win32_Processor")

    
    cputotal = objset.count + 1
    'wscript.echo cputotal
    'wscript.quit
    Dim a()
    
    ReDim a(cputotal, 3, 2)
    For ii = 1 To cputotal
        For i = 1 To 3
            For j = 0 To 1
                a(ii, i, j) = 0
            Next
        Next
    Next
    
    PerfFreq = 0
    
    ' 暂停5秒
    Sleep 3000
    
    ' 取第一次 CPU 数据
    sql = "select * from Win32_PerfRawData_PerfOS_Processor " ' where name='_Total'
    Set objsets = service.ExecQuery(sql)
    
    cpuindex = 1
    For Each o In objsets
        a(cpuindex, 1, 0) = o.PercentProcessorTime
        a(cpuindex, 2, 0) = o.Timestamp_Sys100NS
        a(cpuindex, 3, 0) = o.Frequency_Sys100NS
        cpuindex = cpuindex + 1
    Next
    
    ' 暂停5秒
    Sleep 4000

    ' 取第二次 CPU 数据
    sql = "select * from Win32_PerfRawData_PerfOS_Processor " ' where name='_Total'
    Set objsets = service.ExecQuery(sql)
    
    cpuindex = 1
    For Each o In objsets
        a(cpuindex, 1, 1) = o.PercentProcessorTime
        a(cpuindex, 2, 1) = o.Timestamp_Sys100NS
        a(cpuindex, 3, 1) = o.Frequency_Sys100NS
        cpuindex = cpuindex + 1
    Next

    
    '计算结果
    Dim str As String, tmpValue  As String
    str = ""
    count = 1
    
    For i = 1 To cputotal - 1
        If Round(100 * (1 - (a(i, 1, 1) - a(i, 1, 0)) / (a(i, 2, 1) - a(i, 2, 0)))) > 100# Then
            tmpValue = "100.00"
        Else
            tmpValue = CStr(Round(100 * (1 - (a(i, 1, 1) - a(i, 1, 0)) / (a(i, 2, 1) - a(i, 2, 0)))))
        End If
        str = str & "utilization" & count & "=" & tmpValue & "$"
        count = count + 1
    Next
    
    ' 100*(1-(X1-X0)/(Y1-Y0))
    'wscript.echo a(1,0) & chr(9) & a(1,1)
    'wscript.echo a(2,0) & chr(9) & a(2,1)
    'wscript.echo a(3,0) & chr(9) & a(3,1)
    result = Round(100 * (1 - (a(cputotal, 1, 1) - a(cputotal, 1, 0)) / (a(cputotal, 2, 1) - a(cputotal, 2, 0))))
    If result > 100 Then result = 100
'    If result < 0 Then
'        'wscript.echo "error=utilization=" & result & "$" & str
'        wscript.echo "error=采集数据错误$"
'    Else
'        wscript.echo "utilization=" & result & "$" & str
'    End If

    ' 错误处理
    If Err.Number <> 0 Then
        'getCPU = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getCPU = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        If result < 0 Then
            getCPU = "error=采集数据错误$"
        Else
            getCPU = "utilization=" & result & "$" & str
        End If
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function


'======================================================================================================
' 功能 : 取ASP.net参数
'
' 参数 : a  要连接的服务器域名或IP地址
'        b  用户名
'        c  密码
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getCPUNT = 符合条件的串
'        无取得值时：getCPUNT = 空串
'
'
'        对于多个CPU时，返回串如：utilization=1$utilization1=0$utilization2=2$
'======================================================================================================
'===ApplicationRestarts===0===65536  应用程序在 Web 服务器的生存期内重新启动的次数。
'===ApplicationsRunning===1===65536  当前正在运行的 Web 应用程序的数目。
'===RequestExecutionTime===10===65536  执行最近的请求所用的毫秒数。
'===RequestsDisconnected===0===65536  最近的请求在队列中等待的毫秒数。
'===RequestsQueued===0===65536  由于通讯错误或被用户终止而断开连接的请求数。
'===RequestsRejected===0===65536  等候处理的请求数。
'===RequestWaitTime===0===65536  由于请求队列已满而被拒绝的请求数。
'===StateServerSessionsAbandoned===0===65536  已明确放弃的会话数。
'===StateServerSessionsActive===0===65536  当前活动的会话数。
'===StateServerSessionsTimedOut===0===65536  超时的会话数。
'===StateServerSessionsTotal===0===65536  会话总数。
'===WorkerProcessesRunning===1===65536  辅助进程在计算机上重新启动的次数。
'===WorkerProcessRestarts===0===65536  在计算机上运行的辅助进程数。
'======================================================================================================
Public Function getASPNET(a, b, c)
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
        'getASPNET = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getASPNET = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If

    Dim objset As SWbemObjectSet

    Sleep 5000

    Set objset = service.ExecQuery("select * from Win32_PerfRawData_ASPNET_ASPNET")

    Dim x As SWbemObject, result As String
    result = ""
    If objset.count > 0 Then
        For Each x In objset
                Set objp = x.Properties_
                For Each m In objp
                    'wscript.echo "===" & m.name & "===" & m.value & "===" & m.Qualifiers_.Item("countertype")
                    If InStr(m.Name, "_") > 0 Then
                    Else
                        result = result & m.Name & "=" & m.Value & "$"
                    End If
                Next
        Next
    End If

    ' 错误处理
    If Err.Number <> 0 Then
        getASPNET = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        getASPNET = result
    End If

    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function



'======================================================================================================
' 功能 : 取CPU使用率（直接使用CPU类取得数据）
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getCPUNT = 符合条件的串
'        无取得值时：getCPUNT = 空串
'
'
'        对于多个CPU时，返回串如：utilization=1$utilization1=0$utilization2=2$
'======================================================================================================
Public Function getCPUNT(a, b, c)
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
        'getCPUNT = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getCPUNT = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If

    Dim objset As SWbemObjectSet

    Sleep 5000

    Set objset = service.ExecQuery("select * from Win32_Processor")

    Dim x As SWbemObject, str As String, xxx As Integer, total As Double
    str = ""
    xxx = 0
    total = 0#
    If objset.count > 0 Then
        For Each x In objset
            str = str & "utilization" & xxx + 1 & "=" & x.LoadPercentage & "$"
            xxx = xxx + 1
            total = total + CDbl(x.LoadPercentage)
        Next
    End If

    ' 错误处理
    If Err.Number <> 0 Then
        'getCPUNT = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getCPUNT = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        getCPUNT = "utilization=" & CInt(total / xxx) & "$" & str
    End If

    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function


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
    
    If objset.count > 0 Then
        If Err.Number = 0 Then
        Else
            getExchange = "error=指定主机没有Exchange Server$"
            Exit Function
        End If
    End If
    
    Err.Clear
    
    ' Win32_PerfRawData_POP3Svc_MSExchangePOP3
    Set objset = service.ExecQuery("select * from Win32_PerfRawData_POP3Svc_MSExchangePOP3 where name='_Total'")
    If objset.count = 0 Then
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
    
    If objset.count = 0 Then
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
    
    If objset.count = 0 Then
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
    
    If objset.count = 0 Then
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
    
    If objset.count = 0 Then
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
    Set objset = obj.ExecQuery("select * from Win32_PerfRawData_SMTPSVC_SMTPServer where name='_Total'")
    
    If objset.count = 0 Then
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


'======================================================================================================
' 功能 : 取内存参数
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getMemory = 符合条件的串
'        无取得值时：getMemory = 空串
'======================================================================================================
Public Function getMemory(a, b, c)
    On Error Resume Next
    
    'Dim locator As New SWbemLocator
    Set locator = CreateObject("WbemScripting.SWbemLocator")

    'Dim service As SWbemServices
    
    If Len(a) = 0 And Len(b) = 0 And Len(c) = 0 Then
        Set service = locator.ConnectServer
    Else
        Set service = locator.ConnectServer(a, "root\cimv2", b, c)
    End If
    
    service.Security_.AuthenticationLevel = 6
    
    '-------------------------------------------------------------------------------------
    '取总虚拟内存
    Dim TotalPhysicalMemory As Long
    
    Dim wbemObjectSet As SWbemObjectSet
    Dim wbemObject As SWbemObject
 
    Set wbemObjectSet = service.InstancesOf("Win32_LogicalMemoryConfiguration")

    For Each wbemObject In wbemObjectSet
        TotalPhysicalMemory = wbemObject.TotalVirtualMemory
    Next
    TotalPhysicalMemory = TotalPhysicalMemory / 1024

    '----------------------------------------------------------------------------------
    '取可用的虚拟内存
    Dim AvailablePhysicalMemory As Long
    Dim colItems As SWbemObjectSet
    Dim objItem As SWbemObject

    'AvailablePhysicalMemory = 0
    Set colItems = service.ExecQuery("Select * From Win32_PerfRawData_PerfOS_Memory")
    For Each objItem In colItems
        AvailablePhysicalMemory = AvailablePhysicalMemory + objItem.AvailableMbytes
    Next

    '----------------------------------------------------------------------------------
    Dim percentUsedPhysicalMemory As Long
    Dim usedPhysicalMemory As Long
    percentUsedPhysicalMemory = 0
    usedPhysicalMemory = 0
    
    usedPhysicalMemory = TotalPhysicalMemory - AvailablePhysicalMemory
    percentUsedPhysicalMemory = FormatNumber((usedPhysicalMemory / TotalPhysicalMemory) * 100, 2)
    
    '----------------------------------------------------------------------------------
    
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getMemory = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getMemory = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    
    Dim objset As SWbemObjectSet, pagesSet As SWbemObjectSet
    Set objset = service.ExecQuery("select * from Win32_OperatingSystem")
    
    Dim Process As SWbemObject, str As String
    Dim x As SWbemObject, PagesPersec As Double, PerfFreq As Double
    
    
    
    ' 拿第一次 PagesPersec 参数
    Dim pages_value(2, 2)
    PagesPersec = 0#
    PerfFreq = 1
    
    Set pagesSet = service.ExecQuery("select * from Win32_PerfRawData_PerfOS_Memory")
    If pagesSet.count > 0 Then
        If Err.Number = 0 Then
            For Each x In pagesSet
                pages_value(1, 0) = x.PagesPersec
                pages_value(0, 0) = x.Timestamp_PerfTime
            Next
        End If
    End If
'
'    Err.Clear
'
    ' 拿第一次 PagesPersec 参数
    Set pagesSet = service.ExecQuery("select * from Win32_PerfRawData_PerfOS_Memory")
    If pagesSet.count > 0 Then
        If Err.Number = 0 Then
            For Each x In pagesSet
                pages_value(1, 1) = x.PagesPersec
                pages_value(0, 1) = x.Timestamp_PerfTime
                PerfFreq = x.Frequency_PerfTime
            Next
        End If
    End If
'
'    ' 计算 PagesPersec 的值
    Err.Clear
    
    PagesPersec = (pages_value(1, 1) - pages_value(1, 0)) / ((pages_value(0, 1) - pages_value(0, 0)) / PerfFreq)
    If Err.Number > 0 Then
        PagesPersec = 0
        Err.Clear
    End If
    
    
    Dim total As Long, used As Long
    total = 0
    used = 0
    
    str = ""
    If objset.count > 0 Then
        For Each Process In objset
            total = Process.SizeStoredInPagingFiles 'Process.TotalVirtualMemorySize - Process.TotalVisibleMemorySize
            used = Process.SizeStoredInPagingFiles - Process.FreeSpaceInPagingFiles 'Process.TotalVirtualMemorySize - Process.TotalVisibleMemorySize - (Process.FreeVirtualMemory - Process.FreePhysicalMemory)
        
            str = "percentUsed=" & FormatNumber((used / total) * 100, 2) & "$Mbfree=" & CCur((total - used) / 1024) & "$pagesPerSec=" & PagesPersec & "$" & "TotalMemory=" & CCur(total / 1024) & "$"
        Next
    End If
    
    'str = str & "TotalPhysicalMemory=" & TotalPhysicalMemory & "$"
    'str = str & "FreePhysicalMemory=" & AvailablePhysicalMemory & "$"
    'str = str & "percentUsedPhysicalMemory=" & percentUsedPhysicalMemory & "$"
    'str = str & "usedPhysicalMemory=" & usedPhysicalMemory & "$"
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getMemory = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getMemory = "error=123465 Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        getMemory = str
    End If
   
    Set service = Nothing
    Set locator = Nothing
    Set Process = Nothing
    Set pagesSet = Nothing
    Set x = Nothing
    Set objset = Nothing
End Function




'======================================================================================================
' 功能 : 取内存参数
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getPhysicsMemory = 符合条件的串
'        无取得值时：getPhysicsMemory = 空串
'======================================================================================================
Public Function getPhysicsMemory(a, b, c)
    On Error Resume Next
    
    Dim locator As New SWbemLocator
     Dim service As SWbemServices
    
    If Len(a) = 0 And Len(b) = 0 And Len(c) = 0 Then
        Set service = locator.ConnectServer
    Else
        Set service = locator.ConnectServer(a, "root\cimv2", b, c)
    End If
    
    
    '-------------------------------------------------------------------------------------
    '取总物理内存
    Dim TotalPhysicalMemory As Long
    
    Dim wbemObjectSet As SWbemObjectSet
    Dim wbemObject As SWbemObject
 
    Set wbemObjectSet = service.InstancesOf("Win32_LogicalMemoryConfiguration")

    For Each wbemObject In wbemObjectSet
        TotalPhysicalMemory = wbemObject.TotalPhysicalMemory
    Next
    TotalPhysicalMemory = TotalPhysicalMemory / 1024

    '----------------------------------------------------------------------------------
    '取可用的物理内存
    Dim AvailablePhysicalMemory As Long
    Dim colItems As SWbemObjectSet
    Dim objItem As SWbemObject

    'AvailablePhysicalMemory = 0
    Set colItems = service.ExecQuery("Select * From Win32_PerfRawData_PerfOS_Memory")
    For Each objItem In colItems
        AvailablePhysicalMemory = AvailablePhysicalMemory + objItem.AvailableMbytes
    Next

    '----------------------------------------------------------------------------------
    Dim percentUsedPhysicalMemory As Long
    Dim usedPhysicalMemory As Long
    percentUsedPhysicalMemory = 0
    usedPhysicalMemory = 0
    
    usedPhysicalMemory = TotalPhysicalMemory - AvailablePhysicalMemory
    percentUsedPhysicalMemory = FormatNumber((usedPhysicalMemory / TotalPhysicalMemory) * 100, 2)
    
    '----------------------------------------------------------------------------------
    Dim str As String
    
    str = str & "TotalPhysicalMemory=" & TotalPhysicalMemory & "$"
    str = str & "FreePhysicalMemory=" & AvailablePhysicalMemory & "$"
    str = str & "percentUsedPhysicalMemory=" & percentUsedPhysicalMemory & "$"
    'str = str & "usedPhysicalMemory=" & usedPhysicalMemory & "$"
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getPhysicsMemory = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getPhysicsMemory = "error=123465 Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        getPhysicsMemory = str
    End If
   
    Set service = Nothing
    Set locator = Nothing
    Set Process = Nothing
    Set pagesSet = Nothing
    Set x = Nothing
    Set objset = Nothing
End Function


'======================================================================================================
' 功能 : 取服务列表
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getMemory = 符合条件的串
'        无取得值时：getMemory = 空串
'======================================================================================================
Public Function getServices(a, b, c)
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
        'getServices = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getServices = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    
    Dim objset As SWbemObjectSet
    Set objset = service.ExecQuery("select * from Win32_Service")
    
    Dim x As SWbemObject, str As String
    str = ""
    If objset.count > 0 Then
            For Each x In objset
                str = str & x.Name & "#" & x.DisplayName & "$"
            Next
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getServices = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getServices = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        getServices = str
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function


'======================================================================================================
' 功能 : 取指定服务状态
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'        d  服务的名称，比如 : w3svc
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getServiceStatus = 符合条件的串
'        无取得值时：getServiceStatus = 空串
'======================================================================================================
Public Function getServiceStatus(a, b, c, d)
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
        'getServiceStatus = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getServiceStatus = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim objset As SWbemObjectSet
    Set objset = service.ExecQuery("select * from Win32_Service where Name='" & d & "'")
    
    Dim x As SWbemObject, str As String
    str = "0"
    If objset.count > 0 Then
        For Each x In objset
            If x.State = "Running" Then
                str = "1"
            Else
                str = "0"
            End If
            'str = str & x.Name & "#" & x.Description & "$"
        Next
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getServiceStatus = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getServiceStatus = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        getServiceStatus = "Processes=" & str & "$"
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function


'======================================================================================================
' 功能 : 取ASP.net Application参数
'
' 参数 : aa  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getASPnetApplication = 符合条件的串
'        无取得值时：getASPnetApplication = 空串
'======================================================================================================
'===CompilationsTotal===0===65536  动态编译的 .asax、.ascx、.ashx、.asmx 或 .aspx 源文件的数目。
'===RequestsFailed===0===65536  失败的请求总数。
'===SessionsTimedOut===0===65536  超时的会话数。
'===SessionsActive===0===65536  当前活动的会话数。
'===RequestsSucceeded===3===65536  成功执行的请求数。
'===TransactionsPerSec===0===272696320  每秒启动的事务数。
'===RequestsPerSec===3===272696320  每秒执行的请求数。
'===ErrorsTotalPerSec===0===272696320  发生的错误率。
'===AnonymousRequestsPerSec===3===272696320  每秒的身份验证匿名请求数
'===CacheAPITurnoverRate===0===272696320  每秒对 API 缓存的添加数和移除数。
'======================================================================================================
Public Function getASPnetApplication(aa, b, c)
    On Error Resume Next
    
    
    Dim locator As New SWbemLocator
    Dim service As SWbemServices
    
    If Len(aa) = 0 And Len(b) = 0 And Len(c) = 0 Then
        Set service = locator.ConnectServer
    Else
        Set service = locator.ConnectServer(aa, "root\cimv2", b, c)
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getASPnetApplication = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getASPnetApplication = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim objset As SWbemObjectSet, str As String, sql As String, PerfFreq As Double
    
    '-------------------------------------------------------------------------------------------------------------
    ' 取ASP.net Application的数据
    '-------------------------------------------------------------------------------------------------------------
    Dim a(11, 2)
    For i = 1 To 11
        For j = 0 To 1
            a(i, j) = 0
        Next
    Next
    
    PerfFreq = 0#

    flag = False
    
    ' 取第一次ASP.net Application数据
    sql = "select * from Win32_PerfRawData_ASPNET_ASPNETApplications where Name='__Total__'"
    Set objset = service.ExecQuery(sql)
    
    If objset.count > 0 Then
        If Err.Number = 0 Then
            For Each o In objset
                a(1, 0) = o.CompilationsTotal
                a(2, 0) = o.RequestsFailed
                a(3, 0) = o.SessionsTimedOut
                a(4, 0) = o.SessionsActive
                a(5, 0) = o.RequestsSucceeded
            
                a(6, 0) = o.TransactionsPerSec
                a(7, 0) = o.RequestsPerSec
                a(8, 0) = o.ErrorsTotalPerSec
                a(9, 0) = o.AnonymousRequestsPerSec
                a(10, 0) = o.CacheAPITurnoverRate
                a(11, 0) = o.Timestamp_PerfTime
            Next
        Else
            flag = True
        End If
    
    End If
    Err.Clear
    
    
    ' 取第二次ASP.net Application数据
    sql = "select * from Win32_PerfRawData_ASPNET_ASPNETApplications where Name='__Total__'"
    Set objset = service.ExecQuery(sql)
    
    If objset.count > 0 Then
        If Err.Number = 0 Then
            For Each o In objset
                a(1, 1) = o.CompilationsTotal
                a(2, 1) = o.RequestsFailed
                a(3, 1) = o.SessionsTimedOut
                a(4, 1) = o.SessionsActive
                a(5, 1) = o.RequestsSucceeded
            
                a(6, 1) = o.TransactionsPerSec
                a(7, 1) = o.RequestsPerSec
                a(8, 1) = o.ErrorsTotalPerSec
                a(9, 1) = o.AnonymousRequestsPerSec
                a(10, 1) = o.CacheAPITurnoverRate
                a(11, 1) = o.Timestamp_PerfTime
                PerfFreq = o.Frequency_PerfTime
            Next
        Else
            flag = True
        End If
    
    End If
    Err.Clear
    
    '计算结果
    
    If flag Then
        str = "error=无ASP.net Application计数器$"
    Else
        str = "CompilationsTotal=" & a(1, 1) & "$RequestsFailed=" & a(2, 1) & "$SessionsTimedOut=" & a(3, 1) & "$SessionsActive=" & a(4, 1) & "$RequestsSucceeded=" & a(5, 1) & "$TransactionsPerSec=" & (a(6, 1) - a(6, 0)) / ((a(11, 1) - a(11, 0)) / PerfFreq) & "$RequestsPerSec=" & (a(7, 1) - a(7, 0)) / ((a(11, 1) - a(11, 0)) / PerfFreq) & "$ErrorsTotalPerSec=" & (a(8, 1) - a(8, 0)) / ((a(11, 1) - a(11, 0)) / PerfFreq) & "$AnonymousRequestsPerSec=" & (a(9, 1) - a(9, 0)) / ((a(11, 1) - a(11, 0)) / PerfFreq) & "$CacheAPITurnoverRate=" & (a(10, 1) - a(10, 0)) / ((a(11, 1) - a(11, 0)) / PerfFreq) & "$"
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getASPnetApplication = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getASPnetApplication = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        getASPnetApplication = str
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function



'======================================================================================================
' 功能 : 取IIS参数
'
' 参数 : aa  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getIIS = 符合条件的串
'        无取得值时：getIIS = 空串
'======================================================================================================
Public Function getIIS(aa, b, c)
    On Error Resume Next
    
    
    Dim locator As New SWbemLocator
    Dim service As SWbemServices
    
    If Len(aa) = 0 And Len(b) = 0 And Len(c) = 0 Then
        Set service = locator.ConnectServer
    Else
        Set service = locator.ConnectServer(aa, "root\cimv2", b, c)
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getIIS = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getIIS = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim objset As SWbemObjectSet, str As String, sql As String, PerfFreq As Double
    
    '-------------------------------------------------------------------------------------------------------------
    ' 取ＩＩＳ的数据
    '-------------------------------------------------------------------------------------------------------------
    Dim a(10, 2) As Double
    For i = 1 To 10
        For j = 0 To 1
            a(i, j) = 0#
        Next
    Next
    
    PerfFreq = 0#
    flag = False
    
    ' 取第一次IIS数据
    sql = "select * from Win32_PerfRawData_W3SVC_WebService where Name='_Total'"
    'sql="select * from Win32_PerfRawData_W3SVC_WebService"
    Set objset = service.ExecQuery(sql)
    
    If objset.count > 0 Then
        If Err.Number = 0 Then
            For Each o In objset
                a(1, 0) = o.MaximumConnections
                a(2, 0) = o.CurrentAnonymousUsers
                a(3, 0) = o.CurrentConnections
                a(4, 0) = o.TotalNotFoundErrors
                a(5, 0) = o.PostRequestsPersec
                a(6, 0) = o.GetRequestsPersec
                a(7, 0) = o.BytesSentPerSec
                a(8, 0) = o.BytesReceivedPerSec
                a(9, 0) = o.BytesTotalPersec
                a(10, 0) = o.Timestamp_PerfTime
            Next
        Else
            flag = True
        End If
    
    End If
    Err.Clear
    
    
    ' 取第二次IIS数据
    sql = "select * from Win32_PerfRawData_W3SVC_WebService where Name='_Total'"
    Set objset = service.ExecQuery(sql)
    
    If objset.count > 0 Then
        If Err.Number = 0 Then
            For Each o In objset
                a(1, 1) = o.MaximumConnections
                a(2, 1) = o.CurrentAnonymousUsers
                a(3, 1) = o.CurrentConnections
                a(4, 1) = o.TotalNotFoundErrors
                a(5, 1) = o.PostRequestsPersec
                a(6, 1) = o.GetRequestsPersec
                a(7, 1) = o.BytesSentPerSec
                a(8, 1) = o.BytesReceivedPerSec
                a(9, 1) = o.BytesTotalPersec
                a(10, 1) = o.Timestamp_PerfTime
                PerfFreq = o.Frequency_PerfTime
            Next
        Else
            flag = True
        End If
    
    End If
    Err.Clear
    
    '计算结果
    
    If flag Then
        str = "error=无IIS计数器$"
    Else
        str = "maxconnections=" & a(1, 1) & "$currentnonanonymoususers=" & a(2, 1) & "$currentconnections=" & a(3, 1) & "$totalnotfounderrors=" & a(4, 1) & "$postrequestspersec=" & (a(5, 1) - a(5, 0)) / ((a(10, 1) - a(10, 0)) / PerfFreq) & "$getrequestspersec=" & (a(6, 1) - a(6, 0)) / ((a(10, 1) - a(10, 0)) / PerfFreq) & "$bytessentpersec=" & (a(7, 1) - a(7, 0)) / ((a(10, 1) - a(10, 0)) / PerfFreq) & "$bytesreceivedpersec=" & (a(8, 1) - a(8, 0)) / ((a(10, 1) - a(10, 0)) / PerfFreq) & "$bytestotalpersec=" & (a(9, 1) - a(9, 0)) / ((a(10, 1) - a(10, 0)) / PerfFreq) & "$"
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getIIS = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getIIS = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        getIIS = str
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function


'======================================================================================================
' 功能 : 取IIS参数
'
' 参数 : aa  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getWebserver = 符合条件的串
'        无取得值时：getWebserver = 空串
'======================================================================================================
Public Function getWebserver(aa, b, c)
    On Error Resume Next
    
    
    Dim locator As New SWbemLocator
    Dim service As SWbemServices
    
    If Len(aa) = 0 And Len(b) = 0 And Len(c) = 0 Then
        Set service = locator.ConnectServer
    Else
        Set service = locator.ConnectServer(aa, "root\cimv2", b, c)
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getWebserver = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getWebserver = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Dim objset As SWbemObjectSet, str As String, sql As String, PerfFreq As Double
    
    '-------------------------------------------------------------------------------------------------------------
    ' 取ＩＩＳ的数据
    '-------------------------------------------------------------------------------------------------------------
    Dim a(11, 2) As Double
    For i = 1 To 11
        For j = 0 To 1
            a(i, j) = 0
        Next
    Next
    
    PerfFreq = 0
    flag = False
    
    ' 取第一次IIS数据
    sql = "select * from Win32_PerfRawData_W3SVC_WebService where Name='_Total'"
    'sql="select * from Win32_PerfRawData_W3SVC_WebService"
    Set objset = service.ExecQuery(sql)
    
    If objset.count > 0 Then
        If Err.Number = 0 Then
            For Each o In objset
                a(1, 0) = o.TotalGetRequests
                a(2, 0) = o.TotalHeadRequests
                a(3, 0) = o.TotalPostRequests
                
                a(4, 0) = o.TotalNotFoundErrors
                a(5, 0) = o.PostRequestsPersec
                a(6, 0) = o.GetRequestsPersec
                a(7, 0) = o.BytesSentPerSec
                a(8, 0) = o.BytesReceivedPerSec
                
                a(9, 0) = o.BytesTotalPersec
                
                a(10, 0) = o.Timestamp_PerfTime
                a(11, 0) = o.TotalMethodRequestsPersec
            Next
        Else
            flag = True
        End If
    
    End If
    Err.Clear
    
    
    ' 取第二次IIS数据
    sql = "select * from Win32_PerfRawData_W3SVC_WebService where Name='_Total'"
    Set objset = service.ExecQuery(sql)
    
    If objset.count > 0 Then
        If Err.Number = 0 Then
            For Each o In objset
                a(1, 1) = o.TotalGetRequests
                a(2, 1) = o.TotalHeadRequests
                a(3, 1) = o.TotalPostRequests
                
                a(4, 1) = o.TotalNotFoundErrors
                a(5, 1) = o.PostRequestsPersec
                a(6, 1) = o.GetRequestsPersec
                a(7, 1) = o.BytesSentPerSec
                a(8, 1) = o.BytesReceivedPerSec
                
                a(9, 1) = o.BytesTotalPersec
                
                a(10, 1) = o.Timestamp_PerfTime
                a(11, 1) = o.TotalMethodRequestsPersec
                PerfFreq = o.Frequency_PerfTime
            Next
        Else
            flag = True
        End If
    
    End If
    Err.Clear
    
    
    '计算结果
    
    'str = "maxconnections=" & a(1, 1) & "$currentnonanonymoususers=" & a(2, 1) & "$currentconnections=" & a(3, 1) & "$totalnotfounderrors=" & a(4, 1) & "$postrequestspersec=" & (a(5, 1) - a(5, 0)) / ((a(10, 1) - a(10, 0)) / PerfFreq) & "$getrequestspersec=" & (a(6, 1) - a(6, 0)) / ((a(10, 1) - a(10, 0)) / PerfFreq) & "$bytessentpersec=" & (a(7, 1) - a(7, 0)) / ((a(10, 1) - a(10, 0)) / PerfFreq) & "$bytesreceivedpersec=" & (a(8, 1) - a(8, 0)) / ((a(10, 1) - a(10, 0)) / PerfFreq) & "$bytestotalpersec=" & (a(9, 1) - a(9, 0)) / ((a(10, 1) - a(10, 0)) / PerfFreq) & "$"
    If flag Then
        str = "error=无WebServer计数器$"
    Else
        'str = "hitsPerSec=" & (CLng(a(1, 1)) + CLng(a(2, 1)) + CLng(a(3, 1))) & "$bytesPerSec=" & (a(9, 1) - a(9, 0)) / ((a(10, 1) - a(10, 0)) / PerfFreq) & "$"
        str = "hitsPerSec=" & ((a(11, 1) - a(11, 0)) / ((a(10, 1) - a(10, 0)) / PerfFreq)) & "$bytesPerSec=" & (a(9, 1) - a(9, 0)) / ((a(10, 1) - a(10, 0)) / PerfFreq) & "$"
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getWebserver = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getWebserver = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        getWebserver = str
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function


'======================================================================================================
' 功能 : 取日志文件列表
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getNTEventLogFiles = 符合条件的串
'        无取得值时：getNTEventLogFiles = 空串
'======================================================================================================
Public Function getNTEventLogFiles(a, b, c)
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
        'getNTEventLogFiles = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getNTEventLogFiles = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    
    Dim objset As SWbemObjectSet
    Set objset = service.ExecQuery("select * from Win32_NTEventlogFile")
    
    Dim x As SWbemObject, str As String
    str = ""
    If objset.count > 0 Then
            For Each x In objset
                str = str & x.logfilename & "#" & x.logfilename & "$"
            Next
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getNTEventLogFiles = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getNTEventLogFiles = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        getNTEventLogFiles = str
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function

'====================================================================================
'功能：
'参数说明:
'返回结果：
'====================================================================================

Public Function FilterNTEnventLog(strHostIP, strUserName, strPwd, strLogFile, strType, strEventCodeList, strSourceNameList, strMachine, strStartTime)
    Dim locator As New SWbemLocator
    Dim service As SWbemServices
    Dim swObjectSet As SWbemObjectSet
    Dim swObject As SWbemObject

    Dim str1
    Dim arr() As String
    Dim sql, totalsql As String
    Dim nCheckCount, nFilterCount As Integer
    
    sql = "select message from Win32_NTLogEvent where Logfile='" & strLogFile & "'"
    Select Case CInt(strType)
        Case 2 ' 错误
            sql = sql & " and (Type='error' or Type='错误') "
        Case 3 ' 警报
            sql = sql & " and (Type='warning' or Type='警告') "
        Case 4 ' 警报或者错误
            sql = sql & " and (Type='warning' or Type='error' or Type='错误' or Type='警告') "
        Case 5 ' 信息
            sql = sql & " and (Type='information' or Type='信息') "
    End Select
    totalsql = sql
    totalsql = totalsql & " and (TimeWritten>='" & strStartTime & ".000000+480')"
    
    '过滤事件来源不为空
    If (Len(strSourceNameList) > 0) Then
        arr = Split(strSourceNameList, ";")
        For Each str1 In arr
            sql = sql & " and SourceName<>'" & str1 & "'"
        Next
    End If
    '过滤事件ID不为空
    If (Len(strEventCodeList) > 0) Then
        arr = Split(strEventCodeList, ";")
        For Each str1 In arr
            sql = sql & " and EventCode<>" & str1
        Next
    End If
    '机器名不为空,匹配产生事件的机器
    If (Len(strMachine) > 0) Then
        sql = sql & " and ComputerName='" & strMachine & "'"
    End If
    '添加起始时间
    sql = sql & " and (TimeWritten>='" & strStartTime & ".000000+480')"
    
    If (Len(strHostIP) > 0 And Len(strUserName) > 0) Then '如果用户名和主机名称不为空
        Set service = locator.ConnectServer(strHostIP, "root\cimv2", strUserName, strPwd)
    Else '打开本机
        Set service = locator.ConnectServer
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        FilterNTEnventLog = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    nCheckCount = 0
    Set swObjectSet = service.ExecQuery(totalsql) '执行查询
    nCheckCount = swObjectSet.count

    
    ' 错误处理
    If Err.Number <> 0 Then
        FilterNTEnventLog = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    nFilterCount = 0
    Set swObjectSet = service.ExecQuery(sql) '执行查询
    nFilterCount = swObjectSet.count
    'If swObjectSet.count > 0 Then
    '    For Each swObject In swObjectSet
    '        If swObject.message <> vbNull Then '内容不为NULL
    '        End If
    '    Next
    'End If
    ' 错误处理
    If Err.Number <> 0 Then
        FilterNTEnventLog = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    
    FilterNTEnventLog = "checkcount=" & nCheckCount & "$filtercount=" & nFilterCount & "$"
    
    Set service = Nothing
    Set locator = Nothing
    Set swObjectSet = Nothing
    Set swObject = Nothing
End Function


'======================================================================================================
' 功能 : NT日志监测器
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'        logfilename 日志文件名
'        strstarttime 检测起始时间,上次检测之前已检测的日志条目都不再检测了
'        eventtype 匹配日志类型(information, error, warning)
'        sourceidmatch 源和事件ID匹配
'        sourceidnotmatch 源和事件ID不匹配
'        desmatch 描述匹配
'        desnotmatch 描述不匹配
'        eventcategory 事件分类匹配
'        eventmachine 事件发生机器匹配
'        interval 间隔(具体作用未搞清)
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getNTEventLog = 符合条件的串
'        无取得值时：getNTEventLog = 空串
'======================================================================================================
Public Function getNTEventLog(a, b, c, logfilename, strStartTime, eventtype, sourceidmatch, sourceidnotmatch, desmatch, desnotmatch, eventcategory, eventmachine, interval)
    'On Error Resume Next
    
    
    Dim locator As New SWbemLocator
    Dim service As SWbemServices
    
    If Len(a) = 0 And Len(b) = 0 And Len(c) = 0 Then
        Set service = locator.ConnectServer
    Else
        Set service = locator.ConnectServer(a, "root\cimv2", b, c)
    End If
    
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getNTEventLog = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getNTEventLog = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    '**************************************************************************************************
    ' 根据多项条件来生成查询语句(颇复杂啊)
    '**************************************************************************************************
    Dim sql As String, totalchecksql As String
    
    sql = "select message from Win32_NTLogEvent where Logfile='" & logfilename & "' "
    
    Select Case CInt(eventtype)
        Case 1 ' all
        Case 2 ' error
            sql = sql & " and Type='error'"
        Case 3 ' warning
            sql = sql & " and Type='warning'"
        Case 4 ' error or warning
            sql = sql & " and (Type='warning' or Type='error')"
        Case 5 ' information
            sql = sql & " and Type='information'"
        Case Else ' all
    End Select
    
    totalchecksql = sql & " and (TimeWritten>='" & strStartTime & ".000000+480')"
    
    ' 事件名和ID匹配
    If Len(sourceidmatch) > 0 Then
        If InStr(sourceidmatch, ":") > 0 Then
            arr = Split(sourceidmatch, ":")
            sql = sql & " and (SourceName='" & arr(0) & "' and EventCode=" & arr(1) & ")"
        Else
            sql = sql & " and SourceName='" & sourceidmatch & "'"
        End If
    End If

    ' 事件名和ID不匹配
    If Len(sourceidnotmatch) > 0 Then
        If InStr(sourceidnotmatch, ":") > 0 Then
            arr = Split(sourceidnotmatch, ":")
            sql = sql & " and (SourceName<>'" & arr(0) & "' and EventCode<>" & arr(1) & ")"
        Else
            sql = sql & " and SourceName<>'" & sourceidnotmatch & "'"
        End If
    End If
    
'    ' 描述内容匹配
'    If Len(desmatch) > 0 Then
'        sql = sql & " and (message like '%" & desmatch & "%')"
'    End If
'
'    ' 描述内容不匹配
'    If Len(desnotmatch) > 0 Then
'        sql = sql & " and (not(message like '%" & desnotmatch & "%'))"
'    End If
    
    
    ' 分类匹配
    If Len(eventcategory) > 0 Then
        sql = sql & " and Category=" & eventcategory
    End If
    
    ' 产生事件的机器匹配
    If Len(eventmachine) > 0 Then
        sql = sql & " and ComputerName='" & eventmachine & "'"
    End If
    
    sql = sql & " and (TimeWritten>='" & strStartTime & ".000000+480')"
    '**************************************************************************************************
    ' 根据多项条件来生成查询语句(颇复杂啊)--------结束
    '**************************************************************************************************
    'getNTEventLog = sql
    'Exit Function
    
    Dim objset As SWbemObjectSet
    Set objset = service.ExecQuery(totalchecksql)
    
    Dim x As SWbemObject, str As String, totalmatch As Integer, totalcheck As Integer
    totalmatch = 0
    totalcheck = 0
    
    ' 以下统计共检测了多少条日志记录
    totalcheck = objset.count
    
'    If objset.Count > 0 Then
'        For Each x In objset
'            If Left(x.TimeWritten, 14) > strstarttime Then
'                totalcheck = totalcheck + 1
'            End If
'        Next
'    End If
    
    ' 以下从符合条件的日志记录中继续过滤
    Set objset = service.ExecQuery(sql)
    
    str = ""
    totalmatch = 0 'objset.count
    
    If objset.count > 0 Then
            For Each x In objset

                    If Len(desmatch) > 0 And Len(desnotmatch) > 0 Then  ' 如果内容即要匹配又要不匹配则互斥,匹配数应为0
                    ElseIf Len(desmatch) > 0 And Len(desnotmatch) = 0 Then  ' 如果内容匹配
                        If InStr(x.message, desmatch) > 0 Then totalmatch = totalmatch + 1
                    ElseIf Len(desmatch) = 0 And Len(desnotmatch) > 0 Then  ' 如果内容不匹配
                        If InStr(x.message, desnotmatch) = 0 Then totalmatch = totalmatch + 1
                    Else
                      totalmatch = totalmatch + 1
                    End If
                   
            Next
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getNTEventLog = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getNTEventLog = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        If totalcheck < totalmatch Then
            totalcheck = totalcheck + (totalmatch - totalcheck)
        End If
        getNTEventLog = "totalcheck=" & totalcheck & "$totalmatch=" & totalmatch & "$"
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function



'======================================================================================================
' 功能 : 取SQL Server参数
'
' 参数 : aa  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getSQLServer = IndexSearchesPersec=0$Freepages=59$Totalpages=2048$CacheHitRatio=80.11%$ActiveTransactions=0$DataFilesSizeKB=334976$PercentLogUsed=23$TransactionsPersec=0$LoginsPersec=0$LogoutsPersec=0$UserConnections=0$LockRequestsPersec=0$LockMemoryKB=240$TotalServerMemoryKB=17352$BatchRequestsPersec=0$
'        无取得值时：getSQLServer = 空串
'======================================================================================================
Public Function getSQLServer(aa, b, c)
    On Error Resume Next
    
    
    Dim locator As New SWbemLocator
    Dim service As SWbemServices
    
    If Len(aa) = 0 And Len(b) = 0 And Len(c) = 0 Then
        Set obj = locator.ConnectServer
    Else
        Set obj = locator.ConnectServer(aa, "root\cimv2", b, c)
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getSQLServer = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getSQLServer = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Err.Clear
    
    Dim objsets As SWbemObjectSet, str As String, sql As String, PerfFreq As Double
    Dim CacheHitRatio_Base As Double
    
    '-------------------------------------------------------------------------------------------------------------
    ' 初始化数组数据
    '-------------------------------------------------------------------------------------------------------------
    Dim a(16, 2) As Double
    For i = 1 To 10
        For j = 0 To 1
            a(i, j) = 0
        Next
    Next
    
    PerfFreq = 0
    
    flag = False
    
    '-------------------------------------------------------------------------------------------------------------
    ' 取第一次 SQL Server 数据
    '-------------------------------------------------------------------------------------------------------------
    
    'Access Methods (Win32_PerfRawData_MSSQLSERVER_SQLServerAccessMethods)
    '*  Index Searches/sec  272696320   IndexSearchesPersec
    '   Full Scans/sec      272696320   FullScansPersec
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerAccessMethods"
    Set objsets = obj.ExecQuery(sql)
    
    If objsets.count > 0 Then
        If Err.Number = 0 Then
            For Each o In objsets
                a(1, 0) = o.IndexSearchesPersec
            Next
        Else
            getSQLServer = "error=无SQL Server监测器$"
            Exit Function
        End If
    End If
    
    
    'Buffer Manager (Win32_PerfRawData_MSSQLSERVER_SQLServerBufferManager)
    '   Buffer cache hit ratio  537003008   Buffercachehitratio
    '   Database pages      65536       Databasepages
    '*  Free pages      65536       Freepages
    '   Page life expectancy    65536       Pagelifeexpectancy
    '   Procedure cache pages   65536       Procedurecachepages
    '   Reserved Pages      65536       Reservedpages
    '   Stolen pages        65536       Stolenpages
    '   Target pages        65536       Targetpages
    '*  Total pages     65536       Totalpages
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerBufferManager"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(2, 0) = o.Freepages
        a(3, 0) = o.Totalpages
    Next
    
    'Cache Manager  (Win32_PerfRawData_MSSQLSERVER_SQLServerCacheManager) _Total
    '*  Cache Hit Ratio     537003008   100 * X/B  (B=Base value)   CacheHitRatio
    '   CacheHitRatio_Base  1073939459              CacheHitRatio_Base
    '   Cache Object Counts 65536                   CacheObjectCounts
    '   Cache Pages     65536                   CacheObjectCounts
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerCacheManager where Name='_Total'"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(4, 0) = o.CacheHitRatio
        CacheHitRatio_Base = o.CacheHitRatio_Base
        If CacheHitRatio_Base = 0 Then CacheHitRatio_Base = 1
    Next
    
    
    'Databases  (Win32_PerfRawData_MSSQLSERVER_SQLServerDatabases) _Total
    '*  Active Transactions     65536       ActiveTransactions
    '*  Data File(s) Size (KB)      65536       DataFilesSizeKB
    '   Log File(s) Size(KB)        65536       LogFilesSizeKB
    '   Log File(s) Used Size(KB)   65536       LogFilesUsedSizeKB
    '*  Percent Log Used        65536       PercentLogUsed
    '*  Transactions/sec        272696320   TransactionsPersec
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerDatabases where Name='_Total'"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(5, 0) = o.ActiveTransactions
        a(6, 0) = o.DataFilesSizeKB
        a(7, 0) = o.PercentLogUsed
        a(8, 0) = o.TransactionsPerSec
    Next
    
    
    'General Statistics (Win32_PerfRawData_MSSQLSERVER_SQLServerGeneralStatistics)
    '*  Logins/sec  272696320   LoginsPersec
    '*  Logouts/sec 272696320   LogoutsPersec
    '*  User Connections    65536   UserConnections
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerGeneralStatistics"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(9, 0) = o.LoginsPersec
        a(10, 0) = o.LogoutsPersec
        a(11, 0) = o.UserConnections
    Next
    
    
    'Locks  (Win32_PerfRawData_MSSQLSERVER_SQLServerLocks) _Total
    '*  Lock Requests/sec   272696320   LockRequestsPersec
    '   Number of Deadlocks/sec 272696320   NumberofDeadlocksPersec
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerLocks where Name='_Total'"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(12, 0) = o.LockRequestsPersec
        'a(13,0) = o.NumberofDeadlocksPersec
    Next
    
    
    'Memory Manager (Win32_PerfRawData_MSSQLSERVER_SQLServerMemoryManager)
    '   Connection Memory(KB)       65536   ConnectionMemoryKB
    '   Granted Workspace Memory(KB)    65536   GrantedWorkspaceMemoryKB
    '*  Lock Memory(KB)         65536   LockMemoryKB
    '   Maximum Workspace Memory(KB)    65536   MaximumWorkspaceMemoryKB
    '   SQL Cache Memory(KB)        65536   SQLCacheMemoryKB
    '   Target Server Memory(KB)    65536   TargetServerMemoryKB
    '*  Total Server Memory(KB)     65536   TotalServerMemoryKB
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerMemoryManager"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(13, 0) = o.LockMemoryKB
        a(14, 0) = o.TotalServerMemoryKB
    Next
    
    
    'SQL Statistics (Win32_PerfRawData_MSSQLSERVER_SQLServerSQLStatistics)
    '*  Batch Requests/sec  BatchRequestsPersec
    '   SQL Compilations/sec    SQLCompilationsPersec
    '   SQL Re-Compilations/sec SQLReCompilationsPersec
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerSQLStatistics"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(15, 0) = o.BatchRequestsPersec
        a(16, 0) = o.Timestamp_PerfTime
    Next
    
    
    '-------------------------------------------------------------------------------------------------------------
    ' 取第二次 SQL Server 数据
    '-------------------------------------------------------------------------------------------------------------
    
    'Access Methods (Win32_PerfRawData_MSSQLSERVER_SQLServerAccessMethods)
    '*  Index Searches/sec  272696320   IndexSearchesPersec
    '   Full Scans/sec      272696320   FullScansPersec
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerAccessMethods"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(1, 1) = o.IndexSearchesPersec
    Next
    
    
    'Buffer Manager (Win32_PerfRawData_MSSQLSERVER_SQLServerBufferManager)
    '   Buffer cache hit ratio  537003008   Buffercachehitratio
    '   Database pages      65536       Databasepages
    '*  Free pages      65536       Freepages
    '   Page life expectancy    65536       Pagelifeexpectancy
    '   Procedure cache pages   65536       Procedurecachepages
    '   Reserved Pages      65536       Reservedpages
    '   Stolen pages        65536       Stolenpages
    '   Target pages        65536       Targetpages
    '*  Total pages     65536       Totalpages
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerBufferManager"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(2, 1) = o.Freepages
        a(3, 1) = o.Totalpages
    Next
    
    'Cache Manager  (Win32_PerfRawData_MSSQLSERVER_SQLServerCacheManager) _Total
    '*  Cache Hit Ratio     537003008   100 * X/B  (B=Base value)   CacheHitRatio
    '   CacheHitRatio_Base  1073939459              CacheHitRatio_Base
    '   Cache Object Counts 65536                   CacheObjectCounts
    '   Cache Pages     65536                   CacheObjectCounts
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerCacheManager where Name='_Total'"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(4, 1) = o.CacheHitRatio
        CacheHitRatio_Base = o.CacheHitRatio_Base
        If CacheHitRatio_Base = 0 Then CacheHitRatio_Base = 1
    Next
    
    
    'Databases  (Win32_PerfRawData_MSSQLSERVER_SQLServerDatabases) _Total
    '*  Active Transactions     65536       ActiveTransactions
    '*  Data File(s) Size (KB)      65536       DataFilesSizeKB
    '   Log File(s) Size(KB)        65536       LogFilesSizeKB
    '   Log File(s) Used Size(KB)   65536       LogFilesUsedSizeKB
    '*  Percent Log Used        65536       PercentLogUsed
    '*  Transactions/sec        272696320   TransactionsPersec
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerDatabases where Name='_Total'"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(5, 1) = o.ActiveTransactions
        a(6, 1) = o.DataFilesSizeKB
        a(7, 1) = o.PercentLogUsed
        a(8, 1) = o.TransactionsPerSec
    Next
    
    
    'General Statistics (Win32_PerfRawData_MSSQLSERVER_SQLServerGeneralStatistics)
    '*  Logins/sec  272696320   LoginsPersec
    '*  Logouts/sec 272696320   LogoutsPersec
    '*  User Connections    65536   UserConnections
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerGeneralStatistics"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(9, 1) = o.LoginsPersec
        a(10, 1) = o.LogoutsPersec
        a(11, 1) = o.UserConnections
    Next
    
    
    'Locks  (Win32_PerfRawData_MSSQLSERVER_SQLServerLocks) _Total
    '*  Lock Requests/sec   272696320   LockRequestsPersec
    '   Number of Deadlocks/sec 272696320   NumberofDeadlocksPersec
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerLocks where Name='_Total'"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(12, 1) = o.LockRequestsPersec
        'a(13,0) = o.NumberofDeadlocksPersec
    Next
    
    
    'Memory Manager (Win32_PerfRawData_MSSQLSERVER_SQLServerMemoryManager)
    '   Connection Memory(KB)       65536   ConnectionMemoryKB
    '   Granted Workspace Memory(KB)    65536   GrantedWorkspaceMemoryKB
    '*  Lock Memory(KB)         65536   LockMemoryKB
    '   Maximum Workspace Memory(KB)    65536   MaximumWorkspaceMemoryKB
    '   SQL Cache Memory(KB)        65536   SQLCacheMemoryKB
    '   Target Server Memory(KB)    65536   TargetServerMemoryKB
    '*  Total Server Memory(KB)     65536   TotalServerMemoryKB
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerMemoryManager"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(13, 1) = o.LockMemoryKB
        a(14, 1) = o.TotalServerMemoryKB
    Next
    
    
    'SQL Statistics (Win32_PerfRawData_MSSQLSERVER_SQLServerSQLStatistics)
    '*  Batch Requests/sec  272696320   BatchRequestsPersec
    '   SQL Compilations/sec    272696320   SQLCompilationsPersec
    '   SQL Re-Compilations/sec 272696320   SQLReCompilationsPersec
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerSQLStatistics"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(15, 1) = o.BatchRequestsPersec
        a(16, 1) = o.Timestamp_PerfTime
        PerfFreq = o.Frequency_PerfTime
    Next
    
    
    '计算结果
    
    str = ""

    str = "IndexSearchesPersec=" & CDbl(CDbl(a(1, 1) - a(1, 0)) / (CDbl(a(16, 1) - a(16, 0)) / CDbl(PerfFreq)))
    str = str & "$Freepages=" & a(2, 1) & "$Totalpages=" & a(3, 1)
    str = str & "$CacheHitRatio=" & FormatPercent(CDbl(a(4, 1)) / CacheHitRatio_Base)
    str = str & "$ActiveTransactions=" & a(5, 1) & "$DataFilesSizeKB=" & a(6, 1) & "$PercentLogUsed=" & a(7, 1)
    str = str & "$TransactionsPersec=" & CDbl(CDbl(a(8, 1) - a(8, 0)) / (CDbl(a(16, 1) - a(16, 0)) / CDbl(PerfFreq)))
    str = str & "$LoginsPersec=" & CDbl(CDbl(a(9, 1) - a(9, 0)) / (CDbl(a(16, 1) - a(16, 0)) / CDbl(PerfFreq)))
    str = str & "$LogoutsPersec=" & CDbl(CDbl(a(10, 1) - a(10, 0)) / (CDbl(a(16, 1) - a(16, 0)) / CDbl(PerfFreq)))
    str = str & "$UserConnections=" & a(11, 1)
    str = str & "$LockRequestsPersec=" & CDbl(CDbl(a(12, 1) - a(12, 0)) / (CDbl(a(16, 1) - a(16, 0)) / CDbl(PerfFreq)))
    str = str & "$LockMemoryKB=" & a(13, 1)
    str = str & "$TotalServerMemoryKB=" & a(14, 1)
    str = str & "$BatchRequestsPersec=" & CDbl(CDbl(a(15, 1) - a(15, 0)) / (CDbl(a(16, 1) - a(16, 0)) / CDbl(PerfFreq))) & "$"

    'If flag Then
    '    str = "error=无WebServer计数器$"
    'Else
    '    str = "hitsPerSec=" & (CLng(a(1, 1)) + CLng(a(2, 1)) + CLng(a(3, 1))) & "$bytesPerSec=" & (a(9, 1) - a(9, 0)) / ((a(10, 1) - a(10, 0)) / PerfFreq) & "$"
    'End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getSQLServer = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getSQLServer = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        getSQLServer = str
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function


'======================================================================================================
' 功能 : 取SQL Server7参数
'
' 参数 : aa  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getSQLServer7 = IndexSearchesPersec=0$Freepages=59$Totalpages=2048$CacheHitRatio=80.11%$ActiveTransactions=0$DataFilesSizeKB=334976$PercentLogUsed=23$TransactionsPersec=0$LoginsPersec=0$LogoutsPersec=0$UserConnections=0$LockRequestsPersec=0$LockMemoryKB=240$TotalServerMemoryKB=17352$BatchRequestsPersec=0$
'        无取得值时：getSQLServer7 = 空串
'======================================================================================================
Public Function getSQLServer7(aa, b, c)
    On Error Resume Next
    
    
    Dim locator As New SWbemLocator
    Dim service As SWbemServices
    
    If Len(aa) = 0 And Len(b) = 0 And Len(c) = 0 Then
        Set obj = locator.ConnectServer
    Else
        Set obj = locator.ConnectServer(aa, "root\cimv2", b, c)
    End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getSQLServer7 = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getSQLServer7 = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
        Exit Function
    End If
    
    Err.Clear
    
    Dim objsets As SWbemObjectSet, str As String, sql As String, PerfFreq As Double
    Dim CacheHitRatio_Base As Double
    
    '-------------------------------------------------------------------------------------------------------------
    ' 初始化数组数据
    '-------------------------------------------------------------------------------------------------------------
    Dim a(16, 2) As Double
    For i = 1 To 10
        For j = 0 To 1
            a(i, j) = 0
        Next
    Next
    
    PerfFreq = 0
    
    flag = False
    
    '-------------------------------------------------------------------------------------------------------------
    ' 取第一次 SQL Server 数据
    '-------------------------------------------------------------------------------------------------------------
    
    'Access Methods (Win32_PerfRawData_MSSQLSERVER_SQLServerAccessMethods)
    '*  Index Searches/sec  272696320   IndexSearchesPersec
    '   Full Scans/sec      272696320   FullScansPersec
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerAccessMethods"
    Set objsets = obj.ExecQuery(sql)
    
    If objsets.count > 0 Then
        If Err.Number = 0 Then
            For Each o In objsets
                a(1, 0) = o.IndexSearchesPersec
            Next
        Else
            getSQLServer7 = "error=无SQL Server监测器$"
            Exit Function
        End If
    End If
    
    
    'Buffer Manager (Win32_PerfRawData_MSSQLSERVER_SQLServerBufferManager)
    '   Buffer cache hit ratio  537003008   Buffercachehitratio
    '   Database pages      65536       Databasepages
    '*  Free pages      65536       Freepages
    '   Page life expectancy    65536       Pagelifeexpectancy
    '   Procedure cache pages   65536       Procedurecachepages
    '   Reserved Pages      65536       Reservedpages
    '   Stolen pages        65536       Stolenpages
    '   Target pages        65536       Targetpages
    '*  Total pages     65536       Totalpages
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerBufferManager"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(2, 0) = o.FreeBuffers
        a(3, 0) = o.StolenPageCount
    Next
    
    'Cache Manager  (Win32_PerfRawData_MSSQLSERVER_SQLServerCacheManager) _Total
    '*  Cache Hit Ratio     537003008   100 * X/B  (B=Base value)   CacheHitRatio
    '   CacheHitRatio_Base  1073939459              CacheHitRatio_Base
    '   Cache Object Counts 65536                   CacheObjectCounts
    '   Cache Pages     65536                   CacheObjectCounts
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerCacheManager where Name='Adhoc Sql Plans'"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(4, 0) = o.CacheHitRatio
        CacheHitRatio_Base = o.CacheHitRatio_Base
        If CacheHitRatio_Base = 0 Then CacheHitRatio_Base = 1
    Next
    
    
    'Databases  (Win32_PerfRawData_MSSQLSERVER_SQLServerDatabases) _Total
    '*  Active Transactions     65536       ActiveTransactions
    '*  Data File(s) Size (KB)      65536       DataFilesSizeKB
    '   Log File(s) Size(KB)        65536       LogFilesSizeKB
    '   Log File(s) Used Size(KB)   65536       LogFilesUsedSizeKB
    '*  Percent Log Used        65536       PercentLogUsed
    '*  Transactions/sec        272696320   TransactionsPersec
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerDatabases where Name='master'"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(5, 0) = o.ActiveTransactions
        a(6, 0) = o.DataFilesSizeKB
        a(7, 0) = o.PercentLogUsed
        a(8, 0) = o.TransactionsPerSec
    Next
    
    
    'General Statistics (Win32_PerfRawData_MSSQLSERVER_SQLServerGeneralStatistics)
    '*  Logins/sec  272696320   LoginsPersec
    '*  Logouts/sec 272696320   LogoutsPersec
    '*  User Connections    65536   UserConnections
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerGeneralStatistics"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(9, 0) = o.LoginsPersec
        a(10, 0) = o.LogoutsPersec
        a(11, 0) = o.UserConnections
    Next
    
    
    'Locks  (Win32_PerfRawData_MSSQLSERVER_SQLServerLocks) _Total
    '*  Lock Requests/sec   272696320   LockRequestsPersec
    '   Number of Deadlocks/sec 272696320   NumberofDeadlocksPersec
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerLocks where Name='Database'"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(12, 0) = o.LockRequestsPersec
        'a(13,0) = o.NumberofDeadlocksPersec
    Next
    
    
    'Memory Manager (Win32_PerfRawData_MSSQLSERVER_SQLServerMemoryManager)
    '   Connection Memory(KB)       65536   ConnectionMemoryKB
    '   Granted Workspace Memory(KB)    65536   GrantedWorkspaceMemoryKB
    '*  Lock Memory(KB)         65536   LockMemoryKB
    '   Maximum Workspace Memory(KB)    65536   MaximumWorkspaceMemoryKB
    '   SQL Cache Memory(KB)        65536   SQLCacheMemoryKB
    '   Target Server Memory(KB)    65536   TargetServerMemoryKB
    '*  Total Server Memory(KB)     65536   TotalServerMemoryKB
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerMemoryManager"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(13, 0) = o.LockMemoryKB
        a(14, 0) = o.TotalServerMemoryKB
    Next
    
    
    'SQL Statistics (Win32_PerfRawData_MSSQLSERVER_SQLServerSQLStatistics)
    '*  Batch Requests/sec  BatchRequestsPersec
    '   SQL Compilations/sec    SQLCompilationsPersec
    '   SQL Re-Compilations/sec SQLReCompilationsPersec
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerSQLStatistics"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(15, 0) = o.BatchRequestsPersec
        a(16, 0) = o.Timestamp_PerfTime
    Next
    
    
    '-------------------------------------------------------------------------------------------------------------
    ' 取第二次 SQL Server 数据
    '-------------------------------------------------------------------------------------------------------------
    
    'Access Methods (Win32_PerfRawData_MSSQLSERVER_SQLServerAccessMethods)
    '*  Index Searches/sec  272696320   IndexSearchesPersec
    '   Full Scans/sec      272696320   FullScansPersec
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerAccessMethods"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(1, 1) = o.IndexSearchesPersec
    Next
    
    
    'Buffer Manager (Win32_PerfRawData_MSSQLSERVER_SQLServerBufferManager)
    '   Buffer cache hit ratio  537003008   Buffercachehitratio
    '   Database pages      65536       Databasepages
    '*  Free pages      65536       Freepages
    '   Page life expectancy    65536       Pagelifeexpectancy
    '   Procedure cache pages   65536       Procedurecachepages
    '   Reserved Pages      65536       Reservedpages
    '   Stolen pages        65536       Stolenpages
    '   Target pages        65536       Targetpages
    '*  Total pages     65536       Totalpages
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerBufferManager"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(2, 1) = o.FreeBuffers
        a(3, 1) = o.StolenPageCount
    Next
    
    'Cache Manager  (Win32_PerfRawData_MSSQLSERVER_SQLServerCacheManager) _Total
    '*  Cache Hit Ratio     537003008   100 * X/B  (B=Base value)   CacheHitRatio
    '   CacheHitRatio_Base  1073939459              CacheHitRatio_Base
    '   Cache Object Counts 65536                   CacheObjectCounts
    '   Cache Pages     65536                   CacheObjectCounts
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerCacheManager where Name='Adhoc Sql Plans'"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(4, 1) = o.CacheHitRatio
        CacheHitRatio_Base = o.CacheHitRatio_Base
    Next
    
    
    'Databases  (Win32_PerfRawData_MSSQLSERVER_SQLServerDatabases) _Total
    '*  Active Transactions     65536       ActiveTransactions
    '*  Data File(s) Size (KB)      65536       DataFilesSizeKB
    '   Log File(s) Size(KB)        65536       LogFilesSizeKB
    '   Log File(s) Used Size(KB)   65536       LogFilesUsedSizeKB
    '*  Percent Log Used        65536       PercentLogUsed
    '*  Transactions/sec        272696320   TransactionsPersec
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerDatabases where Name='master'"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(5, 1) = o.ActiveTransactions
        a(6, 1) = o.DataFilesSizeKB
        a(7, 1) = o.PercentLogUsed
        a(8, 1) = o.TransactionsPerSec
    Next
    
    
    'General Statistics (Win32_PerfRawData_MSSQLSERVER_SQLServerGeneralStatistics)
    '*  Logins/sec  272696320   LoginsPersec
    '*  Logouts/sec 272696320   LogoutsPersec
    '*  User Connections    65536   UserConnections
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerGeneralStatistics"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(9, 1) = o.LoginsPersec
        a(10, 1) = o.LogoutsPersec
        a(11, 1) = o.UserConnections
    Next
    
    
    'Locks  (Win32_PerfRawData_MSSQLSERVER_SQLServerLocks) _Total
    '*  Lock Requests/sec   272696320   LockRequestsPersec
    '   Number of Deadlocks/sec 272696320   NumberofDeadlocksPersec
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerLocks where Name='Database'"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(12, 1) = o.LockRequestsPersec
        'a(13,0) = o.NumberofDeadlocksPersec
    Next
    
    
    'Memory Manager (Win32_PerfRawData_MSSQLSERVER_SQLServerMemoryManager)
    '   Connection Memory(KB)       65536   ConnectionMemoryKB
    '   Granted Workspace Memory(KB)    65536   GrantedWorkspaceMemoryKB
    '*  Lock Memory(KB)         65536   LockMemoryKB
    '   Maximum Workspace Memory(KB)    65536   MaximumWorkspaceMemoryKB
    '   SQL Cache Memory(KB)        65536   SQLCacheMemoryKB
    '   Target Server Memory(KB)    65536   TargetServerMemoryKB
    '*  Total Server Memory(KB)     65536   TotalServerMemoryKB
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerMemoryManager"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(13, 1) = o.LockMemoryKB
        a(14, 1) = o.TotalServerMemoryKB
    Next
    
    
    'SQL Statistics (Win32_PerfRawData_MSSQLSERVER_SQLServerSQLStatistics)
    '*  Batch Requests/sec  272696320   BatchRequestsPersec
    '   SQL Compilations/sec    272696320   SQLCompilationsPersec
    '   SQL Re-Compilations/sec 272696320   SQLReCompilationsPersec
    
    sql = "select * from Win32_PerfRawData_MSSQLSERVER_SQLServerSQLStatistics"
    Set objsets = obj.ExecQuery(sql)
    
    For Each o In objsets
        a(15, 1) = o.BatchRequestsPersec
        a(16, 1) = o.Timestamp_PerfTime
        PerfFreq = o.Frequency_PerfTime
    Next
    
    
    '计算结果
    
    str = ""

    str = "IndexSearchesPersec=" & CDbl(CDbl(a(1, 1) - a(1, 0)) / (CDbl(a(16, 1) - a(16, 0)) / CDbl(PerfFreq)))
    str = str & "$Freepages=" & a(2, 1) & "$Totalpages=" & a(3, 1)
    If CacheHitRatio_Base = 0 Then
        str = str & "$CacheHitRatio=0"
    Else
        str = str & "$CacheHitRatio=" & FormatPercent(CDbl(a(4, 1)) / CDbl(CacheHitRatio_Base))
    End If
    str = str & "$ActiveTransactions=" & a(5, 1) & "$DataFilesSizeKB=" & a(6, 1) & "$PercentLogUsed=" & a(7, 1)
    str = str & "$TransactionsPersec=" & CDbl(CDbl(a(8, 1) - a(8, 0)) / (CDbl(a(16, 1) - a(16, 0)) / CDbl(PerfFreq)))
    str = str & "$LoginsPersec=" & CDbl(CDbl(a(9, 1) - a(9, 0)) / (CDbl(a(16, 1) - a(16, 0)) / CDbl(PerfFreq)))
    str = str & "$LogoutsPersec=" & CDbl(CDbl(a(10, 1) - a(10, 0)) / (CDbl(a(16, 1) - a(16, 0)) / CDbl(PerfFreq)))
    str = str & "$UserConnections=" & a(11, 1)
    str = str & "$LockRequestsPersec=" & CDbl(CDbl(a(12, 1) - a(12, 0)) / (CDbl(a(16, 1) - a(16, 0)) / CDbl(PerfFreq)))
    str = str & "$LockMemoryKB=" & a(13, 1)
    str = str & "$TotalServerMemoryKB=" & a(14, 1)
    str = str & "$BatchRequestsPersec=" & CDbl(CDbl(a(15, 1) - a(15, 0)) / (CDbl(a(16, 1) - a(16, 0)) / CDbl(PerfFreq))) & "$"

    'If flag Then
    '    str = "error=无WebServer计数器$"
    'Else
    '    str = "hitsPerSec=" & (CLng(a(1, 1)) + CLng(a(2, 1)) + CLng(a(3, 1))) & "$bytesPerSec=" & (a(9, 1) - a(9, 0)) / ((a(10, 1) - a(10, 0)) / PerfFreq) & "$"
    'End If
    
    ' 错误处理
    If Err.Number <> 0 Then
        'getSQLServer7 = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description
        getSQLServer7 = "error=Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
    Else
        getSQLServer7 = str
    End If
    
    Set service = Nothing
    Set locator = Nothing
    Set objset = Nothing
    Set x = Nothing
End Function

'======================================================================================================
'作者:  张驹武 2007－07－25
'功能:  取Windows进程信息
'
' 参数 : a  要连接的服务器域名或IP
'        b  用户名
'        c  密码
'
' 返回 :
'        错误时：error=Error Number : 123 Error Source : source Error Description : 描述
'        正常时：getProcesses = 符合条件的串
'        无取得值时：getProcesses = 空串
'
'======================================================================================================
Public Function getProcesses(aa, b, c)
    On Error Resume Next
    
    Dim locator As New SWbemLocator
    Dim service As SWbemServices
    
    If Len(aa) = 0 And Len(b) = 0 And Len(c) = 0 Then
        Set service = locator.ConnectServer
    Else
        Set service = locator.ConnectServer(aa, "root\cimv2", b, c)
    End If
        
    Dim Porcesses, Threads, ProcessorQueueLength As Integer
    Dim colItems As SWbemObjectSet
    Dim objItem As SWbemObject

    Set colItems = service.ExecQuery("Select * From Win32_PerfRawData_PerfOS_System")
        
    For Each objItem In colItems
        Processes = Processes + objItem.Processes
        Threads = Threads + objItem.Threads
        ProcessorQueueLength = ProcessorQueueLength + objItem.ProcessorQueueLength
    Next
     
    Dim str As String
    
    str = str & "Processes=" & Processes & "$"
    str = str & "Threads=" & Threads & "$"
    str = str & "ProcessorQueueLength=" & ProcessorQueueLength & "$"
    
    ' 错误处理
    If Err.Number <> 0 Then
        getProcesses = "Error Number : " & Err.Number & vbCrLf & "Error Source : " & Err.Source & vbCrLf & "Error Description : " & Err.Description & "$"
   Else
        getProcesses = str
    End If
   
    Set service = Nothing
    Set locator = Nothing
    Set colItems = Nothing
    Set objItem = Nothing
End Function
