<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML//EN">
<html>

<head>
<meta http-equiv="Content-Type"
content="text/html; charset=iso-8859-1">
<meta name="Version" content="8.0.3429">
<meta name="Date" content="11/28/96">
<meta name="Template" content="D:\Isaoff97\Office\HTML.DOT">
<meta name="GENERATOR" content="Microsoft FrontPage 5.0">
<title>C NetProg Library : MXAddEndingProcedure</title>
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
    <td valign="top" width="100%"><b><font face="Verdana" size="4" color="#000080">MXAddEndingProcedure</font></b></td>
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
          <td width="50%"><b><font face="Verdana" color="#000080" size="2">int        MXAddEndingProcedure (MXCom* pcom, int (*funct)(MXCom* pcom, void* applicationfield), void* applicationfield)</font></b></td>
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
          <td width="10%" align="left"><font face="Verdana" color="#000080" size="2"><i>pcom</i></font>&nbsp;</td>
          <td width="20%" align="left"><font face="Verdana" size="2" color="#000080">the
            connection instance</font></td>
  </tr>
  
  <tr>
          <td width="10%" align="left"><font face="Verdana" color="#000080" size="2"><i>funct</i></font>&nbsp;</td>
          <td width="20%" align="left"><font face="Verdana" size="2" color="#000080">the
            function to call when&nbsp; the connection is closed&nbsp;</font></td>
  </tr>
  <tr>
          <td width="10%" align="left"><font face="Verdana" color="#000080" size="2"><i>applicationfield</i></font>&nbsp;</td>
          <td width="20%" align="left"><font face="Verdana" size="2" color="#000080">Programmer-specified
            data sent to the callback function</font></td>
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

      <p style="margin-bottom: 0"><font color="#000080" face="Verdana" size="2">This function adds a
      callback to the connection instance <i>pcom</i>. The callback is triggered when
      the </font><font color="#000080" face="Verdana" size="2">connection instance
      <i>pcom</i> is closed.</font></p>

      <p style="margin-top: 0; margin-bottom: 0"><font color="#000080" face="Verdana" size="2">If an error occurs on
      the connection&nbsp; the MX manager automatically calls the callback <i>funct</i>&nbsp;
      and closes the connection</font></p>

<p style="margin-top: 0; margin-bottom: 0"><font face="Verdana" size="2" color="#000080"><i>applicationfield</i> specifies
the argument that is to be passed to the specified procedure when it is invoked</font></p>

<p style="margin-top: 0; margin-bottom: 0"><br>
</p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top"><font color="#000080" size="2" face="Verdana"><b>Return
      Values :&nbsp;</b> </font><font face="Verdana" size="2" color="#000080">if error
      -1 else returns a value &gt; 0</font><b><br>
      </b><font face="Verdana" size="2" color="#000080"></font></td>
  </tr>
</table>

<p><br>
</p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top"><font color="#000080" size="2" face="Verdana"><b>See Also
      :</b> <a href="MXAddEndingProcedure.html">MXRemoveEndingProcedure</a>, <a href="MXAddConnectCallBack.html">MXAddConnectCallBack</a></font>,
      <font color="#000080" size="2" face="Verdana"><a href="MXRemoveConnectCallBack.html">MXRemoveConnectCallBack</a>, <a href="MXAddGeneralConnectCallBack.html">MXAddGeneralConnectCallBack</a>,&nbsp;
      <a href="MXRemoveGeneralConnectCallBack.html">MXRemoveGeneralConnectCallBack</a></font>
      <font color="#000080" size="2" face="Verdana"><b>&nbsp;<br>
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

<p><font face="Verdana" size="2" color="#000080">int OnCloseFile (MXCom* pcom, void* appfield)<br>
{<br>
&nbsp; grConnexionFichier = <a href="MXOpenFile.html"> MXOpenFile</a> (pcom-&gt;MX, &quot;MyFileName&quot;, "w",
IOPROTO_STORE, 'D', 63);<br>
&nbsp; <a href="MXCloseCom.html">MXCloseCom</a> (pcom-&gt;MX, grConnexionFichier);<br>
&nbsp; grConnexionFichier = NULL;<br>
&nbsp; return 1;<br>
}</font><br>
<br>
<font face="Verdana" size="2" color="#000080">
<br>
</font>
</p>

      <p><font size="2" face="Verdana" color="#000080">&nbsp;&nbsp; ...</font>

      <p><font size="2" face="Verdana" color="#000080">int main ()<br>
      {</font>

      <p><font face="Verdana" size="2" color="#000080">&nbsp; grConnexionFichier =
      <a href="MXOpenFile.html"> MXOpenFile</a> (pcom-&gt;MX, &quot;MyFileName&quot;, "r", IOPROTO_STORE, 'D', 63);<br>
      &nbsp; if (grConnexionFichier)&nbsp;<br>
      &nbsp; {<br>
      &nbsp;&nbsp;&nbsp;&nbsp; MXMessage* pmessage;<br>
      &nbsp;&nbsp;&nbsp;&nbsp; <a href="MXAddEndingProcedure.html">MXAddEndingProcedure</a> (grConnexionFichier,  OnCloseFile, NULL);<br>
      &nbsp;&nbsp;&nbsp;&nbsp; while (pmessage = <a href="MXRecv.html"> MXRecv</a> (pcom->MX, grConnexionFichier))<br>
      &nbsp;&nbsp;&nbsp;&nbsp; {<br>
      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <a href="MXPutThisMessage.html">MXPutThisMessage</a> (pcom, pmessage);<br>
      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <a href="MXFreeMessage.html">MXFreeMessage</a> (pmessage);<br>
      &nbsp;&nbsp;&nbsp;&nbsp; }<br>
      &nbsp;&nbsp;&nbsp;&nbsp; <a href="MXCloseCom.html">MXCloseCom</a> (pcom->MX, grConnexionFichier);<br>
      &nbsp; }<br>
      </font>

      <p><font size="2" face="Verdana" color="#000080">&nbsp; ...</font>

      <p><font color="#000080" face="Verdana" size="2">}</font></td>
    <td>&nbsp;</td>
  </tr>
</table>
</body>
</html>