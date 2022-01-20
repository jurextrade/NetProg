
#include "sc.h"


char OrgId[10];
char ClientId[10];
char UserId[10];

char BPartnerSeq[20];
int iBPartnerSeq;

int AfterUpdate = 0;
MXCom* omega_com; 
MXCom* compiere_com; 

char Table[100];
char OrderType[20];
int Request = 0;


#define USER_ID         0
#define CLIENT_ID       1
#define ORG_ID          2
#define BPGROUP_ID      3

int QueryFinish = 0;
int j = 0;
int Count = 15000;
int Year = 2005;

typedef struct _Greetings
{
    char GreetingId[20];
    char Greeting[20];
    char Name[20];
}Greetings;

Greetings GreetingTab[30];
int GreetingNbr = 0;

char BPartnerGroupId[20];

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
    case BPGROUP_ID :
        sprintf (Statement, "%s'%s'%s%s;", "select C_BP_GROUP_ID from C_BP_GROUP where NAME = ", Name, " and AD_CLIENT_ID = ", ClientId);
       Table_id = "C_BP_GROUP_ID";
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
    char Statement[400];
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
        if (!AfterUpdate)
        {
            sprintf (Statement, "%s%d%s" , "update AD_SEQUENCE set CURRENTNEXT = ",  iBPartnerSeq, " WHERE NAME = 'I_BPartner'");
            pmessage1 =  MXPutMessage (compiere_com, DB_SYS, "ExecuteUpdate");
            MXSetValue (pmessage1, "SqlStatement", 1, Statement );
            MXSetValue (pmessage1, "Commit", 1, &Commit);
            MXSend (compiere_com->MX, compiere_com, pmessage1);
            MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
            MXFreeMessage (compiere_com->MX, pmessage1);
        }
        MXCloseCom (compiere_com->MX, compiere_com);
        MXCloseCom (compiere_com->MX, omega_com);
   }
   return 1;
}


int SelectBPartner (MXMessage* pmessage, MXCom* pcom, void* appfield)
{  
    MXMessage* pmessage1;
    BYTE Commit = '1';
    char Statement[1400];
    int ToSend = 1;
    char Qname[100];
    char Qaddress[200];
    char QToName[100];
    char Qaddress1[200];
    STRING name    = "";
    STRING phones  =  "";  
    STRING fax     = "";   
    STRING emails  = "";   
    STRING mobiles = "";   
    STRING address =  "";  
    STRING city    =  "UNDEFINED"; 
    STRING country =  "LB"; 
    STRING rank    =  ""; 

    Qaddress1[0] = 0;
    Qaddress[0] = 0;
    Qname[0]= 0;
    QToName[0]= 0;

    pmessage1 =  MXCreateMessage (compiere_com->MX, DB_SYS, "ExecuteUpdate");
    MXSetValue (pmessage1, "Commit", 1, &Commit);

    STRING ToName  =   strend ((STRING) MXGetValue (pmessage, "ToName"            , 1), " ");
    STRING Greeting=   strend ((STRING) MXGetValue (pmessage, "Greeting"            , 1), " ");
   
    if (strcmp (Table, "Customers") == 0)
    {
        name    =   strend ((STRING) MXGetValue (pmessage, "name"            , 1), " ");

        phones  =   strend ((STRING) MXGetValue (pmessage, "phones"            , 1), " ");
        fax     =   strend ((STRING) MXGetValue (pmessage, "fax"            , 1), " ");
        emails  =   strend ((STRING) MXGetValue (pmessage, "emails"            , 1), " ");
        mobiles =   strend ((STRING) MXGetValue (pmessage, "mobiles"            , 1), " ");
        address =   strend ((STRING) MXGetValue (pmessage, "address"            , 1), " ");
        city    =   strend ((STRING) MXGetValue (pmessage, "city"            , 1), " ");
        country =   strend ((STRING) MXGetValue (pmessage, "country"            , 1), " ");
        rank    =   strend ((STRING) MXGetValue (pmessage, "rank"            , 1), " ");
        if (city[0] == 0)  city = "UNDEFINED";

   
        strTakeOffQuote (Qaddress, address);
    

 
        if (strlen (Qaddress) > 60)
        {
         
          for (int i = strlen (Qaddress) - 1; i >= 0; i--)
          if (Qaddress[i] == ',')
            break;
          Qaddress[i] = 0;
          strcpy (Qaddress1, Qaddress + i + 1);
        }   
    }
    else
    {
         name    =   strend ((STRING) MXGetValue (pmessage, "Customer"            , 1), " ");
    }
    
    
    strTakeOffQuote (Qname, name);
    strTakeOffQuote (QToName, ToName);
    char* SGreeting = NULL;

    for (int i = 0; i < GreetingNbr; i++)
    {
        if (strcmp (GreetingTab[i].Name, Greeting) == 0)
        {
            SGreeting = GreetingTab[i].GreetingId;
            break;
        }
    }

    if (SGreeting)
    {
        if (AfterUpdate)
        {
            if (strcmp (Table, "Customers") == 0)
                sprintf (Statement, "UPDATE C_BPARTNER SET C_GREETING_ID = %s , RATING = '%s' where VALUE = '%s'", SGreeting, rank, QToName);
            else
                sprintf (Statement, "UPDATE C_BPARTNER SET C_GREETING_ID = %s where VALUE = '%s'", SGreeting, QToName);
        }
        else
        {
                sprintf (Statement, "INSERT INTO I_BPARTNER(I_BPARTNER_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, \
                          VALUE, NAME, DESCRIPTION, C_BP_GROUP_ID, ADDRESS1, ADDRESS2, CITY, COUNTRYCODE, PHONE, PHONE2, FAX, EMAIL, C_GREETING_ID)\
                          VALUES (%d, %s, %s, %s, %s, '%s',  '%s', '%s', %s, '%s','%s', '%s', '%s', '%s', '%s', '%s', '%s', %s)",  
                          iBPartnerSeq, ClientId, OrgId, UserId, UserId, QToName, QToName, Qname, BPartnerGroupId, Qaddress, Qaddress1, city, country, 
                          phones, mobiles, fax, emails, SGreeting);
        }
     }

     else
     {
        if (AfterUpdate)
        {
            if (strcmp (Table, "Customers") == 0)
                sprintf (Statement, "UPDATE C_BPARTNER SET RATING = '%s' where VALUE = '%s'", rank, QToName);
            else ToSend = 0;
        }
        else
        {
                sprintf (Statement, "INSERT INTO I_BPARTNER(I_BPARTNER_ID, AD_CLIENT_ID, AD_ORG_ID, CREATEDBY, UPDATEDBY, \
                          VALUE, NAME, DESCRIPTION, C_BP_GROUP_ID, ADDRESS1, ADDRESS2, CITY, COUNTRYCODE, PHONE, PHONE2, FAX, EMAIL)\
                          VALUES (%d, %s, %s, %s, %s, '%s',  '%s', '%s', %s, '%s','%s','%s', '%s', '%s', '%s', '%s', '%s')",  
                          iBPartnerSeq, ClientId, OrgId, UserId, UserId, QToName, QToName, Qname, BPartnerGroupId, Qaddress, Qaddress1,  city, country, 
                          phones, mobiles, fax, emails);

        }
     }
     if (ToSend)
     {
        MXSetValue (pmessage1, "SqlStatement", 1, Statement);
        MXSend (compiere_com->MX, compiere_com, pmessage1);
        MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));
     }
     MXFreeMessage (compiere_com->MX, pmessage1);
     iBPartnerSeq++;
     j++; printf ("<--- %d\n", j);
     return 1;
}   

void SelectBPartnerRows (MX* pmx)
{
    MXMessage* pmessage;
    int every = 1;
    int max = Count;
    int i = 0;
    BYTE Commit = 1;
    char Statement[400];



         
    MXAddComCallBack (pmx, omega_com, DB_SYS,  "SelectBPartner", MXONRECV, SelectBPartner, compiere_com); 


    pmessage = MXCreateMessage (pmx, DB_SYS, "ExecuteQuery");

    if (strcmp (Table, "Customers") == 0)
       MXSetValue (pmessage ,"SqlStatement", 1, "SELECT * from Customers");
    else
    {
       sprintf (Statement, "SELECT Greeting, ToName, customer from %s where TATrs_sTransReference like '%s%s'", Table, OrderType, "%");
       MXSetValue (pmessage ,"SqlStatement", 1, Statement);
    }


    MXSetValue (pmessage ,"Every", 1, &every);
    MXSetValue (pmessage ,"MaxRows", 1, &max);
   
    MXSetValue (pmessage ,"Name",    1,    "SelectBPartner"  );
    MXSend (pmx, omega_com, pmessage);


    while (!QueryFinish)
    {
        MXFreeMessage (omega_com->MX, MXRecv (omega_com->MX, omega_com));
    }
    MXFreeMessage (pmx, pmessage);
}


void SelectGreetings (MX* pmx)
{
    MXMessage* pmessage;
    MXMessage* pmessage1;
    int every = 1;
    int max = Count;
    int i = 0;
    BYTE Commit = 1;
    char Statement[400];



    sprintf (Statement, "SELECT * from C_GREETING WHERE AD_CLIENT_ID = %s", ClientId);
    pmessage = MXCreateMessage (pmx, DB_SYS, "ExecuteQuery");
    MXSetValue (pmessage ,"Every", 1, &every);
    MXSetValue (pmessage ,"MaxRows", 1, &max);
    MXSetValue (pmessage ,"Name",    1,    "SelectGreetings"  );
    MXSetValue (pmessage ,"SqlStatement", 1, Statement);

    MXSend (pmx, compiere_com, pmessage);
    MXFreeMessage (compiere_com->MX, MXRecv (compiere_com->MX, compiere_com));  // header

    while ((pmessage1 = MXRecv (compiere_com->MX, compiere_com)) && (strcmp (pmessage1->MessageClass->Name, "AckStatement") != 0))
    {
        strcpy (GreetingTab[GreetingNbr].GreetingId, strend ((STRING)MXGetValue (pmessage1, "C_GREETING_ID", 1), " "));
        strcpy (GreetingTab[GreetingNbr].Name,        strend ((STRING)MXGetValue (pmessage1, "NAME", 1), " "));
        GreetingNbr++;
        MXFreeMessage (pmx, pmessage1);
    }
    MXFreeMessage (pmx, pmessage);
    MXFreeMessage (pmx, pmessage1);
}



void main(int argc, char* argv[])
{
    MX mx;

   if (argc > 1)
    {
        strcpy (Table , argv[1]);
        if (argc > 2)
        {
           if (strcmp (Table, "Customers") == 0)
             AfterUpdate = 1;
            else
            {
                strcpy (OrderType, argv[2]);
                if (argc > 3) AfterUpdate = 1;
            }
        }

    }
    else
        return;



    MXInit (&mx, MXCLIENT, "base", "base", 0, NULL);

   
    omega_com = MXOpenBase (&mx, "omega", "","", DBPROTO_ODBC, TRUE);
    if (!omega_com) return;
    compiere_com = MXOpenBase (&mx, "compiere", "scott","tiger", DBPROTO_ODBC, TRUE);
    if (!compiere_com) return;

    MXAddComCallBack (&mx, omega_com,    DB_SYS, "AckStatement",   MXONRECV,   AckExcuteQuery, compiere_com); 
    SelectId ("Techno Cars", ClientId, CLIENT_ID);
    if (!AfterUpdate)
    {
        SelectId ("Techno Cars", OrgId, ORG_ID);
        SelectId ("SuperUser", UserId, USER_ID);
        SelectId ("Customer", BPartnerGroupId, BPGROUP_ID);
        SelectSeq ("I_BPartner", BPartnerSeq);

        iBPartnerSeq = atol (BPartnerSeq);
    }


    MXAddComCallBack (&mx, compiere_com, DB_SYS, "AckStatement",   MXONRECV,   AckExcuteUpdate, omega_com); 
    
    SelectGreetings (&mx);
    SelectBPartnerRows (&mx);
 

    MXDispatchEvents (&mx, 0);

    MXEnd (&mx);

}