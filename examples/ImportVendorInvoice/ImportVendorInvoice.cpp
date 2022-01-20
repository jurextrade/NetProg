
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
#define CHARGE_ID            16

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
char EUR2004PriceListId[10];
char EUR2004PriceListVersionId[10];
char EUR2005PriceListId[10];
char EUR2005PriceListVersionId[10];

char VOREUR2004PriceListId[10];
char VOREUR2004PriceListVersionId[10];
char VOREUR2005PriceListId[10];
char VOREUR2005PriceListVersionId[10];
char LBPPriceListId[10];
char LBPPriceListVersionId[10];
char USDPriceListId[10];
char USDPriceListVersionId[10];
char ImmediatePaymentTermId[10];
char CreditPaymentTermId[10];
char EURCurrencyId[10];
char USDCurrencyId[10];
char LBPCurrencyId[10];
char SalesRepId[10];
char ConversionTypeId[10];
char TaxId[10];
char TVAId[10];
char ChargeId[10];

char InvoiceSeq[20];
int  iInvoiceSeq;
char InvoiceLineSeq[20];
int  iInvoiceLineSeq;

int Line = 10;
double iGrandTotal = 0;
char GrandTotal[20];

char InvoiceTotal[20];
double iInvoiceTotal = 0;

char ImportPurchase[100];

double iTotalTax = 0;
char TotalTax[20];
char Tax[20];


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
           sprintf (Statement, "%s'%s'%s%s;", "select C_DOCTYPE_ID  from C_DOCTYPE where NAME = ", Name," and AD_CLIENT_ID = ", ClientId);
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
    case  CHARGE_ID :
           sprintf (Statement, "%s'%s';", "select C_CHARGE_ID  from C_CHARGE where NAME = ", Name);
           Table_id = "C_CHARGE_ID";
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

int SelectPrice (char* PriceListVersionId, char* ProductId, char* PriceList, char* StandardPrice, char* LimitPrice)
{
    char Statement[250];
    int every = 1;
    int max = 1;
    MXMessage* pmessage;
    MXMessage* pmessage1;

    sprintf (Statement, "select PRICELIST, PRICESTD, PRICELIMIT from M_PRODUCTPRICE WHERE M_PRODUCT_ID = %s AND M_PRICELIST_VERSION_ID = %s ", ProductId, PriceListVersionId);

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
        strcpy (PriceList,     strend ((STRING) MXGetValue (pmessage1, "PRICELIST", 1), " "));
        strcpy (StandardPrice, strend ((STRING) MXGetValue (pmessage1, "PRICESTD", 1),  " "));
        strcpy (LimitPrice,    strend ((STRING) MXGetValue (pmessage1, "PRICELIMIT", 1), " "));
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
        if (LastTransReference[0] != 0)
        {
           pmessage1 =  MXCreateMessage (compiere_com->MX, DB_SYS, "ExecuteUpdate");
           if (strcmp (ImportPurchase, "Import Credit Purchase") == 0)
            {
                double iFreightCharges = 0;
                char FreightCharges[20];

                iFreightCharges = iInvoiceTotal - iGrandTotal;      
                sprintf (FreightCharges, "%2.2f", iFreightCharges);
                sprintf (Statement, "INSERT INTO C_INVOICELINE  (C_INVOICELINE_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, \
                                                                C_INVOICE_ID, LINE, DESCRIPTION,\
                                                                QTYINVOICED, PRICELIST, PRICEACTUAL, PRICELIMIT, LINENETAMT, C_CHARGE_ID, LINETOTALAMT, \
                                                                C_UOM_ID, C_TAX_ID, PROCESSED, QTYENTERED, PRICEENTERED)\
                                VALUES (%d, %s, %s, %s, %s, \
                                %d,  %d, '%s',\
                                %s,  %s, %s, %s, %s, %s, %s, \
                                %s, %s, '%s', %s, %s)",
                                iInvoiceLineSeq, ClientId, OrgId, UserId, UserId,
                                iInvoiceSeq - 1, Line,  "Freight Charge", 
                                "1",  "0", FreightCharges, "0", FreightCharges, ChargeId, FreightCharges, 
                                UOMId, TaxId, "N", "1", FreightCharges);
                Line += 10;
                iInvoiceLineSeq++;
                MXSetValue (pmessage1, "Commit", 1, &Commit);
                MXSetValue (pmessage1, "SqlStatement", 1, Statement);
                MXSend (compiere_com->MX, compiere_com, pmessage1); 
                MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
                iGrandTotal = iInvoiceTotal;
            }
            char TotalWithTax[20];
            if (strcmp (Tax, TVAId) == 0)
                iTotalTax = (iGrandTotal * 10)/100;
            else
                iTotalTax = 0;
              
            sprintf (TotalTax, "%2.2f", iTotalTax);
            sprintf (GrandTotal, "%2.2f", iGrandTotal);
            sprintf (TotalWithTax, "%2.2f", iGrandTotal + iTotalTax);

            sprintf (Statement, "INSERT INTO C_INVOICETAX (C_INVOICE_ID, C_TAX_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, TAXBASEAMT, TAXAMT, PROCESSED) \
                                VALUES (%d, %s, %s, %s, %s, %s, %s, %s, '%s')", 
                                iInvoiceSeq-1, Tax, ClientId, OrgId, UserId, UserId,  
                                GrandTotal, TotalTax, "N");

            MXSetValue (pmessage1, "Commit", 1, &Commit);
            MXSetValue (pmessage1, "SqlStatement", 1, Statement);
            MXSend (compiere_com->MX, compiere_com, pmessage1);
            MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
              
              
            sprintf (Statement, "update C_INVOICE set TOTALLINES = %s, GRANDTOTAL = %s where DOCUMENTNO = '%s'", GrandTotal, TotalWithTax, LastTransReference);
            MXSetValue (pmessage1, "Commit", 1, &Commit);
            MXSetValue (pmessage1, "SqlStatement", 1, Statement);
            MXSend (compiere_com->MX, compiere_com, pmessage1);
            MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
            MXFreeMessage (compiere_com->MX, pmessage1);
            
        }
        pmessage1 =  MXPutMessage (compiere_com, DB_SYS, "ExecuteUpdate");
        sprintf (Statement, "%s%d%s" , "update AD_SEQUENCE set CURRENTNEXT = ",  iInvoiceSeq, " WHERE NAME = 'C_Invoice'");
        MXSetValue (pmessage1, "SqlStatement", 1, Statement );
        MXSetValue (pmessage1, "Commit", 1, &Commit);
        MXSend (compiere_com->MX, compiere_com, pmessage1);
        MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));


        sprintf (Statement, "%s%d%s" , "update AD_SEQUENCE set CURRENTNEXT = ",  iInvoiceLineSeq, " WHERE NAME = 'C_InvoiceLine'");
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



int SelectInvoice (MXMessage* pmessage, MXCom* pcom, void* appfield)
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
    STRING TAccT_sDesc          =    strend ((STRING)MXGetValue (pmessage, "TAccT_sDesc", 1), " ");
    STRING TSIT_ncUnitPrice     =    strend ((STRING)MXGetValue (pmessage, "TSIT_ncUnitPrice", 1), " ");
    STRING TATrs_ncTotInv       =    strend ((STRING)MXGetValue (pmessage, "TATrs_ncTotInv", 1), " ");
    STRING Devise               =    strend ((STRING)MXGetValue (pmessage, "Devise", 1), " ");
    STRING TATrs_sTransDescEng  =    strend ((STRING)MXGetValue (pmessage, "TATrs_sTransDescEng", 1), " ");

    OrderId[0] = 0;
    SelectOrderId (TATrs_sTransReference, OrderId);


    
    for (int k = 0; k < (int)strlen (Location); k++)
        Location[k] = toupper(Location[k]);

    SelectId (Location,    LocationId,         LOCATOR_ID);
    SelectId (Name,        ProductId,          PRODUCT_ID);
    SelectId (TAccT_sDesc, BPartnerId,         BPARTNER_ID);
    SelectId (BPartnerId,  BPartnerLocationId, BPARTNERLOCATION_ID);
    SelectId (ProductId,   UOMId,              UOM_ID);

    char SDate[100];
    char syear[100];
    char smonth[100];
    char sday[100];
    strcpy (syear, strtok (TATrs_dOperationDate, "-"));
    strcpy (smonth, strtok(NULL, "-"));
    strcpy (sday,  strtok(NULL, " "));
    sprintf (SDate, "%s/%s/%s", smonth, sday, syear);


    int  iTSIT_nsQty1 = atoi (TSIT_nsQty1);
    STRING CurrencyId;
    STRING PriceListId;
    STRING PriceListVersionId;



 
	if ((strcmp (TATrs_sTransReference, "U00000024") == 0) ||
        (strcmp (TATrs_sTransReference, "U00000023") == 0) ||
		(strcmp (TATrs_sTransReference, "U00000022") == 0) ||
		(strcmp (TATrs_sTransReference, "U00000017") == 0) ||
		(strcmp (TATrs_sTransReference, "U00000003") == 0) ||
		(strcmp (TATrs_sTransReference, "RSO000107") == 0) ||
		(strcmp (TATrs_sTransReference, "U00000002") == 0))
	{
		PriceListId = EUR2005PriceListId;
		PriceListVersionId = EUR2005PriceListVersionId;
		CurrencyId = EURCurrencyId;
	}
	else
	if ((strcmp (TATrs_sTransReference, "U00000021") == 0) ||
		(strcmp (TATrs_sTransReference, "U00000013") == 0))
	{
		PriceListId = VOREUR2005PriceListId;
		PriceListVersionId = VOREUR2005PriceListVersionId;
		CurrencyId = EURCurrencyId;
	}
	else
	if ((strcmp (TATrs_sTransReference, "U00000008") == 0) ||
        (strcmp (TATrs_sTransReference, "U00000006") == 0) ||
		(strcmp (TATrs_sTransReference, "U00000020") == 0) ||
		(strcmp (TATrs_sTransReference, "U00000005") == 0))
	{
		PriceListId = EUR2004PriceListId;
		PriceListVersionId = EUR2004PriceListVersionId;
		CurrencyId = EURCurrencyId;
	}
	else
	if ((strcmp (TATrs_sTransReference, "U00000007") == 0) ||
        (strcmp (TATrs_sTransReference, "U00000015") == 0) ||
		(strcmp (TATrs_sTransReference, "U00000012") == 0) ||
		(strcmp (TATrs_sTransReference, "U00000010") == 0) ||
		(strcmp (TATrs_sTransReference, "U00000011") == 0) ||
		(strcmp (TATrs_sTransReference, "U00000009") == 0) ||
		(strcmp (TATrs_sTransReference, "U00000014") == 0))
	{
		PriceListId = VOREUR2004PriceListId;
		PriceListVersionId = VOREUR2004PriceListVersionId;
		CurrencyId = EURCurrencyId;
	}
	else
	if ((strcmp (TATrs_sTransReference, "U00000016") == 0)) 
	{
		PriceListId = LBPPriceListId;
		PriceListVersionId = LBPPriceListVersionId;
		CurrencyId = LBPCurrencyId;
	}
    else
    if (strcmp (Devise, "USD") == 0)
    {
        PriceListId = USDPriceListId;
        PriceListVersionId = USDPriceListVersionId;
        CurrencyId  = USDCurrencyId;
    }
    else
    if (strcmp (Devise, "LBP") == 0)
    {
        PriceListId = LBPPriceListId;
        PriceListVersionId = LBPPriceListVersionId;
        CurrencyId  = LBPCurrencyId;
    }


  
   
    STRING PaymentTermId;

    if (strcmp (TATrs_sTransDescEng, "Credit Purchase") == 0)
    PaymentTermId = CreditPaymentTermId;
    else
    PaymentTermId = ImmediatePaymentTermId;    

    if (strcmp (TATrs_sTransReference, LastTransReference) != 0)
    {
//INOUT
        if (LastTransReference[0] != 0)
        {
            if (strcmp (ImportPurchase, "Import Credit Purchase") == 0)
            {
                double iFreightCharges = 0;
                char FreightCharges[20];

                iFreightCharges = iInvoiceTotal - iGrandTotal;      
                sprintf (FreightCharges, "%2.2f", iFreightCharges);
                sprintf (Statement, "INSERT INTO C_INVOICELINE  (C_INVOICELINE_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, \
                                                                C_INVOICE_ID, LINE, DESCRIPTION,\
                                                                QTYINVOICED, PRICELIST, PRICEACTUAL, PRICELIMIT, LINENETAMT, C_CHARGE_ID, LINETOTALAMT, \
                                                                C_UOM_ID, C_TAX_ID, PROCESSED, QTYENTERED, PRICEENTERED)\
                                VALUES (%d, %s, %s, %s, %s, \
                                %d,  %d, '%s',\
                                %s,  %s, %s, %s, %s, %s, %s, \
                                %s, %s, '%s', %s, %s)",
                                iInvoiceLineSeq, ClientId, OrgId, UserId, UserId,
                                iInvoiceSeq - 1, Line,  "Freight Charge", 
                                "1",  "0", FreightCharges, "0", FreightCharges, ChargeId, FreightCharges, 
                                UOMId, TaxId, "N", "1", FreightCharges);
                Line += 10;
                iInvoiceLineSeq++;
                MXSetValue (pmessage1, "Commit", 1, &Commit);
                MXSetValue (pmessage1, "SqlStatement", 1, Statement);
                MXSend (compiere_com->MX, compiere_com, pmessage1); 
                MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
                iGrandTotal = iInvoiceTotal;
            }
            char TotalWithTax[20];
            if (strcmp (Tax, TVAId) == 0)
                iTotalTax = (iGrandTotal * 10)/100;
            else
                iTotalTax = 0;
              
            sprintf (TotalTax, "%2.2f", iTotalTax);
            sprintf (GrandTotal, "%2.2f", iGrandTotal);
            sprintf (TotalWithTax, "%2.2f", iGrandTotal + iTotalTax);

            sprintf (Statement, "INSERT INTO C_INVOICETAX (C_INVOICE_ID, C_TAX_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, TAXBASEAMT, TAXAMT, PROCESSED) \
                                VALUES (%d, %s, %s, %s, %s, %s, %s, %s, '%s')", 
                                iInvoiceSeq-1, Tax, ClientId, OrgId, UserId, UserId,  
                                GrandTotal, TotalTax, "N");

            MXSetValue (pmessage1, "Commit", 1, &Commit);
            MXSetValue (pmessage1, "SqlStatement", 1, Statement);
            MXSend (compiere_com->MX, compiere_com, pmessage1);
            MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
              
              
            sprintf (Statement, "update C_INVOICE set TOTALLINES = %s, GRANDTOTAL = %s where DOCUMENTNO = '%s'", GrandTotal, TotalWithTax, LastTransReference);
            MXSetValue (pmessage1, "Commit", 1, &Commit);
            MXSetValue (pmessage1, "SqlStatement", 1, Statement);
            MXSend (compiere_com->MX, compiere_com, pmessage1);
            MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
            
        }
    
        iGrandTotal = 0;
        iTotalTax = 0;
        iInvoiceTotal = atof (TATrs_ncTotInv);


        strcpy (LastTransReference, TATrs_sTransReference);

        sprintf (Statement, "INSERT INTO C_INVOICE  (C_INVOICE_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, C_ORDER_ID, \
                                                   ISSOTRX, DOCUMENTNO, DOCACTION, DOCSTATUS, PROCESSING, PROCESSED, C_DOCTYPE_ID, C_DOCTYPETARGET_ID, DESCRIPTION, \
                                                   ISAPPROVED, SALESREP_ID, DATEINVOICED, DATEACCT,  C_BPARTNER_ID, C_BPARTNER_LOCATION_ID, \
                                                   C_CURRENCY_ID, PAYMENTRULE, C_PAYMENTTERM_ID, \
                                                   TOTALLINES, GRANDTOTAL, M_PRICELIST_ID, C_CONVERSIONTYPE_ID)\
                             VALUES (%d, %s, %s, %s, %s, %s, \
                             '%s', '%s', '%s', '%s', '%s', '%s', %s, %s, '%s',\
                             '%s', %s,  %s'%s',%s, %s'%s',%s, %s, %s, \
                             %s, '%s', %s,  \
                             %s, %s, %s, %s)",  
                             iInvoiceSeq, ClientId, OrgId, UserId, UserId, OrderId,
                             "N", LastTransReference,  "CO", "DR", "N", "N", DocumentId, DocumentId, "", 
                             "Y", SalesRepId, "to_date (", SDate, "'mm/dd/yyyy')", "to_date (", SDate, "'mm/dd/yyyy')",  BPartnerId, BPartnerLocationId, 
                             CurrencyId, "T", PaymentTermId, 
                             "0", "0",  PriceListId,  ConversionTypeId);
        MXSetValue (pmessage1, "Commit", 1, &Commit);
        MXSetValue (pmessage1, "SqlStatement", 1, Statement);
        MXSend (compiere_com->MX, compiere_com, pmessage1);
        MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
        iInvoiceSeq++;
        Line = 10;
    }

//NOUTLINE
    char PriceList[30];
    char StandardPrice[30];
    char LimitPrice[30];
    char Discount[30];
    char TotalPrice[30];
     
   

   SelectPrice (PriceListVersionId, ProductId, PriceList, StandardPrice, LimitPrice);
    
   double iStandardPrice, iPriceList, iTotalPrice, iDiscount, iLimitPrice;

   iStandardPrice= atof (StandardPrice);
   iPriceList = atof (PriceList);
    
   double iUnitPrice = atof (TSIT_ncUnitPrice);
  


   if (iStandardPrice == 0)
   {
       iStandardPrice = iUnitPrice;
       iPriceList = iUnitPrice;
       iLimitPrice = iUnitPrice;
       sprintf (PriceList, "%2.3f", iUnitPrice);
       sprintf (StandardPrice, "%2.3f", iUnitPrice);
       sprintf (LimitPrice, "%2.3f", iUnitPrice);

       sprintf (Statement, "UPDATE M_PRODUCTPRICE SET PRICELIST = %s, PRICESTD = %s, PRICELIMIT = %s where M_PRICELIST_VERSION_ID = %s AND M_PRODUCT_ID = %s", 
                PriceList, StandardPrice, LimitPrice, PriceListVersionId, ProductId);

       MXSetValue (pmessage1, "Commit", 1, &Commit);
       MXSetValue (pmessage1, "SqlStatement", 1, Statement);
       MXSend (compiere_com->MX, compiere_com, pmessage1);
       MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
   }
  
   if ((iUnitPrice != iStandardPrice) && (strcmp (Name, "5638046") != 0))
   {
       iStandardPrice = iUnitPrice;
       sprintf (StandardPrice, "%2.3f", iUnitPrice);
   }
   iTotalPrice = iStandardPrice * iTSIT_nsQty1;

   iGrandTotal += iTotalPrice;
   iDiscount =  0;
   
   if (iPriceList != 0)
       iDiscount  = (iPriceList - iStandardPrice)* 100/ iPriceList;
   
   sprintf (Discount, "%2.0f", iDiscount);
   sprintf (TotalPrice, "%2.2f", iTotalPrice);

  

   iTotalTax = 0;
   strcpy (ImportPurchase, TATrs_sTransDescEng);

   if ((strcmp (TATrs_sTransDescEng, "Cash Purchase") == 0) ||
       (strcmp (TATrs_sTransDescEng, "Credit Purchase") == 0))
   {
       strcpy (Tax, TVAId);
       iTotalTax = (iTotalPrice * 10)/100;
   }
   else strcpy (Tax, TaxId);
   
   char TotalTax[20];
   sprintf (TotalTax, "%2.2f", iTotalTax);

   char TotalWithTax[20];
   sprintf (TotalWithTax, "%2.2f", iTotalPrice + iTotalTax);

   OrderLineId[0] = 0;
   SelectOrderLineId (OrderId, ProductId, OrderLineId);


    sprintf (Statement, "INSERT INTO C_INVOICELINE  (C_INVOICELINE_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, C_ORDERLINE_ID, \
                                                   C_INVOICE_ID, LINE, DESCRIPTION, M_PRODUCT_ID, \
                                                   QTYINVOICED, PRICELIST, PRICEACTUAL, PRICELIMIT, LINENETAMT, LINETOTALAMT, \
                                                   C_UOM_ID, C_TAX_ID, TAXAMT, PROCESSED, QTYENTERED, PRICEENTERED)\
                         VALUES (%d, %s, %s, %s, %s, %s, \
                         %d,  %d, '%s', %s,\
                         %s,  %s, %s, %s, %s, %s, \
                         %s, %s, %s, '%s', %s, %s)",
                         iInvoiceLineSeq, ClientId, OrgId, UserId, UserId, OrderLineId, 
                         iInvoiceSeq - 1, Line,  "", ProductId, 
                         TSIT_nsQty1,  PriceList, TSIT_ncUnitPrice, LimitPrice, TotalPrice, TotalWithTax, 
                         UOMId, Tax, TotalTax, "N", TSIT_nsQty1, TSIT_ncUnitPrice);
    Line += 10;
    iInvoiceLineSeq++;
    MXSetValue (pmessage1, "Commit", 1, &Commit);
    MXSetValue (pmessage1, "SqlStatement", 1, Statement);
    MXSend (compiere_com->MX, compiere_com, pmessage1);
    MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));



    MXFreeMessage (compiere_com->MX, pmessage1);
    j++; printf ("<--- %d\n", j);
  return 1;

}   

void SelectInvoiceRows (MX* pmx)
{
    MXMessage* pmessage;
    int every = 1;
    int max = Count;
    int i = 0;
    BYTE Commit = 1;
 
         
    MXAddComCallBack (pmx, omega_com, DB_SYS,  "SelectInvoice", MXONRECV, SelectInvoice, compiere_com); 


    pmessage = MXCreateMessage (pmx, DB_SYS, "ExecuteQuery");
    MXSetValue (pmessage ,"Every", 1, &every);
    MXSetValue (pmessage ,"MaxRows", 1, &max);
   
    MXSetValue (pmessage ,"Name",    1,    "SelectInvoice"  );
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
    char SPriceList[200];
    char SPriceListVersion[200];
    
   if (argc > 2)
   {
       strcpy (Parameter, argv[1]);
       Year = atoi ( argv[2]);
   }
   else
       return;


   
   sprintf (Statement,  "SELECT StkItemsTransactions.TATrs_nlTransactionId, StkItemsTransactions.Name, AccTransactionHeader.TATrs_ncTotInv, AccTransactionHeader.TATrs_nlVoucherNbr, vew_TransactionRules.TATrs_sTransDescEng, StkItemsTransactions.TSIT_nsQty1, StkItemsTransactions.TSIT_nsQty2, AccTransactionHeader.TATrs_dOperationDate, AccTransactionHeader.ToName, AccTransactionHeader.TATrs_sTransReference, StkItems.Devise, StkItems.Location,  StkItemsTransactions.TSIT_ncUnitPrice, StkItemsTransactions.TSIT_ncUnitPriceLL, StkItemsTransactions.TSIT_nsDiscount, StkItemsTransactions.TSIT_ncTotalPrice, vew_AccTiersCategory.TAccT_sDesc \
            FROM ((((StkItemsTransactions INNER JOIN AccTransactionHeader ON StkItemsTransactions.TATrs_nlTransactionId = AccTransactionHeader.TATrs_nlTransactionId) INNER JOIN vew_TransactionRules ON AccTransactionHeader.TATrs_nlTransactionType = vew_TransactionRules.TATrs_nlTransactionType) INNER JOIN StkItems ON StkItemsTransactions.Name = StkItems.Name) INNER JOIN AccTransactionDetails ON AccTransactionHeader.TATrs_nlTransactionId = AccTransactionDetails.TATrs_nlTransactionId) INNER JOIN vew_AccTiersCategory ON AccTransactionDetails.TAccT_nlTiersId = vew_AccTiersCategory.TAccT_nlTiersId \
            WHERE (((AccTransactionHeader.TATrs_sTransReference) = 'U00000016')AND((vew_TransactionRules.TATrs_sTransDescEng) Like '%s') AND ((AccTransactionHeader.TATrs_dOperationDate) %s) AND ((AccTransactionDetails.TATDt_sDesc ='Purchase') OR (AccTransactionDetails.TATDt_sDesc Like 'Cash*'))) \
            ORDER BY StkItemsTransactions.TATrs_nlTransactionId, StkItemsTransactions.Name, AccTransactionHeader.TATrs_dOperationDate",  "%Purch%", ((Year == 2004) ? "<= #12/31/2004#" : "> #12/31/2004#"));

    MXInit (&mx, MXCLIENT, "base", "base", 0, NULL);

   
    omega_com = MXOpenBase (&mx, "omega", "","", DBPROTO_ODBC, TRUE);
    if (!omega_com) return;
    compiere_com = MXOpenBase (&mx, "compiere", "scott","tiger", DBPROTO_ODBC, TRUE);
    if (!compiere_com) return;

    MXAddComCallBack (&mx, omega_com,    DB_SYS, "AckStatement",   MXONRECV,   AckExcuteQuery, compiere_com); 

    
    SelectId ("Techno Cars",     ClientId,           CLIENT_ID);
    SelectId ("Techno Cars",     OrgId,              ORG_ID);
    SelectId ("SuperUser",       UserId,             USER_ID);
    SelectId ("Standard",        WarehouseId,        WAREHOUSE_ID);
    SelectId ("AP Invoice",      DocumentId,         DOCUMENT_ID);
    
    sprintf (SPriceList, "%s", "Purchase Prices for 2004 in EUR");
    sprintf (SPriceListVersion, "%s", "Purchase EUR 2004");
    SelectId (SPriceList,        EUR2004PriceListId,        PRICELIST_ID);
    SelectId (SPriceListVersion, EUR2004PriceListVersionId, PRICELIST_VERSION_ID);
    
    sprintf (SPriceList, "%s", "Purchase Prices for 2005 in EUR");
    sprintf (SPriceListVersion, "%s", "Purchase EUR 2005");
    SelectId (SPriceList,        EUR2005PriceListId,        PRICELIST_ID);
    SelectId (SPriceListVersion, EUR2005PriceListVersionId, PRICELIST_VERSION_ID);

    sprintf (SPriceList, "%s", "Purchase VOR Prices for 2004 in EUR");
    sprintf (SPriceListVersion, "%s", "Purchase VOR EUR 2004");
    SelectId (SPriceList,        VOREUR2004PriceListId,        PRICELIST_ID);
    SelectId (SPriceListVersion, VOREUR2004PriceListVersionId, PRICELIST_VERSION_ID);
    
    sprintf (SPriceList, "%s", "Purchase VOR Prices for 2005 in EUR");
    sprintf (SPriceListVersion, "%s", "Purchase VOR EUR 2005");
    SelectId (SPriceList,        VOREUR2005PriceListId,        PRICELIST_ID);
    SelectId (SPriceListVersion, VOREUR2005PriceListVersionId, PRICELIST_VERSION_ID);
    
    sprintf (SPriceList, "Purchase Prices for %d in USD", Year);
    sprintf (SPriceListVersion, "Purchase USD %d", Year);
    SelectId (SPriceList,        USDPriceListId,        PRICELIST_ID);
    SelectId (SPriceListVersion, USDPriceListVersionId, PRICELIST_VERSION_ID);
    
    sprintf (SPriceList, "Purchase Prices for %d in LBP", Year);
    sprintf (SPriceListVersion, "Purchase LBP %d", Year);
    SelectId (SPriceList,        LBPPriceListId,        PRICELIST_ID);
    SelectId (SPriceListVersion, LBPPriceListVersionId, PRICELIST_VERSION_ID);

    SelectId ("Immediate",       ImmediatePaymentTermId,      PAYMENTTERM_ID);
    SelectId ("Credit",          CreditPaymentTermId,         PAYMENTTERM_ID);
    SelectId ("EUR",             EURCurrencyId,         CURRENCY_ID);
    SelectId ("USD",             USDCurrencyId,         CURRENCY_ID);
    SelectId ("LBP",             LBPCurrencyId,         CURRENCY_ID);

    SelectId ("Spot",            ConversionTypeId,   CONVERSIONTYPE_ID);
    SelectId ("Tony Jureidini",  SalesRepId,         USER_ID);
    SelectId ("Standard",        TaxId,              TAX_ID);
    SelectId ("TVA",             TVAId,              TAX_ID);
    SelectId ("Freight",         ChargeId,           CHARGE_ID);



    SelectSeq ("M_Transaction", TransactionSeq);
    iTransactionSeq = atol (TransactionSeq);

    SelectSeq ("C_Invoice", InvoiceSeq);
    iInvoiceSeq = atol (InvoiceSeq);

    SelectSeq ("C_InvoiceLine", InvoiceLineSeq);
    iInvoiceLineSeq = atol (InvoiceLineSeq);


    MXAddComCallBack (&mx, compiere_com, DB_SYS, "AckStatement",   MXONRECV,   AckExcuteUpdate, omega_com); 
 
    SelectInvoiceRows (&mx);
    MXDispatchEvents (&mx, 0);

    MXEnd (&mx);

}