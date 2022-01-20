<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML//EN">
<html>

<head>
<meta http-equiv="Content-Type"
content="text/html; charset=iso-8859-1">
<meta name="Version" content="8.0.3429">
<meta name="Date" content="11/28/96">
<meta name="Template" content="D:\Isaoff97\Office\HTML.DOT">
<meta name="GENERATOR" content="Microsoft FrontPage 5.0">
<title>C NetProg Library : MXAddPort</title>
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
    <td valign="top" width="100%"><b><font face="Verdana" size="4" color="#000080">MXAddPort</font></b></td>
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
          <td width="50%"><b><font face="Verdana" color="#000080" size="2">int        MXAddPort (MX*
            pmx, int port, int protocol)</font></b></td>
        </tr>
      </table>
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
          <td width="10%" align="left"><font face="Verdana" color="#000080" size="2"><i>pmx</i></font>&nbsp;</td>
          <td width="20%" align="left"><font face="Verdana" size="2" color="#000080">the
            MX manager</font></td>
  </tr>
  
  <tr>
          <td width="10%" align="left"><font face="Verdana" color="#000080" size="2"><i>port</i></font>&nbsp;</td>
          <td width="20%" align="left"><font face="Verdana" size="2" color="#000080">the
            port listening number&nbsp;</font></td>
  </tr>
  
  <tr>
          <td width="10%" align="left"><font face="Verdana" color="#000080" size="2"><i>protocol</i></font>&nbsp;</td>
          <td width="20%" align="left"><font face="Verdana" size="2" color="#000080">the
            protocol used for this port</font></td>
  </tr>
  
</table>

<p><br>
</p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top" width="100%">
      <p align="left"><font color="#000080" size="2" face="Verdana"><b>Description
      :&nbsp;</b></font></p>
    </td>
  </tr>
</table>

      <p><font color="#000080" size="2" face="Verdana">This function adds a listening port allowing the application to
      accept connections on this <i>port</i>. The <i> protocol</i> indicated in the function
      parameter is set for all
      connections on this <i> port</i></font></p>

<p><font face="Verdana" size="2" color="#000080">The protocols supported are :</font></p>
<table border="1" cellpadding="0" cellspacing="0" width="90%">
  <tr>
    <td width="20%" valign="middle" align="center" bgcolor="#E9E9E9"><b><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">IP </font></b></td>
    <td width="10%" valign="middle" align="center" bgcolor="#E9E9E9"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080"><b>client</b></font></td>
    <td width="10%" valign="middle" align="center" bgcolor="#E9E9E9"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080"><b>server</b></font></td>
    <td width="20%" valign="middle" align="center" bgcolor="#E9E9E9"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080"><b>Connection</b></font></td>
    <td width="20%" valign="middle" align="center" bgcolor="#E9E9E9"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080"><b>Dialog</b></font></td>
  </tr>
  <tr>
    <td width="20%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">TCP</font></td>
    <td width="10%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">yes</font></td>
    <td width="10%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">yes</font></td>
    <td width="20%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">IP_PROTO_TCP</font></td>
    <td width="20%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">TCP</font></td>
  </tr>
  <tr>
    <td width="20%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">FTP</font></td>
    <td width="10%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">yes</font></td>
    <td width="10%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">no</font></td>
    <td width="20%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">IP_PROTO_FTP</font></td>
    <td width="20%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">FTP</font></td>
  </tr>
  <tr>
    <td width="20%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">HTTP</font></td>
    <td width="10%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">yes</font></td>
    <td width="10%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">no</font></td>
    <td width="20%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">IP_PROTO_HTTP</font></td>
    <td width="20%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">HTTP</font></td>
  </tr>
  <tr>
    <td width="20%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">SMTP</font></td>
    <td width="10%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">yes</font></td>
    <td width="10%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">no</font></td>
    <td width="20%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">IP_PROTO_SMTP</font></td>
    <td width="20%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">SMTP</font></td>
  </tr>
  <tr>
    <td width="20%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">POP</font></td>
    <td width="10%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">yes</font></td>
    <td width="10%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">no</font></td>
    <td width="20%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">IP_PROTO_POP</font></td>
    <td width="20%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">POP</font></td>
  </tr>
  <tr>
    <td width="20%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">DNS</font></td>
    <td width="10%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">yes</font></td>
    <td width="10%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">no</font></td>
    <td width="20%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">IP_PROTO_DNS</font></td>
    <td width="20%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">DNS</font></td>
  </tr>
  <tr>
    <td width="20%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">DG</font></td>
    <td width="10%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">yes</font></td>
    <td width="10%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">yes</font></td>
    <td width="20%" valign="middle" align="center"><font face="Arial,Geneva,Helvetica,san-serif" size="2" color="#000080">IP_PROTO_DG</font></td>
    <td width="20%" valign="middle" align="center"><font color="#000080" face="Arial,Geneva,Helvetica,san-serif" size="2"><i>USER_DEFINED</i></font></td>
  </tr>
</table>
<p><font face="Verdana" size="2" color="#000080">For example , if an application
wants to write an FTP server, it adds the protocol IPPROTO_FTP on the port 21,
and write the callbacks related to the message classes of the predefined FTP
Dialog class</font></p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top"><font color="#000080" size="2" face="Verdana"><b>Return
      Values :</b> -1 if error else &gt; 0<b>&nbsp;</b></font><b><br>
      </b><font face="Verdana" size="2" color="#000080"></font></td>
  </tr>
</table>

<p><br>
</p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top"><font color="#000080" size="2" face="Verdana"><b>See Also
      :</b> <a href="MXGetPort.html">MXGetPort</a><b>&nbsp;<br>
      </b>
      </font><font size="2" face="Verdana"></font></td>
  </tr>
</table>

<p><br>
</p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top" width="100%"><font color="#000080" size="2" face="Verdana"><b>Example
      :</b></font>
      <p><font color="#000080" size="2" face="Verdana">int OnConnect (MXCom*
      pcom, void* appfield)<br>
      {<br>
      &nbsp; if (<a href="MXGetPort.html">MXGetPort</a>  (pcom) != 8080)  return 1;<br>
      &nbsp; printf ("===> Nbr connection opened= %d\n", ++NbOpen1);&nbsp;<br>
      &nbsp; <a href="MXAddComCallBack.html">MXAddComCallBack</a>  (pcom-&gt;MX,
      pcom, HTTP_SYS, &quot;Request&quot;, MXONRECV, RecvRequest, NULL);&nbsp;<br>
      &nbsp; return 1;<br>
      }<br>
      </font></p>
      <p><font color="#000080" size="2" face="Verdana"><br>
      int main(int argc, char* argv[])<br>
      {<br>
      <br>
      &nbsp; MX mx;<br>
      &nbsp;<br>
      &nbsp; <a href="MXInit.html">MXInit</a> (&amp;mx, MXSERVER, NULL, NULL, 0,
      NULL);<br>
      &nbsp; <a href="MXAddPort.html">MXAddPort</a> (&amp;mx, 8080,
      IPPROTO_HTTP);<br>
      &nbsp; <a href="MXAddGeneralConnectCallBack.html">MXAddGeneralConnectCallBack</a> (&amp;mx,
      MXONCONNECT, OnConnect, &amp;mx);<br>
      &nbsp; <a href="MXDispatchEvents.html">MXDispatchEvents</a> (&amp;mx, 0);<br>
      &nbsp; <a href="MXEnd.html">MXEnd</a> (&amp;mx);<br>
      }<b><br>
      <br>
      </b></font><b><br>
      </b><font face="Verdana" size="2" color="#000080"></font></p>
    </td>
  </tr>
</table>

      <p>&nbsp;</td>
    <td>&nbsp;</td>
  </tr>
</table>
</body>
</html>