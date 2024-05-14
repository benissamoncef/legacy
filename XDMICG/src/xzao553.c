/*E*/
/* Fichier : $Id: xzao553.c,v 1.4 2018/11/08 10:48:40 devgfi Exp $        $Revision: 1.4 $        $Date: 2018/11/08 10:48:40 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao553.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		17/01/2012  : Création (DEM/1016)
* PNI		03/08/2012  : correction passage mode horaire INT->BIT (DEM/1016)
* JPL		08/11/18 : Gestion du paramètre supplémentaire 'Supprime' (DEM 1305)  1.4
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "@(#)xzao553.c    1.1 11/21/07 : xzao553" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO553SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO553_Ajouter_Zone_Reg ( XDY_Basedd        va_Basedd_in,
                               XZAOT_ConfZoneReg va_Equipement_in,
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

    static char         *version = "@(#)xzao553.c    1.1 11/21/07 : XZAO553_Ajouter_Zone_Reg" ;

    CS_CHAR             *rpc_name = XZAOC_XZAO553_RPC_NAME;
    CS_CONTEXT          *context;
    CS_CONNECTION       *connection=XZAGV_Connexion;
    CS_COMMAND          *cmd;
    CS_DATAFMT          datafmt;
    CS_RETCODE          retcode;
    CS_INT              res_type;
    CS_SMALLINT         msg_id;
    int                 num_Tab_donnees2 = XZAOC_XZAO553_NB_PARM_RETURN;
    tg_Colonne_donnees2 *Tab_donnees2;
    XDY_Entier          SPstatus;
    
    
    /*A
    ** Definition des variables locales
    */

	XDY_Eqt			vl_Numero;
	XDY_Nom        	vl_Nom;
    XDY_NomAuto     vl_Autoroute;
	XDY_Sens        vl_Sens;
    XDY_PR          vl_PR_Debut;
	XDY_PR        	vl_PR_Fin;
    XDY_Mot       	vl_Vitesse;
	XDY_Booleen     vl_Inhibition;
	XDY_Booleen		vl_ModeHoraire;
	XDY_NomEqt		vl_NomServeur;
	XDY_Booleen	vl_Supprime;


    XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAO553_Ajouter_Zone_Reg dans base %s: Num=%d \tNom = %s\tAutoroute = %s\tSens = %d\tPR_Debut = %ld\tPR_Fin = %ld\tVitesse = %d\tInhibition = %d\tMod_Horaire = %d\tNomServeur = %s",
    		va_Basedd_in,
    		va_Equipement_in.Numero,
            va_Equipement_in.Nom,
            va_Equipement_in.Autoroute,
            va_Equipement_in.Sens,
            va_Equipement_in.PR_Debut,
            va_Equipement_in.PR_Fin,
            va_Equipement_in.Vitesse,
            va_Equipement_in.Inhibition,
            va_Equipement_in.Mode_Horaire,
            va_Equipement_in.NomServeur);

			vl_Nom[0] = '\0';
			vl_Autoroute[0] = '\0';
			vl_NomServeur[0] = '\0';

			sprintf( vl_Nom, "%s", va_Equipement_in.Nom );
			sprintf( vl_Autoroute, "%s", va_Equipement_in.Autoroute );
	
			vl_Numero           = va_Equipement_in.Numero;
			vl_Sens             = va_Equipement_in.Sens;
			vl_PR_Debut         = va_Equipement_in.PR_Debut;
			vl_PR_Fin           = va_Equipement_in.PR_Fin;
			vl_Vitesse	        = va_Equipement_in.Vitesse;
			vl_Inhibition       = va_Equipement_in.Inhibition;
			vl_ModeHoraire      = va_Equipement_in.Mode_Horaire;
			sprintf( vl_NomServeur,"%s", va_Equipement_in.NomServeur);
			vl_Supprime         = va_Equipement_in.Supprime;
    

    /*A
    **  Allocation de la structure permettant la recuperation des parametres
    */
        
    Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);


    /*A
    ** Initialisation de la commande                
    */

    if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING, "xzao553: ct_cmd_alloc() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO553_Ajouter_Zone_Reg retourne %d", retcode);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    
    
    
    /*A
    ** Envoie de la commande RPC pour notre stored procedure.
    */
    
    if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao553: ct_command() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO553_Ajouter_Zone_Reg retourne %d", retcode);
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
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao553: asql_ctparam_char_input(Basedd) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO553_Ajouter_Zone_Reg retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_Numero,"@va_Numero_in"))!= CS_SUCCEED)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao553: asql_ctparam_smallint_input(numero) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO553_Ajouter_Zone_Reg retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }


    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Nom,"@va_Nom_in"))!= CS_SUCCEED)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao553: asql_ctparam_char_input(Nom) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO553_Ajouter_Zone_Reg retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }

    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Autoroute,"@va_Autoroute_in"))!= CS_SUCCEED)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao553: asql_ctparam_char_input(Autoroute) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO553_Ajouter_Zone_Reg retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }
        
    if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Sens,"@va_Sens_in"))!= CS_SUCCEED)
    {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzao553: asql_ctparam_char_input(Sens) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO553_Ajouter_Zone_Reg retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }
        
    if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PR_Debut,"@va_Debut_in"))!= CS_SUCCEED)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao553: asql_ctparam_char_input(PR_Debut) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO553_Ajouter_Zone_Reg retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }

    if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PR_Fin,"@va_Fin_in"))!= CS_SUCCEED)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao553: asql_ctparam_int_input(PR_Fin) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO553_Ajouter_Zone_Reg retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }
    
    if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_Vitesse,"@va_Vitesse_amont_in"))!= CS_SUCCEED)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao553: asql_ctparam_smallint_input(Vitesse) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO553_Ajouter_Zone_Reg retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }
    
    if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_Inhibition,"@va_Inhibition_in"))!= CS_SUCCEED)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao553: asql_ctparam_tinyint_input(Inhibition) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO553_Ajouter_Zone_Reg retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }
    
    if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_ModeHoraire, "@va_ModeHoraire_in"))!=CS_SUCCEED)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao553: asql_ctparam_int_input(ModeHoraire) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO553_Ajouter_Zone_Reg retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }

    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)&vl_NomServeur, "@va_NomServeur_in"))!=CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao553: asql_ctparam_int_input(va_NomServeur_in) a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO553_Ajouter_Zone_Reg retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
        free(Tab_donnees2);
        return (XDC_NOK);
    }
    
    if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_Supprime,"@va_Supprime_in"))!= CS_SUCCEED)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao553: asql_ctparam_bit_input(Supprime) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO553_Ajouter_Zone_Reg retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }


    Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;
    
    if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)    

    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzao553: asql_ctparam_int_output(Resultat) a echoue "); 
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO553_Ajouter_Zone_Reg retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }

    


    /*A
    ** Envoie de la commande au Sql server
    */
    
    if (ct_send(cmd) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao553: ct_send() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO553_Ajouter_Zone_Reg retourne %d", retcode);
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
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao553: ROW RESULTS");
                        break;

                    case  CS_PARAM_RESULT:
                        retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao553 : PARAMETER RESULTS\n");
                        break;

                    case  CS_STATUS_RESULT:
                        retcode = asql_fetch (cmd, &SPstatus);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao553 : CS_STATUS_RESULT = %ld", retcode);
                        XZST_03EcritureTrace(XZSTC_INFO,"xzao553 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
                        
                        if (SPstatus != XDC_OK)
                        {
                            XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO553_Ajouter_Zone_Reg retourne 1");
                            asql_clean_cmd(connection, cmd);
                            free(Tab_donnees2);
                            return (XDC_NOK);
                        }
                        break;
                }
                
                if (retcode != CS_SUCCEED)
                {
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao553 : asql_fetch_* a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO553_Ajouter_Zone_Reg retourne %d", retcode);
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
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzao553: ct_res_info(msgtype) a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO553_Ajouter_Zone_Reg retourne %d", retcode);
                    asql_clean_cmd(connection, cmd);
                    free(Tab_donnees2);
                    return (XDC_NOK);
                }
                
                XZST_03EcritureTrace(XZSTC_INFO,"xzao553 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
                break;

            case CS_CMD_SUCCEED:
                /*
                ** Pas de colonnes retournees.
                */
                XZST_03EcritureTrace(XZSTC_INFO,"xzao553 : CS_CMD_SUCCEED \n");
                break;

            case CS_CMD_DONE:

                /*
                ** Commande effectuee : positionnement des parametres de retour.
                */
                XZST_03EcritureTrace(XZSTC_INFO,"xzao553 : CS_CMD_DONE \n");
                break;

            case CS_CMD_FAIL:
                /*
                ** Le serveur SQL a detecte une ereeur en executant notre commande.
                */
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao553: ct_results retourne CS_CMD_FAIL.");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO553_Ajouter_Zone_Reg retourne %d", retcode);
                asql_clean_cmd(connection, cmd);
                free(Tab_donnees2);
                return (XDC_NOK);

            default:
                /*
                ** Il s est passe quelque chose d inattendu.
                */
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao553: ct_results retourne un resultat de type inattendu");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO553_Ajouter_Zone_Reg retourne %d", retcode);
                asql_clean_cmd(connection, cmd);
                free(Tab_donnees2);
                return (XDC_NOK);
        }
    }

    
    
    /*!
    ** Retourne le resultat et les parametres de sorties renseignes
    ** retourne -205 en sortie de boucle
    */
     
     XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO553_Ajouter_Zone_Reg retourne %d", retcode);
     asql_clean_cmd(connection, cmd);
     free(Tab_donnees2);
     XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO553_Ajouter_Zone_Reg All is OK", retcode);
    return(XDC_OK);
}




