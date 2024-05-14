/*E*/
/*Fichier : $Id: asrv_mrp.c,v 1.9 2021/04/23 11:02:34 gesconf Exp $      Release : $Revision: 1.9 $        Date : $Date: 2021/04/23 11:02:34 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE MRP * FICHIER asrv_mrp.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Module contenant des registered Procedures.
*	Les tests sont contenus dans BASEDD/tests: 
*	Lorsque l'Open Serveur est lance, on peut lancer amrp05.sh
*	et recuperer dans amrp05.results les resultats renvoyes au SQL Serveur.
*
*****************************
*
*	amrp05 RP affaisant appel a xzit07 (Pour l'instant on ne s'en sert pas)
*
*****************************
*
*	amrp07 RP faisant appel a xzis01,xzit07 et xzis02
*	A noter que l on recoie une date de format Sybase que l on transforme au format UX
*	pour la passer en argument a xzis02().
*
*****************************
------------------------------------------------------
* HISTORIQUE :
*
* mercier	21 Sep 1994	: Creation
------------------------------------------------------
* mercier	29 Sep 1994	: Modification
*	Rajoue des flags  static char *version = "$Id: asrv_mrp.c,v 1.9 2021/04/23 11:02:34 gesconf Exp $ : asrv_mrp*" ;
* JMG	02/10/09	: FACTIS DEM914
------------------------------------------------------*/

/* fichiers inclus */

#include "asrv_mrp.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: asrv_mrp.c,v 1.9 2021/04/23 11:02:34 gesconf Exp $ : asrv_mrp" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : Pour l instant on ne s en sert pas
*  RP affaisant appel a xzit07
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE amrp05(SRV_PROC        *pa_srvproc)

/*
* ARGUMENTS EN ENTREE :
*
*	Pointeur vers une structure de controle thread interne
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
* CODE RETOUR : 
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage PMV.
*
*
* FONCTION 
*
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: asrv_mrp.c,v 1.9 2021/04/23 11:02:34 gesconf Exp $ : asrv_mrp05" ;
	
	int		vl_retcode = XDC_OK;	/* Code de retour		*/
	
	
        CS_BOOL         vl_ok;
        
        CS_INT          vl_numparams;
              				/* Permet de recuperer le nbre  */
                                        /* de param passes a la rpc     */
       

        CS_SERVERMSG    vl_msg;        	/* structure permettant de ren- */
                                        /* voyer un message d erreur au */
                                        /* client.                      */

        CS_CONTEXT      *pl_cp;         /* Structure contenant les infos*/
                                        /* de localisation du serveur   */

        CS_INT          vl_status;      /* 
        				** Contient de le vl_status du srv 
        				** apres reception de la demande
        				*/

        CS_INT		vl_Eqt;		/* variable pour recuperer le   
        				** param venant du sql serveur
        				*/
	
	CS_TINYINT	vl_nump;
	
	CS_INT		vl_param_no=1;

	
	
        /* 
        ** Variables specifiques a l appel des services externes	
        */
        	
	XZETT_CMD_SEQ_NIC   	vl_Commande_SIG_NIC;	/* Commande a effectuee	*/
	
	CS_INT          	vl_NoCMD_In;		/* Numero de la commande*/
	XDY_NomMachine  	vl_NomMachine_In;	/* nom de la machine	*/
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : amrp05 : debut d execution ");
	
	/*A 
	**  initialisation du vl_status a OK pour fin de transaction
        **  Si on oublie vl_status, on peut bloquer la transaction
        */
        vl_status = SRV_DONE_FINAL;

        /* 
        ** initialisation de vl_ok a TRUE                                  
        */
        vl_ok = CS_TRUE;

       
        /*A 
        ** verification du contenu du parametre venant du client	
        */

        /*A 
        ** recupere le nombre de parametres et on teste                
        */
        if (srv_numparams(pa_srvproc, &vl_numparams) != CS_SUCCEED)
                vl_ok = CS_FALSE;

        /*A 
        **  On verifie qu'il y a bien le nb de param attendu,
        **  passe a la rpc         
        */
        
        if (vl_numparams != ca_amrp05_nb_param)
        {
        	XZST_03EcritureTrace(XZSTC_FATAL,"amrp05: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_acrp01_nb_param);
        	vl_ok = CS_FALSE;
        }
        
        
        /*A 
        **  Pas de Transfert des parametres dans les variables locales au
        **  programme acr pas de parametres a recevoir.
        */


	if (!vl_ok )
        {

        	/*A
        	** Initialise le message d erreur a renvoyer si un probleme est
        	** apparu.
        	*/
        	
       		/* 
       		** mise a zero du vl_buffer de message 
       		*/
    		srv_bzero(&vl_msg, sizeof(CS_SERVERMSG));

        	/* 
        	** indicateur du type de message et de sa gravite       
        	*/
        	
        	vl_msg.msgnumber = RPC_FAIL;
        	vl_msg.severity = 10;
        	
        	/*A 
        	** Initialisation du message pour le client             
        	*/
        	
        	(CS_VOID)sprintf(vl_msg.text,"amrp05 : message non correct.\n");
        	vl_msg.textlen = strlen(vl_msg.text);

        	/* 
        	** Allocation de la structure de contexte               
        	*/

        	if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
        	{
        		XZST_03EcritureTrace(XZSTC_FATAL,"amrp05 : cs_ctx_alloc() a echoue.\n");
        	}
        	else
        	{
              		/* 
              		** Recuperation du nom du serveur               
              		*/
                	(CS_VOID)srv_props(pl_cp, CS_GET, SRV_S_SERVERNAME,
                	(CS_VOID *)vl_msg.svrname, sizeof(vl_msg.svrname),&vl_msg.svrnlen);
        	}
        	/*B 
        	**  Envoi du message d erreur au client                  
        	*/
        	if (srv_sendinfo(pa_srvproc, &vl_msg, (CS_INT)0) != CS_SUCCEED)
        	
                        XZST_03EcritureTrace(XZSTC_FATAL,"amrp05 : srv_sendinfo() a echou≈.\n");
                else 
                	XZST_03EcritureTrace(XZSTC_SGBD,"amrp05 : srv_sendinfo a envoye un message d erreur\n");

		/*B Initialisation du vl_status de la requete par le serveur*/
		
                vl_status |= SRV_DONE_ERROR;

                /*B Envoi du vl_status de la requete indiquant une erreur . */
                if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)
                        
                        XZST_03EcritureTrace(XZSTC_FATAL,"amrp05 : srv_sendstatus() a echou≈.\n");
                else 
                	XZST_03EcritureTrace(XZSTC_SGBD,"amrp05 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);      
        }
        
        
        /*A 
        ** Envoi au client la confirmation du traitement de la demande  
        */
        if (vl_ok)
        {
        
        	/*A
        	** Execution de la requete en elle meme : Affichage du vl_msg
        	*/
	
		vl_retcode = XZIT07_Modif_Liste_Alarmes ();
		if (vl_retcode != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_FATAL,"amrp05 : XZIT07_Modif_Liste_Alarmes a ≈chou≈ : code =%d",vl_retcode);
			
		}
		
		
		XZST_03EcritureTrace(XZSTC_SGBD," Execution de \n\t\tXZIT07_Modif_Liste_Alarmes () a reussi");
		/*A 
		**  Initialisation du vl_status a  vl_retcode indiquant le succes ou l 'echec du
        	**  traitement de la requete.
        	*/
        	if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
                {
                	vl_status |= SRV_DONE_ERROR;
                	XZST_03EcritureTrace(XZSTC_FATAL,"amrp05 : srv_sendstatus() a echoue.\n");
                }
        	else
                {
                	XZST_03EcritureTrace(XZSTC_SGBD,"amrp05 : srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
                }

 		/*A 
 		** Envoi du paquet final au client.                             
 		*/
        	if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
                 	XZST_03EcritureTrace(XZSTC_FATAL,"amrp05 : srv_senddone() a echou≈.\n");

 		else XZST_03EcritureTrace(XZSTC_SGBD,"amrp05 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
	}
	/*A
	**	Scrute les messages du RTserver afin de declancher les CallBacks
	*/	
	asrv_lire_RTSRV_MSG();
	

	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : amrp05 : execution reussie");
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : amrp05 : Fin d execution ");
        return (CS_SUCCEED);

}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*  RP affaisant appel a xzis01,xzit07 et xzis02
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE amrp07(SRV_PROC        *pa_srvproc)

/*
* ARGUMENTS EN ENTREE :
*
*	Pointeur vers une structure de controle thread interne
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
* CODE RETOUR : 
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage PMV.
*
*
* FONCTION 
*	Passe les parametres »
*	XZIS01_Positionner_Indicateur_Alarme : Construction du message XDM_Positionner_Indicateur_Alarme
* et envoi dans le datagroupe XDG_Positionner_Indicateur_Alarme.
*	XZIS02_Raf_Etat_Eqt : Construction du message XDM_Etat_Dispo_Eqt et envoi dans le datagroup
* XDG_Etat_Dispo_Eqt
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: asrv_mrp.c,v 1.9 2021/04/23 11:02:34 gesconf Exp $ : asrv_mrp07" ;
	int		vl_retcode = XDC_OK;	/* Code de retour		*/
	
	
        CS_BOOL         vl_ok;
        
        CS_INT          vl_numparams;
              				/* Permet de recuperer le nbre  */
                                        /* de param passes a la rpc     */
       

        CS_SERVERMSG    vl_msg;        	/* structure permettant de ren- */
                                        /* voyer un message d erreur au */
                                        /* client.                      */

        CS_CONTEXT      *pl_cp;         /* Structure contenant les infos*/
                                        /* de localisation du serveur   */

        CS_INT          vl_status;      /* 
        				** Contient de le vl_status du srv 
        				** apres reception de la demande
        				*/

        CS_INT		vl_Eqt;		/* variable pour recuperer le   
        				** param venant du sql serveur
        				*/
	
	CS_TINYINT	vl_nump;
	
	CS_INT		vl_param_no=1;

	
	
	
        /* 
        ** Variables specifiques a l appel des services externes	
        */
        CS_TINYINT	vl_TypeEqt;	/* Type de l'≈quipement » rafra—chir XDY_TypeEqt */
        
	CS_SMALLINT	vl_NumEqt;	/* Num≈ro de l'≈quipement XDY_Eqt*/
	CS_INT		vl_Etat_Eqt;	/* Etat de l'≈quipement : XDY_Etat_Eqt*/
	
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : amrp07 : debut d execution ");
	
	/*A 
	**  initialisation du vl_status a OK pour fin de transaction
        **  Si on oublie vl_status, on peut bloquer la transaction
        */
        vl_status = SRV_DONE_FINAL;

        /* 
        ** initialisation de vl_ok a TRUE                                  
        */
        vl_ok = CS_TRUE;

       
        /*A 
        ** verification du contenu du parametre venant du client	
        */

        /*A 
        ** recupere le nombre de parametres et on teste                
        */
        if (srv_numparams(pa_srvproc, &vl_numparams) != CS_SUCCEED)
                vl_ok = CS_FALSE;

        /*A 
        **  On verifie qu'il y a bien le nb de param attendu,
        **  passe a la rpc         
        */
        
        if (vl_numparams != ca_amrp07_nb_param)
        {
        	XZST_03EcritureTrace(XZSTC_FATAL,"amrp07: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_acrp01_nb_param);
        	vl_ok = CS_FALSE;
        }
        
        /* 
       	** 1 er champ amrp07: vl_TypeEqt de type  XDY_Octet : tinyint
       	*/
       	
	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&vl_TypeEqt)!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"amrp07: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
        
        /* 
       	** 2 eme champ de  amrp07: vl_NumEqt de type XDY_NumEqt = CS_SMALLINT
       	*/
	if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&vl_NumEqt)!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"amrp07: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
        
        
        /* 
       	** 3 eme champ de amrp07 :va_Etat_Eqt type  = CS_INT
       	*/
	if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&vl_Etat_Eqt)!=CS_SUCCEED))	
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"amrp07: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
        
        
        
        /*A 
        **  Transfert des parametres dans les variables locales au
        **  programme
        */

        if (vl_ok && (srv_xferdata(pa_srvproc, CS_GET, SRV_RPCDATA) != CS_SUCCEED))
                vl_ok = CS_FALSE;
        
   
	/*if (!vl_ok || (strlen(vl_buf1)==0))*/
	if (!vl_ok )
        {

        	/*A
        	** Initialise le message d erreur a renvoyer si un probleme est
        	** apparu.
        	*/
        	
       		/* 
       		** mise a zero du vl_buffer de message 
       		*/
    		srv_bzero(&vl_msg, sizeof(CS_SERVERMSG));

        	/* 
        	** indicateur du type de message et de sa gravite       
        	*/
        	
        	vl_msg.msgnumber = RPC_FAIL;
        	vl_msg.severity = 10;
        	
        	/*A 
        	** Initialisation du message pour le client             
        	*/
        	
        	(CS_VOID)sprintf(vl_msg.text,"amrp07 : message non correct.\n");
        	vl_msg.textlen = strlen(vl_msg.text);

        	/* 
        	** Allocation de la structure de contexte               
        	*/

        	if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
        	{
        		XZST_03EcritureTrace(XZSTC_FATAL,"amrp07 : cs_ctx_alloc() a echoue.\n");
        	}
        	else
        	{
              		/* 
              		** Recuperation du nom du serveur               
              		*/
                	(CS_VOID)srv_props(pl_cp, CS_GET, SRV_S_SERVERNAME,
                	(CS_VOID *)vl_msg.svrname, sizeof(vl_msg.svrname),&vl_msg.svrnlen);
        	}
        	/*B 
        	**  Envoi du message d erreur au client                  
        	*/
        	if (srv_sendinfo(pa_srvproc, &vl_msg, (CS_INT)0) != CS_SUCCEED)
        	
                        XZST_03EcritureTrace(XZSTC_FATAL,"amrp07 : srv_sendinfo() a echou≈.\n");
                else 
                	XZST_03EcritureTrace(XZSTC_SGBD,"amrp07 : srv_sendinfo a envoye un message d erreur\n");

		/*B Initialisation du vl_status de la requete par le serveur*/
		
                vl_status |= SRV_DONE_ERROR;

                /*B Envoi du vl_status de la requete indiquant une erreur . */
                if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)
                        
                        XZST_03EcritureTrace(XZSTC_FATAL,"amrp07 : srv_sendstatus() a echou≈.\n");
                else 
                	XZST_03EcritureTrace(XZSTC_SGBD,"amrp07 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);      
        }
        
        
        /*A Envoi au client la confirmation du traitement de la demande  */
        if (vl_ok)
        {
                XZST_03EcritureTrace(XZSTC_SGBD," amrp07 : Arguments recus \n");
        	XZST_03EcritureTrace(XZSTC_SGBD," amrp07 : Valeur recue : \t vl_TypeEqt  \t= %d",vl_TypeEqt);       
     		XZST_03EcritureTrace(XZSTC_SGBD," amrp07 : Valeur recue : \t vl_NumEqt   \t= %d",vl_NumEqt);
     		XZST_03EcritureTrace(XZSTC_SGBD," amrp07 : Valeur recue : \t vl_Etat_Eqt \t= %d",vl_Etat_Eqt);

        	/* Execution des requetes en elles memes : Affichage du vl_msg*/
	
		
		vl_retcode = XZIT07_Modif_Liste_Alarmes();
		if (vl_retcode != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_FATAL,"amrp07 : XZIT07_Modif_Liste_Alarmes() a ≈chou≈ : code = %d",vl_retcode);
		}
		
		vl_retcode = XZIS01_Positionner_Indicateur_Alarme(TRUE);
		if (vl_retcode != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_FATAL,"amrp07 : XZIS01_Positionner_Indicateur_Alarme a ≈chou≈");
			if (srv_sendinfo(pa_srvproc, &vl_msg, (CS_INT)vl_retcode ) != CS_SUCCEED)
        	
                        	XZST_03EcritureTrace(XZSTC_FATAL,"amrp07 : srv_sendinfo() a echou≈.\n");
                	else 
                		XZST_03EcritureTrace(XZSTC_SGBD,"amrp07 : srv_sendinfo a envoye un message d erreur\n");
                	return(vl_retcode);
		}
		vl_retcode = XZIS02_Raf_Etat_Eqt(vl_TypeEqt,vl_NumEqt,vl_Etat_Eqt);
		if (vl_retcode != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_FATAL,"amrp07 : XZIS02_Raf_Etat_Eqt a ≈chou≈");
			if (srv_sendinfo(pa_srvproc, &vl_msg, (CS_INT)vl_retcode ) != CS_SUCCEED)
        	
                        	XZST_03EcritureTrace(XZSTC_FATAL,"amrp07 : srv_sendinfo() a echou≈.\n");
                	else 
                		XZST_03EcritureTrace(XZSTC_SGBD,"amrp07 : srv_sendinfo a envoye un message d erreur\n");
                	return(vl_retcode);
		}
		
		XZST_03EcritureTrace(XZSTC_SGBD," Execution de \n\t\tXZIT07_Modif_Liste_Alarmes()\n\t\tXZIS01_Positionner_Indicateur_Alarme()\n\t\tXZIS02_Raf_Etat_Eqt() a r≈ussi");
		/*A Initialisation du vl_status a 1 indiquant le succes du
        	**  traitement de la requete.
        	*/
        	if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
                {
                	vl_status |= SRV_DONE_ERROR;
                	XZST_03EcritureTrace(XZSTC_FATAL,"amrp07 : srv_sendstatus() a echoue.\n");
                }
        	else
                {
                	XZST_03EcritureTrace(XZSTC_SGBD,"amrp07 : srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
                }

 		/*A 
 		** Envoi du paquet final au client.                             
 		*/
        	if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
                 	XZST_03EcritureTrace(XZSTC_FATAL,"amrp07 : srv_senddone() a echou≈.\n");

 		else XZST_03EcritureTrace(XZSTC_SGBD,"amrp07 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
	}	
	/*A
	**	Scrute les messages du RTserver afin de declancher les CallBacks
	*/	
	asrv_lire_RTSRV_MSG();
	

	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : amrp07 : execution reussie");
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : amrp07 : Fin d execution ");
        return (CS_SUCCEED);

}



