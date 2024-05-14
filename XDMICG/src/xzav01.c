/*E*/
/*Fichier : $Id: xzav01.c,v 1.1 2012/07/04 10:49:21 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:49:21 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAV * FICHIER xzav01.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		   09 Fevr 2012	   : Creation (DEM/1016)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzav.h"
static char *version = "@(#)xzav01.c    1.1 11/21/07 : xzav01" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAV01SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAV01_Tracer_Regulation ( XDY_Basedd       va_Basedd_in,
							   XZAVT_ConfTraceReg va_Equipement_in)


/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
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

    static char         *version = "@(#)xzav01.c    1.1 11/21/07 : XZAV01_Tracer_Regulation" ;

    CS_CHAR             *rpc_name = XZAVC_XZAV01_RPC_NAME;
    CS_CONTEXT          *context;
	CS_CONNECTION   	*connection=XZAGV_Connexion;
	CS_COMMAND			*cmd;
	CS_DATAFMT      	datafmt;
	CS_RETCODE      	retcode = 0;
	CS_INT          	res_type;
	CS_SMALLINT     	msg_id;
    int                 num_Tab_donnees2 = XZAVC_XZAV01_NB_PARM_RETURN;
    tg_Colonne_donnees2 *Tab_donnees2;
    XDY_Entier          SPstatus;
	CS_DATETIME	vl_Horodate;
    
    
    /*A
    ** Definition des variables locales
    */

	XDY_TypeAlerte 	vl_type;
	XDY_Alerte		vl_alerte;
	XDY_Entier		vl_numero_fmc;
	XDY_Octet		vl_cle_fmc;
	XDY_Octet		vl_acq_alerte;
	XDY_Eqt			vl_scenario;
	XDY_Entier		vl_condition;
	XDY_Booleen		vl_simulation;
	XDY_Eqt			vl_zone;
	XDY_Eqt			vl_station;
	XDY_Sens		vl_sens;

/*	XDY_Horodate		vl_Horodate;
	XDY_Entier			vl_Type;
	XDY_Octet			vl_Simulation;*/
	XZAVT_LibelleTrace	vl_Libelle;

    XZST_03EcritureTrace( XZSTC_INFO,"IN : XZAV01_Tracer_Regulation :tHorodate = %f\tType = %d\tLibelle = %s\tSimulation = %d",
            va_Equipement_in.Horodate,
            va_Equipement_in.Type,
            va_Equipement_in.Libelle,
            va_Equipement_in.Simulation);
    XZST_03EcritureTrace( XZSTC_INFO," alerte %d acquittement %d",
				va_Equipement_in.Alerte,
				va_Equipement_in.Acquittement);
	vl_Libelle[0] = '\0';
	
    	sprintf( vl_Libelle, "%s", va_Equipement_in.Libelle );
	
/*    	vl_Horodate     = va_Equipement_in.Horodate; */
    	vl_type         = va_Equipement_in.Type;
    	vl_alerte		= va_Equipement_in.Alerte;
    	vl_numero_fmc	= va_Equipement_in.Numero_FMC;
    	vl_cle_fmc		= va_Equipement_in.Cle_FMC;
    	vl_acq_alerte	= va_Equipement_in.Acquittement;
    	vl_scenario		= va_Equipement_in.Scenario;
    	vl_condition	= va_Equipement_in.Condition;
    	vl_station		= va_Equipement_in.station;
    	vl_sens			= va_Equipement_in.sens;

    	if (va_Equipement_in.Simulation==1)
    		vl_simulation=XDC_VRAI;
    	else
    		vl_simulation = XDC_FAUX;
    	//vl_simulation   = va_Equipement_in.Simulation;
    	vl_zone			= va_Equipement_in.Zone;

   
	if ((retcode = asql_date_Ux2Sybase(va_Equipement_in.Horodate,&vl_Horodate))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"XZAV01_Tracer_Regulation: asql_date_Ux2Sybase(va_Equipement_in.Horodate,&vl_Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV01_Tracer_Regulation retourne %d",
					retcode);
		return (XDC_NOK);
	}


    /*A
    **  Allocation de la structure permettant la recuperation des parametres
    */
        
    Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);


    /*A
    ** Initialisation de la commande                
    */

    if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING, "xzav01: ct_cmd_alloc() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV01_Tracer_Regulation retourne %d", retcode);
        free(Tab_donnees2);
        return (XDC_NOK);
    }

    /*A
    ** Envoie de la commande RPC pour notre stored procedure.
    */
    
    if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzav01: ct_command() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV01_Tracer_Regulation retourne %d", retcode);
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
       XZST_03EcritureTrace(XZSTC_WARNING,"xzav01: asql_ctparam_char_input(Basedd) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV01_Tracer_Regulation retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }


    if ((retcode = asql_ctparam_date_input(cmd,(CS_DATETIME *)&vl_Horodate,"@va_horodate_in"))!= CS_SUCCEED)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzav01: asql_ctparam_char_input(Nom) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV01_Tracer_Regulation retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }
    
    if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_type,"@va_type_in"))!= CS_SUCCEED)
    
    {
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzav01: asql_ctparam_char_input(Sens) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV01_Tracer_Regulation retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }
        

    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)&vl_Libelle,"@va_libelle_in"))!= CS_SUCCEED)
    
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzav01: asql_ctparam_char_input(PR_Debut) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV01_Tracer_Regulation retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }

	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_alerte,"@va_alerte_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAV01_Tracer_Regulation: asql_ctparam_int_input(int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV01_Tracer_Regulation retourne %d", XDC_NOK  );
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_numero_fmc,"@va_numero_fmc_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAV01_Tracer_Regulation: asql_ctparam_int_input(int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV01_Tracer_Regulation retourne %d", XDC_NOK  );
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_cle_fmc,"@va_cle_fmc_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAV01_Tracer_Regulation: asql_ctparam_tinyint_input() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV01_Tracer_Regulation retourne %d", XDC_NOK );
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_acq_alerte,"@va_acq_alerte_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAV01_Tracer_Regulation: asql_ctparam_tinyint_input() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV01_Tracer_Regulation retourne %d", XDC_NOK );
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_station,"@va_num_station_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAV01_Tracer_Regulation: asql_ctparam_smallint_input() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV01_Tracer_Regulation retourne %d", XDC_NOK );
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_sens,"@va_sens_station_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAV01_Tracer_Regulation: asql_ctparam_tinyint_input() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV01_Tracer_Regulation retourne %d", XDC_NOK );
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_scenario,"@va_scenario_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAV01_Tracer_Regulation: asql_ctparam_smallint_input() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV01_Tracer_Regulation retourne %d", XDC_NOK );
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_condition,"@va_condition_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAV01_Tracer_Regulation: asql_ctparam_smallint_input() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV01_Tracer_Regulation retourne %d", XDC_NOK );
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

    if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_simulation,"@va_simulation_in"))!= CS_SUCCEED)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzav01: asql_ctparam_int_input(PR_Fin) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV01_Tracer_Regulation retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }

    if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&vl_zone,"@va_zone_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAV01_Tracer_Regulation: asql_ctparam_smallint_input() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV01_Tracer_Regulation retourne %d", XDC_NOK );
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}
    /*A
    ** Envoie de la commande au Sql server
    */
    
    if (ct_send(cmd) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzav01: ct_send() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV01_Tracer_Regulation retourne %d", retcode);
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
                        XZST_03EcritureTrace(XZSTC_DEBUG1,"xzav01: ROW RESULTS");
                        break;

                    case  CS_PARAM_RESULT:
                        retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
                        XZST_03EcritureTrace(XZSTC_DEBUG1,"xzav01 : PARAMETER RESULTS\n");
                        break;

                    case  CS_STATUS_RESULT:
                        retcode = asql_fetch (cmd, &SPstatus);
                        XZST_03EcritureTrace(XZSTC_DEBUG1,"xzav01 : CS_STATUS_RESULT = %ld", retcode);
                        XZST_03EcritureTrace(XZSTC_DEBUG1,"xzav01 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
                        
                        if (SPstatus != XDC_OK)
                        {
                            XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV01_Tracer_Regulation retourne 1");
                            asql_clean_cmd(connection, cmd);
                            free(Tab_donnees2);
                            return (XDC_NOK);
                        }
                        break;
                }
                
                if (retcode != CS_SUCCEED)
                {
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzav01 : asql_fetch_* a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV01_Tracer_Regulation retourne %d", retcode);
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
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzav01: ct_res_info(msgtype) a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV01_Tracer_Regulation retourne %d", retcode);
                    asql_clean_cmd(connection, cmd);
                    free(Tab_donnees2);
                    return (XDC_NOK);
                }
                
                XZST_03EcritureTrace(XZSTC_DEBUG1,"xzav01 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
                break;

            case CS_CMD_SUCCEED:
                /*
                ** Pas de colonnes retournees.
                */
                XZST_03EcritureTrace(XZSTC_DEBUG1,"xzav01 : CS_CMD_SUCCEED \n");
                break;

            case CS_CMD_DONE:

                /*
                ** Commande effectuee : positionnement des parametres de retour.
                */
                XZST_03EcritureTrace(XZSTC_DEBUG1,"xzav01 : CS_CMD_DONE \n");
                break;

            case CS_CMD_FAIL:
                /*
                ** Le serveur SQL a detecte une ereeur en executant notre commande.
                */
                XZST_03EcritureTrace(XZSTC_WARNING,"xzav01: ct_results retourne CS_CMD_FAIL.");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV01_Tracer_Regulation retourne %d", retcode);
                asql_clean_cmd(connection, cmd);
                free(Tab_donnees2);
                return (XDC_NOK);

            default:
                /*
                ** Il s est passe quelque chose d inattendu.
                */
                XZST_03EcritureTrace(XZSTC_WARNING,"xzav01: ct_results retourne un resultat de type inattendu");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV01_Tracer_Regulation retourne %d", retcode);
                asql_clean_cmd(connection, cmd);
                free(Tab_donnees2);
                return (XDC_NOK);
        }
    }

    
    
    /*!
    ** Retourne le resultat et les parametres de sorties renseignes
    ** retourne -205 en sortie de boucle
    */
     
     XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV01_Tracer_Regulation retourne %d", retcode);
     asql_clean_cmd(connection, cmd);
     free(Tab_donnees2);
     XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV01_Tracer_Regulation tout est OK", retcode);
    return(XDC_OK);
}




