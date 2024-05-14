/*E*/
/*Fichier : $Id: asrv_prp.c,v 1.8 2021/04/23 11:03:48 gesconf Exp $      Release : $Revision: 1.8 $        Date : $Date: 2021/04/23 11:03:48 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE MRP * FICHIER asrv_prp.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
********************************
*
*	aprp22 RP faisant appel a XZIT03_PA_Pret()
*	A noter que cette RP recoit une date au format Sybase 
*	et la traduit au format Ux pout la passer en argument a xzit03
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	21 Sep 1994	: Creation
------------------------------------------------------
* mercier	29 Sep 1994	: Modification V 1.6
*	Modification du code du return en cas de passage de parametres incorrects
------------------------------------------------------
* mercier	29 Sep 1994	: Modification
*	Rajoue des flags  static char *version = "$Id: asrv_prp.c,v 1.8 2021/04/23 11:03:48 gesconf Exp $ : asrv_mrp*" ;
*
------------------------------------------------------*/

/* fichiers inclus */

#include "asrv_prp.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: asrv_prp.c,v 1.8 2021/04/23 11:03:48 gesconf Exp $ : asrv_prp" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*  aprp22 RP faisant appel a XZIT03_PA_Pret()
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

CS_RETCODE aprp22 (SRV_PROC        *pa_srvproc)

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
m
------------------------------------------------------*/
{	
	static char *version = "$Id: asrv_prp.c,v 1.8 2021/04/23 11:03:48 gesconf Exp $ : asrv_prp22" ;
	
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
        					** Num≈ro de la fiche main courante 
        					*/
		
	XDY_NomMachine  vl_NomMachine_In;	/* 
						** Identifiant du Poste Op≈rateur 
						** qui a demand≈ le calcul du PA	
						*/
	CS_DATETIME	vl_DateTime_In;		/* 
						** Param recu de la base a transcrire 
						** en XDY_Horodate
						*/
	XDY_Horodate	vl_Horodate_In;		/*
						** Horodate de calcul
						*/
						
	XDY_Mot 	vl_NumPA_In;		/*
						** Num≈ro du PA calcul≈ et disponible en BDD
						*/		
	
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : APRP22 : debut d execution ");
	
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
        
        if (vl_numparams != ca_aprp22_nb_param)
        {
        	XZST_03EcritureTrace(XZSTC_FATAL,"aprp22: Le nbre de parametres (%d) passe par le SQL Serveur est pas celui attendu (%d)",vl_numparams,ca_aprp22_nb_param);
        	vl_ok = CS_FALSE;
        }
        
        /* 
       	** 1 er champ de  aprp22: vl_Evt_In.numero de type XDY_Entier = CS_INT
       	*/
	if (vl_ok && (asrv_lire_int(pa_srvproc, vl_param_no++,&(vl_Evt_In.numero))!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"APRP22: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
	
       	/* 
       	** 2 eme champ :vl_Evt_In.cle de type  XDY_Octet : tinyint
       	*/
       	
	if (vl_ok && (asrv_lire_tinyint(pa_srvproc, vl_param_no++,&(vl_Evt_In.cle))!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"APRP22: parametre %d errone",vl_param_no);
		vl_ok = CS_FALSE;
	}
       	
       
	/* 
       	** 3 eme champ de l envoie de la commande :vl_NomMachine_In type CS_CHAR de long XDC_Lon_NomMachine
       	*/
	if (vl_ok && (asrv_lire_char(pa_srvproc, vl_param_no++,vl_NomMachine_In,XDC_Lon_NomMachine)!=CS_SUCCEED))	
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"APRP22: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
       	
       	/* 
       	** 4 eme champ de l envoie de la commande vl_DateTime_In : type  = CS_DATETIME
       	*/
	if (vl_ok && (asrv_lire_date(pa_srvproc, vl_param_no++,&vl_DateTime_In)!=CS_SUCCEED))	
     	{       	
         	XZST_03EcritureTrace(XZSTC_FATAL,"APRP22: parametre %d errone",vl_param_no);
         	vl_ok = CS_FALSE;
       	}
       	
       	/* 
       	** 5 eme champ :vl_NumPA_In de type  XDY_Mot : smallint
       	*/
       	
	if (vl_ok && (asrv_lire_smallint(pa_srvproc, vl_param_no++,&vl_NumPA_In)!=CS_SUCCEED))
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"APRP22: parametre %d errone",vl_NumPA_In);
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
        	
        	(CS_VOID)sprintf(vl_msg.text,"aprp22 : message non correct.\n");
        	vl_msg.textlen = strlen(vl_msg.text);

        	/* 
        	** Allocation de la structure de contexte               
        	*/

        	if (cs_ctx_global(CS_VERSION_100, &pl_cp) != CS_SUCCEED)
        	{
        		XZST_03EcritureTrace(XZSTC_FATAL,"aprp22 : cs_ctx_alloc() a echoue.\n");
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
        	
                        XZST_03EcritureTrace(XZSTC_FATAL,"aprp22 : srv_sendinfo() a echou≈.\n");
                else 
                	XZST_03EcritureTrace(XZSTC_SGBD,"aprp22 : srv_sendinfo a envoye un message d erreur\n");

		/*B Initialisation du vl_status de la requete par le serveur*/
		
                vl_status |= SRV_DONE_ERROR;

                /*B Envoi du vl_status de la requete indiquant une erreur . */
                if (srv_sendstatus(pa_srvproc,(CS_INT) -1) != CS_SUCCEED)
                        
                        XZST_03EcritureTrace(XZSTC_FATAL,"aprp22 : srv_sendstatus() a echou≈.\n");
                else 
                	XZST_03EcritureTrace(XZSTC_SGBD,"aprp22 :  srv_sendstatus() a renvoye vl_status = %d",vl_status);      
        }
        
        
        /*A Envoi au client la confirmation du traitement de la demande  */
        if (vl_ok)
        {
        	vl_retcode=asql_date_Sybase2Ux (vl_DateTime_In,&vl_Horodate_In);
        	if (vl_retcode != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_FATAL,"aprp22 : asql_date_Sybase2Ux() a ≈chou≈");	
			return(vl_retcode);
		}
        	XZST_03EcritureTrace(XZSTC_SGBD," aprp22 : Champs pour la structure \n");
        	XZST_03EcritureTrace(XZSTC_SGBD," aprp22 : Valeur recue : \t  vl_Evt_In.numero \t= %d",vl_Evt_In.numero);       
        	XZST_03EcritureTrace(XZSTC_SGBD," aprp22 : Valeur recue : \t  vl_Evt_In.cle    \t= %d",vl_Evt_In.cle);
        	XZST_03EcritureTrace(XZSTC_SGBD," aprp22 : Valeur recue : \t  vl_NomMachine_In \t= %s",vl_NomMachine_In);
		XZST_03EcritureTrace(XZSTC_SGBD," aprp22 : Valeur recue : \t  vl_Horodate_In   \t= %f",vl_Horodate_In);
		XZST_03EcritureTrace(XZSTC_SGBD," aprp22 : Valeur recue : \t  vl_NumPA_In      \t= %d",vl_NumPA_In);
        
        	/* Execution de la requete en elle meme : Affichage du vl_msg*/
	
		vl_retcode = XZIT03_PA_Pret(vl_Evt_In,vl_NomMachine_In,vl_Horodate_In ,vl_NumPA_In, (XDY_Octet) XDC_SANS_NTFM);
		if (vl_retcode != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_FATAL,"aprp22 : XZIT03_PA_Pret() a ≈chou≈ : code = %d",vl_retcode);
		}
		/*A Initialisation du vl_status a vl_retcode indiquant le succes du
        	**  traitement de la requete.
        	*/
        	if (srv_sendstatus(pa_srvproc,(CS_INT)vl_retcode ) != CS_SUCCEED)
                {
                	vl_status |= SRV_DONE_ERROR;
                	XZST_03EcritureTrace(XZSTC_FATAL,"aprp22 : srv_sendstatus() a echoue.\n");
                }
        	else
                {
                	XZST_03EcritureTrace(XZSTC_SGBD,"aprp22 : srv_sendstatus a reussi a envoyer le vl_status = %d du traitement de la requete\n",vl_status);
                }

 		/*A 
 		** Envoi du paquet final au client.                             
 		*/
        	if (srv_senddone(pa_srvproc, vl_status, CS_TRAN_COMPLETED, 0) != CS_SUCCEED)
                 	XZST_03EcritureTrace(XZSTC_FATAL,"aprp22 : srv_senddone() a echou≈.\n");

 		else XZST_03EcritureTrace(XZSTC_SGBD,"aprp22 : srv_senddone envoie CS_TRAN_COMPLETED pour completer la transaction\n");
	}
	/*A
	**	Scrute les messages du RTserver afin de declancher les CallBacks
	*/	
	asrv_lire_RTSRV_MSG();
	
	if (!vl_ok) 
	{
		vl_retcode=XDC_NOK;
		XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : aprp22 a ≈chou≈ : sortie avec %d");		
	}
	else 
	{
		vl_retcode=XDC_OK;
		
	}
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : aprp22 : execution reussie");
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : aprp22 : Fin d execution ");
        return (vl_retcode);

}

