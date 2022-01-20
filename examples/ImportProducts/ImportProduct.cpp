
#include "sc.h"



char OrgId[10];
char ClientId[10];
char UserId[10];
char ProductSeq[20];

int iProductSeq;

MXCom* omega_com; 
MXCom* compiere_com; 
int Request = 0;
#define USER_ID     0
#define CLIENT_ID   1
#define ORG_ID      2

int i = 0;

int Finish  = 0;
int execution = 0;
int Year = 2005;
int Count = 12000;

int QueryFinish = 0;


void SelectId (char* Name, char* Id, int Type)
{
    char Statement[250];
    int every = 1;
    int max = 1;
    MXMessage* pmessage;
    MXMessage* pmessage1;
    char* Table_id;

    switch (Type)
    {
    case ORG_ID :
        if (strcmp (Name, "*") == 0)
             sprintf (Statement, "%s'%s';", "select AD_ORG_ID from AD_ORG where NAME = ", Name);
        else
            sprintf (Statement, "%s'%s'%s%s;", "select AD_ORG_ID from AD_ORG where NAME = ", Name, " and AD_CLIENT_ID = ", ClientId);
         Table_id = "AD_ORG_ID";
       break;
    case CLIENT_ID :
           sprintf (Statement, "%s'%s';", "select AD_CLIENT_ID from AD_CLIENT where NAME = ", Name);
           Table_id = "AD_CLIENT_ID";
       break;
    case  USER_ID :
           sprintf (Statement, "%s'%s';", "select AD_USER_ID from AD_USER where NAME = ", Name);
           Table_id = "AD_USER_ID";
       break;
       default :
            return;
    }
    pmessage =  MXPutMessage (compiere_com, DB_SYS, "ExecuteQuery");
    MXSetValue (pmessage ,"Every", 1, &every);
    MXSetValue (pmessage ,"MaxRows", 1, &max);
    MXSetValue (pmessage ,"Name",    1,         "Statement");
    MXSetValue (pmessage ,"SqlStatement", 1, Statement);
    MXSend (compiere_com->MX, compiere_com, pmessage);

    pmessage1 = MXRecv (compiere_com->MX, compiere_com);    //header
    MXFreeMessage (compiere_com->MX, pmessage1);
    pmessage1 = MXRecv (compiere_com->MX, compiere_com);    //contents
    strcpy (Id, (STRING)MXGetValue (pmessage1, Table_id, 1));
    pmessage1 =MXRecv (compiere_com->MX, compiere_com);  // Ack
    MXFreeMessage (compiere_com->MX, pmessage1);
    MXFreeMessage (compiere_com->MX, pmessage);
}

void SelectSeq (char* Name, char* Id)
{
    char Statement[250];
    int every = 1;
    int max = 1;
    MXMessage* pmessage;
    MXMessage* pmessage1;

    sprintf (Statement, "select CURRENTNEXT  from AD_SEQUENCE where NAME = '%s'", Name);
    pmessage =  MXPutMessage (compiere_com, DB_SYS, "ExecuteQuery");
    MXSetValue (pmessage ,"Every", 1, &every);
    MXSetValue (pmessage ,"MaxRows", 1, &max);
    MXSetValue (pmessage ,"Name",    1,         "Statement");
    MXSetValue (pmessage ,"SqlStatement", 1, Statement);
    MXSend (compiere_com->MX, compiere_com, pmessage);

    pmessage1 = MXRecv (compiere_com->MX, compiere_com);    //header
    MXFreeMessage (compiere_com->MX, pmessage1);
    pmessage1 = MXRecv (compiere_com->MX, compiere_com);    //contents
    strcpy (Id, (STRING)MXGetValue (pmessage1, "CURRENTNEXT", 1));
    pmessage1 =MXRecv (compiere_com->MX, compiere_com);  // Ack
    MXFreeMessage (compiere_com->MX, pmessage1);
    MXFreeMessage (compiere_com->MX, pmessage);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char* strend (char* str, char* schar)
{
    int i = 0;
    for (i = strlen (str) - 1; i >= 0; i--)
      if (str[i] != schar[0])
        break;
    str[i+1] = 0;
    return str;
}



int AckExcuteUpdate (MXMessage* pmessage, MXCom* pcom, void* appfield)
{
    MXMessage* pmessage1;
    BYTE Commit = '1';
    STRING SqlErrMsg = (STRING)MXGetValue (pmessage, "SqlErrMsg", 1);
    LONG RowProcessed = (LONG)MXGetValue (pmessage, "RowsProcessed", 1);
    STRING SqlStatement = (STRING)MXGetValue (pmessage, "SqlStatement", 1);
    SHORT Stop = (SHORT)MXGetValue (pmessage, "Stop", 1);


    execution++;

   if (Stop)
      printf ("Base = Compiere, Statement = %s\n, RowProcessed = %d, Error = %s\n",  SqlStatement,  RowProcessed, SqlErrMsg);
    else
      printf ("Base = Compiere,  RowProcessed = %d\n",  RowProcessed);
    
 

    return 1;
}

int AckExcuteQuery (MXMessage* pmessage, MXCom* pcom, void* appfield)
{
    MXMessage* pmessage1;
    MXCom* compiere_com = (MXCom*)appfield;
    BYTE Commit = '1';
    STRING SqlErrMsg = (STRING)MXGetValue (pmessage, "SqlErrMsg", 1);
    LONG RowProcessed = (LONG)MXGetValue (pmessage, "RowsProcessed", 1);
    STRING SqlStatement = (STRING)MXGetValue (pmessage, "SqlStatement", 1);
    SHORT Stop = (SHORT)MXGetValue (pmessage, "Stop", 1);


    if (Stop)
      printf ("Base = Omega, Statement = %s\n, RowProcessed = %d, Error = %s\n",  SqlStatement,  RowProcessed, SqlErrMsg);
    else
      printf ("Base = Omega, RowProcessed = %d\n",  RowProcessed);
   

    QueryFinish = 1;
  
 
    char Statement[100];
    Finish = 1;
    sprintf (Statement, "%s%d%s" , "update AD_SEQUENCE set CURRENTNEXT = ",  iProductSeq, " WHERE NAME = 'I_Product'");
   
    pmessage1 =  MXPutMessage (compiere_com, DB_SYS, "ExecuteUpdate");
    MXSetValue (pmessage1, "SqlStatement", 1, Statement );
    MXSetValue (pmessage1, "Commit", 1, &Commit);
    MXSend (compiere_com->MX, compiere_com, pmessage1);
    MXFreeMessage (compiere_com->MX, pmessage1);
    pmessage1 = MXRecv (pcom->MX, compiere_com);
    MXFreeMessage (pcom->MX, pmessage1);
        
    MXCloseCom (pcom->MX, compiere_com);
    MXCloseCom (pcom->MX, omega_com);

    return 1;
}



int SelectStkItems (MXMessage* pmessage, MXCom* pcom, void* appfield)
{

    char Statement[3000];
    char PName[300];
    MXMessage* pmessage1;
    BYTE Commit = 1;
    STRING Name =             strend ((STRING) MXGetValue (pmessage, "Name", 1), " ");
    STRING Description  =     strend ((STRING)MXGetValue (pmessage, "Description", 1), " ");
    STRING Description2  =    strend ((STRING)MXGetValue (pmessage, "Description2", 1), " ");
    STRING Note  =            strend ((STRING)MXGetValue (pmessage, "Note", 1), " ");
    STRING BaseUnit  =        strtok ((STRING)MXGetValue (pmessage, "BaseUnit", 1), " ");
    STRING TSI_sParentName  = strend ((STRING)MXGetValue (pmessage, "TSI_sParentName", 1), " ");
    STRING Supplier=          strend ((STRING)MXGetValue (pmessage, "Supplier", 1), " ");
    STRING Currency  =        strtok ((STRING)MXGetValue (pmessage, "Devise", 1), " ");
    STRING PL_2003 =          strtok ((STRING)MXGetValue (pmessage, "PL_2003", 1), " ");
    STRING PL_2004 =          strtok ((STRING)MXGetValue (pmessage, "PL_2004", 1), " ");
    STRING PL_2005 =          strtok ((STRING)MXGetValue (pmessage, "PL_2005", 1), " ");
     //   HELP,\
    //    UPC, \
    //    SKU, \
   

    STRING PL;
    if (Year == 2005) PL = PL_2005;
    else
        if (Year == 2004) PL = PL_2004; 
        else
             if (Year == 2003) PL = PL_2003;

    sprintf (PName, "%s-%s", Name, Description);
    sprintf (Statement, "%s%d,%s,%s,%s,%s,'%s','%s','%s','%s','%s','%s','%s','%s','%s',%s,%s,'%s','%s',%s)",
                         "insert into I_PRODUCT(I_PRODUCT_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, VALUE, NAME, DESCRIPTION, DOCUMENTNOTE,\
                                                X12DE355, PRODUCTCATEGORY_VALUE, PRODUCTTYPE, BPARTNER_VALUE, ISO_CODE, PRICELIST, \
                                                PRICEPO, VENDORPRODUCTNO, VENDORCATEGORY, ORDER_MIN) values (" ,
      iProductSeq, ClientId, OrgId, UserId, UserId,
      Name, Name, Description, Description2,
      BaseUnit, TSI_sParentName, "I", Supplier, Currency, PL,
      PL, Name, TSI_sParentName, "1");
     
    
    
    pmessage1 =  MXPutMessage (compiere_com, DB_SYS, "ExecuteUpdate");
    MXSetValue (pmessage1, "SqlStatement", 1, Statement);
    MXSetValue (pmessage1, "Commit", 1, &Commit);
    MXSend (compiere_com->MX, compiere_com, pmessage1);
    MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
    MXFreeMessage (compiere_com->MX, pmessage1);
    iProductSeq++;
    i++; printf ("<--- %d\n", i);
    return 1;

}   






void SelectStkItemsRows (MX* pmx)
{
    MXMessage* pmessage;
    char Statement[250];
    int every = 1;
    int max = Count;
    int i = 0;
    BYTE Commit = 1;
 
         
    MXAddComCallBack (pmx, omega_com, DB_SYS,  "SelectStkItems", MXONRECV, SelectStkItems, compiere_com); 


    pmessage = MXCreateMessage (pmx, DB_SYS, "ExecuteQuery");
    MXSetValue (pmessage ,"Every", 1, &every);
    MXSetValue (pmessage ,"MaxRows", 1, &max);
   
    MXSetValue (pmessage ,"Name",    1,    "SelectStkItems"  );
    MXSetValue (pmessage ,"SqlStatement", 1, "select * from stkitems ");
    MXSend (pmx, omega_com, pmessage);

    MXFreeMessage (pmx, pmessage);
    Request++;

}




void main(int argc, char* argv[])
{
    MX mx;
    Year = 2005;


    if (argc > 1)
        Year = atoi(argv[1]);
    if (argc > 2)
        Count = atoi(argv[2]);

    MXInit (&mx, MXCLIENT, "base", "base", 0, NULL);

   
    omega_com = MXOpenBase (&mx, "omega", "","", DBPROTO_ODBC, TRUE);
    if (!omega_com) return;
    compiere_com = MXOpenBase (&mx, "compiere", "scott","tiger", DBPROTO_ODBC, TRUE);
    if (!compiere_com) return;

    MXAddComCallBack (&mx, omega_com,    DB_SYS, "AckStatement",   MXONRECV,   AckExcuteQuery, compiere_com); 

    
    
    SelectId ("Techno Cars", ClientId, CLIENT_ID);
    SelectId ("Techno Cars", OrgId, ORG_ID);
    SelectId ("SuperUser", UserId, USER_ID);
    SelectSeq ("I_Product", ProductSeq);
    iProductSeq = atol (ProductSeq);
    MXAddComCallBack (&mx, compiere_com, DB_SYS, "AckStatement",   MXONRECV,   AckExcuteUpdate, omega_com); 

    SelectStkItemsRows (&mx);


    MXDispatchEvents (&mx, 0);

    MXEnd (&mx);

}