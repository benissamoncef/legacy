/*E*/
/* Fichier : $Id: xzaz01.c,v 1.3 2020/11/03 17:57:27 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2020/11/03 17:57:27 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE XZAE * FICHIER xzaz01.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module d'interface pour satir
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.       17/01/95     : Creation
* C.T.       06/03/95     : Modif interface fonction (V 1.2)
* LCL        22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
------------------------------------------------------*/

/* fichiers inclus */

#include "xzaz01.h"

static char *version = "$Id: xzaz01.c,v 1.3 2020/11/03 17:57:27 pc2dpdy Exp $ XZAZ01_Ecrire_Travaux_Satir";

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ecrire fiche travaux satir
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAZ01_Ecrire_Travaux_Satir ( XZAZT_Travaux	va_Travaux, XDY_Evt *va_Evt_out)
				

/*
* ARGUMENTS EN ENTREE :
*
* XZAZT_Travaux		va_Travaux
*
* ARGUMENTS EN SORTIE :
*
* XDY_Evt		va_Evt_out
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*  XDC_ARG_INV	: argument invalide
*
* CONDITION D'UTILISATION :
* date de debut de travaux prevus doit etre superieure a
* la date actuelle plus 24 heures
*
* INTERFACE :
*
* Appelle XZAE08SP, XZAE61SP, XZAE67SP, XZAE11SP
*
* MECANISMES :
*
------------------------------------------------------*/
{
	static char *version = "$Id: xzaz01.c,v 1.3 2020/11/03 17:57:27 pc2dpdy Exp $ XZAZ01_Ecrire_Travaux_Satir";
	
	CS_CHAR 	*vl_RpcName =XZAZC_XZAZ01_RPC_NAME;
	CS_CONNECTION   *vl_Connection=XZAGV_Connexion;
	CS_COMMAND	*vl_Cmd;
	CS_RETCODE      vl_Retcode;
	CS_INT          vl_ResType;
	CS_SMALLINT     vl_MsgId;
	tg_Colonne_donnees2 *Tab_donnees2;
	int 		num_Tab_donnees2 = XZAZC_XZAZ01_NB_PARM_RETURN;
	int		i;
	XDY_Entier	SPstatus;
	XDY_NomSite     pl_NomSite;	
	
	/*A
	** Definition des variables intermediaires pour la conversion des dates au format Sybase
	*/
	CS_DATETIME	vl_DebutPrevu;
	CS_DATETIME	vl_Debut;
	CS_DATETIME	vl_FinPrevue;
	CS_DATETIME	vl_HoroDebC;
	CS_DATETIME	vl_HoroFinC;
	

	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"IN : XZAZ01_Ecrire_Travaux_Satir : Numero=%d Cle=%d \n DebutPrevu=%lf Debut=%lf \n FinPrevue=%lf \n Autoroute=%d \n PR=%ld Sens=%d \n VR=%d VM2=%d VM1=%d \n VL=%d BAU=%d VR_I=%d \n VM2_I=%d VM1_I=%d VL_I=%d BAU_I=%d Texte=%s \n Nature=%d Type%d  Continu=%d \n HoroDebC=%lf HoroFinC=%lf Mobile=%d \n PRqueueM=%ld PRteteM=%ld \n SensM=%d PRtete=%ld VoiesRet=%d\n",
			va_Travaux.Numero, va_Travaux.Cle, 
			va_Travaux.DebutPrevu, va_Travaux.Debut, 
			va_Travaux.FinPrevue, va_Travaux.Autoroute,
			va_Travaux.PR, va_Travaux.Sens, va_Travaux.VR,
			va_Travaux.VM2, va_Travaux.VM1, va_Travaux.VL, 
			va_Travaux.BAU, va_Travaux.VR_I, va_Travaux.VM2_I,
			va_Travaux.VM1_I, va_Travaux.VL_I, va_Travaux.BAU_I,
			va_Travaux.Texte, va_Travaux.Nature, va_Travaux.Type,
			va_Travaux.Continu, va_Travaux.HoroDebC, va_Travaux.HoroFinC,
			va_Travaux.Mobile, va_Travaux.PRqueueM,
			va_Travaux.PRteteM, va_Travaux.SensM, va_Travaux.PRtete,
			va_Travaux.VoiesRet);

        /*A
        ** Recuperation du site de la machine
        */
        if ( (vl_Retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
        {
                XZST_03EcritureTrace( XZSTC_WARNING, "XZAZ01 : Recuperation du Nom du site impossible .");
                return( XDC_NOK);
        }
        /*
        ** Affichage Trace
        */
        XZST_03EcritureTrace( XZSTC_DEBUG1, "XZAZ01 : Recuperation du Nom du site %s.",pl_NomSite);
	
        /*A
	** Conversion des chaines de caracteres en entree du format Unix au format Sybase
	*/
	
	if ((vl_Retcode = asql_date_Ux2Sybase(va_Travaux.DebutPrevu,&vl_DebutPrevu))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_date_Ux2Sybase(DebutPrevu,&vl_Horodate_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
					vl_Retcode);
		return (XDC_NOK);
	}
	
	if ((vl_Retcode = asql_date_Ux2Sybase(va_Travaux.Debut,&vl_Debut))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_date_Ux2Sybase(Debut,&vl_Horodate_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
					vl_Retcode);
		return (XDC_NOK);
	}
	
	if ((vl_Retcode = asql_date_Ux2Sybase(va_Travaux.FinPrevue,&vl_FinPrevue))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_date_Ux2Sybase(FinPrevue,&vl_Horodate_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
					vl_Retcode);
		return (XDC_NOK);
	}
	
	if ((vl_Retcode = asql_date_Ux2Sybase(va_Travaux.HoroDebC,&vl_HoroDebC))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_date_Ux2Sybase(HoroDebC,&vl_Horodate_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
					vl_Retcode);
		return (XDC_NOK);
	}
	
	if ((vl_Retcode = asql_date_Ux2Sybase(va_Travaux.HoroFinC,&vl_HoroFinC))!= XDC_OK)
	{

		XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_date_Ux2Sybase(HoroFinC,&vl_Horodate_in) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
					vl_Retcode);
		return (XDC_NOK);
	}
	
        if ((vl_Retcode = asql_ctparam_char_input(vl_Cmd,(CS_CHAR *)pl_NomSite,"@va_NomSiteLocal_in"))!= CS_SUCCEED)

        {
           XZST_03EcritureTrace(XZSTC_WARNING,"XZAZ01 : asql_ctparam_char_input(va_site_origine_in) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", vl_Retcode);
           asql_clean_cmd(vl_Connection, vl_Cmd);
           return (XDC_NOK);
        }

	
	/*A
	**  Allocation de la structure permettant la recuperation des parametres
	*/
		
	Tab_donnees2 = (tg_Colonne_donnees2 *) malloc (sizeof(tg_Colonne_donnees2)*num_Tab_donnees2);

	/*A
	** Initialisation de la commande				
	*/

	if ((vl_Retcode = asql_cmd_alloc(vl_Connection, &vl_Cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
					vl_Retcode);
		asql_clean_cmd(vl_Connection, vl_Cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	
	if ((vl_Retcode = ct_command(vl_Cmd, CS_RPC_CMD, vl_RpcName, 
				CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: ct_command() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
					vl_Retcode);
		asql_clean_cmd(vl_Connection, vl_Cmd);
		free(Tab_donnees2);
		return (XDC_NOK);
	}

	/*A 
	** Initialisation de la structure pour chacun des parametres d I/O	
	** A faire passer a la rpc.
	*/

	if ((vl_Retcode = asql_ctparam_int_input(vl_Cmd,(CS_INT *)&va_Travaux.Numero,
						"@va_NumEvt_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_int_input(Numero) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	if ((vl_Retcode = asql_ctparam_tinyint_input(vl_Cmd,(CS_TINYINT *)&va_Travaux.Cle,
						"@va_CleEvt_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_tinyint_input(Cle) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	if ((vl_Retcode = asql_ctparam_date_input(vl_Cmd,(CS_DATETIME *)&vl_DebutPrevu,
						"@va_DebutPrevu_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_date_input(Horodate) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	if ((vl_Retcode = asql_ctparam_date_input(vl_Cmd,(CS_DATETIME *)&vl_Debut,
						"@va_Debut_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_date_input(Horodate) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	if ((vl_Retcode = asql_ctparam_date_input(vl_Cmd,(CS_DATETIME *)&vl_FinPrevue,
						"@va_FinPrevue_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_date_input(Horodate) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((vl_Retcode = asql_ctparam_tinyint_input(vl_Cmd,(CS_TINYINT *)&va_Travaux.Autoroute,
						"@va_Autoroute_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_int_input(Autoroute) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((vl_Retcode = asql_ctparam_int_input(vl_Cmd,(CS_INT *)&va_Travaux.PR,
						"@va_PR_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_int_input(PR) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((vl_Retcode = asql_ctparam_tinyint_input(vl_Cmd,(CS_TINYINT *)&va_Travaux.Sens,
						"@va_Sens_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_tinyint_input(Sens) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((vl_Retcode = asql_ctparam_tinyint_input(vl_Cmd,(CS_TINYINT *)&va_Travaux.VR,
						"@va_VR_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_tinyint_input(VR) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((vl_Retcode = asql_ctparam_tinyint_input(vl_Cmd,(CS_TINYINT *)&va_Travaux.VM2,
						"@va_VM2_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_tinyint_input(VM2) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((vl_Retcode = asql_ctparam_tinyint_input(vl_Cmd,(CS_TINYINT *)&va_Travaux.VM1,
						"@va_VM1_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_tinyint_input(VM1) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((vl_Retcode = asql_ctparam_tinyint_input(vl_Cmd,(CS_TINYINT *)&va_Travaux.VL,
						"@va_VL_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_tinyint_input(VL) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((vl_Retcode = asql_ctparam_tinyint_input(vl_Cmd,(CS_TINYINT *)&va_Travaux.BAU,
						"@va_BAU_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_tinyint_input(BAU) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((vl_Retcode = asql_ctparam_tinyint_input(vl_Cmd,(CS_TINYINT *)&va_Travaux.VR_I,
						"@va_VR_I_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_tinyint_input(VR_I) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((vl_Retcode = asql_ctparam_tinyint_input(vl_Cmd,(CS_TINYINT *)&va_Travaux.VM2_I,
						"@va_VM2_I_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_tinyint_input(VM2_I) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((vl_Retcode = asql_ctparam_tinyint_input(vl_Cmd,(CS_TINYINT *)&va_Travaux.VM1_I,
						"@va_VM1_I_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_tinyint_input(VM1_I) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((vl_Retcode = asql_ctparam_tinyint_input(vl_Cmd,(CS_TINYINT *)&va_Travaux.VL_I,
						"@va_VL_I_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_tinyint_input(VL_I) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((vl_Retcode = asql_ctparam_tinyint_input(vl_Cmd,(CS_TINYINT *)&va_Travaux.BAU_I,
						"@va_BAU_I_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_tinyint_input(BAU_I) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((vl_Retcode = asql_ctparam_char_input(vl_Cmd,(CS_CHAR *)&va_Travaux.Texte,
						"@va_Texte_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_char_input(Texte) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((vl_Retcode = asql_ctparam_bit_input(vl_Cmd,(CS_BIT *)&va_Travaux.Nature,
						"@va_Nature_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_bit_input(Nature) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((vl_Retcode = asql_ctparam_tinyint_input(vl_Cmd,(CS_TINYINT *)&va_Travaux.Type,
						"@va_Type_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_char_input(Type) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((vl_Retcode = asql_ctparam_bit_input(vl_Cmd,(CS_BIT *)&va_Travaux.Continu,
						"@va_Continu_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_bit_input(Continu) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((vl_Retcode = asql_ctparam_date_input(vl_Cmd,(CS_DATETIME *)&vl_HoroDebC,
						"@va_HoroDebC_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_date_input(HoroDebC) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((vl_Retcode = asql_ctparam_date_input(vl_Cmd,(CS_DATETIME *)&vl_HoroFinC,
						"@va_HoroFinC_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_date_input(HoroFinC) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	if ((vl_Retcode = asql_ctparam_bit_input(vl_Cmd,(CS_BIT *)&va_Travaux.Mobile,
						"@va_Mobile_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_bit_input(Mobile) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((vl_Retcode = asql_ctparam_int_input(vl_Cmd,(CS_INT *)&va_Travaux.PRqueueM,
						"@va_PRqueueM_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_int_input(PRqueueM) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((vl_Retcode = asql_ctparam_int_input(vl_Cmd,(CS_INT *)&va_Travaux.PRteteM,
						"@va_PRteteM_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_int_input(PRteteM) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((vl_Retcode = asql_ctparam_tinyint_input(vl_Cmd,(CS_TINYINT *)&va_Travaux.SensM,
						"@va_SensM_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_tinyint_input(SensM) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((vl_Retcode = asql_ctparam_int_input(vl_Cmd,(CS_INT *)&va_Travaux.PRtete,
						"@va_PRtete_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_int_input(PRtete) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	if ((vl_Retcode = asql_ctparam_bit_input(vl_Cmd,(CS_BIT *)&va_Travaux.VoiesRet,
						"@va_VoiesRet_in"))!= CS_SUCCEED)
	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_bit_input(VoiesRet) a echoue");
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
				vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}
        
	Tab_donnees2[0].pt_value = (CS_VOID *)&(va_Evt_out->numero);
	
	if ((vl_Retcode = asql_ctparam_int_output(vl_Cmd,(CS_INT *)&(va_Evt_out->numero),"@va_NumEvt_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_int_output(NumEvt) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	Tab_donnees2[1].pt_value = (CS_VOID *)&(va_Evt_out->cle);
	
	if ((vl_Retcode = asql_ctparam_tinyint_output(vl_Cmd,(CS_TINYINT *)&(va_Evt_out->cle),"@va_CleEvt_out")) != CS_SUCCEED)	

	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_ctparam_tinyint_output(CleEvt) a echoue "); 
	   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", vl_Retcode);
	   asql_clean_cmd(vl_Connection, vl_Cmd);
	   free(Tab_donnees2);
	   return (XDC_NOK);
	}

	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_Cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", 
					vl_Retcode);
		asql_clean_cmd(vl_Connection, vl_Cmd);
	   	free(Tab_donnees2);
		return (XDC_NOK);
	}
	
	
	
	/*A
	** Traitement du resultat
	*/

	while ((vl_Retcode = ct_results(vl_Cmd, &vl_ResType)) == CS_SUCCEED)
	{
		switch ((int)vl_ResType)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/*B
				** Impression de l entete en fonction du type de resultat.
				*/
				switch ((int)vl_ResType)
				{
					case  CS_ROW_RESULT:
						XZST_03EcritureTrace(XZSTC_INFO,"xzaz01: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_Retcode = asql_fetch_param_result (vl_Cmd , num_Tab_donnees2, Tab_donnees2);
						XZST_03EcritureTrace(XZSTC_INFO,"xzaz01 : PARAMETER RESULTS\n");
						if (vl_Retcode != CS_SUCCEED)
						{
							XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_fetch_param_result* a echoue");
							XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", vl_Retcode);
							asql_clean_cmd(vl_Connection, vl_Cmd);
	   						free(Tab_donnees2);
							return (XDC_NOK);
						}
						break;

					case  CS_STATUS_RESULT:
						vl_Retcode = asql_fetch (vl_Cmd, &SPstatus);
						XZST_03EcritureTrace(XZSTC_INFO,"xzaz01 : CS_STATUS_RESULT = %ld", vl_Retcode);
						XZST_03EcritureTrace(XZSTC_INFO,"xzaz01 : STATUS RESULT PROCEDURE STOCKEE = %ld", SPstatus);
						
						if (SPstatus != XDC_OK)
						{
						   XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", SPstatus);
 						   asql_clean_cmd(vl_Connection, vl_Cmd);
	   					   free(Tab_donnees2);
						   return (SPstatus);
						}
						break;
				}
				
				
				if (vl_Retcode != CS_SUCCEED)
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: asql_fetch_data* a echoue");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", vl_Retcode);
					asql_clean_cmd(vl_Connection, vl_Cmd);
	   				free(Tab_donnees2);
					return (XDC_NOK);
				}
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_Retcode = ct_res_info(vl_Cmd, CS_MSGTYPE,(CS_VOID *)&vl_MsgId, CS_UNUSED, NULL);
					if (vl_Retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", vl_Retcode);
						asql_clean_cmd(vl_Connection, vl_Cmd);
	   					free(Tab_donnees2);
						return (XDC_NOK);
					}
					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une ereeur en executant notre commande.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", vl_Retcode);
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"xzaz01: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d", vl_Retcode);
					asql_clean_cmd(vl_Connection, vl_Cmd);
	   				free(Tab_donnees2);
					return (XDC_NOK);
				}
	}
	
	
	XZST_03EcritureTrace(	XZSTC_INTERFACE,
			"OUT : XZAZ01_Ecrire_Travaux_Satir retourne %d\n", SPstatus);
			
 	
 	asql_clean_cmd(vl_Connection, vl_Cmd);
	free(Tab_donnees2);

	return(XDC_OK);
}

