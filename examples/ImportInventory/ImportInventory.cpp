
#include "sc.h"


char Parameter[100];
char Statement[3000];

#define USER_ID             0
#define CLIENT_ID           1
#define ORG_ID              2
#define LOCATOR_ID          3
#define BPARTNER_ID         4
#define WAREHOUSE_ID        5
#define DOCUMENT_ID         6
#define BPARTNERLOCATION_ID 7
#define PRODUCT_ID          8
#define UOM_ID              9
#define CURRENCY_ID         10
#define PAYMENT_ID          11
#define PRICELIST_ID        12


char OrgId[10];
char ClientId[10];
char UserId[10];
char LocationId[10];
char BPartnerId[10];
char BPartnerLocationId[10];
char WarehouseId[10];
char ProductId[10];
char UOMId[10];
char DocumentId[10];
char PriceListId[10];
char PaymentId[10];
char CurrencyId[10];


char InventorySeq[20];
int  iInventorySeq;
char InventoryLineSeq[20];
int  iInventoryLineSeq;
int Line = 10;

char TransactionSeq[20];
int iTransactionSeq; 

char LastTransReference[30];


MXCom* omega_com; 
MXCom* compiere_com; 
int Request = 0;


int QueryFinish = 0;
int j = 0;
int Count = 15000;
int Year = 2005;

int AfterUpdate = 0;

char* strend (char* str, char* schar)
{
    int i = 0;
    for (i = strlen (str) - 1; i >= 0; i--)
      if (str[i] != schar[0])
        break;
    str[i+1] = 0;
    return str;
}


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
    case  LOCATOR_ID :
           sprintf (Statement, "%s'%s';", "select M_LOCATOR_ID  from M_LOCATOR where VALUE = ", Name);
           Table_id = "M_LOCATOR_ID";
       break;
    case  WAREHOUSE_ID :
           sprintf (Statement, "%s'%s';", "select M_WAREHOUSE_ID  from M_WAREHOUSE where NAME = ", Name);
           Table_id = "M_WAREHOUSE_ID";
       break;
    case  BPARTNER_ID :
           sprintf (Statement, "%s'%s';", "select C_BPARTNER_ID  from C_BPARTNER where NAME = ", Name);
           Table_id = "C_BPARTNER_ID";
       break;
    case  BPARTNERLOCATION_ID :
           sprintf (Statement, "%s%s;", "select C_BPARTNER_LOCATION_ID  from C_BPARTNER_LOCATION where C_BARTNER_ID = ", Name);
           Table_id = "C_BPARTNER_LOCATION_ID";
       break;
    case  DOCUMENT_ID :
           sprintf (Statement, "%s'%s'%s%s;", "select C_DOCTYPE_ID  from C_DOCTYPE where NAME = ", Name, " and AD_CLIENT_ID = ", ClientId);
           Table_id = "C_DOCTYPE_ID";
       break;
    case  PRODUCT_ID :
           sprintf (Statement, "%s'%s';", "select M_PRODUCT_ID  from M_PRODUCT where VALUE = ", Name);
           Table_id = "M_PRODUCT_ID";
       break;
    case  UOM_ID :
           sprintf (Statement, "%s%s;", "select C_UOM_ID  from M_PRODUCT where M_PRODUCT_ID = ", Name);
           Table_id = "C_UOM_ID";
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
        pmessage1 =  MXPutMessage (compiere_com, DB_SYS, "ExecuteUpdate");
        sprintf (Statement, "%s%d%s" , "update AD_SEQUENCE set CURRENTNEXT = ",  iInventorySeq, " WHERE NAME = 'M_Inventory'");
        MXSetValue (pmessage1, "SqlStatement", 1, Statement );
        MXSetValue (pmessage1, "Commit", 1, &Commit);
        MXSend (compiere_com->MX, compiere_com, pmessage1);
        MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));


        sprintf (Statement, "%s%d%s" , "update AD_SEQUENCE set CURRENTNEXT = ",  iInventoryLineSeq, " WHERE NAME = 'M_InventoryLine'");
        MXSetValue (pmessage1, "SqlStatement", 1, Statement );
        MXSetValue (pmessage1, "Commit", 1, &Commit);
        MXSend (compiere_com->MX, compiere_com, pmessage1);
        MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
        
        sprintf (Statement, "update AD_SEQUENCE set CURRENTNEXT = %d WHERE NAME = '%s'",  iTransactionSeq, "M_Transaction");
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

    STRING Name                 =    strend ((STRING) MXGetValue (pmessage, "name", 1), " ");
    STRING TSIT_nsQty1          =    strend ((STRING)MXGetValue (pmessage, "TSIT_nsQty1", 1), " ");
    STRING Location             =    strend ((STRING)MXGetValue (pmessage, "Location", 1), " ");
    STRING TATrs_dOperationDate =    strend ((STRING)MXGetValue (pmessage, "TATrs_dOperationDate", 1), " ");
    STRING TATrs_sTransReference=    strend ((STRING)MXGetValue (pmessage, "TATrs_sTransReference", 1), " ");
    STRING TAccT_sDesc          =    strend ((STRING)MXGetValue (pmessage, "TAccT_sDesc", 1), " ");
   
    
    
    for (int k = 0; k < (int)strlen (Location); k++)
        Location[k] = toupper(Location[k]);

    SelectId (Location,    LocationId,         LOCATOR_ID);
    SelectId (Name,        ProductId,          PRODUCT_ID);
  //  SelectId (TAccT_sDesc, BPartnerId,         BPARTNER_ID);
  // SelectId (BPartnerId,  BPartnerLocationId, BPARTNERLOCATION_ID);
  // SelectId (ProductId,   UOMId,              UOM_ID);

    char Statement[1000];
    char SDate[100];
    char syear[100];
    char smonth[100];
    char sday[100];
    strcpy (syear, strtok (TATrs_dOperationDate, "-"));
    strcpy (smonth, strtok(NULL, "-"));
    strcpy (sday,  strtok(NULL, " "));
    sprintf (SDate, "%s/%s/%s", smonth, sday, syear);

    if (strcmp (TATrs_sTransReference, LastTransReference) != 0)
    {
//INVENTORY
        strcpy (LastTransReference, TATrs_sTransReference);
        sprintf (Statement, "INSERT INTO M_INVENTORY  (M_INVENTORY_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, \
                                                       DOCUMENTNO, DESCRIPTION, M_WAREHOUSE_ID, MOVEMENTDATE, PROCESSED, PROCESSING, \
                                                       GENERATELIST, ISAPPROVED, DOCSTATUS, DOCACTION,  C_DOCTYPE_ID)\
                             VALUES (%d, %s, %s, %s, %s, \
                             '%s', '%s', %s, %s'%s',%s, '%s', '%s',\
                             '%s',  '%s', '%s', '%s', %s)",    
                             iInventorySeq, ClientId, OrgId, UserId, UserId,  
                             LastTransReference,  "", WarehouseId, "to_date (", SDate, "'mm/dd/yyyy')", "Y", "N", 
                             "N", "Y", "CL", "--",  DocumentId);
 
        MXSetValue (pmessage1, "Commit", 1, &Commit);
        MXSetValue (pmessage1, "SqlStatement", 1, Statement);
        MXSend (compiere_com->MX, compiere_com, pmessage1);
        MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
        iInventorySeq++;
        Line = 10;
    }

//INVENTORYLINE
    sprintf (Statement, "INSERT INTO M_INVENTORYLINE  (M_INVENTORYLINE_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, \
                                                       M_INVENTORY_ID, M_LOCATOR_ID, M_PRODUCT_ID, LINE, QTYCOUNT, PROCESSED)\
                         VALUES (%d, %s, %s, %s, %s, %d,  %s, %s, %d, %s, '%s')",
                         iInventoryLineSeq, ClientId, OrgId, UserId, UserId, iInventorySeq - 1, LocationId, ProductId,  Line, TSIT_nsQty1, "Y");
    Line += 10;
    iInventoryLineSeq++;
    MXSetValue (pmessage1, "Commit", 1, &Commit);
    MXSetValue (pmessage1, "SqlStatement", 1, Statement);
    MXSend (compiere_com->MX, compiere_com, pmessage1);
    MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));

//TRANSACTION


    sprintf (Statement, "insert into M_TRANSACTION (M_TRANSACTION_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, \
                                                    MOVEMENTTYPE, M_LOCATOR_ID, M_PRODUCT_ID, MOVEMENTQTY, MOVEMENTDATE, M_INVENTORYLINE_ID) \
                                 VALUES (%d, %s, %s, %s, %s, '%s', %s, %s, %s, %s'%s',%s, %d)",
                                 iTransactionSeq, ClientId, OrgId, UserId, UserId, 
                                 "I+",  LocationId, ProductId, TSIT_nsQty1, "to_date (", SDate, "'mm/dd/yyyy')",iInventoryLineSeq-1);

    MXSetValue (pmessage1, "Commit", 1, &Commit);
    MXSetValue (pmessage1, "SqlStatement", 1, Statement);
    MXSend (compiere_com->MX, compiere_com, pmessage1);
    MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
    iTransactionSeq++;



//STORAGE
    sprintf (Statement, "insert into M_STORAGE (M_PRODUCT_ID, M_LOCATOR_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, \
                                                QTYONHAND, DATELASTINVENTORY) \
                         VALUES (%s, %s, %s, %s, %s, %s, %s, %s'%s',%s)",
                         ProductId, LocationId, ClientId, OrgId, UserId, UserId, 
                         TSIT_nsQty1, "to_date (", SDate, "'mm/dd/yyyy')");
   
    MXSetValue (pmessage1, "Commit", 1, &Commit);
    MXSetValue (pmessage1, "SqlStatement", 1, Statement);
    MXSend (compiere_com->MX, compiere_com, pmessage1);
    MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
    MXFreeMessage (compiere_com->MX, pmessage1);
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



void main(int argc, char* argv[])
{
    MX mx;
    LastTransReference[0] = 0;
    
    if (argc > 2)
   {
       strcpy (Parameter, argv[1]);
       Year = atoi ( argv[2]);
   }
   else
       return;

    sprintf (Statement,  "SELECT StkItemsTransactions.TATrs_nlTransactionId, StkItemsTransactions.Name, AccTransactionHeader.TATrs_nlVoucherNbr, vew_TransactionRules.TATrs_sTransDescEng, StkItemsTransactions.TSIT_nsQty1, StkItemsTransactions.TSIT_nsQty2, AccTransactionHeader.TATrs_dOperationDate, AccTransactionHeader.ToName, AccTransactionHeader.TATrs_sTransReference, StkItems.Devise, StkItems.Location,  StkItemsTransactions.TSIT_ncUnitPrice, StkItemsTransactions.TSIT_ncUnitPriceLL, StkItemsTransactions.TSIT_nsDiscount, StkItemsTransactions.TSIT_ncTotalPrice, vew_AccTiersCategory.TAccT_sDesc \
            FROM ((((StkItemsTransactions INNER JOIN AccTransactionHeader ON StkItemsTransactions.TATrs_nlTransactionId = AccTransactionHeader.TATrs_nlTransactionId) INNER JOIN vew_TransactionRules ON AccTransactionHeader.TATrs_nlTransactionType = vew_TransactionRules.TATrs_nlTransactionType) INNER JOIN StkItems ON StkItemsTransactions.Name = StkItems.Name) INNER JOIN AccTransactionDetails ON AccTransactionHeader.TATrs_nlTransactionId = AccTransactionDetails.TATrs_nlTransactionId) INNER JOIN vew_AccTiersCategory ON AccTransactionDetails.TAccT_nlTiersId = vew_AccTiersCategory.TAccT_nlTiersId \
            WHERE (((vew_TransactionRules.TATrs_sTransDescEng)='%s') AND ((AccTransactionHeader.TATrs_dOperationDate)<=#12/31/%d#)) \
            ORDER BY StkItemsTransactions.TATrs_nlTransactionId, StkItemsTransactions.Name, AccTransactionHeader.TATrs_dOperationDate", Parameter, Year);
        


    MXInit (&mx, MXCLIENT, "base", "base", 0, NULL);

   
    omega_com = MXOpenBase (&mx, "omega", "","", DBPROTO_ODBC, TRUE);
    if (!omega_com) return;
    compiere_com = MXOpenBase (&mx, "compiere", "scott","tiger", DBPROTO_ODBC, TRUE);
    if (!compiere_com) return;

    MXAddComCallBack (&mx, omega_com,    DB_SYS, "AckStatement",   MXONRECV,   AckExcuteQuery, compiere_com); 

    
    
    SelectId ("Techno Cars", ClientId, CLIENT_ID);
    SelectId ("Techno Cars", OrgId, ORG_ID);
    SelectId ("SuperUser",   UserId, USER_ID);
    SelectId ("Standard",    WarehouseId, WAREHOUSE_ID);
    SelectId ("Physical Inventory",  DocumentId, DOCUMENT_ID);



    SelectSeq ("M_Transaction", TransactionSeq);
    iTransactionSeq = atol (TransactionSeq);

    SelectSeq ("M_Inventory", InventorySeq);
    iInventorySeq = atol (InventorySeq);

    SelectSeq ("M_InventoryLine", InventoryLineSeq);
    iInventoryLineSeq = atol (InventoryLineSeq);

    MXAddComCallBack (&mx, compiere_com, DB_SYS, "AckStatement",   MXONRECV,   AckExcuteUpdate, omega_com); 
 
    SelectInventoryRows (&mx);
    MXDispatchEvents (&mx, 0);

    MXEnd (&mx);

}