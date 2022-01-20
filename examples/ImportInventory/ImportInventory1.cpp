
#include "sc.h"


char OrgId[10];
char ClientId[10];
char UserId[10];

char InventorySeq[20];
int iInventorySeq;

char TransactionSeq[20];
int iTransactionSeq; 

MXCom* omega_com; 
MXCom* compiere_com; 
int Request = 0;
#define USER_ID     0
#define CLIENT_ID   1
#define ORG_ID      2

int QueryFinish = 0;
int j = 0;
int Count = 15000;
int Year = 2005;

int AfterUpdate = 0;

char Statement[3000] = "SELECT StkItemsTransactions.TATrs_nlTransactionId, StkItemsTransactions.Name, vew_TransactionRules.TATrs_sTransDescEng, StkItemsTransactions.TSIT_nsQty1, StkItemsTransactions.TSIT_nsQty2, AccTransactionHeader.TATrs_dOperationDate, AccTransactionHeader.TATrs_sTransReference, AccTransactionHeader.Customer, StkItems.Location \
FROM ((StkItemsTransactions INNER JOIN AccTransactionHeader ON StkItemsTransactions.TATrs_nlTransactionId = AccTransactionHeader.TATrs_nlTransactionId) \
INNER JOIN vew_TransactionRules ON AccTransactionHeader.TATrs_nlTransactionType = vew_TransactionRules.TATrs_nlTransactionType) \
INNER JOIN StkItems ON StkItemsTransactions.Name = StkItems.Name \
WHERE (((vew_TransactionRules.TATrs_sTransDescEng)='Inventory'))\
ORDER BY StkItemsTransactions.Name, AccTransactionHeader.TATrs_dOperationDate";


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
void SelectLocatorId (char* Name, char* Id)
{
    char Statement[250];
    int every = 1;
    int max = 1;
    MXMessage* pmessage;
    MXMessage* pmessage1;
    char* Table_id;
    sprintf (Statement, "select M_LOCATOR_ID  from M_LOCATOR where VALUE = '%s'", Name);
    Table_id = "M_LOCATOR_ID";
  
    pmessage =  MXPutMessage (compiere_com, DB_SYS, "ExecuteQuery");
    MXSetValue (pmessage ,"Every", 1, &every);
    MXSetValue (pmessage ,"MaxRows", 1, &max);
    MXSetValue (pmessage ,"Name",    1,         "Statement");
    MXSetValue (pmessage ,"SqlStatement", 1, Statement);
    MXSend (compiere_com->MX, compiere_com, pmessage);
    MXFreeMessage (compiere_com->MX, pmessage);



    pmessage1 = MXRecv (compiere_com->MX, compiere_com);    //header
   
    MXFreeMessage (compiere_com->MX, pmessage1);
    pmessage1 = MXRecv (compiere_com->MX, compiere_com);    //contents
    if (strcmp (pmessage1->MessageClass->Name, "AckStatement") != 0)    //error
    {
        strcpy (Id, (STRING)MXGetValue (pmessage1, Table_id, 1));
        pmessage1 =MXRecv (compiere_com->MX, compiere_com);  // Ack
    }
    MXFreeMessage (compiere_com->MX, pmessage1);

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
    BYTE Commit = '1';
    STRING SqlErrMsg = (STRING)MXGetValue (pmessage, "SqlErrMsg", 1);
    LONG RowProcessed = (LONG)MXGetValue (pmessage, "RowsProcessed", 1);
    STRING SqlStatement = (STRING)MXGetValue (pmessage, "SqlStatement", 1);
    SHORT Stop = (SHORT)MXGetValue (pmessage, "Stop", 1);


   if (Stop)
      printf ("Base = Compiere, Statement = %s\n, RowProcessed = %d, Error = %s\n",  SqlStatement,  RowProcessed, SqlErrMsg);
    else
      printf ("Base = Compiere,  RowProcessed = %d\n",  RowProcessed);

    return 1;
}

int AckExcuteQuery (MXMessage* pmessage, MXCom* pcom, void* appfield)
{

    MXCom* compiere_com = (MXCom*)appfield;
    MXMessage* pmessage1;
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
    if (QueryFinish)
    {
        sprintf (Statement, "%s%d%s" , "update AD_SEQUENCE set CURRENTNEXT = ",  iInventorySeq, " WHERE NAME = 'I_Inventory'");
        pmessage1 =  MXPutMessage (compiere_com, DB_SYS, "ExecuteUpdate");
        MXSetValue (pmessage1, "SqlStatement", 1, Statement );
        MXSetValue (pmessage1, "Commit", 1, &Commit);
        MXSend (compiere_com->MX, compiere_com, pmessage1);
        MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
        MXFreeMessage (compiere_com->MX, pmessage1);
        MXCloseCom (compiere_com->MX, compiere_com);
        MXCloseCom (compiere_com->MX, omega_com);
   }
    return 1;
}

int SelectInventory (MXMessage* pmessage, MXCom* pcom, void* appfield)
{  
    MXMessage* pmessage1;
    BYTE Commit = '1';
    char LocationId[100];



    pmessage1 =  MXCreateMessage (compiere_com->MX, DB_SYS, "ExecuteUpdate");
    MXSetValue (pmessage1, "Commit", 1, &Commit);

    STRING Name =             strend ((STRING) MXGetValue (pmessage, "name", 1), " ");
    STRING TSIT_nsQty1  =     (STRING)MXGetValue (pmessage, "TSIT_nsQty1", 1);
    STRING Location  =    strend ((STRING)MXGetValue (pmessage, "Location", 1), " ");
    STRING TATrs_dOperationDate =    strend ((STRING)MXGetValue (pmessage, "TATrs_dOperationDate", 1), " ");
   
    for (int k = 0; k < (int)strlen (Location); k++)
        Location[k] = toupper(Location[k]);

    SelectLocatorId (Location,  LocationId);
    char Statement[1000];
    char SDate[100];
    char syear[100];
    char smonth[100];
    char sday[100];
    strcpy (syear, strtok (TATrs_dOperationDate, "-"));
    strcpy (smonth, strtok(NULL, "-"));
    strcpy (sday,  strtok(NULL, " "));
    sprintf (SDate, "%s/%s/%s", smonth, sday, syear);


    sprintf (Statement, "INSERT INTO I_INVENTORY  (I_INVENTORY_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, \
                          VALUE, WAREHOUSEVALUE, M_LOCATOR_ID, QTYCOUNT, QTYBOOK, MOVEMENTDATE)\
                          VALUES (%d, %s, %s, %s, %s, '%s',  '%s', '%s', %s, %s, %s'%s',%s)",  
                               iInventorySeq, ClientId, OrgId, UserId, UserId, Name, "Standard",  LocationId, TSIT_nsQty1 ,0,   "to_date (", SDate, "'mm/dd/yyyy')");
           
     MXSetValue (pmessage1, "SqlStatement", 1, Statement);
     MXSend (compiere_com->MX, compiere_com, pmessage1);
     MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
     iInventorySeq++;
     j++; printf ("<--- %d\n", j);
  return 1;

}   

void SelectInventoryRows (MX* pmx)
{
    MXMessage* pmessage;
    int every = 1;
    int max = Count;
    int i = 0;
    BYTE Commit = 1;
 
         
    MXAddComCallBack (pmx, omega_com, DB_SYS,  "SelectInventory", MXONRECV, SelectInventory, compiere_com); 


    pmessage = MXCreateMessage (pmx, DB_SYS, "ExecuteQuery");
    MXSetValue (pmessage ,"Every", 1, &every);
    MXSetValue (pmessage ,"MaxRows", 1, &max);
   
    MXSetValue (pmessage ,"Name",    1,    "SelectInventory"  );
    MXSetValue (pmessage ,"SqlStatement", 1, Statement);
    MXSend (pmx, omega_com, pmessage);
    while (!QueryFinish)
    {
        MXFreeMessage (omega_com->MX, MXRecv (omega_com->MX, omega_com));

    }
    MXFreeMessage (pmx, pmessage);
}


void SelectPhysicalInventoryRows (MX* pmx, MXCom* pcom, MXCom* pcom1)
{
    char Statement[1500];
    MXMessage* pmessage;
    MXMessage* pmessage1;
    MXMessage* pmessage2;
    int every = 1;
    int max = Count;
    BYTE Commit = 1;




    pmessage = MXCreateMessage (pmx, DB_SYS, "ExecuteQuery");
    MXSetValue (pmessage ,"Every", 1,   &every);
    MXSetValue (pmessage ,"MaxRows", 1, &max);
    MXSetValue (pmessage ,"Name",    1, "SelectPhysicalInventory"  );
    

    sprintf (Statement, "SELECT M_PRODUCT_ID, M_LOCATOR_ID, QTYCOUNT, M_INVENTORYLINE_ID from M_INVENTORYLINE WHERE AD_CLIENT_ID = %s AND PROCESSED = 'N';", ClientId );
    MXSetValue (pmessage ,"SqlStatement", 1, Statement);

 
    MXSend (pmx, pcom, pmessage);
    MXFreeMessage (pmx, pmessage);


    pmessage =  MXCreateMessage (pmx, DB_SYS, "ExecuteUpdate");


    pmessage2 = MXRecv (pmx, pcom);
    
    if (strcmp (pmessage2->MessageClass->Name, "AckStatement") != 0)   //header
    {
        while ( (pmessage1 = MXRecv (pmx, pcom)) &&
                (strcmp (pmessage1->MessageClass->Name, "AckStatement") != 0) )
        {
             STRING M_PRODUCT_ID           =  (STRING) MXGetValue (pmessage1, "M_PRODUCT_ID", 1);
             STRING M_LOCATOR_ID           =  (STRING) MXGetValue (pmessage1, "M_LOCATOR_ID", 1);
             STRING QTYCOUNT               =  (STRING) MXGetValue (pmessage1, "QTYCOUNT", 1);
             STRING M_INVENTORYLINE_ID     =  (STRING) MXGetValue (pmessage1, "M_INVENTORYLINE_ID", 1);
	
             sprintf (Statement, "insert into M_STORAGE (M_PRODUCT_ID, M_LOCATOR_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, QTYONHAND, DATELASTINVENTORY) \
                                 VALUES (%s, %s, %s, %s, %s, %s, %s, %s)", M_PRODUCT_ID, M_LOCATOR_ID, ClientId, OrgId, UserId, UserId, QTYCOUNT, "to_date ('1/1/2004', 'mm/dd/yyyy')");
                
  
             MXSetValue (pmessage, "SqlStatement", 1, Statement);
             MXSetValue (pmessage, "Commit", 1, &Commit);
             MXSend (pmx, pcom1, pmessage);
             MXFreeMessage (pmx, MXRecv (pmx, pcom1));

             sprintf (Statement, "insert into M_TRANSACTION (M_TRANSACTION_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, MOVEMENTTYPE, M_LOCATOR_ID, M_PRODUCT_ID, MOVEMENTQTY, MOVEMENTDATE, M_INVENTORYLINE_ID) \
                                 VALUES (%d, %s, %s, %s, %s, '%s', %s, %s, %s, %s, %s)",
                                 iTransactionSeq, ClientId, OrgId, UserId, UserId, "I+",  M_LOCATOR_ID, M_PRODUCT_ID, QTYCOUNT, "to_date ('1/1/2004', 'mm/dd/yyyy')",M_INVENTORYLINE_ID);

             iTransactionSeq++;   
  
             MXSetValue (pmessage, "SqlStatement", 1, Statement);
             MXSetValue (pmessage, "Commit", 1, &Commit);
             MXSend (pmx, pcom1, pmessage);
             MXFreeMessage (pmx, MXRecv (pmx, pcom1));
            
        }
    }

    sprintf (Statement, "update AD_SEQUENCE set CURRENTNEXT = %d WHERE NAME = '%s'",  iTransactionSeq, "M_Transaction");
    MXSetValue (pmessage, "SqlStatement", 1, Statement);
    MXSetValue (pmessage, "Commit", 1, &Commit);
    MXSend (pmx, pcom1, pmessage);
    MXFreeMessage (pmx, MXRecv (pmx, pcom1));

    sprintf (Statement, "update M_INVENTORYLINE set PROCESSED = '%s' WHERE AD_CLIENT_ID = %s AND PROCESSED = 'N'", "Y", ClientId);
    MXSetValue (pmessage, "SqlStatement", 1, Statement);
    MXSetValue (pmessage, "Commit", 1, &Commit);
    MXSend (pmx, pcom1, pmessage);
    MXFreeMessage (pmx, MXRecv (pmx, pcom1));
   
    sprintf (Statement, "update M_INVENTORY set PROCESSED = '%s', ISAPPROVED = '%s', DOCSTATUS = '%s', DOCACTION = '%s'  WHERE AD_CLIENT_ID = %s AND PROCESSED = 'N'", "Y", "Y", "CO", "CL", ClientId);
    MXSetValue (pmessage, "SqlStatement", 1, Statement);
    MXSetValue (pmessage, "Commit", 1, &Commit);
    MXSend (pmx, pcom1, pmessage);
    MXFreeMessage (pmx, MXRecv (pmx, pcom1));


    MXFreeMessage (pmx, pmessage);
    MXFreeMessage (pmx, pmessage2);

}



void main(int argc, char* argv[])
{
    MX mx;

    
    if (argc > 1)
    {
        AfterUpdate = 1;
    }
        
    MXInit (&mx, MXCLIENT, "base", "base", 0, NULL);

   
    omega_com = MXOpenBase (&mx, "omega", "","", DBPROTO_ODBC, TRUE);
    if (!omega_com) return;
    compiere_com = MXOpenBase (&mx, "compiere", "scott","tiger", DBPROTO_ODBC, TRUE);
    if (!compiere_com) return;

    MXAddComCallBack (&mx, omega_com,    DB_SYS, "AckStatement",   MXONRECV,   AckExcuteQuery, compiere_com); 

    
    
    SelectId ("Techno Cars", ClientId, CLIENT_ID);
    SelectId ("Techno Cars", OrgId, ORG_ID);
    SelectId ("SuperUser",   UserId, USER_ID);


    MXAddComCallBack (&mx, compiere_com, DB_SYS, "AckStatement",   MXONRECV,   AckExcuteUpdate, omega_com); 


    if (!AfterUpdate)
    {
        SelectSeq ("I_Inventory", InventorySeq);
        iInventorySeq = atol (InventorySeq);

        SelectInventoryRows (&mx);
    }
    else
    {
        MXCom* pcom1  = MXOpenBase (&mx, "compiere", "scott","tiger", DBPROTO_ODBC, TRUE);
        if (!pcom1) return;
        
        MXAddComCallBack (&mx, pcom1, DB_SYS, "AckStatement",   MXONRECV,   AckExcuteUpdate, NULL); 

        SelectSeq ("M_Transaction", TransactionSeq);
        iTransactionSeq = atol (TransactionSeq);

        SelectPhysicalInventoryRows(&mx, compiere_com, pcom1);

        MXCloseCom (&mx, pcom1);
        MXCloseCom (&mx, compiere_com);
        MXCloseCom (&mx, omega_com);
        
    }

    MXDispatchEvents (&mx, 0);

    MXEnd (&mx);

}