/*E*/
/* Fichier : $Id: xzac260.c,v 1.1 2018/04/18 17:58:09 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2018/04/18 17:58:09 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac260.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
*  LCL	26/02/18	: Creation CTRL_FEUX 1.1 DEM1284
------------------------------------------------------*/

/* fichiers inclus */

#include "xzac.h"
#include "xzac260.h"

static char *version = "@(#)xzac260.c	1.1 1.1 09/13/05 : xzac260" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAC260SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAC260_Recap_Info_CFE (	XDY_Horodate	va_Horodate_in,
				XDY_Evt		va_NumEvt_in,
				XDY_FonctionInt	pa_FonctionUtil_in )

/*X*/
/*-----------------------------------------------------
* Service rendu
* Recapitule l'utilisation actuelle des CFE pour un evenement.
* 
* Sequence d'appel
* CL	XZAC260_Recap_Info_CFE
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* 
* Arguments en sortie
* XDY_Octet	va_NumCFE_out
* tinyint 	va_PlanDeFeux_out		
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Les informations sont retournees dans une liste
* 
* Fonction
* select dans les tables ACTION et E_CFE 
* where evt, heure_succes<Horodate<heure_fin
* 
------------------------------------------------*/


{

	static char *version = "@(#)xzac260.c	1.3 04/24/95 : XZAC260_Recap_Info_CFE" ;

	CS_CHAR *rpc_name = XZACC_XZAC260_RPC_NAME;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode = 0;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int		vl_Nb_Ligne_Lue;
	int		vl_Nb_Col;
	tg_row_result 	vl_Tab_Ligne;
	int		i;
	XDY_Entier 	SPstatus;
	CS_DATETIME	vl_Horodate;
	XDY_Entier 	vl_NumEvt;
	XDY_Octet 	vl_CleEvt;
	XDY_Octet	vl_PlanDeFeux;
	char		vl_LibellePlanDeFeux[150];
	
	
	/*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/
	
	if ((retcode = asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_date_Ux2Sybase(va_Horodate_in,&vl_Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC260_Recap_Info_CFEretourne %d", retcode);
		return (XDC_NOK);
	}
	
	
	vl_NumEvt = va_NumEvt_in.numero;
	vl_CleEvt = va_NumEvt_in.cle;
	
	
	
	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAC260_Recap_Info_CFE : Jours = %d\tHeure = %d\tNumEqt = %d\tCleEqt = %d\n" , vl_Horodate.dtdays,vl_Horodate.dttime,va_NumEvt_in.numero,va_NumEvt_in.cle );
			
			
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzac260: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC260_Recap_Info_CFE retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzac260: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC260_Recap_Info_CFE retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_date_input(cmd,(CS_DATETIME *)&vl_Horodate,"@va_Horodate_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac260: asql_ctparam_date_input(Horodate) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC260_Recap_Info_CFE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_NumEvt,"@va_NumEvt_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac260: asql_ctparam_int_input(NumEvt) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC260_Recap_Info_CFE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_CleEvt,"@va_CleEvt_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac260: asql_ctparam_tinyint_input(CleEvt) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC260_Recap_Info_CFE retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzac260: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC260_Recap_Info_CFE retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
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
						retcode = asql_fetch_row_result(cmd,&vl_Nb_Ligne_Lue,&vl_Nb_Col,&vl_Tab_Ligne);
						XZST_03EcritureTrace(XZSTC_INFO,"xzac260: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzac260 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzac260 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzac260 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC260_Recap_Info_CFE retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzac260 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC260_Recap_Info_CFE retourne %d", retcode);
                                        asql_clean_cmd(connection, cmd);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzac260: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC260_Recap_Info_CFE retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
						return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzac260 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzac260 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzac260 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzac260: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC260_Recap_Info_CFE retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzac260: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC260_Recap_Info_CFE retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
					return (XDC_NOK);
				}
	}

	
	
	
	for ( i = 0; i < vl_Nb_Ligne_Lue; i++ )
	{
        
           /*B
           ** Affectation de la variable vl_TypeAstreinte
           ** Appel de la fonction utilisateur FonctionUtili
           */ 
           
	   vl_PlanDeFeux = * (XDY_Octet *) vl_Tab_Ligne[i][0].pt_value;

           strcpy ( vl_LibellePlanDeFeux, (char *) vl_Tab_Ligne[i][1].pt_value );
        
           if ( pa_FonctionUtil_in == NULL)
	   {
	      XZST_03EcritureTrace(XZSTC_WARNING,"xzac260 : pa_FonctionUtil est nulle");
	      asql_clean_cmd(connection, cmd);
	      return ( XDC_NOK );
	   }
	   else
	   {
	      if ( ( retcode = (*pa_FonctionUtil_in) ( vl_Message, vl_Alternat ) ) != XDC_OK )
	      {
	         XZST_03EcritureTrace(XZSTC_WARNING,"xzac260 : appel de pa_FonctionUtil s'est mal deroule");
	         asql_clean_cmd(connection, cmd);
	         return ( XDC_NOK );
	      }
	   }
	}         						
	      							
	
	/*A
	** Desallocation du tableau de lignes de colonnes renvoye par un select effectue par la SP
	** si il y a lieu
	*/ 
	
	retcode =  asql_Free_Tab_Ligne_Results(vl_Nb_Ligne_Lue,vl_Nb_Col,&vl_Tab_Ligne);
	
	if (retcode != XDC_OK)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac260 : asql_fetch_row_result: asql_Free_Tab_Ligne_Results n a pu liberer correctement la memoire");
           asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}	
 	
 	
 	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC260_Recap_Info_CFE retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




