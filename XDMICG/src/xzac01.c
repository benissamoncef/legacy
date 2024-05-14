/*E*/
/*X  Fichier : @(#)xzac01.c	1.2      Release : 1.2        Date : 03/26/07
------------------------------------------------------
*  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac01.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* PNI	23/08/06	creation
* PFE   17/10/06	DEM 591 : on supprime les free si pas de alloc pb en version HPUX 11.11
* VR    04/11/11        Ajout destination (DEM 1014)
* PNI	29/10/20	Ajout du parametre manquant va_DestinationNoeud_in a l'appel de XZAC01 SAE-185
------------------------------------------------------*/

/* fichiers inclus */

#include "xzac.h"
#include "xzac01.h"

static char *version = "@(#)xzac01.c	1.2 1.2 03/26/07 : xzac01" ;


/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAC01SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAC01_Commande_PMV ( XDY_Entier    va_NumEvt_in,
                                 XDY_Octet     va_CleEvt_in,
                                 XDY_Operateur va_Operateur_in,
                                 XDY_Eqt       va_NumEqt_in,
                                 XDY_Horodate  va_HeureLancement_in,
                                 XDY_Octet      va_Site_in,
                                 char      	va_Ligne1_in[20],
                                 char      	va_Ligne2_in[20],
                                 char      	va_Ligne3_in[20],
                                 char      	va_Alternat1_in[20],
                                 char      	va_Alternat2_in[20],
                                 char      	va_Alternat3_in[20],
                                 XDY_Octet     	va_Clignotement1_in,
                                 XDY_Octet     	va_Clignotement2_in,
                                 XDY_Octet     	va_Clignotement3_in,
                                 XDY_Octet     	va_Flash_in,
                                 char      	va_TypePicto_in[10],
                                 char      	va_TextePicto_in[50],
                                 XDY_Octet     	va_ClignotementPicto_in,
                                 XDY_Octet     va_Priorite_in,
                                 XDY_Entier    va_DistanceEvt_in,
                                 char      	va_LigneDir_in[20],
                                 char      	va_AlternatDir_in[20],
				 XDY_Mot	va_Destination_in,
				 XDY_Mot	va_DestinationAlternat_in,
				 XDY_Octet	va_DepassSeuil_in,
                                 XDY_Octet      va_site_origine_in,
				 XDY_Mot                va_DestinationNoeud_in)

/*X*/
/*-----------------------------------------------------
* Service rendu
* commande de BAF
* 
* Sequence d'appel
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
* 
------------------------------------------------*/


{

	static char *version = "@(#)xzac01.c	1.2 1.2 03/26/07 :  XZAC01_Commande_PMV" ;

	CS_CHAR *rpc_name = XZACC_XZAC01_RPC_NAME;
	CS_CONTEXT      *context;
	CS_CONNECTION   *connection=XZAGV_Connexion;
	CS_COMMAND	*cmd;
	CS_DATAFMT      datafmt;
	CS_RETCODE      retcode = 0;
	CS_INT          res_type;
	CS_SMALLINT     msg_id;
	int 		num_Tab_donnees2 = XZACC_XZAC01_NB_PARM_RETURN;
	tg_Colonne_donnees2 *Tab_donnees2;
	XDY_Entier 	SPstatus;
	CS_DATETIME	vl_Horodate;
	
	/*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/
	
	if ((retcode = asql_date_Ux2Sybase(va_HeureLancement_in,&vl_Horodate))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzae20: asql_date_Ux2Sybase(va_HeureLancement_in,&vl_Horodate) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMVretourne %d", retcode);
		return (XDC_NOK);
	}
	
	/* si texte picto est vide alors on met un blan */
	if ( va_TypePicto_in[0]==(char)0 ) strcpy(va_TypePicto_in," ");
	if ( va_TextePicto_in[0]==(char)0 ) strcpy(va_TextePicto_in," ");

	XZST_03EcritureTrace( XZSTC_WARNING,
		"IN : XZAC01_Commande_PMV (%d/%d) ope(%d) eqt(%d) horo(%lf) site (%d) l1(%s) l2(%s) l3(%s) a1(%s) a2(%s) a3(%s) c1(%d) c2(%d) c3(%d) flash(%d) picto(%s) tpic(%s) cp(%d) ",
				va_NumEvt_in,
				va_CleEvt_in,va_Operateur_in,
				va_NumEqt_in,va_HeureLancement_in,
				va_Site_in,va_Ligne1_in,va_Ligne2_in,va_Ligne3_in,va_Alternat1_in,va_Alternat2_in,va_Alternat3_in,
va_Clignotement1_in,va_Clignotement2_in,va_Clignotement3_in,va_Flash_in,va_TypePicto_in,va_TextePicto_in,va_ClignotementPicto_in
);
			
	/*A
	** Initialisation de la commande				
	*/

	if ((retcode = asql_cmd_alloc(connection, &cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
	/*PFE	
		free(Tab_donnees2);
	*/
	return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((retcode = ct_command(cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
		asql_clean_cmd(connection, cmd);
	/* PFE
	   free(Tab_donnees2);
	*/
	return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

        if ((retcode = asql_ctparam_date_input(cmd,(CS_DATETIME *)&vl_Horodate,"@va_Horodate_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_date_input(Horodate) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	/* PFE
	   free(Tab_donnees2);
	*/
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_NumEvt_in,"@va_NumEvt_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_int_input(NumEvt) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
        /* PFE
		   free(Tab_donnees2);
			   */
			   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_CleEvt_in,"@va_CleEvt_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_tinyint_input(CleEvt) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	  /* PFE
	  free(Tab_donnees2);
	   */
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&va_Operateur_in,"@va_Operateur_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_smallint_input(va_Operateur_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
          /* PFE
	  free(Tab_donnees2);
	  */
	  return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&va_NumEqt_in,"@va_NumEqt_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_smallint_input(va_NumEqt_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
           /* PFE
	     free(Tab_donnees2);
	   */
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Site_in,"@va_Site_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_tinyint_input(va_Site_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
          /* PFE
	  free(Tab_donnees2);
	  */
	  return (XDC_NOK);
	}
        
        
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Ligne1_in,"@va_Ligne1_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_char_input(va_Ligne1_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
          /* PFE
	  free(Tab_donnees2);
	  */
	  return (XDC_NOK);
	}
 	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Ligne2_in,"@va_Ligne2_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_char_input(va_Ligne2_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
          /* PFE
	  free(Tab_donnees2);
	  */
	  return (XDC_NOK);
	}
 	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Ligne3_in,"@va_Ligne3_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_char_input(va_Ligne3_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   /* PFE
	   free(Tab_donnees2);
	   */ 
	   return (XDC_NOK);
	}
 	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Alternat1_in,"@va_Alternat1_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_char_input(va_Alternat1_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   /* PFE 
	   free(Tab_donnees2);
	   */
	   return (XDC_NOK);
	}
  	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Alternat2_in,"@va_Alternat2_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_char_input(va_Alternat2_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
          /* PFE
	  free(Tab_donnees2);
	  */
	  return (XDC_NOK);
	}
    
 	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_Alternat3_in,"@va_Alternat3_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_char_input(va_Alternat3_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
          /* PFE
	  free(Tab_donnees2);
	  */
	  return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Clignotement1_in,"@va_Clignotement1_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_tinyint_input(va_Clignotement1_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
          /* PFE
	  free(Tab_donnees2);
	  */
	  return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Clignotement2_in,"@va_Clignotement2_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_tinyint_input(va_Clignotement2_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Clignotement3_in,"@va_Clignotement3_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_tinyint_input(va_Clignotement3_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Flash_in,"@va_Flash_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_tinyint_input(va_Flash_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

  	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_TypePicto_in,"@va_TypePicto_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_char_input(va_TypePicto_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
    
  	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_TextePicto_in,"@va_TextePicto_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_char_input(va_TextePicto_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
    
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_ClignotementPicto_in,"@va_ClignotementPicto_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_tinyint_input(va_ClignotementPicto_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_Priorite_in,"@va_Priorite_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_tinyint_input(va_Priorite_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	if ((retcode = asql_ctparam_int_input(cmd,(CS_INT *)&va_DistanceEvt_in,"@va_DistanceEvt_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_int_input(va_DistanceEvt_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
       
	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_LigneDir_in,"@va_LigneDir_in"))!= CS_SUCCEED)
	{	
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_char_input(va_LigneDir_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

  	if ((retcode = asql_ctparam_char_input(cmd,(CS_CHAR *)va_AlternatDir_in,"@va_AlternatDir_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_char_input(va_AlternatDir_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
    
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&va_Destination_in,"@va_Destination_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_smallint_input(va_Destination_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&va_DestinationAlternat_in,"@va_DestinationAlternat_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_smallint_input(va_DestinationAlternat_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_DepassSeuil_in,"@va_DepassSeuil_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_tinyint_input(va_DepassSeuil_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}

	if ((retcode = asql_ctparam_tinyint_input(cmd,(CS_TINYINT *)&va_site_origine_in,"@va_site_origine_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_tinyint_input(va_site_origine_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}
        
	
	if ((retcode = asql_ctparam_smallint_input(cmd,(CS_SMALLINT *)&va_DestinationNoeud_in,"@va_DestinationNoeud_in"))!= CS_SUCCEED)
	
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: asql_ctparam_smallint_input(va_DestinationNoeud_in) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
	   asql_clean_cmd(connection, cmd);
	   return (XDC_NOK);
	}


	
	
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
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
						retcode = asql_fetch_data (cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"xzac01: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						retcode = asql_fetch_param_result (cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzac01 : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						retcode = asql_fetch (cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzac01 : CS_STATUS_RESULT = %ld", retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzac01 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne 1");
 						   asql_clean_cmd(connection, cmd);
	   					   return (XDC_NOK);
						}
						break;
				}
				
				if (retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_WARNING,"xzac01 : asql_fetch_* a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
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
						XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
						asql_clean_cmd(connection, cmd);
	   					return (XDC_NOK);
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"xzac01 : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", msg_id);
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzac01 : CS_CMD_SUCCEED \n");
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"xzac01 : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzac01: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
					asql_clean_cmd(connection, cmd);
	   				return (XDC_NOK);
				}
	}

	/*A
	** Retourne le resultat et les parametres de sorties renseignes
	*/
 	
 	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAC01_Commande_PMV retourne %d", retcode);
 	asql_clean_cmd(connection, cmd);
	return(XDC_OK);
}




