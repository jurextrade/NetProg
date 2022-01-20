<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML//EN">
<html>

<head>
<meta http-equiv="Content-Type"
content="text/html; charset=iso-8859-1">
<meta name="Version" content="8.0.3429">
<meta name="Date" content="11/28/96">
<meta name="Template" content="D:\Isaoff97\Office\HTML.DOT">
<meta name="GENERATOR" content="Microsoft FrontPage 5.0">
<title>C NetProg Library : MXCopyMessage</title>
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
    <td valign="top" width="100%"><b><font face="Verdana" size="4" color="#000080">MXCopyMessage</font></b></td>
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

<p><br>
</p>
      

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
          <td width="50%"><b><font face="Verdana" color="#000080" size="2">MXMessage* MXCopyMessage (MXMessage*
            frommessage)</font></b></td>
        </tr>
      </table>
      <p><font face="Verdana" size="2" color="#000080"><b></b></font>
    </td>
  </tr>
</table>

      
<p><br>
</p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td width="10%" height="21" valign="top"><b><font color="#000080" size="2" face="Verdana">Parameters</font></b></td>
    <td bordercolor="#000080" bordercolorlight="#000080" bordercolordark="#000080" height="21" valign="top"><b><font color="#000080" size="2" face="Verdana">Description</font></b></td>
  </tr>
  
  <tr>
          <td width="10%" align="left"><font face="Verdana" color="#000080" size="2"><i>frommessage</i></font>&nbsp;</td>
          <td width="20%" align="left"><font face="Verdana" size="2" color="#000080">the
            message instance to copy</font></td>
  </tr>
  
</table>

<p><br>
</p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top" width="100%">
      <p align="left"><font color="#000080" size="2" face="Verdana"><b>Description
      :&nbsp;</b></font><b><br>
      </b></p>
    </td>
  </tr>
</table>

<p><font face="Verdana" size="2" color="#000080">Copy the message <i>frommessage</i>
in a new instance</font></p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top"><font color="#000080" size="2" face="Verdana"><b>Return
      Values :</b><b></b> returns a copy of the message</font><b><br>
      </b><font face="Verdana" size="2" color="#000080"></font></td>
  </tr>
</table>

<p><br>
</p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top"><font color="#000080" size="2" face="Verdana"><b>See Also
      :</b><b>&nbsp;</b><a href="MXPutThisMessage.html">MXPutThisMessage</a><b><br>
      </b>
      </font><font size="2" face="Verdana"></font></td>
  </tr>
</table>

<p><br>
</p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top" width="100%"><font color="#000080" size="2" face="Verdana"><b>Example
      :</b></font><b><br>
      </b><font face="Verdana" size="2" color="#000080"></font></td>
  </tr>
</table>

<p><br>
<font face="Verdana" color="#008000" size="2"><b>/* Navigator should connect on
8080 port */</b></font></p>
<p><font face="Verdana" size="2" color="#000080">#include "mx.h"<br>
#include "http.h"<br>
<br>
DWORD NbOpen = 0;<br>
DWORD NbOpen1 = 0;<br>
<br>
int OnCloseClient (MXCom* pcom, void* appfield)<br>
{<br>
&nbsp; MXCom* ServerCom = MXGetComApplicationField (pcom);<br>
&nbsp; printf ("===> Navigator closed = %d\n", --NbOpen1);&nbsp;<br>
&nbsp; if (!ServerCom)<br>
&nbsp;&nbsp;&nbsp; return 1;<br>
&nbsp; return 1;<br>
}<br>
<br>
int OnCloseServer (MXCom* pcom, void* appfield)<br>
{<br>
&nbsp; MXCom* ClientCom = = MXGetComApplicationField (pcom);<br>
&nbsp; int ret = 0;<br>
&nbsp; printf ("===> Server closed = %d\n", --NbOpen);&nbsp;<br>
&nbsp; if (!ClientCom)&nbsp;<br>
&nbsp; return 1;<br>
&nbsp; ret = <a href="MXFlushCom.html"> MXFlushCom</a> (pcom-&gt;MX, ClientCom);<br>
&nbsp; return 1;<br>
}<br>
<br>
int RecvResponse (MXMessage* pmessage, MXCom* pcom, void* AppField)<br>
{<br>
&nbsp; MXCom* ClientCom = (MXCom*)AppField;<br>
&nbsp; BUFFERPARM* Buffer;<br>
<br>
&nbsp; MXMessage* CopyMessage = <a href="MXCopyMessage.html"> MXCopyMessage</a> (pmessage);<br>
&nbsp; Buffer = (BUFFERPARM*)<a href="MXGetValue.html">MXGetValue</a> (pmessage, "Content", 1);<br>
<br>
&nbsp; if (!ClientCom->Client)<br>
&nbsp;&nbsp; printf ("Oupssssss\n");<br>
&nbsp; else<br>
&nbsp;&nbsp; <a href="MXPutThisMessage.html">MXPutThisMessage</a> (ClientCom, CopyMessage);<br>
&nbsp; return 1;<br>
}<br>
</font></p>
<p><font face="Verdana" size="2" color="#000080">int RecvRequest (MXMessage* pmessage, MXCom* pcom, void* AppField)<br>
{<br>
&nbsp; MXCom* webpcom;<br>
<br>
&nbsp; STRING server = <a href="MXGetValue.html"> MXGetValue</a> (pmessage, "Host", 1);<br>
&nbsp; STRING Request = (STRING)MXGetValue (pmessage, "Request-Line", 1);<br>
<br>
&nbsp; MXMessage* CopyMessage = <a href="MXCopyMessage.html">MXCopyMessage</a> (pmessage);<br>
<br>
&nbsp; webpcom = <a href="MXOpenTCP.html">MXOpenTCP</a> (pcom->MX, server, 80, IPPROTO_HTTP, NULL, NULL, MXBlock);<br>
&nbsp; if (!webpcom) return 1;<br>
<br>
&nbsp; <a href="MXAddEndingProcedure.html">MXAddEndingProcedure</a> (webpcom, OnCloseServer, pcom);<br>
&nbsp; MXAddEndingProcedure (pcom,    OnCloseClient, webpcom);<br>
<br>
&nbsp; <a href="MXAddComCallBack.html">MXAddComCallBack</a> (pcom->MX, webpcom, HTTP_SYS, "Response",MXONRECV, RecvResponse, pcom);&nbsp;<br>
&nbsp; MXSetComApplicationField (pcom, webpcom);<br>
&nbsp; MXSetComApplicationField (webpcom, pcom);<br>
<br>
&nbsp; printf ("===> Open a new connection %d\n", ++NbOpen);<br>
&nbsp; CopyMessage = MXCopyMessage (pmessage);&nbsp;<br>
&nbsp; <a href="MXPutThisMessage.html">MXPutThisMessage</a> (webpcom, CopyMessage);<br>
<br>
&nbsp; return 1;<br>
}<br>
<br>
int OnConnect (MXCom* pcom, void* appfield)<br>
{<br>
&nbsp; if (MXGetPort (pcom) != 8080)  return 1;<br>
&nbsp; printf ("===> Navigator opened = %d\n", ++NbOpen1);&nbsp;<br>
&nbsp; <a href="MXAddComCallBack.html">MXAddComCallBack</a> (pcom->MX, pcom, HTTP_SYS, "Request",  MXONRECV, RecvRequest, NULL);&nbsp;<br>
&nbsp; return 1;<br>
}</font><br>
</p>
<p><font face="Verdana" size="2" color="#000080">&nbsp;int main(int argc, char* argv[])<br>
{<br>
&nbsp; MX mx;<br>
<br>
&nbsp; <a href="MXInit.html">MXInit</a> (&amp;mx, MXSERVER, NULL, NULL, 0, NULL);<br>
&nbsp; <a href="MXAddPort.html">MXAddPort</a> (&amp;mx, 8080, IPPROTO_HTTP);<br>
<br>
&nbsp; <a href="MXAddGeneralConnectCallBack.html">MXAddGeneralConnectCallBack</a> (&amp;mx, MXONCONNECT, OnConnect, &amp;mx);<br>
<br>
&nbsp; <a href="MXDispatchEvents.html">MXDispatchEvents</a> (&amp;mx, 0);<br>
<br>
&nbsp; <a href="MXEnd.html">MXEnd</a> (&amp;mx);<br>
}</font><br>
<br>
</p>

      <p>&nbsp;</td>
    <td>&nbsp;</td>
  </tr>
</table>
</body>
</html>