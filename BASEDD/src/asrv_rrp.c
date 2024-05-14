/*E*/
/*Fichier : $Id: asrv_rrp.c,v 1.12 2021/04/23 11:03:14 gesconf Exp $      Release : $Revision: 1.12 $        Date : $Date: 2021/04/23 11:03:14 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE RRP * FICHIER asrv_rrp.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier asrv_rrp.h >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	21 Sep 1994	: Creation
------------------------------------------------------
* mercier	29 Sep 1994	: Modification
*	Rajoue des flags  static char *version = "$Id: asrv_rrp.c,v 1.12 2021/04/23 11:03:14 gesconf Exp $ : asrv_rrp*" ;
* mercier	25 Oct 1994	: Modification       V 1.5
*				 de l appel a XZIT08 et donc
*				  de la rp03
* gaborit	21 dec 1994	: suppression espaces a la fin de nom machine (1.7)
* torregrossa	14 Mar 1995	: modif longueur texte de reveil (1.8)
* Niepceron     27 Nov 1995     : suppression d'un srv_sendinfo v1.9
* Claudel		31 Mai 2022		: correction appel TipcMsgWrite / ajout NULL en fin de liste des parametres JIRA SAE-386
------------------------------------------------------*/

/* fichiers inclus */

#include "asrv_rrp.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: asrv_rrp.c,v 1.12 2021/04/23 11:03:14 gesconf Exp $ : asrv_rrp" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*  RP affaisant appel a xzit08 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE arrp03(SRV_PROC        *pa_srvproc)

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
*	Passe les parametres � XZIT08.
*	On passe soit NomMachine soit NomSite, l un des deux doit etre NULL.
*	
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: asrv_rrp.c,v 1.12 2021/04/23 11:03:14 gesconf Exp $ : asrv_rrp03" ;
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
      	XDY_Evt		vl_Evt_In;		/*
      						** Num�ro de la fiche main courante 
        					*/ 

	XDY_NomMachine 		vl_NomMachine_In;
	XDY_NomSite 		vl_NomSite_In;	
	XDY_TexteReveil  	vl_TexteReveil_In;
	XZITT_Destinataire 	vl_Destinataire ;
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN %s",version);
	
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
        if (vl_numparams != ca_arrp03_nb_param)
        {
        	XZST_03EcritureTrace(XZSTC_WARNING,"arrp03: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_arrp03_nb_param);
        	vl_ok = CS_FALSE;
        }      
        /* 
       	** 1 er champ de  aprp22: vl_Evt_In.numero de type XDY_Entier = CS_INT
       	*/
	if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&(vl_Evt_In.numero))!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"arrp03: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
	
       	/* 
       	** 2 eme champ :vl_Evt_In.cle de type  XDY_Octet : tinyint
       	*/
       	
	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Evt_In.cle))!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"arrp03: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
     	
     	/* 
       	** 3 eme champ de l envoie de la commande :vl_NomMachine_In type CS_CHAR de long XDC_Lon_NomMachine
       	*/
	if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_NomMachine_In,XDC_Lon_NomMachine)!=CS_SUCCEED))	
     	{       	
         	XZST_03EcritureTrace(XZSTC_WARNING,"arrp03: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
       	
	/* 
       	** 4 eme champ de l envoie de la commande :vl_NomSite_In type CS_CHAR de long XDC_Lon_NomSite
       	*/
	if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_NomSite_In,XDC_Lon_NomSite)!=CS_SUCCEED))	
     	{       	
         	XZST_03EcritureTrace(XZSTC_WARNING,"arrp03: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
     	/* 
       	** 5 eme champ de l envoie de la commande :vl_TexteReveil_In type CS_CHAR de long XDC_Lon_Phrase
       	*/
	if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_TexteReveil_In,XDC_Lon_TexteReveil)!=CS_SUCCEED))	
     	{       	
         	XZST_03EcritureTrace(XZSTC_WARNING,"arrp03: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
    	/*A 
        **  Transfert des parametres dans les variables locales au
        **  programme
        */

        if (vl_ok && (srv_xferdata(pa_srvproc, CS_GET, SRV_RPCDATA) != CS_SUCCEED))
                vl_ok = CS_FALSE;

     	
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
        	(CS_VOID)sprintf(vl_msg.text,"arrp03 : message non correct.\n");
        	vl_msg.textlen = strlen(vl_msg.text);

        	/* 
        	** Allocation de la structure de contexte               
        	*/

        	if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
        	{
        		XZST_03EcritureTrace(XZSTC_WARNING,"arrp03 : cs_ctx_alloc() a echoue.\n");
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
        	
                        XZST_03EcritureTrace(XZSTC_WARNING,"arrp03 : srv_sendinfo() a echoue.\n");
                else 
                	XZST_03EcritureTrace(XZSTC_SGBD,"arrp03 : srv_sendinfo a envoye un message d erreur\n");

		/*B Initialisation du vl_status de la requete par le serveur*/
		
                vl_status |= SRV_DONE_ERROR;

                /*B Envoi du vl_status de la requete indiquant une erreur . */
                if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)
                        
                        XZST_03EcritureTrace(XZSTC_WARNING,"arrp03 : srv_sendstatus() a echoue.\n");
                else 
                	XZST_03EcritureTrace(XZSTC_SGBD,"arrp03 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);      
        }
        
        
        /*A Envoi au client la confirmation du traitement de la demande  */
        if (vl_ok)
        {
                
                XZST_03EcritureTrace(XZSTC_DEBUG1,"Valeurs recues : \n Evt.numero = %d \n Evt.cle = %d NomMachine = %s\n NomSite = %s TexteReveil = %s",vl_Evt_In.numero,vl_Evt_In.cle,vl_NomMachine_In,vl_NomSite_In,vl_TexteReveil_In);
                if (!strncmp(vl_TexteReveil_In,"NOUVELLE FMC",strlen("NOUVELLE FMC")))
			XZST_03EcritureTrace(XZSTC_WARNING,"Valeurs recues :  Evt.numero = %d  Evt.cle = %d NomMachine = %s NomSite = %s TexteReveil = %s",vl_Evt_In.numero,vl_Evt_In.cle,vl_NomMachine_In,vl_NomSite_In,vl_TexteReveil_In);
        	/*A 
        	** Execution de la requete en elle meme : 
        	** Pr�vient d'une modification dans la liste des fiches main courante
        	*/
		if ((vl_NomMachine_In == NULL)|| (*vl_NomMachine_In =='\0'))
				strcpy(vl_Destinataire.site,vl_NomSite_In);
		
		else strcpy(vl_Destinataire.machine,strtok(vl_NomMachine_In," "));
			
		vl_retcode =XZIT08_Modif_Liste_FMC(vl_Evt_In,vl_Destinataire,vl_TexteReveil_In);	
		
		if (!strncmp(vl_TexteReveil_In,XDC_REV_FMC_MOD, strlen(XDC_REV_FMC_MOD))) {
			/*on demande a tetdp un eventuel reveil de surcharge*/
			TipcSrvMsgWrite(XDG_ETDP_SURCHARGE,
					TipcMtLookupByNum(XDM_ETDP_SURCHARGE),
					FALSE,
					T_IPC_FT_INT2, -1,
					T_IPC_FT_INT4,0,
					T_IPC_FT_INT4, vl_Evt_In.numero,
					T_IPC_FT_INT2, vl_Evt_In.cle,
					NULL);
			TipcSrvFlush();
		}

		if (vl_retcode != XDC_OK) 
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"arrp03 : XZIT08_Modif_Liste_FMC a echoue");
/* ************************
   Suppression du srv_sendinfo pour ce cas. OM joint par tel me le conseille. PN
   *************************
			if (srv_sendinfo(pa_srvproc, &vl_msg, (CS_INT)vl_retcode ) != CS_SUCCEED)
        	
                        	XZST_03EcritureTrace(XZSTC_WARNING,"arrp03 : srv_sendinfo() a echou�.\n");
                	else 
                		XZST_03EcritureTrace(XZSTC_SGBD,"arrp03 : srv_sendinfo a envoye un message d erreur\n");
   *************************
                */
             	}
		
		
		/*A Initialisation du vl_status a 1 indiquant le succes ou l echec du
        	**  traitement de la requete.
        	*/
        	if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
                {
                	vl_status |= SRV_DONE_ERROR;
                	XZST_03EcritureTrace(XZSTC_WARNING,"arrp03 : srv_sendstatus() a echoue.\n");
                }
        	else
                {
                	XZST_03EcritureTrace(XZSTC_SGBD,"arrp03 : srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
                }

 		/*A 
 		** Envoi du paquet final au client.                             
 		*/
        	if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
                 	XZST_03EcritureTrace(XZSTC_WARNING,"arrp03 : srv_senddone() a echoue.\n");

 		else XZST_03EcritureTrace(XZSTC_SGBD,"arrp03 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
	}
	/*A
	**	Scrute les messages du RTserver afin de declancher les CallBacks
	*/	
	asrv_lire_RTSRV_MSG();
	

	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT %s",version);
        return (CS_SUCCEED);

}


