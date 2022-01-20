<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML//EN">
<html>

<head>
<meta http-equiv="Content-Type"
content="text/html; charset=iso-8859-1">
<meta name="Version" content="8.0.3429">
<meta name="Date" content="11/28/96">
<meta name="Template" content="D:\Isaoff97\Office\HTML.DOT">
<meta name="GENERATOR" content="Microsoft FrontPage 5.0">
<title>C NetProg Library : MXSetValue</title>
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
    <td valign="top" width="100%"><b><font face="Verdana" size="4" color="#000080">MXSetValue</font></b></td>
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
          <td width="50%"><b><font face="Verdana" color="#000080" size="2">int        MXSetValue (MXMessage* pmessage, char* name, int range, void* value)</font></b></td>
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
          <td width="10%" align="left"><font face="Verdana" color="#000080" size="2"><i>pmessage</i></font>&nbsp;</td>
          <td width="20%" align="left"><font face="Verdana" size="2" color="#000080">the
            message instance</font></td>
  </tr>
  
  <tr>
          <td width="10%" align="left"><font face="Verdana" size="2" color="#000080"><i>name</i>&nbsp;</font></td>
          <td width="20%" align="left"><font face="Verdana" size="2" color="#000080">the
            name of the object</font></td>
  </tr>
  
  <tr>
          <td width="10%" align="left"><font face="Verdana" size="2" color="#000080"><i>range</i>&nbsp;</font></td>
          <td width="20%" align="left"><font face="Verdana" size="2" color="#000080">the
            range of the object</font></td>
  </tr>
  
  <tr>
          <td width="10%" align="left"><font face="Verdana" size="2" color="#000080"><i>value</i>&nbsp;</font></td>
          <td width="20%" align="left"><font face="Verdana" size="2" color="#000080">the
            value to set</font>&nbsp;</td>
  </tr>
  
</table>

     <p><br></p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top" width="100%">
      <p align="left"><font face="Verdana" size="2" color="#000080"><b>Description
      :&nbsp;<br>
      </b></font></p>
    </td>
  </tr>
</table>

<p><font face="Verdana" size="2" color="#000080">This function set values to
objects in the message <i>pmessage</i>. The <i>name</i> and the <i>range</i> parmeters
represent the name and the range of the object .&nbsp;&nbsp;</font></p>

<p><font face="Verdana" size="2" color="#000080">The <i>value</i> specifies a
pointer to the value to set</font></p>
<p><font face="Verdana" size="2" color="#000080">To understand we suppose a
dialog class defined in the resource file as follow :</font></p>
<blockquote>
  <blockquote>
<p><font face="Verdana" color="#000080" size="2">DIALOGCLASS 1 *mydialog*<br>
BEGIN<br>
&nbsp; MESSAGECLASS Ask (STRING session[2] STRING passwd1)<br>
&nbsp; MESSAGECLASS reply (STRING session[2] SHORT ret)<br>
END<br>
</font></p>
  </blockquote>
</blockquote>
<ul>
  <li><font face="Verdana" size="2" color="#000080">Load the resource file</font></li>
</ul>
<blockquote>
  <blockquote>
    <p><font face="Verdana" size="2" color="#000080">MXLoadResourceFile (pmx,
    &quot;ResourceFileName&quot;);</font></p>
  </blockquote>
</blockquote>
<ul>
  <li><font face="Verdana" size="2" color="#000080">Open a connection with
    another application</font></li>
</ul>
<blockquote>
  <blockquote>
    <p><font face="Verdana" size="2" color="#000080">pcom = MXOpenTCP
    (pmx,&nbsp; &quot;MachineName&quot;, port, ...);</font></p>
  </blockquote>
</blockquote>
<ul>
  <li><font face="Verdana" size="2" color="#000080">In the application program
    you create and put a message in the output queue of the connection <i>pcom</i>
    as follow :</font></li>
</ul>
<blockquote>
  <blockquote>
    <p><font face="Verdana" size="2" color="#000080">pmessage = MXPutMesage
    (pcom, &quot;mydialog&quot;, &quot;Ask&quot;);</font></p>
  </blockquote>
</blockquote>
<ul>
  <li><font size="2" face="Verdana" color="#000080">At this point <i> pmessage</i> is
    not sent, you affect values to this message as follow :</font></li>
</ul>
<blockquote>
  <blockquote>
    <p><font size="2" face="Verdana" color="#000080">MXSetValue (pmessage,
    &quot;Session&quot;, 1, &quot;String1&quot;);<br>
    MXSetValue (pmessage, &quot;Session&quot;, 2, &quot;String2&quot;);<br>
    MXSetValue (pmessage, &quot;Ask&quot;, &quot;Myname&quot;);</font></p>
  </blockquote>
</blockquote>
<ul>
  <li><font size="2" face="Verdana" color="#000080">Or another method to set
    values to <i> pmessage</i> is in the callback that the application adds to the
    pmessage message class :</font></li>
</ul>
<blockquote>
  <blockquote>
    <p><font face="Verdana" color="#000080" size="2">MXAddComCallBack (pmx, pcom,
    &quot;mydialog&quot;, &quot;Ask&quot;,&nbsp; MXONSEND, Myfunction, NULL);&nbsp;</font></p>
    <p><font face="Verdana" size="2" color="#000080">int MyFunction (MXMessage*
    pmessage, MXCom* pcom, void* appfield)<br>
    {<br>
    &nbsp; MXSetValue (pmessage, &quot;Session&quot;, 1, &quot;String1&quot;);<br>
    &nbsp; MXSetValue (pmessage, &quot;Session&quot;, 2, &quot;String2&quot;);<br>
    &nbsp; MXSetValue (pmessage, &quot;Ask&quot;, &quot;Myname&quot;);<br>
    &nbsp; return 1;<br>
    }</font></p>
  </blockquote>
</blockquote>
<p><font size="2" face="Verdana" color="#000080"><i>&nbsp;&nbsp; Values will be set in the
callback just before sending the message.</i></font></p>
<ul>
  <li><font size="2" face="Verdana" color="#000080">The pmessage is sent either
    by MXDispatchEvents in an Asynchronous way or explicitly with the function
    MXSend in a Synchronous way :</font></li>
</ul>
<blockquote>
  <blockquote>
    <p style="margin-top: 0; margin-bottom: 0"><font face="Verdana" size="2" color="#000080">MXSend
    (pmx, pcom , pmessage);</font></p>
    <p style="margin-top: 0; margin-bottom: 0"><font face="Verdana" size="2" color="#000080">MXFreeMessage
    (pmx, pmessage);&nbsp;</font></p>
    <p style="margin-top: 0; margin-bottom: 0">&nbsp;</p>
  </blockquote>
</blockquote>
<p style="margin-top: 0; margin-bottom: 0"><font face="Verdana" size="2" color="#000080"><i>&nbsp;&nbsp;
With
MXSend the message instance should be freed</i></font></p>
<p style="margin-top: 0; margin-bottom: 0">&nbsp;</p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top"><font face="Verdana" size="2" color="#000080"><b>Return
      Values :&nbsp;</b>returns -1 if error else returns &gt; 0<b><br>
      </b></font></td>
  </tr>
</table>

     <p><br></p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top"><font color="#000080" size="2" face="Verdana"><b>See Also
      : </b><a href="MXGetValue.html">MXGetValue</a>, <a href="MXGetRangeValue.html">MXGetRangeValue</a>,
      <a href="MXSetRangeValue.html">MXSetRangeValue</a><b>&nbsp;<br>
      </b></font><font size="2" face="Verdana"></font></td>
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

      <p><font face="Verdana" color="#000080" size="2">&nbsp; ...</font>

      <p><font face="Verdana" size="2" color="#000080"><br>
      int AckStatement (MXMessage* pmessage, MXCom* pcom, void* par)<br>
      {<br>
      &nbsp; LONG RowsProcessed = (LONG)  MXGetValue (pmessage,  "RowsProcessed",    1);<br>
      &nbsp; LONG SqlCode =       (LONG)  MXGetValue (pmessage,  "SqlCode",          1);<br>
      &nbsp; STRING SqlErrMsg =   (STRING)MXGetValue (pmessage,  "SqlErrMsg",        1);<br>
      &nbsp; printf ("Ack Result = %s , %d, %d\n", SqlErrMsg , RowsProcessed, SqlCode);<br>
      &nbsp; return 1 ;<br>
      }<br>
      </font><br>

      <p><font face="Verdana" size="2" color="#000080">&nbsp; ...</font>

      <p><font face="Verdana" size="2" color="#000080">int main(int argc, char *args[])<br>
      {<br>
      &nbsp; MX mx;<br>
      &nbsp; MXMessage* pmessage;<br>
      &nbsp; MXCom* DBCom1;<br>
      &nbsp; BYTE TreatmentType  = OSSEQUENTIALTREATMENT;&nbsp;<br>
      &nbsp;     BYTE StorageType    = 1;&nbsp;<br>
      &nbsp;     DWORD Every         = 2;&nbsp;<br>
      &nbsp; DWORD MaxRows       = 30;&nbsp;<br>
      <br>
      &nbsp; <a href="MXInit.html">MXInit</a> (&amp;mx, MXSERVER, NULL, NULL, 0, NULL);</font>

      <p><font face="Verdana" size="2" color="#000080"><br>
      &nbsp; DBCom1 = <a href="MXOpenBase.html"> MXOpenBase</a> (&amp;mx, "Douane_W2K-TST",
      &quot;Douane&quot;, &quot;Douane&quot;, DBPROTO_ODBC,   TRUE);<br>
      &nbsp; if (!DBCom1) return 0;<br>
      <br>
      &nbsp; <a href="MXAddComCallBack.html">MXAddComCallBack</a> (&amp;mx, DBCom1,
      DB_SYS, &quot;ReceiveHeader&quot;, MXONRECV, ReceiveQueryHeader, NULL);<br>
      &nbsp; MXAddComCallBack (&amp;mx, DBCom1, DB_SYS, &quot;ResultSet&quot;, MXONRECV, ReceiveQueryBuffer, NULL);<br>
      &nbsp; MXAddComCallBack (&amp;mx, DBCom1, DB_SYS, "AckStatement",       MXONRECV, AckStatement, NULL);<br>
      <br>
      &nbsp; pmessage = <a href="MXPutMessage.html"> MXPutMessage</a> (DBCom1, DB_SYS,
      &quot;ExecuteQuery&quot;);<br>
      &nbsp; <a href="MXSetValue.html">MXSetValue</a> (pmessage, "SqlStatement",    1, "SELECT * FROM colis");<br>
      &nbsp; MXSetValue (pmessage, "TreatmentType",   1, &amp;TreatmentType);<br>
      &nbsp; MXSetValue (pmessage, "StorageType",     1, &amp;StorageType);<br>
      &nbsp; MXSetValue (pmessage, "Every",           1, &amp;Every);<br>
      &nbsp; MXSetValue (pmessage, "MaxRows",         1, &amp;MaxRows);<br>
      <br>
      &nbsp; ...</font>

      <p><font face="Verdana" size="2" color="#000080">&nbsp; <a href="MXDispatchEvents.html">MXDispatchEvents</a> (&amp;mx, 0);</font>

      <p><font face="Verdana" size="2" color="#000080">&nbsp; <a href="MXEnd.html">MXEnd</a> (&amp;mx);<br>
      <br>
      &nbsp; return 0;<br>
      }<br>
      <br>
      <br>
      </font><br>
    </td>
    <td>&nbsp;</td>
  </tr>
</table>
</body>
</html>