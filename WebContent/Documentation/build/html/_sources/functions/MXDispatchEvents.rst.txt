<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML//EN">
<html>

<head>
<meta http-equiv="Content-Type"
content="text/html; charset=iso-8859-1">
<meta name="Version" content="8.0.3429">
<meta name="Date" content="11/28/96">
<meta name="Template" content="D:\Isaoff97\Office\HTML.DOT">
<meta name="GENERATOR" content="Microsoft FrontPage 5.0">
<title>C NetProg Library : MXDispatchEvents</title>
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
    <td width="175" valign="top" bgcolor="#FFFFE6">

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
    <td valign="top" width="100%"><b><font face="Verdana" size="4" color="#000080">MXDispatchEvents</font></b></td>
    <td valign="baseline" align="right" nowrap><font face="Arial,Geneva,Helvetica,san-serif" size="2"><b><font color="#FF9900">C
      NetProg Library</font></b></font></td>
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
          <td width="50%"><b><font face="Verdana" color="#000080" size="2">LONG       MXDispatchEvents (MX* pmx, DWORD milliseconds)</font></b></td>
        </tr>
      </table>
      <p><font face="Verdana" size="2" color="#000080"><b></b></font>
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
          <td width="10%" align="left"><font face="Verdana" color="#000080" size="2"><i>milliseconds</i></font>&nbsp;</td>
          <td width="20%" align="left"><font face="Verdana" size="2" color="#000080">timeout
            in milleseconds before function returning&nbsp;</font></td>
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

<p><font face="Verdana" size="2" color="#000080">The MXDispatchEvents function
manages all the events related to connections (open, close), to connections
queues (input,output) </font><font face="Verdana" size="2" color="#000080">and
then it dispatches the events to the appropriate registered procedures. It </font><font face="Verdana"><font size="2" color="#000080"> cooperatively schedule the cpu or other resources
to each connection&nbsp;</font></font>
</p>

<p><font face="Verdana" size="2" color="#000080">If there is nothing to process,
MXDispatchEvents blocks (No cpu is consumed) until there is&nbsp;</font></p>
<p><font face="Verdana" size="2" color="#000080">This constitutes the main loop
of the MX manager, and, as such, it does not return unless the parameter <i>milliseconds</i>
is different from 0&nbsp;</font></p>
<p><font color="#000080" size="2" face="Verdana">
If <i> milliseconds</i> is 0, applications are expected to exit in response to
an event that triggers a callback procedure.&nbsp;If an application wants to do
some background work, it shoud use the <a href="MXAddApplicationProcedure.html"> MXAddApplicationProcedure</a><b>&nbsp;</b>function</font></p>
<p><font color="#000080" size="2" face="Verdana">
If milliseconds is different from 0, this function returns every <i> milliseconds</i>.&nbsp;In
this case this function is usually called in a loop</font></p>
<p><font face="Verdana" size="2" color="#000080">This function is used for
asynchronous programming</font></p>
<p><font face="Verdana" size="2" color="#000080"></font></p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top"><font color="#000080" size="2" face="Verdana"><b>Return
      Values : </b>-1 if error else returns 0 <b>&nbsp;</b></font><b><br>
      </b><font face="Verdana" size="2" color="#000080"></font></td>
  </tr>
</table>

      <p><br></p>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top"><font color="#000080" size="2" face="Verdana"><b>See Also
      :</b> <a href="MXAddApplicationProcedure.html">MXAddApplicationProcedure</a>,
      <a href="MXRemoveApplicationProcedure.html">MXRemoveApplicationProcedure</a><b>&nbsp;<br>
      </b>
      </font><font size="2" face="Verdana"></font></td>
  </tr>
</table>

<table border="0" width="100%" bordercolor="#000000" bordercolordark="#000080" bordercolorlight="#000080">
  <tr>
    <td valign="top" width="100%">&nbsp;
      <p><font color="#000080" size="2" face="Verdana"><b>Remarks
      :</b></font><b>&nbsp;
      </b></p>
<p><font face="Verdana" size="2" color="#000080">With async, or 'event-driven'
programming, you cooperatively schedule the cpu or other resources you wish to
apply to each connection. How you do this really depends on the application -
and it's not always possible or reasonable to try. But if you can capture the
state of any one connection, and divide the work it will do into relatively
small pieces, then this solution might work for you. If your connections do not
require much (or any) state, then this is an ideal approach.</font></p>
<p><font face="Verdana" size="2" color="#000080"><i>pros</i>:</font>
<ul>
  <li><font face="Verdana" size="2" color="#000080">efficient and elegant</font>
  <li><font face="Verdana" size="2" color="#000080">scales well - hundreds of
    connections means only hundreds of socket/state objects, not hundreds of
    threads or processes.</font>
  <li><font face="Verdana" size="2" color="#000080">requires no interlocking for
    access to shared resources. If your database provides no interlocking of its
    own (as is the case for dbm, dbz, and berkeley db), than the need to
    serialize access to the database is provided trivially.</font>
  <li><font face="Verdana" size="2" color="#000080">integrates easily with
    event-driven window-system programming. GUI programs that use blocking
    network calls are not very graceful.</font></li>
</ul>
<font face="Verdana" size="2" color="#000080"><i>cons</i>:</font>
<ul>
  <li><font face="Verdana" size="2" color="#000080">more complex - you may need
    to build state machines.</font>
  <li><font face="Verdana" size="2" color="#000080">requires a fundamentally
    different approach to programming that can be confusing at first.</font></li>
</ul>

      <p><b><br>
      </b></p>
    </td>
  </tr>
</table>

<p><br></p>

      <p>&nbsp;</td>
    <td>&nbsp;</td>
  </tr>
</table>
</body>
</html>