编译 ECC 各模块的重要说明：

（1）凡是涉及到公共部分的 .h, .cpp, .dll, .lib，以及 snmp 等，都放在这个目录下
（2）一定要把 Ecc_common 放在 c:\siteview 下，即：
     c:\siteview\Ecc_Common 
（3）本目录里面没有的，请程序员把用到的也一定复制到这个目录，并提交到svn