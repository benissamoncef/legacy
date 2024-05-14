/*E*/
/*Fichier :  $Id: ewst_cmd.c,v 1.6 2020/10/02 14:47:53 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2020/10/02 14:47:53 $ ------------------------------------------------------
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TESWT * FICHIER ewst_cmd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* C'est le module directeur de la tache EWST
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	    11/04/22    creation DEM-SAE375
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>
#include "xzsm.h"
#include "xzsc.h"
#include "ewst_cmd.h"
#include "xzamc.h"

/* definitions de constantes */
#define	LIBELLE_EQUIPEMENT	"numero-eqt" 
#define	LIBELLE_MACHINE_LOCALE	"machine-locale"
#define LIBELLE_LOGIN "login-"
#define LIBELLE_PASSWORD "password-"
/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variable externe */
extern XDY_NomMachine	vg_NomMachine;
const char inifile[] = "/produits/migrazur/appliSD/fichiers/deq/tewst.ini";

/* declaration de variables locales */

void cb_tewst_appel_tel(T_IPC_CONN va_Cnx,
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
* Initialisation : callback provenant de l'ihm
* 
------------------------------------------------------*/
{
    XDY_Entier va_codecmd;
    XDY_Tel va_NumTel;
    XDY_Mot va_NumPO;
    XDY_NomSite va_nomSite;
    XDY_Texte vl_IniFileData;
    XDY_Texte vl_login;
    XDY_Texte vl_pswd;
    XDY_Texte vl_param;

    TipcMsgRead(pa_Data->msg, 
    T_IPC_FT_INT4, &va_codecmd, 
    T_IPC_FT_STR,  &va_NumTel, 
    T_IPC_FT_INT2, &va_NumPO, 
    T_IPC_FT_STR, &va_nomSite,
    NULL) ; 
    //XZST_03EcritureTrace(XZSTC_INFO, "cb_tewst_appel NomSite:%s",va_codecmd,va_NumTel,va_NumPO,va_nomSite);
    XZST_03EcritureTrace(XZSTC_INFO, "cb_tewst_appel codecmd:%d ",va_codecmd);
    XZST_03EcritureTrace(XZSTC_INFO, "cb_tewst_appel numTel:%s",va_NumTel);
    XZST_03EcritureTrace(XZSTC_INFO, "cb_tewst_appel NumPo:%d",va_NumPO);
    XZST_03EcritureTrace(XZSTC_INFO, "cb_tewst_appel NomSite:%s",va_nomSite);

    /* recupere le login qui correspond au poste */
    sprintf(vl_param,"%s%d",LIBELLE_LOGIN,va_NumPO);
     XZST_03EcritureTrace(XZSTC_WARNING, "param:%s",vl_param);
    if(tewst_lecture_ini(vl_param, vl_IniFileData)!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING, "cb_tewst_appel pb lecture fichier ini, fin du traitement !");
        return;
    }
    strcpy(vl_login,vl_IniFileData);

    /* recupere le psw qui correspond au poste */
    sprintf(vl_param,"%s%d",LIBELLE_PASSWORD,va_NumPO);
     XZST_03EcritureTrace(XZSTC_WARNING, "param:%s",vl_param);
    if(tewst_lecture_ini(vl_param, vl_IniFileData)!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING, "cb_tewst_appel pb lecture fichier ini, fin du traitement !");
        return;
    }
    strcpy(vl_pswd,vl_IniFileData);

    XZST_03EcritureTrace(XZSTC_INFO, "cb_tewst_appel login:%s password:%s",vl_login,vl_pswd);

    TipcSrvMsgWrite ( XDG_API_GW_TEL
                    ,XDM_IdentMsg(XDM_GTW_TEL_APPEL)
                    ,XDM_FLG_SRVMSGWRITE
                    ,T_IPC_FT_STR, va_NumTel
                    ,T_IPC_FT_STR, vl_login
                    ,T_IPC_FT_STR, vl_pswd
                    ,NULL);    

}

void cb_tewst_retour_appel(T_IPC_CONN va_Cnx,
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
* Retour de la gw sur l'appel
* Declenchement alarme eqt
* 
------------------------------------------------------*/
{
    T_INT4 va_retour_appel;
	double vl_horodate;
    XDY_Datagroup pl_DG_Error;
    XDY_Texte vl_IniData;
    XDY_NomMachine	vl_NomMachineLocale;
    XDY_Eqt	vl_NumEquipement;

    TipcMsgRead(pa_Data->msg,
                T_IPC_FT_INT4, &va_retour_appel,
                NULL);

    if(tewst_lecture_ini(LIBELLE_EQUIPEMENT,vl_IniData)!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING, "cb_tewst_appel pb lecture fichier ini LIBELLE_EQUIPEMENT !");
        return;
    }    
    vl_NumEquipement=atoi(vl_IniData);

    if(tewst_lecture_ini(LIBELLE_MACHINE_LOCALE,vl_IniData)!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING, "cb_tewst_appel pb lecture fichier ini LIBELLE_MACHINE_LOCALE !");
        return;
    }
    strcpy(vl_NomMachineLocale,vl_IniData);

    XZST_03EcritureTrace(XZSTC_WARNING, "cb_tewst_retour_appel retour appel = %d, eqt:%d, NomMachine:%s",va_retour_appel,vl_NumEquipement,vl_NomMachineLocale);

    switch (va_retour_appel)
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
                XZST_03EcritureTrace(XZSTC_WARNING, "cb_tewst_retour_appel RAZ Alarme(TEL) non envoyee");
            }
            else
            {
                XZST_03EcritureTrace(XZSTC_INFO, "cb_tewst_retour_appel RAZ Alarme(TEL) envoyee");
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
                    XZST_03EcritureTrace(XZSTC_WARNING, "cb_tewst_retour_appel Alarme(TEL) envoyee non envoyee");
                }
                else
                {
                    XZST_03EcritureTrace(XZSTC_INFO, "cb_tewst_retour_appel Alarme(TEL) envoyee");
                }
            }
        }
        break;
    }
}

int tewst_lecture_ini(XDY_Texte va_param, XDY_Texte va_IniFileData)
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

    XZST_03EcritureTrace(  XZSTC_FONCTION,"tewst_lecture_ini : IN");
    XZST_03EcritureTrace(  XZSTC_INFO,"tewst_lecture_ini param=%s",va_param);
    
    strcpy(va_IniFileData,""); //RAZ
	
    /* Lecture du fichier des destinataires */
	if ( (pl_fp = fopen(inifile, "r")) == NULL ) 
    {
		XZST_03EcritureTrace(   XZSTC_WARNING, "tewst_lecture_ini : erreur de lecture du fichier %s !", inifile );
		return(XDC_NOK);
	}

	/* On lit les lignes du fichier une par une */
	/* ------------------------------------------------------ */
	while ( fgets(pl_ligne, 200, pl_fp) != NULL) 
	{
		/* On ignore les lignes commencant par un # */
		if ( pl_ligne[0] == '#' )
			continue;

		/* On cherche le pattern de vl_param et on prend la value apres le = */
		if (strstr(pl_ligne,va_param))
		{
            sscanf(pl_ligne,"%[^=]=%s",vl_str,va_IniFileData);
		}
	}
	fclose(pl_fp);
    
    XZST_03EcritureTrace(  XZSTC_INFO,"tewst_lecture_ini IniFileData=%s",va_IniFileData);
    XZST_03EcritureTrace(  XZSTC_WARNING,"tewst_lecture_ini : OUT");	

	return (XDC_OK);
}