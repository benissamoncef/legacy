/*E*/
/* Fichier : $Id: xzao80.c,v 1.6 2020/11/03 17:57:20 pc2dpdy Exp $        $Revision: 1.6 $        $Date: 2020/11/03 17:57:20 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao80.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       22 Sep 1994     : Creation
* volcic       01 Nov 1994     : Modification entete (v1.2)
* JMG   13/11/07        : ajout site de gestion DEM715
* JPL	06/11/18 	: Gestion du paramètre supplémentaire 'Supprime' (DEM 1305)  1.4
* LCL	11/06/19	: DAI/IP DEM1333 1.5
* CGR	11/09/20	: Ajout deuxieme adresse IP DEM-SAE175 1.6
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao80.c,v 1.6 2020/11/03 17:57:20 pc2dpdy Exp $ : xzao80" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAO80SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO80_Ajouter_DAI (	XDY_Basedd		va_Basedd_in,
				XZAOT_ConfDAI	 	va_DAI_in,
				XDY_ResConf  		*va_Resultat_out )

/*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in        		
* XDY_NomEqt    va_NomDAI_in
* XDY_Nom       va_Autoroute_in
* XDY_PR        va_PR_in
* XDY_Sens      va_Sens_in
* XDY_NomEqt    va_NomServeur_in
* XDY_Octet     va_LiaisonMaitre_in
* XDY_Octet     va_LiaisonEsclave_in
* XDY_Rgs       va_Rgs_in			
* XDY_AdresseIP va_Adresse_IP_in
* XDY_Port      va_Port_in
* XDY_AdresseIP va_Adresse_IPE_in
* XDY_Port	va_PortE_in
* XDY_AdresseIP	va_Adresse2_IP_in
* XDY_Port	va_Port2_in
* XDY_AdresseIP va_Adresse2_IPE_in
* XDY_Port	va_Port2E_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute une DAI en base
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Config OFF LINE
*
* INTERFACE :
*
* Insertion ou modification dans les tables EQT_DAI (DAI) et EQT_GEN (EQUIPEMENT)
*
* MECANISMES :
*
------------------------------------------------------*/

{

	static char *version = "@(#)xzao80.c	1.2 12/15/94 : XZAO80_Ajouter_DAI" ;

	CS_CHAR *rpc_name = XZAOC_XZAO80_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZAOC_XZAO80_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier	SPstatus;
	
	
	/*A
	** Definition des variables locales
	*/
	
	XDY_NomEqt	vl_NomDAI;
	XDY_NomSite	vl_NomSite;
   	XDY_NomAuto	vl_Autoroute;
   	XDY_PR		vl_PR;
   	XDY_Sens	vl_Sens;
   	XDY_NomEqt	vl_NomServeur;
   	XDY_Octet	vl_LiaisonMaitre;
   	XDY_Octet	vl_LiaisonEsclave;
   	XDY_Rgs		vl_Rgs; 
   	XDY_Booleen	vl_Supprime;
        XDY_AdresseIP   vl_Adresse_IP;
        XDY_PortIP      vl_Port;
        XDY_AdresseIP   vl_Adresse_IPE;
        XDY_PortIP      vl_PortE;
	XDY_AdresseIP	vl_Adresse2_IP;
	XDY_PortIP	vl_Port2;
	XDY_AdresseIP	vl_Adresse2_IPE;
	XDY_PortIP	vl_Port2E;
   	
	/*A
	** Definition des variables locales
	*/
	
	strcpy( vl_NomDAI, va_DAI_in.NomDAI );
	strcpy( vl_NomSite, va_DAI_in.NomSite );
	strcpy( vl_Autoroute, va_DAI_in.Autoroute );
	vl_PR             = va_DAI_in.PR;
	vl_Sens           = va_DAI_in.Sens;
	strcpy( vl_NomServeur, va_DAI_in.NomServeur );
	vl_LiaisonMaitre  = va_DAI_in.LiaisonMaitre;
	vl_LiaisonEsclave = va_DAI_in.LiaisonEsclave;
	strcpy( vl_Rgs, va_DAI_in.Rgs );
	vl_Supprime       = va_DAI_in.Supprime;
        strcpy( vl_Adresse_IP, va_DAI_in.Adresse_IP );
        vl_Port 	  = va_DAI_in.Port;
        strcpy( vl_Adresse_IPE, va_DAI_in.Adresse_IPE );
        vl_PortE 	  = va_DAI_in.PortE;
	strcpy( vl_Adresse2_IP, va_DAI_in.Adresse2_IP );
	vl_Port2	  = va_DAI_in.Port2;
	strcpy( vl_Adresse2_IPE, va_DAI_in.Adresse2_IPE );
	vl_Port2E	  = va_DAI_in.Port2E;
	
	
	
	XZST_03EcritureTrace( XZSTC_WARNING,"IN : XZAO80_Ajouter_DAI : DAI = %s\tAutoroute = %s\tPR = %d\tSens = %d\tServeur = %s\tLiaisonMaitre = %d\tLiaisonEsclave = %d\tRgs = %s\tSiteGestion=%s\t Adresse IP = %s\t Port = %d\t Adresse IPE = %s\t PortE = %d\t Adresse IP 2 = %s\t Port 2 = %d\t Adresse IPE 2 = %s\t PortE 2 = %d \n",
			va_DAI_in.NomDAI,
			va_DAI_in.Autoroute,
			va_DAI_in.PR,
			va_DAI_in.Sens,
			va_DAI_in.NomServeur,
			va_DAI_in.LiaisonMaitre,
			va_DAI_in.LiaisonEsclave,
			va_DAI_in.Rgs ,
			va_DAI_in.NomSite,
                        va_DAI_in.Adresse_IP,
                        va_DAI_in.Port,
			va_DAI_in.Adresse_IPE,
			va_DAI_in.PortE,
			va_DAI_in.Adresse2_IP,
			va_DAI_in.Port2,
			va_DAI_in.Adresse2_IPE,
			va_DAI_in.Port2E);
			
			
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);
	
	
	
	
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzao80: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80_Ajouter_DAI retourne %d", retcode);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}

	
	
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao80: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80_Ajouter_DAI retourne %d", retcode);
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
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao80: asql_ctparam_char_input(Basedd) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80_Ajouter_DAI retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomDAI,"@va_NomDAI_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao80: asql_ctparam_char_input(NomDAI) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80_Ajouter_DAI retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Autoroute,"@va_Autoroute_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao80: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80_Ajouter_DAI retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PR,"@va_PR_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao51: asql_ctparam_int_input(PR) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80_Ajouter_DAI retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_Sens,"@va_Sens_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao51: asql_ctparam_tinyint_input(Sens) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80_Ajouter_DAI retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomServeur,"@va_NomServeur_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao80: asql_ctparam_char_input(NomServeur) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80_Ajouter_DAI retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_LiaisonMaitre,"@va_LiaisonMaitre_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao51: asql_ctparam_tinyint_input(LiaisonMaitre) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80_Ajouter_DAI retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&vl_LiaisonEsclave,"@va_LiaisonEsclave_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao51: asql_ctparam_tinyint_input(LiaisonEsclave) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80_Ajouter_DAI retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Rgs,"@va_Rgs_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao80: asql_ctparam_char_input(Rgs) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80_Ajouter_DAI retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_NomSite,"@va_NomSite_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao80: asql_ctparam_char_input(NomSite) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80_Ajouter_DAI retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}


	if ((retcode = asql_ctparam_bit_input(cmd,(CS_BIT *)&vl_Supprime,"@va_Supprime_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao80: asql_ctparam_bit_input(Supprime) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80_Ajouter_DAI retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	XZST_03EcritureTrace( XZSTC_WARNING,"IN : XZAO80_Ajouter_DAI : Adresse IP = %s",vl_Adresse_IP);
        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Adresse_IP,"@va_Adresse_IP_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzao80: asql_ctparam_char_input() a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80 retourne %d", retcode);
           asql_clean_cmd(connection, cmd);
           free(Tab_donnees2);
           return (XDC_NOK);
        }


        if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_Port,"@va_Port_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzao80: asql_ctparam_int_input(int) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80 retourne %d", retcode);
           asql_clean_cmd(connection, cmd);
           free(Tab_donnees2);
           return (XDC_NOK);
        }


        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Adresse_IPE,"@va_Adresse_IPE_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzao80: asql_ctparam_char_input() a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80 retourne %d", retcode);
           asql_clean_cmd(connection, cmd);
           free(Tab_donnees2);
           return (XDC_NOK);
        }


        if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_PortE,"@va_PortE_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzao80: asql_ctparam_int_input(int) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80 retourne %d", retcode);
           asql_clean_cmd(connection, cmd);
           free(Tab_donnees2);
           return (XDC_NOK);
        }


	XZST_03EcritureTrace( XZSTC_WARNING,"IN : XZAO80_Ajouter_DAI : Adresse IP 2 = %s",vl_Adresse2_IP);
        if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Adresse2_IP,"@va_Adresse2_IP_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao80: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80 retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}


	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_Port2,"@va_Port2_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao80: asql_ctparam_int_input(int) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80 retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}


	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)vl_Adresse2_IPE,"@va_Adresse2_IPE_in"))!= CS_SUCCEED)
        {
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao80: asql_ctparam_char_input() a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80 retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
	
	
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&vl_Port2E,"@va_Port2E_in"))!= CS_SUCCEED)
	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao80: asql_ctparam_int_input(int) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80 retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	
	Tab_donnees2[0].pt_value = (CS_VOID *)va_Resultat_out;
	
	if ((retcode = asql_ctparam_int_output(cmd,(CS_INT *)va_Resultat_out,"@va_Resultat_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzao80: asql_ctparam_int_output(Resultat) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80_Ajouter_DAI retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	


	/*A
	** Envoie de la commande au Sql server
	*/
	
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao80: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80_Ajouter_Aire retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_INFO,"xzao80: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao80 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao80 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzao80 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80_Ajouter_DAI retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   free(Tab_donnees2);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzao80 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80_Ajouter_DAI retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao80: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80_Ajouter_DAI retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	   					free(Tab_donnees2);
	   					return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzao80 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao80 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzao80 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao80: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80_Ajouter_DAI retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao80: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80_Ajouter_DAI retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				free(Tab_donnees2);
	   				return (XDC_NOK);
				}
	}

	
	
	/*!
	** Retourne le resultat et les parametres de sorties renseignes
	** retourne -205 en sortie de boucle
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO80_Ajouter_DAI retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
 	free(Tab_donnees2);
	return(XDC_OK);
}




