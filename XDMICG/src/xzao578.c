/*E*/
/* Fichier : $Id: xzao578.c,v 1.1 2012/07/04 10:46:41 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:46:41 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzao578.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* XZAO578_Donnees_Trafic : Mise a jour des donnees trafic
********************** 
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    15/03/12   : Creation (DEM/1016)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzao578.c	1.5 1.5 12/09/10 : xzao578" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO578_Donnees_Trafic(XDY_Horodate	va_Horodate)


/*
* ARGUMENTS EN ENTREE :CFG
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION
*
* FONCTION 
* XZAO19 appelle XZAO;19
* AOSP19 select dans les tables EQUIPEMENT et PORTION
* XZAO19 retourne les parametres
*
------------------------------------------------------*/
{

	CS_CHAR 	*pl_rpc_name 		= XZAOC_XZAO578_RPC_NAME;
	CS_CONTEXT      *pl_context;
	CS_CONNECTION   *pl_connection		= XZAGV_Connexion;
	CS_COMMAND	*pl_cmd;
	
	CS_RETCODE      vl_retcode		= XDC_NOK;
	CS_INT          vl_res_type		= 0;
	CS_SMALLINT     vl_msg_id		= 0;
	int 		vl_num_param_retour 	= XZAOC_XZAO578_NB_PARM_RETURN;
	tg_Colonne_donnees2 *pl_Tab_param_retour;
	
	
	int             vl_Nb_Ligne_Lue		= 0;
        int             vl_Nb_Col		= 0;
	int	i,vl_cr;
        tg_row_result   vl_Tab_Ligne;
	
	XDY_Entier      vl_SPstatus;
	CS_DATETIME	vl_HorodateDebut;
	CS_DATETIME	vl_HorodateFin;


	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/

	pl_Tab_param_retour = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*vl_num_param_retour);

	/*A
    ** Conversion des dates
    */


	


   /*A
    ** Envoie de la commande au Sql server
    */
    
    if (ct_send(pl_cmd) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"XZAO578_Donnees_Trafic: ct_send() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO578_Donnees_Trafic retourne %d", vl_retcode);
        asql_clean_cmd(pl_connection, pl_cmd);
           free(pl_Tab_param_retour);
           return (XDC_NOK);
    }

    /*A
    ** Traitement du resultat
    */

    

    while ((vl_retcode = ct_results(pl_cmd, &vl_res_type)) == CS_SUCCEED)
    {
        switch ((int)vl_res_type)
        {
            case CS_ROW_RESULT:
            case CS_PARAM_RESULT:
            case CS_STATUS_RESULT:
                /*B
                ** Impression de l entete en fonction du type de resultat.
                */
                switch ((int)vl_res_type)
                {
                    case  CS_ROW_RESULT:
                        vl_retcode = asql_fetch_data (pl_cmd);
                        XZST_03EcritureTrace(XZSTC_INFO,"XZAO578_Donnees_Trafic: ROW RESULTS");
                        break;

                    case  CS_PARAM_RESULT:
                        vl_retcode = asql_fetch_param_result (pl_cmd , vl_num_param_retour, pl_Tab_param_retour);
                        XZST_03EcritureTrace(XZSTC_INFO,"XZAO578_Donnees_Trafic : PARAMETER RESULTS\n");
                        break;

                    case  CS_STATUS_RESULT:
                        vl_retcode = asql_fetch (pl_cmd, &vl_SPstatus);
                        XZST_03EcritureTrace(XZSTC_INFO,"XZAO578_Donnees_Trafic : CS_STATUS_RESULT = %ld", vl_retcode);
                        XZST_03EcritureTrace(XZSTC_INFO,"XZAO578_Donnees_Trafic : STATUS RESULT PROCEDURE STOCKEE = %ld", vl_SPstatus);
                        
                        if (vl_SPstatus != XDC_OK)
                        {
                            XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO578_Donnees_Trafic retourne 1");
                            asql_clean_cmd(pl_connection, pl_cmd);
                            free(pl_Tab_param_retour);
                            return (XDC_NOK);
                        }
                        break;
                }
                
                if (vl_retcode != CS_SUCCEED)
                {
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao553 : asql_fetch_* a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO578_Donnees_Trafic retourne %d", vl_retcode);
                    asql_clean_cmd(pl_connection, pl_cmd);
                    free(pl_Tab_param_retour);
                    return (XDC_NOK);
                }

    
                break;


            case CS_MSG_RESULT:
                /*
                **
                */
                vl_retcode = ct_res_info(pl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
                if (vl_retcode != CS_SUCCEED)
                {
                    XZST_03EcritureTrace(XZSTC_WARNING,"XZAO578_Donnees_Trafic: ct_res_info(msgtype) a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO578_Donnees_Trafic retourne %d", vl_retcode);
                    asql_clean_cmd(pl_connection, pl_cmd);
                    free(pl_Tab_param_retour);
                    return (XDC_NOK);
                }
                
                XZST_03EcritureTrace(XZSTC_INFO,"XZAO578_Donnees_Trafic : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);
                break;

            case CS_CMD_SUCCEED:
                /*
                ** Pas de colonnes retournees.
                */
                XZST_03EcritureTrace(XZSTC_INFO,"XZAO578_Donnees_Trafic : CS_CMD_SUCCEED \n");
                break;

            case CS_CMD_DONE:

                /*
                ** Commande effectuee : positionnement des parametres de retour.
                */
                XZST_03EcritureTrace(XZSTC_INFO,"XZAO578_Donnees_Trafic : CS_CMD_DONE \n");
                break;

            case CS_CMD_FAIL:
                /*
                ** Le serveur SQL a detecte une ereeur en executant notre commande.
                */
                XZST_03EcritureTrace(XZSTC_WARNING,"XZAO578_Donnees_Trafic: ct_results retourne CS_CMD_FAIL.");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO578_Donnees_Trafic retourne %d", vl_retcode);
                asql_clean_cmd(pl_connection, pl_cmd);
                free(pl_Tab_param_retour);
                return (XDC_NOK);

            default:
                /*
                ** Il s est passe quelque chose d inattendu.
                */
                XZST_03EcritureTrace(XZSTC_WARNING,"XZAO578_Donnees_Trafic: ct_results retourne un resultat de type inattendu");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO578_Donnees_Trafic retourne %d", vl_retcode);
                asql_clean_cmd(pl_connection, pl_cmd);
                free(pl_Tab_param_retour);
                return (XDC_NOK);
        }
    }

    
    
    /*!
    ** Retourne le resultat et les parametres de sorties renseignes
    ** retourne -205 en sortie de boucle
    */
     
     XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO553_Ajouter_Zone_Reg retourne %d", vl_retcode);
     asql_clean_cmd(pl_connection, pl_cmd);
     free(pl_Tab_param_retour);
     XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO553_Ajouter_Zone_Reg tout est OK", vl_retcode);
    return(XDC_OK);
}




