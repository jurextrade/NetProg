<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML//EN">
<html>

<head>
<meta http-equiv="Content-Type"
content="text/html; charset=iso-8859-1">
<meta name="Version" content="8.0.3429">
<meta name="Date" content="11/28/96">
<meta name="Template" content="D:\Isaoff97\Office\HTML.DOT">
<meta name="GENERATOR" content="Microsoft FrontPage 5.0">
<title>C NetProg Library : MXCreateComClass</title>
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
    <td valign="top" width="100%"><b><font face="Verdana" size="4" color="#000080">MXCreateComClass</font></b></td>
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
          <td width="50%"><b><font face="Verdana" color="#000080" size="2">MXComClass* MXCreateComClass (MX* pmx, char* name, WORD code, int protocol, BYTE fromto, char* tablename)</font></b></td>
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
          <td width="10%" align="left"><font face="Verdana" color="#000080" size="2"><i>pmx</i></font>&nbsp;</td>
          <td width="20%" align="left"><font face="Verdana" size="2" color="#000080">the
            MX manager</font></td>
  </tr>
  
  <tr>
          <td width="10%" align="left"><font face="Verdana" color="#000080" size="2"><i>name</i></font>&nbsp;</td>
          <td width="20%" align="left"><font face="Verdana" size="2" color="#000080">the
            name of the connection class to create</font></td>
  </tr>
  
  <tr>
          <td width="10%" align="left"><font face="Verdana" color="#000080" size="2"><i>code</i></font>&nbsp;</td>
          <td width="20%" align="left"><font face="Verdana" size="2" color="#000080">the
            identifier of the connection class</font></td>
  </tr>
  
  <tr>
          <td width="10%" align="left"><font face="Verdana" color="#000080" size="2"><i>protocol</i></font>&nbsp;</td>
          <td width="20%" align="left"><font face="Verdana" size="2" color="#000080">the
            protocol identifier of the connection class</font></td>
  </tr>
  
  <tr>
          <td width="10%" align="left"><font face="Verdana" color="#000080" size="2"><i>fromto</i></font>&nbsp;</td>
          <td width="20%" align="left"><font face="Verdana" size="2" color="#000080">the
            transcoding side of the connection</font></td>
  </tr>
  
  <tr>
          <td width="10%" align="left"><font face="Verdana" color="#000080" size="2"><i>tablename</i></font>&nbsp;</td>
          <td width="20%" align="left"><font face="Verdana" size="2" color="#000080">the
            transcodification table name for the connection class</font></td>
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

<dl>
  <dt><font face="Verdana" size="2" color="#000080">Creates a connection class.
    This function is equivalent to the CONNETIONCLASS section in the MX Resource
    File. The <i>code</i> and <i>name</i> must be unique in the application.
    Both sides of the communicating applications should have the same <i>code</i>
    and <i>name</i></font>.&nbsp;<br>
    <br>
    <font face="Verdana" size="2" color="#000080">The parameter <i>protocol</i>
    represents the protocol to use for all connection instances of this
    class.&nbsp;<br>
    Supported values are :<br>
    <br>
    </font></dt>
  <table border="0" cellpadding="0" cellspacing="0" width="100%">
    <tr>
      <td width="27%"><font color="#000080" face="Verdana" size="2"><b>protocol
        parameter</b></font></td>
      <td width="73%"></td>
      <td width="73%">
        <p align="center"><font color="#000080" face="Verdana" size="2"><b>DIALOG
        CLASS</b></font></td>
    </tr>
    <tr>
      <td width="27%"><font size="2" color="#000080" face="Verdana">IPPROTO_TCP&nbsp;</font></td>
      <td width="73%"><font color="#000080" face="Verdana" size="2">TCP protocol&nbsp;</font></td>
      <td width="73%"></td>
    </tr>
    <tr>
      <td width="27%"><p><font size="2" color="#000080" face="Verdana">IPPROTO_FTP&nbsp;<br>
        </font></p>

      </td>
      <td width="73%"><font color="#000080" face="Verdana" size="2">FTP protocol</font></td>
      <td width="73%"></td>
    </tr>
    <tr>
      <td width="27%"><font size="2" color="#000080" face="Verdana">IPPROTO_SMTP&nbsp;<br>
        </font></td>
      <td width="73%"><font color="#000080" face="Verdana" size="2">SMTP
        protocol</font></td>
      <td width="73%"></td>
    </tr>
    <tr>
      <td width="27%"><font size="2" color="#000080" face="Verdana">IPPROTO_DNS&nbsp;<br>
        </font></td>
      <td width="73%"><font color="#000080" face="Verdana" size="2">DNS protocol</font></td>
      <td width="73%"></td>
    </tr>
    <tr>
      <td width="27%"><font size="2" color="#000080" face="Verdana">IPPROTO_HTTP&nbsp;<br>
        </font></td>
      <td width="73%"><font color="#000080" face="Verdana" size="2">HTTP
        protocol</font></td>
      <td width="73%"></td>
    </tr>
    <tr>
      <td width="27%"><font size="2" color="#000080" face="Verdana">IPPROTO_POP&nbsp;<br>
        </font></td>
      <td width="73%"><font color="#000080" face="Verdana" size="2">POP protocol</font></td>
      <td width="73%"></td>
    </tr>
    <tr>
      <td width="27%"><font size="2" color="#000080" face="Verdana">IPPROTO_BC&nbsp;<br>
        </font></td>
      <td width="73%"><font color="#000080" face="Verdana" size="2">BC protocol
        (MX Defined)</font></td>
      <td width="73%"></td>
    </tr>
    <tr>
      <td width="27%"><font size="2" color="#000080" face="Verdana">IPPROTO_DG&nbsp;</font></td>
      <td width="73%"><font color="#000080" face="Verdana" size="2">DG protocol
        (MX Defined)</font></td>
      <td width="73%"></td>
    </tr>
    <tr>
      <td width="27%"><font color="#000080" face="Verdana" size="2">IOPROTO_STORE</font></td>
      <td width="73%"></td>
      <td width="73%"></td>
    </tr>
    <tr>
      <td width="27%"><font color="#000080" face="Verdana" size="2">IOPROTO_FIXED</font></td>
      <td width="73%"></td>
      <td width="73%"></td>
    </tr>
    <tr>
      <td width="27%"><font color="#000080" face="Verdana" size="2">DBPROTO_ODBC</font></td>
      <td width="73%"></td>
      <td width="73%"></td>
    </tr>
    <tr>
      <td width="27%"><font face="Verdana" size="2" color="#000080">DBPROTO_ORACLE&nbsp;</font></td>
      <td width="73%"></td>
      <td width="73%"></td>
    </tr>
    <tr>
      <td width="27%"><font size="2" face="Verdana" color="#000080">DBPROTO_SQLDS</font></td>
      <td width="73%"></td>
      <td width="73%"></td>
    </tr>
  </table>
</dl>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top"><font color="#000080" size="2" face="Verdana"><b>Return
      Values :</b><b>&nbsp;</b></font><b><br>
      </b><font face="Verdana" size="2" color="#000080"></font></td>
  </tr>
</table>

      <p><br></p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top"><font color="#000080" size="2" face="Verdana"><b>See Also
      :</b><b>&nbsp;<br>
      </b>
      </font><font size="2" face="Verdana"></font></td>
  </tr>
</table>

      <p><br></p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top" width="100%"><font color="#000080" size="2" face="Verdana"><b>Example
      :</b></font><b><br>
      </b><font face="Verdana" size="2" color="#000080"></font></td>
  </tr>
</table>

      <p>&nbsp;</td>
    <td>&nbsp;</td>
  </tr>
</table>
</body>
</html>