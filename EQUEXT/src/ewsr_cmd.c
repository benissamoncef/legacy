/*E*/
/*Fichier :  $Id: ewsr_cmd.c,v 1.6 2020/10/02 14:47:53 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2020/10/02 14:47:53 $ ------------------------------------------------------
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TESWT * FICHIER ewsr_cmd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* C'est le module directeur de la tache EWSR
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	    17/05/22    creation DEM-SAE380
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>
#include "xzsm.h"
#include "xzsc.h"
#include "ewsr_cmd.h"
#include "xzamc.h"

/* definitions de constantes */
#define	LIBELLE_EQUIPEMENT	"numero-eqt" 
#define	LIBELLE_MACHINE_LOCALE	"machine-locale"
#define LIBELLE_IP "my-ip"
#define LIBELLE_PORT "my-port"
#define LIBELLE_URI "my-service-uri"

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variable externe */
extern XDY_NomMachine	vg_NomMachine;


/* declaration de variable interne */
const char inifile[] = "/produits/migrazur/appliSD/fichiers/deq/tewsr.ini";
//valeurs par defaut
char myIP[16] = "10.16.12.154";
char myPort[6] = "9090";
char myServiceUri[100] = "/axis2/services/echoSAE";

/* declaration de variables locales */

void cb_tewsr_appel_radio(T_IPC_CONN va_Cnx,
		T_IPC_CONN_PROCESS_CB_DATA pa_Data,
		T_CB_ARG	pa_Arg)

/*
* ARGUMENTS EN ENTREE :
*   
*
* ARGUMENTS EN SORTIE :
*   Aucune
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*  
* Initialisations : callback provenant de l'ihm
* Transfert l'appel radio de l'ihm vers la passerelle SOAP
* 
------------------------------------------------------*/
{
    XDY_Texte va_nomSite;
    XDY_TypeAppelRadio va_typeAppel;
    XDY_Texte va_numeroGroupeRadio;
    XDY_Texte va_numeroCibleAppel;
    XDY_Entier va_NumPO;

    XDY_Texte vl_param;
    XDY_Texte vl_IniFileData;
    XDY_Texte str_typeAppel;

XDY_Texte tmp;

	if (TipcMsgRead(pa_Data->msg,
			 T_IPC_FT_INT4,	&va_typeAppel,
			 T_IPC_FT_STR,	&va_numeroGroupeRadio,
			 T_IPC_FT_STR,	&va_numeroCibleAppel,
			 T_IPC_FT_INT2,	&va_NumPO,
			 T_IPC_FT_STR,	&va_nomSite,
			NULL) != TRUE) {
		XZST_03EcritureTrace(XZSTC_WARNING, "cb_tewsr_appel_radio : erreur dans TipcMsgRead");
		return;
	}


/*sprintf(tmp, "type:%d, numerogrp:%s, numerocible:%s, numPO:%d, nomSite:%s",va_typeAppel,va_numeroGroupeRadio,va_numeroCibleAppel,va_NumPO,va_nomSite);
XZST_03EcritureTrace(XZSTC_WARNING, "cb_tewsr_appel_radio message appel : %s",tmp);*/
   XZST_03EcritureTrace(XZSTC_WARNING, "cb_tewsr_appel_radio message appel : type=%d",va_typeAppel);
    XZST_03EcritureTrace(XZSTC_WARNING, "cb_tewsr_appel_radio message appel :  numeroGroupeRadio:%s",va_numeroGroupeRadio);
    XZST_03EcritureTrace(XZSTC_WARNING, "cb_tewsr_appel_radio message appel : NumPO:%d",va_NumPO);
    XZST_03EcritureTrace(XZSTC_WARNING, "cb_tewsr_appel_radio message appel : nomSite:%s",va_nomSite);
    if (va_numeroCibleAppel==NULL)XZST_03EcritureTrace(XZSTC_WARNING, "c nul");
    XZST_03EcritureTrace(XZSTC_WARNING, "%d",strlen(va_numeroCibleAppel));
    XZST_03EcritureTrace(XZSTC_WARNING, "cb_tewsr_appel_radio message appel : numeroCibleAppel:%s",va_numeroCibleAppel);

    switch (va_typeAppel)
	{
		case 3:
			strcpy(str_typeAppel,"GRP");
			break;
		case 2:
			strcpy(str_typeAppel, "IND");
			break;
		case 1:
			strcpy(str_typeAppel, "AIG");
			break;
		default:
			strcpy(str_typeAppel, "AIG");
			break;
	}

    /* recupere les infos concernant l'appel dans le fichier ini*/
    tewsr_lecture_ini(LIBELLE_IP, myIP);
    tewsr_lecture_ini(LIBELLE_PORT, myPort);
    tewsr_lecture_ini(LIBELLE_URI, myServiceUri);
    
    TipcSrvMsgWrite (  XDG_API_GW_RAD,
                    XDM_IdentMsg(XDM_GTW_RAD_APPEL),
                    XDM_FLG_SRVMSGWRITE,
                    T_IPC_FT_STR,	str_typeAppel,
                    T_IPC_FT_STR,	va_numeroGroupeRadio,
                    T_IPC_FT_STR,	va_numeroCibleAppel,
                    T_IPC_FT_INT2,	va_NumPO,
                    T_IPC_FT_STR,	va_nomSite,
                    T_IPC_FT_STR,   myIP,
                    T_IPC_FT_STR,   myPort,
                    T_IPC_FT_STR,   myServiceUri,
                    NULL);
}


void cb_tewsr_retour_appel(T_IPC_CONN va_Cnx,
		T_IPC_CONN_PROCESS_CB_DATA pa_Data,
		T_CB_ARG	pa_Arg)
/*
* ARGUMENTS EN ENTREE :
*   
*
* ARGUMENTS EN SORTIE :
*   Aucune
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*  
* Retour de la gw sur l'appel radio
* 
------------------------------------------------------*/
{
    T_INT4 vl_retour_appel;
	double vl_horodate;
    XDY_Datagroup pl_DG_Error;
    XDY_Texte vl_IniData;
    XDY_NomMachine	vl_NomMachineLocale;
    XDY_Eqt	vl_NumEquipement;


    TipcMsgRead(pa_Data->msg,
                T_IPC_FT_INT4, &vl_retour_appel,
                NULL);


    if(tewsr_lecture_ini(LIBELLE_EQUIPEMENT,vl_IniData)!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING, "cb_tewst_appel pb lecture fichier ini LIBELLE_EQUIPEMENT !");
        return;
    }    
    vl_NumEquipement=atoi(vl_IniData);

    if(tewsr_lecture_ini(LIBELLE_MACHINE_LOCALE,vl_IniData)!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING, "cb_tewst_appel pb lecture fichier ini LIBELLE_MACHINE_LOCALE !");
        return;
    }
    strcpy(vl_NomMachineLocale,vl_IniData);

    XZST_03EcritureTrace(XZSTC_WARNING, "cb_tewst_retour_appel retour appel = %d, eqt:%d, NomMachine:%s, etat alarme = %d",vl_retour_appel,vl_NumEquipement,vl_NomMachineLocale,vg_etat_alarme);

    switch (vl_retour_appel)
    {
        case XDC_OK:
        {
            /* Maj etat alarme */
            vg_etat_alarme = XDC_FAUX;

            /* on coupe l'alarme */
            XDG_EncodeDG2(pl_DG_Error,XDG_AM,XDC_NOM_SITE_CI);
            vl_horodate=TutGetWallTime();
            if (!TipcSrvMsgWrite ( pl_DG_Error,
                    XDM_IdentMsg(XDM_AM_alarme), XDM_FLG_SRVMSGWRITE,
                    T_IPC_FT_REAL8, vl_horodate,
                    T_IPC_FT_CHAR, XDC_EQT_MAA,
                    T_IPC_FT_INT2, vl_NumEquipement,
                    T_IPC_FT_INT2, XZAMC_DEF_TRANS,
                    T_IPC_FT_CHAR, XDC_FAUX,		   /* XDC_FAUX sur comm OK*/ /*XDC_VRAI sur comm HS*/
                    T_IPC_FT_STR, vl_NomMachineLocale,
                    NULL))
            {
                XZST_03EcritureTrace(XZSTC_WARNING, "cb_tewsr_retour_appel RAZ Alarme(RADIO) non envoyee");
            }
            else
            {
                XZST_03EcritureTrace(XZSTC_INFO, "cb_tewsr_retour_appel RAZ Alarme(RADIO) envoyee");
            }
        }
        break;
        case XDC_NOK: /* Retour appel echec */
        {
            if (vg_etat_alarme == XDC_FAUX)
            {
                /* Maj etat alarme */
                vg_etat_alarme = XDC_VRAI;
                /* on lance l'alarme */
                XDG_EncodeDG2(pl_DG_Error,XDG_AM,XDC_NOM_SITE_CI);
                vl_horodate=TutGetWallTime();
                if (!TipcSrvMsgWrite ( pl_DG_Error,
                        XDM_IdentMsg(XDM_AM_alarme), XDM_FLG_SRVMSGWRITE,
                        T_IPC_FT_REAL8, vl_horodate,
                        T_IPC_FT_CHAR, XDC_EQT_MAA,
                        T_IPC_FT_INT2, vl_NumEquipement,
                        T_IPC_FT_INT2, XZAMC_DEF_TRANS,
                        T_IPC_FT_CHAR, XDC_VRAI,		   /* XDC_FAUX sur comm OK*/ /*XDC_VRAI sur comm HS*/
                        T_IPC_FT_STR, vl_NomMachineLocale,
                        NULL))
                {
                    XZST_03EcritureTrace(XZSTC_WARNING, "cb_tewsr_retour_appel Alarme(RADIO) envoyee non envoyee");
                }
                else
                {
                    XZST_03EcritureTrace(XZSTC_INFO, "cb_tewsr_retour_appel Alarme(RADIO) envoyee");
                }
            }
            else
                XZST_03EcritureTrace(XZSTC_INFO, "cb_tewsr_retour_appel alarme deja en cours");
        }
        break;
    }
}

int tewsr_lecture_ini(XDY_Texte va_param, XDY_Texte va_IniFileData)
/*
* ARGUMENTS EN ENTREE :
*   
*
* ARGUMENTS EN SORTIE :
*   Aucune
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*  
* Lecture du fichier de config
* 
------------------------------------------------------*/
{
    FILE					*pl_fp;
    char                    pl_ligne[200];
    XDY_Texte vl_str;

    XZST_03EcritureTrace(  XZSTC_WARNING,"tewsr_lecture_ini : IN");

	/* Lecture du fichier des destinataires */
	if ( (pl_fp = fopen(inifile, "r")) == NULL ) 
    {
		XZST_03EcritureTrace(   XZSTC_WARNING, "tewsr_lecture_ini : erreur de lecture du fichier %s !", inifile );
		return(XDC_NOK);
	}

	/* On lit les lignes du fichier une par une */
	/* ------------------------------------------------------ */
	while ( fgets(pl_ligne, 200, pl_fp) != NULL) 
	{
		/* On ignore les lignes commencant par un # */
		if ( pl_ligne[0] == '#' )
			continue;

		/* On cherche les login et psw qui corresponde au numPO de la forme : Login-numPO=SAE_Pposte_Num */
		if (strstr(pl_ligne,va_param))
		{
            sscanf(pl_ligne,"%[^=]=%s",vl_str,va_IniFileData);
		}
	}
	fclose(pl_fp);

    XZST_03EcritureTrace(  XZSTC_WARNING,"tewsr_lecture_ini : OUT");	

	return (XDC_OK);
}