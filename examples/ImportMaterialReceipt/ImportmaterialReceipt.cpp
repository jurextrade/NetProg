
#include "sc.h"


char Parameter[100];
char Statement[10000];

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
#define PAYMENTTERM_ID      11
#define PRICELIST_ID        12
#define CONVERSIONTYPE_ID   13
#define TAX_ID              14
#define PRICELIST_VERSION_ID 15


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
char PriceListVersionId[10];
char PaymentTermId[10];
char CurrencyId[10];
char SalesRepId[10];
char ConversionTypeId[10];
char TaxId[10];

char VStr[5];


char InOutSeq[20];
int  iInOutSeq;
char InOutLineSeq[20];
int  iInOutLineSeq;
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

void strTakeOffQuote (char* str1, char* str)
{
    while (*str)
    {
        if (*str == '\'') 
        { *str1 = '\'';
          str1++;
          *str1 = '\'';
        }
        else    *str1 = *str;          
        str1++;
        str++;
    }
   *str1 = 0;
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
     case TAX_ID :
            sprintf (Statement, "%s'%s'%s%s;", "select C_TAX_ID from C_TAX where NAME = ", Name, " and AD_CLIENT_ID = ", ClientId);
         Table_id = "C_TAX_ID";
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
           sprintf (Statement, "%s%s;", "select C_BPARTNER_LOCATION_ID  from C_BPARTNER_LOCATION where C_BPARTNER_ID = ", Name);
           Table_id = "C_BPARTNER_LOCATION_ID";
       break;
    case  DOCUMENT_ID :
           sprintf (Statement, "%s'%s'%s%s;", "select C_DOCTYPE_ID  from C_DOCTYPE where NAME = ", Name, "  and AD_CLIENT_ID = ", ClientId);
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
    case  PAYMENTTERM_ID :
           sprintf (Statement, "%s'%s';", "select C_PAYMENTTERM_ID  from C_PAYMENTTERM where Name = ", Name);
           Table_id = "C_PAYMENTTERM_ID";
       break;
    case  CURRENCY_ID :
           sprintf (Statement, "%s'%s';", "select C_CURRENCY_ID  from C_CURRENCY where ISO_CODE = ", Name);
           Table_id = "C_CURRENCY_ID";
       break;
    case  PRICELIST_ID :
           sprintf (Statement, "%s'%s';", "select M_PRICELIST_ID  from M_PRICELIST where DESCRIPTION = ", Name);
           Table_id = "M_PRICELIST_ID";
       break;
    case  PRICELIST_VERSION_ID :
           sprintf (Statement, "%s'%s';", "select M_PRICELIST_VERSION_ID  from M_PRICELIST_VERSION where Name = ", Name);
           Table_id = "M_PRICELIST_VERSION_ID";
       break;
    case  CONVERSIONTYPE_ID :
           sprintf (Statement, "%s'%s';", "select C_CONVERSIONTYPE_ID  from C_CONVERSIONTYPE where NAME = ", Name);
           Table_id = "C_CONVERSIONTYPE_ID";
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
    MXFreeMessage (compiere_com->MX, pmessage1);
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
    MXFreeMessage (compiere_com->MX, pmessage1);
    pmessage1 =MXRecv (compiere_com->MX, compiere_com);  // Ack
    MXFreeMessage (compiere_com->MX, pmessage1);
    MXFreeMessage (compiere_com->MX, pmessage);
}


int SelectStorage (char* ProductId, char* LocationId)
{
    char Statement[250];
    int every = 1;
    int max = 1;
    MXMessage* pmessage;
    MXMessage* pmessage1;

    sprintf (Statement, "select M_PRODUCT_ID from M_STORAGE WHERE M_PRODUCT_ID = %s AND M_LOCATOR_ID = %s ", ProductId, LocationId);

    pmessage =  MXPutMessage (compiere_com, DB_SYS, "ExecuteQuery");
    MXSetValue (pmessage ,"Every", 1, &every);
    MXSetValue (pmessage ,"MaxRows", 1, &max);
    MXSetValue (pmessage ,"Name",    1,         "Statement");
    MXSetValue (pmessage ,"SqlStatement", 1, Statement);
    MXSend (compiere_com->MX, compiere_com, pmessage);

    pmessage1 = MXRecv (compiere_com->MX, compiere_com);    //header
    MXFreeMessage (compiere_com->MX, pmessage1);
    pmessage1 = MXRecv (compiere_com->MX, compiere_com);    //contents

    if (strcmp (pmessage1->MessageClass->Name, "AckStatement") == 0)
    {
        MXFreeMessage (compiere_com->MX, pmessage1);
        MXFreeMessage (compiere_com->MX, pmessage);
        return NULL;
    }
    else
    {
        MXFreeMessage (compiere_com->MX, pmessage1);
        pmessage1 =MXRecv (compiere_com->MX, compiere_com);  // Ack
        MXFreeMessage (compiere_com->MX, pmessage1);
        MXFreeMessage (compiere_com->MX, pmessage);
        return 1;
    }
}

int SelectOrderId (char* Transaction, char* OrderId)
{
    char Statement[250];
    int every = 1;
    int max = 1;
    MXMessage* pmessage;
    MXMessage* pmessage1;

    sprintf (Statement, "select C_ORDER_ID from C_ORDER WHERE DOCUMENTNO = '%s'", Transaction);

    pmessage =  MXPutMessage (compiere_com, DB_SYS, "ExecuteQuery");
    MXSetValue (pmessage ,"Every", 1, &every);
    MXSetValue (pmessage ,"MaxRows", 1, &max);
    MXSetValue (pmessage ,"Name",    1,         "Statement");
    MXSetValue (pmessage ,"SqlStatement", 1, Statement);
    MXSend (compiere_com->MX, compiere_com, pmessage);

    pmessage1 = MXRecv (compiere_com->MX, compiere_com);    //header
    MXFreeMessage (compiere_com->MX, pmessage1);
    pmessage1 = MXRecv (compiere_com->MX, compiere_com);    //contents
    if (strcmp (pmessage1->MessageClass->Name, "AckStatement") == 0)
    {
        MXFreeMessage (compiere_com->MX, pmessage1);
        MXFreeMessage (compiere_com->MX, pmessage);
        return NULL;
    }
    else
    {
        strcpy (OrderId, strend ((STRING)MXGetValue (pmessage1, "C_ORDER_ID", 1), " "));
        MXFreeMessage (compiere_com->MX, pmessage1);
        pmessage1 =MXRecv (compiere_com->MX, compiere_com);  // Ack
        MXFreeMessage (compiere_com->MX, pmessage1);
        MXFreeMessage (compiere_com->MX, pmessage);
        return 1;
    }
}

int SelectOrderLineId (char* OrderId, char* ProductId, char* OrderLineId)
{
    char Statement[250];
    int every = 1;
    int max = 1;
    MXMessage* pmessage;
    MXMessage* pmessage1;

    sprintf (Statement, "select C_ORDERLINE_ID from C_ORDERLINE WHERE C_ORDER_ID = %s AND M_PRODUCT_ID = %s", OrderId, ProductId);

    pmessage =  MXPutMessage (compiere_com, DB_SYS, "ExecuteQuery");
    MXSetValue (pmessage ,"Every", 1, &every);
    MXSetValue (pmessage ,"MaxRows", 1, &max);
    MXSetValue (pmessage ,"Name",    1,         "Statement");
    MXSetValue (pmessage ,"SqlStatement", 1, Statement);
    MXSend (compiere_com->MX, compiere_com, pmessage);

    pmessage1 = MXRecv (compiere_com->MX, compiere_com);    //header
    MXFreeMessage (compiere_com->MX, pmessage1);
    pmessage1 = MXRecv (compiere_com->MX, compiere_com);    //contents
    if (strcmp (pmessage1->MessageClass->Name, "AckStatement") == 0)
    {
        MXFreeMessage (compiere_com->MX, pmessage1);
        MXFreeMessage (compiere_com->MX, pmessage);
        return NULL;
    }
    else
    {
        strcpy (OrderLineId, strend ((STRING)MXGetValue (pmessage1, "C_ORDERLINE_ID", 1), " "));
        MXFreeMessage (compiere_com->MX, pmessage1);
        pmessage1 =MXRecv (compiere_com->MX, compiere_com);  // Ack
        MXFreeMessage (compiere_com->MX, pmessage1);
        MXFreeMessage (compiere_com->MX, pmessage);
        return 1;
    }
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
        if (Year != 2004)
        {
            
            sprintf (Statement, "%s%d%s" , "update AD_SEQUENCE set CURRENTNEXT = ",  iInOutSeq, " WHERE NAME = 'M_InOut'");
            MXSetValue (pmessage1, "SqlStatement", 1, Statement );
            MXSetValue (pmessage1, "Commit", 1, &Commit);
            MXSend (compiere_com->MX, compiere_com, pmessage1);
            MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));


            sprintf (Statement, "%s%d%s" , "update AD_SEQUENCE set CURRENTNEXT = ",  iInOutLineSeq, " WHERE NAME = 'M_InOutLine'");
            MXSetValue (pmessage1, "SqlStatement", 1, Statement );
            MXSetValue (pmessage1, "Commit", 1, &Commit);
            MXSend (compiere_com->MX, compiere_com, pmessage1);
            MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
        
		}
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



int SelectInOut (MXMessage* pmessage, MXCom* pcom, void* appfield)
{  
    MXMessage* pmessage1;
    BYTE Commit = '1';
    char LocationId[100];
    char OrderId[30];
    char OrderLineId[30];

    pmessage1 =  MXCreateMessage (compiere_com->MX, DB_SYS, "ExecuteUpdate");

    STRING Name                 =    strend ((STRING) MXGetValue (pmessage, "name", 1), " ");
    STRING TSIT_nsQty1          =    strend ((STRING)MXGetValue (pmessage, "TSIT_nsQty1", 1), " ");
    STRING Location             =    strend ((STRING)MXGetValue (pmessage, "Location", 1), " ");
    STRING TATrs_dOperationDate =    strend ((STRING)MXGetValue (pmessage, "TATrs_dOperationDate", 1), " ");
    STRING TATrs_sTransReference=    strend ((STRING)MXGetValue (pmessage, "TATrs_sTransReference", 1), " ");




    OrderId[0] = 0;
    SelectOrderId (TATrs_sTransReference, OrderId);
    STRING TAccT_sDesc;
    if (strcmp (Parameter, "Purchase") == 0)
    {
        TAccT_sDesc          =    strend ((STRING)MXGetValue (pmessage, "TAccT_sDesc", 1), " ");
        TATrs_sTransReference[0] = 'M';
        TATrs_sTransReference[1] = 'T';
        TATrs_sTransReference[2] = 'R';
    }

    else
    {
        char QToName[100];
        TAccT_sDesc          =    strend ((STRING) MXGetValue (pmessage, "ToName", 1), " ");

        strTakeOffQuote (QToName, TAccT_sDesc);
        strcpy (TAccT_sDesc, QToName);
        TSIT_nsQty1 =  TSIT_nsQty1 + 1;

        TATrs_sTransReference[0] = 'S';
        TATrs_sTransReference[1] = 'H';
        TATrs_sTransReference[2] = 'P';
    }   
    



    char Statement[1000];
    char SDate[100];
    char syear[100];
    char smonth[100];
    char sday[100];
    strcpy (syear, strtok (TATrs_dOperationDate, "-"));
    strcpy (smonth, strtok(NULL, "-"));
    strcpy (sday,  strtok(NULL, " "));
    sprintf (SDate, "%s/%s/%s", smonth, sday, syear);


    for (int k = 0; k < (int)strlen (Location); k++)
        Location[k] = toupper(Location[k]);

    SelectId (Location,    LocationId,         LOCATOR_ID);
    SelectId (Name,        ProductId,          PRODUCT_ID);
    SelectId (ProductId,   UOMId,              UOM_ID);
    
    
    if (TAccT_sDesc[0] !=0) 
    {
        SelectId (TAccT_sDesc, BPartnerId,         BPARTNER_ID);
        SelectId (BPartnerId,  BPartnerLocationId, BPARTNERLOCATION_ID);
        



        if (strcmp (TATrs_sTransReference, LastTransReference) != 0)
        {

            strcpy (LastTransReference, TATrs_sTransReference);
            sprintf (Statement, "INSERT INTO M_INOUT  (M_INOUT_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, C_ORDER_ID, \
                                                    ISSOTRX, DOCUMENTNO, DOCACTION, DOCSTATUS, PROCESSED, C_DOCTYPE_ID, DESCRIPTION, \
                                                    MOVEMENTTYPE, MOVEMENTDATE, DATEACCT, C_BPARTNER_ID, C_BPARTNER_LOCATION_ID, \
                                                    M_WAREHOUSE_ID, DELIVERYRULE, FREIGHTCOSTRULE, DELIVERYVIARULE, PRIORITYRULE, ISAPPROVED)\
                                VALUES (%d, %s, %s, %s, %s, %s, \
                                '%s', '%s', '%s', '%s', '%s', %s, '%s',\
                                '%s',  %s'%s',%s, %s'%s',%s, %s, %s, \
                                %s, '%s', '%s', '%s', '%s', '%s')",  
                                iInOutSeq, ClientId, OrgId, UserId, UserId, OrderId,
                                "N", LastTransReference,  "--", "CL", "Y", DocumentId, "", 
                                VStr, "to_date (", SDate, "'mm/dd/yyyy')", "to_date (", SDate, "'mm/dd/yyyy')", BPartnerId, BPartnerLocationId, 
                                WarehouseId, "A", "I", "P", "5", "Y");
            MXSetValue (pmessage1, "Commit", 1, &Commit);
            MXSetValue (pmessage1, "SqlStatement", 1, Statement);
            MXSend (compiere_com->MX, compiere_com, pmessage1);
            MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
            iInOutSeq++;
            Line = 10;
        }

    //NOUTLINE
        OrderLineId[0] = 0;
        SelectOrderLineId (OrderId, ProductId, OrderLineId);


        sprintf (Statement, "INSERT INTO M_INOUTLINE  (M_INOUTLINE_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, C_ORDERLINE_ID, \
                                                    LINE, M_INOUT_ID, M_LOCATOR_ID, M_PRODUCT_ID, C_UOM_ID, MOVEMENTQTY,\
                                                    ISINVOICED, PROCESSED, QTYENTERED)\
                            VALUES (%d, %s, %s, %s, %s, %s, \
                            %d,  %d, %s, %s, %s, %s,\
                            '%s', '%s', %s)",
                            iInOutLineSeq, ClientId, OrgId, UserId, UserId, OrderLineId, 
                            Line, iInOutSeq - 1, LocationId, ProductId, UOMId, TSIT_nsQty1,
                            "N", "Y",  TSIT_nsQty1);
        Line += 10;
        iInOutLineSeq++;
        MXSetValue (pmessage1, "Commit", 1, &Commit);
        MXSetValue (pmessage1, "SqlStatement", 1, Statement);
        MXSend (compiere_com->MX, compiere_com, pmessage1);
        MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
    }
    //TRANSACTION
    if (strcmp (Parameter, "Purchase") != 0)
        TSIT_nsQty1 =  TSIT_nsQty1 - 1;

    if (Year == 2005)
    {
        sprintf (Statement, "insert into M_TRANSACTION (M_TRANSACTION_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, \
                                                        MOVEMENTTYPE, M_LOCATOR_ID, M_PRODUCT_ID, MOVEMENTQTY, MOVEMENTDATE) \
                                VALUES (%d, %s, %s, %s, %s, '%s', %s, %s, %s, %s'%s',%s)",
                                iTransactionSeq, ClientId, OrgId, UserId, UserId, 
                                VStr,  LocationId, ProductId, TSIT_nsQty1, "to_date (", SDate, "'mm/dd/yyyy')");


    }
    else
    {
        sprintf (Statement, "insert into M_TRANSACTION (M_TRANSACTION_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, \
                                                        MOVEMENTTYPE, M_LOCATOR_ID, M_PRODUCT_ID, MOVEMENTQTY, MOVEMENTDATE, M_INOUTLINE_ID) \
                                VALUES (%d, %s, %s, %s, %s, '%s', %s, %s, %s, %s'%s',%s, %d)",
                                iTransactionSeq, ClientId, OrgId, UserId, UserId, 
                                VStr,  LocationId, ProductId, TSIT_nsQty1, "to_date (", SDate, "'mm/dd/yyyy')", iInOutLineSeq-1);
    }


    MXSetValue (pmessage1, "Commit", 1, &Commit);
    MXSetValue (pmessage1, "SqlStatement", 1, Statement);
    MXSend (compiere_com->MX, compiere_com, pmessage1);
    MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
    iTransactionSeq++;

    

//STORAGE

    if (strcmp (Parameter, "Purchase") != 0)
        TSIT_nsQty1 =  TSIT_nsQty1 + 1;

    if (SelectStorage (ProductId, LocationId) == 1)
    {
        if (strcmp (Parameter, "Purchase") == 0)
            sprintf (Statement, "update M_STORAGE SET QTYONHAND = QTYONHAND + %s, QTYORDERED = 0 WHERE M_PRODUCT_ID = %s AND M_LOCATOR_ID = %s ", TSIT_nsQty1, ProductId, LocationId); 
        else
            sprintf (Statement, "update M_STORAGE SET QTYONHAND = QTYONHAND - %s, QTYRESERVED = 0 WHERE M_PRODUCT_ID = %s AND M_LOCATOR_ID = %s ", TSIT_nsQty1, ProductId, LocationId); 
    }
    else
    { 
        if (strcmp (Parameter, "Purchase") == 0)
            sprintf (Statement, "insert into M_STORAGE (M_PRODUCT_ID, M_LOCATOR_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, QTYONHAND) \
                         VALUES (%s, %s, %s, %s, %s, %s, %s)",
                         ProductId, LocationId, ClientId, OrgId, UserId, UserId, TSIT_nsQty1);
        else
            sprintf (Statement, "insert into M_STORAGE (M_PRODUCT_ID, M_LOCATOR_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, QTYONHAND) \
                         VALUES (%s, %s, %s, %s, %s, %s, %s%s)",
                         ProductId, LocationId, ClientId, OrgId, UserId, UserId, "-", TSIT_nsQty1);
        printf ("%s\n", "NO PRODUCT");

    }
    MXSetValue (pmessage1, "Commit", 1, &Commit);
    MXSetValue (pmessage1, "SqlStatement", 1, Statement);
    MXSend (compiere_com->MX, compiere_com, pmessage1);
    MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
    MXFreeMessage (compiere_com->MX, pmessage1);
    j++; printf ("<--- %d\n", j);
  return 1;

}   

void SelectInOutRows (MX* pmx)
{
    MXMessage* pmessage;
    int every = 1;
    int max = Count;
    int i = 0;
    BYTE Commit = 1;
 
         
    MXAddComCallBack (pmx, omega_com, DB_SYS,  "SelectInOut", MXONRECV, SelectInOut, compiere_com); 


    pmessage = MXCreateMessage (pmx, DB_SYS, "ExecuteQuery");
    MXSetValue (pmessage ,"Every", 1, &every);
    MXSetValue (pmessage ,"MaxRows", 1, &max);
   
    MXSetValue (pmessage ,"Name",    1,    "SelectInOut"  );
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

   // sprintf (Statement,  "SELECT StkItemsTransactions.TATrs_nlTransactionId, StkItemsTransactions.Name, AccTransactionHeader.TATrs_nlVoucherNbr, vew_TransactionRules.TATrs_sTransDescEng, StkItemsTransactions.TSIT_nsQty1, StkItemsTransactions.TSIT_nsQty2, AccTransactionHeader.TATrs_dOperationDate, AccTransactionHeader.ToName, AccTransactionHeader.TATrs_sTransReference, StkItems.Devise, StkItems.Location,  StkItemsTransactions.TSIT_ncUnitPrice, StkItemsTransactions.TSIT_ncUnitPriceLL, StkItemsTransactions.TSIT_nsDiscount, StkItemsTransactions.TSIT_ncTotalPrice, vew_AccTiersCategory.TAccT_sDesc \
   //         FROM ((((StkItemsTransactions INNER JOIN AccTransactionHeader ON StkItemsTransactions.TATrs_nlTransactionId = AccTransactionHeader.TATrs_nlTransactionId) INNER JOIN vew_TransactionRules ON AccTransactionHeader.TATrs_nlTransactionType = vew_TransactionRules.TATrs_nlTransactionType) INNER JOIN StkItems ON StkItemsTransactions.Name = StkItems.Name) INNER JOIN AccTransactionDetails ON AccTransactionHeader.TATrs_nlTransactionId = AccTransactionDetails.TATrs_nlTransactionId) INNER JOIN vew_AccTiersCategory ON AccTransactionDetails.TAccT_nlTiersId = vew_AccTiersCategory.TAccT_nlTiersId \
   //         WHERE (((vew_TransactionRules.TATrs_sTransDescEng)='%s') AND ((AccTransactionHeader.TATrs_dOperationDate)<#12/31/%d#)) \
   //         ORDER BY StkItemsTransactions.TATrs_nlTransactionId, StkItemsTransactions.Name, AccTransactionHeader.TATrs_dOperationDate", Parameter, Year);
        
    if (strcmp (Parameter, "Purchase") == 0)
    {
         sprintf (Statement,  "SELECT StkItemsTransactions.TATrs_nlTransactionId, StkItemsTransactions.Name, AccTransactionHeader.TATrs_ncTotInv, AccTransactionHeader.TATrs_nlVoucherNbr, vew_TransactionRules.TATrs_sTransDescEng, StkItemsTransactions.TSIT_nsQty1, StkItemsTransactions.TSIT_nsQty2, AccTransactionHeader.TATrs_dOperationDate, AccTransactionHeader.ToName, AccTransactionHeader.TATrs_sTransReference, StkItems.Devise, StkItems.Location,  StkItemsTransactions.TSIT_ncUnitPrice, StkItemsTransactions.TSIT_ncUnitPriceLL, StkItemsTransactions.TSIT_nsDiscount, StkItemsTransactions.TSIT_ncTotalPrice, vew_AccTiersCategory.TAccT_sDesc \
            FROM ((((StkItemsTransactions INNER JOIN AccTransactionHeader ON StkItemsTransactions.TATrs_nlTransactionId = AccTransactionHeader.TATrs_nlTransactionId) INNER JOIN vew_TransactionRules ON AccTransactionHeader.TATrs_nlTransactionType = vew_TransactionRules.TATrs_nlTransactionType) INNER JOIN StkItems ON StkItemsTransactions.Name = StkItems.Name) INNER JOIN AccTransactionDetails ON AccTransactionHeader.TATrs_nlTransactionId = AccTransactionDetails.TATrs_nlTransactionId) INNER JOIN vew_AccTiersCategory ON AccTransactionDetails.TAccT_nlTiersId = vew_AccTiersCategory.TAccT_nlTiersId \
			WHERE (((vew_TransactionRules.TATrs_sTransDescEng) Like '%s')  AND   ((AccTransactionHeader.TATrs_dOperationDate) %s) AND ((AccTransactionDetails.TATDt_sDesc ='Purchase') OR (AccTransactionDetails.TATDt_sDesc Like 'Cash*'))) \
            ORDER BY StkItemsTransactions.TATrs_nlTransactionId, StkItemsTransactions.Name, AccTransactionHeader.TATrs_dOperationDate",  "%Purch%", ((Year == 2004) ? "<= #12/31/2004#" : "> #12/31/2004#"));


        strcpy (VStr, "V+");
    }
    else
    {
      sprintf (Statement,  "SELECT StkItemsTransactions.TATrs_nlTransactionId, StkItems.Location, StkItemsTransactions.Name, AccTransactionHeader.TATrs_nlVoucherNbr, vew_TransactionRules.TATrs_sTransDescEng, StkItemsTransactions.TSIT_nsQty1, AccTransactionHeader.TATrs_dOperationDate, AccTransactionHeader.ToName, AccTransactionHeader.TATrs_sTransReference, StkItems.Devise, StkItemsTransactions.TSIT_ncVATLL, StkItemsTransactions.TSIT_ncUnitPriceLL, StkItemsTransactions.TSIT_nsDiscount, StkItemsTransactions.TSIT_ncTotalPrice, AccTransactionDetails.TATDt_ncAmountLL, AccTransactionDetails.TATDt_sDesc, AccTransactionHeader.TATrs_ncVATLL, AccTransactionDetails.TATDt_bIsDebit \
                            FROM (((StkItemsTransactions INNER JOIN AccTransactionHeader ON StkItemsTransactions.TATrs_nlTransactionId = AccTransactionHeader.TATrs_nlTransactionId) INNER JOIN vew_TransactionRules ON AccTransactionHeader.TATrs_nlTransactionType = vew_TransactionRules.TATrs_nlTransactionType) INNER JOIN StkItems ON StkItemsTransactions.Name = StkItems.Name) INNER JOIN AccTransactionDetails ON AccTransactionHeader.TATrs_nlTransactionId = AccTransactionDetails.TATrs_nlTransactionId \
                            WHERE (((vew_TransactionRules.TATrs_sTransDescEng) Like '%s') AND ((AccTransactionHeader.TATrs_dOperationDate) %s) AND ((AccTransactionDetails.TATDt_bIsDebit)=-1)) \
                            ORDER BY StkItemsTransactions.TATrs_nlTransactionId, StkItemsTransactions.Name",  "Credit Sale", (Year == 2004) ? "<= #12/31/2004#" : "> #12/31/2004#");

        
        strcpy (VStr, "C-");
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
    SelectId ("Standard",    WarehouseId, WAREHOUSE_ID);
    if (strcmp (Parameter, "Purchase") == 0)
        SelectId ("MM Receipt",  DocumentId, DOCUMENT_ID);
    else
        SelectId ("MM Shipment",  DocumentId, DOCUMENT_ID);

    



    SelectSeq ("M_Transaction", TransactionSeq);
    iTransactionSeq = atol (TransactionSeq);

    SelectSeq ("M_InOut", InOutSeq);
    iInOutSeq = atol (InOutSeq);

    SelectSeq ("M_InOutLine", InOutLineSeq);
    iInOutLineSeq = atol (InOutLineSeq);

    MXAddComCallBack (&mx, compiere_com, DB_SYS, "AckStatement",   MXONRECV,   AckExcuteUpdate, omega_com); 
 
    SelectInOutRows (&mx);
    MXDispatchEvents (&mx, 0);

    MXEnd (&mx);

}