<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML//EN">
<html>

<head>
<meta http-equiv="Content-Type"
content="text/html; charset=iso-8859-1">
<meta name="Version" content="8.0.3429">
<meta name="Date" content="11/28/96">
<meta name="Template" content="D:\Isaoff97\Office\HTML.DOT">
<meta name="GENERATOR" content="Microsoft FrontPage 5.0">
<title>C NetProg Library : MXFlushCom</title>
</head>
<SCRIPT language=javascript src="2nd_roll_en.js" 
type=text/javascript></SCRIPT>

<SCRIPT language=JavaScript>
<!--
function MM_jumpMenu(targ,selObj,restore){ //v3.0
  eval(targ+".location='"+selObj.options[selObj.selectedIndex].value+"'");
  if (restore) selObj.selectedIndex=0;
}
//-->
</SCRIPT>

<body topmargin="0" leftmargin="0" link="#0000FF" vlink="#0099FF">



  <table border="0" cellpadding="0" cellspacing="0" width="100%" bgcolor="#FFFFE6" style="border-collapse: collapse" bordercolor="#111111">
    <tr>
      <td>

      <img border="0" src="../images/eagle.GIF" width="221" height="188">
      </td>
      <td width="100%" align="center">
  <div align="left">
  <table border="0" cellpadding="0" cellspacing="0" width="100%" bordercolorlight="#000080">
    <tr>
      <td width="100%" valign="baseline">
        <p align="center"><b><font face="Arial" size="7" color="#000080">NetProg</font></b></td>
    </tr>
    <tr>
      <td bgcolor="#000080" height="0">
        <p style="text-indent: -4; margin-top: -17">&nbsp;</td>
    </tr>
  </table>
      </div>
      </td>
    </tr>
  </table>



<table border="0" cellpadding="0" cellspacing="0" width="100%">
  <tr>
    <td width="175" valign="top" align="center" bgcolor="#FFFFE6">

      <FONT FACE='Arial,Geneva,Helvetica,san-serif' SIZE=-1>










<table width="220" cellspacing="0" cellpadding="0" style="border-collapse: collapse" bordercolor="#111111" bgcolor="#FFFFE6">
  <tr>
    <td width="220" colspan="4" height="100" align="center">

      <FONT FACE='Arial,Geneva,Helvetica,san-serif' SIZE=-1>










    <img src="../images/blank.gif" width="18" height="1" border="0"><p>
    <a href="http://sourceforge.net">
    <img border="0" src="http://sourceforge.net/sflogo.php?group_id=144520&amp;type=4"></a><br>


      </font>
    </td>
  </tr>
  <tr>
    <td width="14" valign="top">&nbsp;</td>
    <td width="18" valign="top">&nbsp;</td>
    <td colspan="2" width="188">
      <p><font FACE="Arial,Geneva,Helvetica,san-serif" SIZE="-1" color="#000080"><b>
      NETPROG&nbsp; PRESENTATION</b></font></p>
      <p>&nbsp;</p>
    </td>
  </tr>
  <tr>
    <td width="14" valign="top">&nbsp;</td>
    <td width="18" valign="top"><img SRC="../images/red_bullet.gif" BORDER="0" width="13" height="16"></td>
    <td valign="top" colspan="2" width="188"><font FACE="Arial,Geneva,Helvetica,san-serif" SIZE="-1">
    <a href="../index.html">OverView</a></font></td>
  </tr>
  <tr>
    <td width="14" valign="top">&nbsp;</td>
    <td width="18" valign="top"><img SRC="../images/red_bullet.gif" BORDER="0" width="13" height="16"></td>
    <td valign="top" colspan="2" width="188"><font FACE="Arial,Geneva,Helvetica,san-serif" SIZE="-1">
    <a href="../Functionalities.html">Functionalities</a></font></td>
  </tr>
  <tr>
    <td width="14" valign="top">&nbsp;</td>
    <td width="18" valign="top"><img SRC="../images/red_bullet.gif" BORDER="0" width="13" height="16"></td>
    <td valign="top" colspan="2" width="188"><b><a href="../Programming.html"><font face="Arial,Geneva,Helvetica,san-serif" size="2">
    Programming</font></a></b></td>
  </tr>










<tr>
  <td width="14" valign="top"></td>
  <td width="18" valign="top"></td>
  <td valign="top" width="17"><FONT FACE='Arial,Geneva,Helvetica,san-serif' SIZE=-1>
  <img SRC="../images/red_bullet.gif" BORDER="0" width="13" height="16"></font></td>


      </font>
<td valign="top" width="171"><b><a href="API_index.html">
<font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#FF9900">API</font></a></b></td>
</tr>

      <FONT FACE='Arial,Geneva,Helvetica,san-serif' SIZE=-1>










<tr>
  <td width="14" valign="top"></td>
  <td width="18" valign="top"></td>
  <td valign="top" width="17"><FONT FACE='Arial,Geneva,Helvetica,san-serif' SIZE=-1>
  <img SRC="../images/red_bullet.gif" BORDER="0" width="13" height="16"></font></td>


      </font>
<td valign="top" width="171"><a href="../Prog_C_Examples.html"><font face="Arial,Geneva,Helvetica,san-serif" size="2">
C Examples</font></a></td>
</tr>










<font FACE="Arial,Geneva,Helvetica,san-serif" SIZE="-1">
<tr>
  <td width="14" valign="top">&nbsp;</td>
  <td width="18" valign="top"><img SRC="../images/red_bullet.gif" BORDER="0" width="13" height="16"></td>
  <td valign="top" colspan="2" width="188"><font face="Arial,Geneva,Helvetica,san-serif" size="2">
  <a href="https://sourceforge.net/project/showfiles.php?group_id=144520">
  Download</a></font></td>
</tr>
</table>
</font>


      </td>
    <td>

<table border="0" width="100%">
  <tr>
    <td valign="top" width="100%"><b><font face="Verdana" size="4" color="#000080">MXFlushCom</font></b></td>
    <td valign="baseline" align="right" nowrap><font face="Arial,Geneva,Helvetica,san-serif" size="2"><b><font color="#FF9900">C
      NetProg Library&nbsp;</font></b></font></td>
  </tr>
  <tr>
    <td valign="top" colspan="2" bgcolor="#FFAE1F" width="100%">
      <table border="0" cellpadding="0" cellspacing="0" width="100%">
        <tr>
          <td width="100%"></td>
        </tr>
      </table>
    </td>
  </tr>
</table>

<p><br></p>
      

<table border="0" width="100%" cellspacing="0" cellpadding="8">
  <tr>
    <td align="left" valign="top"><font face="Verdana" size="2" color="#000080">#include
      &quot;mx.h&quot;</font>
      <table border="0" cellpadding="0" cellspacing="0" width="100%">
        <tr>
          <td width="0%"></td>
          <td width="50%"></td>
        </tr>
        <tr>
          <td width="0%"></td>
          <td width="50%"><b><font face="Verdana" color="#000080" size="2">int        MXFlushCom (MX* pmx, MXCom* pcom)</font></b></td>
        </tr>
      </table>
    </td>
  </tr>
</table>

      
      <p><br></p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td width="10%" height="21" valign="top"><b><font color="#000080" size="2" face="Verdana">Parameters</font></b></td>
    <td bordercolor="#000080" bordercolorlight="#000080" bordercolordark="#000080" height="21" valign="top"><b><font color="#000080" size="2" face="Verdana">Description</font></b></td>
  </tr>
  
  <tr>
          <td width="10%" align="left"><font face="Verdana" color="#000080" size="2"><i>pmx</i></font>&nbsp;</td>
          <td width="20%" align="left"><font face="Verdana" size="2" color="#000080">the
            MX manager</font></td>
  </tr>
  
  <tr>
          <td width="10%" align="left"><font face="Verdana" color="#000080" size="2"><i>pcom</i></font>&nbsp;</td>
          <td width="20%" align="left"><font face="Verdana" size="2" color="#000080">the
            connection instance</font></td>
  </tr>
  
</table>

      <p><br></p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top" width="100%">
      <p align="left"><font color="#000080" size="2" face="Verdana"><b>Description
      :&nbsp;</b></font><b><br>
      </b></p>
    </td>
  </tr>
</table>

<p><font face="Verdana" size="2" color="#000080">This function flushes the
output queue of a connection. Messages in the output queue of a connection are
sent by the MX manager, in a scheduled manner. If an application wants to close
a connection it uses this function to send all messages still pending.&nbsp;
Notice that an application that works in an asynchronous mode uses MXPutMessage
which only puts the message in the output queue. The messages in the output are
sent by <a href="MXDispatchEvents.html">MXDispatchEvents</a>&nbsp;</font></p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top"><font color="#000080" size="2" face="Verdana"><b>Return
      Values :</b><b>&nbsp;</b></font><font face="Verdana" color="#000080" size="2">
      if error returns &lt; 0 else returns &gt;= 0</font><b><br>
      </b><font face="Verdana" size="2" color="#000080"></font></td>
  </tr>
</table>

      <p><br></p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top"><font color="#000080" size="2" face="Verdana"><b>See Also
      :</b><b>&nbsp;</b></font><a href="MXSend.html"><font face="Verdana" size="2">MXSend</font></a><font color="#000080" size="2" face="Verdana"><b><br>
      </b>
      </font><font size="2" face="Verdana"></font></td>
  </tr>
</table>

      <p><br></p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top" width="100%"><font color="#000080" size="2" face="Verdana"><b>Example
      :</b></font> &nbsp; <a href="MXSend.html"><font face="Verdana" size="2"></font></a><b><br>
      </b><font face="Verdana" size="2" color="#000080"></font></td>
  </tr>
</table>

<p>&nbsp; ...</p>
<p><font face="Verdana" size="2" color="#000080">   if (STORE)<br>
   {<br>
&nbsp; char FileType = 't';<br>
<br>
&nbsp; FileCom = <a href="MXOpenFile.html"> MXOpenFile</a> (&amp;mx, &quot;FTP.sto&quot;, "w", IOPROTO_STORE, 'D', 63);<br>
&nbsp; if (!FileCom) return 0;&nbsp;<br>
<br>
&nbsp; pmessage = <a href="MXPutMessage.html"> MXPutMessage</a> (FileCom,
FTP_SYS, "Command");<br>
&nbsp; <a href="MXSetValue.html">MXSetValue</a> (pmessage, "Value", 1, "USER");<br>
&nbsp; MXSetValue (pmessage, "Par",   1, "bartocci");<br>
<br>
&nbsp; pmessage = MXPutMessage (FileCom, FTP_SYS, "Command");<br>
&nbsp; MXSetValue (pmessage, "Value", 1, "PASS");<br>
&nbsp; MXSetValue (pmessage, "Par",   1, &quot;pass&quot;);<br>
<br>
&nbsp; pmessage = MXPutMessage (FileCom, FTP_SYS, "dir");<br>
&nbsp; MXSetValue (pmessage, "Value", 1, "LIST");<br>
&nbsp; MXSetValue (pmessage, "Par",   1, "");<br>
<br>
&nbsp; pmessage = MXPutMessage (FileCom, FTP_SYS, "get") ;<br>
&nbsp; MXSetValue(pmessage, "FileName", 1, "t.c") ;<br>
&nbsp;	MXSetValue(pmessage, "DestName", 1, "t.c") ;<br>
&nbsp;	MXSetValue(pmessage, "FileType", 1, &amp;FileType) ;<br>
&nbsp;	MXSetValue(pmessage, "Command",  1, "get") ;<br>
<br>
&nbsp; <a href="MXFlushCom.html">MXFlushCom</a> (&amp;mx, FileCom);<br>
<br>
&nbsp; <a href="MXCloseCom.html">MXCloseCom</a> (&amp;mx, FileCom);<br>
<br>
&nbsp; FTPCom = <a href="MXOpenTCP.html">MXOpenTCP</a> (&amp;mx, &quot;194.214.100.8&quot;, 21, IPPROTO_FTP, NULL, NULL, TRUE);<br>
&nbsp; if (!FTPCom) return 0;<br>
<br>
&nbsp; <a href="MXAddComCallBack.html">MXAddComCallBack</a> (&amp;mx, FTPCom, FTP_SYS, "Command",        MXONSEND, Command,      NULL);<br>
&nbsp; MXAddComCallBack (&amp;mx, FTPCom, FTP_SYS, "Command_Reply",  MXONRECV, CommandReply, NULL);<br>
&nbsp; MXAddComCallBack (&amp;mx, FTPCom, FTP_SYS, "dir_reply",      MXONRECV, DirReply,     NULL);<br>
&nbsp; MXAddComCallBack (&amp;mx, FTPCom, FTP_SYS, "get_reply",      MXONRECV, GetReply,     NULL);<br>
<br>
&nbsp; FileCom = <a href="MXOpenFile.html"> MXOpenFile</a> (&amp;mx, &quot;FTP.sto&quot;, "r", IOPROTO_STORE, 'D', 63);<br>
&nbsp; if (!FileCom) return 0;<br>
<br>
&nbsp; while (pmessage = <a href="MXRecv.html"> MXRecv</a> (&amp;mx, FileCom))<br>
&nbsp; {<br>
&nbsp;&nbsp;&nbsp; <a href="MXPutMessage.html">MXPutThisMessage</a> (FTPCom, pmessage);&nbsp;&nbsp;&nbsp;<br>
&nbsp; }<br>
&nbsp; <a href="MXCloseCom.html">MXCloseCom</a> (&amp;mx, FileCom);<br>
   }</font><br>
<br>
&nbsp; ...</p>
<p><font face="Verdana" size="2" color="#000080">&nbsp; <a href="MXDispatchEvents.html">MXDispatchEvents</a> (&amp;mx, 0);<br>
&nbsp; <a href="MXEnd.html">MXEnd</a> (&amp;mx);<br>
&nbsp; return 0;</font><br>
</p>

      <p>&nbsp;</td>
    <td>&nbsp;</td>
  </tr>
</table>
</body>
</html>