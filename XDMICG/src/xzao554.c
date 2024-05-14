/*E*/
/* Fichier : $Id: xzao554.c,v 1.1 2012/07/04 10:43:11 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:43:11 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao554.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		   18 Janv 2012	   : Cr�ation (DEM/1016)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "@(#)xzao554.c    1.1 11/21/07 : xzao554" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO554SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO554_Ajouter_Eqt_PRV ( XDY_Basedd        va_Basedd_in,
                               XZAOT_ConfEqtPRV va_Equipement_in,
                               XDY_ResConf       *va_Resultat_out )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf           va_Resultat_out    
*
* VALEUR RENDUE :
*
* Ajout d'un equipement GTC en base
* 
* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Config OFF LINE
*
* INTERFACE :
*
* Insertion ou modification dans la table ZONE_REG
*
* MECANISMES :
*
------------------------------------------------------*/

{

    static char         *version = "@(#)xzao554.c    1.1 11/21/07 : XZAO554_Ajouter_Eqt_PRV" ;

    CS_CHAR             *rpc_name = XZAOC_XZAO554_RPC_NAME;
    CS_CONTEXT          *context;
    CS_CONNECTION       *connection=XZAGV_Connexion;
    CS_COMMAND          *cmd;
    CS_DATAFMT          datafmt;
    CS_RETCODE          retcode;
    CS_INT              res_type;
    CS_SMALLINT         msg_id;
    int                 num_Tab_donnees2 = XZAOC_XZAO554_NB_PARM_RETURN;
    tg_Colonne_donnees2 *Tab_donnees2;
    XDY_Entier          SPstatus;
    
    
    /*A
    ** Definition des variables locales
    */

	   
	XDY_Eqt				vl_Numero;
	XDY_TypeEqt			vl_Type;
	XDY_VitesseRegul	vl_Vitesse_Nominale;
	XDY_Entier			vl_Zone;
	XDY_Booleen			vl_Critique;
    
    XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO554_Ajouter_Eqt_PRV : Autoroute = %s\tSens = %d\tPR = %ld\tVitesse_Nominale = %d\tCritique = %d\tZone = %d\tInhibition = %d",
            va_Equipement_in.Numero,
			va_Equipement_in.Type,
            va_Equipement_in.Vitesse_Nominale,
			va_Equipement_in.Zone,
            va_Equipement_in.Critique);
    
     sprintf( vl_Vitesse_Nominale, "%s", va_Equipement_in.Vitesse_Nominale );

    vl_Numero             = va_Equipement_in.Numero;
	vl_Type             = va_Equipement_in.Type;
    vl_Critique         = va_Equipement_in.Critique;
	vl_Zone         = va_Equipement_in.Zone;

    
    

    /*A
    **  Allocation de la structure permettant la recuperation des parametres
    */
        
    Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);


    /*A
    ** Initialisation de la commande                
    */

    if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING, "xzao554: ct_cmd_alloc() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO554_Ajouter_Eqt_PRV retourne %d", retcode);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    
    
    
    /*A
    ** Envoie de la commande RPC pour notre stored procedure.
    */
    
    if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao554: ct_command() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO554_Ajouter_Eqt_PRV retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
           free(Tab_donnees2);
        return (XDC_NOK);
    }

    /*A 
    ** Initialisation de la structure pour chacun des parametres d I/O    
    ** A faire passer a la rpc.
    */

    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Basedd_in,"@va_Basedd_in"))!= CS_SUCCEED)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao554: asql_ctparam_char_input(Basedd) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO554_Ajouter_Eqt_PRV retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }


    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)&vl_Numero,"@va_Numero_in"))!= CS_SUCCEED)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao554: asql_ctparam_char_input(Autoroute) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO554_Ajouter_Eqt_PRV retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }
        
    
    
    
    if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Type,"@va_Type_in"))!= CS_SUCCEED)
    
    {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzao554: asql_ctparam_char_input(Sens) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO554_Ajouter_Eqt_PRV retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }

  
    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)&vl_Vitesse_Nominale,"@va_Vitesse_Nominale_in"))!= CS_SUCCEED)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao554: asql_ctparam_tinyint_input(Vitesse) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO554_Ajouter_Eqt_PRV retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }
     
    
     if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Zone,"@va_Zone_in"))!= CS_SUCCEED)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao554: asql_ctparam_char_input(Autoroute) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO554_Ajouter_Eqt_PRV retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }
		
	   if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Critique,"@va_Critique_in"))!= CS_SUCCEED)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao554: asql_ctparam_tinyint_input(Inhibition) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO554_Ajouter_Eqt_PRV retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }

    
    Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;
    
    if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)    

    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao554: asql_ctparam_int_output(Resultat) a echoue "); 
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO554_Ajouter_Eqt_PRV retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }

    


    /*A
    ** Envoie de la commande au Sql server
    */
    
    if (ct_send(cmd) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao554: ct_send() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO554_Ajouter_Eqt_PRV retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
           free(Tab_donnees2);
           return (XDC_NOK);
    }

    /*A
    ** Traitement du resultat
    */

    

    while ((retcode = ct_results(cmd, &res_type)) == CS_SUCCEED)
    {
        switch ((int)res_type)
        {
            case CS_ROW_RESULT:
            case CS_PARAM_RESULT:
            case CS_STATUS_RESULT:
                /*B
                ** Impression de l entete en fonction du type de resultat.
                */
                switch ((int)res_type)
                {
                    case  CS_ROW_RESULT:
                        retcode = asql_fetch_data (cmd);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao554: ROW RESULTS");
                        break;

                    case  CS_PARAM_RESULT:
                        retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao554 : PARAMETER RESULTS\n");
                        break;

                    case  CS_STATUS_RESULT:
                        retcode = asql_fetch (cmd, &SPstatus);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao554 : CS_STATUS_RESULT = %ld", retcode);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao554 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
                        
                        if (SPstatus != XDC_OK)
                        {
                            XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO554_Ajouter_Eqt_PRV retourne 1");
                            asql_clean_cmd(connection, cmd);
                            free(Tab_donnees2);
                            return (XDC_NOK);
                        }
                        break;
                }
                
                if (retcode != CS_SUCCEED)
                {
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao554 : asql_fetch_* a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO554_Ajouter_Eqt_PRV retourne %d", retcode);
                    asql_clean_cmd(connection, cmd);
                    free(Tab_donnees2);
                    return (XDC_NOK);
                }

    
                break;


            case CS_MSG_RESULT:
                /*
                **
                */
                retcode = ct_res_info(cmd, CS_MSGTYPE,(CS_VOID *)&msg_id, CS_UNUSED, NULL);
                if (retcode != CS_SUCCEED)
                {
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao554: ct_res_info(msgtype) a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO554_Ajouter_Eqt_PRV retourne %d", retcode);
                    asql_clean_cmd(connection, cmd);
                    free(Tab_donnees2);
                    return (XDC_NOK);
                }
                
                XZST_03EcritureTrace(XZSTC_INFO,"xzao554 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
                break;

            case CS_CMD_SUCCEED:
                /*
                ** Pas de colonnes retournees.
                */
                XZST_03EcritureTrace(XZSTC_INFO,"xzao554 : CS_CMD_SUCCEED \n");
                break;

            case CS_CMD_DONE:

                /*
                ** Commande effectuee : positionnement des parametres de retour.
                */
                XZST_03EcritureTrace(XZSTC_INFO,"xzao554 : CS_CMD_DONE \n");
                break;

            case CS_CMD_FAIL:
                /*
                ** Le serveur SQL a detecte une ereeur en executant notre commande.
                */
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao554: ct_results retourne CS_CMD_FAIL.");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO554_Ajouter_Eqt_PRV retourne %d", retcode);
                asql_clean_cmd(connection, cmd);
                free(Tab_donnees2);
                return (XDC_NOK);

            default:
                /*
                ** Il s est passe quelque chose d inattendu.
                */
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao554: ct_results retourne un resultat de type inattendu");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO554_Ajouter_Eqt_PRV retourne %d", retcode);
                asql_clean_cmd(connection, cmd);
                free(Tab_donnees2);
                return (XDC_NOK);
        }
    }

    
    
    /*!
    ** Retourne le resultat et les parametres de sorties renseignes
    ** retourne -205 en sortie de boucle
    */
     
     XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO554_Ajouter_Eqt_PRV retourne %d", retcode);
     asql_clean_cmd(connection, cmd);
     free(Tab_donnees2);
     XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO554_Ajouter_Eqt_PRV tout est OK", retcode);
    return(XDC_OK);
}




