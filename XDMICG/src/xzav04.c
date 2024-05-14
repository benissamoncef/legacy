/*E*/
/*Fichier : %W%      Release : %I%        Date : %G%
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAV * FICHIER xzav04.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	12/01/18 creation GABEB 1.1
------------------------------------------------------*/

/* fichiers inclus */

#include "xzav.h"
static char *version = "@(#)xzav04.c    1.1 11/21/07 : xzav04" ;

/* declaration de fonctions internes */
CS_STATIC CS_RETCODE CS_INTERNAL
BusyWait(CS_CONNECTION *connection, char *where)
{
        CS_COMMAND      *compcmd;
        CS_INT          compid;
        CS_RETCODE      compstat;
        CS_RETCODE      retstat;

        XZST_03EcritureTrace(XZSTC_WARNING,"XZAE213: Waiting [%s]", where);
        

        do
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZAE213: .");
                
                retstat = ct_poll(NULL, connection, 100, NULL, &compcmd,
                        &compid, &compstat);
                if (retstat != CS_SUCCEED && retstat != CS_TIMED_OUT &&
                    retstat != CS_INTERRUPT)
                {
						if (retstat == CS_QUIET)
                        	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE213:ct_poll returned CS_QUIET",
                                retstat);
						else
                        	XZST_03EcritureTrace(XZSTC_WARNING,"XZAE213:ct_poll returned unexpected status of %d",
                                retstat);
                        break;
                }
        } while (retstat != CS_SUCCEED);

        if (retstat == CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"XZAE213:ct_poll completed: compid = %d, compstat = %d\n",
                        compid, compstat);
                
        }

        return compstat;
}
/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAV04SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAV04_Tracer_GABEB ( XZAVT_ConfTraceCAB va_Equipement_in)


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

    static char         *version = "@(#)xzav04.c    1.1 11/21/07 : XZAV04_Tracer_GABEB" ;

    CS_CHAR             *rpc_name = XZAVC_XZAV04_RPC_NAME;
    CS_CONTEXT          *context;
	CS_CONNECTION   	*connection=XZAGV_Connexion;
	CS_COMMAND			*cmd;
	CS_DATAFMT      	datafmt;
	CS_RETCODE      	retcode = 0;
	CS_INT          	res_type;
	CS_SMALLINT     	msg_id;
    int                 num_Tab_donnees2 = XZAVC_XZAV04_NB_PARM_RETURN;
    tg_Colonne_donnees2 *Tab_donnees2;
    XDY_Entier          SPstatus;
	CS_DATETIME	vl_Horodate;
    
    
    /*A
    ** Definition des variables locales
    */

	XDY_TypeAlerte 	vl_type;
	XDY_Entier		vl_numero_fmc;
	XDY_Octet		vl_cle_fmc;
	XDY_Octet		vl_contexte;
	XDY_Eqt			vl_cab1;
	XDY_Entier		vl_ech;
	XDY_Octet		vl_acquittement;
	XDY_Entier		vl_alerte;

	XZAVT_LibelleTrace	vl_Libelle;

    XZST_03EcritureTrace( XZSTC_INFO,"IN : XZAV04_Tracer_GABEB :tHorodate = %f\tType = %d\tLibelle = %s",
            va_Equipement_in.Horodate,
            va_Equipement_in.Type,
            va_Equipement_in.Libelle);
	vl_Libelle[0] = '\0';
	
    	sprintf( vl_Libelle, "%s", va_Equipement_in.Libelle );
	
    	vl_type         = va_Equipement_in.Type;
    	vl_numero_fmc	= va_Equipement_in.Numero_FMC;
    	vl_cle_fmc		= va_Equipement_in.Cle_FMC;
    	vl_cab1	= va_Equipement_in.Cab1;
    	vl_ech		= (XDY_Entier)(va_Equipement_in.Echangeur);
    	vl_contexte	= va_Equipement_in.Contexte;
	vl_acquittement = va_Equipement_in.Acquittement;
	vl_alerte = va_Equipement_in.Alerte;

	if ((retcode = asql_date_Ux2Sybase(va_Equipement_in.Horodate,&vl_Horodate))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"XZAV04_Tracer_GABEB: asql_date_Ux2Sybase(va_Equipement_in.Horodate,&vl_Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV04_Tracer_GABEB retourne %d",
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
//if (retcode==CS_BUSY)
		if (retcode==CS_SUCCEED)
		{ //Wait par ct_poll
			retcode = BusyWait(connection, "ct_cmd_alloc");
        	if (retcode != CS_SUCCEED && retcode != CS_PENDING)
        	{
                XZST_03EcritureTrace(XZSTC_WARNING,"XZAE213: ct_poll() pour ct_cmd_alloc a echoue");
				XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE213_Fermeture_Echangeur retourne %d", retcode);
				asql_clean_cmd(connection, &cmd);
                free(Tab_donnees2);
				return (XDC_NOK);
        	}
			else
			{
				if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
				{

                    XZST_03EcritureTrace(XZSTC_WARNING, "xzav04: ct_cmd_alloc() a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV04_Tracer_GABEB retourne %d", retcode);
                    asql_clean_cmd(connection, &cmd);
                    free(Tab_donnees2);
                    return (XDC_NOK);
                }
            }
        }
    }

    /*A
    ** Envoie de la commande RPC pour notre stored procedure.
    */
    
    if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzav04: ct_command() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV04_Tracer_GABEB retourne %d", retcode);
        asql_clean_cmd(connection, cmd);
           free(Tab_donnees2);
        return (XDC_NOK);
    }

    /*A 
    ** Initialisation de la structure pour chacun des parametres d I/O    
    ** A faire passer a la rpc.
    */

    if ((retcode = asql_ctparam_date_input(cmd,(CS_DATETIME *)&vl_Horodate,"@va_horodate_in"))!= CS_SUCCEED)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzav04: asql_ctparam_char_input(Nom) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV04_Tracer_GABEB retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }
    
    if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_type,"@va_type_in"))!= CS_SUCCEED)
    
    {
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzav04: asql_ctparam_char_input(Sens) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV04_Tracer_GABEB retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }
        

    if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)&vl_Libelle,"@va_libelle_in"))!= CS_SUCCEED)
    
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"xzav04: asql_ctparam_char_input(PR_Debut) a echoue");
       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV04_Tracer_GABEB retourne %d", retcode);
       asql_clean_cmd(connection, cmd);
       free(Tab_donnees2);
       return (XDC_NOK);
    }

	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_alerte,"@va_alerte_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAV04_Tracer_GABEB: asql_ctparam_int_input(int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV04_Tracer_GABEB retourne %d", XDC_NOK  );
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_numero_fmc,"@va_numero_fmc_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAV04_Tracer_GABEB: asql_ctparam_int_input(int) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV04_Tracer_GABEB retourne %d", XDC_NOK  );
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_cle_fmc,"@va_cle_fmc_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAV04_Tracer_GABEB: asql_ctparam_tinyint_input() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV04_Tracer_GABEB retourne %d", XDC_NOK );
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_acquittement,"@va_acq_alerte_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAV04_Tracer_GABEB: asql_ctparam_tinyint_input() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV04_Tracer_GABEB retourne %d", XDC_NOK );
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_ech,"@va_echangeur_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAV04_Tracer_GABEB: asql_ctparam_smallint_input() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV04_Tracer_GABEB retourne %d", XDC_NOK );
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_cab1,"@va_capteur1_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAV04_Tracer_GABEB: asql_ctparam_smallint_input() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV04_Tracer_GABEB retourne %d", XDC_NOK );
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_contexte,"@va_contexte_in"))!= CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAV04_Tracer_GABEB: asql_ctparam_smallint_input() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV04_Tracer_GABEB retourne %d", XDC_NOK );
		asql_clean_cmd(connection, cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}


    /*A
    ** Envoie de la commande au Sql server
    */
    
    if (ct_send(cmd) != CS_SUCCEED)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"xzav04: ct_send() a echoue");
        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV04_Tracer_GABEB retourne %d", retcode);
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
                        XZST_03EcritureTrace(XZSTC_DEBUG1,"xzav04: ROW RESULTS");
                        break;

                    case  CS_PARAM_RESULT:
                        retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
                        XZST_03EcritureTrace(XZSTC_DEBUG1,"xzav04 : PARAMETER RESULTS\n");
                        break;

                    case  CS_STATUS_RESULT:
                        retcode = asql_fetch (cmd, &SPstatus);
                        XZST_03EcritureTrace(XZSTC_DEBUG1,"xzav04 : CS_STATUS_RESULT = %ld", retcode);
                        XZST_03EcritureTrace(XZSTC_DEBUG1,"xzav04 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
                        
                        if (SPstatus != XDC_OK)
                        {
                            XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV04_Tracer_GABEB retourne 1");
                            asql_clean_cmd(connection, cmd);
                            free(Tab_donnees2);
                            return (XDC_NOK);
                        }
                        break;
                }
                
                if (retcode != CS_SUCCEED)
                {
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzav04 : asql_fetch_* a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV04_Tracer_GABEB retourne %d", retcode);
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
                    XZST_03EcritureTrace(XZSTC_WARNING,"xzav04: ct_res_info(msgtype) a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV04_Tracer_GABEB retourne %d", retcode);
                    asql_clean_cmd(connection, cmd);
                    free(Tab_donnees2);
                    return (XDC_NOK);
                }
                
                XZST_03EcritureTrace(XZSTC_DEBUG1,"xzav04 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
                break;

            case CS_CMD_SUCCEED:
                /*
                ** Pas de colonnes retournees.
                */
                XZST_03EcritureTrace(XZSTC_DEBUG1,"xzav04 : CS_CMD_SUCCEED \n");
                break;

            case CS_CMD_DONE:

                /*
                ** Commande effectuee : positionnement des parametres de retour.
                */
                XZST_03EcritureTrace(XZSTC_DEBUG1,"xzav04 : CS_CMD_DONE \n");
                break;

            case CS_CMD_FAIL:
                /*
                ** Le serveur SQL a detecte une ereeur en executant notre commande.
                */
                XZST_03EcritureTrace(XZSTC_WARNING,"xzav04: ct_results retourne CS_CMD_FAIL.");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV04_Tracer_GABEB retourne %d", retcode);
                asql_clean_cmd(connection, cmd);
                free(Tab_donnees2);
                return (XDC_NOK);

            default:
                /*
                ** Il s est passe quelque chose d inattendu.
                */
                XZST_03EcritureTrace(XZSTC_WARNING,"xzav04: ct_results retourne un resultat de type inattendu");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV04_Tracer_GABEB retourne %d", retcode);
                asql_clean_cmd(connection, cmd);
                free(Tab_donnees2);
                return (XDC_NOK);
        }
    }

    
    
    /*!
    ** Retourne le resultat et les parametres de sorties renseignes
    ** retourne -205 en sortie de boucle
    */
     
     XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV04_Tracer_GABEB retourne %d", retcode);
     asql_clean_cmd(connection, cmd);
     free(Tab_donnees2);
     XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV04_Tracer_GABEB tout est OK", retcode);
    return(XDC_OK);
}




