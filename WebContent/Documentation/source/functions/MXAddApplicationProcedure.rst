MXAddApplicationProcedure
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML//EN">
<html>

<head>
<meta http-equiv="Content-Type"
content="text/html; charset=iso-8859-1">
<meta name="Version" content="8.0.3429">
<meta name="Date" content="11/28/96">
<meta name="Template" content="D:\Isaoff97\Office\HTML.DOT">
<meta name="GENERATOR" content="Microsoft FrontPage 5.0">
<title>C NetProg Library : MXAddApplicationProcedure</title>
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



<table border="0" cellpadding="0" cellspacing="0" width="1230">
  <tr>
    <td width="221" valign="top" align="center" bgcolor="#FFFFE6">

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
    <td width="1001">

<table border="0" width="100%">
  <tr>
    <td valign="top" width="100%"><b><font face="Verdana" size="4" color="#000080">MXAddApplicationProcedure</font></b></td>
    <td valign="baseline" align="right" nowrap><b><font color="#FF9900" face="Verdana" size="2">C
      NetProg Library&nbsp;</font></b></td>
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
          <td width="50%"><b><font face="Verdana" color="#000080" size="2">int        MXAddApplicationProcedure (MX* pmx, int
            (*funct)(MX* pmx, void* applicationfield), void* applicationfield)</font></b></td>
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
          <td width="10%" align="left"><font face="Verdana"><i><font color="#000080" size="2">pmx</font></i>&nbsp;</font></td>
          <td width="20%" align="left"><font face="Verdana" size="2" color="#000080">the
            MX manager</font></td>
  </tr>
  <tr>
          <td width="10%" align="left"><font face="Verdana"><i><font color="#000080" size="2">funct</font></i>&nbsp;</font></td>
          <td width="20%" align="left"><font face="Verdana" size="2" color="#000080">the
            function to call by the manager</font></td>
  </tr>
  <tr>
          <td width="10%" align="left"><font face="Verdana"><i><font color="#000080" size="2">applicationfield</font></i>&nbsp;</font></td>
          <td width="20%" align="left"><font face="Verdana" size="2" color="#000080">Programmer-specified
            data sent to the callback function</font></td>
  </tr>
  
</table>

<p><br>
</p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top" width="100%">
      <p align="left"><font face="Verdana"><b><font color="#000080" size="2">Description
      :&nbsp;</font><br>
      </b></font></p>
    </td>
  </tr>
</table>

<p><font color="#000080" size="2" face="Verdana">This function is used when an application wants to do
some background work, by the MX manager. Usually the function MXDispatchEvents
which manages all events does not return if its time parameter is set to 0. In
this case, if the application wants to do some processing, it should add a
callback that will be called automatically by the MX manager in a scheduled
manner as long as the callback returns a value different from 0. If the callback
returns the value 0, and if the MX manager has no other events to treat, this
function will not be called and&nbsp; no more cpu is consumed.</font><font face="Verdana" size="2" color="#000080">The
next call to this callback will occur after an external event wakes up the MX
manager.</font></p>

<p><br>
</p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top"><font face="Verdana"><b><font color="#000080" size="2">Return
      Values :</font></b><font color="#000080" size="2"><b>&nbsp;</b>returns -1 if error
      else returns 0</font><b><br>
      </b></font></td>
  </tr>
</table>

<p><br>
</p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top"><b><font color="#000080" size="2" face="Verdana">See Also
      :</font></b> <font color="#000080" size="2" face="Verdana"><a href="MXRemoveApplicationProcedure.html">MXRemoveApplicationProcedure</a>,
      <a href="MXDispatchEvents.html">MXDispatchEvents</a><b>&nbsp;<br>
      </b></font></td>
  </tr>
</table>

<p><br>
</p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top" width="100%"><font face="Verdana"><b><font color="#000080" size="2">Example
      :</font><br>
      </b></font></td>
  </tr>
</table>

<p style="margin: 0">&nbsp;
</p>

<p style="margin: 0"><font color="#000080" face="Verdana" size="2">int
PromptUser (MX* pmx, void* appfield)<br>
{</font><br>
</p>

<p style="margin: 0"><font color="#000080" face="Verdana" size="2">&nbsp; MXCom*
pcom = (MXCom*)appfield;<br>
&nbsp; MXMessage* pmessage;<br>
&nbsp; char Buffer[500];</font>
</p>

<p style="margin: 0"><font color="#000080" face="Verdana" size="2"><br>
&nbsp; putchar ('S');putchar ('C');putchar ('>');<br>
&nbsp; gets (Buffer);</font>
</p>

<p style="margin: 0"><font face="Verdana" size="2" color="#008000"><b>/* do
whatever you want */</b></font>
</p>

<p style="margin: 0">&nbsp;</p>
<p style="margin: 0"><font color="#000080" face="Verdana" size="2">&nbsp; <a href="MXPutMessage.html">MXPutMessage</a>
(pcom, &quot;DialogName&quot;, &quot;MessageClassName&quot;);</font>
</p>

<p style="margin: 0"><font color="#000080" face="Verdana" size="2">&nbsp; <a href="MXSetValue.html">MXSetValue</a>
(pmessage, ....</font><br>
&nbsp; ...<br>
<font face="Verdana" size="2" color="#000080">&nbsp; if (ContinueToPrompt)</font>
</p>

<p style="margin: 0"><font face="Verdana" size="2" color="#000080">&nbsp;&nbsp;&nbsp;
return 1;</font>
</p>

<p style="margin: 0"><font face="Verdana" size="2" color="#000080">&nbsp; else</font>
</p>

<p style="margin: 0"><font face="Verdana" size="2" color="#000080">&nbsp;&nbsp;&nbsp;
return 0;</font>
</p>

<p style="margin: 0"><font color="#000080" face="Verdana" size="2">}</font><br>
</p>

<p style="margin: 0">&nbsp;
</p>

<p><font face="Verdana" size="2" color="#000080">&nbsp;int main(int argc, char*
argv[])<br>
{<br>
&nbsp; MX mx;<br>
&nbsp; MXCom* pcom;<br>
</font>
</p>

<p style="margin-top: 0; margin-bottom: 0"><font face="Verdana" size="2" color="#008000"><b>/*
You define your dialog class in the file MyResourceFile.irc */</b></font>
</p>

<p style="margin-top: 0; margin-bottom: 0"><font face="Verdana" size="2" color="#000080">
<br>
&nbsp; <a href="MXInit.html">MXInit</a> (&amp;mx, MXSERVER, NULL, NULL, 0,
&quot;MyResourceFile.irc&quot;);<br>
&nbsp; pcom = MXOpenTCP (pmx, &quot;MachineName&quot;, 3005, IPPROTO_DG, NULL,
NULL, TRUE);<br>
&nbsp; if (!pcom) return -1;<br>
<br>
&nbsp; <a href="MXAddApplicationProcedure.html">MXAddAddApplicationProcedure</a>
(&amp;mx, PromptUser, pcom);<br>
<br>
&nbsp; <a href="MXDispatchEvents.html">MXDispatchEvents</a> (&amp;mx, 0);<br>
<br>
&nbsp; <a href="MXEnd.html">MXEnd</a> (&amp;mx);<br>
}</font><br>
</p>

      <p>&nbsp;</td>
    <td width="8"><font face="Verdana">&nbsp;</font></td>
  </tr>
</table>
</body>
</html>